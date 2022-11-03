#include <stdio.h>

#include "./board.h"
#include "./debug.h"

int main() {
   char FEN[] = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";
   Board *b = new_board(FEN);

   free_board(b);
   return 0;
}
