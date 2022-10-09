#include "./board.h"

PieceColor get_piece_color(const Board *b, const PiecePos pos) {
   for (int i=0; i < PIECES_LEN; ++i) {
      if (pos & b->pos[i]) {
         return i <= WHITE_PIECES_END ? WHITE : BLACK;
      }
   }
   return NONE;
}
