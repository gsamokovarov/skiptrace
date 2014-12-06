$LOAD_PATH << File.expand_path('../lib', __FILE__)

require 'exception_bindings/version'

Gem::Specification.new do |spec|
  spec.name          = "exception_bindings"
  spec.version       = ExceptionBindings::VERSION
  spec.authors       = ["Genadi Samokovarov"]
  spec.email         = ["gsamokovarov@gmail.com"]
  spec.extensions    = ["ext/exception_bindings/extconf.rb"]
  spec.summary       = "Get the bindings of the traces of an exception"
  spec.homepage      = "https://github.com/gsamokovarov/exception_bindings"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.7"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rake-compiler"
end
