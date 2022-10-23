#include <unistd.h>
#include <iostream>

#include "./board.hpp"
#include "./bitopt.hpp"
#include "./debug.hpp"
#include "./utils.hpp"

using std::cout;

#include <bitset>

int main() {
   const std::string FEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
   Chess::Board b{FEN};
   /* int i = 10; */
   // for (Chess::Square i = Chess::H8; i >= Chess::A1; --i) {
   print_board(b);
   // for (Chess::Square i = Chess::A1; i <= Chess::H8; ++i) {
   //    setbit(b.piecesBB[Chess::WP], i);
   //    // print_color(b.get_piece_color(1ul << i));
   //    cout << '\n';
   //    usleep(300 * 1000);
   //    unsetbit(b.piecesBB[Chess::WP], i);
   // }
   return 0;
}
