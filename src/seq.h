#ifndef C4LIFE_SEQ
#define C4LIFE_SEQ

struct c4seq {
  void (*free)(struct c4seq *);
  void *(*next)(struct c4seq *);
};

struct c4seq *c4seq_init(struct c4seq *self);
void *c4seq_next(struct c4seq *self);
void s4seq_free(struct c4seq *self);

#endif
