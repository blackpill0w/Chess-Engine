#include "./board.hpp"

#include <string>
#include <iostream>
#include <algorithm>

#include "./utils.hpp"
#include "./bitboard.hpp"
#include "./bitopt.hpp"
#include "./move_gen.hpp"
#include "./debug.hpp"

using std::string;
using std::find_if;

namespace Chess
{

Board::Board (const string &FEN)
   : piecesBB{}, enpassant_square{NoSquare}, color_to_play{White},
     move_history{}, movelist{}, cr{AnyCastling}, attacked_by_enemy{},
     checkers{}
{
   move_history.reserve(64);
   movelist.reserve(64);
   load_fen(FEN);
}

Bitboard Board::white_pieces() const {
   return piecesBB[WK] | piecesBB[WQ] | piecesBB[WR] | piecesBB[WB] | piecesBB[WN] | piecesBB[WP];
}

Bitboard Board::black_pieces() const {
   return piecesBB[BK] | piecesBB[BQ] | piecesBB[BR] | piecesBB[BB] | piecesBB[BN] | piecesBB[BP];
}

Bitboard Board::all_pieces() const {
   return white_pieces() | black_pieces();
}

Bitboard Board::get_pieces(const PieceColor c, const PieceType pt) const {
   const int i = c == White ? 0 : 6;
   switch (pt) {
      case King:
         return piecesBB[WK + i];
      case Queen:
         return piecesBB[WQ + i];
      case Rook:
         return piecesBB[WR + i];
      case Bishop:
         return piecesBB[WB + i];
      case Knight:
         return piecesBB[WN + i];
      case Pawn:
         return piecesBB[WP + i];
      case NoType:
         return c == White ? white_pieces() : black_pieces();
      default:
         return 0;
   }
}

PieceType Board::get_piece_type(const Square sq) const {
   PieceType t = NoType;
   for (size_t i = 0; i < piecesBB.size(); ++i) {
      if (sqbb(sq) & piecesBB[i]) {
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

size_t Board::get_pieceBB_index(const Square sq) const {
   for (size_t i = 0; i < piecesBB.size(); ++i) {
      if (sqbb(sq) & piecesBB[i]) {
         return i;
      }
   }
   return invalid_index;
}

size_t Board::get_pieceBB_index(const PieceType pt, const PieceColor c) const {
   return pt + (c == White ? 0 : 6);
}

PieceColor Board::get_piece_color(const Square sq) const {
   return sqbb(sq) & white_pieces() ? White : sqbb(sq) & black_pieces() ? Black : NoColor;
}

bool Board::is_square_occupied(const Square sq) const {
   return (sqbb(sq) & all_pieces()) != 0;
}

void Board::remove_piece_at(const Square sq) {
   unsetbit(piecesBB[get_pieceBB_index(sq)], sq);
}

MoveData Board::gen_move_data(const Square from, const Square to, const PieceType promote_to) const {
   MoveType mt = Normal;
   PieceType pt = get_piece_type(from);
   PieceType taken_pt = get_piece_type(to);
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
   return new_md(from, to, promote_to, mt, taken_pt, cr, enpassant_square);
}

Bitboard Board::attackers_of(const Square sq) const {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(sq);

   res |= gen_sliding_piece_moves(sq, Rook, all_pieces(), get_pieces(myc, NoType)) & (get_pieces(~myc, Rook) | get_pieces(~myc, Queen));
   res |= gen_sliding_piece_moves(sq, Bishop, all_pieces(), get_pieces(myc, NoType)) & (get_pieces(~myc, Bishop) | get_pieces(~myc, Queen));
   res |= gen_knight_moves(sq, all_pieces(), get_pieces(myc, NoType)) & get_pieces(~myc, Knight);
   res |= gen_pawn_attacks(sq, myc, all_pieces(), get_pieces(myc, NoType)) & get_pieces(~myc, Pawn);
   res |= gen_king_moves(sq, all_pieces(), get_pieces(myc, NoType), myc, NoCastling) & get_pieces(~myc, King);

   return res;
}

Bitboard Board::gen_piece_moves(const Square sq, const Bitboard occ, const Bitboard mypieces) const {
   Bitboard res = 0;
   const PieceType pt = get_piece_type(sq);
   const PieceColor myc = get_piece_color(sq);
   if (pt == Queen || pt == Rook || pt == Bishop) {
      res = gen_sliding_piece_moves(sq, pt, occ, mypieces);
   }
   else if (pt == Knight) {
      res = gen_knight_moves(sq, occ, mypieces);
   }
   else if (pt == King) {
      res = gen_king_moves(sq, occ, mypieces, myc, cr);
   }
   else if (pt == Pawn) {
      res = gen_pawn_attacks(sq, myc, occ, mypieces);
      if ((myc == White && get_rank(sq) == rank2)
         || (myc == Black && get_rank(sq) == rank7)
      ) {
         res |= gen_double_push(sq, myc, occ);
      }
      else {
         res |= gen_pawn_push(sq, myc, occ);
      }
   }
   return res;
}

void Board::limit_moves_of_pinned_pieces() {
   const Square king_sq = lsb(get_pieces(color_to_play, King));
   const array<Bitboard, 2> possible_pinners = {
      (rank_bb(king_sq) | file_bb(king_sq)) & (get_pieces(~color_to_play, Queen) | get_pieces(~color_to_play, Rook)),
      (diagonal_bb(king_sq) | anti_diagonal_bb(king_sq)) & (get_pieces(~color_to_play, Queen) | get_pieces(~color_to_play, Bishop))
   };
   for (auto i : possible_pinners) {
      while (i) {
         const Square pinner = pop_lsb(i);
         const Bitboard between = between_bb(king_sq, pinner);
         if (!more_than_one(between & all_pieces() & ~sqbb(pinner))) {
            auto pm = find_if(
               movelist.begin(),
               movelist.end(),
               [&](PieceMoves &pm){ return 0 != (sqbb(pm.pos) & between); }
            );
            pm->possible_moves &= between;
         }
      }
   }
}

void Board::gen_board_legal_moves() {
   movelist.clear();
   attacked_by_enemy = 0;
   checkers = 0;
   possible_moves = ~0ull;

   const Square king_sq = lsb(get_pieces(color_to_play, King));

   // Generated squares attacked by the enemy
   for (Square sq = A1; sq <= H8; ++sq) {
      // no need to check if bit is set because `get_piece_color()` returns
      // `NoColor` if the square is empty.
      if (get_piece_color(sq) == ~color_to_play) {
         if (get_piece_type(sq) == Pawn) {
            attacked_by_enemy |= gen_piece_moves(sq, ~0, 0);
         }
         else {
            attacked_by_enemy |= gen_piece_moves(sq, all_pieces() & ~sqbb(king_sq), 0);
         }
      }
   }
   // Checks
   const Bitboard attackers = attackers_of(king_sq);
   if (more_than_one(attackers)) {
      possible_moves = 0;
   }
   else if (attackers) {
      possible_moves = between_bb(king_sq, lsb(attackers));
   }

   // Handling en passant discovered check
   // TODO: improve this?
   const Bitboard ep_pawn = enpassant_square == NoSquare ? 0 : sqbb(enpassant_square - 8*pawn_direction(color_to_play));
   if (rank_bb(king_sq) & ep_pawn) {
      const Bitboard possible_attacker = rank_bb(king_sq) & (get_pieces(~color_to_play, Queen) | get_pieces(~color_to_play, Rook));
      if (possible_attacker) {
         const Square attacker_sq = lsb(possible_attacker);
         const Bitboard between = between_bb(king_sq, attacker_sq) & ~sqbb(attacker_sq);
         if (
            popcnt(between & all_pieces()) == 2
            && (ep_pawn & get_pieces(~color_to_play, Pawn))
            && ((ep_pawn >> 1 | ep_pawn << 1) & get_pieces(color_to_play, Pawn))
         ) {
            enpassant_square = NoSquare;
         }
      }
   }

   for (Square sq = A1; sq <= H8; ++sq) {
      if (get_piece_color(sq) == color_to_play) {
         const Bitboard ep = get_piece_type(sq) == Pawn ? sqbb(enpassant_square) : 0;
         PieceMoves pm{ sq, gen_piece_moves(sq, all_pieces() | ep, get_pieces(color_to_play, NoType)) };
         if (sq != king_sq) {
            pm.possible_moves &= possible_moves;
         }
         else {
            pm.possible_moves &= ~attacked_by_enemy;
            if ((cr & (color_to_play == White ? WhiteCastling : BlackCastling)) && attackers) {
               pm.possible_moves &= ~(sqbb(sq + 2) | sqbb(sq - 2));
            }
         }
         movelist.emplace_back(pm);
      }
   }
   limit_moves_of_pinned_pieces();
}

void Board::change_piece_pos(Square from, Square to) {
   const int i = get_pieceBB_index(from);
   if (i == invalid_index) {
      std::cerr << "Invalid read\n";
      print_board(*this);
      std::cerr << from << " -> " << to << '\n';
      exit(1);
   }
   unsetbit(piecesBB[i], from);
   setbit(piecesBB[i], to);
}

bool Board::is_valid_move(const Square from, const Square to) const {
   auto x = find_if(
      movelist.begin(),
      movelist.end(),
      [&](const PieceMoves &pm){ return pm.pos == from && (sqbb(to) & pm.possible_moves); }
   );
   return x != movelist.end();
}

static void handle_castling_rights_changes(Board &b, PieceColor myc, Square from, Square to) {
   if (b.get_piece_type(from) == King) {
      b.cr &= ~(myc == White ? WhiteCastling : BlackCastling);
   }
   else if (b.get_piece_type(from) == Rook) {
      if (from == (myc == White ? H1 : H8)) { // king side rook
         b.cr &= ~(myc == White ? White_OO : Black_OO);
      }
      else if (from == (myc == White ? A1 : A8)) { // queen side rook
         b.cr &= ~(myc == White ? White_OOO : Black_OOO);
      }
   }
   if (b.get_piece_type(to) == Rook) { // rook was taken
      if (to == (~myc == White ? H1 : H8)) { // king side rook
         b.cr &= ~(~myc == White ? White_OO : Black_OO);
      }
      else if (to == (~myc == White ? A1 : A8)) { // queen side rook
         b.cr &= ~(~myc == White ? White_OOO : Black_OOO);
      }
   }
}

BoardState Board::make_move(const Square from, const Square to, const PieceType promote_to) {
   if (!is_valid_move(from, to)) {
      return InavlidMove;
   }

   PieceColor myc   = get_piece_color(from);
   MoveData md      = gen_move_data(from, to, promote_to);
   enpassant_square = NoSquare;
   handle_castling_rights_changes(*this, myc, from, to);

   // check if we should take a piece
   if (get_piece_color(to) == ~myc) {
      remove_piece_at(to);
   }
   if (md_get_move_type(md) == En_passant) { // take if en passant
      remove_piece_at(to - 8*pawn_direction(myc));
   }
   else if (get_piece_type(from) == Pawn && abs((int) (from - to)) == 8*2) {
      enpassant_square = from + 8*pawn_direction(myc);
   }
   else if (md_get_move_type(md) == Promotion) {
      assert(promote_to == Queen || promote_to == Rook || promote_to == Bishop || promote_to == Knight);
      remove_piece_at(from);
      piecesBB.at(get_pieceBB_index(promote_to, color_to_play)) |= sqbb(to);
   }
   else if (md_get_move_type(md) == Castling) {
      if (from < to) { // king side
         change_piece_pos(to + 1, to - 1);
      }
      else { // queen side
         change_piece_pos(to - 2, to + 1);
      }
   }

   if (md_get_move_type(md) != Promotion) change_piece_pos(from, to);
   move_history.emplace_back(md);

   color_to_play = ~myc;
   gen_board_legal_moves();

   return NoErr;
}

BoardState Board::unmake_move() {
   if (move_history.size() == 0) return NoMoveToUnmake;

   color_to_play = ~color_to_play;
   const int i = color_to_play == White ? 0 : 6;
   MoveData md = move_history[move_history.size() - 1];
   move_history.pop_back();

   const Square from = md_get_square_from(md);
   const Square to = md_get_square_to(md);
   const PieceType taken_pt = md_get_taken_piece_type(md);

   if (md_get_move_type(md) == Promotion) {
      setbit(piecesBB[Pawn + i], from);
      unsetbit(piecesBB[get_pieceBB_index(md_get_promotion_type(md), color_to_play)], to);
   }
   else if (md_get_move_type(md) == En_passant) {
      change_piece_pos(to, from);
      piecesBB[get_pieceBB_index(Pawn, ~color_to_play)] |= sqbb(to - 8*pawn_direction(color_to_play));
      enpassant_square = to;
   }
   else if (md_get_move_type(md) == Castling) {
      if (to > from) { // king side
         change_piece_pos(to, from);
         change_piece_pos(to - 1, to + 1); // rook
      }
      else {
         change_piece_pos(to, from);
         change_piece_pos(to + 1, to - 2); // rook
      }
   }
   else {
      change_piece_pos(to, from);
   }
   if (taken_pt != NoType) {
      piecesBB[get_pieceBB_index(taken_pt, ~color_to_play)] |= sqbb(to);
   }
   cr = md_get_castling_rights(md);
   enpassant_square = md_get_ep_square(md);
   gen_board_legal_moves();
   return NoErr;
}

} // namespace Chess
