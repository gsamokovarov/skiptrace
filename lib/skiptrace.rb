case RUBY_ENGINE
when 'rbx'
  require 'skiptrace/rubinius'
when 'jruby'
  require 'skiptrace/jruby'
when 'ruby'
  require 'skiptrace/cruby'
end

require 'skiptrace/location'
require 'skiptrace/binding_locations'
require 'skiptrace/binding_ext'
require 'skiptrace/exception_ext'
require 'skiptrace/version'
