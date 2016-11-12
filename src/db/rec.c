#include <string.h>
#include "col.h"
#include "rec.h"
#include "seqs/slab.h"
#include "val.h"

struct c4rec *c4rec_init(struct c4rec *self, c4uid_t id) {
  if (id) { c4uid_copy(self->id, id); }
  else { c4uid_init(self->id); }
  c4bmap_init(&self->flds, c4cols_cmp);
  return self;
}

void c4rec_free(struct c4rec *self) {
  C4SEQ(c4bmap, &self->flds, fld_seq);
  for (struct c4bmap_it *it; (it = c4seq_next(fld_seq));) {
    struct c4col *col = it->key;
    col->type->free_val(it->val);
  }

  c4bmap_free(&self->flds);
}

void *c4rec_get(struct c4rec *self, struct c4col *col) {
  return c4bmap_get(&self->flds, col);
}

void c4rec_set(struct c4rec *self, struct c4col *col, void *val) {
  c4bmap_set(&self->flds, col, val ? col->type->clone_val(val) : val);
}
