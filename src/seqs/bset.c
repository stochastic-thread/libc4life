#include <assert.h>
#include <stdlib.h>
#include "coro.h"
#include "macros.h"
#include "bset.h"
#include "seq.h"

struct c4bset *c4bset_init(struct c4bset *self, size_t key_size, c4cmp_t cmp) {
  self->cmp = cmp;
  c4dyna_init(&self->its, key_size);
  return self;
}

void c4bset_free(struct c4bset *self) { c4dyna_free(&self->its); }

void *c4bset_add(struct c4bset *self, void *key) {
  size_t idx;
  struct c4bset_it *it = c4bset_find(self, key, 0, &idx);
  if (it) { return NULL; }
  return c4bset_insert(self, idx, key);
}

void c4bset_clear(struct c4bset *self) { c4dyna_clear(&self->its); }

void *c4bset_find(struct c4bset *self, void *key, size_t start, size_t *idx) {
  size_t min = start, max = self->its.len;
  
  while (min < max) {
    size_t i = (min + max) / 2;
    void *it = c4dyna_idx(&self->its, i);
    int cmp = self->cmp(key, it);

    if (cmp < 0) { max = i; }
    else if (cmp > 0) { min = i + 1; }
    else {
      if (idx) { *idx = i; }
      return it;
    }
  }

  if (idx) { *idx = max; }
  return NULL;
}

void *c4bset_get(struct c4bset *self, void *key) {
  return c4bset_find(self, key, 0, NULL);
}

void *c4bset_idx(struct c4bset *self, size_t idx) {
  return c4dyna_idx(&self->its, idx);
}

void *c4bset_insert(struct c4bset *self, size_t idx, void *key) {
  return c4dyna_insert(&self->its, idx);
}

size_t cbset_len(struct c4bset *self) { return self->its.len; }

void c4bset_merge(struct c4bset *self, struct c4bset *src) {
}

static void *seq_next(struct c4seq *_seq) {
  struct c4bset_seq *seq = C4PTROF(c4bset_seq, seq, _seq);
  return (_seq->idx < seq->bset->its.len)
    ? c4dyna_idx(&seq->bset->its, _seq->idx)
    : NULL;
}

struct c4seq *c4bset_seq(struct c4bset *self, struct c4bset_seq *seq) {
  c4seq_init(&seq->seq);
  seq->seq.next = seq_next;
  seq->bset = self;
  return &seq->seq;
}
