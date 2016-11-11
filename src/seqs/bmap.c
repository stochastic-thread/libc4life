#include <assert.h>
#include <stdlib.h>
#include "coro.h"
#include "macros.h"
#include "bmap.h"
#include "seq.h"

struct c4bmap *c4bmap_init(struct c4bmap *self, c4cmp_t cmp) {
  self->cmp = cmp;
  c4slab_init(&self->its, sizeof(struct c4bmap_it));
  self->len = 0;
  return self;
}

void c4bmap_free(struct c4bmap *self) { c4slab_free(&self->its); }

size_t c4bmap_add(struct c4bmap *self, void *key, void *val) {
  size_t idx;
  struct c4bmap_it *it = c4bmap_find(self, key, 0, &idx);
  if (it) { return -1; }
  c4bmap_insert(self, idx, key, val);
  return idx;
}

void c4bmap_clear(struct c4bmap *self) { self->len = 0; }

struct c4bmap_it *c4bmap_find(struct c4bmap *self,
			    void *key, size_t start,
			    size_t *idx) {
  size_t min = start, max = self->len;
  while (min < max) {
    size_t i = (min + max) / 2;
    struct c4bmap_it *it = c4slab_idx(&self->its, i);

    int cmp = self->cmp(key, it->key);
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

void *c4bmap_get(struct c4bmap *self, void *key) {
  struct c4bmap_it *it = c4bmap_find(self, key, 0, NULL);
  return it ? it->val : NULL;
}

struct c4bmap_it *c4bmap_idx(struct c4bmap *self, size_t idx) {
  return c4slab_idx(&self->its, idx);
}

struct c4bmap_it *c4bmap_insert(struct c4bmap *self,
			      size_t idx,
			      void *key, void *val) {
  if (self->len == self->its.len) { c4slab_grow(&self->its, self->len + 1); }

  struct c4bmap_it *it = c4slab_insert(&self->its, idx);
  it->key = key;
  it->val = val;
  self->len++;
  return it;
}

void c4bmap_merge(struct c4bmap *self, struct c4bmap *src) {
}

size_t c4bmap_set(struct c4bmap *self, void *key, void *val) {
  size_t idx;
  struct c4bmap_it *it = c4bmap_find(self, key, 0, &idx);
  if (it) { it->val = val; }
  else { c4bmap_insert(self, idx, key, val); }
  return idx;
}

static void *seq_next(struct c4seq *_seq) {
  struct c4bmap_seq *seq = c4ptrof(c4bmap_seq, super, _seq);

  return (_seq->idx < seq->map->len)
    ? c4slab_idx(&seq->map->its, _seq->idx)
    : NULL;
}

struct c4seq *c4bmap_seq(struct c4bmap *self, struct c4bmap_seq *seq) {
  c4seq_init(&seq->super);
  seq->super.next = seq_next;
  seq->map = self;
  return &seq->super;
}
