#ifndef PAWN_MOVE_GEN_H
#define PAWN_MOVE_GEN_H

Bitboard gen_pawn_moves(const Board *b, const PiecePos p);
Bitboard gen_pawn_push(const Board *b, const PiecePos p, const PieceColor c);
Bitboard gen_double_push(const Board *b, const PiecePos p);

#endif // PAWN_MOVE_GEN_H
