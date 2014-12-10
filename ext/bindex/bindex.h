#ifndef _BINDEX_H
#define _BINDEX_H

#include <ruby.h>

VALUE current_bindings(void);

/* This will come from one of the ruby_*.h files. */
extern VALUE rb_check_backtrace(VALUE);

#endif
