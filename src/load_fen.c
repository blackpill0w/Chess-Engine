#include "./utils.h"
#include "./board.h"

#include <stdio.h>
#include <regex.h>
#include <ctype.h>

// enum to access tokens
enum { PiecesPos, ColorToPlay,
       CastlingRights, EnPassantPos,
       FiftyMoveRule, HundredMoveRule };

static void fen_add_pieces(Board *b, const char *fen_pieces);

void load_fen(Board *b, char FEN[]) {
   const char *fen_regex_str = "^([kqrbnpKQRBNP1-8]{1,8}\\/){7}[kqrbnpKQRBNP1-8]{1,8} "
      "(w|b) (-|[KQkq]{1,4}) (-|([a-h][0-8])) [1-4]?[0-9] [0-9]{1,3}$";
   regex_t fen_regex;
   int res = regcomp(&fen_regex, fen_regex_str, REG_EXTENDED);
   if (res) {
      regfree(&fen_regex);
      fprintf(stderr, "Error: couldn't compile regex.\n");
      exit(1);
   }
   res = regexec(&fen_regex, FEN, 0, NULL, 0);
   regfree(&fen_regex);
   if (res == REG_NOMATCH) {
      fprintf(stderr, "Error: invalid FEN string.\n");
      exit(1);
   }
   strtok(FEN, " ");
   fen_add_pieces(b, FEN);
}

void fen_add_pieces(Board *b, const char *fen_pieces) {
   int x = 0;
   int y = 7;
   char c = 0;
   while ((c = *fen_pieces++)) {
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
