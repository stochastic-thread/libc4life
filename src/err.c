#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "c4.h"
#include "ctx.h"
#include "err.h"
#include "macros.h"

struct c4try *c4try() {
  struct c4try *try = STRUCTOF(c4ctx()->tries.prev, struct c4try, tries_node);
  assert(try);
  return try;
}

struct c4try *c4try_init(struct c4try *self,
			 const char *msg,
			 const char *file, int line) {
  self->msg = strdup(msg);
  self->file = file;
  self->line = line;
  self->refs = 1;
  c4ls_init(&self->errs);
  c4ls_prepend(&c4ctx(NULL)->tries, &self->tries_node);
  return self;
}

void c4try_free(struct c4try *self) {
  self->refs--;
  if (self->refs == 0) {
    free(self->msg);
    //TODO forward errs to next try
    c4ls_delete(&self->tries_node);
  }
}

struct c4err_t *c4err_t_init(struct c4err_t *self, const char *name) {
  self->name = strdup(name);
  return self;
}

void c4err_t_free(struct c4err_t *self) {
  free(self->name);
}

struct c4err *c4err_first(struct c4err_t *type) {
  return c4err_next(&c4try()->errs, type);
}

struct c4err *c4err_next(struct c4ls *start, struct c4err_t *type) {
  for (struct c4ls *_e = start->next, *end = &c4try()->errs;
       _e != end;
       _e = _e->next) {
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
  self->try = c4try();
  self->try->refs++;
  self->type = type;
  self->msg = strdup(msg);
  self->data = data;
  self->file = file;
  self->line = line;
  c4ls_init(&self->errs_node);
  return self;
}

void c4err_free(struct c4err *self) {
  free(self->msg);
  c4ls_delete(&self->errs_node);
  c4try_free(self->try);
}

struct c4err *c4err_throw(struct c4err *self) {
  c4ls_prepend(&c4try()->errs, &self->errs_node);
  return self;
}
