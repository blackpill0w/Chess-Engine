#include <unistd.h>
#include <iostream>

#include "./board.hpp"
#include "./bitopt.hpp"
#include "./debug.hpp"
#include "./utils.hpp"

using std::cout;

#include <bitset>

int main() {
   // Chess::Board b{};
   // /* int i = 10; */
   // // for (Chess::Square i = Chess::H8; i >= Chess::A1; --i) {
   // for (Chess::Square i = Chess::A1; i <= Chess::H8; ++i) {
   //    setbit(b.piecesBB[Chess::WP], i);
   //    // print_color(b.get_piece_color(1ul << i));
   //    print_bb(b.all_pieces() | b.gen_pawn_attacks(i));
   //    cout << '\n';
   //    usleep(300 * 1000);
   //    unsetbit(b.piecesBB[Chess::WP], i);
   // }
   // return 0;
   cout << "K: " << std::bitset<16>{ Chess::new_move(Chess::A1, Chess::H8, Chess::Normal, Chess::Knight)} << "\n";
   cout << "B: " << std::bitset<16>{ Chess::new_move(Chess::A1, Chess::H8, Chess::Normal, Chess::Bishop)} << "\n";
   cout << "R: " << std::bitset<16>{ Chess::new_move(Chess::A1, Chess::H8, Chess::Normal, Chess::Rook)} << "\n";
   cout << "Q: " << std::bitset<16>{ Chess::new_move(Chess::A1, Chess::H8, Chess::Normal, Chess::Queen)} << "\n";

}
