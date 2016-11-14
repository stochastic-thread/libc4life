#ifndef C4MALLOC_H
#define C4MALLOC_H

#include <stdlib.h>

struct c4malloc {
  void *(*acquire)(struct c4malloc *, size_t);
  void (*release)(struct c4malloc *, void *);
};

struct c4malloc *c4malloc_init(struct c4malloc *self);
void *c4malloc_acquire(struct c4malloc *self, size_t size);
void c4malloc_release(struct c4malloc *self, void *ptr);

#endif
