#ifndef C4LIFE_UID
#define C4LIFE_UID

#include <uuid/uuid.h>

typedef uuid_t c4uid_t;

void c4uid_copy(c4uid_t dest, c4uid_t src);
void c4uid_init(c4uid_t id);

#endif
