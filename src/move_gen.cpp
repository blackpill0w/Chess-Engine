#include "./board.hpp"
#include "./utils.hpp"

#include <cmath>
#include <array>
using std::array;

namespace Chess
{

// When iterating through directions, if (1ull << target_square) & corresponding_mask != 0,
// it means the target_square is in the edge of the board
static constexpr array<Bitboard, 8> DIR_MASKS {
   // Rook directions
   RANK_MASKS[RANK_8],
   RANK_MASKS[RANK_1],
   FILE_MASKS[FILE_H],
   FILE_MASKS[FILE_A],
   // Bishop directions
   FILE_MASKS[FILE_H] | RANK_MASKS[RANK_8],
   FILE_MASKS[FILE_A] | RANK_MASKS[RANK_8],
   FILE_MASKS[FILE_H] | RANK_MASKS[RANK_1],
   FILE_MASKS[FILE_A] | RANK_MASKS[RANK_1],
};

static constexpr array<int, 8> KNIGHT_MOVES {
   6, 10, 15, 17, -6, -10, -15, -17,
};

// Masks to avoid wrap arounds
static constexpr array<Bitboard, 8> KNIGHT_MOVES_CORRESPONDING_MASK {
   ~(RANK_MASKS[RANK_1] | FILE_MASKS[FILE_G] | FILE_MASKS[FILE_H]),
   ~(RANK_MASKS[RANK_1] | FILE_MASKS[FILE_A] | FILE_MASKS[FILE_B]),
   ~(RANK_MASKS[RANK_1] | RANK_MASKS[RANK_2] | FILE_MASKS[FILE_H]),
   ~(RANK_MASKS[RANK_1] | RANK_MASKS[RANK_2] | FILE_MASKS[FILE_A]),
   ~(RANK_MASKS[RANK_8] | FILE_MASKS[FILE_B] | FILE_MASKS[FILE_A]),
   ~(RANK_MASKS[RANK_8] | FILE_MASKS[FILE_H] | FILE_MASKS[FILE_G]),
   ~(RANK_MASKS[RANK_8] | RANK_MASKS[RANK_7] | FILE_MASKS[FILE_A]),
   ~(RANK_MASKS[RANK_8] | RANK_MASKS[RANK_7] | FILE_MASKS[FILE_H]),
};

Bitboard Board::gen_sliding_piece_moves(const Square s, const PieceType t) const {
   Bitboard res{ 0 };
   const PieceColor myc{ get_piece_color(s) };
   const unsigned start{ (t == Bishop) ? BISHOP_DIR_START : ROOK_DIR_START };
   const unsigned end{ (t == Rook) ? ROOK_DIR_END : BISHOP_DIR_END };
   for (unsigned i = start; i <= end; ++i) {
      if ((1ull << s) & DIR_MASKS[i]) {
         continue;
      }
      Square pos{ s + DIR[i] };
      while (pos >= 0 && pos < 64) {
         const PieceColor enemyc{ get_piece_color(pos) };
         if (enemyc != NoColor) {
            if (enemyc == opposite_color(myc)) {
               res |= (1ull << pos);
            }
            break;
         }
         res |= (1ull << pos);
         if ((1ull << pos) & DIR_MASKS[i]) {
            break;
         }
         pos += DIR[i];
      }
   }
   return res;
}

Bitboard Board::gen_knight_moves(const Square s) const {
   Bitboard res{0};
   const PieceColor myc{ get_piece_color(s) };
   for (int i = 0; i < 8; ++i) {
      const Square target = s + KNIGHT_MOVES[i];
      if (get_piece_color(target) != myc) {
         res |= (1ull << target) & KNIGHT_MOVES_CORRESPONDING_MASK[i];
      }
   }
   return res;
}

Bitboard Board::gen_king_moves(const Square s) const {
   Bitboard res{0};
   const PieceColor myc{ get_piece_color(s) };
   for (int i = 0; i < 8; ++i) {
      if ((1ull << s) & DIR_MASKS[i]) {
         continue;
      }
      const Square target{ s + DIR[i] };
      if (get_piece_color(target) != myc) {
         res |= 1ull << target;
      }
   }
   return res & ~(myc == White ? black_attacked : white_attacked);
}

} // namespace Chess
