#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "col.h"

struct c4ls *c4col_ts() {
  static struct c4ls ts;
  static bool init = true;

  if (init) {
    c4ls_init(&ts);
    init = false;
  }

  return &ts;
}

struct c4col_t *c4col_t_init(struct c4col_t *self, const char *name) {
  self->name = strdup(name);
  c4ls_prepend(c4col_ts(), &self->ts_node);
  return self;
}

void c4col_t_free(struct c4col_t *self) {
  free(self->name);
  c4ls_delete(&self->ts_node);
}

struct c4col *c4col_init(struct c4col *self,
			 const char *name,
			 struct c4col_t *type) {
  self->name = strdup(name);
  self->type = type;
  return self;
}

void c4col_free(struct c4col *self) { free(self->name); }
