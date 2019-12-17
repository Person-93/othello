#pragma once

#include "gui/ImGuiWrapper.hpp"

class Othello {
public:
    enum class State {
        EMPTY, WHITE, BLACK
    };

    explicit Othello( bool& stayOpen );

    void render( gui::ImGuiWrapper& imGuiWrapper );

    /**
     * Gets the current score as a pair, black score first, white score second
     * @return
     */
    [[nodiscard]] std::pair<int, int> score() const;

    using Captures = std::vector<std::pair<int, int>>;
private:

    static void renderGrid();

    void renderPieces();

    [[nodiscard]] Captures captured( int x, int y, bool isBlack ) const;

    void drawGhosts( int x, int y, bool black, const Captures& captures );

    void placePiece( int x, int y, bool isBlack, const Captures& captures );

    gui::WindowConfig                   config;
    std::array<std::array<State, 8>, 8> boardState;
    bool                                blackTurn = true;
};



