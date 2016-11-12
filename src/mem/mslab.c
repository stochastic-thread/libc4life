#include "mslab.h"

static void *acquire(struct c4malloc *self, size_t size) {
  return NULL;
}

static void release(struct c4malloc *self, void *ptr) {
}

static void *require(struct c4malloc *self, void *ptr, size_t size) {
  return NULL;
}

struct c4mslab *c4mslab_init(struct c4mslab *self) {
  c4malloc_init(&self->malloc);
  self->malloc.acquire = acquire;
  self->malloc.release = release;
  self->malloc.require = require;

  self->offs = self->size = 0;
  self->data = NULL;
  return self;
}

void c4mslab_free(struct c4mslab *self) { free(self->data); }
