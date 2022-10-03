#include <stdio.h>
#include <unistd.h>

#include "./board.h"
#include "./bitopt.h"
#include "./move_gen.h"

void print_bb(Bitboard n);

int main(void) {
   Board b;
   init_board(&b);
   for (int i = 0; i < 64; ++i) {
      print_bb(gen_rook_moves(&b, i));
      putchar('\n');
      sleep(1);
   }
   return 0;
}

void print_bb(Bitboard n) {
   for (int i=7; i >= 0; --i) {
      for (unsigned j=0; j < 8; ++j) {
         putchar(isbitset(n, i*8 + j) ? '1' : '.');
      }
      putchar('\n');
   }
}
