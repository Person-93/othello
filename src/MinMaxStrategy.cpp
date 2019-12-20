#include "MinMaxStrategy.hpp"
#include "Othello.hpp"


struct MinMaxStrategy::Node {
    Othello  othello;
    AI::Move move;
    double   score;
};

MinMaxStrategy::MinMaxStrategy( int maxDepth ) : maxDepth{ maxDepth } {}

AI::Move MinMaxStrategy::nextMove( HeuristicFunction heuristic, const Othello& othello ) {
    Node origin{
            .othello = othello,
            .move = { -1, -1 },
            .score = std::numeric_limits<double>::min()
    };
    Node node = minimax( heuristic, origin, 0, true );
    return node.move;
}

MinMaxStrategy::Node
MinMaxStrategy::minimax( HeuristicFunction heuristic,
                         const MinMaxStrategy::Node& node,
                         int depth,
                         bool maximizingPlayer ) {
    if ( depth == maxDepth || node.othello.legalMoves().empty())
        return node;

    Node value{
            .othello = node.othello,
            .move = { -1, -1 },
            .score = maximizingPlayer ? std::numeric_limits<double>::min() : std::numeric_limits<double>::max()
    };
    for ( const auto&[move, capture]: node.othello.legalMoves()) {
        Node child         = makeNode( heuristic, node.othello, move );
        Node opponentChild = minimax( heuristic, child, depth + 1, !maximizingPlayer );
        if ( maximizingPlayer ) {
            if ( opponentChild.score > value.score ) value = opponentChild;
        }
        else {
            if ( opponentChild.score < value.score ) value = opponentChild;
        }
    }
    if ( depth > 0 ) value.move = node.move;
    return value;
}

MinMaxStrategy::Node MinMaxStrategy::makeNode( HeuristicFunction heuristic, const Othello& othello, AI::Move move ) {
    Node node{
            .othello = othello,
            .move = move
    };
    node.othello.placePiece( move.first, move.second );
    node.score = heuristic( node.othello );
    return node;
}
