#pragma once

#include "Othello.hpp"
#include "gui/ImGuiWrapper.hpp"

class OthelloWindow {
public:
    explicit OthelloWindow( bool& stayOpen );

    void render( gui::ImGuiWrapper& imGuiWrapper );


    [[nodiscard]] const Othello& othello() const { return othello_; }

private:
    static void renderGrid();

    void renderPieces();

    void drawGhosts( int x, int y, bool black, const Othello::Captures& captures );

    Othello           othello_;
    gui::WindowConfig config;
};



