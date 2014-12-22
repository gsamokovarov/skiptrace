#include "bindex.h"

#ifdef RUBY_193

#include "ruby_193.h"

static rb_thread_t *current_thread;

#define GetCoreDataFromValueAsExpr(obj, type, ptr) \
  ((ptr) = (type*)DATA_PTR(obj))

#define ruby_current_thread \
  GetCoreDataFromValueAsExpr(rb_thread_current(), rb_thread_t, current_thread)

/*
 * Create a binding from a thread and control frame.
 *
 * Based on rb_binding new, defined in proc.c. A major difference from
 * rb_binding_new is that it doesn't raises an exception, when we try to create
 * Binding on top of a Fiber.
 *
 * The reason for this is that binding_new is to be used in exception handling
 * code. If we raise, it may lead to an infinite recursion.
 *
 * Also, we'll assume a bit more hostile environment and check if cfp->iseq is
 * NULL.
 *
 * Ruby nil is returned when the control frame pointer isn't suitable.
 */

static VALUE
binding_new(rb_thread_t *th, rb_control_frame_t *cfp)
{
  VALUE bindval;
  rb_binding_t *bind;

  if (cfp == NULL || cfp->iseq == NULL) {
    return Qnil;
  }

  bindval = binding_alloc(rb_cBinding);

  GetBindingPtr(bindval, bind);
  bind->env = rb_vm_make_env_object(th, cfp);
  bind->filename = cfp->iseq->filename;
  bind->line_no = rb_vm_get_sourceline(cfp);
  return bindval;
}

VALUE
current_bindings(void)
{
  rb_thread_t *th = GET_THREAD();
  rb_control_frame_t *cfp = th->cfp;
  rb_control_frame_t *cfp_limit = RUBY_VM_END_CONTROL_FRAME(th);
  VALUE binding, bindings = rb_ary_new();

  while (cfp_limit > cfp) {
    cfp = RUBY_VM_PREVIOUS_CONTROL_FRAME(cfp);
    binding = binding_new(th, cfp);

    if (!NIL_P(binding)) {
      rb_ary_push(bindings, binding);
    }
  }

  return bindings;
}

#else

#include <ruby/debug.h>

#ifdef RUBY_20
#include "ruby_20.h"
#elif RUBY_21
#include "ruby_21.h"
#elif RUBY_21PREVIEW
#include "ruby_21preview.h"
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

#endif
