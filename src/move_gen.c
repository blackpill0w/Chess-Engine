#include "./move_gen.h"

// Indices
#define ROOK_FIRST_DIRECTION 0
#define ROOK_LAST_DIRECTION 3
#define BISHOP_FIRST_DIRECTION 4
#define BISHOP_LAST_DIRECTION 7

static const int DIRECTIONS[] = {
   // Rook directions
   1,
   -1,
   8,
   -8,
   // Bishop directions
   9,
   7,
   -9,
   -7,
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
   255ul,
   255ul << 8,
   255ul << 16,
   255ul << 24,
   255ul << 32,
   255ul << 40,
   255ul << 48,
   255ul << 56,
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

Bitboard gen_knight_moves(const Board *b, const Bitboard np) {
   Bitboard res = 0;
   Color myc = get_piece_color(b, np);
   for (int i = 0; i < KNIGHT_MOVES_LEN; ++i) {
      res |= (1ul << (np + KNIGHT_MOVES[i])) & KNIGHT_MOVES_CORRESPONDING_MASK[i];
      if (np < 2*8) { // if 1st or 2nd rank, remove moves in the 7th and 8th rank
         res &= ~(RANK_MASKS[7] | RANK_MASKS[6]);
      }
      else if (np > 7*8) { // if 7th or 8th rank, remove moves in the 1st and 2nd rank
         res &= ~(RANK_MASKS[0] | RANK_MASKS[1]);
      }
   }
   return res;
}

Bitboard gen_rook_moves(const Board *b, const Bitboard rp) {
   Bitboard res = 0;
   for (int i = ROOK_FIRST_DIRECTION; i <= ROOK_LAST_DIRECTION; ++i) {
      int pos = rp + DIRECTIONS[i];
      // (x >= 0 && x < 8 && y >= 0 && y < 8)
      // while (pos >= 0 && pos < 64 && pos % 8) {
      for (int j = 0; j < 8 && ; ++j) {   
         if (isbitset(all_pieces(b) & ~(rp), pos)) {
            break;
         }
         setbit(res, pos);
         pos += DIRECTIONS[i];
      }
   }
   return res;
}

/*Bitboard gen_bishop_moves(const Board *b, const Bitboard bp) {
   Bitboard res = 0;
   for (int i = BISHOP_FIRST_DIRECTION; i <= BISHOP_LAST_DIRECTION; ++i) {
      int x = (bp % 8) + DIRECTIONS[i][0];
      int y = (bp / 8) + DIRECTIONS[i][1];
      while (x >= 0 && x < 8 && y >= 0 && y < 8) {
         if (isbitset(all_pieces(b) & ~(bp), x+y*8)) {
            break;
         }
         setbit(res, x + y*8);
         x += DIRECTIONS[i][0];
         y += DIRECTIONS[i][1];
      }
   }
   return res;
}

Bitboard gen_queen_moves(const Board *b, const Bitboard qp) {
   return ( gen_bishop_moves(b, qp) | gen_rook_moves(b, qp) );
}
*/
