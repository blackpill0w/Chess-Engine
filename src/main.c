#include <stdio.h>
#include <unistd.h>

#include "./board.h"
#include "./bitopt.h"
#include "./move_gen.h"
#include "./pawn_move_gen.h"
#include "./debug.h"

int main(void) {
   Board b = board_new();
   /* int i = 10; */
   /* for (int i = 63; i >= 0; --i) { */
   for (int i = 0; i < 64; ++i) {
      setbit(b.pos[WQ], i);
      //print_color(get_piece_color(&b, 1ul << i));
      print_bb(gen_sliding_piece_moves(&b, 1ull << i, QUEEN));
      putchar('\n');
      usleep(500 * 1000);
      unsetbit(b.pos[WQ], i);
   }
   return 0;
}

