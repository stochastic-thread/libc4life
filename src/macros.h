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

#define _C4LET_FN(var, code, ret, _fn, ...)			\
  ret _fn(__VA_ARGS__) code					\
  for (typeof(_fn) *var = &_fn; var != NULL; var = NULL)	\

#define C4LET_FN(var, code, ret, ...)				\
  _C4LET_FN(var, code, ret, UNIQUE(fn), ##__VA_ARGS__)	\

#endif
