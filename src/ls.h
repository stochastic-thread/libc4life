#ifndef C4LIFE_LS
#define C4LIFE_LS

struct c4ls {
  struct c4ls *next, *prev;
};

struct c4ls *c4ls_init(struct c4ls *self);
struct c4ls *c4ls_append(struct c4ls *self, struct c4ls *next);
void c4ls_delete(struct c4ls *self);
struct c4ls *c4ls_prepend(struct c4ls *self, struct c4ls *prev);
void c4ls_splice(struct c4ls *self, struct c4ls *first, struct c4ls *last);

#endif
