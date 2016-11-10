#include "uid.h"

void c4uid_copy(c4uid_t dest, c4uid_t src) { uuid_copy(dest, src); }

void c4uid_init(c4uid_t id) { uuid_generate_random(id); }
