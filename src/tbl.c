#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "coro.h"
#include "macros.h"
#include "rec.h"
#include "tbl.h"
#include "uid.h"

static int uid_cmp(void *x, void *y) {
  return c4uid_cmp(x, y);
}

struct c4tbl *c4tbl_init(struct c4tbl *self) {
  c4map_init(&self->recs, uid_cmp);
  return self;
}
