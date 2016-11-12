#ifndef C4SEQ_H
#define C4SEQ_H

#include <stdbool.h>

#define _C4SEQ(type, model, var, _tseq)		\
  struct type _tseq;				\
  struct c4seq *var = type(model, &_tseq);	\

#define C4SEQ(type, model, var)				\
  _C4SEQ(C4SYMS(type, _seq), model, var, C4GSYM(tseq))	\

#define _C4SEQ_MAP(seq, fn, it, out, _out)				\
  struct c4seq_map _out;						\
  struct c4seq *out =							\
    c4seq_map(seq, C4LAMBDA(fn, void *, void *it), &_out);		\
  
#define C4SEQ_MAP(seq, fn, it, out)		\
  _C4SEQ_MAP(seq, fn, it, out, C4GSYM(out))	\
  
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
struct c4seq *c4seq_map(struct c4seq *self, c4seq_map_fnt fn,
			struct c4seq_map *out);
void *c4seq_next(struct c4seq *self);

#endif
