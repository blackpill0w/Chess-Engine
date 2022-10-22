#ifndef _DEBUG_HPP
#define _DEBUG_HPP

#include <iostream>

#include "./board.hpp"
#include "./bitopt.hpp"


using std::cout;

inline void print_color(Chess::PieceColor c) {
    cout << (c == Chess::WHITE ? "WHITE" : c == Chess::BLACK ? "BLACK" : "NONE") << '\n';
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

#endif // _DEBUG_HPP