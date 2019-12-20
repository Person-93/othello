#pragma once

#include "HeuristicFunction.hpp"
#include "AI.hpp"

class Strategy {
public:
    virtual AI::Move nextMove( HeuristicFunction heuristic, const Othello& othello ) = 0;

    virtual ~Strategy() = default;
};
