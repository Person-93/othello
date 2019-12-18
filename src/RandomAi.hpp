#pragma once

#include "AI.hpp"

class RandomAI : public AI {
public:
    ComputerMove go( const Othello& othello, bool isBlack ) override;
};



