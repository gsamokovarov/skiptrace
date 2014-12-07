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
#ifndef _RUBY_20_H
#define _RUBY_20_H

#include "ruby_20/internal.h"
#include "ruby_20/method.h"
#include "ruby_20/vm_core.h"

typedef struct rb_backtrace_location_struct {
  enum LOCATION_TYPE {
    LOCATION_TYPE_ISEQ = 1,
    LOCATION_TYPE_ISEQ_CALCED,
    LOCATION_TYPE_CFUNC,
    LOCATION_TYPE_IFUNC
  } type;

  union {
    struct {
      const rb_iseq_t *iseq;
      union {
        const VALUE *pc;
        int lineno;
      } lineno;
    } iseq;
    struct {
      ID mid;
      struct rb_backtrace_location_struct *prev_loc;
    } cfunc;
  } body;
} rb_backtrace_location_t;

typedef struct rb_backtrace_struct {
    rb_backtrace_location_t *backtrace;
    rb_backtrace_location_t *backtrace_base;
    int backtrace_size;
    VALUE strary;
} rb_backtrace_t;

static void
location_mark_entry(rb_backtrace_location_t *fi)
{
  switch (fi->type) {
    case LOCATION_TYPE_ISEQ:
    case LOCATION_TYPE_ISEQ_CALCED:
      rb_gc_mark(fi->body.iseq.iseq->self);
      break;
    case LOCATION_TYPE_CFUNC:
    case LOCATION_TYPE_IFUNC:
    default:
      break;
  }
}

static void
backtrace_mark(void *ptr)
{
  if (ptr) {
    rb_backtrace_t *bt = (rb_backtrace_t *)ptr;
    size_t i, s = bt->backtrace_size;

    for (i=0; i<s; i++) {
      location_mark_entry(&bt->backtrace[i]);
      rb_gc_mark(bt->strary);
    }
  }
}

static void
backtrace_free(void *ptr)
{
  if (ptr) {
    rb_backtrace_t *bt = (rb_backtrace_t *)ptr;
    if (bt->backtrace) ruby_xfree(bt->backtrace_base);
    ruby_xfree(bt);
  }
}

static size_t
backtrace_memsize(const void *ptr)
{
  rb_backtrace_t *bt = (rb_backtrace_t *)ptr;
  return sizeof(rb_backtrace_t) + sizeof(rb_backtrace_location_t) * bt->backtrace_size;
}

static const rb_data_type_t backtrace_data_type = {
  "backtrace",
  {backtrace_mark, backtrace_free, backtrace_memsize,},
};

int
rb_backtrace_p(VALUE obj)
{
  return rb_typeddata_is_kind_of(obj, &backtrace_data_type);
}

VALUE
rb_check_backtrace(VALUE bt)
{
  long i;
  static const char err[] = "backtrace must be Array of String";

  if (!NIL_P(bt)) {
    if (RB_TYPE_P(bt, T_STRING)) return rb_ary_new3(1, bt);
    if (rb_backtrace_p(bt)) return bt;
    if (!RB_TYPE_P(bt, T_ARRAY)) {
      rb_raise(rb_eTypeError, err);
    }
    for (i=0;i<RARRAY_LEN(bt);i++) {
      if (!RB_TYPE_P(RARRAY_PTR(bt)[i], T_STRING)) {
        rb_raise(rb_eTypeError, err);
      }
    }
  }
  return bt;
}

#endif
