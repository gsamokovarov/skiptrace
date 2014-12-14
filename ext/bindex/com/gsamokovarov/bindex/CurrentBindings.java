package com.gsamokovarov.bindex;

import org.jruby.runtime.EventHook;
import org.jruby.runtime.RubyEvent;
import org.jruby.runtime.ThreadContext;
import org.jruby.runtime.DynamicScope;
import org.jruby.runtime.Binding;
import org.jruby.runtime.Frame;
import org.jruby.runtime.DynamicScope;
import org.jruby.runtime.builtin.IRubyObject;
import org.jruby.runtime.backtrace.BacktraceElement;
import org.jruby.RubyBinding;
import org.jruby.RubyArray;
import org.jruby.RubyClass;
import org.jruby.Ruby;

public class CurrentBindings {
  private final Ruby runtime;

  private Frame[] frameStack;
  private int frameIndex;

  private DynamicScope[] scopeStack;
  private int scopeIndex;

  private BacktraceElement[] backtrace;
  private int backtraceIndex;

  public static RubyArray get(ThreadContext context) {
    return new CurrentBindings(context).get();
  }

  private CurrentBindings(ThreadContext context) {
    ThreadContextInternals contextInternals = new ThreadContextInternals(context);

    this.frameStack = contextInternals.getFrameStack();
    this.frameIndex = contextInternals.getFrameIndex();

    this.scopeStack = contextInternals.getScopeStack();
    this.scopeIndex = contextInternals.getScopeIndex();

    this.backtrace = contextInternals.getBacktrace();
    this.backtraceIndex = contextInternals.getBacktraceIndex();

    this.runtime = context.getRuntime();
  }

  private RubyArray get() {
    RubyArray bindings = RubyArray.newArray(runtime);

    Frame frame;
    DynamicScope scope;
    BacktraceElement element;
    Binding binding;

    while (frameIndex >= 0 && scopeIndex >= 0 && backtraceIndex >= 0) {
      frame = frameStack[frameIndex--];
      scope = scopeStack[scopeIndex--];
      element = backtrace[backtraceIndex--];

      binding = new Binding(frame, scope.getStaticScope().getModule(), scope, element.clone());

      bindings.append(((IRubyObject) RubyBinding.newBinding(runtime, binding)));
    }

    return bindings;
  }
}
