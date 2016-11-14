#ifndef C4COL_H
#define C4COL_H

struct c4val_t;

struct c4col {
  char *name;
  struct c4val_t *type;
};

int c4cols_cmp(void *_x, void *_y, void *data);

struct c4col *c4col_init(struct c4col *self,
			 const char *name,
			 struct c4val_t *type);

void c4col_free(struct c4col *self);

#endif
