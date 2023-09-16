#pragma once

#include "HeuristicFunction.hpp"

namespace gui {
struct ImGuiWrapper;
}

class OthelloWindow;

class MainMenu {
public:
  MainMenu(gui::ImGuiWrapper &imGuiWrapper, OthelloWindow &othelloWindow)
      : imGuiWrapper{imGuiWrapper}, othelloWindow{othelloWindow} {}

  void operator()();

private:
  void gameMenu();

  template <HeuristicFunction function, class Strategy, class... Args>
  void strategicAiMenuItem(const char *label, Args &&...args);

  gui::ImGuiWrapper &imGuiWrapper;
  OthelloWindow &othelloWindow;
};
