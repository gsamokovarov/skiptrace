#include "bindex.h"

VALUE bx_mBindex;

static VALUE
bx_current_bindings(VALUE self)
{
  return current_bindings();
}

static VALUE
bx_exc_set_backtrace(VALUE self, VALUE bt)
{
  /* rb_check_backtrace can raise an exception, if the input arguments are not
   * to its likings. Set the bindings afterwards, so we don't waste when not
   * needed. */
  VALUE backtrace = rb_iv_set(self, "bt", rb_check_backtrace(bt));

  rb_iv_set(self, "bindings", current_bindings());

  return backtrace;
}

static VALUE
bx_exc_bindings(VALUE self)
{
  VALUE bindings;

  bindings = rb_iv_get(self, "bindings");
  if (NIL_P(bindings)) {
    bindings = rb_ary_new();
  }

  return bindings;
}

void
Init_cruby(void)
{
  bx_mBindex = rb_define_module("Bindex");

  rb_define_singleton_method(bx_mBindex, "current_bindings", bx_current_bindings, 0);

  rb_define_method(rb_eException, "set_backtrace", bx_exc_set_backtrace, 1);
  rb_define_method(rb_eException, "bindings", bx_exc_bindings, 0);
}
