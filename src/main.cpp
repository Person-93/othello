#include <atomic>
#include <csignal>
#include <boost/exception/diagnostic_information.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "version.hpp"
#include "util/configure_logging.hpp"
#include "gui/ImGuiWrapper.hpp"
#include "Othello.hpp"

std::atomic_bool shouldRun = true;

extern "C" void signalHandler( int ) { shouldRun = false; }

int main() try {
    std::signal( SIGTERM, signalHandler );
    util::ConfigureLogging();
    LOG4CPLUS_DEBUG( log4cplus::Logger::getRoot(), "Running version: " << version::longVersion());
    gui::ImGuiWrapper imGuiWrapper( "Othello" );
    bool              stayOpen = true;
    Othello           othello{ stayOpen };
    gui::WindowConfig scoreWindowConfig{
            .title = "Scores",
            .open = &stayOpen,
            .flags = ImGuiWindowFlags_NoTitleBar & ImGuiWindowFlags_AlwaysAutoResize
    };

    while ( shouldRun && !imGuiWrapper.shouldClose()) {
        auto f = imGuiWrapper.frame( 20 );
        othello.render( imGuiWrapper );

        ImGui::SetNextWindowPos( { 0, 0 } );
        imGuiWrapper.window( scoreWindowConfig, [ othello ] {
            auto score = othello.score();
            ImGui::Columns( 2 );
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

    return 0;
}
catch ( ... ) {
    LOG4CPLUS_FATAL( log4cplus::Logger::getRoot(), boost::current_exception_diagnostic_information( true ));
    return -1;
}
