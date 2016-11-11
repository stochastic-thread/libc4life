#ifndef C4LS_H
#define C4LS_H

#include "macros.h"

#define _C4LS_DO(ls, var, _root, _next)					\
  for (struct c4ls *_root = ls, *var = _root->next, *_next = var->next; \
       var != _root;							\
       var = _next, _next = var->next)					\
    
#define C4LS_DO(ls, var)			\
  _C4LS_DO(ls, var, C4GSYM(root), C4GSYM(next))	\

struct c4ls {
  struct c4ls *next, *prev;
};

struct c4ls *c4ls_init(struct c4ls *self);
struct c4ls *c4ls_append(struct c4ls *self, struct c4ls *next);
void c4ls_delete(struct c4ls *self);
struct c4ls *c4ls_prepend(struct c4ls *self, struct c4ls *prev);
void c4ls_splice(struct c4ls *self, struct c4ls *first, struct c4ls *last);

#endif
