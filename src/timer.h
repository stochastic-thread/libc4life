#ifndef C4TIMER_H
#define C4TIMER_H

#include <stdbool.h>
#include <stdint.h>
#include "time.h"

#define _C4TIMER_RUN(self, _cpu, _real, _done)			\
  struct timespec _cpu[2], _real[2];				\
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_cpu[0]);		\
  clock_gettime(CLOCK_REALTIME, &_real[0]);			\
  bool _done = false;						\
								\
  for (; !_done;						\
       clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_cpu[1]),	\
	 clock_gettime(CLOCK_REALTIME, &_real[1]),		\
	 self->cpu += c4nsecs(&_cpu[0], &_cpu[1]),		\
	 self->real += c4nsecs(&_real[0], &_real[1]),		\
	 _done = true)						\

#define C4TIMER_RUN(self)					\
  _C4TIMER_RUN((self), C4GSYM(cpu), C4GSYM(real), C4GSYM(done))	\

uint64_t c4nsecs(struct timespec *from, struct timespec *to);

struct c4timer { uint64_t cpu, real; };

struct c4timer *c4timer_reset(struct c4timer *self);

#endif
