#ifndef C4LIFE_SEQ
#define C4LIFE_SEQ

#include <stdbool.h>

#define _C4DO(type, model, var, _tseq, _seq)	\
  struct type _tseq;				\
  struct c4seq *_seq = type(model, &_tseq);	\
  C4DO_SEQ(_seq, var)				\
    
#define C4DO(type, model, var)						\
  _C4DO(C4SYMS(type, _seq), model, var, C4GSYM(tseq), C4GSYM(seq))	\

#define C4DO_SEQ(seq, var)			\
  for (void *var; (var = c4seq_next(seq));)	\

#define _C4SEQ(type, model, var, _tseq)		\
  struct type _tseq;				\
  struct c4seq *var = type(model, &_tseq);	\

#define C4SEQ(type, model, var)				\
  _C4SEQ(C4SYMS(type, _seq), model, var, C4GSYM(tseq))	\

struct c4seq {
  bool eof;
  size_t idx;
  void (*free)(struct c4seq *);
  void *(*next)(struct c4seq *);
};

typedef void *(*c4seq_map_fnt)(void *);

struct c4seq_map {
  struct c4seq super;
  c4seq_map_fnt fn;
  struct c4seq *src;
};

struct c4seq *c4seq_init(struct c4seq *self);
void c4seq_free(struct c4seq *self);

struct c4seq *c4seq_map(struct c4seq *self,
			c4seq_map_fnt fn,
			struct c4seq_map *out);
void *c4seq_next(struct c4seq *self);

#endif
