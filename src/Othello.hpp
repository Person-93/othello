#pragma once

#include "gui/ImGuiWrapper.hpp"

class Othello {
public:
    enum class State {
        EMPTY, WHITE, BLACK
    };

    explicit Othello( bool& stayOpen );

    void render( gui::ImGuiWrapper& imGuiWrapper );

private:
    using Captures = std::vector<std::pair<int, int>>;

    static void renderGrid();

    void renderPieces();

    [[nodiscard]] bool isLegalMove( int x, int y, bool isBlack ) const;

    [[nodiscard]] Captures captures( int x, int y, bool isBlack ) const;

    void drawGhost( int x, int y, bool black );

    bool placePiece( int x, int y, bool isBlack );

    gui::WindowConfig                   config;
    std::array<std::array<State, 8>, 8> boardState;
    bool                                blackTurn = true;
};



