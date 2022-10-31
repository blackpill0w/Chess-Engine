#pragma once

#include <stdbool.h>

#include "./arr.h"
#include "./vec.h"
#include "./utils.h"

vec_typedef(Move, Move);
arr_typedef(Bitboard, Bitboard);

static const unsigned WP_START = 0;
static const unsigned WP_END = 5;
static const unsigned BP_START = 6;
static const unsigned BP_END = 11;

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
   Bitboard white_attacked;
   Bitboard black_attacked;
   vecMove move_history;
   vecPieceMoves movelist;
} Board;

/*!
  Default constructor, initialize an empty board.
*/
Board *new_board(char FEN[]);

void free_board(Board *b);

/*!
  TODO
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
  The result is a `Bitboard` that is put in `movelist`.
  It is just a wrapper for `gen_knight_moves()` and other similar functions.
*/
void gen_piece_moves(Board *b, const Square s);

/*!
  TODO
*/
void gen_board_legal_moves(Board *b);

/*!
  Saves move's data.
*/
void save_move(Board *b, const Square from, const Square to);

/*!
  TODO
*/
void move(Board *b, const Square from, const Square to);
