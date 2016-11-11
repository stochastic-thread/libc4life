#include <assert.h>
#include <stdlib.h>
#include "coro.h"
#include "macros.h"
#include "map.h"
#include "seq.h"

struct c4map *c4map_init(struct c4map *self, c4cmp_t cmp) {
  self->cmp = cmp;
  c4slab_init(&self->its, sizeof(struct c4map_it));
  self->len = 0;
  return self;
}

void c4map_free(struct c4map *self) { c4slab_free(&self->its); }

size_t c4map_add(struct c4map *self, void *key, void *val) {
  size_t idx;
  struct c4map_it *it = c4map_find(self, key, 0, &idx);
  if (it) { return -1; }
  c4map_insert(self, idx, key, val);
  return idx;
}

void c4map_clear(struct c4map *self) { self->len = 0; }

struct c4map_it *c4map_find(struct c4map *self,
			    void *key, size_t start,
			    size_t *idx) {
  size_t min = start, max = self->len;
  while (min < max) {
    size_t i = (min + max) / 2;
    struct c4map_it *it = c4slab_idx(&self->its, i);

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

void *c4map_get(struct c4map *self, void *key) {
  struct c4map_it *it = c4map_find(self, key, 0, NULL);
  return it ? it->val : NULL;
}

struct c4map_it *c4map_idx(struct c4map *self, size_t idx) {
  return c4slab_idx(&self->its, idx);
}

struct c4map_it *c4map_insert(struct c4map *self,
			      size_t idx,
			      void *key, void *val) {
  if (self->len == self->its.len) { c4slab_grow(&self->its, self->len + 1); }

  struct c4map_it *it = c4slab_insert(&self->its, idx);
  it->key = key;
  it->val = val;
  self->len++;
  return it;
}

void c4map_merge(struct c4map *self, struct c4map *src) {
}

size_t c4map_set(struct c4map *self, void *key, void *val) {
  size_t idx;
  struct c4map_it *it = c4map_find(self, key, 0, &idx);
  if (it) { it->val = val; }
  else { c4map_insert(self, idx, key, val); }
  return idx;
}

static void *seq_next(struct c4seq *_seq) {
  struct c4map_seq *seq = STRUCTOF(_seq, struct c4map_seq, super);
  
  C4CORO(&seq->line)
    for (seq->idx = 0; seq->idx < seq->map->len; seq->idx++) {
      void *it = c4slab_idx(&seq->map->its, seq->idx);
      C4CORO_RET(it);
    }
  C4CORO_END();

  return NULL;
}

struct c4seq *c4map_seq(struct c4map *self, struct c4map_seq *seq) {
  c4seq_init(&seq->super);
  seq->super.next = seq_next;
  seq->line = 0;
  seq->map = self;
  return &seq->super;
}
