require 'bindex/jruby_internals'

java_import com.gsamokovarov.bindex.SetExceptionBindingsEventHook

begin
  previous_verbose, $VERBOSE = $VERBOSE, nil
  JRuby.runtime.add_event_hook(SetExceptionBindingsEventHook.new)
ensure
  $VERBOSE = previous_verbose
end

::Exception.class_eval do
  def bindings
    @bindings || []
  end
end
