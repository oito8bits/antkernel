#ifndef _INCLUDE_MAP_
#define _INCLUDE_MAP_

#include <ant/types.h>

void *map_mmap(void *, size_t, int, int, int, int);
int map_munmap(void *, size_t);

#endif
