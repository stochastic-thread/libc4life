#ifndef C4LIFE_SEQ
#define C4LIFE_SEQ

#include <stdbool.h>

#define _C4DO(type, model, var, _tseq, _seq)			\
  struct type _tseq;						\
  struct c4seq *_seq = type(model, &_tseq);			\
  C4DO_SEQ(_seq, var)						\
    
#define C4DO(type, model, var)						\
  _C4DO(CONCAT(type, _seq), model, var, UNIQUE(tseq), UNIQUE(seq))	\

#define C4DO_SEQ(seq, var)					\
  for (void *var = c4seq_next(seq);				\
       !seq->eof;						\
       var = c4seq_next(seq))					\
    
#define _C4SEQ(type, model, var, _tseq)			\
  struct type _tseq;					\
  struct c4seq *var = type(model, &_tseq);		\

#define C4SEQ(type, model, var)					\
  _C4SEQ(CONCAT(type, _seq), model, var, UNIQUE(tseq))		\

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
