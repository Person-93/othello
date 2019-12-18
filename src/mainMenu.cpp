#include "mainMenu.hpp"
#include "gui/ImGuiWrapper.hpp"
#include "OthelloWindow.hpp"
#include "AI.hpp"
#include "RandomAi.hpp"

void mainMenu( gui::ImGuiWrapper& imGuiWrapper, OthelloWindow& othelloWindow ) {
    imGuiWrapper.mainMenu( [ & ] {
        imGuiWrapper.menu( "Game", true, [ & ] {
            imGuiWrapper.menuItem( "2 player", false, true, [ & ] {
                othelloWindow.reset( nullptr );
            } );
            imGuiWrapper.menu( "Against computer", true, [ & ] {
                imGuiWrapper.menuItem( "Random", false, true, [ & ] {
                    othelloWindow.reset( std::make_unique<RandomAI>());
                } );
            } );
        } );
    } );
}
