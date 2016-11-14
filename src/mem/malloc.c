#include <assert.h>
#include "c4.h"
#include "malloc.h"

struct c4malloc *c4malloc_init(struct c4malloc *self) {
  self->acquire = NULL;
  self->release = NULL;
  return self;
}

void c4malloc_free(struct c4malloc *self) {
}

void *c4malloc_acquire(struct c4malloc *self, size_t size) {
  assert(self->acquire);
  return self->acquire(self, size);
}

void c4malloc_release(struct c4malloc *self, void *ptr) {
  if (self->release) { self->release(self, ptr); }
}
