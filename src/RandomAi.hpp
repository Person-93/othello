#pragma once

#include "AI.hpp"

class RandomAI : public AI {
public:
    Move go( const Othello& othello, bool isBlack ) override;
};



