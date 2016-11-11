#ifndef C4LIFE_TBL
#define C4LIFE_TBL

#include "seqs/map.h"

struct c4rec;

struct c4tbl {
  char *name;
  struct c4map recs;
};

struct c4tbl_seq {
  int line;
  struct c4map_seq recs_seq;
  struct c4rec rec;
};

struct c4tbl *c4tbl_init(struct c4tbl *self, const char *name);
void c4tbl_free(struct c4tbl *self);

struct c4tbl_seq *c4tbl_seq(struct c4tbl *self, struct c4tbl_seq *seq);
void c4tbl_seq_free(struct c4tbl_seq *seq);
struct c4rec *c4tbl_seq_next(struct c4tbl_seq *seq);

struct c4rec *c4tbl_upsert(struct c4tbl *self, struct c4rec *rec);

#endif
