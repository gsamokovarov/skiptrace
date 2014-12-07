#include "bindex.h"

extern VALUE rb_check_backtrace(VALUE);

static VALUE
bx_exc_set_backtrace(VALUE self, VALUE bt)
{
  rb_iv_set(self, "bindings", current_bindings());

  return rb_iv_set(self, "bt", rb_check_backtrace(bt));
}

static VALUE
bx_exc_bindings(VALUE self, VALUE bt)
{
  VALUE bindings;

  bindings = rb_iv_get(self, "bindings");
  if (NIL_P(bindings)) {
    bindings = rb_ary_new();
  }

  return bindings;
}

void
Init_exception_ext(void)
{
  rb_define_method(rb_eException, "set_backtrace", bx_exc_set_backtrace, 1);
  rb_define_method(rb_eException, "bindings", bx_exc_bindings, 0);
}
