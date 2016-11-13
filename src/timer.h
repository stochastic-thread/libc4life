#ifndef C4TIMER_H
#define C4TIMER_H

#include "time.h"

struct c4timer { struct timespec cpu, real; };

long c4timer_cpu(struct c4timer *self);
long c4timer_real(struct c4timer *self);
struct c4timer *c4timer_reset(struct c4timer *self);

#endif
