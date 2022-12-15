#pragma once

#include <iostream>

#include "./board.hpp"
#include "./bitboard.hpp"

using std::cout;
using namespace Chess;

inline std::ostream& operator<<(std::ostream& os, PieceColor c) {
   return os << (c == White ? "White" : c == Black ? "Black" : "NoColor");
}

inline string pt_to_str(const PieceType pt) {
   if (pt >= 6) return "";
   return array{
      "Knight", "Bishop", "Rook",
      "Queen", "King", "Pawn"
   }[pt];
}

inline std::ostream& operator<<(std::ostream& os, Square sq) {
   if (sq == NoSquare) {
      return os << "No Square";
   }
   else {
      char str[3] = {0};
      str[0] = 'A' + (sq%8);
      str[1] = '1' + (sq/8);
      return os << str;
   }
}
[[maybe_unused]]
inline void print_bb(Bitboard bb) {
   for (int i=7; i >= 0; --i) {
      for (unsigned j=0; j < 8; ++j) {
         cout << ((bb & sqbb( Square(i*8+j) )) ? '1' : '.');
      }
      cout << '\n';
   }
   cout << '\n';
}

static const char pieces_char[] = {'N', 'B', 'R', 'Q', 'K', 'P'};

[[maybe_unused]]
static void print_board(Board& b, PieceColor p = NoColor) {
   for (int i = 7; i >= 0; --i) {
      for (int j = 0; j < 8; ++j) {
         const Square sq = Square(i*8 + j);
         PieceType t = b.get_piece_type(sq);
         if (b.is_square_occupied(sq) && (p == NoColor || b.get_piece_color(sq) == p)) {
            char c = pieces_char[t] + (b.get_piece_color(sq) == White ? 0 : 32);
            cout << c;
         }
         else {
            cout << '.';
         }
      }
      cout << '\n';
   }
   cout << '\n';
}

struct Move {
   Square from;
   Square to;
   PieceType pt;
   Move(Square from, Square to, PieceType pt = Queen) : from{ from }, to{ to }, pt{ pt } {};
   Move() : from{ NoSquare }, to{ NoSquare }, pt{ Queen } {};
};

[[maybe_unused]]
inline string sqstr(const Square sq) {
   string str = "a1";
   str[0] += sq%8;
   str[1] += sq/8;
   return str;
}

inline string str_repeat(const string &str, int n) {
   string res = "";
   for (int i = 0; i < n; ++i) res += str;
   return res;
}

[[maybe_unused]]
inline Bitboard perft(Board &b, int depth, int original_depth = 0) {
   if (depth == 0) return 1ull;

   int pos_num = 0;
   vector<Move> moves{};
   moves.reserve(64);

   for (auto pm: b.movelist) {
      while (pm.possible_moves) {
         Move m{ pm.pos, pop_lsb(pm.possible_moves), Queen };
         Move *move = &m;
         if (b.get_piece_type(pm.pos) == Pawn && (move->to <= H1 || move->to >= A8)) {
            m.pt = Queen;
            // promotion to other pieces
            for (auto pt: { Rook, Bishop, Knight }) {
               Move m = *move;
               m.pt = pt;
               moves.emplace_back(m);
            }
         }
         moves.emplace_back(m);
      }
   }

   int x = 0;
   for (size_t i = 0; i < moves.size(); ++i) {
      string m = sqstr(moves[i].from) + sqstr(moves[i].to);
      if (b.get_piece_type(moves[i].from) == Pawn && (moves[i].to <= H1 || moves[i].to >= A8)) {
         m += pieces_char[moves[i].pt];
      }
      if (b.make_move(moves[i].from, moves[i].to, moves[i].pt) == Chess::InvalidMove) {
         cout << "invalid\n";
         exit(1);
      };
      int j = perft(b, depth - 1, original_depth == 0 ? depth : original_depth);
      x = j;
      pos_num += j;
      b.unmake_move();
      if (depth > 1) cout << str_repeat("-- ", depth) << ' ' << m << ' ' << x << '\n';
   }
   return pos_num;
}
