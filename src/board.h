#ifndef _BOARD_H
#define _BOARD_H

#include <stdint.h>

#include "./utils.h"

// enum to access pieces' positions
enum {WK = 0, WQ, WR, WB, WN, WP, BK, BQ, BR, BB, BN, BP};
#define PIECES_LEN 12
#define WHITE_PIECES_START 0
#define WHITE_PIECES_END 5
#define BLACK_PIECES_START 6
#define BLACK_PIECES_END 11

typedef struct {
   Bitboard pos[PIECES_LEN]; // Pieces' bitboards
   Bitboard white_attacked;
   Bitboard black_attacked;
} Board;

/*!
  Returns an initialized Board (to 0).
*/
#define board_new() (Board) {.pos = {0}, .white_attacked = 0, .black_attacked = 0}

/*!
  Returns a Bitboard of all white pieces.
*/
#define white_pieces(b) ((b)->pos[WK] | (b)->pos[WQ] | (b)->pos[WR] | (b)->pos[WB] | (b)->pos[WN] | (b)->pos[WP])

/*!
  Returns a Bitboard of all black pieces.
*/
#define black_pieces(b) ((b)->pos[BK] | (b)->pos[BQ] | (b)->pos[BR] | (b)->pos[BB] | (b)->pos[BN] | (b)->pos[BP])

/*!
  Returns a Bitboard of all pieces.
*/
#define all_pieces(b) (white_pieces(b) | black_pieces(b))

/*!
  Check if a position is occupied.
  @param b the board (pointer) we're working with.
  @param a Bitboard contaning the position to be tested.
  @return true (1) if occupied, otherwise false (0).
*/
#define ispos_occupied(b, pos) ( (all_pieces(b) & (pos)) != 0)

/*!
  Get opposite color of a color.
  @param c a PieceColor.
  @return NONE if c is NONE, otherwise the opposite color of c.
*/
#define opposite_color(c) (c == WHITE ? BLACK : c == BLACK ? WHITE : NONE)

/*!
  Get color of a piece given its positions.
  @return NONE if the square is empty.
  @return WHITE or BLACK if square is occupied; the color of the occupying piece.
*/
PieceColor get_piece_color(const Board *b, const PiecePos pos);

#endif // _BOARD_H
