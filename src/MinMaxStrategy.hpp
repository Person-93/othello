#pragma once

#include "Strategy.hpp"

class MinMaxStrategy : public Strategy {
public:
    explicit MinMaxStrategy( int maxDepth );

    AI::Move nextMove( HeuristicFunction heuristic, const Othello& othello ) override;

private:
    struct Node;

    MinMaxStrategy::Node minimax( HeuristicFunction heuristic,
                                  const MinMaxStrategy::Node& node,
                                  int depth,
                                  bool maximizingPlayer );

    static MinMaxStrategy::Node makeNode( HeuristicFunction heuristic, const Othello& othello, AI::Move move );

    const int maxDepth;
};
