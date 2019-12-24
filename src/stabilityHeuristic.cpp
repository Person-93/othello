#include "stabilityHeuristic.hpp"
#include "Othello.hpp"
#include "Exception.hpp"


/**
 * A piece is stable when, along all four axes (horizontal, vertical, and each diagonal),
 * it is on a boundary, in a filled row, or next to a stable piece of the same color.
 *
 * A piece is unstable if it can be outflanked this turn
 *
 * Otherwise, it is semi-stable
 */

namespace {
    log4cplus::Logger& GetLogger() {
        static log4cplus::Logger logger = log4cplus::Logger::getInstance( "StabilityHeuristic" );
        return logger;
    }

    enum class Stability {
        SEMI_STABLE, STABLE, UNSTABLE
    };

    using State = std::array<std::array<Stability, Othello::boardSize>, Othello::boardSize>;

    /**
     * The othello object already has a list of pieces that can be captured next turn, so just use that
     * @param othello
     * @param state
     */
    void markUnstablePieces( const Othello& othello, State& state ) {
        for ( const auto&[move, captures]: othello.legalMoves()) {
            for ( const auto& capture: captures ) {
                state[ capture.first ][ capture.second ] = Stability::UNSTABLE;
            }
        }
    }

    void markEdgesAsStable( const Othello::BoardState& boardState, State& state ) {
        const auto checkCorner = [ & ]( int x, int y ) {
            if ( boardState[ x ][ y ] != Othello::State::EMPTY ) {
                const auto xIncrement = x == 0 ? []( int& x ) { ++x; } : []( int& x ) { --x; };
                const auto xCheck     = x == 0 ?
                                        []( int x ) { return x < Othello::boardSize; } :
                                        []( int x ) { return x >= 0; };
                const auto yIncrement = y == 0 ? []( int& y ) { ++y; } : []( int& y ) { --y; };
                const auto yCheck     = y == 0 ?
                                        []( int y ) { return y < Othello::boardSize; } :
                                        []( int y ) { return y >= 0; };

                for ( int i = x; xCheck( i ); xIncrement( i )) {
                    if ( boardState[ i ][ y ] == boardState[ x ][ y ] )
                        state[ i ][ y ] = Stability::STABLE;
                    else
                        break;
                }

                for ( int i = y; yCheck( i ); yIncrement( i )) {
                    if ( boardState[ x ][ i ] == boardState[ x ][ y ] )
                        state[ x ][ i ] = Stability::STABLE;
                    else
                        break;
                }
            }
        };

        checkCorner( 0, 0 );
        checkCorner( 0, Othello::boardSize - 1 );
        checkCorner( Othello::boardSize - 1, 0 );
        checkCorner( Othello::boardSize - 1, Othello::boardSize - 1 );
    }

    bool checkIfPieceIsStable( const Othello::BoardState& boardState, const State& state, int x, int y ) {
        const Othello::State color = boardState[ x ][ y ];
        if ( color == Othello::State::EMPTY )
            THROW_SIMPLE_EXCEPTION( "Tried to check stability of empty tile" );

        {
            // check x-axis
            bool xAxis = false;
            if (( boardState[ x - 1 ][ y ] == color && state[ x - 1 ][ y ] == Stability::STABLE ) ||
                ( boardState[ x + 1 ][ y ] == color && state[ x + 1 ][ y ] == Stability::STABLE ))
                xAxis = true;
            if ( !xAxis ) {
                xAxis = true;
                for ( int i = 0; i < Othello::boardSize; ++i ) {
                    if ( boardState[ i ][ y ] == Othello::State::EMPTY ) {
                        xAxis = false;
                        break;
                    }
                }
            }
            if ( !xAxis ) return false;
        }
        {
            // check y-axis
            bool yAxis = false;
            if (( boardState[ x ][ y - 1 ] == color && state[ x ][ y - 1 ] == Stability::STABLE ) ||
                ( boardState[ x ][ y + 1 ] == color && state[ x ][ y + 1 ] == Stability::STABLE ))
                yAxis  = true;
            if ( !yAxis ) {
                yAxis = true;
                for ( int i = 0; i < Othello::boardSize; ++i ) {
                    if ( boardState[ x ][ i ] == Othello::State::EMPTY ) {
                        yAxis = false;
                        break;
                    }
                }
            }
            if ( !yAxis ) return false;
        }
        {
            // check diagonal
            bool diagonal = false;
            if (( boardState[ x + 1 ][ y + 1 ] == color && state[ x + 1 ][ y + 1 ] == Stability::STABLE ) ||
                ( boardState[ x - 1 ][ y - 1 ] == color && state[ x - 1 ][ y - 1 ] == Stability::STABLE ))
                diagonal  = true;
            if ( !diagonal ) {
                diagonal = true;
                for ( int i = x + 1, j = y + 1; i < Othello::boardSize && j < Othello::boardSize; ++i, ++j ) {
                    if ( boardState[ i ][ j ] == Othello::State::EMPTY ) {
                        diagonal = false;
                        break;
                    }
                }
                if ( diagonal ) {
                    for ( int i = x - 1, j = y - 1; i >= 0 && j >= 0; --i, --j ) {
                        if ( boardState[ i ][ j ] == Othello::State::EMPTY ) {
                            diagonal = false;
                            break;
                        }
                    }
                }
            }
            if ( !diagonal ) return false;
        }
        {
            // check other diagonal
            bool diagonal = false;
            if (( boardState[ x + 1 ][ y - 1 ] == color && state[ x + 1 ][ y - 1 ] == Stability::STABLE ) ||
                ( boardState[ x - 1 ][ y + 1 ] == color && state[ x - 1 ][ y + 1 ] == Stability::STABLE ))
                diagonal  = true;
            if ( !diagonal ) {
                diagonal = true;
                for ( int i = x + 1, j = y - 1; x < Othello::boardSize && y >= 0; ++i, --j ) {
                    if ( boardState[ i ][ j ] == Othello::State::EMPTY ) {
                        diagonal = false;
                        break;
                    }
                }
                if ( diagonal ) {
                    for ( int i = x - 1, j = y + 1; i >= 0 && j < Othello::boardSize; --i, ++j ) {
                        if ( boardState[ i ][ j ] == Othello::State::EMPTY ) {
                            diagonal = false;
                            break;
                        }
                    }
                }
            }
            if ( !diagonal ) return false;
        }
        return true;
    }

    std::pair<int, int> tallyScore( const Othello& othello, const State& state ) {
        int       whitePoints = 0, blackPoints = 0;
        for ( int i           = 0; i < Othello::boardSize; ++i ) {
            for ( int j = 0; j < Othello::boardSize; ++j ) {
                int points;
                switch ( state[ i ][ j ] ) {
                    case Stability::STABLE: points = 2;
                        break;
                    case Stability::UNSTABLE: continue; // no points for an unstable piece
                    case Stability::SEMI_STABLE: points = 1;
                        break;
                }
                switch ( othello.boardState()[ i ][ j ] ) {
                    case Othello::State::BLACK: blackPoints += points;
                        break;
                    case Othello::State::WHITE: whitePoints += points;
                        break;
                    case Othello::State::EMPTY: break;
                }
            }
        }

        return { whitePoints, blackPoints };
    }
}

double stabilityHeuristic( const Othello& othello ) {
    State state{};
    markUnstablePieces( othello, state );
    markEdgesAsStable( othello.boardState(), state );

    bool madeChange;
    do {
        madeChange  = false;
        for ( int i = 1; i < Othello::boardSize - 1; ++i ) {
            for ( int j = 1; j < Othello::boardSize - 1; ++j ) {
                if ( othello.boardState()[ i ][ j ] == Othello::State::EMPTY ) continue;
                if ( state[ i ][ j ] != Stability::SEMI_STABLE ) continue;
                if ( checkIfPieceIsStable( othello.boardState(), state, i, j )) {
                    state[ i ][ j ] = Stability::STABLE;
                    madeChange = true;
                }
            }
        }
    } while ( madeChange );

    auto[whitePoints, blackPoints] = tallyScore( othello, state );
    if ( whitePoints == blackPoints ) return 0;
    double blackScore = 100 * (double) ( blackPoints - whitePoints ) / ( blackPoints + whitePoints );
    return othello.isBlackTurn() ? blackPoints : -blackScore;
}
