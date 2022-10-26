#include "./board.hpp"
#include "./utils.hpp"

#include <array>
using std::array;

namespace Chess
{

Bitboard Board::gen_pawn_push(const Square s, const PieceColor c) const {
   const PieceColor myc{ c == NoColor ? get_piece_color(s) : c };
   Square pushpos{ s + 8*pawn_direction(myc) };
   if (pushpos < 0 || pushpos > 63 || ispos_occupied(pushpos)) {
      return 0;
   }
   return (1ull << pushpos);
}

Bitboard Board::gen_double_push(const Square s) const {
   Bitboard res{ 0 };
   const PieceColor myc{ get_piece_color(s) };
   res |= gen_pawn_push(s, myc);
   if (res != 0) {
      res |= gen_pawn_push(s + 8*pawn_direction(myc), myc);
   }
   return res;
}

Bitboard Board::gen_pawn_attacks(const Square s) const {
   Bitboard res{ 0 };
   const PieceColor myc{ get_piece_color(s) };
   constexpr array<int, 2> targets{9, 7};
   const array<Bitboard, 2> masks {
      myc == White ? ~(FILE_MASKS[FILE_A] | RANK_MASKS[RANK_1]) : ~(FILE_MASKS[FILE_H] | RANK_MASKS[RANK_8]),
      myc == White ? ~(FILE_MASKS[FILE_H] | RANK_MASKS[RANK_1]) : ~(FILE_MASKS[FILE_A] | RANK_MASKS[RANK_8]),
   };
   for (int i = 0; i < 2; ++i) {
      const Square target{ s + targets[i]*pawn_direction(myc) };
      if (get_piece_color(target) == opposite_color(myc) || target == enpassant_square) {
         res |= (1ull << target) & masks[i];
      }
   }
   return res;
}

} // namespace Chess
