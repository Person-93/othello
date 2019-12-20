#include "MainMenu.hpp"
#include "gui/ImGuiWrapper.hpp"
#include "OthelloWindow.hpp"
#include "RandomAi.hpp"
#include "StrategicAi.hpp"
#include "coinParityHeuristic.hpp"
#include "MinMaxStrategy.hpp"

void MainMenu::operator()() {
    imGuiWrapper.mainMenu( [ this ] {
        gameMenu();
    } );
}

void MainMenu::gameMenu() {
    imGuiWrapper.menu( "Game", true, [ this ] {
        imGuiWrapper.menuItem( "2 player", false, true, [ this ] {
            othelloWindow.reset( nullptr );
        } );
        imGuiWrapper.menu( "Against computer", true, [ this ] {
            imGuiWrapper.menuItem( "Random", false, true, [ this ] {
                othelloWindow.reset( std::make_unique<RandomAI>());
            } );
            strategicAiMenuItem<coinParityHeuristic, MinMaxStrategy>( "MinMax - Coin Parity", 5 );
        } );
    } );
}

template< HeuristicFunction function, class Strategy, class... Args >
void MainMenu::strategicAiMenuItem( const char* label, Args&& ... args ) {
    static_assert( std::is_constructible_v<Strategy, Args...> );
    imGuiWrapper.menuItem( label, false, true, [ & ] {
        othelloWindow.reset(
                std::make_unique<StrategicAi>( std::make_unique<Strategy>( std::forward<Args>( args )... ), function ));
    } );
}
