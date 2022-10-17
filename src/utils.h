#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

typedef uint64_t Bitboard;
typedef uint64_t PiecePos;

typedef enum {NONE = 0, WHITE, BLACK} PieceColor;

typedef enum {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN} PieceType;

static const Bitboard FILE_MASKS[] = {
  0x0101010101010101,
  0x0202020202020202,
  0x0404040404040404,
  0x0808080808080808,
  0x1010101010101010,
  0x2020202020202020,
  0x4040404040404040,
  0x8080808080808080,
};

static const Bitboard RANK_MASKS[] = {
   255ull,
   255ull << 8,
   255ull << 16,
   255ull << 24,
   255ull << 32,
   255ull << 40,
   255ull << 48,
   255ull << 56,
};

// Indices
#define ROOK_DIR_START 0
#define ROOK_DIR_END 3
#define BISHOP_DIR_START 4
#define BISHOP_DIR_END 7
#define DIR_LEN 8

enum {
   North = 0,
   South,
   East,
   West,
   NoWe,
   NoEa,
   SoEa,
   SoWe,
};

static const int DIR[DIR_LEN] = {
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

#endif // UTILS_H
