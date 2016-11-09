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

#endif
