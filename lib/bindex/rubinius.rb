module Bindex
  class Rubinius
    # Gets the current bindings for all available Ruby frames.
    #
    # Filters the internal Rubinius frames.
    def self.current_bindings
      locations = ::Rubinius::VM.backtrace(1, true)

      InternalLocationFilter.new(locations).filter.map do |location|
        Binding.setup(
          location.variables,
          location.variables.method,
          location.constant_scope,
          location.variables.self,
          location
        )
      end
    end

    # Filters internal Rubinius locations.
    #
    # The reason for this is that some methods, like ::Kernel.raise, are
    # implemented in Ruby for Rubinius. Therefore, we will get a binding for
    # them as well.
    #
    # To align the current_bindings implementation with the CRuby one, where
    # the first binding will usually be binding of caller, we can use this
    # object to clean bindings for internal Rubinius methods.
    class InternalLocationFilter
      def initialize(locations)
        @locations = locations
      end

      def filter
        @locations.reject do |location|
          location.file.start_with?('kernel/') || location.file == __FILE__
        end
      end
    end
  end
end

::Exception.class_eval do
  def bindings
    @bindings || []
  end
end

::Rubinius.singleton_class.class_eval do
  raise_exception = instance_method(:raise_exception)

  define_method(:raise_exception) do |exc|
    exc.instance_variable_set(:@bindings, Bindex::Rubinius.current_bindings)
    raise_exception.bind(self).call(exc)
  end
end
