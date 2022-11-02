#include <stdio.h>

#include "./board.h"
#include "./debug.h"

int main() {
   // standard chess: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
   char FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";
   Board *b = new_board(FEN);

   make_move(b, E1, G1);
   print_board(b, NoColor);

   free_board(b);
   return 0;
}
