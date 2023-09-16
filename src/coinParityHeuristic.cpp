#include "coinParityHeuristic.hpp"
#include "Othello.hpp"

double coinParityHeuristic(const Othello &othello) {
  auto [blackCoins, whiteCoins] = othello.score();
  if (whiteCoins == blackCoins)
    return 0;
  double blackScore =
      100 * (double)(blackCoins - whiteCoins) / (blackCoins + whiteCoins);
  return othello.isBlackTurn() ? blackScore : -blackScore;
}
