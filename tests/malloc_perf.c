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

#define MIN 10
#define MAX 1000000

#define _BENCHMARK(name, slab_size, code, _t)		\
  struct c4timer _t;					\
  c4timer_reset(&_t);					\
  C4TIMER_RUN(&_t) code;				\
  fprintf(out, "%s\t%zu\t%" PRIu64 "\t%" PRIu64 "\n",	\
	  name, slab_size, _t.cpu, _t.real);		\
  printf("%s\t%zu\t%" PRIu64 "\t%" PRIu64 "\n",		\
	 name, slab_size, _t.cpu, _t.real)		\

#define BENCHMARK(name, slab_size, code)	\
  _BENCHMARK(name, slab_size, code, C4GSYM(t))	\

static void run(struct c4malloc *m, size_t cnt, size_t size) {
  for (int i = 0; i < cnt; i++) {
    void *ptr = c4malloc_acquire(m, (int)(c4rnd()*size));
    c4malloc_release(m, ptr);
  }
}

static void run_malloc(struct c4malloc *m, size_t max_size) {
  for (size_t cnt = MIN; cnt < MAX; cnt *= 10) {
    for (size_t size = MIN; size < max_size; size *= 10) {
      run(m, cnt, size);
    }
  }
}

void malloc_perf_tests() {
  FILE *out = fopen("malloc_perf.txt", "w");
  assert(out);
  
  for (size_t slab_size = MIN; slab_size < MAX; slab_size *= 10) {
    BENCHMARK("basic", slab_size, {
	run_malloc(&c4malloc, slab_size);
      });
    
    BENCHMARK("slab", slab_size, {
	C4MSLAB(slab, slab_size, &c4malloc);
	run_malloc(&slab.malloc, slab_size);
	c4mslab_free(&slab);
      });

    BENCHMARK("pool", slab_size, {
    	C4MPOOL(pool, &c4malloc);
	run_malloc(&pool.malloc, slab_size);
	c4mpool_free(&pool);
      });

    BENCHMARK("pool/slab", slab_size, {
	C4MSLAB(slab, slab_size, &c4malloc);
	C4MPOOL(pool, &slab.malloc);
	run_malloc(&pool.malloc, slab_size);
	c4mpool_free(&pool);
	c4mslab_free(&slab);
      });

    BENCHMARK("freel", slab_size, {
	C4MPOOL(pool, &c4malloc);
	C4MFREEL(freel, &pool);
	run_malloc(&freel.malloc, slab_size);
	c4mfreel_free(&freel);
	c4mpool_free(&pool);
      });

    BENCHMARK("freel/slab", slab_size, {
	C4MSLAB(slab, slab_size, &c4malloc);
	C4MPOOL(pool, &slab.malloc);
	C4MFREEL(freel, &pool);
	run_malloc(&freel.malloc, slab_size);
	c4mfreel_free(&freel);
	c4mpool_free(&pool);
	c4mslab_free(&slab);
      });
  }

  fclose(out);
}
