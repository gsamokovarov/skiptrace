require 'test_helper'

class ExceptionTest < BaseTest
  test 'bindings returns all the bindings of where the error originated' do
    exc = FlatFixture.new.call

    assert_equal 3, exc.bindings.first.source_location.last
  end

  test 'bindings returns all the bindings of where a custom error originate' do
    exc = CustomErrorFixture.new.call

    assert_equal 5, exc.bindings.first.source_location.last
  end

  test 'bindings goes down the stack' do
    exc = BasicNestedFixture.new.call

    assert_equal 11, exc.bindings.first.source_location.last
  end

  test 'bindings inside of an eval' do
    exc = EvalNestedFixture.new.call

    assert_equal 11, exc.bindings.first.source_location.last
  end

  test "re-raising doesn't lose bindings information" do
    exc = ReraisedFixture.new.call

    assert_equal 3, exc.bindings.first.source_location.last
  end

  test 'bindings is empty when exception is still not raised' do
    exc = RuntimeError.new

    assert_equal [], exc.bindings
  end

  test 'bindings is empty when set backtrace is badly called' do
    exc = RuntimeError.new

    # Exception#set_backtrace expects a string or array of strings. If the
    # input isn't like this it will raise a TypeError.
    assert_raises(TypeError) do
      exc.set_backtrace([nil])
    end

    assert_equal [], exc.bindings
  end
end
