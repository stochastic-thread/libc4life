#ifndef C4TBL_H
#define C4TBL_H

#include "seqs/bmap.h"
#include "seqs/seq.h"

struct c4rec;

struct c4tbl {
  char *name;
  struct c4bmap recs;
};

struct c4tbl_seq {
  struct c4seq super;
  struct c4bmap_seq recs_seq;
  struct c4rec rec;
};

struct c4tbl *c4tbl_init(struct c4tbl *self, const char *name);
void c4tbl_free(struct c4tbl *self);
struct c4seq *c4tbl_seq(struct c4tbl *self, struct c4tbl_seq *seq);
struct c4rec *c4tbl_upsert(struct c4tbl *self, struct c4rec *rec);

#endif
