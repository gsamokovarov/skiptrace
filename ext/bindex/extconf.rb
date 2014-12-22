case RUBY_ENGINE
when "ruby"
  require "mkmf"

  case "#{RUBY_VERSION}#{RUBY_PATCHLEVEL}"
  when /1\.9\.3/
    $CFLAGS   << " -D RUBY_193"
    $INCFLAGS << " -I./ruby_193/"
  when /2\.0\.*/
    $CFLAGS   << " -D RUBY_20"
    $INCFLAGS << " -I./ruby_20/"
  when "2.1.0-1"
    $CFLAGS   << " -D RUBY_21PREVIEW"
    $INCFLAGS << " -I./ruby_21PREVIEW/"
  when /2\.1\.*/
    $CFLAGS   << " -D RUBY_21"
    $INCFLAGS << " -I./ruby_21/"
  end

  $CFLAGS << " -g3 -O0" if ENV["DEBUG"]

  create_makefile("bindex/cruby")
else
  IO.write(File.expand_path("../Makefile", __FILE__), <<-END)
    all install static install-so install-rb: Makefile
    .PHONY: all install static install-so install-rb
    .PHONY: clean clean-so clean-static clean-rb
  END
end
