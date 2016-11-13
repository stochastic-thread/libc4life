#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "c4.h"
#include "coro.h"
#include "ctx.h"
#include "macros.h"
#include "mem/malloc.h"
#include "slab.h"

struct c4slab *c4slab_init(struct c4slab *self, size_t it_size) {
  self->it_size = it_size;
  self->its = NULL;
  self->len = 0;
  return self;
}

void c4slab_free(struct c4slab *self) {
  if (self->its) { c4malloc_release(c4ctx()->malloc, self->its); }
}

void c4slab_grow(struct c4slab *self, size_t len) {
  if (self->len) {
    while (self->len < len) { self->len *= 2; }
    self->its = c4malloc_require(c4ctx()->malloc,
				 self->its, self->len * self->it_size);
  } else {
    self->len = len;
    self->its = c4malloc_acquire(c4ctx()->malloc, self->len * self->it_size);
  }
}

void *c4slab_idx(struct c4slab *self, size_t idx) {
  return self->its + idx * self->it_size;
}

void c4slab_move(struct c4slab *self, size_t dest, size_t src, size_t len) {
  void *dest_ptr = c4slab_idx(self, dest);
  void *src_ptr = c4slab_idx(self, src);
  memmove(dest_ptr, src_ptr, len * self->it_size);
}

static void *seq_next(struct c4seq *_seq) {
  struct c4slab_seq *seq = C4PTROF(c4slab_seq, seq, _seq);
  return (_seq->idx < seq->slab->len) ? c4slab_idx(seq->slab, _seq->idx) : NULL;
}

struct c4seq *c4slab_seq(struct c4slab *self, struct c4slab_seq *seq) {
  c4seq_init(&seq->seq);
  seq->seq.next = seq_next;
  seq->slab = self;
  return &seq->seq;
}
