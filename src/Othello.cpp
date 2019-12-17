#include "Othello.hpp"
#include <iostream>
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

    std::ostream& operator<<( std::ostream& ostream, const Othello::Captures& captures ) {
        ostream << '[';
        for ( const auto&[x, y] : captures ) {
            ostream << '(' << x << ',' << y << ')' << ", ";
        }
        return ostream << ']';
    }
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

            if ( x >= 8 || y >= 8 ) return;

            if ( boardState[ x ][ y ] != State::EMPTY ) return;

            const Captures captures = captured( x, y, blackTurn );
            if ( captures.empty()) return;

            const State newState = blackTurn ? State::BLACK : State::WHITE;
            drawGhosts( x, y, blackTurn, captures );

            if ( ImGui::IsMouseClicked( 0 )) {
                placePiece( x, y, blackTurn, captures );
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
                case State::WHITE: color = whiteColor;
                    break;
                case State::BLACK: color = blackColor;
                    break;
            }
            drawList->AddCircleFilled( { pos.x + xSize * (float) i + xOffset, pos.y + ySize * (float) j + yOffset },
                                       std::min( xSize / 3, ySize / 3 ), color, 24 );
        }
    }
}

void Othello::placePiece( int x, int y, bool isBlack, const Captures& captures ) {
    auto& place = boardState.at( x ).at( y );
    LOG4CPLUS_DEBUG( GetLogger(),
                     "Placing " << ( isBlack ? "Black" : "White" ) <<
                                " piece at (" << x << ',' << y << ')' << " Capturing:" << captures );
    const auto& newState = isBlack ? State::BLACK : State::WHITE;
    place = newState;
    for ( const auto[x_, y_]: captures ) {
        boardState[ x_ ][ y_ ] = newState;
    }
}

void Othello::drawGhosts( int x, int y, bool black, const Captures& captures ) {
    auto  windowSize = ImGui::GetWindowSize();
    auto  drawList   = ImGui::GetWindowDrawList();
    auto  pos        = ImGui::GetWindowPos();
    float xOffset    = windowSize.x / 16;
    float yOffset    = windowSize.y / 16;
    float xSize      = windowSize.x / 8;
    float ySize      = windowSize.y / 8;
    drawList->AddCircleFilled( { pos.x + xSize * (float) x + xOffset, pos.y + ySize * (float) y + yOffset },
                               std::min( xSize / 3, ySize / 3 ), black ? blackGhostColor : whiteGhostColor, 24 );
    for ( const auto[x, y]: captured( x, y, black )) {
        drawList->AddCircleFilled( { pos.x + xSize * (float) x + xOffset, pos.y + ySize * (float) y + yOffset },
                                   std::min( xSize / 3, ySize / 3 ), black ? blackGhostColor : whiteGhostColor, 24 );
    }
}

std::vector<std::pair<int, int>> Othello::captured( int x, int y, bool isBlack ) const {
    Captures captures;
    const auto& state = boardState.at( x ).at( y );
    if ( state != State::EMPTY ) return captures;
    const State same     = isBlack ? State::BLACK : State::WHITE;
    const State opposite = isBlack ? State::WHITE : State::BLACK;

    // check y-axis up
    if ( y < 5 && boardState[ x ][ y + 1 ] == opposite ) {
        Captures  temp;
        for ( int i = y + 1; i < 8; ++i ) {
            const State& nextSpot = boardState[ x ][ i ];
            if ( nextSpot == opposite ) temp.push_back( { x, i } );
            if ( nextSpot == same ) {
                captures.reserve( temp.size());
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    // check y-axis down
    if ( y > 2 && boardState[ x ][ y - 1 ] == opposite ) {
        Captures  temp;
        for ( int i = y - 1; i >= 0; --i ) {
            const State& nextSpot = boardState[ x ][ i ];
            if ( nextSpot == opposite ) temp.push_back( { x, i } );
            if ( nextSpot == same ) {
                captures.reserve( temp.size());
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }


    // check x-axis right
    if ( x < 5 && boardState[ x + 1 ][ y ] == opposite ) {
        Captures  temp;
        for ( int i = x + 1; x < 8; ++x ) {
            const State& nextSpot = boardState[ i ][ y ];
            if ( nextSpot == opposite ) temp.push_back( { i, y } );
            if ( nextSpot == same ) {
                captures.reserve( temp.size());
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    // check x-axis left
    if ( x > 2 && boardState[ x - 1 ][ y ] == opposite ) {
        Captures  temp;
        for ( int i = x - 1; i >= 0; ++i ) {
            const State& nextSpot = boardState[ i ][ y ];
            if ( nextSpot == opposite ) temp.push_back( { i, y } );
            if ( nextSpot == same ) {
                captures.reserve( temp.size());
                captures.insert( captures.end(), temp.begin(), temp.end());
                break;
            }
            if ( nextSpot == State::EMPTY ) break;
        }
    }

    // TODO in captures() check for diagonal captures

    return captures;
}


