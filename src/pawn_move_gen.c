#include "./board.h"
#include "./pawn_move_gen.h"

Bitboard gen_pawn_moves(const Board *b, const PiecePos p) {
}

/*!
  Generate pawn push.
  @param b the board we're operating with.
  @param p, a bitboard containing the piece only.
  @param c, color of the piece, NONE to determin it automatically
*/
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
