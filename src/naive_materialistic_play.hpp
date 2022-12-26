#pragma once

#include "./board.hpp"
#include "./bitboard.hpp"
#include "./debug.hpp"

#include <cstdint>
#include <limits.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

namespace Chess
{
struct MoveEval {
   Move move{};
   int eval{};
   bool operator<(const MoveEval& other) const { return eval < other.eval; }
   bool operator>(const MoveEval& other) const { return eval > other.eval; }
   bool operator<=(const MoveEval& other) const { return eval <= other.eval; }
   bool operator>=(const MoveEval& other) const { return eval >= other.eval; }
};

inline int calc_material(const Board& b) {
   using namespace Chess;
   // { WN, WB, WR, WQ, WK, WP, BN, BB, BR, BQ, BK, BP }
   static constexpr int piece_value[] = {3, 3, 5, 9, 0, 1, -3, -3, -5, -9, 0, -1};
   int res = 0;
   for (size_t i = 0; i < b.piecesBB.size(); ++i) {
      res += piece_value[i] * popcnt(b.piecesBB[i]);
   }
   return res;
}

inline MoveEval alpha_beta_pruning(Board& b, const unsigned depth,
                                   const bool maximizing_player,
                                   int alpha, int beta)
{
   std::vector<Move> moves{ b.get_moves() };
   // Sort according to priority
   std::sort(moves.begin(), moves.end(),
             [&](const Move& m1, const Move& m2){ return m1.less_than(m2, b); }
      );

   MoveEval res{{}, maximizing_player ? -999 : 999 };
   for (auto& m: moves) {
      b.make_move(m.from, m.to, m.pt);
      MoveEval curr{m, };

      if (depth == 0) {
         curr.eval = calc_material(b);
         res = maximizing_player ? std::max(res, curr) : std::min(res, curr);
      }
      else if (b.get_state() == Checkmate) {
         curr.eval = maximizing_player ? 999 : -999;
         res = curr;
         b.unmake_move();
         break;
      }
      else if (b.get_state() == Draw) res.eval = 0;
      else if (maximizing_player) {
         curr.eval = alpha_beta_pruning(b, depth - 1, false, alpha, beta).eval;
         res = std::max(res, curr);
         alpha = std::max(alpha, curr.eval);
         if (beta <= alpha) {
            b.unmake_move();
            break;
         }
      }
      else {
         curr.eval = alpha_beta_pruning(b, depth - 1, true, alpha, beta).eval;
         res = std::min(res, curr);
         beta = std::min(beta, curr.eval);
         if (beta <= alpha) {
            b.unmake_move();
            break;
         }
      }
      b.unmake_move();
   }
   return res;
}

inline MoveEval naive_materialistic_play(Board b, const unsigned depth) {
   return alpha_beta_pruning(b, depth, b.color_to_play == White, -999, 999);
}

} // namespace Chess
