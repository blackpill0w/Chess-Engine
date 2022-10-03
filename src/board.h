#ifndef _BOARD_H
#define _BOARD_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef uint64_t Bitboard;
typedef uint64_t PiecePos;

enum Color {NONE, WHITE, BLACK};
typedef enum Color Color;

enum PieceType {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};
typedef enum PieceType PieceType;

// enum to access pieces' positions
enum {WK = 0, WQ, WR, WB, WN, WP, BK, BQ, BR, BB, BN, BP};
#define PIECES_LEN 12
#define WHITE_PIECES_START 0
#define WHITE_PIECES_END 5
#define BLACK_PIECES_START 6
#define BLACK_PIECES_END 11

typedef struct {
   Bitboard pos[PIECES_LEN]; // Pieces' positions
   Bitboard white_attacked;
   Bitboard black_attacked;
} Board;

#define init_board(b) memset(b, 0, sizeof(Board))
#define white_pieces(b) ((b)->pos[WK] | (b)->pos[WQ] | (b)->pos[WR] | (b)->pos[WB] | (b)->pos[WN] | (b)->pos[WP])
#define black_pieces(b) ((b)->pos[BK] | (b)->pos[BQ] | (b)->pos[BR] | (b)->pos[BB] | (b)->pos[BN] | (b)->pos[BP])
#define all_pieces(b) (white_pieces(b) | black_pieces(b))
#define ispos_occupied(b, pos) ( (all_pieces(b) & (pos)) != 0)

//Bitboard attacked_pos(Board b, Color c);
/*!
  Get color of a piece given its positions.
  @return NONE if the square is empty.
  @return WHITE or BLACK if square is occupied.
*/
Color get_piece_color(const Board *b, const PiecePos pos);

#endif // _BOARD_H
