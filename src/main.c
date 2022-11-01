#include <stdio.h>

#include "./board.h"
#include "./debug.h"

int main() {
   // standard chess: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
   char FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
   Board *b = new_board(FEN);

   print_board(b, NoColor);

   move(b, E2, E4);
   print_board(b, NoColor);
   printf("---- En passant target after move: %s\n", str_square(b->enpassant_square));

   move(b, B7, B5);
   print_board(b, NoColor);
   printf("---- En passant target after move: %s\n", str_square(b->enpassant_square));

   move(b, C2, C3);
   print_board(b, NoColor);
   printf("---- En passant target after move: %s\n", str_square(b->enpassant_square));

   move(b, B5, B4);
   print_board(b, NoColor);
   printf("---- En passant target after move: %s\n", str_square(b->enpassant_square));

   move(b, A2, A4);
   print_board(b, NoColor);
   printf("---- En passant target after move: %s\n", str_square(b->enpassant_square));

   move(b, B4, A3);
   print_board(b, NoColor);
   printf("---- En passant target after move: %s\n", str_square(b->enpassant_square));

   free_board(b);
   return 0;
}
