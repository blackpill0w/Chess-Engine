#ifndef _DEBUG_HPP
#define _DEBUG_HPP

#include <iostream>

#include "./board.hpp"
#include "./bitopt.hpp"


using std::cout;

inline void print_color(Chess::PieceColor c) {
    cout << (c == Chess::White ? "White" : c == Chess::Black ? "Black" : "NoColor") << '\n';
}

inline void print_bb(Chess::Bitboard n) {
   for (int i=7; i >= 0; --i) {
      for (unsigned j=0; j < 8; ++j) {
         cout << (isbitset(n, i*8 + j) ? '1' : '.');
      }
      cout << '\n';
   }
   cout << '\n';
}

static const char pieces_char[] = {'K', 'B', 'R', 'Q', 'K', 'P'};

inline void print_board(Chess::Board& b, Chess::PieceColor p = Chess::NoColor) {
   for (int i=7; i >= 0; --i) {
      for (int j=0; j < 8; ++j) {
         const Chess::Square s{i*8 + j};
         if (b.ispos_occupied(s) && (p == Chess::NoColor || b.get_piece_color(s) == p)) {
            Chess::PieceType t{ b.get_piece_type(s) };
            int c{ pieces_char[t] + (b.get_piece_color(s) == Chess::White ? 0 : 32)};
            cout << (char) c;
         }
         else {
            cout << '.';
         }
      }
      cout << '\n';
   }
}
#endif // _DEBUG_HPP
