#ifndef C4LIFE_COL
#define C4LIFE_COL

struct c4col_t {
  char *name;
};

struct c4col_t *c4col_t_init(struct c4col_t *self, const char *name);
void c4col_t_free(struct c4col_t *self);

struct c4col {
  char *name;
  struct c4col_t *type;
};

struct c4col *c4col_init(struct c4col *self,
			 const char *name,
			 struct c4col_t *type);

void c4col_free(struct c4col *self);

#endif
