#include "./debug.h"

#include <stdio.h>

#include "./bitopt.h"

void print_color(PieceColor c) {
   printf("%s\n", c == WHITE ? "WHITE" : c == BLACK ? "BLACK" : "NONE");
}

void print_bb(Bitboard n) {
   for (int i=7; i >= 0; --i) {
      for (unsigned j=0; j < 8; ++j) {
         putchar(isbitset(n, i*8 + j) ? '1' : '.');
      }
      putchar('\n');
   }
   putchar('\n');
}

