require "bundler/gem_tasks"
require "rake/extensiontask"
require "rake/testtask"

Rake::ExtensionTask.new("exception_bindings") do |ext|
  ext.name = 'current_bindings'
  ext.lib_dir = "lib/exception_bindings"
end

Rake::TestTask.new do |t|
  t.libs << "test"
  t.test_files = FileList['test/*_test.rb']
  t.verbose = true
end

task default: [:compile, :test]
