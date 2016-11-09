#ifndef C4LIFE_VAL
#define C4LIFE_VAL

#include "ls.h"

struct c4val_t {
  char *name;
  struct c4ls ts_node;
};

extern struct c4val_t c4str;

struct c4ls *c4val_ts();

struct c4val_t *c4val_t_init(struct c4val_t *self, const char *name);
void c4val_t_free(struct c4val_t *self);

#endif
