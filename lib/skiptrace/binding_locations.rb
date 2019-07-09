module Skiptrace
  class BindingLocations < Array
    def initialize(locations, bindings)
      super(locations)

      @bindings = bindings
      @cached_locations = {}
    end

    def [](index)
      cached_locations(super, index)
    end

    private

    def cached_locations(location, index)
      @cached_locations[index] ||= Location.new(location, guess_binding_around(location))
    end

    def guess_binding_around(location)
      location && @bindings.find do |binding|
        binding.source_location == [location.path, location.lineno]
      end
    end
  end
end
