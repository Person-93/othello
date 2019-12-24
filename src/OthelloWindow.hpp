#pragma once

#include "Othello.hpp"
#include "gui/ImGuiWrapper.hpp"
#include <memory>
#include <chrono>
#include <future>
#include "AI.hpp"

class OthelloWindow {
public:
    explicit OthelloWindow( gui::ImGuiWrapper& imGuiWrapper );

    ~OthelloWindow();

    void operator()();

    [[nodiscard]] const Othello& othello() const { return othello_; }

    void reset( std::unique_ptr<AI> ai );

    bool gameOver() const;

private:
    static void renderGrid();

    void renderPieces();

    void drawGhosts( int x, int y );

    [[nodiscard]] bool isPlayerTurn() const;

    void handlePlayerTurn();

    void handleComputerTurn();

    void placePiece( int x, int y );

    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    Othello                              othello_;
    TimePoint                            computerMoveTime;
    std::optional<std::future<AI::Move>> computerMoveFuture{};
    std::optional<AI::Move>              computerMove;
    gui::WindowConfig                    config;
    gui::WindowConfig                    errorWindowConfig;
    std::unique_ptr<AI>                  ai;
    bool                                 aiIsBlack = true;
    std::optional<std::string>           errorInfo{};
    gui::ImGuiWrapper& imGuiWrapper;
};



