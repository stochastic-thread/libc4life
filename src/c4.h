#ifndef C4LIFE
#define C4LIFE

struct c4ctx;

typedef struct c4ctx *(*c4get_ctx_t)();

struct c4ctx *c4ctx(c4get_ctx_t get_ctx);

#endif
