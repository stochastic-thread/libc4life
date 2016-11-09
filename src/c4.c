#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "c4.h"
#include "err.h"
#include "val.h"

static c4get_ctx_t _get_ctx;

struct c4ctx *c4ctx(c4get_ctx_t get_ctx) {
  assert(_get_ctx);
  return _get_ctx();
}

struct c4err_t c4err;

void c4init(c4get_ctx_t get_ctx) {
  _get_ctx = get_ctx;
  c4err_t_init(&c4err, NULL, "c4err");
  c4init_val_ts();
}

void c4free() {
  C4LS_DO(c4err_ts(), t) { c4err_t_free(STRUCTOF(t, struct c4err_t, ts_node)); }
  C4LS_DO(c4val_ts(), t) { c4val_t_free(STRUCTOF(t, struct c4val_t, ts_node)); }
}

