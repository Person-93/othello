#include "Othello.hpp"
#include "gui/ImGuiWrapper.hpp"

namespace {
    const ImColor boardColor{ 20, 62, 28, 255 }; // NOLINT(cert-err58-cpp)
}

Othello::Othello( bool& stayOpen ) : config{
        .title = "Game",
        .open = &stayOpen,
}, boardState{} {
    boardState[ 3 ][ 3 ] = State::WHITE;
    boardState[ 3 ][ 4 ] = State::BLACK;
    boardState[ 4 ][ 3 ] = State::BLACK;
    boardState[ 4 ][ 4 ] = State::WHITE;
}

void Othello::render( gui::ImGuiWrapper& imGuiWrapper ) const {
    ImGui::SetNextWindowSize( { 720, 720 }, ImGuiCond_Once );
    ImGui::PushStyleColor( ImGuiCol_WindowBg, ImU32( boardColor ));
    imGuiWrapper.window( config, [ this ] {
        auto      size     = ImGui::GetWindowSize();
        auto      drawList = ImGui::GetWindowDrawList();
        for ( int i        = 1; i < 8; ++i ) {
            auto x = size.x / 8 * (float) i;
            auto y = size.y / 8 * (float) i;
            drawList->AddLine( { x, 0 }, { x, size.y }, ImU32( ImColor{ 0, 0, 0, 255 } ), 2 );
            drawList->AddLine( { 0, y }, { size.x, y }, ImU32( ImColor{ 0, 0, 0, 255 } ), 2 );

        }
    } );
    ImGui::PopStyleColor();
}

