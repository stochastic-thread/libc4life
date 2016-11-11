#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c4.h"
#include "coro.h"
#include "ctx.h"
#include "db/col.h"
#include "db/cols/str_col.h"
#include "db/rec.h"
#include "db/tbl.h"
#include "err.h"
#include "seqs/ls.h"
#include "seqs/map.h"
#include "val.h"

static int int_cmp(void *_x, void *_y) {
  int *x = _x, *y = _y;
  if  (*x < *y) return -1;
  return *x > *y;
}

static void col_tests() {
  struct c4val_t type;
  c4val_t_init(&type, "foo");

  struct c4col col;
  c4col_init(&col, "bar", &type);
  
  c4col_free(&col);
  c4val_t_free(&type);
}

struct coro_ctx { int i, line; };

static int coro(struct coro_ctx *ctx, int foo) {
  C4CORO(&ctx->line);
  for(ctx->i = 1; ctx->i <= 10; ctx->i++) { C4CORO_RET(foo + ctx->i); }
  C4CORO_END();
  
  return -1;
}

static void coro_tests() {
  struct coro_ctx ctx = {0, 0};
  for (int i = 1; i <= 10; i++) { assert(coro(&ctx, i) == i*2); }
  assert(coro(&ctx, 0) == -1);
}

static void defer_tests() {
  bool called = false;

  {
    C4DEFER({ called = true; });
    assert(!called);
  }
  
  assert(called);
}

static struct c4err_t custom_type;

static void err_tests() {
  c4err_t_init(&custom_type, NULL, "custom"); // NULL super type
  
  C4TRY("outer") {
    struct c4err *err = NULL;
    C4TRY("inner") { err = C4THROW(&custom_type, "test throw"); }
    bool caught = false;
    
    C4CATCH(e, &custom_type) {
      assert(e == err);
      c4err_free(e); // handle err by freeing
      caught = true;
    }

    assert(caught);

    // make sure queue is empty, NULL matches any type
    C4CATCH(e, NULL) { assert(false); }    
  }
}

static void lambda_tests() {
  assert(C4LAMBDA({ return x*y; }, int, int x, int y)(2, 3) == 6);
}

static void ls_splice_tests() {
  struct c4ls foo;
  struct c4ls bar;
  struct c4ls its[10];

  c4ls_init(&foo);
  c4ls_init(&bar);
  
  for (int i = 0; i < 5; i++) {
    c4ls_prepend(&foo, its+i);
    c4ls_prepend(&bar, its+i+5);    
  }

  c4ls_splice(&foo, bar.next, bar.prev);
  struct c4ls *it = &foo;
  
  for (int i = 0; i < 10; i++) {
    it = it->next;
    assert(it == its+i);
  }

  assert(it->next == &foo);
}

static void ls_tests() {
  ls_splice_tests();
}

static void map_add_tests() {
  struct c4map m;
  c4map_init(&m, int_cmp);

  int ks[3] = {1, 2, 3};
  char vs[3] = {'a', 'b', 'c'};
  
  c4map_add(&m, ks+1, vs+1);
  c4map_add(&m, ks+2, vs+2);
  c4map_add(&m, ks, vs);

  for (int i = 0; i < 3; i++) { assert(c4map_get(&m, ks+i) == vs+i); }
  c4map_free(&m);
}

static void map_seq_tests() {
  struct c4map m;
  c4map_init(&m, int_cmp);

  int ks[3] = {1, 2, 3};
  char vs[3] = {'a', 'b', 'c'};
  for (int i = 0; i < 3; i++) { c4map_add(&m, ks, vs); }

  int i = 0;
  C4DO(c4map, &m, _it) {
    struct c4map_it *it = _it;
    assert(it->key == ks+i);
    i++;
  }

  c4map_free(&m);
}

static void map_set_tests() {
  struct c4map m;
  c4map_init(&m, int_cmp);

  int ks[3] = {1, 2, 3};
  char vs[3] = {'a', 'b', 'c'};

  for (int i = 0; i < 3; i++) { c4map_add(&m, ks+i, vs+i); }
  for (int i = 0; i < 3; i++) { c4map_set(&m, ks+i, vs+3-i); }
  for (int i = 0; i < 3; i++) { assert(c4map_get(&m, ks+i) == vs+3-i); }
  c4map_free(&m);
}

static void map_tests() {
  map_add_tests();
  map_seq_tests();
  map_set_tests();
}

static void rec_tests() {
  struct c4str_col foo;
  c4str_col_init(&foo, "foo");

  struct c4rec rec;
  c4rec_init(&rec, NULL);
  c4set_str(&rec, &foo, "abc");
  assert(strcmp(c4get_str(&rec, &foo), "abc") == 0);

  c4rec_free(&rec);
  c4str_col_free(&foo);
}

static void tbl_seq_tests() {
  struct c4tbl tbl;
  c4tbl_init(&tbl, "foo");
 
  struct c4rec rec;
  c4rec_init(&rec, NULL);
  c4tbl_upsert(&tbl, &rec);

  C4SEQ(c4tbl, &tbl, seq);  
  assert(c4uid_cmp(((struct c4rec *)c4seq_next(seq))->id, rec.id) == 0);
  assert(c4seq_next(seq) == NULL);

  c4rec_free(&rec);
  c4tbl_free(&tbl);
}

static void tbl_tests() {
  tbl_seq_tests();
}

int main() {
  c4init();

  C4TRY("run all tests") {
    col_tests();
    coro_tests();
    defer_tests();
    err_tests();
    lambda_tests();
    ls_tests();
    map_tests();
    rec_tests();
    tbl_tests();

    //C4THROW(&c4err, "test print");
  }

  c4free();
  c4ctx_free(c4ctx());
  return 0;
}
