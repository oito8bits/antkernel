#include <mm/bitset.h>

static u64 calculate_bit_pos(size_t pos)
{
  return 1U << pos % 64;
}

u64 bitset_is_set(u64 *bitset, size_t pos)
{
  return bitset[pos / 64] & calculate_bit_pos(pos);
}

void bitset_set(u64 *bitset, size_t pos)
{
  bitset[pos / 64] |= calculate_bit_pos(pos); 
}

void bitset_reset(u64 *bitset, size_t pos)
{
  bitset[pos / 64] &= ~calculate_bit_pos(pos);
}
