#ifndef C4LIFE_TBL
#define C4LIFE_TBL

#include "map.h"

struct c4tbl {
  struct c4map recs;
};

struct c4tbl *c4tbl_init(struct c4tbl *self);
  
#endif
