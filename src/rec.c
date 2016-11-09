#include <string.h>
#include "col.h"
#include "rec.h"
#include "val.h"

static int col_cmp(void *_x, void *_y) {
  struct c4col *x = _x, *y = _y;
  return strcmp(x->name, y->name);
}

struct c4rec *c4rec_init(struct c4rec *self) {
  c4map_init(&self->flds, col_cmp);
  return self;
}

void c4rec_free(struct c4rec *self) {
  C4DO_MAP(&self->flds, _col, val) {
    struct c4col *col = _col;
    col.type.free_val(val);
  }
}

void *c4rec_get(struct c4rec *self, struct c4col *col) {
  return c4map_get(&self->flds, col);
}

void c4rec_set(struct c4rec *self, struct c4col *col, void *val) {
  c4map_set(&self->flds, col, val ? col->type->clone_val(val) : val);
}
