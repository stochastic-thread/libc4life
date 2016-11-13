#ifndef C4DEFER_H
#define C4DEFER_H

#include <seqs/dyna.h>

#define _C4DEFER(code, _def)				\
  void _def() code;					\
  bool _def_trigger __attribute__((cleanup(_def)))	\

#define C4DEFER(code)				\
  _C4DEFER(code, C4GSYM(def))			\

#define _C4DEFER_SCOPE(label, _dyna, _free)		\
  struct c4dyna _dyna;					\
  void _free() {					\
    C4DYNA_DO(&_dyna, _fn) { (*(c4defer_fnt *)_fn)(); }	\
    c4dyna_free(&_dyna);				\
  }							\
							\
  for (struct c4dyna *label				\
	 __attribute__((cleanup(_free))) =		\
	 c4dyna_init(&_dyna, sizeof(c4defer_fnt));	\
       label;						\
       label = NULL)					\
    
#define C4DEFER_SCOPE(label)				\
  _C4DEFER_SCOPE(label, C4GSYM(dyna), C4GSYM(free))	\

#define C4DEFER_TO(label, code)					\
  *((c4defer_fnt *)c4dyna_push(label)) = C4LAMBDA(code, void)	\

typedef void (*c4defer_fnt)();

#endif
