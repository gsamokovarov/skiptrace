/**********************************************************************

 Copyright (C) 1993-2013 Yukihiro Matsumoto. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

**********************************************************************/
#ifndef _RUBY_193_H
#define _RUBY_193_H

#include "ruby_193/gc.h"
#include "ruby_193/internal.h"
#include "ruby_193/vm_core.h"

static void
binding_free(void *ptr)
{
  rb_binding_t *bind;
  RUBY_FREE_ENTER("binding");
  if (ptr) {
    bind = ptr;
    ruby_xfree(ptr);
  }
  RUBY_FREE_LEAVE("binding");
}

static void
binding_mark(void *ptr)
{
  rb_binding_t *bind;
  RUBY_MARK_ENTER("binding");
  if (ptr) {
    bind = ptr;
    RUBY_MARK_UNLESS_NULL(bind->env);
    RUBY_MARK_UNLESS_NULL(bind->filename);
  }
  RUBY_MARK_LEAVE("binding");
}

static size_t
binding_memsize(const void *ptr)
{
  return ptr ? sizeof(rb_binding_t) : 0;
}

static const rb_data_type_t binding_data_type = {
  "binding",
  {binding_mark, binding_free, binding_memsize,},
};

static VALUE
binding_alloc(VALUE klass)
{
  VALUE obj;
  rb_binding_t *bind;
  obj = TypedData_Make_Struct(klass, rb_binding_t, &binding_data_type, bind);
  return obj;
}

VALUE
rb_check_backtrace(VALUE bt)
{
  long i;
  static const char err[] = "backtrace must be Array of String";

  if (!NIL_P(bt)) {
    int t = TYPE(bt);

    if (t == T_STRING) return rb_ary_new3(1, bt);
    if (t != T_ARRAY) {
      rb_raise(rb_eTypeError, err);
    }
    for (i=0;i<RARRAY_LEN(bt);i++) {
      if (TYPE(RARRAY_PTR(bt)[i]) != T_STRING) {
        rb_raise(rb_eTypeError, err);
      }
    }
  }
  return bt;
}

#endif
