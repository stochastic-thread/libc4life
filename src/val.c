#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "val.h"

struct c4val_t c4str;

static void *clone_str(void *val) { return strdup(val); }
static void free_str(void *val) { free(val); }

void c4init_val_ts() {
  c4val_t_init(&c4str, "str");
  c4str.clone_val = clone_str;
  c4str.free_val = free_str;
}

struct c4ls *c4val_ts() {
  static struct c4ls ts;
  static bool init = true;

  if (init) {
    c4ls_init(&ts);
    init = false;
  }

  return &ts;
}

static void *clone_val(void *val) { return val; }

static void free_val(void *val) { /* do nothing by default */ }

struct c4val_t *c4val_t_init(struct c4val_t *self, const char *name) {
  self->name = strdup(name);
  c4ls_prepend(c4val_ts(), &self->ts_node);
  self->clone_val = clone_val;
  self->free_val = free_val;
  return self;
}

void c4val_t_free(struct c4val_t *self) {
  free(self->name);
  c4ls_delete(&self->ts_node);
}
