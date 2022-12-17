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
   using namespace Chess;
   // { WN, WB, WR, WQ, WK, WP, BN, BB, BR, BQ, BK, BP }
   static constexpr int piece_value[] = {3, 3, 5, 9, 0, 1, -3, -3, -5, -9, 0, -1};
   int res = 0;
   for (auto i : { WN, WB, WR, WQ, WK, WP, BN, BB, BR, BQ, BK, BP }) {
      res += piece_value[i] * Chess::popcnt(b.piecesBB[i]);
   }
   return res;
}

// TODO: remove recursion, performance is terrible
inline MoveEval naive_materialistic_play(Chess::Board& b, const unsigned depth) {
   std::vector<MoveEval> res;
   res.reserve(64);
   for (auto& m: b.get_moves()) {
      b.make_move(m.from, m.to, m.pt);
      int eval = 0;
      if (b.get_state() == Chess::Checkmate) eval = b.color_to_play == Chess::White ? -99999 : +99999;
      else if (b.get_state() == Chess::Draw) eval = 0;
      else if (depth == 0) eval = calc_material(b);
      else {
         MoveEval best_move{ naive_materialistic_play(b, depth - 1) };
         eval = best_move.eval;
      }

      res.emplace_back(MoveEval{m, eval});
      b.unmake_move();
   }
   auto best = b.color_to_play == Chess::White ?
      *std::max_element(res.begin(), res.end(), [](const MoveEval& me1, const MoveEval& me2){ return me1.eval > me2.eval; })
      :*std::min_element(res.begin(), res.end(), [](const MoveEval& me1, const MoveEval& me2){ return me1.eval < me2.eval; });
   return best;
}
