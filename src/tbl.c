#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "col.h"
#include "coro.h"
#include "macros.h"
#include "rec.h"
#include "tbl.h"
#include "uid.h"

struct c4tbl *c4tbl_init(struct c4tbl *self, const char *name) {
  self->name = strdup(name);
  c4map_init(&self->recs, c4uids_cmp);
  return self;
}

void c4tbl_free(struct c4tbl *self) {
  free(self->name);
  c4map_free(&self->recs);
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
    for (iter->idx = 0; iter->idx < iter->tbl->recs.len; iter->idx++) {
      struct c4map_it *it = c4map_idx(&iter->tbl->recs, iter->idx);
      c4uid_copy(iter->rec.id, it->key);
      c4map_clear(&iter->rec.flds);
      c4map_merge(&iter->rec.flds, it->val);
      C4CORO_RET(&iter->rec);
    }
  C4CORO_END();
  
  c4tbl_iter_free(iter);
  return NULL;
}

struct c4rec *c4tbl_upsert(struct c4tbl *self, struct c4rec *rec) {
  size_t idx;
  struct c4map_it *it = c4map_find(&self->recs, rec->id, 0, &idx);

  struct c4map *flds = NULL;
  
  if (it) {
    flds = it->val;
    c4map_clear(flds);
  } else {
    flds = c4map_init(malloc(sizeof(struct c4map)), c4cols_cmp);
    c4map_insert(&self->recs, idx, rec->id, flds);
  }

  c4map_merge(flds, &rec->flds);
  return rec;
}
