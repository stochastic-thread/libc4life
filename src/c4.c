#include <assert.h>
#include <stddef.h>
#include "c4.h"
#include "col.h"
#include "err.h"

static c4get_ctx_t _get_ctx;

struct c4ctx *c4ctx(c4get_ctx_t get_ctx) {
  assert(_get_ctx);
  return _get_ctx();
}

struct c4err_t c4err;
struct c4col_t c4str_col;

void c4init(c4get_ctx_t get_ctx) {
  _get_ctx = get_ctx;
  c4err_t_init(&c4err, "err");
  c4col_t_init(&c4str_col, "str");
}

void c4free() {
  c4err_t_free(&c4err);
  c4col_t_free(&c4str_col);
}

