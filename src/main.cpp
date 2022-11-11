#include <iostream>

#include "./board.hpp"
#include "./debug.hpp"

int main() {
   string fen{ standard_chess };
   Board b{ fen };

   print_board(b, NoColor);

   return 0;
}
