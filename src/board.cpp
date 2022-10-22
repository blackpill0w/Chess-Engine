#include "./board.hpp"
#include "./utils.hpp"

namespace Chess
{

Bitboard Board::white_pieces() const {
   return (piecesBB[WK] | piecesBB[WQ] | piecesBB[WR] | piecesBB[WB] | piecesBB[WN] | piecesBB[WP]);
}

Bitboard Board::black_pieces() const {
   return (piecesBB[BK] | piecesBB[BQ] | piecesBB[BR] | piecesBB[BB] | piecesBB[BN] | piecesBB[BP]);
}

Bitboard Board::all_pieces() const {
   return white_pieces() | black_pieces();
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
   return res <= WP_END ? WHITE : res <= BP_END ? BLACK : NONE;
}

void Board::move(const Square from, const Square to) {
   move_history.at(++last_move_index) = new_move(from, to);
}

} // namespace Chess
