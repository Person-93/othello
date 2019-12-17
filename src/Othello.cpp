#include "Othello.hpp"
#include "gui/ImGuiWrapper.hpp"
#include "util/define_logger.hpp"

DEFINE_LOGGER( Othello )

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
namespace {
    const ImColor boardColor{ 20, 62, 28, 255 };
    const ImColor blackColor{ 0, 0, 0 };
    const ImColor blackGhostColor{ 0, 0, 0, 100 };
    const ImColor whiteColor{ 255, 255, 255 };
    const ImColor whiteGhostColor{ 255, 255, 255, 100 };
}
#pragma clang diagnostic pop

Othello::Othello( bool& stayOpen ) : config{
        .title = "Game",
        .open = &stayOpen,
        .flags = ImGuiWindowFlags_NoTitleBar,
}, boardState{} {
    boardState[ 3 ][ 3 ] = State::WHITE;
    boardState[ 3 ][ 4 ] = State::BLACK;
    boardState[ 4 ][ 3 ] = State::BLACK;
    boardState[ 4 ][ 4 ] = State::WHITE;
}

void Othello::render( gui::ImGuiWrapper& imGuiWrapper ) {
    ImGui::SetNextWindowPosCenter();
    ImGui::SetNextWindowSize( { 720, 720 }, ImGuiCond_Once );
    ImGui::PushStyleColor( ImGuiCol_WindowBg, ImU32( boardColor ));
    struct Finally {
        ~Finally() {
            ImGui::PopStyleColor();
        }
    } aFinally;
    imGuiWrapper.window( config, [ this ] {
        renderGrid();
        renderPieces();
        if ( ImGui::IsMouseHoveringWindow()) {
            auto mouse = ImGui::GetMousePos();
            auto pos   = ImGui::GetWindowPos();
            auto size  = ImGui::GetWindowSize();

            // translate mouse into current window coordinates
            mouse.x -= pos.x;
            mouse.y -= pos.y;

            auto x = mouse.x / size.x * 8;
            auto y = mouse.y / size.y * 8;

            if ( x < 8 && y < 8 && boardState.at( x ).at( y ) != State::EMPTY ) return;

            if ( isLegalMove( x, y, blackTurn )) drawGhost( x, y, blackTurn );

            if ( ImGui::IsMouseClicked( 0 )) {
                LOG4CPLUS_DEBUG( GetLogger(), "Grid was clicked" );
                if ( placePiece( mouse.x / size.x * 8, mouse.y / size.y * 8, blackTurn ))
                    blackTurn = !blackTurn;
            }
        }
    } );
}

void Othello::renderGrid() {
    auto      windowSize = ImGui::GetWindowSize();
    auto      drawList   = ImGui::GetWindowDrawList();
    auto      pos        = ImGui::GetWindowPos();
    for ( int i          = 1; i < 8; ++i ) {
        auto x = windowSize.x / 8 * (float) i;
        auto y = windowSize.y / 8 * (float) i;
        drawList->AddLine( { pos.x + x, pos.y },
                           { pos.x + x, pos.y + windowSize.y },
                           blackColor,
                           2 );
        drawList->AddLine( { pos.x, pos.y + y },
                           { pos.x + windowSize.x, pos.y + y },
                           blackColor,
                           2 );

    }
}

void Othello::renderPieces() {
    auto      windowSize = ImGui::GetWindowSize();
    auto      drawList   = ImGui::GetWindowDrawList();
    auto      pos        = ImGui::GetWindowPos();
    float     xOffset    = windowSize.x / 16;
    float     yOffset    = windowSize.y / 16;
    float     xSize      = windowSize.x / 8;
    float     ySize      = windowSize.y / 8;
    for ( int i          = 0; i < boardState.size(); ++i ) {
        for ( int j = 0; j < boardState.size(); ++j ) {
            ImColor color{};
            switch ( boardState[ i ][ j ] ) {
                case State::EMPTY: continue;
                case State::WHITE:color = whiteColor;
                    break;
                case State::BLACK:color = blackColor;
                    break;
            }
            drawList->AddCircleFilled( { pos.x + xSize * (float) i + xOffset, pos.y + ySize * (float) j + yOffset },
                                       std::min( xSize / 3, ySize / 3 ), color, 24 );
        }
    }
}

bool Othello::placePiece( int x, int y, bool isBlack ) {
    auto& place = boardState.at( x ).at( y );
    if ( isLegalMove( x, y, isBlack )) {
        LOG4CPLUS_DEBUG( GetLogger(),
                         "Placing " << ( isBlack ? "Black" : "White" ) <<
                                    " piece at (" << x << ',' << y << ')' );
        place = isBlack ? State::BLACK : State::WHITE;
        return true;
    }
    return false;
}

bool Othello::isLegalMove( int x, int y, bool isBlack ) {
    // TODO implement isLegalMove
    return true;
}

void Othello::drawGhost( int x, int y, bool black ) {
    auto  windowSize = ImGui::GetWindowSize();
    auto  drawList   = ImGui::GetWindowDrawList();
    auto  pos        = ImGui::GetWindowPos();
    float xOffset    = windowSize.x / 16;
    float yOffset    = windowSize.y / 16;
    float xSize      = windowSize.x / 8;
    float ySize      = windowSize.y / 8;
    drawList->AddCircleFilled( { pos.x + xSize * (float) x + xOffset, pos.y + ySize * (float) y + yOffset },
                               std::min( xSize / 3, ySize / 3 ), black ? blackGhostColor : whiteGhostColor, 24 );
}


