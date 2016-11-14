#include <assert.h>
#include <stdlib.h>
#include "coro.h"
#include "macros.h"
#include "bmap.h"
#include "seq.h"

static void *get_key(void *_it) {
  struct c4bmap_it *it = _it;
  return it->key;
}

struct c4bmap *c4bmap_init(struct c4bmap *self, c4cmp_t cmp) {
  self->cmp = cmp;
  self->cmp_data = NULL;
  c4bset_init(&self->its, sizeof(struct c4bmap_it), cmp);
  self->its.get_key = get_key;
  return self;
}

void c4bmap_free(struct c4bmap *self) { c4bset_free(&self->its); }

void c4bmap_clear(struct c4bmap *self) { c4bset_clear(&self->its); }

size_t c4bmap_add(struct c4bmap *self, void *key, void *val) {
  size_t idx;
  struct c4bmap_it *it = c4bmap_find(self, key, 0, &idx);
  if (it) { return -1; }
  c4bmap_insert(self, idx, key, val);
  return idx;
}

struct c4bmap_it *c4bmap_find(struct c4bmap *self, void *key, size_t start,
			      size_t *idx) {
  return c4bset_find(&self->its, key, start, idx);
}

void *c4bmap_get(struct c4bmap *self, void *key) {
  struct c4bmap_it *it = c4bmap_find(self, key, 0, NULL);
  return it ? it->val : NULL;
}

struct c4bmap_it *c4bmap_idx(struct c4bmap *self, size_t idx) {
  return c4bset_idx(&self->its, idx);
}

struct c4bmap_it *c4bmap_insert(struct c4bmap *self,
			      size_t idx,
			      void *key, void *val) {
  struct c4bmap_it *it = c4bset_insert(&self->its, idx);
  it->key = key;
  it->val = val;
  return it;
}

void c4bmap_merge(struct c4bmap *self, struct c4bmap *src) {
  c4bset_merge(&self->its, &src->its);
}

size_t c4bmap_set(struct c4bmap *self, void *key, void *val) {
  size_t idx;
  struct c4bmap_it *it = c4bmap_find(self, key, 0, &idx);
  if (it) { it->val = val; }
  else { c4bmap_insert(self, idx, key, val); }
  return idx;
}

struct c4seq *c4bmap_seq(struct c4bmap *self, struct c4bmap_seq *seq) {
  return c4bset_seq(&self->its, &seq->seq);
}
