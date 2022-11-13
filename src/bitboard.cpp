#include "./bitboard.hpp"

namespace Chess {

static constexpr Bitboard _between_bb(Square sq1, Square sq2) {
   if (sq1 == sq2) {
      return sqbb(sq2);
   }
   const Square max = sq1 > sq2 ? sq1 : sq2;
   const Square min = max == sq1 ? sq2 : sq1;
   Bitboard res = sqbb(sq2);

   if (squares_on_same_file(sq1, sq2)) {
      int d = get_rank(max) - get_rank(min);
      res |= sqbb(sq2) | (vt & ((1ull << (d*8)) - 2)) << min;
   }
   else if (squares_on_same_rank(sq1, sq2)) {
      int d = max - min;
      res |= ((1ull << d)-2) << min;
   }
   else if (squares_on_same_antidiagonal(max, min)) {
      int d = (max - min)/9;
      res |= (dt & ((1ull << (d*9))-2)) << min;
   }
   else if (squares_on_same_diagonal(max, min)) {
      int d = (max - min)/7;
      res |= (adt & ((1ull << (d*7))-2)) << min;
   }
   return res;
}

static array<array<Bitboard, 64>, 64> _between_bb_all() {
   array<array<Bitboard, 64>, 64> arr{};
   for (Square sq1 = A1; sq1 <= H8; ++sq1) {
      for (Square sq2 = A1; sq2 <= H8; ++sq2) {
         arr[sq1][sq2] = _between_bb(sq1, sq2);
      }
   }
   return arr;
}

const array<array<Bitboard, 64>, 64> _betweenBB = _between_bb_all();

} // namespace Chess
