#ifndef C4REC
#define C4REC

#include "seqs/bmap.h"
#include "uid.h"

struct c4col;

struct c4rec {
  c4uid_t id;
  struct c4bmap flds;
};

struct c4rec *c4rec_init(struct c4rec *self, c4uid_t id);
void c4rec_free(struct c4rec *self);

void *c4rec_get(struct c4rec *self, struct c4col *col);
void c4rec_set(struct c4rec *self, struct c4col *col, void *val);

#endif
