class FlatFixture
  def call
    unused_local_variable = "Test"
    raise
  rescue => exc
    exc
  end
end
