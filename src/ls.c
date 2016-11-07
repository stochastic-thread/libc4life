#include "ls.h"

struct c4ls *c4ls_init(struct c4ls *self) {
  self->next = self->prev = self;
  return self;
}

struct c4ls *c4ls_append(struct c4ls *self, struct c4ls *next) {
  self->next->prev = next;
  next->next = self->next;
  self->next = next;
  next->prev = self;
  return next;
}

void c4ls_delete(struct c4ls *self) {
  self->next->prev = self->prev;
  self->prev->next = self->next;
}
