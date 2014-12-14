case RUBY_ENGINE
when 'rbx'
  require 'bindex/rubinius'
when 'ruby'
  require 'bindex/exception_ext'
end

require "bindex/version"
