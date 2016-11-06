#include <assert.h>
#include <stddef.h>
#include "c4.h"
#include "err.h"

struct c4err_t C4Err;

static c4get_ctx_t _get_ctx;

struct c4ctx *c4ctx(c4get_ctx_t get_ctx) {
  assert(_get_ctx);
  return _get_ctx();
}

void c4init(c4get_ctx_t get_ctx) {
  _get_ctx = get_ctx;
  c4err_t_init(&C4Err, "C4Err");
}
