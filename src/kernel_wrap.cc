/*
 * kernel (system call) wapper
 * Copyright (C) 2012 kazuya kawaguchi. See Copyright Notice in cabinet.h
 */

#define BUILDING_NODE_EXTENSION

#include "kernel_wrap.h"
#include <assert.h>
#include "debug.h"
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <node_buffer.h>

using namespace node;

namespace kernel {


struct mmap_req_t {
  size_t size;
  int32_t protection;
  int32_t flags;
  int32_t fd;
  off_t offset;
  char *map;
  Persistent<Function> cb;
};


void unmap(char *data, void *hint) {
  TRACE("unmap: data=%p, hint=%p\n", data, hint);
  munmap(data, (size_t)hint);
}

void on_work(uv_work_t *work_req) {
  TRACE("argument: work_req=%p\n", work_req);

  mmap_req_t *req = reinterpret_cast<mmap_req_t *>(work_req->data);
  assert(req != NULL);

  req->map = (char *)mmap(NULL, req->size, req->protection, req->flags, req->fd, req->offset);
}

void on_work_done(uv_work_t *work_req) {
  HandleScope scope;
  TRACE("argument: work_req=%p\n", work_req);

  mmap_req_t *req = static_cast<mmap_req_t *>(work_req->data);
  assert(req != NULL);

  // init callback arguments.
  int32_t argc = 0;
  Local<Value> argv[2] = { 
    Local<Value>::New(Null()),
    Local<Value>::New(Null()),
  };

  // set error to callback arguments.
  if (req->map == MAP_FAILED) {
    const char *name = "mmap error";
    Local<String> message = String::NewSymbol(name);
    Local<Value> err = Exception::Error(message);
    argv[argc] = err;
  }
  argc++;

  Buffer *buffer = Buffer::New(req->map, req->size, unmap, (void *)req->size);
  Local<Value> buf = Local<Value>::New(buffer->handle_);
  argv[argc++] = buf;

  // execute callback
  if (!req->cb.IsEmpty()) {
    TryCatch try_catch;
    MakeCallback(Context::GetCurrent()->Global(), req->cb, argc, argv);
    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }
  } 

  // releases
  req->cb.Dispose();
  req->map = NULL;
  free(req);
  work_req->data = NULL;
  free(work_req);
}


Handle<Value> map(const Arguments &args) {
  HandleScope scope;
  TRACE("map\n");

	if (args.Length() <= 3) {
		return ThrowException(Exception::Error(String::New("Bad argument")));
	}

	const size_t size = args[0]->ToInteger()->Value();
	const int32_t protection = args[1]->ToInteger()->Value();
	const int32_t flags = args[2]->ToInteger()->Value();
	const int32_t fd = args[3]->ToInteger()->Value();

  mmap_req_t *req = NULL;
  off_t offset = 0;
  if (args.Length() == 5) {
    if (args[4]->IsFunction()) {
      req = reinterpret_cast<mmap_req_t *>(malloc(sizeof(mmap_req_t)));
      req->size = size;
      req->protection = protection;
      req->flags = flags;
      req->fd = fd;
      req->offset = offset;
      req->map = NULL;
      req->cb = Persistent<Function>::New(Handle<Function>::Cast(args[4]));
    } else {
      offset = args[4]->ToInteger()->Value();
    }
  } else if (args.Length() == 6) {
    req = reinterpret_cast<mmap_req_t *>(malloc(sizeof(mmap_req_t)));
    req->size = size;
    req->protection = protection;
    req->flags = flags;
    req->fd = fd;
    req->offset = args[4]->ToInteger()->Value();
    req->map = NULL;
    req->cb = Persistent<Function>::New(Handle<Function>::Cast(args[5]));
  }


  if (req == NULL) { // sync
    char *data = (char *)mmap(NULL, size, protection, flags, fd, offset);
    if (data == MAP_FAILED) {
      return ThrowException(ErrnoException(errno, "mmap", ""));
    }

    Buffer *buffer = Buffer::New(data, size, unmap, (void *)size);
    return buffer->handle_;
  } else { // async
    uv_work_t *uv_req = reinterpret_cast<uv_work_t*>(malloc(sizeof(uv_work_t)));
    assert(uv_req != NULL);
    uv_req->data = req;

    int32_t ret = uv_queue_work(uv_default_loop(), uv_req, on_work, on_work_done);
    TRACE("uv_queue_work: ret=%d\n", ret);
    assert(ret == 0);

    return scope.Close(args.This());
  }
}

void load(Handle<Object> target) {
  HandleScope scope;
  TRACE("load kernel module\n");

  // kernel
  Local<Object> kernel = Object::New();

  // mmap
  Local<Object> mmap = Object::New();
  kernel->Set(String::NewSymbol("mmap"), mmap);
  NODE_DEFINE_CONSTANT(mmap, PROT_READ);
  NODE_DEFINE_CONSTANT(mmap, PROT_NONE);
  NODE_DEFINE_CONSTANT(mmap, PROT_READ);
  NODE_DEFINE_CONSTANT(mmap, PROT_WRITE);
  NODE_DEFINE_CONSTANT(mmap, PROT_EXEC);
  NODE_DEFINE_CONSTANT(mmap, MAP_SHARED);
  NODE_DEFINE_CONSTANT(mmap, MAP_PRIVATE);
  NODE_DEFINE_CONSTANT(mmap, MAP_NORESERVE);
  NODE_DEFINE_CONSTANT(mmap, MAP_FIXED);
  // TODO: conditional definition
  //NODE_DEFINE_CONSTANT(mmap, MAP_ANONYMOUS);
  //NODE_DEFINE_CONSTANT(mmap, MAP_GROWSDOWN);
  //NODE_DEFINE_CONSTANT(mmap, MAP_32BIT);
  //NODE_DEFINE_CONSTANT(mmap, MAP_HUGETLB);
  //NODE_DEFINE_CONSTANT(mmap, MAP_LOCKED);
  //NODE_DEFINE_CONSTANT(mmap, MAP_NONBLOCK);
  //NODE_DEFINE_CONSTANT(mmap, MAP_POPULATE);
  //NODE_DEFINE_CONSTANT(mmap, MAP_STACK);
  //NODE_DEFINE_CONSTANT(mmap, MAP_UNINITIALIZED);
  mmap->Set(
    String::NewSymbol("PAGESIZE"), Integer::New(sysconf(_SC_PAGESIZE)),
    static_cast<PropertyAttribute>(ReadOnly | DontDelete)
  );
	mmap->Set(String::NewSymbol("map"), FunctionTemplate::New(map)->GetFunction());

  target->Set(String::NewSymbol("kernel"), kernel);
}


} // endo of namespace kernel
