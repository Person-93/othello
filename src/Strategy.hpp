#pragma once

#include "AI.hpp"
#include "HeuristicFunction.hpp"

class Strategy {
public:
  virtual AI::Move nextMove(HeuristicFunction heuristic,
                            const Othello &othello) = 0;

  virtual ~Strategy() = default;
};
