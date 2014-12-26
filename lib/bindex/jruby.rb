if JRUBY_VERSION.start_with?('9')
  require 'bindex/jruby_internals_9k'
else
  require 'bindex/jruby_internals'
end

java_import com.gsamokovarov.bindex.JRubyIntegration

JRubyIntegration.setup(JRuby.runtime)
