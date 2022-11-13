#include "./utils.hpp"
#include "./board.hpp"

#include <iostream>
#include <string>
#include <regex>

namespace Chess {

static size_t gettok_index(string &str, const string &delim);
static void fen_add_pieces(Board *b, const string &fen_pieces);
static void fen_set_castling_rights(Board *b, const string &cr);

void Board::load_fen(string FEN) {
   if (!move_history.empty()) {
      move_history.clear();
   }
   const std::regex fen_regex{ "^([kqrbnpKQRBNP1-8]{1,8}\\/){7}[kqrbnpKQRBNP1-8]{1,8} "
         "(w|b) (-|[KQkq]{1,4}) (-|([a-h][0-8])) [1-4]?[0-9] [0-9]{1,3}$" };
   if (!std::regex_search(FEN, fen_regex)) {
      fprintf(stderr, "Error: invalid FEN notation string.\n");
      exit(1);
   }
   fen_add_pieces(this, FEN.substr(0, gettok_index(FEN, " ")));

   color_to_play = FEN.substr(0, gettok_index(FEN, " "))[0] == 'w' ? White : Black;

   fen_set_castling_rights(this, FEN.substr(0, gettok_index(FEN, " ")));

   gen_board_legal_moves();
}

size_t gettok_index(string &str, const string &delim) {
    static size_t i = string::npos;
    if (i != string::npos) str.erase(0, i + delim.length());
    i = str.find(delim);
    return i;
}

void fen_add_pieces(Board *b, const string &fen_pieces) {
   int x = 0;
   int y = 7;
   for (auto c : fen_pieces) {
      if (c == '/') {
         x = 0;
         --y;
      }
      else if (isdigit(c)) {
         x += c - '0';
      }
      else {
         const char lc = tolower(c);
         int i = lc == 'k' ? WK
            : lc == 'q' ? WQ
            : lc == 'r' ? WR
            : lc == 'b' ? WB
            : lc == 'n' ? WN
            : WP;
         if (islower(c)) {
            i += 6;
         }
         const Bitboard bb = 1ull << (x + y*8);
         b->piecesBB[i] |= bb;
         ++x;
      }
   }
}

void fen_set_castling_rights(Board *b, const string &cr) {
   if (cr[0] == '-') {
      b->cr = NoCastling;
   }
   for (auto c : cr) {
      if (c == 'K') {
         b->cr |= White_OO;
      }
      else if (c == 'Q') {
         b->cr |= White_OOO;
      }
      else if (c == 'k') {
         b->cr |= Black_OO;
      }
      else if (c == 'q') {
         b->cr |= Black_OOO;
      }
   }
}

} // namespace Chess
