#include "val.h"
#include "str_col.h"

struct c4str_col *c4str_col_init(struct c4str_col *self, const char *name) {
  c4col_init(&self->super, name, &c4str);
  return self;
}

void c4str_col_free(struct c4str_col *self) {
  c4col_free(&self->super);
}

const char *c4get_str(struct c4rec *rec, struct c4str_col *col) {
  return c4rec_get(&col->super);
}

void c4set_str(struct c4rec *rec, struct c4str_col *col, const char *val) {
  c4rec_set(&col->super, val);
}
