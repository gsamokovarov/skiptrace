#include "ruby.h"
#include "ruby/debug.h"

#ifdef RUBY_20
#include "ruby_20.h"
#elif RUBY_21
#include "ruby_21.h"
#endif

static VALUE
current_bindings_callback(const rb_debug_inspector_t *context, void *data)
{
  VALUE locations = rb_debug_inspector_backtrace_locations(context);
  VALUE binding, bindings = rb_ary_new();
  long i, length = RARRAY_LEN(locations);

  for (i = 0; i < length; i++) {
    binding = rb_debug_inspector_frame_binding_get(context, i);

    if (!NIL_P(binding)) {
      rb_ary_push(bindings, binding);
    }
  }

  return bindings;
}

static VALUE
current_bindings(void)
{
  return rb_debug_inspector_open(current_bindings_callback, NULL);
}

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
