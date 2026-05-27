/*
 * This file contains helper template functions
 **/
#include <cstring>

template <typename Dst, typename Src> Dst typePunning(const Src &rs) {
  static_assert(sizeof(Dst) == sizeof(Src), "Types must be of equal size");

  Dst rd;
  std::memcpy(&rd, &rs, sizeof(rd));

  return rd;
}
