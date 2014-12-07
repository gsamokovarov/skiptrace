#include "bindex.h"

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

VALUE
current_bindings(void)
{
  return rb_debug_inspector_open(current_bindings_callback, NULL);
}

