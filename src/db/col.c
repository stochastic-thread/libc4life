#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "col.h"

int c4cols_cmp(void *_x, void *_y, void *data) {
  struct c4col *x = _x, *y = _y;
  return strcmp(x->name, y->name);
}

struct c4col *c4col_init(struct c4col *self,
			 const char *name,
			 struct c4val_t *type) {
  self->name = strdup(name);
  self->type = type;
  return self;
}

void c4col_free(struct c4col *self) { free(self->name); }
