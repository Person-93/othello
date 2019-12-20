#include "RandomAi.hpp"
#include "Othello.hpp"
#include <random>

auto RandomAI::go( const Othello& othello, bool isBlack ) -> Move {
    static std::mt19937           generator{ std::random_device{}() };
    std::uniform_int_distribution distribution{ 0, Othello::boardSize - 1 };

    Move move{ -1, -1 };
    while ( othello.legalMoves().find( move ) == othello.legalMoves().end()) {
        move.first  = distribution( generator );
        move.second = distribution( generator );
    }

    return move;
}
