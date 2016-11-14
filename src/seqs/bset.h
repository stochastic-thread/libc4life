#ifndef C4BSET_H
#define C4BSET_H

#include <stdbool.h>
#include <stddef.h>
#include "dyna.h"
#include "seq.h"

#define C4BSET(var, key_size, cmp)		\
  struct c4bset var;				\
  c4bset_init(&var, key_size, cmp);		\

typedef int (*c4cmp_t)(void *x, void *y, void *data);
typedef void *(*c4bset_key_fnt)(void *it);

struct c4bset {
  c4bset_key_fnt get_key;
  c4cmp_t cmp;
  struct c4dyna its;
  void *cmp_data;
};

struct c4bset_seq {
  struct c4seq seq;
  struct c4bset *bset;
};

struct c4bset *c4bset_init(struct c4bset *self, size_t key_size, c4cmp_t cmp);
void c4bset_free(struct c4bset *self);
void *c4bset_add(struct c4bset *self, void *key);
void c4bset_clear(struct c4bset *self);
void *c4bset_find(struct c4bset *self, void *key, size_t start, size_t *idx);
void *c4bset_get(struct c4bset *self, void *key);
void *c4bset_idx(struct c4bset *self, size_t idx);
void *c4bset_insert(struct c4bset *self, size_t idx);
size_t cbset_len(struct c4bset *self);
void c4bset_merge(struct c4bset *self, struct c4bset *src);

struct c4seq *c4bset_seq(struct c4bset *self, struct c4bset_seq *seq);

#endif
