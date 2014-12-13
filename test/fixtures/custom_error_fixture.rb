class CustomErrorFixture
  Error = Class.new(StandardError)

  def call
    unused_local_variable = 'Test'
    raise Error
  rescue => exc
    exc
  end
end
