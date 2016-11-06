# libc4life
#### an integrated framework of esoteric C essentials

### contexts
c4life uses contexts for keeping track of global state; error handling etc. A context lookup function can be supplied on init to control which context is used where. For single-threaded use, the following example is a good start:

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