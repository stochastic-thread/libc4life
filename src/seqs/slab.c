#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "coro.h"
#include "macros.h"
#include "slab.h"

struct c4slab *c4slab_init(struct c4slab *self, size_t it_size) {
  self->it_size = it_size;
  self->its = NULL;
  self->len = 0;
  return self;
}

void c4slab_free(struct c4slab *self) {
  if (self->its) { free(self->its); }
}

void c4slab_delete(struct c4slab *self, size_t idx, size_t len) {
  assert(idx < self->len);
  void *start = c4slab_idx(self, idx, len);
  
  if (idx < len) {
    memmove(start, start + self->it_size, (len-idx-1) * self->it_size);
  }
}

void c4slab_grow(struct c4slab *self, size_t len) {
  if (self->len) { while (self->len < len) { self->len *= 2; } }
  else { self->len = len; }
  self->its = realloc(self->its, self->len * self->it_size);
}

void *c4slab_idx(struct c4slab *self, size_t idx, size_t len) {
  assert(idx < len);
  return self->its + idx * self->it_size;
}

void *c4slab_insert(struct c4slab *self, size_t idx, size_t len) {
  void *start = c4slab_idx(self, idx, len);

  if (idx < len-1) {
    memmove(start + self->it_size, start, (len-idx-1) * self->it_size);
  }
  
  return start;
}

static void *seq_next(struct c4seq *_seq) {
  struct c4slab_seq *seq = C4PTROF(c4slab_seq, super, _seq);
  return (_seq->idx < seq->slab->len)
    ? c4slab_idx(seq->slab, _seq->idx, seq->slab->len)
    : NULL;
}

struct c4seq *c4slab_seq(struct c4slab *self, struct c4slab_seq *seq) {
  c4seq_init(&seq->super);
  seq->super.next = seq_next;
  seq->slab = self;
  return &seq->super;
}

