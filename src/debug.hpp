#pragma once

#include <iostream>

#include "./board.hpp"
#include "./bitopt.hpp"

using std::cout;
using namespace Chess;

inline std::ostream& operator<<(std::ostream& os, PieceColor c) {
   return os << (c == White ? "White" : c == Black ? "Black" : "NoColor");
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

static void print_board(Board& b, PieceColor p) {
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
