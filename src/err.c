#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "c4.h"
#include "ctx.h"
#include "err.h"
#include "macros.h"

struct c4try *c4try_init(struct c4try *self,
			 const char *msg,
			 const char *file, int line) {
  self->msg = strdup(msg);
  self->file = file;
  self->line = line;
  c4ls_init(&self->errs);
  c4ls_append(&c4ctx(NULL)->tries, &self->tries_node);
  return self;
}

void c4try_free(struct c4try *self) {
  free(self->msg);
}

struct c4err *c4err_init(struct c4err *self,
			 const char *msg,
			 const char *file, int line) {
  self->msg = strdup(msg);
  self->file = file;
  self->line = line;
  c4ls_init(&self->errs_node);
  return self;
}

struct c4err *c4err_throw(struct c4err *self) {
  struct c4try *try = STRUCTOF(c4ctx(NULL)->tries.prev,
			       struct c4try,
			       tries_node);
  assert(try);
  c4ls_append(&try->errs, &self->errs_node);
  return self;
}
