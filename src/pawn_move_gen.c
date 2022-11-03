#include "./utils.h"
#include "./board.h"
#include "./debug.h"

Bitboard gen_pawn_push(Board *b, const Square s) {
   const PieceColor myc = get_piece_color(b, s);
   Square pushpos = s + 8*pawn_direction(myc);
   if (pushpos < 0 || pushpos > 63 || ispos_occupied(b, pushpos)) {
      return 0;
   }
   return (1ull << pushpos);
}

Bitboard gen_double_push(Board *b, const Square s) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, s);
   res |= gen_pawn_push(b, s);
   if (res != 0) {
      Square pushpos = s + 16*pawn_direction(myc);
      if (pushpos >= 0 && pushpos <= 63 && !ispos_occupied(b, pushpos)) {
         res |= (1ull << pushpos);
      }
   }
   return res;
}

Bitboard gen_pawn_attacks(Board *b, const Square s) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, s);
   const int targets[2] = {9, 7};
   const Bitboard masks[2] = {
      myc == White ? ~(fileA | rank1) : ~(fileH | rank8),
      myc == White ? ~(fileH | rank1) : ~(fileA | rank8),
   };
   for (int i = 0; i < 2; ++i) {
      const Square target = s + targets[i]*pawn_direction(myc);
      if (get_piece_color(b, target) == opposite_color(myc) || target == b->enpassant_square) {
         res |= (1ull << target) & masks[i];
      }
   }
   return res;
}
