#include "Othello.hpp"
#include <iostream>
#include "util/define_logger.hpp"

namespace {
    std::ostream& operator<<( std::ostream& ostream, const Othello::Captures& captures ) {
        ostream << '[';
        for ( const auto&[x, y] : captures ) {
            ostream << '(' << x << ',' << y << ')' << ", ";
        }
        return ostream << ']';
    }
}

DEFINE_LOGGER( Othello )

Othello::Othello() : boardState_{} {
    boardState_[ 3 ][ 3 ] = State::WHITE;
    boardState_[ 3 ][ 4 ] = State::BLACK;
    boardState_[ 4 ][ 3 ] = State::BLACK;
    boardState_[ 4 ][ 4 ] = State::WHITE;
}

void Othello::placePiece( int x, int y, bool isBlack, const Captures& captures ) {
    auto& place = boardState_.at( x ).at( y );
    LOG4CPLUS_DEBUG( GetLogger(),
                     "Placing " << ( isBlack ? "Black" : "White" ) <<
                                " piece at (" << x << ',' << y << ')' << " Capturing:" << captures );
    const auto& newState = isBlack ? State::BLACK : State::WHITE;
    place = newState;
    for ( const auto[x_, y_]: captures ) {
        boardState_[ x_ ][ y_ ] = newState;
    }
    blackTurn = !blackTurn;
}

std::vector<std::pair<int, int>> Othello::captured( int x, int y, bool isBlack ) const {
    Captures captures;
    const auto& state = boardState_.at( x ).at( y );
    if ( state != State::EMPTY ) return captures;
    const State same     = isBlack ? State::BLACK : State::WHITE;
    const State opposite = isBlack ? State::WHITE : State::BLACK;

    // check y-axis up
    {
        Captures  temp;
        for ( int i = y + 1; i < 8; ++i ) {
            const State& nextSpot = boardState_[ x ][ i ];
            if ( nextSpot == opposite ) temp.push_back( { x, i } );
            if ( nextSpot == same ) {
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    // check y-axis down
    {
        Captures  temp;
        for ( int i = y - 1; i >= 0; --i ) {
            const State& nextSpot = boardState_[ x ][ i ];
            if ( nextSpot == opposite ) temp.push_back( { x, i } );
            if ( nextSpot == same ) {
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }


    // check x-axis right
    {
        Captures  temp;
        for ( int i = x + 1; x < 8; ++i ) {
            const State& nextSpot = boardState_[ i ][ y ];
            if ( nextSpot == opposite ) temp.push_back( { i, y } );
            if ( nextSpot == same ) {
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    // check x-axis left
    {
        Captures  temp;
        for ( int i = x - 1; i >= 0; --i ) {
            const State& nextSpot = boardState_[ i ][ y ];
            if ( nextSpot == opposite ) temp.push_back( { i, y } );
            if ( nextSpot == same ) {
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    {
        Captures  temp;
        for ( int i = x + 1, j = y + 1; i < 8 && j < 8; ++i, ++j ) {
            const State& nextSpot = boardState_[ i ][ j ];
            if ( nextSpot == opposite ) temp.push_back( { i, j } );
            if ( nextSpot == same ) {
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    {
        Captures  temp;
        for ( int i = x - 1, j = y - 1; i >= 0 && j >= 0; --i, --j ) {
            const State& nextSpot = boardState_[ i ][ j ];
            if ( nextSpot == opposite ) temp.push_back( { i, j } );
            if ( nextSpot == same ) {
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    {
        Captures  temp;
        for ( int i = x - 1, j = y + 1; i >= 0 && j < 8; --i, ++j ) {
            const State& nextSpot = boardState_[ i ][ j ];
            if ( nextSpot == opposite ) temp.push_back( { i, j } );
            if ( nextSpot == same ) {
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    {
        Captures  temp;
        for ( int i = x + 1, j = y - 1; i < 8 && j >= 0; ++i, --j ) {
            const State& nextSpot = boardState_[ i ][ j ];
            if ( nextSpot == opposite ) temp.push_back( { i, j } );
            if ( nextSpot == same ) {
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    return captures;
}

std::pair<int, int> Othello::score() const {
    std::pair<int, int> value{};
    for ( const auto& cells: boardState_ )
        for ( const auto& cell: cells )
            switch ( cell ) {
                case State::EMPTY: break;
                case State::BLACK: value.first++;
                    break;
                case State::WHITE: value.second++;
                    break;
            }
    return value;
}


