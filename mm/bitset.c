#include <mm/bitset.h>

static u64 calculate_bit_pos(size_t pos)
{
  return 1UL << pos % BITS_PER_ENTRY;
}

u64 bitset_is_set(u64 *bitset, size_t pos)
{
  return bitset[pos / BITS_PER_ENTRY] & calculate_bit_pos(pos);
}

void bitset_set(u64 *bitset, size_t pos)
{
  bitset[pos / BITS_PER_ENTRY] |= calculate_bit_pos(pos); 
}

void bitset_reset(u64 *bitset, size_t pos)
{
  bitset[pos / BITS_PER_ENTRY] &= ~calculate_bit_pos(pos);
}
