#ifndef C4LIFE
#define C4LIFE

struct c4ctx;
struct c4err_t;

typedef struct c4ctx *(*c4get_ctx_t)();

extern struct c4err_t C4Err;

struct c4ctx *c4ctx();
void c4init(c4get_ctx_t get_ctx);

#endif
