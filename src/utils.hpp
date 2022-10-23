#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <array>
#include <cstdint>

using std::array;

namespace Chess
{

inline constexpr unsigned invalid_index{ 99999999 };

typedef uint64_t Bitboard;
typedef uint64_t PiecePos;

enum PieceColor : int {NoColor, White, Black};
inline PieceColor opposite_color(PieceColor c) {
   return (c == White ? Black : c == Black ? White : NoColor);
}
inline int pawn_direction(PieceColor c) {
   return (c == White ? 1 : -1);
}

enum PieceType : int {Knight, Bishop, Rook, Queen, King, Pawn, NoType};

inline constexpr array<Bitboard, 8> FILE_MASKS {
   0x0101010101010101,
   0x0202020202020202,
   0x0404040404040404,
   0x0808080808080808,
   0x1010101010101010,
   0x2020202020202020,
   0x4040404040404040,
   0x8080808080808080,
};

enum File : int {
   FILE_A,
   FILE_B,
   FILE_C,
   FILE_D,
   FILE_E,
   FILE_F,
   FILE_G,
   FILE_H,
};

inline constexpr array<Bitboard, 8> RANK_MASKS {
   255ull,
   255ull << 8,
   255ull << 16,
   255ull << 24,
   255ull << 32,
   255ull << 40,
   255ull << 48,
   255ull << 56,
};

enum Rank : int {
   RANK_1,
   RANK_2,
   RANK_3,
   RANK_4,
   RANK_5,
   RANK_6,
   RANK_7,
   RANK_8,
};

enum Direction : int {
   North,
   South,
   East,
   West,
   NoEa,
   NoWe,
   SoEa,
   SoWe,
};

inline constexpr array<int, 8> DIR {
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

inline constexpr unsigned ROOK_DIR_START{ 0 };
inline constexpr unsigned ROOK_DIR_END{ 3 };
inline constexpr unsigned BISHOP_DIR_START{ 4 };
inline constexpr unsigned BISHOP_DIR_END{ 7 };

enum Square : int {
   A1, B1, C1, D1, E1, F1, G1, H1,
   A2, B2, C2, D2, E2, F2, G2, H2,
   A3, B3, C3, D3, E3, F3, G3, H3,
   A4, B4, C4, D4, E4, F4, G4, H4,
   A5, B5, C5, D5, E5, F5, G5, H5,
   A6, B6, C6, D6, E6, F6, G6, H6,
   A7, B7, C7, D7, E7, F7, G7, H7,
   A8, B8, C8, D8, E8, F8, G8, H8,
   NO_SQUARE
};

#define ENABLE_BASE_OPERATORS_FOR(T)                                \
constexpr T operator+(T d1, int d2) { return T(int(d1) + d2); }    \
constexpr T operator-(T d1, int d2) { return T(int(d1) - d2); }    \
constexpr T operator-(T d) { return T(-int(d)); }                  \
inline T& operator+=(T& d1, int d2) { return d1 = d1 + d2; }       \
inline T& operator-=(T& d1, int d2) { return d1 = d1 - d2; }

#define ENABLE_INCR_OPERATORS_FOR(T)                                \
inline T& operator++(T& d) { return d = T(int(d) + 1); }           \
inline T& operator--(T& d) { return d = T(int(d) - 1); }

#define ENABLE_OPERATORS_FOR(T)  \
ENABLE_BASE_OPERATORS_FOR(T)     \
ENABLE_INCR_OPERATORS_FOR(T)


ENABLE_OPERATORS_FOR(Square)

/*
  bit 0-5: the origin square
  bit 6-11: the target square
  bit 12-13: promotion type
  bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
*/
typedef uint16_t Move;

enum MoveType {
   Normal,
   Promotion = 1 << 14,
   En_passant = 2 << 14,
   Castling = 3 << 14
};

inline Move new_move(const Square from, const Square to,
                     const MoveType mt = Normal,
                     const PieceType pt = Queen) {
   return mt | (pt << 12) | (to << 6) | from;
}

} // namespace Chess

#endif // _UTILS_HPP
