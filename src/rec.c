#include "rec.h"

static int col_cmp(void *x, void *y) {
  return 0;
}

struct c4rec *c4rec_init(struct c4rec *self) {
  c4map_init(&self->flds, col_cmp);
  return self;
}
