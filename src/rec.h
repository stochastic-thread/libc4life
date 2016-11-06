#ifndef C4LIFE_REC
#define C4LIFE_REC

#include "map.h"

struct c4rec {
  struct c4map flds;
};

struct c4rec *c4rec_init(struct c4rec *self);
  
#endif
