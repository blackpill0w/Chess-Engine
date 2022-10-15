#ifndef _MOVE_GEN_H
#define _MOVE_GEN_H

#include "./board.h"
#include "./bitopt.h"

Bitboard gen_sliding_piece_moves(const Board *b, const PiecePos p, const PieceType t);

Bitboard gen_rook_moves(const Board *b, const PiecePos p);

Bitboard gen_king_moves(const Board *b, const PiecePos p);

Bitboard gen_knight_moves(const Board *b, const Bitboard p);

#endif // _MOVE_GEN_H
