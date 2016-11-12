#ifndef C4MALLOC_H
#define C4MALLOC_H

#include <stdlib.h>

struct c4malloc {
  struct c4malloc *prev;
  void *(*acquire)(struct c4malloc *, size_t);
  void (*release)(struct c4malloc *, void *);
  void *(*require)(struct c4malloc *, void *, size_t);
};

struct c4malloc *c4malloc_init(struct c4malloc *self);
void *c4malloc_acquire(struct c4malloc *self, size_t size);
void c4malloc_release(struct c4malloc *self, void *ptr);
void *c4malloc_require(struct c4malloc *self, void *ptr, size_t size);

#endif
