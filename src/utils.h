#pragma once

#include <stdint.h>

static const unsigned invalid_index = 99999999;

typedef uint64_t Bitboard;
typedef uint64_t PiecePos;

typedef enum {NoColor, White, Black} PieceColor;
static PieceColor opposite_color(PieceColor c) {
   return (c == White ? Black : c == Black ? White : NoColor);
}
static int pawn_direction(PieceColor c) {
   return (c == White ? 1 : -1);
}

typedef enum { Knight, Bishop, Rook, Queen, King, Pawn, NoType } PieceType;

static const Bitboard fileA = 0x0101010101010101;
static const Bitboard fileB = 0x0202020202020202;
static const Bitboard fileC = 0x0404040404040404;
static const Bitboard fileD = 0x0808080808080808;
static const Bitboard fileE = 0x1010101010101010;
static const Bitboard fileF = 0x2020202020202020;
static const Bitboard fileG = 0x4040404040404040;
static const Bitboard fileH = 0x8080808080808080;

static const Bitboard rank1 = 255ull;
static const Bitboard rank2 = 255ull << 8;
static const Bitboard rank3 = 255ull << 16;
static const Bitboard rank4 = 255ull << 24;
static const Bitboard rank5 = 255ull << 32;
static const Bitboard rank6 = 255ull << 40;
static const Bitboard rank7 = 255ull << 48;
static const Bitboard rank8 = 255ull << 56;

typedef enum {
   North,
   South,
   East,
   West,
   NoEa,
   NoWe,
   SoEa,
   SoWe,
} Direction;

static const int DIR[8] = {
   // Rook directions
   8,
   -8,
   1,
   -1,
   // Bishop directions
   9,
   7,
   -7,
   -9,
};

static const unsigned ROOK_DIR_START = 0;
static const unsigned ROOK_DIR_END = 3;
static const unsigned BISHOP_DIR_START = 4;
static const unsigned BISHOP_DIR_END = 7;

typedef enum {
   A1, B1, C1, D1, E1, F1, G1, H1,
   A2, B2, C2, D2, E2, F2, G2, H2,
   A3, B3, C3, D3, E3, F3, G3, H3,
   A4, B4, C4, D4, E4, F4, G4, H4,
   A5, B5, C5, D5, E5, F5, G5, H5,
   A6, B6, C6, D6, E6, F6, G6, H6,
   A7, B7, C7, D7, E7, F7, G7, H7,
   A8, B8, C8, D8, E8, F8, G8, H8,
   NoSquare
} Square;

#ifdef CHESS_DEBUG

#include <string.h>
static char *square_to_str(const Square s) {
   static char str[10];
   if (s == NoSquare) {
      strcpy(str, "No Square");
   }
   else {
      str[0] = 'A' + int(s%8);
      str[1] = '1' + int(s/8);
      str[2] = 0;
   }
   return str;
}

#endif // CHESS_DEBUG

/*
  bit 0-5: the origin square
  bit 6-11: the target square
  bit 12-13: promotion type
  bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
*/
typedef uint16_t Move;

typedef enum {
   Normal,
   Promotion = 1 << 14,
   En_passant = 2 << 14,
   Castling = 3 << 14
} MoveType;

static Move new_move(const Square from, const Square to,
                     const MoveType mt,
                     const PieceType pt) { // promotion type
   return mt | (pt << 12) | (to << 6) | from;
}
