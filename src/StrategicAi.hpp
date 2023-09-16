#pragma once

#include "AI.hpp"
#include "Strategy.hpp"
#include <memory>

class StrategicAi : public AI {
public:
  StrategicAi(std::unique_ptr<Strategy> strategy, HeuristicFunction heuristic)
      : strategy{std::move(strategy)}, heuristic{heuristic} {}

  Move go(const Othello &othello) override;

private:
  const std::unique_ptr<Strategy> strategy;
  const HeuristicFunction heuristic;
};
