class Exception
  instance_method(:set_backtrace).tap do |set_backtrace|
    define_method(:set_backtrace) do |backtrace|
      @bindings = ExceptionBindings.current_bindings.drop(1)

      set_backtrace.bind(self).call(backtrace)
    end
  end

  def bindings
    @bindings || []
  end
end
