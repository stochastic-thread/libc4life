#include "tbl.h"

static int recid_cmp(void *x, void *y) {
  return 0;
}

struct c4tbl *c4tbl_init(struct c4tbl *self) {
  c4map_init(&self->recs, recid_cmp);
  return self;
}
