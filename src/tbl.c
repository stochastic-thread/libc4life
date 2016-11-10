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

struct c4tbl_iter *c4tbl_iter(struct c4tbl *self, struct c4tbl_iter *iter) {
  iter->line = 0;
  c4rec_init(&iter->rec, NULL);
  iter->tbl = self;
  return iter;
}

void c4tbl_iter_free(struct c4tbl_iter *iter) { c4rec_free(&iter->rec); }

struct c4rec *c4tbl_iter_next(struct c4tbl_iter *iter) {
  C4CORO(&iter->line)
    for (iter->i = 0; iter->i < iter->tbl->recs.len; iter->i++) {
      C4CORO_RET(&iter->rec);
    }
  C4CORO_END();
  c4tbl_iter_free(iter);
  return NULL;
}
