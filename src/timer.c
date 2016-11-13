#include <stdarg.h>
#include <stdio.h>
#include "timer.h"

static long msecs(struct timespec *l, struct timespec *r) {
  return (r->tv_sec - l->tv_sec) * 1000000 + 
    (r->tv_nsec - l->tv_nsec) / 1000; 
}

struct c4timer *c4timer_reset(struct c4timer *self) {
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &self->cpu);
  clock_gettime(CLOCK_REALTIME, &self->real);
  return self;
}

long c4timer_cpu(struct c4timer *self) {
  struct timespec now;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
  return msecs(&self->cpu, &now);
}

long c4timer_real(struct c4timer *self) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  return msecs(&self->real, &now);
}
