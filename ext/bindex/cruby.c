#include <ruby.h>

#ifdef RUBY_193

#include "ruby_193.h"

/* Ruby 1.9.3 is a bit special and we have to backport the current thread to
 * make sure the GET_THREAD works. */
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

static inline VALUE
binding_new(rb_thread_t *th, rb_control_frame_t *cfp)
{
  VALUE bindval;
  rb_binding_t *bind;

  /* Note that cfp->iseq won't be NULL because of RUBY_VM_NORMAL_ISEQ_P guard.
   * However, if this goes away, cfp->iseq can be NULL and have to guard for
   * that. */

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
  VALUE bindings = rb_ary_new();

  while (RUBY_VM_VALID_CONTROL_FRAME_P(cfp, cfp_limit)) {
    cfp = RUBY_VM_PREVIOUS_CONTROL_FRAME(cfp);

    if (RUBY_VM_NORMAL_ISEQ_P(cfp->iseq)) {
      rb_ary_push(bindings, binding_new(th, cfp));
    }
  }

  return bindings;
}

void
set_exception_bindings_callback(rb_event_flag_t event, VALUE proc, VALUE self, ID id, VALUE klass)
{
  VALUE exception = GET_THREAD()->errinfo;
  VALUE bindings = current_bindings();
  const char *srcfile = rb_sourcefile();

  /* We need to create an additional binding for where the exception actually
   * occurred, as the RAISE event isn't raised out at that point. */
  if (srcfile && !NIL_P(self)) {
    rb_ary_unshift(bindings, rb_binding_new());
  }

  rb_iv_set(exception, "bindings", bindings);
}

void
set_exception_bindings_on_raise(void)
{
  rb_add_event_hook(set_exception_bindings_callback, RUBY_EVENT_RAISE, Qnil);
}

#else

#include <ruby/debug.h>

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

static void
set_exception_bindings_callback(VALUE tpval, void *data)
{
  rb_trace_arg_t *trace_arg = rb_tracearg_from_tracepoint(tpval);
  VALUE exception = rb_tracearg_raised_exception(trace_arg);

  rb_iv_set(exception, "bindings", current_bindings());
}

void
set_exception_bindings_on_raise(void)
{
  VALUE tpval = rb_tracepoint_new(0, RUBY_EVENT_RAISE, set_exception_bindings_callback, 0);
  rb_tracepoint_enable(tpval);
}

#endif

VALUE bx_mBindex;

static VALUE
bx_current_bindings(VALUE self)
{
  return current_bindings();
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
  rb_define_method(rb_eException, "bindings", bx_exc_bindings, 0);

  set_exception_bindings_on_raise();
}
