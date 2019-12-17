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
    ImGui::SetNextWindowPosCenter( ImGuiCond_Once );
    ImGui::SetNextWindowSize( { 720, 720 }, ImGuiCond_Once );
    ImGui::PushStyleColor( ImGuiCol_WindowBg, ImU32( boardColor ));
    imGuiWrapper.window( config, [ this ] {
        auto      windowSize = ImGui::GetWindowSize();
        auto      drawList   = ImGui::GetWindowDrawList();
        auto      pos        = ImGui::GetWindowPos();
        for ( int i          = 1; i < 8; ++i ) {
            auto x = windowSize.x / 8 * (float) i;
            auto y = windowSize.y / 8 * (float) i;
            drawList->AddLine( { pos.x + x, pos.y },
                               { pos.x + x, pos.y + windowSize.y },
                               ImU32( ImColor{ 0, 0, 0, 255 } ),
                               2 );
            drawList->AddLine( { pos.x, pos.y + y },
                               { pos.x + windowSize.x, pos.y + y },
                               ImU32( ImColor{ 0, 0, 0, 255 } ),
                               2 );

        }

        float     xOffset = windowSize.x / 16;
        float     yOffset = windowSize.y / 16;
        float     xSize   = windowSize.x / 8;
        float     ySize   = windowSize.y / 8;
        for ( int i       = 0; i < boardState.size(); ++i ) {
            for ( int j = 0; j < boardState.size(); ++j ) {
                ImColor color{};
                switch ( boardState[ i ][ j ] ) {
                    case State::EMPTY: continue;
                    case State::WHITE:color = ImColor{ 0, 0, 0 };
                        break;
                    case State::BLACK:color = ImColor{ 255, 255, 255 };
                        break;
                }
                drawList->AddCircleFilled( { pos.x + xSize * (float) i + xOffset, pos.y + ySize * (float) j + yOffset },
                                           std::min( xSize / 3, ySize / 3 ), color, 24 );
            }
        }
    } );
    ImGui::PopStyleColor();
}

