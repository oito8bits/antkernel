#ifndef _INCLUDE_BITSET_
#define _INCLUDE_BITSET_

#include <ant/types.h>

u64 bitset_is_set(u64 *bitset, size_t pos);
void bitset_set(u64 *bitset, size_t pos);
void bitset_reset(u64 *bitset, size_t pos);

#endif
