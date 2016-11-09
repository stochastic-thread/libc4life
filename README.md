# libc4life
#### esoteric C essentials

### intro
c4life is a library of C utilities that has been polished over 30 years of writing software for fun on a daily basis. I've found that given experience and a solid foundation, coding in C is therapy compared to pretending the mountain of hidden complexity in your stack is someone else's problem. Pretending isn't healthy; it splits the personality and destroys self confidence.

### stack allocation
c4life is designed to support and encourage stack allocation wherever possible. Most initializers and finalizers make no assumptions about how the memory pointed to was allocated, and take no responsibility for freeing memory explicitly allocated by user code.

### contexts
c4life uses contexts to keep track of global state; error handling etc. A context lookup function may be specified on init to control which context is used when and where. For single-threaded use, the following example is a good start:

```C
#include <c4life/c4.h>
#include <c4life/ctx.h>

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
  //...
  c4ctx_free(ctx());
  c4free();
  return 0;
}
```

### errors
c4life adds the ability to throw and catch typed errors out of band. Throwing an error doesn't unwind the stack to make sure someone is there to catch it. Errors are accumulated in the current try scope and propagated on exit; unhandled errors are printed to ```stderr``` on final exit. Catching errors scans the accumulated error queue for the specified type or one of it's super types. Printing includes a stacktrace with try scopes, file names and line numbers.

#### types
Since c4life keeps track of error types internally, the result of creating new error types from more than one thread at a time is undefined. ```c4free()``` frees all registered types, calling ```c4err_t_free()``` unregisters the type.

```C

struct c4err_t custom_type;

void err_tests() {
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

```

### license
MIT