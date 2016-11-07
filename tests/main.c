#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "c4.h"
#include "col.h"
#include "ctx.h"
#include "err.h"
#include "ls.h"
#include "map.h"
#include "rec.h"
#include "tbl.h"

static int int_cmp(void *_x, void *_y) {
  int *x = _x, *y = _y;
  return *x < *y;
}

static void col_tests() {
  struct c4col_t ct;
  c4col_t_init(&ct, "foo");

  struct c4col c;
  c4col_init(&c, "bar", &ct);
  
  c4col_free(&c);
  c4col_t_free(&ct);
}

void err_tests() {
  C4TRY("outer") {
    struct c4err *err = NULL;
    const char *data = "guru meditation"; // data describing error

    //C4TRY("inner") {
    err = C4THROW(&c4err, "test throw", data); // throw basic c4err type
    assert(err->data == data);
      //}

    bool caught = false;
    
    C4CATCH(e, NULL) { // NULL catches everything
      assert(e == err);
      c4err_free(e); // free to handle err
      caught = true;
    }

    assert(caught);
  }
}

static void ls_tests() {
  struct c4ls ls;
  c4ls_init(&ls);
}

static void map_tests() {
  struct c4map m;
  c4map_init(&m, int_cmp);
  c4map_free(&m);
}

static void rec_tests() {
  struct c4rec r;
  c4rec_init(&r); 
}

static void tbl_tests() {
  struct c4tbl t;
  c4tbl_init(&t); 
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
    col_tests();
    err_tests();
    ls_tests();
    map_tests();
    rec_tests();
    tbl_tests();
  }

  c4ctx_free(ctx());
  c4free();
  return 0;
}
