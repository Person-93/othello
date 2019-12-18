#include "RandomAi.hpp"
#include "Othello.hpp"
#include <random>

ComputerMove RandomAI::go( const Othello& othello, bool isBlack ) {
    static std::mt19937           generator{ std::random_device{}() };
    std::uniform_int_distribution distribution{ 0, 7 };

    ComputerMove move{};
    while ( move.captures.empty()) {
        move.x        = distribution( generator );
        move.y        = distribution( generator );
        move.captures = othello.captured( move.x, move.y, isBlack );
    }

    return move;
}
