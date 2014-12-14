require 'bundler/gem_tasks'
require 'rake/extensiontask'
require 'rake/testtask'

Rake::TestTask.new do |t|
  t.libs << 'test'
  t.test_files = FileList['test/*_test.rb']
  t.verbose = true
end

case RUBY_ENGINE
when 'ruby'
  Rake::ExtensionTask.new('bindex') do |ext|
    ext.name = 'cruby'
    ext.lib_dir = 'lib/bindex'
  end

  task default: [:clean, :compile, :test]
else
  task default: [:test]
end
