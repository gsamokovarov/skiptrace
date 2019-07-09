require 'test_helper'

class CurrentBindingsTest < BaseTest
  test 'first binding returned is the current one' do
    _, lineno = Skiptrace.current_bindings.first.source_location

    assert_equal __LINE__ - 2, lineno
  end
end
