#include "./utils.hpp"
#include "./board.hpp"

#include <string>
#include <regex>
#include <iostream>

using std::regex;
using std::string;
using std::regex_match;


namespace Chess
{

static size_t find_nth(const string &s, const char delim, int n = 1);
static void add_pieces(Board *b, const string &fen_pieces);

void Board::load_FEN(const string& FEN) {
   const regex fen_regex{"^([kqrbnpKQRBNP1-8]{1,8}\\/){7}[kqrbnpKQRBNP1-8]{1,8} "
                   "(w|b) (-|[KQkq]{1,4}) (-|([a-h][0-8])) [1-4]?[0-9] [0-9]{1,3}$"};
   if (!regex_match(FEN, fen_regex)) {
      std::cerr << "Error loading the board: invalid FEN string." << std::endl;
      exit(1);
   }
   size_t i{ find_nth(FEN, ' ') };
   add_pieces(this,  FEN.substr(0, i));
}

static size_t find_nth(const string &s, const char delim, int n) {
   size_t pos{ string::npos };
   size_t i{ 0 };
   while (i < s.length()) {
      if (s[i] == delim) {
         --n;
      }
      if (n == 0) {
         pos = i;
         break;
      }
      ++i;
   }
   return pos;
}

void add_pieces(Board *b, const string& fen_pieces) {
   int x{ 0 };
   int y{ 7 };
   for (auto c: fen_pieces) {
      if (c == '/') {
         x = 0;
         --y;
      }
      else if (isdigit(c)) {
         x += c - '0';
      }
      else {
         int i{ 0 };
         if (tolower(c) == 'k') {
            i = WK;
         }
         else if (tolower(c) == 'q') {
            i = WQ;
         }
         else if (tolower(c) == 'r') {
            i = WR;
         }
         else if (tolower(c) == 'b') {
            i = WB;
         }
         else if (tolower(c) == 'n') {
            i = WN;
         }
         else if (tolower(c) == 'p') {
            i = WP;
         }
         if (islower(c)) {
            i += 6;
         }
         const Bitboard bb{ 1ull << (x + y*8) };
         b->piecesBB.at(i) |= bb;
         ++x;
      }
   }
}

} // namespace Chess
