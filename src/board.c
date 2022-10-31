#include "./board.h"

#include <stdbool.h>
#include <stdlib.h>

#include "./utils.h"
#include "./bitopt.h"

Board *new_board(char FEN[]) {
   Board *b = malloc(sizeof(Board));

   b->color_to_play = White;
   b->white_attacked = 0;
   b->black_attacked = 0;
   b->enpassant_square = NoSquare;
   memset(b->piecesBB, 0, PiecesLen*sizeof(b->piecesBB[0]));

   vec_init(b->move_history);
   vec_reserve(b->move_history, 64);

   vec_init(b->movelist);
   vec_reserve(b->movelist, 64);

   load_fen(b, FEN);

   return b;
}

void free_board(Board *b) {
   vec_free(b->move_history);
   vec_free(b->movelist);
   free(b);
}

Bitboard white_pieces(Board *b) {
   return (b->piecesBB[WK] | b->piecesBB[WQ] | b->piecesBB[WR] | b->piecesBB[WB] | b->piecesBB[WN] | b->piecesBB[WP]);
}

Bitboard black_pieces(Board *b) {
   return (b->piecesBB[BK] | b->piecesBB[BQ] | b->piecesBB[BR] | b->piecesBB[BB] | b->piecesBB[BN] | b->piecesBB[BP]);
}

Bitboard all_pieces(Board *b) {
   return white_pieces(b) | black_pieces(b);
}

PieceType get_piece_type(Board *b, const Square s) {
   PieceType t = NoType;
   for (unsigned i = 0; i < PiecesLen; ++i) {
      if ((1ull << s) & b->piecesBB[i]) {
         if (i == WK || i == BK) {
            t = King;
         }
         else if (i == WQ || i == BQ) {
            t = Queen;
         }
         else if (i == WR || i == BR) {
            t = Rook;
         }
         else if (i == WB || i == BB) {
            t = Bishop;
         }
         else if (i == WN || i == BN) {
            t = Knight;
         }
         else if (i == WP || i == BP) {
            t = Pawn;
         }
         break;
      }
   }
   return t;
}

unsigned get_pieceBB_index(Board *b, const Square s) {
   for (unsigned i = 0; i < PiecesLen; ++i) {
      if ((1ull << s) & b->piecesBB[i]) {
         return i;
      }
   }
   return invalid_index;
}

PieceColor get_piece_color(Board *b, const Square s) {
   const unsigned res = get_pieceBB_index(b, s);
   return res <= WP_END ? White : res <= BP_END ? Black : NoColor;
}

bool ispos_occupied(Board *b, const Square s) {
   return ((1ull << s) & all_pieces(b)) != 0;
}

void save_move(Board *b, const Square from, const Square to) {
   MoveType mt = Normal;
   PieceType pt = get_piece_type(b, from);
   if (b->enpassant_square != NoSquare) {
      b->enpassant_square = NoSquare;
   }
   if (pt == Pawn) {
      if (abs(to - from) == 8*2) {
         mt = En_passant;
         b->enpassant_square = to - 8*pawn_direction(get_piece_color(b, from));
      }
      else if (to <= H1 || to >= A8) {
         mt = Promotion;
      }
   } else if (pt == King && abs(to - from) == 2) {
      mt = Castling;
   }
   vec_push(b->move_history, new_move(from, to, mt, Queen) );
}

void move(Board *b, const Square from, const Square to) {
   bool valid_move = false;
   for (int i = 0; i < vec_len(b->movelist); ++i) {
      if (vec_at(b->movelist, i).piece_pos == from
          && (vec_at(b->movelist, i).piece_pos & to)
      ) {
         valid_move = true;

         PieceColor myc = get_piece_color(b, from);

         const int bb = get_pieceBB_index(b, from);
         unsetbit(b->piecesBB[bb], from);
         if (get_piece_color(b, to) == opposite_color(myc)) {
            unsetbit(b->piecesBB[get_pieceBB_index(b, to)], to);
         }
         setbit(b->piecesBB[bb], to);
         save_move(b, from, to);

         b->color_to_play = opposite_color(myc);
         gen_board_legal_moves(b);
      }
   }
   if (!valid_move) {
      printf("Invalide move.");
   }
}
