#include "compositeHeuristic.hpp"
#include "Othello.hpp"
#include "coinParityHeuristic.hpp"
#include "cornerHeuristic.hpp"
#include "mobilityHeuristic.hpp"
#include "stabilityHeuristic.hpp"

namespace {
int piecesOnBoard(const Othello &othello) {
  const auto score = othello.score();
  return score.first + score.second;
}
} // namespace

double compositeHeuristic(const Othello &othello) {
  return 10 * coinParityHeuristic(othello) +
         // only consider corners in first 40 moves
         (piecesOnBoard(othello) <= 40 ? 500 * cornerHeuristic(othello) : 0) +
         80 * mobilityHeuristic(othello) + 50 * stabilityHeuristic(othello);
}
