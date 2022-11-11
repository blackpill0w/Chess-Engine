#pragma once

#include <array>
#include <string_view>
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
enum { WK, WQ, WR, WB, WN, WP, BK, BQ, BR, BB, BN, BP };
// enum used at move generation
enum { gen_default, gen_with_no_block, gen_pseudo, gen_ignoring_enemy_king = 4 };

struct PieceMoves {
   Square pos;
   Bitboard possible_moves;
   PieceMoves() = default;
   PieceMoves(Square s, Bitboard bb) : pos{s}, possible_moves{bb} {};
};

inline const string standard_chess = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

struct Board {
public:
   //static array<array<Bitboard, 64>, 64> betweenBB;
   array<Bitboard, 12> piecesBB; // Pieces' bitboards
   Square enpassant_square;
   PieceColor color_to_play;
   vector<MoveData> move_history;
   vector<PieceMoves> movelist;
   CastlingRights cr;
   Bitboard attacked_by_enemy;
   Checkers checkers;
   Bitboard possible_moves = 0;
public:
   /*!
     Default constructor.
     WARNING: the string MUST be a valid FEN notation, otherwise the engine aborts.
   */
   Board(string FEN);

   /*!
     Load a FEN string into a board and generate legal moves.
   */
   void load_fen(string& FEN);

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
     @return the index of the bitboard containing the piece if it exists.
   */
   size_t get_pieceBB_index(const Square s) const;

   /*!
     Get color of a piece given its positions.
     @return NoColor if the square is empty, otherwise the color of the occupying piece.
   */
   PieceColor get_piece_color(const Square s) const;

   /*!
     Check if a position is occupied.
     @param s: the position of the piece to be tested.
     @return true if occupied, otherwise false.
   */
   bool ispos_occupied(const Square s) const;

   //! Remove a piece from the board.
   void remove_piece_at(const Square s);

   /*!
     Generate (pseudo-legal) moves of sliding pieces (queen, rook and bishop).
     @param s: the position of the piece.
     @param t: type of the piece (Queen/Rook/Bishop).

     @return a Bitboard containing the moves.
   */
   Bitboard gen_sliding_piece_moves(const Square s, const PieceType t,
                                    const int flags = gen_default) const;

   /*!
     Generate legal moves of king.
     @param s: the position of the piece.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_king_moves(const Square s, const int flags = gen_default) const;

   /*!
     Generate (pseudo-legal) moves of knight.
     @param s: the position of the piece.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_knight_moves(const Square s, const int flags = gen_default) const;

   /*!
     Generate pawn push if the target square is empty.
     @param s: the position of the pawn.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_pawn_push(const Square s) const;

   /*!
     Generate pawn push and double push if the target square is empty.
     @param s: the position of the pawn.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_double_push(const Square s) const;

   /*!
     Generates pawn attacks if corresponding positions are occupied by enemy.
     @param s: the position of the pawn.

     @return a Bitboard containing the moves.
   */
   Bitboard gen_pawn_attacks(const Square s, const int flags = gen_default) const;

   /*!
     Generate all possible moves of a piece.
     It is just a wrapper for `gen_knight_moves()` and other similar functions.
   */
   Bitboard gen_piece_moves(const Square s, const int flags = gen_default) const;

   /*!
     Generate all legal moves at a given position, the result is stored
     in `Board::movelist`.
   */
   void gen_board_legal_moves();

   /*!
     TODO
   */
   MoveData gen_move_data(const Square from, const Square to) const;

   /*!
     Gets attackers of a square.
   */
   Bitboard attackers_of(const Square s) const;

   /*!
     Make a move.
     @return 0 on success, -1 if move is invalid.
   */
   int make_move(const Square from, const Square to);

};

} // namesapce Chess
