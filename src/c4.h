#ifndef C4LIFE
#define C4LIFE

struct c4ctx;
struct c4err_t;

extern struct c4err_t c4err;

struct c4ctx *c4ctx();
void c4init();
void c4free();

#endif
