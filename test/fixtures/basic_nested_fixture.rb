class BasicNestedFixture
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
