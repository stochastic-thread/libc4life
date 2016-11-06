#include <stdlib.h>
#include "c4.h"
#include "ctx.h"
#include "err.h"
#include "map.h"
#include "ls.h"

static int int_cmp(void *_x, void *_y) {
  int *x = _x, *y = _y;
  return *x < *y;
}

static void map_tests() {
  struct c4map m;
  c4map_init(&m, int_cmp);
  c4map_free(&m);
}

static void ls_tests() {
  struct c4ls ls;
  c4ls_init(&ls);
}

static struct c4ctx *ctx() {
  static struct c4ctx ctx;
  static bool init = true;

  if (init) {
    c4ctx_init(&ctx);
    init = false;
  }

  return &ctx;
}

int main() {
  c4ctx(ctx);

  C4TRY("run all tests") {
    map_tests();
    ls_tests();
  }
  
  return 0;
}
