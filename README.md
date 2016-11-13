# libc4life
#### esoteric C essentials

### intro
c4life is a library of ideas and tools that have accumulated over 30 years of writing software for fun. I've found that given a solid foundation, which is what this library is aiming to provide; coding in C is a welcome therapy after seemingly wasting years exploring various ways of pretending the hidden complexity in my stack was someone else's problem.

### status
Any feature documented here can be assumed to be reasonably stable. I'll add more pleasant surprises as soon as I consider them polished enough for a wider audience. I'm always aiming for maximum leverage and symbiosis; simple problems should be trivial to solve, complex as simple as possible.

### setup
In any modern Debian-based distro, this should get you started:

```bash
sudo apt-get install clang cmake uuid-dev
git clone https://github.com/codr4life/libc4life.git
cd libc4life
mkdir build
cd build
cmake ..
make
./tests
sudo make install
```

### init
c4life needs to keep track of internal state for some of it's features. Calling ```c4init()``` before accessing the library is highly recommended, ```c4free()``` deallocates all internal state. A context is allocated internally on demand per thread, calling ```c4ctx_free(c4ctx())``` deallocates the context for the current thread.

### memory
c4life is designed to support and encourage stack allocation wherever possible. Most initializers and finalizers make no assumptions about how the memory pointed to was allocated, and take no responsibility for freeing memory explicitly allocated by user code.

#### allocators
c4life provides a general, extensible allocator interface and a set of stackable implementations that focus on specific aspects of dynamic memory allocation.

#### pool
The pool allocator allows treating sets of separate allocations as single blocks of memory, while retaining the ability to release individual pointers. The data needed for book keeping is prefixed to each allocation and supports O(1) addition and removal without additional allocations.

```C

#include <c4life/c4.h>
#include <c4life/defer.h>
#include <c4life/mem/mpool.h>

void mpool_tests() {
  // Define and initialize with default source

  C4MPOOL(mp, &c4malloc);
  C4DEFER({ c4mpool_free(&mp); });

  const int LEN = 10;
  void *ptrs[LEN];
  
  // Allocate memory

  for (int i = 0; i < LEN; i++) {
    ptrs[i] = c4mpool_acquire(&mp, sizeof(int));
  }

  // Release pointer

  c4mpool_release(&mp, ptrs[0]);

  // Reallocate pointer
  
  ptrs[1] = c4mpool_require(&mp, ptrs[1], sizeof(long));
}

```

#### slab
The slab allocator allocates memory as slabs of user defined size and keeps track of available space within them. Since it doesn't keep any information about individual allocations; the only way to release allocated memory is to free the allocator. It's useful for reducing the number of allocations in code that doesn't need to free individual pointers; or to feed pool allocators.

```C

#include <c4life/c4.h>
#include <c4life/defer.h>
#include <c4life/mem/mslab.h>

void mslab_tests() {
  const int LEN = 10;

  // Define and initialize with specified slab size and default source

  C4MSLAB(ms, sizeof(int) * LEN, &c4malloc);
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

```

#### freelist
The freelist allocator is useful for recycling released pool memory, it reuses the embedded book keeping to track released pointers.

```C

#include <c4life/c4.h>
#include <c4life/defer.h>
#include <c4life/mem/mfreel.h>

void mfreel_tests() {
  // Define and initialize with default source
  
  C4MPOOL(mp, &c4malloc);
  C4MFREEL(mf, &mp);
  C4DEFER({ c4mfreel_free(&mf); c4mpool_free(&mp); });

  const int LEN = 10;
  void *ptrs[LEN];

  for (int i = 0; i < LEN; i++) {
    // Allocate from mpool since we know freelist is empty
    
    ptrs[i] = c4mpool_acquire(&mp, sizeof(int));
  }

  // Release all memory to freelist
  
  for (int i = 0; i < LEN; i++) { c4mfreel_release(&mf, ptrs[i]); }

  for (int i = 0; i < LEN; i++) {
    // Make sure that memory is recycled by freelist
    
    assert(c4mfreel_acquire(&mf, sizeof(int)) == ptrs[i]);
  }
}

```

### lambdas
The ```C4LAMBDA()``` macro defines anonymous nested functions.

```C

#include <c4life/macros.h>

void lambda_tests() {
  assert(C4LAMBDA({ return x*y; }, int, int x, int y)(2, 3) == 6);
}

```

### deferred actions
c4life supports two flavors of defer, both based on cleanup __attributes__; one for current scope and a more elaborate version for deferring to user defined scopes.

```C

#include <c4life/defer.h>

void defer_tests() {
  bool called = false;

  {
    C4DEFER({ called = true; });
    assert(!called);
  }
  
  assert(called);
}

void defer_scope_tests() {
  int called = false;
  
  C4DEFER_SCOPE(outer) {
    C4DEFER_SCOPE(inner) {
      C4DEFER_TO(outer, { called = true; });
    }
    
    assert(!called);
  }

  assert(called);
}

```

### coroutines
c4life provides coroutines in the form of a minimalistic layer of macros inspired by Duff's Device. Anything that should persist across calls needs to be declared static, global or passed as parameters; the only thing the coroutine really cares about is the current line number. Calling a coroutine is the same as calling any other function, all the details are neatly encapsulated inside.

```C

#include <c4life/coro.h>

struct coro_ctx { int i, line; };

int coro(struct coro_ctx *ctx, int foo) {
  C4CORO(&ctx->line);
  for(ctx->i = 1; ctx->i <= 10; ctx->i++) { C4CORO_RET(foo + ctx->i); }
  C4CORO_END();
  
  return -1;
}

void coro_tests() {
  struct coro_ctx ctx = {0, 0};
  for (int i = 1; i <= 10; i++) { assert(coro(&ctx, i) == i*2); }
  assert(coro(&ctx, 0) == -1);
}

```

### sequences
c4life implements several types that provide a sequence of values; embedded lists, dynamic arrays, ordered maps, tables and more. Each of them provide a function in the form of ```struct c4seq *[type]_seq(self, seq)``` to initialize a new sequential view of self. Any memory allocated by the sequence is automatically deallocated when it reaches it's end, or manually by calling ```c4seq_free(seq)```. All operations in the following example are supported by any type that implements the sequence protocol.

```C

#include <c4life/seqs/bmap.h>

void seq_tests() {
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
  // struct c4seq_map _val_seq;
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

```

#### dynamic arrays
c4life provides dynamic arrays with user specified item size; they're implemented as a single block of memory that is grown automatically when needed.

```C

#include <c4life/seqs/dyna.h>

void dyna_tests() {
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


```

#### rolling your own
Hooking into the sequence framework is trivial; you need a struct named ```[type]_seq``` to hold your state and the ```c4seq``` struct; a constructor named ```[type]_seq``` to initialize the sequence; and a function that provides the next value. The framework keeps track of index and eof status.

```C

#include <c4life/seq.h>

struct my {
  // Your type
};

struct my_seq {
  struct c4seq super;

  // Additional state, if needed
};

static void seq_free(struct c4seq *_seq) {
  struct my_seq *seq = C4PTROF(my_seq, super, _seq);

  // Code that frees additional state, if needed
}

static void *seq_next(struct c4seq *_seq) {
  struct my_seq *seq = C4PTROF(my_seq, super, _seq);

  // Code that returns next value, or NULL on eof;
  // _seq->idx contains the current index
}

struct c4seq *my_seq(struct my *self, struct my_seq *seq) {
  c4seq_init(&seq->super);
  seq->super.free = seq_free; // Optional
  seq->super.next = seq_next;

  // Code that inits additional state, if needed

  return &seq->super;
}

```

### errors
c4life adds the ability to throw and catch typed errors out of band. Throwing an error doesn't unwind the stack to make sure someone is there to catch it. Errors are accumulated in the current try scope and propagated on exit; unhandled errors are printed to ```stderr``` on final exit. Catching errors scans the accumulated error queue for the specified type or one of it's sub types. Printing includes a stacktrace with try scopes, file names and line numbers.

#### types
Since c4life keeps track of error types internally, the result of creating new error types from more than one thread at a time is undefined. ```c4free()``` frees all registered types, calling ```c4err_t_free()``` manually unregisters the type.

```C

#include <c4life/err.h>

struct c4err_t custom_type;

void err_tests() {
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

```

### license
MIT

### support
Thanks to a permanently messed up spine and a complete lack of patience with authorities and capitalistic, imperialistic bullshit; I'm mostly unemployed these days. The good news is that it frees a lot of time for thinking, coding and sharing. Any assistance with solving the rest of the equation would be most appreciated.

https://www.paypal.me/codr4life

peace, out<br/>
/Andreas