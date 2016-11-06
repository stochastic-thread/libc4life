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

struct c4err_t *c4err_t_init(struct c4err_t *self, const char *name) {
  self->name = strdup(name);
  return self;
}

void c4err_t_free(struct c4err_t *self) {
  free(self->name);
}

struct c4err *c4err_first(struct c4err_t *type) {
  struct c4try *try = STRUCTOF(c4ctx()->tries.prev, struct c4try, tries_node);
  assert(try);
  return c4err_next(&try->errs, type);
}

struct c4err *c4err_next(struct c4ls *start, struct c4err_t *type) {
  struct c4try *try = STRUCTOF(c4ctx()->tries.prev, struct c4try, tries_node);
  assert(try);
  
  for (struct c4ls *_e = start->next; _e != &try->errs; _e = _e->next) {
    struct c4err *e = STRUCTOF(_e, struct c4err, errs_node);
    if (!type || e->type == type) { return e; }
  }

  return NULL;
}

struct c4err *c4err_init(struct c4err *self,
			 struct c4err_t *type,
			 const char *msg,
			 void *data,
			 const char *file, int line) {
  self->type = type;
  self->msg = strdup(msg);
  self->data = data;
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
