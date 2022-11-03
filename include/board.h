#pragma once

#include <stdbool.h>

#include "./arr.h"
#include "./vec.h"
#include "./utils.h"

vec_typedef(MoveData, MoveData);
arr_typedef(Bitboard, Bitboard);

// useful indices
#define WP_START   0
#define WP_END     5
#define BP_START   6
#define BP_END     11

// enum to access pieces' positions
enum { WK, WQ, WR, WB, WN, WP, BK, BQ, BR, BB, BN, BP, PiecesLen }; // pieces_len = 12

typedef struct {
   Square piece_pos;
   Bitboard possible_moves;
} PieceMoves;
vec_typedef(PieceMoves, PieceMoves);

typedef struct {
   Bitboard piecesBB[PiecesLen]; // Pieces' bitboards
   Square enpassant_square;
   PieceColor color_to_play;
   vecMoveData move_history;
   vecPieceMoves movelist;
   CastlingRights cr;
   Bitboard white_attacked;
   Bitboard black_attacked;
} Board;

#define STANDARD_CHESS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

/*!
  Default constructor.
  @param FEN: a modifiable array of characters that contains a FEN notation string.
  WARNING: the FEN string MUST be valid, otherwise, the engine aborts.

  @return a pointer to a `Board` object and NULL on error.
*/
Board *new_board(char FEN[]);

//! Free allocated resources.
void free_board(Board *b);

/*!
  Load a FEN string into a board and generate legal moves.
  @param b: a pointer to a `Board` object.
  @parab FEN: a modifiable array of characters that contains a FEN notation string.
*/
void load_fen(Board *b, char FEN[]);

/*!
  Returns a Bitboard containing all white pieces.
*/
Bitboard white_pieces(Board *b);

/*!
  Returns a Bitboard containing all black pieces.
*/
Bitboard black_pieces(Board *b);

/*!
  Returns a Bitboard containing all pieces.
*/
Bitboard all_pieces(Board *b);

/*!
  TODO
*/
PieceType get_piece_type(Board *b, const Square s);

/*!
  @return the index of the bitboard containing the piece if it exists.
*/
unsigned get_pieceBB_index(Board *b, const Square s);

/*!
  Get color of a piece given its positions.
  @return NoColor if the square is empty.
  @return White or Black if square is occupied; the color of the occupying piece.
*/
PieceColor get_piece_color(Board *b, const Square s);

/*!
  Check if a position is occupied.
  @param s: the position of the piece to be tested.
  @return true if occupied, otherwise false.
*/
bool ispos_occupied(Board *b, const Square s);

//! Remove a piece from the board.
void remove_piece_at(Board *b, const Square s);

/*!
  Generate (pseudo-legal) moves of sliding pieces (queen, rook and bishop).
  @param s: the position of the piece to be tested.
  @param t: type of the piece.

  @return a Bitboard containing the moves.
*/
Bitboard gen_sliding_piece_moves(Board *b, const Square s, const PieceType t);

/*!
  Generate legal moves of king.
  @param s: the position of the piece to be tested.

  @return a Bitboard containing the moves.
*/
Bitboard gen_king_moves(Board *b, const Square s);

/*!
  Generate (pseudo-legal) moves of knight.
  @param s: the position of the piece to be tested.

  @return a Bitboard containing the moves.
*/
Bitboard gen_knight_moves(Board *b, const Square s);

/*!
  Generate pawn push if the target square is empty.
  @param s: the position of the piece to be tested.

  @return a Bitboard containing the moves.
*/
Bitboard gen_pawn_push(Board *b, const Square s);

/*!
  Generate pawn push and double push if the target square is empty.
  @param s: the position of the piece to be tested.

  @return a Bitboard containing the moves.
*/
Bitboard gen_double_push(Board *b, const Square s);

/*!
  Generates pawn attacks if corresponding positions are occupied by enemy.
  @param a bitboard containing the pawn only.

  @return a Bitboard containing the moves.
*/
Bitboard gen_pawn_attacks(Board *b, const Square s);

/*!
  Generate all possible moves of a piece.
  The result is a `PieceMoves` struct.
  It is just a wrapper for `gen_knight_moves()` and other similar functions.
*/
PieceMoves gen_piece_moves(Board *b, const Square s);

/*!
  Generate all legal moves at a given position, the result is stored
  in `b->movelist`.
  @param b: a pointer to a `Board` object.
*/
void gen_board_legal_moves(Board *b);

/*!
  Saves move's data.
*/
void save_move(Board *b, const Square from, const Square to);

/*!
  Make a move.
  @return 0 on success, -1 if move is invalid.
*/
int make_move(Board *b, const Square from, const Square to);
