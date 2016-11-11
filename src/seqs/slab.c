#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "coro.h"
#include "macros.h"
#include "slab.h"

struct c4slab *c4slab_init(struct c4slab *self, size_t slot_size) {
  self->slot_size = slot_size;
  self->slots = NULL;
  self->len = 0;
  return self;
}

void c4slab_free(struct c4slab *self) {
  if (self->slots) { free(self->slots); }
}

struct c4slab *c4slab_grow(struct c4slab *self, size_t len) {
  if (self->len) { while (self->len < len) { self->len *= 2; } }
  else { self->len = len; }
  self->slots = realloc(self->slots, self->len * self->slot_size);
  return self;
}

void *c4slab_idx(struct c4slab *self, size_t idx) {
  return self->slots + idx * self->slot_size;
}

void *c4slab_insert(struct c4slab *self, size_t idx) {
  void *start = c4slab_idx(self, idx);

  if (idx < self->len-1) {
    memmove(start + self->slot_size, start,
	    (self->len-idx-1) * self->slot_size);
  }
  
  return start;
}

static void *seq_next(struct c4seq *_seq) {
  struct c4slab_seq *seq = c4ptrof(c4slab_seq, super, _seq);
  return (_seq->idx < seq->slab->len) ? c4slab_idx(seq->slab, _seq->idx) : NULL;
}

struct c4seq *c4slab_seq(struct c4slab *self, struct c4slab_seq *seq) {
  c4seq_init(&seq->super);
  seq->super.next = seq_next;
  seq->slab = self;
  return &seq->super;
}

size_t c4slab_size(struct c4slab *self) { return self->len * self->slot_size; }

