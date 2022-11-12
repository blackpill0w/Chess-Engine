#include "./board.hpp"

#include <iostream>
#include <string>

#include "./utils.hpp"
#include "./bitboard.hpp"
#include "./bitopt.hpp"

using std::string;

namespace Chess
{
Board::Board (string FEN)
   : piecesBB{}, enpassant_square{NoSquare}, color_to_play{White},
     move_history{}, movelist{}, cr{AnyCastling}, attacked_by_enemy{},
     checkers{}
{
   move_history.reserve(64);
   movelist.reserve(64);
   load_fen(FEN);
}

Bitboard Board::white_pieces() const {
   return (piecesBB[WK] | piecesBB[WQ] | piecesBB[WR] | piecesBB[WB] | piecesBB[WN] | piecesBB[WP]);
}

Bitboard Board::black_pieces() const {
   return (piecesBB[BK] | piecesBB[BQ] | piecesBB[BR] | piecesBB[BB] | piecesBB[BN] | piecesBB[BP]);
}

Bitboard Board::all_pieces() const {
   return white_pieces() | black_pieces();
}

PieceType Board::get_piece_type(const Square s) const {
   PieceType t = NoType;
   for (size_t i = 0; i < piecesBB.size(); ++i) {
      if (sqbb(s) & piecesBB[i]) {
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

size_t Board::get_pieceBB_index(const Square s) const {
   for (size_t i = 0; i < piecesBB.size(); ++i) {
      if (sqbb(s) & piecesBB[i]) {
         return i;
      }
   }
   return invalid_index;
}

PieceColor Board::get_piece_color(const Square s) const {
   const unsigned res = get_pieceBB_index(s);
   return res <= wp_end ? White : res <= bp_end ? Black : NoColor;
}

bool Board::ispos_occupied(const Square s) const {
   return (sqbb(s) & all_pieces()) != 0;
}

void Board::remove_piece_at(const Square s) {
   unsetbit(piecesBB[get_pieceBB_index(s)], s);
}

MoveData Board::gen_move_data(const Square from, const Square to) const {
   MoveType mt = Normal;
   PieceType pt = get_piece_type(from);
   if (pt == Pawn) {
      if (to <= H1 || to >= A8) {
         mt = Promotion;
      }
      else if (to == enpassant_square) {
         mt = En_passant;
      }
   }
   else if (pt == King && abs((int) (to - from)) == 2) {
      mt = Castling;
   }
   return new_md(from, to, Queen, mt);
}

Bitboard Board::attackers_of(const Square s) const {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(s);
   const Bitboard enemyq = piecesBB[myc == White ? BQ : WQ];
   const Bitboard enemyr = piecesBB[myc == White ? BR : WR];
   const Bitboard enemyb = piecesBB[myc == White ? BB : WB];

   res |= gen_sliding_piece_moves(s, Rook) & (enemyr | enemyq);

   res |= gen_sliding_piece_moves(s, Bishop) & (enemyb | enemyq);

   res |= (gen_knight_moves(s) & piecesBB[myc == White ? BN : WN]);
   res |= (gen_king_moves(s) & piecesBB[myc == White ? BK : WK]);

   return res;
}

static void handle_castling_rights_changes(Board *b, PieceColor myc, Square from, Square to) {
   if (b->get_piece_type(from) == King) {
      b->cr &= ~(myc == White ? WhiteCastling : BlackCastling);
   }
   else if (b->get_piece_type(from) == Rook) {
      if (sqbb(from) & fileH) {
         b->cr &= ~(myc == White ? White_OO : Black_OO);
      }
      else if (sqbb(from) & fileA) {
         b->cr &= ~(myc == White ? White_OOO : Black_OOO);
      }
   }
}

static void change_piece_pos(Board *b, Square from, Square to) {
   int i = b->get_pieceBB_index(from);
   unsetbit(b->piecesBB[i], from);
   setbit(b->piecesBB[i], to);
}

int Board::make_move(const Square from, const Square to) {
   // 0 on success, -1 if move is invalid
   int err = -1;
   for (size_t i = 0; i < movelist.size(); ++i) {
      if (movelist.at(i).pos == from
          && (movelist.at(i).possible_moves & sqbb(to))
      ) {
         err = 0;

         PieceColor myc = get_piece_color(from);
         MoveData md    = gen_move_data(from, to);
         enpassant_square = NoSquare;

         // check if we should take a piece
         if (get_piece_color(to) == ~myc) {
            remove_piece_at(to);
         }
         else if (md_get_move_type(md) == En_passant) { // take if en passant
            remove_piece_at(to - 8*pawn_direction(myc));
         }
         else if (get_piece_type(from) == Pawn && abs((int) (from - to)) == 8*2) {
            enpassant_square = from + 8*pawn_direction(myc);
         }
         else if (md_get_move_type(md) == Castling) { // take if en passant
            if (from < to) { // king side
               change_piece_pos(this, to + 1, to - 1);
            }
            else { // queen side
               change_piece_pos(this, to - 2, to + 1);
            }
         }
         handle_castling_rights_changes(this, myc, from, to);

         change_piece_pos(this, from, to);
         move_history.push_back(md);

         color_to_play = ~myc;
         gen_board_legal_moves();

         return 0;
      }
   }
   if (err) {
      std::cerr << "--- Invalid move.\n\n";
   }
   return err;
}

} // namespace Chess
