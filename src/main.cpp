#include <atomic>
#include <csignal>
#include <boost/exception/diagnostic_information.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "version.hpp"
#include "util/configure_logging.hpp"
#include "gui/ImGuiWrapper.hpp"
#include "OthelloWindow.hpp"
#include "MainMenu.hpp"

std::atomic_bool shouldRun = true;

extern "C" void signalHandler( int ) { shouldRun = false; }

void scoreWindow( gui::ImGuiWrapper& imGuiWrapper, std::pair<int, int> score );

void gameOverWindow( gui::ImGuiWrapper& imGuiWrapper, std::pair<int, int> score );

int main() try {
    std::signal( SIGTERM, signalHandler );
    util::ConfigureLogging();
    LOG4CPLUS_DEBUG( log4cplus::Logger::getRoot(), "Running version: " << version::longVersion());
    gui::ImGuiWrapper imGuiWrapper( "Othello" );
    OthelloWindow     othelloWindow{ imGuiWrapper };
    MainMenu          mainMenu{ imGuiWrapper, othelloWindow };

    while ( shouldRun && !imGuiWrapper.shouldClose()) {
        auto f = imGuiWrapper.frame( 20 );
        mainMenu();
        othelloWindow();
        scoreWindow( imGuiWrapper, othelloWindow.othello().score());
        if ( othelloWindow.gameOver()) gameOverWindow( imGuiWrapper, othelloWindow.othello().score());
    }

    return 0;
}
catch ( ... ) {
    LOG4CPLUS_FATAL( log4cplus::Logger::getRoot(), boost::current_exception_diagnostic_information( true ));
    return -1;
}

void scoreWindow( gui::ImGuiWrapper& imGuiWrapper, std::pair<int, int> score ) {
    static gui::WindowConfig scoreWindowConfig{
            .title = "Scores",
            .flags = ImGuiWindowFlags_NoDecoration
    };
    ImGui::SetNextWindowPos( { 0, 25 } );
    ImGui::SetNextWindowSize( { 125, 50 } );
    imGuiWrapper.window( scoreWindowConfig, [ score ] {
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, 90 );
        ImGui::TextUnformatted( "Black score" );
        ImGui::NextColumn();
        ImGui::Text( "%d", score.first );
        ImGui::NextColumn();
        ImGui::Separator();
        ImGui::TextUnformatted( "White score" );
        ImGui::NextColumn();
        ImGui::Text( "%d", score.second );
        ImGui::Columns();
    } );
}

void gameOverWindow( gui::ImGuiWrapper& imGuiWrapper, std::pair<int, int> score ) {
    static gui::WindowConfig config{
            .title = "Game Over",
            .flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
    };
    ImGui::SetNextWindowPosCenter();
    imGuiWrapper.window( config, [ & ] {
        if ( score.first > score.second )
            ImGui::TextUnformatted( "Black wins!" );
        else if ( score.second > score.first )
            ImGui::TextUnformatted( "White wins!" );
        else
            ImGui::TextUnformatted( "Tie game!" );
    } );
}
