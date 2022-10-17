#include "./board.h"

PieceColor get_piece_color(const Board *b, const PiecePos p) {
   for (int i=0; i < PIECES_LEN; ++i) {
      if (p & b->pos[i]) {
         return i <= WP_END ? WHITE : BLACK;
      }
   }
   return NONE;
}
