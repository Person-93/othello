#include "OthelloWindow.hpp"

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

OthelloWindow::OthelloWindow() : config{
        .title = "Game",
        .flags = ImGuiWindowFlags_NoTitleBar,
}, legalMoves{ othello_.legalMoves() } {}

OthelloWindow::~OthelloWindow() = default;

void OthelloWindow::render( gui::ImGuiWrapper& imGuiWrapper ) {
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
        if ( legalMoves.empty()) return;
        if ( isPlayerTurn())
            handlePlayerTurn();
        else
            handleComputerTurn();
    } );
}

void OthelloWindow::renderGrid() {
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

void OthelloWindow::renderPieces() {
    auto      windowSize = ImGui::GetWindowSize();
    auto      drawList   = ImGui::GetWindowDrawList();
    auto      pos        = ImGui::GetWindowPos();
    float     xOffset    = windowSize.x / 16;
    float     yOffset    = windowSize.y / 16;
    float     xSize      = windowSize.x / 8;
    float     ySize      = windowSize.y / 8;
    for ( int i          = 0; i < othello_.boardState().size(); ++i ) {
        for ( int j = 0; j < othello_.boardState().size(); ++j ) {
            ImColor color{};
            switch ( othello_.boardState()[ i ][ j ] ) {
                case Othello::State::EMPTY: continue;
                case Othello::State::WHITE: color = whiteColor;
                    break;
                case Othello::State::BLACK: color = blackColor;
                    break;
            }
            drawList->AddCircleFilled( { pos.x + xSize * (float) i + xOffset, pos.y + ySize * (float) j + yOffset },
                                       std::min( xSize / 3, ySize / 3 ), color, 24 );
        }
    }
}

void OthelloWindow::drawGhosts( int x,
                                int y,
                                bool black,
                                const Othello::Captures& captures ) { // NOLINT(readability-convert-member-functions-to-static)
    auto  windowSize = ImGui::GetWindowSize();
    auto  drawList   = ImGui::GetWindowDrawList();
    auto  pos        = ImGui::GetWindowPos();
    float xOffset    = windowSize.x / 16;
    float yOffset    = windowSize.y / 16;
    float xSize      = windowSize.x / 8;
    float ySize      = windowSize.y / 8;
    drawList->AddCircleFilled( { pos.x + xSize * (float) x + xOffset, pos.y + ySize * (float) y + yOffset },
                               std::min( xSize / 3, ySize / 3 ), black ? blackGhostColor : whiteGhostColor, 24 );
    for ( const auto[x, y]: captures ) {
        drawList->AddCircleFilled( { pos.x + xSize * (float) x + xOffset, pos.y + ySize * (float) y + yOffset },
                                   std::min( xSize / 3, ySize / 3 ), black ? blackGhostColor : whiteGhostColor, 24 );
    }
}

void OthelloWindow::reset( std::unique_ptr<AI> ai ) {
    othello_ = {};
    this->ai = std::move( ai );
}

bool OthelloWindow::isPlayerTurn() const {
    return ai == nullptr || ( aiIsBlack && !othello().isBlackTurn()) || ( !aiIsBlack && othello().isBlackTurn());
}

void OthelloWindow::handlePlayerTurn() {
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

        if ( othello_.boardState()[ x ][ y ] != Othello::State::EMPTY ) return;

        const auto iter = legalMoves.find( { x, y } );
        if ( iter == legalMoves.end()) return;
        const auto& captures = iter->second;

        drawGhosts( x, y, othello_.isBlackTurn(), captures );

        if ( ImGui::IsMouseClicked( 0 )) {
            placePiece( x, y, othello_.isBlackTurn(), captures );
        }
        playerMovedTime = Clock::now();
    }
}

void OthelloWindow::handleComputerTurn() {
    if ( !computerMoveFuture.has_value()) {
        computerMoveFuture = std::async( std::launch::async, [ this ] { return ai->go( othello(), aiIsBlack ); } );
        return;
    }
    if ( !computerMove.has_value() &&
         computerMoveFuture->valid() &&
         computerMoveFuture->wait_for( std::chrono::duration<int>::zero()) == std::future_status::ready )
        computerMove = computerMoveFuture->get();

    if ( !computerMove.has_value()) return;

    if ( Clock::now() - playerMovedTime < std::chrono::seconds{ 1 } )
        drawGhosts( computerMove->x, computerMove->y, aiIsBlack, computerMove->captures );
    else {
        placePiece( computerMove->x, computerMove->y, aiIsBlack, computerMove->captures );
        computerMoveFuture = std::nullopt;
        computerMove       = std::nullopt;
    }
}

void OthelloWindow::placePiece( int x, int y, bool isBlack, const Othello::Captures& captures ) {
    othello_.placePiece( x, y, isBlack, captures );
    legalMoves = othello().legalMoves();
}

bool OthelloWindow::gameOver() const {
    return legalMoves.empty();
}
