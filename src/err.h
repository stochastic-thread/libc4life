#ifndef C4LIFE_ERR
#define C4LIFE_ERR

#include "ls.h"
#include "macros.h"

#define C4THROW(msg)							\
  ({									\
    struct c4err *e = malloc(sizeof(struct c4err));			\
    c4err_init(e, msg, __FILE__, __LINE__);				\
    c4err_throw(e);							\
  })									\

#define _C4TRY(msg, _try)						\
  for (struct c4try *_try = c4try_init(malloc(sizeof(struct c4try)),	\
				       msg, __FILE__, __LINE__);	\
       _try != NULL;							\
       c4try_free(_try), _try = NULL)					\
    
#define C4TRY(msg)				\
  _C4TRY(msg, UNIQUE(try))			\

struct c4try {
  char *msg;
  const char *file;
  int line;
  struct c4ls errs, tries_node;
};

struct c4try *c4try_init(struct c4try *self,
			 const char *msg,
			 const char *file, int line);

void c4try_free(struct c4try *self);

struct c4err_t {
  char *name;
};

struct c4err_t *c4err_t_init(struct c4err_t *self, const char *name);
void c4err_t_free(struct c4err_t *self);

struct c4err {
  char *msg;
  const char *file;
  int line;
  struct c4ls errs_node;
};

struct c4err *c4err_init(struct c4err *self,
			 const char *msg,
			 const char *file, int line);

#endif
