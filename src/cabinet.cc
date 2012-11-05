/*
 * main
 * Copyright (C) 2012 kazuya kawaguchi. See Copyright Notice in cabinet.h
 */

#include <node.h>
#include "cabinet.h"
#include "kernel_wrap.h"

using namespace v8;
using namespace kernel;


void init(Handle<Object> target) {
  kernel::load(target);
}

NODE_MODULE(cabinet, init);
