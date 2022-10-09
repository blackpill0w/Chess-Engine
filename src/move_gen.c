#include <stdint.h>

#include "./move_gen.h"

// Indices
#define ROOK_DIRECTION_START 0
#define ROOK_DIRECTION_END 3
#define BISHOP_DIRECTION_START 4
#define BISHOP_DIRECTION_END 7

static const int DIRECTIONS[][2] = {
   // Rook directions
   {1, 0},
   {-1, 0},
   {0, 1},
   {0, -1},
   // Bishop directions
   {1, 1},
   {1, -1},
   {-1, 1},
   {-1, -1},
};

static const Bitboard FILE_MASKS[8] = {
  0x0101010101010101,
  0x0202020202020202,
  0x0404040404040404,
  0x0808080808080808,
  0x1010101010101010,
  0x2020202020202020,
  0x4040404040404040,
  0x8080808080808080,
};

static const Bitboard RANK_MASKS[8] = {
   (uint64_t) 255,
   (uint64_t) 255 << 8,
   (uint64_t) 255 << 16,
   (uint64_t) 255 << 24,
   (uint64_t) 255 << 32,
   (uint64_t) 255 << 40,
   (uint64_t) 255 << 48,
   (uint64_t) 255 << 56,
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

// NOTE: this still requires piece's index
Bitboard gen_sliding_piece_moves(const Board *b, const PiecePos p, const PieceType t) {
   Bitboard res = 0;
   const PieceColor myc = get_piece_color(b, p);
   const int start = (t == BISHOP) ? BISHOP_DIRECTION_START : ROOK_DIRECTION_START;
   const int end = (t == ROOK) ? ROOK_DIRECTION_END : BISHOP_DIRECTION_END;
   
   for (int i = start; i <= end; ++i) {
      int x = (p % 8) + DIRECTIONS[i][0];
      int y = (p / 8) + DIRECTIONS[i][1];
      while (x >= 0 && x < 8 && y >= 0 && y < 8) {
         const PieceColor enemyc = get_piece_color(b, x+y*8);
         if (enemyc != NONE) {
            if (enemyc == opposite_color(myc)) {
               setbit(res, x+y*8);
            }
            break;
         }
         setbit(res, x+y*8);
         x += DIRECTIONS[i][0];
         y += DIRECTIONS[i][1];
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

