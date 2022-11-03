#include "./board.h"

#include "./utils.h"
#include "./bitopt.h"

#include <stdlib.h>
#include <stdbool.h>

// When iterating through directions, if (1ull << target_square) & corresponding_mask != 0,
// it means the target_square is in the edge of the board
static const Bitboard dir_masks[8] = {
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

static const int knight_moves[8] = {
   6, 10, 15, 17, -6, -10, -15, -17,
};

// Masks to avoid wrap arounds
static const Bitboard knight_moves_corresponding_mask[8] = {
   ~(rank1 | fileG | fileH),
   ~(rank1 | fileA | fileB),
   ~(rank1 | rank2 | fileH),
   ~(rank1 | rank2 | fileA),
   ~(rank8 | fileB | fileA),
   ~(rank8 | fileH | fileG),
   ~(rank8 | rank7 | fileA),
   ~(rank8 | rank7 | fileH),
};

Bitboard gen_sliding_piece_moves(Board *b, const Square s, const PieceType t, const bool gen_attacked_by_enemy) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, s);
   const unsigned start = (t == Bishop) ? bishop_dir_start : rook_dir_start;
   const unsigned end = (t == Rook) ? rook_dir_end : bishop_dir_end;
   for (unsigned i = start; i <= end; ++i) {
      if ((1ull << s) & dir_masks[i]) {
         continue;
      }
      Square pos = s + dir[i];
      while (pos >= 0 && pos < 64) {
         const PieceColor enemyc = get_piece_color(b, pos);
         if (enemyc != NoColor) {
            if (enemyc == opposite_color(myc) || gen_attacked_by_enemy) {
               res |= (1ull << pos);
            }
            break;
         }
         res |= (1ull << pos);
         if ((1ull << pos) & dir_masks[i]) {
            break;
         }
         pos += dir[i];
      }
   }
   return res;
}

Bitboard gen_knight_moves(Board *b, const Square s, const bool gen_attacked_by_enemy) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, s);
   for (int i = 0; i < 8; ++i) {
      const Square target = s + knight_moves[i];
      if (get_piece_color(b, target) != myc || gen_attacked_by_enemy) {
         res |= (1ull << target) & knight_moves_corresponding_mask[i];
      }
   }
   return res;
}

Bitboard gen_king_moves(Board *b, const Square s, const bool gen_attacked_by_enemy) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, s);
   for (int i = 0; i < 8; ++i) {
      if ((1ull << s) & dir_masks[i]) {
         continue;
      }
      const Square target = s + dir[i] ;
      if (get_piece_color(b, target) != myc || gen_attacked_by_enemy) {
         res |= 1ull << target;
      }
   }

   const Bitboard KSCastlingSquaresBB = (myc == White ? WKSCastlingSquaresBB : BKSCastlingSquaresBB);
   const Bitboard QSCastlingSquaresBB = (myc == White ? WQSCastlingSquaresBB : BQSCastlingSquaresBB);

   // King side castling
   if (b->cr & (myc == White ? White_OO : Black_OO)) {
      if (!(all_pieces(b) & KSCastlingSquaresBB)
          && !(KSCastlingSquaresBB & b->attacked_by_enemy)
         ) {
         res |= (1ull << (s + 2));
      }
   }
   // Queen side castling
   if (b->cr & (myc == White ? White_OOO : Black_OOO)) {
      if (!(all_pieces(b) & QSCastlingSquaresBB)
          && !(QSCastlingSquaresBB & b->attacked_by_enemy)
         ) {
         res |= (1ull << (s - 2));
      }
   }
   return gen_attacked_by_enemy == true ? res : res & ~(b->attacked_by_enemy);
}

Bitboard gen_piece_moves(Board *b, const Square s, const bool gen_attacked_by_enemy) {
   Bitboard res = 0;
   PieceType pt = get_piece_type(b, s);
   if (pt == Queen || pt == Rook || pt == Bishop) {
      res = gen_sliding_piece_moves(b, s, pt, gen_attacked_by_enemy);
   }
   else if (pt == Knight) {
      res = gen_knight_moves(b, s, gen_attacked_by_enemy);
   }
   else if (pt == King) {
      res = gen_king_moves(b, s, gen_attacked_by_enemy);
   }
   else if (pt == Pawn) {
      const PieceColor myc = get_piece_color(b, s);
      res = gen_pawn_attacks(b, s, gen_attacked_by_enemy);

      if ((myc == White && ((1ull << s) & rank2))
          || (myc == Black && ((1ull << s) & rank7))
         ) {
         res |= gen_double_push(b, s);
      }
      else {
         res |= gen_pawn_push(b, s);
      }
   }
   return res;
}

void gen_board_legal_moves(Board *b) {
   vec_clear(b->movelist);
   b->attacked_by_enemy = 0;

   for (int i = 0; i < 64; ++i) {
      // no need to check if bit is set because `get_piece_color()` returns
      // `NoColor` if the square is empty.
      if (get_piece_color(b, i) == opposite_color(b->color_to_play)) {
         b->attacked_by_enemy |= gen_piece_moves(b, i, true);
      }
   }
   for (int i = 0; i < 64; ++i) {
      if (get_piece_color(b, i) == b->color_to_play) {
         PieceMoves pm = { .piece_pos = i, .possible_moves = gen_piece_moves(b, i, false) };
         vec_push(b->movelist, pm);
      }
   }
}
