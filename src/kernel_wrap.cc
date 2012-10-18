/*
 * kernel (system call) wapper
 * Copyright (C) 2012 kazuya kawaguchi. See Copyright Notice in kvs.h
 */

#define BUILDING_NODE_EXTENSION

#include "kernel_wrap.h"
#include <assert.h>
#include "debug.h"
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>


using namespace node;
using namespace v8;


void kernel::load(Handle<Object> target) {
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

  target->Set(String::NewSymbol("kernel"), kernel);
}
