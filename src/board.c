#include "./board.h"

/*Bitboard attacked_pos(Board b, Color c) {
   int i1 = WHITE_PIECES_START;
   int i2 = WHITE_PIECES_END;
   if (c == BLACK) {
      i1 = BLACK_PIECES_START;
      i2 = BLACK_PIECES_END;
   }
   Bitboard res = 0;
   for (; i1 < i2; ++i) {
      res |= 
   }
}*/

Color get_piece_color(const Board *b, const PiecePos pos) {
   for (int i=0; i < PIECES_LEN; ++i) {
      if (( ( ((uint64_t) 1) << pos) & b->pos[i]) != 0) {
         return i <= WHITE_PIECES_END ? WHITE : BLACK;
      }
   }
   return NONE;
}
