#pragma once

#include <cstdint>
#include <array>
#include <string>

using std::string;
using std::array;

#define ENABLE_BASE_OPERATORS_ON(T)                                \
constexpr T operator+(T d1, int d2) { return T(int(d1) + d2); }    \
constexpr T operator-(T d1, int d2) { return T(int(d1) - d2); }    \
constexpr T operator-(T d) { return T(-int(d)); }                  \
inline T& operator+=(T& d1, int d2) { return d1 = d1 + d2; }       \
inline T& operator-=(T& d1, int d2) { return d1 = d1 - d2; }

#define ENABLE_INCR_OPERATORS_ON(T)                                \
inline T& operator++(T& d) { return d = T(int(d) + 1); }           \
inline T& operator--(T& d) { return d = T(int(d) - 1); }

#define ENABLE_OPERATORS_ON(T)                  \
   ENABLE_BASE_OPERATORS_ON(T)                  \
   ENABLE_INCR_OPERATORS_ON(T)


namespace Chess
{

inline size_t constexpr invalid_index = 99999999;

using Bitboard = uint64_t;
using PiecePos = uint64_t;

enum PieceColor { White, Black, NoColor };
inline constexpr PieceColor operator~(PieceColor c) {
   return PieceColor(c ^ 1);
}
inline constexpr int pawn_direction(PieceColor c) {
   return c == White ? 1 : -1;
}

enum PieceType { Knight, Bishop, Rook, Queen, King, Pawn, NoType };

enum Square {
   A1, B1, C1, D1, E1, F1, G1, H1,
   A2, B2, C2, D2, E2, F2, G2, H2,
   A3, B3, C3, D3, E3, F3, G3, H3,
   A4, B4, C4, D4, E4, F4, G4, H4,
   A5, B5, C5, D5, E5, F5, G5, H5,
   A6, B6, C6, D6, E6, F6, G6, H6,
   A7, B7, C7, D7, E7, F7, G7, H7,
   A8, B8, C8, D8, E8, F8, G8, H8,
   NoSquare
};

ENABLE_OPERATORS_ON(Square);

enum File {
   fileA, fileB, fileC, fileD, fileE, fileF, fileG, fileH
};
inline constexpr Bitboard _fileA = 0x0101010101010101;
inline constexpr File get_file(Square s) { return File(s % 8); }
inline constexpr Bitboard file_bb(File chess_file) { return _fileA << chess_file; }
inline constexpr Bitboard file_bb(Square sq) { return file_bb(get_file(sq)); }
inline constexpr bool squares_on_same_file(Square s1, Square s2) { return ((s1 ^ s2) & 7) == 0; }

enum Rank { rank1, rank2, rank3, rank4, rank5, rank6, rank7, rank8 };

inline constexpr Bitboard _rank1 = 255;
inline constexpr Rank get_rank(Square s) { return Rank(s / 8); }
inline constexpr Bitboard rank_bb(Rank chess_rank) { return _rank1 << (8*chess_rank); }
inline constexpr Bitboard rank_bb(Square sq) { return rank_bb(get_rank(sq)); }
inline constexpr bool squares_on_same_rank(Square s1, Square s2) { return ((s1 ^ s2) & 56) == 0; }

enum Direction {
   North,
   South,
   East,
   West,
   NoEa,
   NoWe,
   SoEa,
   SoWe,
};

static constexpr array<int, 8> dir = {
    // Rook directions
    8, -8, 1, -1,
    // Bishop directions
    9, 7, -7, -9,
};
// Useful indices
inline constexpr size_t rook_dir_start    = 0;
inline constexpr size_t rook_dir_end      = 3;
inline constexpr size_t bishop_dir_start  = 4;
inline constexpr size_t bishop_dir_end    = 7;


/*
  bit 0-5: the origin square
  bit 6-11: the target square
  bit 12-13: promotion type
  bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
*/
using MoveData = uint16_t;
inline constexpr Bitboard md_from_mask           = 63;
inline constexpr Bitboard md_to_mask             = 63 << 6;
inline constexpr Bitboard md_promotion_type_mask = 3 << 12;
inline constexpr Bitboard md_move_type_mask      = 3 << 14;

enum MoveType {
   Normal,
   Promotion  = 1 << 14,
   En_passant = 2 << 14,
   Castling   = 3 << 14
};
inline constexpr MoveData new_md(Square from, Square to, PieceType pt, MoveType mt) {
   return mt | (pt << 12) | (to << 6) | from;
}
inline constexpr Bitboard md_get_square_from(MoveData m) {
   return m & md_from_mask;
}
inline constexpr Bitboard md_get_square_to(MoveData m) {
   return (m & md_to_mask) >> 6;
}
inline constexpr Bitboard md_get_move_type(MoveData m) {
   return m & md_move_type_mask;
}
inline constexpr Bitboard md_get_promotion_type(MoveData m) {
   return (m & md_promotion_type_mask) >> 12;
}

using CastlingRights = char;
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

inline constexpr Bitboard WKSCastlingSquaresBB = (1ull << F1) | (1ull << G1);
inline constexpr Bitboard BKSCastlingSquaresBB = (1ull << F8) | (1ull << G8);
inline constexpr Bitboard WQSCastlingSquaresBB = (1ull << C1) | (1ull << D1);
inline constexpr Bitboard BQSCastlingSquaresBB = (1ull << C8) | (1ull << D8);

/*
  0-5   bits: first checker position
  6-8   bits: first checker piece type
  9-14  bits: second checker position
  15-17 bits: second checker piece type
*/
using Checkers = uint32_t;

inline Checkers add_checker(const PieceType pt, const Square pos,
                            const bool first = true, const Checkers c = 0) {
   return first ? ((pt << 6) | pos) : (pt << 15) | (pos << 9) | c;
}

inline Square checkers_get_pos(const Checkers c, const bool first = true) {
   return first ? Square(c & 63) : Square((c & (63 << 9)) >> 9);
}

inline PieceType checkers_get_pt(const Checkers c, const bool first = true) {
   return first ? PieceType((c & (7 << 6)) >> 6) : PieceType((c & (7 << 15)) >> 15);
}

} // namespace Chess
