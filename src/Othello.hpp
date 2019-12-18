#pragma once

#include <array>
#include <utility>
#include <vector>

class Othello {
public:
    enum class State {
        EMPTY, WHITE, BLACK
    };
    using BoardState = std::array<std::array<State, 8>, 8>;
    using Captures   = std::vector<std::pair<int, int>>;

    Othello();

    /**
     * Gets the current score as a pair, black score first, white score second
     * @return
     */
    [[nodiscard]] std::pair<int, int> score() const;

    [[nodiscard]] Captures captured( int x, int y, bool isBlack ) const;

    void placePiece( int x, int y, bool isBlack, const Captures& captures );

    [[nodiscard]] const BoardState& boardState() const { return boardState_; }

    [[nodiscard]] bool isBlackTurn() const { return blackTurn; }

private:
    BoardState boardState_;
    bool       blackTurn = true;
};



