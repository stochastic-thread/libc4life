#ifndef C4LIFE_CORO
#define C4LIFE_CORO

#define C4CORO(line)						\
  { int *_c4coro_line = line;					\
    switch(*_c4coro_line) {					\
    case 0:;							\
    
#define C4CORO_END()					\
  *_c4coro_line = 0;					\
  break;						\
 default: assert(false);				\
  }}							\
  
#define C4CORO_RET(...)				\
  do {						\
    *_c4coro_line =  __LINE__;			\
    return __VA_ARGS__;				\
  case __LINE__:;				\
  } while (0);					\
  
#endif
