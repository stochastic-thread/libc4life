#include <stdlib.h>
#include "c4.h"
#include "ctx.h"
#include "mem/malloc.h"
#include "utils.h"

void *c4acquire(size_t size) { return c4malloc_acquire(c4ctx()->malloc, size); }

void c4release(void *ptr) { c4malloc_release(c4ctx()->malloc, ptr); }

double c4rnd() { return rand() / (double)RAND_MAX; }
