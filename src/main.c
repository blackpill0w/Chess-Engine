#include <stdio.h>
#include <unistd.h>

#include "./board.h"
#include "./bitopt.h"
#include "./move_gen.h"
#include "./pawn_move_gen.h"

void print_color(PieceColor c);
void print_bb(Bitboard n);

int main(void) {
   Board b;
   init_board(&b);
   /* int i = 10; */
   for (int i = 0; i < 64; ++i) {
   /* for (int i = 63; i >= 0; --i) { */
      setbit(b.pos[WK], i);
      //print_color(get_piece_color(&b, 1ul << i));
      print_bb(gen_king_moves(&b, 1ul << i));
      putchar('\n');
      unsetbit(b.pos[WK], i);
      usleep(300 * 1000);
   }
   return 0;
}

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
}
