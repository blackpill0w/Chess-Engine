#include "./utils.hpp"
#include "./board.hpp"

#include <string>
#include <regex>
#include <iostream>
#include <unordered_map>
#include <array>

using std::array;
using std::regex;
using std::string;
using std::regex_match;
using std::cout;

using FEN_tokens = array<string, 6>;
// enum to access tokens
enum : unsigned { PiecesPos, ColorToPlay,
                  CastlingRights, EnPassantPos,
                  FiftyMoveRule, HundredMoveRule };

namespace Chess
{

static void load_FEN_tokens(FEN_tokens& tokens, const string& FEN);
static size_t find_nth(const string &s, const char delim, int n = 1);
static void add_pieces(Board *b, const string &fen_pieces);

void Board::load_FEN(const string& FEN) {
   const regex fen_regex{"^([kqrbnpKQRBNP1-8]{1,8}\\/){7}[kqrbnpKQRBNP1-8]{1,8} "
               "(w|b) (-|[KQkq]{1,4}) (-|([a-h][0-8])) [1-4]?[0-9] [0-9]{1,3}$"};
   if (!regex_match(FEN, fen_regex)) {
      std::cerr << "Error loading the board: invalid FEN string." << std::endl;
      exit(1);
   }
   FEN_tokens tokens{};
   load_FEN_tokens(tokens, FEN);
   add_pieces(this, tokens[PiecesPos]);
}

static void load_FEN_tokens(FEN_tokens& tokens, const string& FEN) {
   for (int i = 0; i < tokens.size(); ++i) {
      size_t begin{ find_nth(FEN, ' ', i) };
      if (begin != 0) {
         ++begin;
      }
      size_t end{ find_nth(FEN, ' ', i+1) - begin };
      tokens[i] = FEN.substr(begin, end);
   }
}

static size_t find_nth(const string &s, const char delim, int n) {
   if (n == 0) {
      return 0;
   }
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


static const std::unordered_map<char, unsigned> char_to_piecesI{
   {'k', WK}, {'q', WQ},
   {'r', WR}, {'b', WB},
   {'n', WN}, {'p', WP},
};
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
         unsigned i { char_to_piecesI.at( tolower(c) ) };
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
