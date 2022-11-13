#pragma once

#include "./utils.hpp"

#include <array>
#include <cassert>

using std::array;

namespace Chess {

inline constexpr Bitboard sqbb(Square sq) { return 1ull << sq; };

constexpr bool more_than_one(Bitboard b) { return b & (b - 1); }

// WARNING: the following three functions work only with GCC, Clang or ICC
inline Square lsb(Bitboard b) {
   assert(b);
   return Square(__builtin_ctzll(b));
}
inline Square msb(Bitboard b) {
   assert(b);
   return Square(63 ^ __builtin_clzll(b));
}
inline Square pop_lsb(Bitboard& b) {
  assert(b);
  const Square s = lsb(b);
  b &= b - 1;
  return s;
}

extern const array<array<Bitboard, 64>, 64> _betweenBB;
inline Bitboard between_bb(Square sq1, Square sq2) { return _betweenBB.at(sq1).at(sq2); }

inline constexpr Bitboard _main_diagonal = 0x8040201008040201;
inline constexpr array<Bitboard, 15> diagonals = {
   _main_diagonal >> (7 << 3),
   _main_diagonal >> (6 << 3),
   _main_diagonal >> (5 << 3),
   _main_diagonal >> (4 << 3),
   _main_diagonal >> (3 << 3),
   _main_diagonal >> (2 << 3),
   _main_diagonal >> (1 << 3),

   _main_diagonal,

   _main_diagonal << (1 << 3),
   _main_diagonal << (2 << 3),
   _main_diagonal << (3 << 3),
   _main_diagonal << (4 << 3),
   _main_diagonal << (5 << 3),
   _main_diagonal << (6 << 3),
   _main_diagonal << (7 << 3),
};

inline constexpr Bitboard diagonal_bb(Square s) { return diagonals[(get_rank(s) - get_file(s)) + 7]; }
inline constexpr bool squares_on_same_diagonal(Square s1, Square s2) { return ((s2 - s1) % 9) == 0; }

inline constexpr Bitboard _main_anti_diagonal = 0x0102040810204080;
inline constexpr array<Bitboard, 15> anti_diagonals = {
   _main_anti_diagonal >> (7 << 3),
   _main_anti_diagonal >> (6 << 3),
   _main_anti_diagonal >> (5 << 3),
   _main_anti_diagonal >> (4 << 3),
   _main_anti_diagonal >> (3 << 3),
   _main_anti_diagonal >> (2 << 3),
   _main_anti_diagonal >> (1 << 3),

   _main_anti_diagonal,

   _main_anti_diagonal << (1 << 3),
   _main_anti_diagonal << (2 << 3),
   _main_anti_diagonal << (3 << 3),
   _main_anti_diagonal << (4 << 3),
   _main_anti_diagonal << (5 << 3),
   _main_anti_diagonal << (6 << 3),
   _main_anti_diagonal << (7 << 3),
};

inline constexpr Bitboard vt  = 0x0101010101010101;
inline constexpr Bitboard dt  = 0x0102040810204080;
inline constexpr Bitboard adt = 0x8040201008040201;

inline constexpr Bitboard anti_diagonal_bb(Square s) {
   return anti_diagonals.at(get_rank(s) + get_file(s));
}
inline constexpr bool squares_on_same_antidiagonal(Square s1, Square s2) {
   // NOTE: for some reason the code snippet from https://chessprogramming.org didn't
   //       work (ex: A1 and H2 returns true)
   return anti_diagonal_bb(s1) == anti_diagonal_bb(s2);
}

extern const array<array<Bitboard, 64>, 64> _betweenBB;
extern Bitboard between_bb(Square sq1, Square sq2);

};
