#include "coinParityHeuristic.hpp"
#include "Othello.hpp"
#include <algorithm>

double coinParityHeuristic( const Othello& othello ) {
    int whiteCoins = 0;
    int blackCoins = 0;
    for ( const auto& column : othello.boardState()) {
        for ( const auto& cell: column ) {
            switch ( cell ) {
                case Othello::State::EMPTY: break;
                case Othello::State::WHITE: ++whiteCoins;
                    break;
                case Othello::State::BLACK: ++blackCoins;
                    break;
            }
        }
    }

    auto[minPlayerCoins, maxPlayerCoins] = std::minmax( whiteCoins, blackCoins );
    return 100 * (double) ( maxPlayerCoins - minPlayerCoins ) / ( maxPlayerCoins + minPlayerCoins );
}
