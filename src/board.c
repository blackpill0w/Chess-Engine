#include "./board.h"

#include <stdbool.h>
#include <stdlib.h>

#include "./utils.h"
#include "./bitopt.h"

Board *new_board(char FEN[]) {
   Board *b = malloc(sizeof(Board));
   if (b == NULL) {
      return NULL;
   }
   b->color_to_play = White;
   b->attacked_by_enemy = 0;
   b->enpassant_square = NoSquare;
   b->cr = AnyCastling;
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
         t = (i == WK || i == BK) ? King
            : (i == WQ || i == BQ) ? Queen
            : (i == WR || i == BR) ? Rook
            : (i == WB || i == BB) ? Bishop
            : (i == WN || i == BN) ? Knight
            : Pawn;
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
   return res <= wp_end ? White : res <= bp_end ? Black : NoColor;
}

bool ispos_occupied(Board *b, const Square s) {
   return ((1ull << s) & all_pieces(b)) != 0;
}

void remove_piece_at(Board *b, const Square s) {
   unsetbit(b->piecesBB[get_pieceBB_index(b, s)], s);
}

MoveData gen_move_data(Board *b, const Square from, const Square to) {
   MoveType mt = Normal;
   PieceType pt = get_piece_type(b, from);
   if (pt == Pawn) {
      if (to <= H1 || to >= A8) {
         mt = Promotion;
      }
      else if (to == b->enpassant_square) {
         mt = En_passant;
      }
   }
   else if (pt == King && abs((int) (to - from)) == 2) {
      mt = Castling;
   }
   return new_md(from, to, Queen, mt);
}

static void handle_castling_rights_changes(Board *b, PieceColor myc, Square from, Square to) {
   if (get_piece_type(b, from) == King) {
      b->cr &= ~(myc == White ? WhiteCastling : BlackCastling);
   }
   else if (get_piece_type(b, from) == Rook) {
      if ((1ull << from) & fileH) {
         b->cr &= ~(myc == White ? White_OO : Black_OO);
      }
      else if ((1ull << from) & fileA) {
         b->cr &= ~(myc == White ? White_OOO : Black_OOO);
      }
   }
}

static void change_piece_pos(Board *b, Square from, Square to) {
   int i = get_pieceBB_index(b, from);
   unsetbit(b->piecesBB[i], from);
   setbit(b->piecesBB[i], to);
}

int make_move(Board *b, const Square from, const Square to) {
   // 0 on success, -1 if move is invalid
   int err = -1;
   for (int i = 0; i < vec_len(b->movelist); ++i) {
      if (vec_at(b->movelist, i).piece_pos == from
          && (vec_at(b->movelist, i).possible_moves & (1ull << to))
      ) {
         err = 0;

         PieceColor myc = get_piece_color(b, from);
         MoveData md    = gen_move_data(b, from, to);
         b->enpassant_square = NoSquare;

         // check if we should take a piece
         if (get_piece_color(b, to) == opposite_color(myc)) {
            remove_piece_at(b, to);
         }
         else if (md_get_move_type(md) == En_passant) { // take if en passant
            remove_piece_at(b, to - 8*pawn_direction(myc));
         }
         else if (get_piece_type(b, from) == Pawn && abs((int) (from - to)) == 8*2) {
            b->enpassant_square = from + 8*pawn_direction(myc);
         }
         else if (md_get_move_type(md) == Castling) { // take if en passant
            if (from < to) { // king side
               change_piece_pos(b, to + 1, to - 1);
            }
            else { // queen side
               change_piece_pos(b, to - 2, to + 1);
            }
         }
         handle_castling_rights_changes(b, myc, from, to);

         change_piece_pos(b, from, to);
         vec_push(b->move_history, md);

         b->color_to_play = opposite_color(myc);
         gen_board_legal_moves(b);

         return 0;
      }
   }
   if (err) {
      printf(" -- Invalid move.\n\n");
   }
   return err;
}
