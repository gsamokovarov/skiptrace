# Bindex [![Build Status](https://travis-ci.org/gsamokovarov/bindex.svg?branch=master)](https://travis-ci.org/gsamokovarov/bindex)

This gem adds `Exception#bindings` method to every exception.

`Exception#bindings` gets populated with `Binding` objects in the context of
the traces in which the exception bubbled up when it was raised.

## Usage

The code is in alpha quality! Give it a thought, before using it. With that in
mind, just raise an exception :)

## Support

### CRuby

CRuby 1.9.2 and below is **not** supported.

### JRuby

To get the best support, run JRuby in interpreted mode.

```bash
export JRUBY_OPTS=-J-Djruby.compile.mode=OFF

# If you run JRuby 1.7.12 and above, you can use:
export JRUBY_OPTS=--dev
```

### Rubinius

Internal errors like `ZeroDevisionError` aren't caught.

## Credits

Thanks to John Mair for his work on binding_of_caller, which is a huge
inspiration. Thanks to Charlie Somerville for better_errors where the idea
comes from. Thanks to Koichi Sasada for the debug inspector API in CRuby.
