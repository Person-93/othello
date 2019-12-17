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
    static void renderGrid();

    void renderPieces();

    bool isLegalMove( int x, int y, bool isBlack );

    void drawGhost( int x, int y, bool black );

    bool placePiece( int x, int y, bool isBlack );

    gui::WindowConfig                   config;
    std::array<std::array<State, 8>, 8> boardState;
    bool                                blackTurn = true;
};



