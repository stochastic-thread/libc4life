#ifndef C4LIFE_MAP
#define C4LIFE_MAP

#include <stdbool.h>
#include <stddef.h>
#include "slab.h"

typedef int (*c4cmp_t)(void *x, void *y);

struct c4map_it {
  void *key, *val;
};

struct c4map {
  c4cmp_t cmp;
  size_t len;
  struct c4slab its;
};

struct c4map *c4map_init(struct c4map *map, c4cmp_t cmp);
void c4map_free(struct c4map *map);
size_t c4map_add(struct c4map *self, void *key, void *val);
struct c4map_it *c4map_find(struct c4map *self,
			    void *key, size_t start,
			    size_t *idx);
void *c4map_get(struct c4map *self, void *key);
struct c4map_it *c4map_insert(struct c4map *map,
			      size_t idx,
			      void *key, void *val);
size_t c4map_set(struct c4map *self, void *key, void *val);

#endif
