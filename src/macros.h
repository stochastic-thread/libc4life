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

#endif
