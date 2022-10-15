#include <stdint.h>
#include <stdlib.h>

#include "./utils.h"
#include "./move_gen.h"
#include "./debug.h"

// Indices
#define ROOK_DIRECTION_START 0
#define ROOK_DIRECTION_END 3
#define BISHOP_DIRECTION_START 4
#define BISHOP_DIRECTION_END 7
#define DIRECTIONS_LEN 8

static const int DIRECTIONS[DIRECTIONS_LEN] = {
   // Rook directions
   1,
   8,
   -1,
   -8,
   // Bishop directions
   9,
   7,
   -7,
   -9,
};

static const Bitboard DIRECTIONS_MASKS[DIRECTIONS_LEN] = {
   // Rook directions
   FILE_MASKS['h' - 'a'],
   RANK_MASKS[7],
   FILE_MASKS['a' - 'a'],
   RANK_MASKS[0],
   // Bishop directions
   FILE_MASKS['h' - 'a'] | RANK_MASKS[7],
   FILE_MASKS['a' - 'a'] | RANK_MASKS[7],
   FILE_MASKS['h' - 'a'] | RANK_MASKS[0],
   FILE_MASKS['a' - 'a'] | RANK_MASKS[0],
};

#define KNIGHT_MOVES_LEN 8
static const int KNIGHT_MOVES[KNIGHT_MOVES_LEN] = {
   6, 10, 15, 17, -6, -10, -15, -17,
};

// Masks to avoid wrap arounds
static const Bitboard KNIGHT_MOVES_CORRESPONDING_MASK[KNIGHT_MOVES_LEN] = {
   ~(FILE_MASKS['g' - 'a'] | FILE_MASKS['h' - 'a']),
   ~(FILE_MASKS['a' - 'a'] | FILE_MASKS['b' - 'a']),
   ~(FILE_MASKS['h' - 'a']),
   ~(FILE_MASKS['a' - 'a']),
   ~(FILE_MASKS['a' - 'a'] | FILE_MASKS['b' - 'a']),
   ~(FILE_MASKS['g' - 'a'] | FILE_MASKS['h' - 'a']),
   ~(FILE_MASKS['a' - 'a']),
   ~(FILE_MASKS['h' - 'a']),
};

#define KING_MOVES_LEN 8
static const int KING_MOVES[KING_MOVES_LEN] = {
   1, 7, 8, 9, -1, -7, -8, -9
};

// Masks to avoid wrap arounds
static const Bitboard KING_MOVES_CORRESPONDING_MASK[KING_MOVES_LEN] = {
   ~(FILE_MASKS['a' - 'a']),
   ~(FILE_MASKS['h' - 'a']),
   ~(0),
   ~(FILE_MASKS['a' - 'a']),
   ~(FILE_MASKS['h' - 'a']),
   ~(FILE_MASKS['a' - 'a']),
   ~(0),
   ~(FILE_MASKS['h' - 'a']),
};

Bitboard gen_sliding_piece_moves(const Board *b, const PiecePos p, const PieceType t) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, p);
   const int start = (t == BISHOP) ? BISHOP_DIRECTION_START : ROOK_DIRECTION_START;
   const int end = (t == ROOK) ? ROOK_DIRECTION_END : BISHOP_DIRECTION_END;
   for (int i = start; i <= end; ++i) {
      if (p & DIRECTIONS_MASKS[i]) {
         continue;
      }
      Bitboard pos = DIRECTIONS[i] > 0 ? p << DIRECTIONS[i] : p >> -DIRECTIONS[i];
      while (pos != 0) {
         const PieceColor enemyc = get_piece_color(b, pos);
         if (enemyc != NONE) {
            if (enemyc == opposite_color(myc)) {
               res |= pos;
            }
            break;
         }
         res |= pos;
         if (pos & DIRECTIONS_MASKS[i]) {
            break;
         }
         if (DIRECTIONS[i] > 0) {
            pos <<= DIRECTIONS[i];
         }
         else {
            pos >>= -DIRECTIONS[i];
         }
      }
   }
   return res;
}

Bitboard gen_knight_moves(const Board *b, const PiecePos p) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, p);
   for (int i = 0; i < KNIGHT_MOVES_LEN; ++i) {
      const PiecePos target = KNIGHT_MOVES[i] > 0 ? p << KNIGHT_MOVES[i] : p >> -KNIGHT_MOVES[i];
      if (get_piece_color(b, target) != myc) {
         res |= target & KNIGHT_MOVES_CORRESPONDING_MASK[i];
      }
   }
   return res;
}

Bitboard gen_king_moves(const Board *b, const PiecePos p) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, p);
   for (int i = 0; i < KING_MOVES_LEN; ++i) {
      const PiecePos target = KING_MOVES[i] > 0 ? p << KING_MOVES[i] : p >> -KING_MOVES[i];
      if (get_piece_color(b, target) != myc) {
         res |= target & KING_MOVES_CORRESPONDING_MASK[i];
      }
   }
   return res & ~(myc == WHITE ? b->black_attacked : b->white_attacked);
}

