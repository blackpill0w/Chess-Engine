#pragma once

#include <string>
#include <array>

#include "./utils.hpp"

using std::string;
using std::array;

constexpr unsigned WP_START{ 0 };
constexpr unsigned WP_END{ 5 };
constexpr unsigned BP_START{ 6 };
constexpr unsigned BP_END{ 11 };

namespace Chess {

// enum to access pieces' positions
enum : unsigned { WK, WQ, WR, WB, WN, WP, BK, BQ, BR, BB, BN, BP };

class Board {
protected:
   array<Move, 256> move_history{};
   int last_move_index{ -1 };
public:
   Square enpassant_square{ NoSquare };
   array<Bitboard, 12> piecesBB{}; // Pieces' bitboards
   Bitboard white_attacked{};
   Bitboard black_attacked{};
public:
   /*!
     Default constructor, initialize an empty board.
   */
   Board() = default;
   /*!
     TODO
   */
   Board(const string& FEN);
   /*!
     TODO
   */
   void load_FEN(const string& FEN);
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
   /*!
     TODO
   */
   PieceType get_piece_type(const Square s) const;
   /*!
     Check if a position is occupied.
     @param s: the position of the piece to be tested.
     @return true if occupied, otherwise false.
   */
   bool ispos_occupied(const Square s) const;

   /*!
     @return the index of the bitboard containing the piece if it exists.
   */
   unsigned get_piece_at(const Square s) const;
   /*!
     Get color of a piece given its positions.
     @return NONE if the square is empty.
     @return White or Black if square is occupied; the color of the occupying piece.
   */
   PieceColor get_piece_color(const Square s) const;
   /*!
     Generate (pseudo-legal) moves of sliding pieces (queen, rook and bishop).
     @param s: the position of the piece to be tested.
     @param t: type of the piece.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_sliding_piece_moves(const Square s, const PieceType t) const;

   /*!
     Generate legal moves of king.
     @param s: the position of the piece to be tested.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_king_moves(const Square s) const;

   /*!
     Generate (pseudo-legal) moves of knight.
     @param s: the position of the piece to be tested.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_knight_moves(const Square s) const;

   /*!
     Generate pawn push if the target square is empty.
     @param s: the position of the piece to be tested.
     @param c: color of the piece, NONE to determine it automatically.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_pawn_push(const Square s, const PieceColor c = NoColor) const;

   /*!
     Generate pawn push and double push if the target square is empty.
     @param s: the position of the piece to be tested.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_double_push(const Square s) const;

   /*!
     Generates pawn attacks if corresponding positions are occupied by enemy.
     @param a bitboard containing the pawn only.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_pawn_attacks(const Square s) const;

   /*!
     TODO
   */
   void move(const Square from, const Square to);
};

} // namespace Chess
