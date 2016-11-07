# libc4life
#### esoteric C essentials

### intro
c4life is a library of C utilities that has been polished over 30 years of hacking C for fun. I've found that given experience and a solid foundation that plays on C's strengths, coding in C is therapy compared to pretending the mountain of hidden complexity in your stack is someone else's problem. Pretending isn't healthy; it splits your personality and destroys your self confidence.

### contexts
c4life uses contexts to keep track of global state; for error handling etc. A context lookup function may be specified on init to control which context is used when and where. For single-threaded use, the following example is a good start:

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
c4life's error handling facility is designed to complement prevailing strategies rather than replacing them. What's missing in C is a way to pass information describing the error out of band to the code that needs it. No one really likes automagic stack unwinding, even forcing it down peoples throats like Java didn't work; and the fact that it's doable in C has been proven enough times. Throwing an error in c4life doesn't stop the world to make sure someone is there to catch it. Errors are accumulated in the current try scope and propagated on exit; unhandled errors are printed to ```stderr``` on final scope exit.

```C
#include <c4life/c4.h>
#include <c4life/err.h>

void err_tests() {
  C4TRY("outer") {
    struct c4err *err = NULL;
    const char *data = "guru meditation"; // data describing error

    C4TRY("inner") {
      err = C4THROW(&c4err, "test throw", data); // throw basic c4err type
      assert(err->data == data);
    }

    bool caught = false;
    
    C4CATCH(e, NULL) { // NULL catches everything
      assert(e == err);
      c4err_free(e); // free to handle err
      caught = true;
    }

    assert(caught);
  }
}

```

### license
MIT