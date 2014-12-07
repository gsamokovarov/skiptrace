require "mkmf"

case RUBY_VERSION
when /2\.0\.*/
  $CFLAGS   << " -D RUBY_20"
  $INCFLAGS << " -I./ruby_20/"
when /2\.1\.*/
  $CFLAGS   << " -D RUBY_21"
  $INCFLAGS << " -I./ruby_21/"
end

create_makefile("exception_bindings/exception_ext")
