#ifndef C4LIFE_STR_COL
#define C4LIFE_STR_COL

#include "db/col.h"

struct c4rec;

struct c4str_col {
  struct c4col super;
};

struct c4str_col *c4str_col_init(struct c4str_col *self, const char *name);
void c4str_col_free(struct c4str_col *self);

const char *c4get_str(struct c4rec *rec, struct c4str_col *col);
void c4set_str(struct c4rec *rec, struct c4str_col *col, const char *val);
		
#endif
