#ifndef C4MACROS_H
#define C4MACROS_H

#define _C4LAMBDA(code, ret, _fn, ...)		\
  ({						\
    ret _fn(__VA_ARGS__) code;			\
    &_fn;					\
  })						\

#define C4LAMBDA(code, ret, ...)			\
  _C4LAMBDA(code, ret, C4GSYM(fn), ##__VA_ARGS__)	\

#define _C4MAX(x, y)				\
  (x >= y ? x : y)				\
  
#define C4MAX(x, y)				\
  _C4MAX((x), (y))				\
  
#define _C4MIN(x, y)				\
  (x <= y ? x : y)				\

#define C4MAX(x, y)				\
  _C4MAX((x), (y))				\

#define _C4SYMS(x, y)				\
  x ## y					\
  
#define C4SYMS(x, y)				\
  _C4SYMS(x, y)					\

#define C4GSYM(prefix)				\
  C4SYMS(_c4, C4SYMS(prefix, __LINE__))		\
  
#define _C4PTROF(type, field, ptr)				\
  ({								\
    const typeof(((struct type *)0)->field) *_field_ptr = ptr;	\
    (struct type *)((char *)_field_ptr -			\
		    offsetof(struct type, field));		\
  })								\

#define C4PTROF(type, field, ptr)		\
  _C4PTROF(type, field, (ptr))			\
	  
#endif
