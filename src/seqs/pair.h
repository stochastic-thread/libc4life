#ifndef C4PAIR_H
#define C4PAIR_H

#include <stddef.h>

struct c4pair {
  size_t offs;
  char data[];
};

struct c4pair *c4pair(size_t left, size_t right);
void *c4pair_left(struct c4pair *self);
void *c4pair_right(struct c4pair *self);

#endif
