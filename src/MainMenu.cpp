#include "MainMenu.hpp"
#include "MinMaxStrategy.hpp"
#include "OthelloWindow.hpp"
#include "RandomAi.hpp"
#include "StrategicAi.hpp"
#include "coinParityHeuristic.hpp"
#include "compositeHeuristic.hpp"
#include "gui/ImGuiWrapper.hpp"
#include "mobilityHeuristic.hpp"
#include "stabilityHeuristic.hpp"

void MainMenu::operator()() {
  imGuiWrapper.mainMenu([this] { gameMenu(); });
}

void MainMenu::gameMenu() {
  imGuiWrapper.menu("Game", true, [this] {
    imGuiWrapper.menuItem("2 player", false, true,
                          [this] { othelloWindow.reset(nullptr); });
    imGuiWrapper.menu("Against computer", true, [this] {
      imGuiWrapper.menuItem("Random", false, true, [this] {
        othelloWindow.reset(std::make_unique<RandomAI>());
      });
      strategicAiMenuItem<coinParityHeuristic, MinMaxStrategy>(
          "MinMax - Coin Parity", 5);
      strategicAiMenuItem<mobilityHeuristic, MinMaxStrategy>(
          "MinMax - Mobility", 5);
      strategicAiMenuItem<stabilityHeuristic, MinMaxStrategy>(
          "MinMax - Stability", 5);
      strategicAiMenuItem<compositeHeuristic, MinMaxStrategy>(
          "MinMax - Composite", 5);
    });
  });
}

template <HeuristicFunction function, class Strategy, class... Args>
void MainMenu::strategicAiMenuItem(const char *label, Args &&...args) {
  static_assert(std::is_constructible_v<Strategy, Args...>);
  imGuiWrapper.menuItem(label, false, true, [&] {
    othelloWindow.reset(std::make_unique<StrategicAi>(
        std::make_unique<Strategy>(std::forward<Args>(args)...), function));
  });
}
