#ifndef C4LIFE_SEQ
#define C4LIFE_SEQ

#include <stdbool.h>

#define _C4DO(type, owner, var, _seq)				\
  _C4SEQ(type, owner, _seq, UNIQUE(tseq));			\
  for (void *var = c4seq_next(_seq);				\
       !_seq->eof;						\
       var = c4seq_next(_seq))					\
    
#define C4DO(type, owner, var)					\
  _C4DO(CONCAT(type, _seq), owner, var, UNIQUE(seq))	\
  
#define _C4SEQ(type, owner, var, _tseq)			\
  struct type _tseq;					\
  struct c4seq *var = type(owner, &_tseq);		\

#define C4SEQ(type, owner, var)					\
  _C4SEQ(CONCAT(type, _seq), owner, var, UNIQUE(tseq))		\

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
