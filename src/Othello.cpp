#include "Othello.hpp"
#include "Exception.hpp"
#include "util/define_logger.hpp"
#include <iostream>

namespace {
std::ostream &operator<<(std::ostream &ostream,
                         const Othello::Captures &captures) {
  ostream << '[';
  for (const auto &[x, y] : captures) {
    ostream << '(' << x << ',' << y << ')' << ", ";
  }
  return ostream << ']';
}
} // namespace

DEFINE_LOGGER(Othello)

Othello::Othello() : boardState_{} {
  constexpr int halfBoardSize = boardSize / 2;
  boardState_[halfBoardSize - 1][halfBoardSize - 1] = State::WHITE;
  boardState_[halfBoardSize - 1][halfBoardSize] = State::BLACK;
  boardState_[halfBoardSize][halfBoardSize - 1] = State::BLACK;
  boardState_[halfBoardSize][halfBoardSize] = State::WHITE;
  calculateLegalMoves();
}

void Othello::placePiece(int x, int y) {
  auto iter = legalMoves().find({x, y});
  if (iter == legalMoves().end()) {
    using namespace exception;
    THROW_EXCEPTION((Exception{} << Because{"Illegal move"} << Move{{x, y}}
                                 << Board{boardState_}));
  }
  const Captures &captures = iter->second;
  auto &place = boardState_.at(x).at(y);
  const auto &newState = isBlackTurn() ? State::BLACK : State::WHITE;
  place = newState;
  for (const auto [x_, y_] : captures) {
    boardState_.at(x_).at(y_) = newState;
  }
  blackTurn = !blackTurn;
  calculateLegalMoves();
  if (legalMoves().empty()) {
    blackTurn = !blackTurn;
    calculateLegalMoves();
  }
}

std::vector<std::pair<int, int>> Othello::captured(int x, int y,
                                                   bool isBlack) const {
  Captures captures;
  const auto &state = boardState_.at(x).at(y);
  if (state != State::EMPTY)
    return captures;
  const State same = isBlack ? State::BLACK : State::WHITE;
  const State opposite = isBlack ? State::WHITE : State::BLACK;

  // check y-axis up
  {
    Captures temp;
    for (int i = y + 1; i < boardSize; ++i) {
      const State &nextSpot = boardState_[x][i];
      if (nextSpot == opposite)
        temp.push_back({x, i});
      if (nextSpot == same) {
        captures.insert(captures.end(), temp.begin(), temp.end());
        break;
      }
      if (nextSpot == State::EMPTY)
        break;
    }
  }

  // check y-axis down
  {
    Captures temp;
    for (int i = y - 1; i >= 0; --i) {
      const State &nextSpot = boardState_[x][i];
      if (nextSpot == opposite)
        temp.push_back({x, i});
      if (nextSpot == same) {
        captures.insert(captures.end(), temp.begin(), temp.end());
        break;
      }
      if (nextSpot == State::EMPTY)
        break;
    }
  }

  // check x-axis right
  {
    Captures temp;
    for (int i = x + 1; i < boardSize; ++i) {
      const State &nextSpot = boardState_[i][y];
      if (nextSpot == opposite)
        temp.push_back({i, y});
      if (nextSpot == same) {
        captures.insert(captures.end(), temp.begin(), temp.end());
        break;
      }
      if (nextSpot == State::EMPTY)
        break;
    }
  }

  // check x-axis left
  {
    Captures temp;
    for (int i = x - 1; i >= 0; --i) {
      const State &nextSpot = boardState_[i][y];
      if (nextSpot == opposite)
        temp.push_back({i, y});
      if (nextSpot == same) {
        captures.insert(captures.end(), temp.begin(), temp.end());
        break;
      }
      if (nextSpot == State::EMPTY)
        break;
    }
  }

  {
    Captures temp;
    for (int i = x + 1, j = y + 1; i < boardSize && j < boardSize; ++i, ++j) {
      const State &nextSpot = boardState_[i][j];
      if (nextSpot == opposite)
        temp.push_back({i, j});
      if (nextSpot == same) {
        captures.insert(captures.end(), temp.begin(), temp.end());
        break;
      }
      if (nextSpot == State::EMPTY)
        break;
    }
  }

  {
    Captures temp;
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; --i, --j) {
      const State &nextSpot = boardState_[i][j];
      if (nextSpot == opposite)
        temp.push_back({i, j});
      if (nextSpot == same) {
        captures.insert(captures.end(), temp.begin(), temp.end());
        break;
      }
      if (nextSpot == State::EMPTY)
        break;
    }
  }

  {
    Captures temp;
    for (int i = x - 1, j = y + 1; i >= 0 && j < boardSize; --i, ++j) {
      const State &nextSpot = boardState_[i][j];
      if (nextSpot == opposite)
        temp.push_back({i, j});
      if (nextSpot == same) {
        captures.insert(captures.end(), temp.begin(), temp.end());
        break;
      }
      if (nextSpot == State::EMPTY)
        break;
    }
  }

  {
    Captures temp;
    for (int i = x + 1, j = y - 1; i < boardSize && j >= 0; ++i, --j) {
      const State &nextSpot = boardState_[i][j];
      if (nextSpot == opposite)
        temp.push_back({i, j});
      if (nextSpot == same) {
        captures.insert(captures.end(), temp.begin(), temp.end());
        break;
      }
      if (nextSpot == State::EMPTY)
        break;
    }
  }

  return captures;
}

std::pair<int, int> Othello::score() const {
  std::pair<int, int> value{};
  for (const auto &cells : boardState_)
    for (const auto &cell : cells)
      switch (cell) {
      case State::EMPTY:
        break;
      case State::BLACK:
        value.first++;
        break;
      case State::WHITE:
        value.second++;
        break;
      }
  return value;
}

void Othello::calculateLegalMoves() {
  legalMoves_.clear();
  for (int i = 0; i < boardSize; ++i) {
    for (int j = 0; j < boardSize; ++j) {
      Captures captures = captured(i, j, blackTurn);
      if (!captures.empty()) {
        legalMoves_.insert({{i, j}, {std::move(captures)}});
      }
    }
  }
}

bool operator==(const Othello &a, const Othello &b) {
  return a.boardState() == b.boardState();
}
