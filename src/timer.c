#include "timer.h"

uint64_t c4nsecs(struct timespec *from, struct timespec *to) {
  return (to->tv_sec - from->tv_sec) * 1000000000 + 
    (to->tv_nsec - from->tv_nsec); 
}

struct c4timer *c4timer_reset(struct c4timer *self) {
  self->cpu = self->real = 0;
  return self;
}
