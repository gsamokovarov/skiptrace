#include "ruby.h"
#include "ruby/debug.h"

VALUE eb_cExceptionBindings;

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
eb_current_bindings(void)
{
  return rb_debug_inspector_open(current_bindings_callback, NULL);
}

void
Init_current_bindings(void)
{
  eb_cExceptionBindings = rb_define_module("ExceptionBindings");

  rb_define_singleton_method(eb_cExceptionBindings, "current_bindings", eb_current_bindings, 0);
}
