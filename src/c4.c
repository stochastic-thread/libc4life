#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "c4.h"
#include "ctx.h"
#include "err.h"
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

void c4init() {
  c4err_t_init(&c4err, NULL, "c4err");
  c4init_val_ts();
}

void c4free() {
  C4LS_DO(c4err_ts(), t) { c4err_t_free(c4ptrof(c4err_t, ts_node, t)); }
  C4LS_DO(c4val_ts(), t) { c4val_t_free(c4ptrof(c4val_t, ts_node, t)); }
}

