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

Bitboard gen_knight_moves(const Board *b, const PiecePos np) {
   Bitboard res = 0;
   const Color myc = get_piece_color(b, np);
   for (int i = 0; i < KNIGHT_MOVES_LEN; ++i) {
      if (get_piece_color(b, np + KNIGHT_MOVES[i]) != myc) {
         res |= (((uint64_t) 1) << (np + KNIGHT_MOVES[i])) & KNIGHT_MOVES_CORRESPONDING_MASK[i];
         if (np >= 6*8) { // if 7th or 8th rank, remove moves in the 1st and 2nd rank
            res &= ~(RANK_MASKS[0] | RANK_MASKS[1]);
         }
         else if (np < 2*8) { // if 1st or 2nd rank, remove moves in the 7th and 8th rank
            res &= ~(RANK_MASKS[7] | RANK_MASKS[6]);
         }
      }
   }
   return res;
}

Bitboard gen_sliding_piece_moves(const Board *b, const PiecePos p, const PieceType t) {
   Bitboard res = 0;
   const Color myc = get_piece_color(b, p);
   const int start = (t == BISHOP) ? BISHOP_DIRECTION_START : ROOK_DIRECTION_START;
   const int end = (t == ROOK) ? ROOK_DIRECTION_END : BISHOP_DIRECTION_END;
   
   for (int i = start; i <= end; ++i) {
      int x = (p % 8) + DIRECTIONS[i][0];
      int y = (p / 8) + DIRECTIONS[i][1];
      while (x >= 0 && x < 8 && y >= 0 && y < 8) {
         const Color enemyc = get_piece_color(b, x+y*8);
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

