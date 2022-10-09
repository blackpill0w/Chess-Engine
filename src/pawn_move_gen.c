#include "./board.h"
#include "./pawn_move_gen.h"

Bitboard gen_pawn_moves(const Board *b, const PiecePos p) {
}

Bitboard gen_pawn_push(const Board *b, const PiecePos p) {
   PieceColor myc = get_piece_color(b, p);
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
   res |= gen_pawn_push(b, p);
   if (res != 0) {
      res |= gen_pawn_push(b, get_piece_color(b, p) == WHITE ? p << 8 : p >> 8);
   }
   return res;
}
