#pragma once

#include <array>
#include <vector>
#include <string>

#include "./utils.hpp"

using std::string;
using std::array;
using std::vector;

namespace Chess
{
// useful indices
inline constexpr size_t wp_start = 0;
inline constexpr size_t wp_end   = 5;
inline constexpr size_t bp_start = 6;
inline constexpr size_t bp_end   = 11;

// enum to access pieces' positions
enum { WN, WB, WR, WQ, WK, WP, BN, BB, BR, BQ, BK, BP };

struct PieceMoves {
   Square pos;
   Bitboard possible_moves;
   PieceMoves() : pos{ NoSquare }, possible_moves{} {};
   PieceMoves(Square sq, Bitboard bb) : pos{sq}, possible_moves{bb} {};
};

inline const string standard_chess = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

enum BoardState { NoErr, InavlidMove, NoMoveToUnmake, Draw, CheckMate };

struct Board {
public:
   // A list of bitboards, one for each color-piece combination
   array<Bitboard, 12> piecesBB;
   // Save en passant target square if it exists
   Square enpassant_square;
   // The color of the pieces to play
   PieceColor color_to_play;
   // A list of previously played moves
   vector<MoveData> move_history;
   // The list of moves each piece (with color to play) can make
   vector<PieceMoves> movelist;
   // Castling rights
   CastlingRights cr;
   // A bitboard containing the squares attacked by the enemy
   Bitboard attacked_by_enemy;
   // A bitboard containing the pieces putting the king (with color to play) in check
   Bitboard checkers;
   // This variable is used to restrict movement of pieces when in check, etc
   Bitboard possible_moves = ~0;
public:
   /*!
     Default constructor.
     @param FEN: a FEN notation string.

     WARNING: the string MUST be a valid FEN notation, otherwise the engine aborts.
   */
   Board(const string &FEN);

   /*!
     Load a FEN string into a board and generate legal moves.
   */
   void load_fen(string FEN);

   /*!
     Returns a Bitboard containing all white pieces.
   */
   Bitboard white_pieces() const;

   /*!
     Returns a Bitboard containing all black pieces.
   */
   Bitboard black_pieces() const;

   /*!
     Returns a Bitboard containing all pieces.
   */
   Bitboard all_pieces() const;

   /*
      TODO
   */
   Bitboard get_pieces(const PieceColor c, const PieceType pt) const;

   /*!
      Get the type of a piece.
      If the square is empty, `NoType` is returned.
   */
   PieceType get_piece_type(const Square sq) const;

   /*!
      Return the index of the bitboard containing the piece if it exists.
   */
   size_t get_pieceBB_index(const Square sq) const;
   size_t get_pieceBB_index(const PieceType pt, const PieceColor c) const;

   /*!
      Get color of a piece given its positions.
      If the square is empty, `NoColor` is returned.
   */
   PieceColor get_piece_color(const Square sq) const;

   /*!
     Check if a square is occupied.
   */
   bool is_square_occupied(const Square sq) const;

   //! Remove a piece from the board.
   void remove_piece_at(const Square sq);

   /*!
      Generate all possible moves of a piece.
      It is just a wrapper for `gen_knight_moves()` and other similar functions.
   */
   Bitboard gen_piece_moves(const Square sq, const Bitboard occ, const Bitboard mypieces) const;

   /*
      TODO
   */
   void limit_moves_of_pinned_pieces();

   /*!
      Generate all legal moves at a given position, the result is stored
      in `Board::movelist`.
   */
   void gen_board_legal_moves();

   /*!
      TODO
   */
   MoveData gen_move_data(const Square from, const Square to, const PieceType promote_to) const;

   /*!
      Gets attackers of a square.
   */
   Bitboard attackers_of(const Square sq) const;

   /*
      TODO
   */
   void change_piece_pos(Square from, Square to);

   /*
      TODO
   */
   bool is_valid_move(const Square from, const Square to) const;

   /*!
      Make a move.
      @return `NoError` on success, `InvalidMove` if move is invalid.
   */
   BoardState make_move(const Square from, const Square to, const PieceType promote_to = Queen);

   BoardState unmake_move();

};

} // namesapce Chess
