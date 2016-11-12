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

bool c4ls_empty(struct c4ls *self) { return self->next == self; }

struct c4ls *c4ls_prepend(struct c4ls *self, struct c4ls *prev) {
  self->prev->next = prev;
  prev->prev = self->prev;
  prev->next = self;
  self->prev = prev;
  return prev;
}

void c4ls_splice(struct c4ls *self, struct c4ls *first, struct c4ls *last) {
  self->prev->next = first;
  first->prev = self->prev;
  self->prev = last;
  last->next = self;
}

