#include "./pawn_move_gen.h"
#include "./utils.h"

/* Bitboard gen_pawn_moves(const Board *b, const PiecePos p) {
} */

Bitboard gen_pawn_push(const Board *b, const PiecePos p, const PieceColor c) {
   PieceColor myc = c == NONE ? get_piece_color(b, p) : c;
   if ((myc == WHITE && p >= 1ul << 8*7) || (myc == BLACK && p < 1ul << 8)) {
      return 0;
   }
   Bitboard pushpos = myc == WHITE ? p << 8 : p >> 8;
   if (ispos_occupied(b, pushpos)) {
      return 0;
   }
   return pushpos;
}

Bitboard gen_double_push(const Board *b, const PiecePos p) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, p);
   res |= gen_pawn_push(b, p, myc);
   if (res != 0) {
      res |= gen_pawn_push(b, res, myc);
   }
   return res;
}

Bitboard gen_pawn_attacks(const Board *b, const PiecePos p) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, p);
   // up right, up left
   const int targets[] = {9, 7};
   const Bitboard masks[] = {
      myc == WHITE ? ~FILE_MASKS[0] : ~FILE_MASKS[7],
      myc == WHITE ? ~FILE_MASKS[7] : ~FILE_MASKS[0],
   };
   for (int i = 0; i < 2; ++i) {
      const Bitboard target = myc == WHITE ? p << targets[i] : p >> targets[i];
      if (get_piece_color(b, target) == opposite_color(myc)) {
         res |= target & masks[i];
      }
   }
   return res;
}

