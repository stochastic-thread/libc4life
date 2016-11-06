#ifndef C4LIFE_MAP
#define C4LIFE_MAP

#include <stdbool.h>
#include <stddef.h>

typedef int (*c4cmp_t)(void *x, void *y);

struct c4map {
  c4cmp_t cmp;
  size_t capac, len;
  void **its;
};

struct c4map *c4map_init(struct c4map *map, c4cmp_t cmp);
void c4map_free(struct c4map *map);

#endif
