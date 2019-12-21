#include "mobilityHeuristic.hpp"
#include "Othello.hpp"


double mobilityHeuristic( const Othello& othello ) {
    int emptySpaces = 0;
    for ( const auto& column: othello.boardState())
        for ( const auto& cell: column )
            if ( cell == Othello::State::EMPTY )
                ++emptySpaces;
    if ( emptySpaces == 0 ) return 0;
    return 100 * (double) ( emptySpaces - othello.legalMoves().size()) / emptySpaces;
}
