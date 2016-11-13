#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "col.h"
#include "coro.h"
#include "macros.h"
#include "rec.h"
#include "tbl.h"
#include "uid.h"

struct c4tbl *c4tbl_init(struct c4tbl *self, const char *name) {
  self->name = strdup(name);
  c4bmap_init(&self->recs, c4uids_cmp);
  return self;
}

void c4tbl_free(struct c4tbl *self) {
  free(self->name);

  C4SEQ(c4bmap, &self->recs, rec_seq);
  for (struct c4bmap_it *e; (e = c4seq_next(rec_seq));) {
    c4bmap_free(e->val);
    free(e->val);
  }

  c4bmap_free(&self->recs);
}

static void seq_free(struct c4seq *_seq) {
  struct c4tbl_seq *seq = C4PTROF(c4tbl_seq, seq, _seq);
  c4rec_free(&seq->rec);
}

static void *seq_next(struct c4seq *_seq) {
  struct c4tbl_seq *seq = C4PTROF(c4tbl_seq, seq, _seq);
  struct c4seq *recs_seq = &seq->recs_seq.seq;
  struct c4bmap_it *it;

  if (!(it = c4seq_next(recs_seq))) { return NULL; }

  if (_seq->idx) { c4uid_copy(seq->rec.id, it->key); }   
  else { c4rec_init(&seq->rec, it->key); }
  c4bmap_clear(&seq->rec.flds);
  c4bmap_merge(&seq->rec.flds, it->val);
  return &seq->rec;
}

struct c4seq *c4tbl_seq(struct c4tbl *self, struct c4tbl_seq *seq) {
  c4seq_init(&seq->seq);
  seq->seq.free = seq_free;
  seq->seq.next = seq_next;
  c4rec_init(&seq->rec, NULL);
  c4bmap_seq(&self->recs, &seq->recs_seq);
  return &seq->seq;
}

struct c4rec *c4tbl_upsert(struct c4tbl *self, struct c4rec *rec) {
  size_t idx;
  struct c4bmap_it *it = c4bmap_find(&self->recs, rec->id, 0, &idx);
  struct c4bmap *flds = NULL;
  
  if (it) {
    flds = it->val;
    c4bmap_clear(flds);
  } else {
    flds = c4bmap_init(malloc(sizeof(struct c4bmap)), c4cols_cmp);
    c4bmap_insert(&self->recs, idx, rec->id, flds);
  }

  c4bmap_merge(flds, &rec->flds);
  return rec;
}
