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

### lambdas
The ```C4LAMBDA()``` macro defines anonymous nested functions.

```C

#include <c4life/macros.h>

void lambda_tests() {
  assert(C4LAMBDA({ return x*y; }, int, int x, int y)(2, 3) == 6);
}

```

### deferred actions
c4life supports deferring actions until scope exit using the ```C4DEFER()``` macro.

```C

#include <c4life/macros.h>

void defer_tests() {
  bool called = false;

  {
    C4DEFER({ called = true; });
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
c4life implements several types that provide a sequence of values; embedded lists, dynamic arrays, ordered maps, tables and more. Each of them provide a function in the form of ```struct c4seq *[type]_seq(self, seq)``` to initialize a new sequential view of self. Any memory allocated by the sequence is automatically deallocated when it reaches it's end, or manually by calling ```c4seq_free(seq)```. All operations in the following example are supported by any type implementing the sequence protocol.

```C

#include <c4life/seqs/bmap.h>

void seq_tests() {
  C4BMAP(bmap, int_cmp);

  // Populate bmap
  
  int keys[3] = {1, 2, 3};
  char vals[3] = {'a', 'b', 'c'};
  for (int i = 0; i < 3; i++) { c4bmap_add(&bmap, keys+i, vals+i); }

  // Loop anonymous sequence for bmap
  
  int *key = keys;
  C4DO(c4bmap, &bmap, _e) {
    struct c4bmap_it *e = _e;
    assert(e->key == key);
    key++;
  }

  // Define and initialize seq to point to new sequence for bmap
  
  C4SEQ(c4bmap, &bmap, seq);

  // Assign lazy sequence mapping lambda over bmap to val_seq,
  // NULLs are automatically filtered from the result.
  // Last parameter is out sequence, a new one is heap allocated
  // and automatically deallocated if not specified.

  struct c4seq *val_seq =
    c4seq_map(seq,
	      C4LAMBDA({
		  struct c4bmap_it *e = _e;
		  return (e->key == keys + 1) ? e->val : NULL;
		}, void *, void *_e),
	      NULL);
  
  // Loop over val_seq and check we got the right value
  
  C4DO_SEQ(val_seq, val) {
    assert(val_seq->idx == 1);
    assert(val == vals + 1);
  }
  
  c4bmap_free(&bmap);
}

```

#### rolling your own
Hooking into the sequence framework is trivial; you need a struct named ```[type]_seq``` to hold your state and the ```c4seq``` struct; a constructor named ```[type]_seq``` to initialize the sequence; and a function that provides the next value. The framework keeps track of the index and eof status.

```C

#include <c4life/seq.h>

struct my_seq {
  struct c4seq super;

  // Additional state, if needed
};

static void seq_free(struct c4seq *_seq) {
  struct c4map_seq *seq = C4PTROF(my_seq, super, _seq);

  // Code that frees additional state, if needed
}

static void *seq_next(struct c4seq *_seq) {
  struct c4map_seq *seq = C4PTROF(my_seq, super, _seq);

  // Code that returns next value, or NULL on eof;
  // _seq->idx contains the current index.
}

struct c4seq *c4map_seq(struct c4map *self, struct c4map_seq *seq) {
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