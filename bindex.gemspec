$LOAD_PATH << File.expand_path('../lib', __FILE__)

require 'bindex/version'

Gem::Specification.new do |spec|
  spec.name          = "bindex"
  spec.version       = Bindex::VERSION
  spec.authors       = ["Genadi Samokovarov"]
  spec.email         = ["gsamokovarov@gmail.com"]
  spec.extensions    = ["ext/bindex/extconf.rb"]
  spec.summary       = "Bindings for your Ruby exceptions"
  spec.homepage      = "https://github.com/gsamokovarov/bindex"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.7"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "minitest", "~> 5.4"
end
