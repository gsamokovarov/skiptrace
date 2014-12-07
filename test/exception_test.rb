require 'test_helper'

class ExceptionTest < MiniTest::Test
  class TestScenarionWithNestedCalls
    def call
      raise_an_error
    rescue => exc
      exc
    end

    private

      def raise_an_error
        unused_local_variable = 42
        raise
      end
  end

  CustomError = Class.new(StandardError)

  def test_bindings_returns_all_the_bindings_of_where_the_error_originated
    unused_local_variable = "Test"
    raise
  rescue => exc
    assert_equal 'Test', exc.bindings.first.eval('unused_local_variable')
  end

  def test_bindings_returns_all_the_bindings_of_where_a_custom_error_originated
    unused_local_variable = "Test"
    raise CustomError
  rescue => exc
    assert_equal 'Test', exc.bindings.first.eval('unused_local_variable')
  end

  def test_bindings_goes_down_the_stack
    exc = TestScenarionWithNestedCalls.new.call

    assert_equal 42, exc.bindings.first.eval('unused_local_variable')
  end

  def test_bindings_is_empty_when_exception_is_still_not_raised
    exc = CustomError.new

    assert_equal [], exc.bindings
  end

  def test_bindings_is_empty_when_set_backtrace_is_badly_called
    exc = CustomError.new

    # Exception#set_backtrace expects a string or array of strings. If the
    # input isn't like this it will raise a TypeError.
    assert_raises(TypeError) do
      exc.set_backtrace([nil])
    end

    assert_equal [], exc.bindings
  end
end
