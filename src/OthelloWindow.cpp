#include <boost/exception/diagnostic_information.hpp>
#include "OthelloWindow.hpp"
#include "util/define_logger.hpp"

DEFINE_LOGGER( OthelloWindow )

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

OthelloWindow::OthelloWindow( gui::ImGuiWrapper& imGuiWrapper ) : config{
        .title = "Game",
        .flags = ImGuiWindowFlags_NoTitleBar,
}, errorWindowConfig{
        .title = "Error"
}, imGuiWrapper{ imGuiWrapper } {}

OthelloWindow::~OthelloWindow() = default;

void OthelloWindow::operator()() {
    ImGui::SetNextWindowPosCenter();
    ImGui::SetNextWindowSize( { 720, 720 }, ImGuiCond_Once );
    {
        ImGui::PushStyleColor( ImGuiCol_WindowBg, ImU32( boardColor ));
        struct Finally {
            ~Finally() {
                ImGui::PopStyleColor();
            }
        } aFinally;
        imGuiWrapper.window( config, [ this ] {
            renderGrid();
            renderPieces();
            if ( errorInfo ) return;
            if ( othello().legalMoves().empty()) return;
            if ( isPlayerTurn())
                handlePlayerTurn();
            else
                handleComputerTurn();
        } );
    }
    if ( errorInfo ) {
        imGuiWrapper.window( errorWindowConfig, [ this ] {
            ImGui::TextUnformatted( errorInfo->c_str());
        } );
    }
}

void OthelloWindow::renderGrid() {
    auto      windowSize = ImGui::GetWindowSize();
    auto      drawList   = ImGui::GetWindowDrawList();
    auto      pos        = ImGui::GetWindowPos();
    for ( int i          = 1; i < Othello::boardSize; ++i ) {
        auto x = windowSize.x / Othello::boardSize * (float) i;
        auto y = windowSize.y / Othello::boardSize * (float) i;
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
    float     xOffset    = windowSize.x / ( Othello::boardSize * 2 );
    float     yOffset    = windowSize.y / ( Othello::boardSize * 2 );
    float     xSize      = windowSize.x / Othello::boardSize;
    float     ySize      = windowSize.y / Othello::boardSize;
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

void OthelloWindow::drawGhosts( int x, int y ) {
    const auto iter = othello().legalMoves().find( { x, y } );
    if ( iter == othello().legalMoves().end()) {
        using namespace exception;
        THROW_EXCEPTION(( Exception{} << Because{ "Cannot draw ghosts for illegal move" } << Move{{ x, y }}
                                      << Board{ othello().boardState() } ));
    }
    const auto& captures = othello().legalMoves().find( { x, y } )->second;
    auto  windowSize = ImGui::GetWindowSize();
    auto  drawList   = ImGui::GetWindowDrawList();
    auto  pos        = ImGui::GetWindowPos();
    float xOffset    = windowSize.x / ( Othello::boardSize * 2 );
    float yOffset    = windowSize.y / ( Othello::boardSize * 2 );
    float xSize      = windowSize.x / Othello::boardSize;
    float ySize      = windowSize.y / Othello::boardSize;
    drawList->AddCircleFilled( { pos.x + xSize * (float) x + xOffset, pos.y + ySize * (float) y + yOffset },
                               std::min( xSize / 3, ySize / 3 ),
                               othello().isBlackTurn() ? blackGhostColor : whiteGhostColor, 24 );
    for ( const auto[x, y]: captures ) {
        drawList->AddCircleFilled( { pos.x + xSize * (float) x + xOffset, pos.y + ySize * (float) y + yOffset },
                                   std::min( xSize / 3, ySize / 3 ),
                                   othello().isBlackTurn() ? blackGhostColor : whiteGhostColor, 24 );
    }
}

void OthelloWindow::reset( std::unique_ptr<AI> ai ) {
    othello_ = {};
    this->ai = std::move( ai );
    errorInfo = std::nullopt;
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

        auto x = mouse.x / size.x * Othello::boardSize;
        auto y = mouse.y / size.y * Othello::boardSize;

        if ( x >= Othello::boardSize || y >= Othello::boardSize ) return;

        if ( othello_.boardState()[ x ][ y ] != Othello::State::EMPTY ) return;

        const auto iter = othello().legalMoves().find( { x, y } );
        if ( iter == othello().legalMoves().end()) return;
        const auto& captures = iter->second;

        drawGhosts( x, y );

        if ( ImGui::IsMouseClicked( 0 )) {
            placePiece( x, y );
        }
    }
}

void OthelloWindow::handleComputerTurn() {
    if ( !computerMoveFuture.has_value()) {
        computerMoveFuture = std::async( std::launch::async, [ this ] { return ai->go( othello()); } );
        return;
    }
    if ( !computerMove.has_value() &&
         computerMoveFuture->valid() &&
         computerMoveFuture->wait_for( std::chrono::duration<int>::zero()) == std::future_status::ready ) {
        try {
            computerMove     = computerMoveFuture->get();
            computerMoveTime = Clock::now();
        }
        catch ( ... ) {
            errorInfo = boost::current_exception_diagnostic_information( true );
        }
    }

    if ( !computerMove.has_value()) return;

    if ( Clock::now() - computerMoveTime < std::chrono::seconds{ 1 } )
        drawGhosts( computerMove->first, computerMove->second );
    else {
        placePiece( computerMove->first, computerMove->second );
        computerMoveFuture = std::nullopt;
        computerMove       = std::nullopt;
    }
}

void OthelloWindow::placePiece( int x, int y ) {
    othello_.placePiece( x, y );
}

bool OthelloWindow::gameOver() const {
    return othello().legalMoves().empty();
}
