if JRUBY_VERSION.start_with?('9')
  require 'bindex/jruby_internals_9k'
else
  require 'bindex/jruby_internals'
end

java_import com.gsamokovarov.bindex.JRubyIntegration
java_import com.gsamokovarov.bindex.SetExceptionBindingsEventHook

JRubyIntegration.setup(JRuby.runtime)

begin
  previous_verbose, $VERBOSE = $VERBOSE, nil
  JRuby.runtime.add_event_hook(SetExceptionBindingsEventHook.new)
ensure
  $VERBOSE = previous_verbose
end
