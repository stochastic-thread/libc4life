#ifndef C4LIFE_SEQ
#define C4LIFE_SEQ

#include <stdbool.h>

#define _C4SEQ(var, type, owner, _tseq)			\
  struct type _tseq;					\
  struct c4seq *var = type(owner, &_tseq);		\

#define C4SEQ(var, type, owner)					\
  _C4SEQ(var, type, owner, UNIQUE(tseq))			\

struct c4seq {
  bool eof;
  size_t idx;
  void (*free)(struct c4seq *);
  void *(*next)(struct c4seq *);
};

struct c4seq *c4seq_init(struct c4seq *self);
void *c4seq_next(struct c4seq *self);
void c4seq_free(struct c4seq *self);

#endif
