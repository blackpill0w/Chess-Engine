#include "./board.hpp"
#include "./utils.hpp"

namespace Chess
{

Board::Board(const string& FEN) {
   load_FEN(FEN);
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
   PieceType t{ NoType };
   for (unsigned i = 0; i < piecesBB.size(); ++i) {
      if ((1ull << s) & piecesBB[i]) {
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

bool Board::ispos_occupied(const Square s) const {
   return ((1ull << s) & all_pieces()) != 0;
}

unsigned Board::get_piece_at(const Square s) const {
   for (unsigned i = 0; i < piecesBB.size(); ++i) {
      if ((1ull << s) & piecesBB[i]) {
         return i;
      }
   }
   return invalid_index;
}

PieceColor Board::get_piece_color(const Square s) const {
   const unsigned res{ get_piece_at(s) };
   return res <= WP_END ? White : res <= BP_END ? Black : NoColor;
}

void Board::move(const Square from, const Square to) {
   MoveType mt;
   if (enpassant_square != NoSquare) {
      enpassant_square = NoSquare;
   }
   if (get_piece_type(from) == Pawn && abs(to - from) == 8*2) {
      mt = En_passant;
      enpassant_square = to - 8*pawn_direction(get_piece_color(from));
   } else if (get_piece_type(from) == King && abs(to - from) == 2) {
      mt = Castling;
   }
   move_history.at(++last_move_index) = new_move(from, to, mt);
}

} // namespace Chess
