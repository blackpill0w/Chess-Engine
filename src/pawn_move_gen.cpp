#include "./board.hpp"
#include "./utils.hpp"
#include "./bitboard.hpp"

namespace Chess {

Bitboard Board::gen_pawn_push(const Square s) const {
   const PieceColor myc = get_piece_color(s);
   Square pushpos = s + 8*pawn_direction(myc);
   if (pushpos < 0 || pushpos > 63 || ispos_occupied(pushpos)) {
      return 0;
   }
   return sqbb(pushpos);
}

Bitboard Board::gen_double_push(const Square s) const {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(s);
   res |= gen_pawn_push(s);
   if (res != 0) {
      Square pushpos = s + 16*pawn_direction(myc);
      if (pushpos >= 0 && pushpos <= 63 && !ispos_occupied(pushpos)) {
         res |= sqbb(pushpos);
      }
   }
   return res;
}

Bitboard Board::gen_pawn_attacks(const Square s, const int flags) const {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(s);
   constexpr int targets[2] = {9, 7};
   const Bitboard masks[2] {
      myc == White ? ~(file_bb(fileA) | rank_bb(rank1)) : ~(file_bb(fileH) | rank_bb(rank8)),
      myc == White ? ~(file_bb(fileH) | rank_bb(rank1)) : ~(file_bb(fileA) | rank_bb(rank8)),
   };
   for (int i = 0; i < 2; ++i) {
      const Square target = s + targets[i]*pawn_direction(myc);
      if (get_piece_color(target) == ~myc
          || target == enpassant_square || (flags & gen_pseudo)
         ) {
         res |= sqbb(target) & masks[i];
      }
   }
   return res;
}

} // namespace Chess
