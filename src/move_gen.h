#ifndef _MOVE_GEN_H
#define _MOVE_GEN_H

#include "./board.h"
#include "./bitopt.h"

Bitboard gen_knight_moves(const Board *b, const Bitboard np);

Bitboard gen_rook_moves(const Board *b, Bitboard rp);

Bitboard gen_bishop_moves(const Board *b, Bitboard bp);

Bitboard gen_queen_moves(const Board *b, const Bitboard qp);

#endif // _MOVE_GEN_H
