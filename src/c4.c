#include <assert.h>
#include <stddef.h>
#include "c4.h"

static c4get_ctx_t _get_ctx;

struct c4ctx *c4ctx(c4get_ctx_t get_ctx) {
  if (get_ctx) {
    _get_ctx = get_ctx;
    return NULL;
  }

  assert(_get_ctx);
  return _get_ctx();
}
