#pragma once

#include <unordered_map>
#include "./board.hpp"

namespace Chess {

static const std::unordered_map<Square, int> piece_value_on_square{};

//* Evaluation function, for now it just counts material.
inline int evaluate(const Board &b) {
  using namespace Chess;
  // { WN, WB, WR, WQ, WK, WP, BN, BB, BR, BQ, BK, BP }
  static constexpr array<int, 12> piece_value = {3, 3, 5, 9, 0, 1, -3, -3, -5, -9, 0, -1};
  int res                                     = 0;
  for (Square sq = A1; sq <= H8; ++sq) {
  }
  return res;
}
}  // namespace Chess
