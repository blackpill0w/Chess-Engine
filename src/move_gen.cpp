#include "./board.hpp"
#include <array>

#include "./utils.hpp"
#include "./bitboard.hpp"
#include "./bitopt.hpp"


using std::array;

namespace Chess {
// When iterating through directions, if (sqbb(target_square) & corresponding_mask != 0),
// it means the target_square is in the edge of the board
static constexpr array<Bitboard, 8> dir_masks {
   // Rook directions
   rank_bb(rank8),
   rank_bb(rank1),
   file_bb(fileH),
   file_bb(fileA),
   // Bishop directions
   file_bb(fileH) | rank_bb(rank8),
   file_bb(fileA) | rank_bb(rank8),
   file_bb(fileH) | rank_bb(rank1),
   file_bb(fileA) | rank_bb(rank1),
};

static constexpr array<int, 8> knight_moves {
   6, 10, 15, 17, -6, -10, -15, -17,
};

// Masks to avoid wrap arounds
static constexpr array<Bitboard, 8> knight_moves_corresponding_mask {
   ~(rank_bb(rank1) | file_bb(fileG) | file_bb(fileH)),
   ~(rank_bb(rank1) | file_bb(fileA) | file_bb(fileB)),
   ~(rank_bb(rank1) | rank_bb(rank2) | file_bb(fileH)),
   ~(rank_bb(rank1) | rank_bb(rank2) | file_bb(fileA)),
   ~(rank_bb(rank8) | file_bb(fileB) | file_bb(fileA)),
   ~(rank_bb(rank8) | file_bb(fileH) | file_bb(fileG)),
   ~(rank_bb(rank8) | rank_bb(rank7) | file_bb(fileA)),
   ~(rank_bb(rank8) | rank_bb(rank7) | file_bb(fileH)),
};

Bitboard Board::gen_sliding_piece_moves(const Square s, const PieceType t,
                                        const int flags) const {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(s);
   const unsigned start = (t == Bishop) ? bishop_dir_start : rook_dir_start;
   const unsigned end = (t == Rook) ? rook_dir_end : bishop_dir_end;
   for (unsigned i = start; i <= end; ++i) {
      if (sqbb(s) & dir_masks[i]) {
         continue;
      }
      Square pos = s + dir[i];
      while (pos >= A1 && pos <= H8) {
         const PieceColor enemyc = get_piece_color(pos);
         if (enemyc != NoColor && !(flags & gen_with_no_block)
             && !((flags & gen_ignoring_enemy_king) && (sqbb(pos) & piecesBB[myc == White ? BK : WK]))
            ) {
            if (enemyc == ~myc || (flags & gen_pseudo)) {
               res |= sqbb(pos);
            }
            break;
         }
         res |= sqbb(pos);
         if (sqbb(pos) & dir_masks[i]) {
            break;
         }
         pos += dir[i];
      }
   }
   return res;
}

Bitboard Board::gen_knight_moves(const Square s, const int flags) const {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(s);
   for (int i = 0; i < 8; ++i) {
      const Square target = s + knight_moves[i];
      if (get_piece_color(target) != myc || (flags & gen_pseudo)) {
         res |= sqbb(target) & knight_moves_corresponding_mask[i];
      }
   }
   return res;
}

Bitboard Board::gen_king_moves(const Square s, const int flags) const {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(s);
   for (int i = 0; i < 8; ++i) {
      if (sqbb(s) & dir_masks[i]) {
         continue;
      }
      const Square target = s + dir[i] ;
      if (get_piece_color(target) != myc || (flags & gen_pseudo)) {
         res |= sqbb(target);
      }
   }

   const Bitboard KSCastlingSquaresBB = (myc == White ? WKSCastlingSquaresBB : BKSCastlingSquaresBB);
   const Bitboard QSCastlingSquaresBB = (myc == White ? WQSCastlingSquaresBB : BQSCastlingSquaresBB);

   // King side castling
   if (cr & (myc == White ? White_OO : Black_OO)) {
      if (!(all_pieces() & KSCastlingSquaresBB)
          && !(KSCastlingSquaresBB & attacked_by_enemy)
         ) {
         res |= sqbb(s + 2);
      }
   }
   // Queen side castling
   if (cr & (myc == White ? White_OOO : Black_OOO)) {
      if (!(all_pieces() & QSCastlingSquaresBB)
          && !(QSCastlingSquaresBB & attacked_by_enemy)
         ) {
         res |= sqbb(s - 2);
      }
   }
   return (flags & gen_pseudo) != 0 ? res : res & ~(attacked_by_enemy);
}

Bitboard Board::gen_piece_moves(const Square s, const int flags) const {
   Bitboard res = 0;
   PieceType pt = get_piece_type(s);
   if (pt == Queen || pt == Rook || pt == Bishop) {
      res = gen_sliding_piece_moves(s, pt, flags);
   }
   else if (pt == Knight) {
      res = gen_knight_moves(s, flags);
   }
   else if (pt == King) {
      res = gen_king_moves(s, flags);
   }
   else if (pt == Pawn) {
      const PieceColor myc = get_piece_color(s);
      res = gen_pawn_attacks(s, flags);

      if ((myc == White && (sqbb(s) & rank_bb(rank2)))
          || (myc == Black && (sqbb(s) & rank_bb(rank7)))
         ) {
         res |= gen_double_push(s);
      }
      else {
         res |= gen_pawn_push(s);
      }
   }
   return res;
}

void Board::gen_board_legal_moves() {
   movelist.clear();
   attacked_by_enemy = 0;
   checkers = 0;
   possible_moves = ~0;

   for (Square sq = A1; sq <= H8; ++sq) {
      // no need to check if bit is set because `get_piece_color()` returns
      // `NoColor` if the square is empty.
      if (get_piece_color(sq) == ~color_to_play) {
         attacked_by_enemy |= gen_piece_moves(sq, gen_pseudo | gen_ignoring_enemy_king);
      }
   }
   const Square king = lsb(piecesBB[color_to_play == White ? WK : BK]);
   const Bitboard attackers = attackers_of(king);

   if (more_than_one(attackers)) {
      possible_moves = piecesBB[color_to_play == White ? WK : BK];
   }
   else if (attackers) {
      possible_moves = between_bb(king, lsb(attackers));
   }

   for (Square sq = A1; sq < H8; ++sq) {
      if (get_piece_color(sq) == color_to_play) {
         PieceMoves pm{ sq, gen_piece_moves(sq) };
         if (sq != lsb(piecesBB[color_to_play == White ? WK : BK])) {
            pm.possible_moves &= possible_moves;
         }
         movelist.push_back(pm);
      }
   }
}

} // namespace Chess
