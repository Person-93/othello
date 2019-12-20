#pragma once

#include <utility>

class Othello;

class AI {
public:
    using Move = std::pair<int, int>;

    virtual Move go( const Othello& othello, bool isBlack ) = 0;

    virtual ~AI() = default;
};
