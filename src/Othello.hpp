#pragma once

#include "gui/ImGuiWrapper.hpp"

class Othello {
public:
    enum class State {
        EMPTY, WHITE, BLACK
    };

    explicit Othello( bool& stayOpen );

    void render( gui::ImGuiWrapper& imGuiWrapper ) const;

private:
    mutable gui::WindowConfig           config;
    std::array<std::array<State, 8>, 8> boardState;
};



