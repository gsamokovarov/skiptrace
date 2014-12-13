class EvalNestedFixture
  def call
    tap { raise_an_error_in_eval }
  rescue => exc
    exc
  end

  private

    def raise_an_error_in_eval
      unused_local_variable = 42
      eval 'raise'
    end
end
