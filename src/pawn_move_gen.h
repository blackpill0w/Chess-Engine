#ifndef PAWN_MOVE_GEN_H
#define PAWN_MOVE_GEN_H

#include "./board.h"

/* Bitboard gen_pawn_moves(const Board *b, const PiecePos p); */

/*!
  Generate pawn push if the target square is empty.
  @param b the board (pointer) we're operating with.
  @param p a bitboard containing the piece only.
  @param c color of the piece, NONE to determine it automatically.

  @return a Bitboard containing the moves.
*/
Bitboard gen_pawn_push(const Board *b, const PiecePos p, const PieceColor c);
/*!
  Generate pawn push and double push if the target square is empty.
  @param b (pointer) the board we're operating with.
  @param a bitboard containing the pawn only.

  @return a Bitboard containing the moves.
*/
Bitboard gen_double_push(const Board *b, const PiecePos p);

/*!
  Generates pawn attacks if corresponding positions are occupied by enemy.
  @param b (pointer) the board we're operating with.
  @param a bitboard containing the pawn only.

  @return a Bitboard containing the moves.
*/
Bitboard gen_pawn_attacks(const Board *b, const PiecePos p);

#endif // PAWN_MOVE_GEN_H
