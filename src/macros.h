#ifndef C4LIFE_MACROS
#define C4LIFE_MACROS

#define _CONCAT(x, y)                           \
  x ## y					\

#define CONCAT(x, y)                            \
  _CONCAT(x, y)					\
  
#define STRUCTOF(ptr, type, field)					\
  ({									\
    const typeof(((type *)0)->field) *_field_ptr = (ptr);		\
    (type *)((char *)_field_ptr - offsetof(type, field));		\
  })									\

#define UNIQUE(prefix)                          \
  CONCAT(prefix, __LINE__)			\

#define _C4DEFER(code, _def)					\
  void _def() code;						\
  bool _def_trigger __attribute__((cleanup(_def)))		\
       
#define C4DEFER(code)				\
  _C4DEFER(code, UNIQUE(def))			\
  
#define _C4LAMBDA(code, ret, _fn, ...)			\
  ({							\
    ret _fn(__VA_ARGS__) code;				\
    &_fn;						\
  })							\
	       
#define C4LAMBDA(code, ret, ...)			\
  _C4LAMBDA(code, ret, UNIQUE(fn), ##__VA_ARGS__)	\

#endif
