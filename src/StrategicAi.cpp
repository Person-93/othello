#include "StrategicAi.hpp"
#include "Othello.hpp"
#include "util/define_logger.hpp"
#include "Exception.hpp"

DEFINE_LOGGER( StrategicAi )

AI::Move StrategicAi::go( const Othello& othello, bool isBlack ) {
    switch ( othello.legalMoves().size()) {
        case 0: THROW_SIMPLE_EXCEPTION( "No legal moves available" );
        case 1: return othello.legalMoves().begin()->first;
        default: return strategy->nextMove( heuristic, othello );
    }
}
