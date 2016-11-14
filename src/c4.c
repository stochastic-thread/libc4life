#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "c4.h"
#include "ctx.h"
#include "err.h"
#include "mem/malloc.h"
#include "val.h"

struct c4ctx *c4ctx() {
  static __thread struct c4ctx ctx;
  static __thread bool init = true;

  if (init) {
    c4ctx_init(&ctx);
    init = false;
  }

  return &ctx;
}

struct c4err_t c4err;
struct c4malloc c4malloc;

static void *acquire(struct c4malloc *self, size_t size) {
  return malloc(size);
}

static void release(struct c4malloc *self, void *ptr) {
  free(ptr);
}

void c4init() {
  srand(time(NULL));
  c4err_t_init(&c4err, NULL, "c4err");

  c4malloc_init(&c4malloc);
  c4malloc.acquire = acquire;
  c4malloc.release = release;
  
  c4init_val_ts();
}

void c4free() {
  C4LS_DO(c4err_ts(), t) { c4err_t_free(C4PTROF(c4err_t, ts_node, t)); }
  C4LS_DO(c4val_ts(), t) { c4val_t_free(C4PTROF(c4val_t, ts_node, t)); }
}

