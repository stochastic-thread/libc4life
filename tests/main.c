#include <assert.h>
#include <stdio.h>
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
  c4init(ctx);

  C4TRY("run all tests") {
    map_tests();
    ls_tests();
    struct c4err *err = NULL;
    //C4TRY("nested") {
    err = C4THROW(&C4Err, "test throw", ctx);
      //}

    bool caught = false;
    
    C4CATCH(e, NULL) {
      assert(e == err); 
      caught = true;
    }

    assert(caught);
  }

  c4free();
  return 0;
}
