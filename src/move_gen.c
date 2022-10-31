#include "./board.h"

#include "./utils.h"
#include "./bitopt.h"

#include <stdlib.h>

// When iterating through directions, if (1ull << target_square) & corresponding_mask != 0,
// it means the target_square is in the edge of the board
static const Bitboard DIR_MASKS[8] = {
   // Rook directions
   rank8,
   rank1,
   fileH,
   fileA,
   // Bishop directions
   fileH | rank8,
   fileA | rank8,
   fileH | rank1,
   fileA | rank1,
};

static const int KNIGHT_MOVES[8] = {
   6, 10, 15, 17, -6, -10, -15, -17,
};

// Masks to avoid wrap arounds
static const Bitboard KNIGHT_MOVES_CORRESPONDING_MASK[8] = {
   ~(rank1 | fileG | fileH),
   ~(rank1 | fileA | fileB),
   ~(rank1 | rank2 | fileH),
   ~(rank1 | rank2 | fileA),
   ~(rank8 | fileB | fileA),
   ~(rank8 | fileH | fileG),
   ~(rank8 | rank7 | fileA),
   ~(rank8 | rank7 | fileH),
};

Bitboard gen_sliding_piece_moves(Board *b, const Square s, const PieceType t) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, s);
   const unsigned start = (t == Bishop) ? BISHOP_DIR_START : ROOK_DIR_START;
   const unsigned end = (t == Rook) ? ROOK_DIR_END : BISHOP_DIR_END;
   for (unsigned i = start; i <= end; ++i) {
      if ((1ull << s) & DIR_MASKS[i]) {
         continue;
      }
      Square pos = s + DIR[i];
      while (pos >= 0 && pos < 64) {
         const PieceColor enemyc = get_piece_color(b, pos);
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

Bitboard gen_knight_moves(Board *b, const Square s) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, s);
   for (int i = 0; i < 8; ++i) {
      const Square target = s + KNIGHT_MOVES[i];
      if (get_piece_color(b, target) != myc) {
         res |= (1ull << target) & KNIGHT_MOVES_CORRESPONDING_MASK[i];
      }
   }
   return res;
}

Bitboard gen_king_moves(Board *b, const Square s) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, s);
   for (int i = 0; i < 8; ++i) {
      if ((1ull << s) & DIR_MASKS[i]) {
         continue;
      }
      const Square target = s + DIR[i] ;
      if (get_piece_color(b, target) != myc) {
         res |= 1ull << target;
      }
   }
   return res & ~(myc == White ? b->black_attacked : b->white_attacked);
}

void gen_piece_moves(Board *b, const Square s) {
   const PieceType pt = get_piece_type(b, s);
   PieceMoves pm = { .piece_pos = s, .possible_moves = 0 };
   if (pt == Queen || pt == Rook || pt == Bishop) {
      pm.possible_moves = gen_sliding_piece_moves(b, s, pt);
   }
   else if (pt == Knight) {
      pm.possible_moves = gen_knight_moves(b, s);
   }
   else if (pt == King) {
      pm.possible_moves = gen_king_moves(b, s);
   }
   else if (pt == Pawn) {
      const PieceColor pc = get_piece_color(b, s);
      pm.possible_moves = gen_pawn_attacks(b, s);

      if ((pc == White && (s & rank2))
          || (pc == Black && (s & rank7))
         ) {
         pm.possible_moves |= gen_double_push(b, s);
      }
      else {
         pm.possible_moves |= gen_pawn_push(b, s);
      }
   }
   vec_push(b->movelist, pm);
}

void gen_board_legal_moves(Board *b) {
   for (int k = 0; k < 64; ++k) {
      // no need to check if bit is set because `get_piece_color()` returns
      // `NoColor` if square is empty.
      if (get_piece_color(b, k) == b->color_to_play) {
         gen_piece_moves(b, k);
      }
   }
}
