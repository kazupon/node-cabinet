/*
 * kernel (system call) wapper
 * Copyright (C) 2012 kazuya kawaguchi. See Copyright Notice in cabinet.h
 */

#ifndef __KERNEL_WRAP_H__
#define __KERNEL_WRAP_H__

#include <node.h>

using namespace v8;


namespace kernel {
  void load(Handle<Object> target);
}


#endif /* __KERNEL_WRAP_H__ */

