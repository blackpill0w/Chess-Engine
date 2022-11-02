#include <stdio.h>

#include "./board.h"
#include "./debug.h"

int main() {
   // standard chess: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
   char FEN[] = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";
   Board *b = new_board(FEN);

   make_move(b, E1, C1);
   make_move(b, E8, G8);
   print_board(b, NoColor);

   free_board(b);
   return 0;
}
