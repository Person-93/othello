#include "cornerHeuristic.hpp"
#include "Othello.hpp"

double cornerHeuristic(const Othello &othello) {
  int blackCorners = 0;
  int whiteCorners = 0;

  const auto processCorner = [&](int x, int y) {
    switch (othello.boardState()[x][y]) {
    case Othello::State::WHITE:
      ++whiteCorners;
      break;
    case Othello::State::BLACK:
      ++blackCorners;
      break;
    case Othello::State::EMPTY:
      break;
    }
  };

  processCorner(0, 0);
  processCorner(0, Othello::boardSize - 1);
  processCorner(Othello::boardSize - 1, 0);
  processCorner(Othello::boardSize - 1, Othello::boardSize - 1);

  double blackScore = 25 * (blackCorners - whiteCorners);
  return othello.isBlackTurn() ? blackScore : -blackScore;
}
