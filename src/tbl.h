#ifndef C4LIFE_TBL
#define C4LIFE_TBL

#include "map.h"

struct c4tbl {
  struct c4map recs;
};

typedef struct c4rec *(*c4tbl_iter_t)();

struct c4tbl *c4tbl_init(struct c4tbl *self);

c4tbl_iter_t c4tbl_iter(struct c4tbl *self);

#endif
