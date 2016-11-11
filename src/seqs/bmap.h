#ifndef C4BMAP_H
#define C4BMAP_H

#include <stdbool.h>
#include <stddef.h>
#include "seq.h"
#include "slab.h"

#define C4BMAP(var, cmp)			\
  struct c4bmap var;				\
  c4bmap_init(&var, cmp);			\

typedef int (*c4cmp_t)(void *x, void *y);

struct c4bmap {
  c4cmp_t cmp;
  size_t len;
  struct c4slab its;
};

struct c4bmap_it {
  void *key, *val;
};

struct c4bmap_seq {
  struct c4seq super;
  struct c4bmap *map;
};

struct c4bmap *c4bmap_init(struct c4bmap *bmap, c4cmp_t cmp);
void c4bmap_free(struct c4bmap *bmap);
size_t c4bmap_add(struct c4bmap *self, void *key, void *val);
void c4bmap_clear(struct c4bmap *self);
struct c4bmap_it *c4bmap_find(struct c4bmap *self,
			    void *key, size_t start,
			    size_t *idx);
void *c4bmap_get(struct c4bmap *self, void *key);
struct c4bmap_it *c4bmap_idx(struct c4bmap *self, size_t idx);
struct c4bmap_it *c4bmap_insert(struct c4bmap *bmap,
			      size_t idx,
			      void *key, void *val);

void c4bmap_merge(struct c4bmap *self, struct c4bmap *src);
size_t c4bmap_set(struct c4bmap *self, void *key, void *val);

struct c4seq *c4bmap_seq(struct c4bmap *self, struct c4bmap_seq *seq);

#endif
