#pragma once

#include <iostream>

#include "./board.hpp"
#include "./bitopt.hpp"
#include "./bitboard.hpp"

using std::cout;
using namespace Chess;

inline std::ostream& operator<<(std::ostream& os, PieceColor c) {
   return os << (c == White ? "White" : c == Black ? "Black" : "NoColor");
}

inline std::ostream& operator<<(std::ostream& os, PieceType pt) {
   return os << (
      pt == King ? "King"
      : pt == Queen ? "Queen"
      : pt == Rook ? "Rook"
      : pt == Bishop ? "Bishop"
      : pt == Knight ? "Knight"
      : pt == Pawn ? "Pawn"
      : "NoType"
   );
}

inline std::ostream& operator<<(std::ostream& os, Square sq) {
   if (sq == NoSquare) {
      return os << "No Square";
   }
   else {
      char str[3] = {0};
      str[0] = 'A' + (sq%8);
      str[1] = '1' + (sq/8);
      return os << str;
   }
}
[[maybe_unused]]
inline void print_bb(Bitboard bb) {
   for (int i=7; i >= 0; --i) {
      for (unsigned j=0; j < 8; ++j) {
         cout << (isbitset(bb, i*8 + j) ? '1' : '.');
      }
      cout << '\n';
   }
   cout << '\n';
}

static const char pieces_char[] = {'N', 'B', 'R', 'Q', 'K', 'P'};
[[maybe_unused]]
static void print_board(Board& b, PieceColor p = NoColor) {
   for (int i = 7; i >= 0; --i) {
      for (int j = 0; j < 8; ++j) {
         const Square sq = Square(i*8 + j);
         PieceType t = b.get_piece_type(sq);
         if (b.is_square_occupied(sq) && (p == NoColor || b.get_piece_color(sq) == p)) {
            char c = pieces_char[t] + (b.get_piece_color(sq) == White ? 0 : 32);
            cout << c;
         }
         else {
            cout << '.';
         }
      }
      cout << '\n';
   }
   cout << '\n';
}

struct Move {
   Square from;
   Square to;
   PieceType pt = Queen;
   Move(Square from, Square to) : from{ from }, to{ to } {};
   Move() : from{ NoSquare }, to{ NoSquare } {};
};

[[maybe_unused]]
inline string sqstr(const Square sq) {
   string str = "xx";
   str[0] = 'A' + (sq%8);
   str[1] = '1' + (sq/8);
   return str;
}

[[maybe_unused]]
inline Bitboard perft(Board &b, int depth, string prev = "") {
   if (depth == 0) return 1ull;

   int pos_num = 0;
   vector<Move> moves{};
   moves.reserve(64);

   for (auto pm: b.movelist) {
      while (pm.possible_moves) {
         moves.push_back( {pm.pos, pop_lsb(pm.possible_moves)} );
         Move *move = &moves[moves.size() - 1];
         if (b.get_piece_type(move->from) == Pawn && (move->to <= H1 || move->to >= A8)) {

            // promotion to other pieces
            for (auto pt: { Rook, Bishop, Knight }) {
               Move m = *move;
               m.pt = pt;
               moves.push_back(m);
            }
         }
      }
   }

   for (auto &move: moves) {
      //print_board(b);
      string prev2 = prev + sqstr(move.from) + " -> " + sqstr(move.to) + " ~~ ";
      //cout << prev2 << '\n';
      if (b.make_move(move.from, move.to, move.pt) == Chess::InavlidMove) {
         cout << "invalid\n";
         exit(1);
      };
      pos_num += perft(b, depth - 1, prev2);
      b.unmake_move();
   }
   return pos_num;
}
