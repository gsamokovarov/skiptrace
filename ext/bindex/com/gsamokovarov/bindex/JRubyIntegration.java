package com.gsamokovarov.bindex;

import org.jruby.Ruby;
import org.jruby.RubyArray;
import org.jruby.RubyModule;
import org.jruby.RubyClass;
import org.jruby.runtime.ThreadContext;
import org.jruby.runtime.builtin.IRubyObject;
import org.jruby.runtime.builtin.InstanceVariables;
import org.jruby.anno.JRubyMethod;

public class JRubyIntegration {
    public static void setup(Ruby runtime) {
        RubyModule bindex = runtime.defineModule("Bindex");
        bindex.getSingletonClass().defineAnnotatedMethods(BindexMethods.class);
    }

    private static class BindexMethods {
        @JRubyMethod(name = "current_bindings")
        public static IRubyObject currentBindings(ThreadContext context, IRubyObject self) {
            return RubyBindingsCollector.collectCurrentFor(context);
        }
    }
}
