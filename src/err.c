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
  self->refs = 1;
  c4ls_init(&self->errs);
  struct c4ctx *ctx = c4ctx();
  self->super = ctx->try;
  if (self->super) { c4try_ref(self->super); }
  ctx->try = self;
  return self;
}

void c4try_free(struct c4try *self) {
  self->refs--;

  if (self->super) { c4try_free(self->super); }
  
  if (self->refs == 0) {
    free(self->msg);
    free(self);
  }
}

void c4try_close(struct c4try *self) {
  struct c4try *super = self->super;
  
  if (self->errs.next != &self->errs) {  
    if (super) { c4ls_splice(&super->errs, self->errs.next, self->errs.prev); }
    else {
      C4LS_DO(&self->errs, e) {
	c4err_print(STRUCTOF(e, struct c4err, errs_node), stderr);
      }
    }
  }
  
  c4ctx()->try = super;
  c4try_free(self);
}

void c4try_ref(struct c4try *self) {
  for (struct c4try *t = self; t; t = t->super) { t->refs++; }
}

struct c4ls *c4err_ts() {
  static struct c4ls ts;
  static bool init = true;

  if (init) {
    c4ls_init(&ts);
    init = false;
  }

  return &ts;
}

struct c4err_t *c4err_t_init(struct c4err_t *self,
			     struct c4err_t *super,
			     const char *name) {
  self->super = super;
  self->name = strdup(name);
  c4ls_prepend(c4err_ts(), &self->ts_node);
  return self;
}

void c4err_t_free(struct c4err_t *self) {
  free(self->name);
  c4ls_delete(&self->ts_node);
}

struct c4err *c4err_first(struct c4err_t *type) {
  struct c4try *try = c4ctx()->try;
  assert(try);
  return c4err_next(&try->errs, type);
}

struct c4err *c4err_next(struct c4ls *start, struct c4err_t *type) {
  struct c4try *try = c4ctx()->try;
  assert(try);
  
  for (struct c4ls *_e = start->next, *end = &try->errs;
       _e != end;
       _e = _e->next) {
    struct c4err *e = STRUCTOF(_e, struct c4err, errs_node);
    if (!type || c4err_isa(e, type)) { return e; }
  }

  return NULL;
}

struct c4err *c4err_init(struct c4err *self,
			 struct c4try *try,
			 struct c4err_t *type,
			 const char *msg,
			 const char *file, int line) {
  assert(try);
  self->try = try;
  c4try_ref(try);
  c4ls_prepend(&self->try->errs, &self->errs_node);
  self->type = type;
  self->msg = strdup(msg);
  self->data = NULL;
  self->file = file;
  self->line = line;
  return self;
}

void c4err_free(struct c4err *self) {
  free(self->msg);
  c4ls_delete(&self->errs_node);
  c4try_free(self->try);
  free(self);
}

bool c4err_isa(struct c4err *self, struct c4err_t *type) {
  for (struct c4err_t *t = self->type; t != NULL; t = t->super) {
    if (t == type) { return true; }
  }

  return false;
}

void c4err_print(struct c4err *self, FILE *out) {
  fprintf(out, "error: %s\n%s:%d\n",
	  self->msg, self->file, self->line);

  for (struct c4try *try = self->try; try; try = try->super) {
    fprintf(out, "\t%s\n\t%s:%d\n", try->msg, try->file, try->line);
  }
}
