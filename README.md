# Bindex

This gem adds `Exception#bindings` method to every exception.

`Exception#bindings` gets populated with `Binding` objects in the context of
the traces in which the exception bubbled up when it was raised.

## Usage

The code is in alpha quality! Give it a thought, before using it. With that in
mind, just raise an exception :)

## Support

CRuby 1.9.2 and below is **not** supported.

### CRuby 1.9.3

No known issues at the moment. If you find any, please use the issue tracker to
report them.

### CRuby 2.0, 2.1

No known issues at the moment. If you find any, please use the issue tracker to
report them.

### Rubinius

Internal errors like `ZeroDevisionError` aren't caught.

## Credits

Thanks to John Mair for his work on binding_of_caller, which is a huge
inspiration. Thanks to Charlie Somerville for better_errors where the idea
comes from. Thanks to Koichi Sasada for the debug inspector API in CRuby.
