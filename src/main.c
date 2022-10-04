#include <stdio.h>
#include <unistd.h>

#include "./board.h"
#include "./bitopt.h"
#include "./move_gen.h"

void print_bb(Bitboard n);

int main(void) {
   Board b;
   init_board(&b);
   setbit(b.pos[BN], 1);
   for (PiecePos i = 2; i < 64; ++i) {
      setbit(b.pos[BB], i);
      print_bb(gen_sliding_piece_moves(&b, i, BISHOP));
      putchar('\n');
      unsetbit(b.pos[BB], i);
      usleep(500 * 1000);
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
