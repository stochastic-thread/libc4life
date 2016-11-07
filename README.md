# libc4life
#### esoteric C essentials

### intro
c4life is a library of C utilities that has been polished over 30 years of sporadic but intensive adventures in C. Every abstraction has been carefully chiseled to complement the rest for maximum leverage. Writing large scale software in C is not only possible; given wide enough shoulders to stand on, it's therapy compared to pretending the mountain of hidden complexity in your stack is someone else's problem. Pretending isn't healthy; it splits your personality and destroys your self confidence.

### contexts
c4life uses contexts to keep track of global state; for error handling etc. A context lookup function can be supplied on init to control which context is used when and where. For single-threaded use, the following example is a good start:

```C
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
  c4free();
}
```

### license
MIT