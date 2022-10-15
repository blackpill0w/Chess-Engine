#ifndef _MOVE_GEN_H
#define _MOVE_GEN_H

#include "./board.h"
#include "./bitopt.h"

/*!
  Generate (pseudo-legal) moves of sliding pieces (queen, rook and bishop).
  @param b the board (pointer) we're operating with.
  @param p a bitboard containing the piece only.
  @param t type of the piece.

  @return a Bitboard containing the moves.
*/
Bitboard gen_sliding_piece_moves(const Board *b, const PiecePos p, const PieceType t);

/*!
  Generate legal moves of king.
  @param b the board (pointer) we're operating with.
  @param p a bitboard containing the piece only.

  @return a Bitboard containing the moves.
*/
Bitboard gen_king_moves(const Board *b, const PiecePos p);

/*!
  Generate (pseudo-legal) moves of knight.
  @param b the board (pointer) we're operating with.
  @param p a bitboard containing the piece only.

  @return a Bitboard containing the moves.
*/
Bitboard gen_knight_moves(const Board *b, const Bitboard p);

#endif // _MOVE_GEN_H
