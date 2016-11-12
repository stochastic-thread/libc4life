#ifndef C4MSLAB_H
#define C4MSLAB_H

#include "malloc.h"

struct c4mslab {
  size_t offs, size;
  struct c4malloc malloc;
  void *data;
};

struct c4mslab *c4mslab_init(struct c4mslab *self);
void c4mslab_free(struct c4mslab *self);

#endif
