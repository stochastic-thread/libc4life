#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "c4.h"
#include "mem/mfreel.h"
#include "mem/mpool.h"
#include "mem/mslab.h"
#include "timer.h"
#include "utils.h"

#define ITERS 1000000
#define MAX_SIZE 10000

#define _BENCHMARK(name, malloc, _t)			\
  struct c4timer _t;					\
  c4timer_reset(&_t);					\
  C4TIMER_RUN(&_t) { run(malloc); }			\
  printf("%-20s %15" PRIu64 " %15" PRIu64 "\n",	\
	 name,_t.cpu, _t.real)				\

#define BENCHMARK(name, malloc)			\
  _BENCHMARK((name), malloc, C4GSYM(t))		\

static void run(struct c4malloc *m) {
  for (int i = 0; i < ITERS; i++) {
    void *ptr = c4malloc_acquire(m, (int)(c4rnd()*MAX_SIZE));
    c4malloc_release(m, ptr);
  }
}

#define SLAB_SIZE MAX_SIZE * 1000

void malloc_perf_tests() {
  printf("%-20s %15s %15s\n", "name", "cpu", "real");

  // --- basic

  BENCHMARK("basic", &c4malloc);

  // --- slab

  {
    C4MSLAB(slab, SLAB_SIZE, &c4malloc);

    BENCHMARK("slab", &slab.malloc);

    c4mslab_free(&slab);
  }

  // --- pool

  {
    C4MPOOL(pool, &c4malloc);

    BENCHMARK("pool", &pool.malloc);

    c4mpool_free(&pool);
  }
    
  // --- pool/slab

  {
    C4MSLAB(slab, SLAB_SIZE, &c4malloc);
    C4MPOOL(pool, &slab.malloc);

    BENCHMARK("pool/slab", &pool.malloc);

    c4mpool_free(&pool);
    c4mslab_free(&slab);
  }
  
  // --- freel
  
  {
    C4MPOOL(pool, &c4malloc);
    C4MFREEL(freel, &pool);

    BENCHMARK("freel", &freel.malloc);

    c4mfreel_free(&freel);
    c4mpool_free(&pool);
  }

  // --- freel/slab

  {
    C4MSLAB(slab, SLAB_SIZE, &c4malloc);
    C4MPOOL(pool, &slab.malloc);
    C4MFREEL(freel, &pool);

    BENCHMARK("freel/slab", &freel.malloc);

    c4mfreel_free(&freel);
    c4mpool_free(&pool);
    c4mslab_free(&slab);
  }
}
