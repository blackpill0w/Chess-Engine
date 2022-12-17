#pragma once

#include "./board.hpp"
#include "./bitboard.hpp"
#include "./debug.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

struct MoveEval {
   Chess::Move move{};
   int eval = 0;
};

inline int calc_material(const Chess::Board& b) {
   // Knight, Bishop, Rook, Queen, King, Pawn
   static constexpr int piece_value[] = {3, 3, 5, 9, 0, 1};
   int res = 0;
   Chess::Bitboard pieces = b.all_pieces();
   while (pieces) {
      auto sq = Chess::pop_lsb(pieces);
      res += piece_value[b.get_piece_type(sq)] * (b.get_piece_color(sq) == Chess::Black ? 1 : -1);
   }
   return res;
}

// TODO: remove recursion, performance is terrible
inline MoveEval naive_materialistic_play(Chess::Board& b, const unsigned depth) {
   MoveEval best{{}, b.color_to_play == Chess::White ? 99999 : -99999};
   for (auto& m: b.get_moves()) {
      b.make_move(m.from, m.to, m.pt);

      int eval = 0;
      if (b.get_state() == Chess::Checkmate) return {m, eval};
      else if (b.get_state() == Chess::Draw) eval = 0;
      else if (depth == 0) eval = calc_material(b);
      else {
         MoveEval best_move{ naive_materialistic_play(b, depth - 1) };
         eval = best_move.eval;
      }
      if ((b.color_to_play == Chess::White && eval > best.eval)
         || (b.color_to_play == Chess::Black && eval < best.eval)) best.move = m, best.eval = eval;

      b.unmake_move();
   }
   return best;
}
