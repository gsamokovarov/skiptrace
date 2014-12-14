case RUBY_ENGINE
when 'rbx'
  require 'bindex/rubinius'
when 'ruby'
  require 'bindex/cruby'
end

require "bindex/version"
