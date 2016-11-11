#ifndef C4LIFE_REC
#define C4LIFE_REC

#include "seqs/map.h"
#include "uid.h"

struct c4col;

struct c4rec {
  c4uid_t id;
  struct c4map flds;
};

struct c4rec *c4rec_init(struct c4rec *self, c4uid_t id);
void c4rec_free(struct c4rec *self);

void *c4rec_get(struct c4rec *self, struct c4col *col);
void c4rec_set(struct c4rec *self, struct c4col *col, void *val);

#endif
