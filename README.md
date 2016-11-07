# libc4life
#### esoteric C essentials

### intro
c4life is a library of C utilities that has been polished over 30 years of coding in C for fun. Given a solid foundation that plays on C's strengths, coding in C is therapy compared to pretending the mountain of hidden complexity in your stack is someone else's problem. Pretending isn't healthy; it splits your personality and destroys your self confidence.

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