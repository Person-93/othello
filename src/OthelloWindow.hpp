#pragma once

#include "Othello.hpp"
#include "gui/ImGuiWrapper.hpp"
#include <memory>
#include <chrono>
#include <future>
#include "AI.hpp"

class OthelloWindow {
public:
    OthelloWindow();

    ~OthelloWindow();

    void render( gui::ImGuiWrapper& imGuiWrapper );

    [[nodiscard]] const Othello& othello() const { return othello_; }

    void reset( std::unique_ptr<AI> ai );

    bool gameOver() const;

private:
    static void renderGrid();

    void renderPieces();

    void drawGhosts( int x, int y, bool black, const Othello::Captures& captures );

    [[nodiscard]] bool isPlayerTurn() const;

    void handlePlayerTurn();

    void handleComputerTurn();

    void placePiece( int x, int y, bool isBlack, const Othello::Captures& captures );

    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    Othello                                  othello_;
    Othello::LegalMoves                      legalMoves;
    TimePoint                                playerMovedTime;
    std::optional<std::future<ComputerMove>> computerMoveFuture{};
    std::optional<ComputerMove>              computerMove;
    gui::WindowConfig                        config;
    std::unique_ptr<AI>                      ai;
    bool                                     aiIsBlack = true;
};



