#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "val.h"

struct c4val_t c4str;

struct c4ls *c4val_ts() {
  static struct c4ls ts;
  static bool init = true;

  if (init) {
    c4ls_init(&ts);
    init = false;
  }

  return &ts;
}

struct c4val_t *c4val_t_init(struct c4val_t *self, const char *name) {
  self->name = strdup(name);
  c4ls_prepend(c4val_ts(), &self->ts_node);
  return self;
}

void c4val_t_free(struct c4val_t *self) {
  free(self->name);
  c4ls_delete(&self->ts_node);
}
