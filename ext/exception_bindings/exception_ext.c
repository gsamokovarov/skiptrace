#include "ruby.h"
#include "ruby/debug.h"

VALUE exc_set_backtrace;

static void
init_exc_set_backtrace(void)
{
  ID id_instance_method = rb_intern("instance_method");
  VALUE sym_set_backtrace = ID2SYM(rb_intern("set_backtrace"));

  exc_set_backtrace = rb_funcall(rb_eException, id_instance_method, 1, sym_set_backtrace);
}

static VALUE
bind_and_call_exc_set_backtrace(VALUE exc, VALUE backtrace)
{
  VALUE bound_method = rb_funcall(exc_set_backtrace, rb_intern("bind"), 1, exc);

  return rb_funcall(bound_method, rb_intern("call"), 1, backtrace);
}

static VALUE
extract_bindings_callback(const rb_debug_inspector_t *context, void *data)
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
extract_bindings(void)
{
  return rb_debug_inspector_open(extract_bindings_callback, NULL);
}

static VALUE
eb_set_backtrace(VALUE self, VALUE backtrace)
{
  rb_iv_set(self, "bindings", extract_bindings());

  return bind_and_call_exc_set_backtrace(self, backtrace);
}

static VALUE
eb_bindings(VALUE self)
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
  init_exc_set_backtrace();

  rb_define_method(rb_eException, "set_backtrace", eb_set_backtrace, 1);
  rb_define_method(rb_eException, "bindings", eb_bindings, 0);
}
