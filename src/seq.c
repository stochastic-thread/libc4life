#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "macros.h"
#include "seq.h"

struct c4seq *c4seq_init(struct c4seq *self) {
  self->eof = false;
  self->idx = 0;
  self->free = NULL;
  self->next = NULL;
  return self;
}

static void map_free(struct c4seq *_seq) {
  struct c4seq_map *seq = STRUCTOF(_seq, struct c4seq_map, super);
  free(seq);
}

static void *map_next(struct c4seq *_seq) {
  struct c4seq_map *seq = STRUCTOF(_seq, struct c4seq_map, super);
  void *it = c4seq_next(seq->src);
  return it ? seq->fn(it) : NULL;
}

struct c4seq *c4seq_map(struct c4seq *self,
			c4seq_map_fnt fn,
			struct c4seq_map *out) {
  if (out) { c4seq_init(&out->super); }
  else {
    out = malloc(sizeof(struct c4seq_map));
    c4seq_init(&out->super);
    out->super.free = map_free;
  }

  out->super.next = map_next;
  out->src = self;
  out->fn = fn;  
  return &out->super;
}

void *c4seq_next(struct c4seq *self) {
  assert(self->next);
  void *it = self->next(self);

  if (it) { self->idx++; }
  else {
    self->eof = true;
    c4seq_free(self);
  }

  return it;
}

void c4seq_free(struct c4seq *self) {
  if (self->free) { self->free(self); }
}
