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

  test '#bindings returns all the bindings of where the error originated' do
    begin
      unused_local_variable = "Test"
      raise
    rescue => exc
      assert_equal 'Test', exc.bindings.first.eval('unused_local_variable')
    end
  end

  test '#bindings returns all the bindings of where a custom error originated' do
    begin
      unused_local_variable = "Test"
      raise CustomError
    rescue => exc
      assert_equal 'Test', exc.bindings.first.eval('unused_local_variable')
    end
  end

  test '#bindings goes down the stack' do
    exc = TestScenarionWithNestedCalls.new.call

    assert_equal 42, exc.bindings.first.eval('unused_local_variable')
  end

  test '#bindings is empty when exception is still not raised' do
    exc = CustomError.new

    assert_equal [], exc.bindings
  end
end
