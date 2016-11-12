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
#include "mem/mpool.h"
#include "mem/mslab.h"
#include "seqs/bmap.h"
#include "seqs/dyna.h"
#include "seqs/ls.h"
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

static void dyna_tests() {
  // Declare and initialize dynamic array named arr for int sized items
  
  C4DYNA(arr, sizeof(int));
  const int LEN = 10;

  // Preallocate to fit at least LEN/2 items

  c4dyna_grow(&arr, LEN/2);

  // Populate array

  for (int i = 0; i < LEN; i++) { *(int *)c4dyna_push(&arr) = 1; }
  assert(arr.len == LEN);

  // Empty array and check reverse order

  for (int i = LEN-1; i >= 0; i--) { *(int *)c4dyna_pop(&arr) = i; }
  assert(arr.len == 0);

  c4dyna_free(&arr);
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
      c4err_free(e); // Handle err by freeing
      caught = true;
    }

    assert(caught);

    // Make sure queue is empty, NULL matches any type
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
  C4BMAP(m, int_cmp);
  int ks[3] = {1, 2, 3};
  char vs[3] = {'a', 'b', 'c'};
  
  c4bmap_add(&m, ks+1, vs+1);
  c4bmap_add(&m, ks+2, vs+2);
  c4bmap_add(&m, ks, vs);

  for (int i = 0; i < 3; i++) { assert(c4bmap_get(&m, ks+i) == vs+i); }
  c4bmap_free(&m);
}

static void map_seq_tests() {
  C4BMAP(m, int_cmp);
  int ks[3] = {1, 2, 3};
  char vs[3] = {'a', 'b', 'c'};
  for (int i = 0; i < 3; i++) { c4bmap_add(&m, ks, vs); }

  int i = 0;

  C4SEQ(c4bmap, &m, seq);
  for (struct c4bmap_it *it; (it = c4seq_next(seq));) {
    assert(it->key == ks+i);
    i++;
  }

  c4bmap_free(&m);
}

static void map_set_tests() {
  C4BMAP(m, int_cmp);
  int ks[3] = {1, 2, 3};
  char vs[3] = {'a', 'b', 'c'};

  for (int i = 0; i < 3; i++) { c4bmap_add(&m, ks+i, vs+i); }
  for (int i = 0; i < 3; i++) { c4bmap_set(&m, ks+i, vs+3-i); }
  for (int i = 0; i < 3; i++) { assert(c4bmap_get(&m, ks+i) == vs+3-i); }
  c4bmap_free(&m);
}

static void map_tests() {
  map_add_tests();
  map_seq_tests();
  map_set_tests();
}

static void mpool_tests() {
  // Define and initialize with default source

  C4MPOOL(mp, NULL);
  
  // Deallocate on scope exit

  C4DEFER({ c4mpool_free(&mp); });

  const int LEN = 10;
  void *ptrs[LEN];
  
  // Allocate memory

  for (int i = 0; i < LEN; i++) {
    ptrs[i] = c4mpool_acquire(&mp, sizeof(int));
  }

  // Remove pointer and deallocate
  
  c4mpool_release(&mp, ptrs[0]);

  // Reallocate pointer
  
  ptrs[1] = c4mpool_require(&mp, ptrs[1], sizeof(long));
}

static void mslab_tests() {
  const int LEN = 10;

  // Define and initialize with specified slab size and default source

  C4MSLAB(ms, sizeof(int) * LEN, NULL);
  
  // Deallocate on scope exit

  C4DEFER({ c4mslab_free(&ms); });
  
  void *prev_ptr = NULL;
  for (int i = 0; i < LEN; i++) {
    // Allocate memory
    void *ptr = c4mslab_acquire(&ms, sizeof(int));

    // Make sure we're using the same block of memory
    
    assert(!prev_ptr || ptr == prev_ptr + sizeof(int));
    
    // Make sure slab offset matches our view of reality

    assert(c4mslab_it(&ms)->offs == sizeof(int) * (i+1));
    prev_ptr = ptr;
  }

  // Trigger allocation of new slab and verify offset
  
  c4mslab_acquire(&ms, 1);
  assert(c4mslab_it(&ms)->offs == 1);
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

static void seq_tests() {
  C4BMAP(bmap, int_cmp);

  // Populate bmap
  
  int keys[3] = {1, 2, 3};
  char vals[3] = {'a', 'b', 'c'};
  for (int i = 0; i < 3; i++) { c4bmap_add(&bmap, keys+i, vals+i); }

  // Define and initialize seq to stack allocated sequence for bmap,
  // same as:
  // 
  // struct c4bmap_seq _seq;
  // struct c4seq *seq = c4bmap_seq(&bmap, &_seq);
  
  C4SEQ(c4bmap, &bmap, seq);

  // Define and initialize val_seq to stack allocated sequence mapping
  // lambda over bmap, NULLs are automatically filtered from the result;
  // same as:
  //
  // struct c4map_seq _val_seq;
  // struct c4seq *val_seq = c4seq_map(seq, C4LAMBDA({
  //   struct c4bmap_it *it = _it;
  //   return (it->key == keys + 1) ? it->val : NULL;
  // }, void *, void *_it), &_val_seq);

  C4SEQ_MAP(seq, {
      struct c4bmap_it *it = _it;
      return (it->key == keys + 1) ? it->val : NULL;
    }, _it, val_seq);
      
  // Loop val_seq and check value

  for (char *val; (val = c4seq_next(val_seq));) {
    assert(val_seq->idx == 1);
    assert(val == vals + 1);
  }
  
  c4bmap_free(&bmap);
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
    dyna_tests();
    err_tests();
    lambda_tests();
    ls_tests();
    map_tests();
    mpool_tests();
    mslab_tests();
    rec_tests();
    seq_tests();
    tbl_tests();

    //C4THROW(&c4err, "test print");
  }

  c4free();
  c4ctx_free(c4ctx());
  return 0;
}
