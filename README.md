# Exception Bindings

This gem adds `Exception#bindings` method to every exception.

`Exception#bindings` gets populated with `Binding` objects in the context of
the traces in which the exception bubbled up when it was raised.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'exception_bindings', group: [:development]
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install exception_bindings

Currently, the gem only works on CRuby 2.0 and above.

## Usage

The code is in alpha quality! Give it a thought, before using it. With that in
mind, just raise an exception :)

## Credits

Thanks to John Mair for his work on binding_of_caller, which is a huge
inspiration. Thanks to Charlie Somerville for better_errors where the idea
comes from. Thanks to Koichi Sasada for the debug inspector API in CRuby.
