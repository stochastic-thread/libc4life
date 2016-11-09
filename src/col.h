#ifndef C4LIFE_COL
#define C4LIFE_COL

struct c4val_t;

struct c4col {
  char *name;
  struct c4val_t *type;
};

struct c4col *c4col_init(struct c4col *self,
			 const char *name,
			 struct c4val_t *type);

void c4col_free(struct c4col *self);

#endif
