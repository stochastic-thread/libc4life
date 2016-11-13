#include <stdlib.h>
#include <stdio.h>
#include "c4.h"
#include "mem/mfreel.h"
#include "mem/mpool.h"
#include "mem/mslab.h"
#include "timer.h"

#define MIN 10
#define MAX 1000000

static void run(struct c4malloc *m, size_t cnt, size_t size) {
  void **ptrs = malloc(cnt * sizeof(void *));
    
  for (int i = 0; i < cnt; i++) {
    void *ptr = c4malloc_acquire(m, size);

    if (i > cnt / 10) {
      c4malloc_release(m, c4malloc_require(m, ptr, size));
    }
  }

  free(ptrs);
}

static void run_malloc(struct c4malloc *m, size_t max_size) {
  for (size_t cnt = MIN; cnt < MAX; cnt *= 10) {
    for (size_t size = MIN; size < max_size; size *= 10) {
      run(m, cnt, size);
    }
  }
}

void malloc_perf_tests() {
  struct c4timer t;
  
  for (size_t it_size = MIN; it_size < MAX; it_size *= 10) {
    {
      c4timer_reset(&t);
      run_malloc(&c4malloc, it_size);
      printf("malloc %zu:\t%ld\t%ld\n",
	     it_size, c4timer_cpu(&t), c4timer_real(&t));
    }
    
    {
      c4timer_reset(&t);
      C4MSLAB(slab, it_size, &c4malloc);
      run_malloc(&slab.malloc, it_size);
      c4mslab_free(&slab);
      printf("slab %zu:\t%ld\t%ld\n",
	     it_size, c4timer_cpu(&t), c4timer_real(&t));
    }
    
    {
      c4timer_reset(&t);
      C4MPOOL(pool, &c4malloc);
      run_malloc(&pool.malloc, it_size);
      c4mpool_free(&pool);
      printf("pool %zu:\t%ld\t%ld\n",
	     it_size, c4timer_cpu(&t), c4timer_real(&t));
    }

    {
      c4timer_reset(&t);
      C4MSLAB(slab, it_size, &c4malloc);
      C4MPOOL(pool, &slab.malloc);
      run_malloc(&pool.malloc, it_size);
      c4mpool_free(&pool);
      c4mslab_free(&slab);
      printf("pools %zu:\t%ld\t%ld\n",
	     it_size, c4timer_cpu(&t), c4timer_real(&t));
    }

    {
      c4timer_reset(&t);
      C4MPOOL(pool, &c4malloc);
      C4MFREEL(freel, &pool);
      run_malloc(&freel.malloc, it_size);
      c4mfreel_free(&freel);
      c4mpool_free(&pool);
      printf("freel %zu:\t%ld\t%ld\n",
	     it_size, c4timer_cpu(&t), c4timer_real(&t));
    }

    {
      c4timer_reset(&t);
      C4MSLAB(slab, it_size, &c4malloc);
      C4MPOOL(pool, &slab.malloc);
      C4MFREEL(freel, &pool);
      run_malloc(&freel.malloc, it_size);
      c4mfreel_free(&freel);
      c4mpool_free(&pool);
      c4mslab_free(&slab);
      printf("freels %zu:\t%ld\t%ld\n",
	     it_size, c4timer_cpu(&t), c4timer_real(&t));
    }

    printf("---\n");
  }
}
