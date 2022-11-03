#pragma once

#include <stdint.h>

#define invalid_index 999999999

typedef uint64_t Bitboard;
typedef uint64_t PiecePos;

typedef enum { NoColor, White, Black } PieceColor;
#define opposite_color(c) (c == White ? Black : c == Black ? White : NoColor)
#define str_piece_color(c) (c == White ? "White" : c == Black ? "Black" : "NoColor")
#define pawn_direction(c) (c == White ? 1 : -1)

typedef enum { Knight, Bishop, Rook, Queen, King, Pawn, NoType } PieceType;

#define fileA 0x0101010101010101
#define fileB 0x0202020202020202
#define fileC 0x0404040404040404
#define fileD 0x0808080808080808
#define fileE 0x1010101010101010
#define fileF 0x2020202020202020
#define fileG 0x4040404040404040
#define fileH 0x8080808080808080

#define rank1 (255ull)
#define rank2 (255ull << 8)
#define rank3 (255ull << 16)
#define rank4 (255ull << 24)
#define rank5 (255ull << 32)
#define rank6 (255ull << 40)
#define rank7 (255ull << 48)
#define rank8 (255ull << 56)

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

static const int dir[8] = {
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

#define rook_dir_start    0
#define rook_dir_end      3
#define bishop_dir_start  4
#define bishop_dir_end    7

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

/*
  bit 0-5: the origin square
  bit 6-11: the target square
  bit 12-13: promotion type
  bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
*/
typedef uint16_t MoveData;
#define md_from_mask            63
#define md_to_mask              (63 << 6)
#define md_promotion_type_mask  (3 << 12)
#define md_move_type_mask       (3 << 14)

typedef enum {
   Normal,
   Promotion  = 1 << 14,
   En_passant = 2 << 14,
   Castling   = 3 << 14
} MoveType;

/*!
  @param from: the origin `Square`
  @param to: the destination `Square`
  @param mt: MoveType
  @param pt: (PieceType) promotion type; type of piece to promote to
*/
#define new_md(from, to, pt, mt)   ((mt) | ((pt) << 12) | ((to) << 6) | (from))
#define md_get_square_from(m)      (m & md_from_mask)
#define md_get_square_to(m)        ((m & md_to_mask) >> 6)
#define md_get_move_type(m)        (m & md_move_type_mask)
#define md_get_promotion_type(m)   ((m & md_promotion_type_mask) >> 12)

typedef char CastlingRights;
enum {
   NoCastling,
   White_OO,
   Black_OO       = 1 << 1,
   White_OOO      = 1 << 2,
   Black_OOO      = 1 << 3,
   KingSide      = White_OO  | Black_OO,
   QueenSide     = White_OOO | Black_OOO,
   WhiteCastling = White_OO  | White_OOO,
   BlackCastling = Black_OO  | Black_OOO,
   AnyCastling   = WhiteCastling | BlackCastling,
};

#define WKSCastlingSquaresBB (1ull << F1 | 1ull << G1)
#define BKSCastlingSquaresBB (1ull << F8 | 1ull << G8)
#define WQSCastlingSquaresBB (1ull << C1 | 1ull << D1)
#define BQSCastlingSquaresBB (1ull << C8 | 1ull << D8)
