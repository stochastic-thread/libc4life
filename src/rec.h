#ifndef C4LIFE_REC
#define C4LIFE_REC

#include <uuid/uuid.h>
#include "map.h"

struct c4col;

typedef uuid_t c4rec_id_t;

struct c4rec {
  c4rec_id_t id;
  struct c4map flds;
};

void c4rec_id_copy(c4rec_id_t dest, c4rec_id_t src);
void c4rec_id_init(c4rec_id_t id);

struct c4rec *c4rec_init(struct c4rec *self, c4rec_id_t id);
void c4rec_free(struct c4rec *self);

void *c4rec_get(struct c4rec *self, struct c4col *col);
void c4rec_set(struct c4rec *self, struct c4col *col, void *val);

#endif
