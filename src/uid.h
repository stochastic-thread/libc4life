#ifndef C4UID_H
#define C4UID_H

#include <uuid/uuid.h>

typedef uuid_t c4uid_t;

int c4uids_cmp(void *x, void *y, void *data);
void c4uid_init(c4uid_t id);
int c4uid_cmp(c4uid_t x, c4uid_t y);
void c4uid_copy(c4uid_t dest, c4uid_t src);

#endif
