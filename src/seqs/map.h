#ifndef C4LIFE_MAP
#define C4LIFE_MAP

#include <stdbool.h>
#include <stddef.h>
#include "seq.h"
#include "slab.h"

#define _C4DO_MAP(map, vkey, vval, _idx, _it)				\
  size_t idx = 0;							\
  struct c4map_it *it = c4map_idx(map, 0);				\
  for (void *vkey = it ? it->key : NULL,				\
	 *vval = it ? it->val: NULL;					\
       idx < (map)->len;						\
       idx++,								\
	 it = (idx < (map)->len)					\
	 ? c4map_idx(map, idx)						\
	 : NULL,							\
	 vkey = it ? it->key : NULL,					\
	 vval = it ? it->val : NULL)					\

#define C4DO_MAP(map, key, val)				\
  _C4DO_MAP(map, key, val, UNIQUE(idx), UNIQUE(it))	\

typedef int (*c4cmp_t)(void *x, void *y);

struct c4map {
  c4cmp_t cmp;
  size_t len;
  struct c4slab its;
};

struct c4map_it {
  void *key, *val;
};

struct c4map_seq {
  struct c4seq super;
  int idx, line;
  struct c4map *map;
};

struct c4map *c4map_init(struct c4map *map, c4cmp_t cmp);
void c4map_free(struct c4map *map);
size_t c4map_add(struct c4map *self, void *key, void *val);
void c4map_clear(struct c4map *self);
struct c4map_it *c4map_find(struct c4map *self,
			    void *key, size_t start,
			    size_t *idx);
void *c4map_get(struct c4map *self, void *key);
struct c4map_it *c4map_idx(struct c4map *self, size_t idx);
struct c4map_it *c4map_insert(struct c4map *map,
			      size_t idx,
			      void *key, void *val);

void c4map_merge(struct c4map *self, struct c4map *src);
size_t c4map_set(struct c4map *self, void *key, void *val);

struct c4seq *c4map_seq(struct c4map *self, struct c4map_seq *seq);

#endif
