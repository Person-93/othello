#pragma once

#include <utility>
#include "Othello.hpp"

struct ComputerMove {
    int               x;
    int               y;
    Othello::Captures captures;
};

class AI {
public:
    virtual ComputerMove go( const Othello& othello, bool isBlack ) = 0;

    virtual ~AI() = default;
};
