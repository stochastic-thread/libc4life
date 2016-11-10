#ifndef C4LIFE_TBL
#define C4LIFE_TBL

#include "map.h"

struct c4rec;

struct c4tbl {
  struct c4map recs;
};

struct c4tbl_iter {
  int i, line;
  struct c4rec rec;
  struct c4tbl *tbl;
};

struct c4tbl *c4tbl_init(struct c4tbl *self);
struct c4tbl_iter *c4tbl_iter(struct c4tbl *self, struct c4tbl_iter *iter);
void c4tbl_iter_free(struct c4tbl_iter *iter);
struct c4rec *c4tbl_iter_next(struct c4tbl_iter *iter);

#endif
