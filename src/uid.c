#include "uid.h"

int c4uids_cmp(void *x, void *y, void *data) {
  return uuid_compare(x, y);
}

void c4uid_init(c4uid_t id) { uuid_generate_random(id); }

int c4uid_cmp(c4uid_t x, c4uid_t y) { return uuid_compare(x, y); }

void c4uid_copy(c4uid_t dest, c4uid_t src) { uuid_copy(dest, src); }
