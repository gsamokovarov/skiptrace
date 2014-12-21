require 'erb'
require 'bundler/gem_tasks'
require 'rake/testtask'

def jruby_9k?
  JRUBY_VERSION.start_with?('9')
end

def root
  Pathname.new File.expand_path('..', __FILE__)
end

def render_template(template_path, target = nil)
  target ||= root.join(template_path).to_s.chomp('.erb')
  template = ERB.new File.read(root.join(template_path))
  File.write target, template.result(binding)
end

Rake::TestTask.new do |t|
  t.libs << 'test'
  t.test_files = FileList['test/*_test.rb']
  t.verbose = true
end

case RUBY_ENGINE
when 'ruby'
  require 'rake/extensiontask'

  Rake::ExtensionTask.new('bindex') do |ext|
    ext.name = 'cruby'
    ext.lib_dir = 'lib/bindex'
  end

  task default: [:clean, :compile, :test]
when 'jruby'
  require 'rake/javaextensiontask'

  Rake::JavaExtensionTask.new('bindex') do |ext|
    ext.name = jruby_9k? ? 'jruby_internals_9k' : 'jruby_internals'
    ext.lib_dir = 'lib/bindex'
  end

  render_template 'ext/bindex/com/gsamokovarov/bindex/BindingBuilder.java.erb'

  task default: [:clean, :compile, :test]
else
  task default: [:test]
end
