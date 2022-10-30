#include <unistd.h>
#include <stdio.h>

#include "./board.h"
#include "./bitopt.h"
#include "./debug.h"
#include "./utils.h"

// TODO: moves that are supposed to be valid can not be played

int main() {
   char FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
   Board *b = newBoard();
   load_fen(b, FEN);
   // for (Square i = H8; i >= A1; --i) {
   print_board(b, NoColor);
   move(b, A2, A4);
   print_board(b, NoColor);
   // for (Square i = A1; i <= H8; ++i) {
   //    setbit(b->piecesBB[WP], i);
   //    // print_color(get_piece_color(b, 1ul << i);
   //    putchar('\n');
   //    usleep(300 * 1000);
   //    unsetbit(b->piecesBB[WP], i);
   // }
   freeBoard(b);
   return 0;
}
