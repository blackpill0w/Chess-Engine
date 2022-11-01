#pragma once

#include <stdio.h>
#include <string.h>

#include "./board.h"
#include "./bitopt.h"

static char *str_square(const Square s) {
   static char str[] = "No Square";
   if (s == NoSquare) {
      strcpy(str, "No Square");
   }
   else {
      str[0] = 'A' + (s%8);
      str[1] = '1' + (s/8);
      str[2] = 0;
   }
   return str;
}

static void print_bb(Bitboard bb) {
   for (int i=7; i >= 0; --i) {
      for (unsigned j=0; j < 8; ++j) {
         putchar(isbitset(bb, i*8 + j) ? '1' : '.');
      }
      putchar('\n');
   }
   putchar('\n');
}

static const char pieces_char[] = {'K', 'B', 'R', 'Q', 'K', 'P'};

static void print_board(Board *b, PieceColor p) {
   for (int i = 7; i >= 0; --i) {
      for (int j = 0; j < 8; ++j) {
         const Square s = i*8 + j;
         PieceType t = get_piece_type(b, s);
         if (ispos_occupied(b, s) && (p == NoColor || get_piece_color(b, s) == p)) {
            int c = pieces_char[t] + (get_piece_color(b, s) == White ? 0 : 32);
            putchar(c);
         }
         else {
            putchar('.');
         }
      }
      putchar('\n');
   }
   putchar('\n');
}
