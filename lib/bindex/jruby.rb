require 'bindex/jruby_internals'

java_import com.gsamokovarov.bindex.SetExceptionBindingsEventHook

JRuby.runtime.add_event_hook(SetExceptionBindingsEventHook.new)

::Exception.class_eval do
  def bindings
    @bindings || []
  end
end
