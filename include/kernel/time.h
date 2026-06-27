#ifndef _INCLUDE_TIME_
#define _INCLUDE_TIME_

#include <ant/types.h>

typedef s64 time_t;
typedef s64 suseconds_t;
typedef s64 useconds_t;

struct timeval
{
  time_t      tv_sec;
  suseconds_t tv_usec;
};

#endif
