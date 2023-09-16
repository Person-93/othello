#pragma once

#include <array>
#include <unordered_map>
#include <utility>
#include <vector>

struct HashPair {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

class Othello {
public:
  enum class State { EMPTY, WHITE, BLACK };
  static constexpr int boardSize = 8;
  using BoardState = std::array<std::array<State, boardSize>, boardSize>;
  using Captures = std::vector<std::pair<int, int>>;
  using LegalMoves =
      std::unordered_map<std::pair<int, int>, Captures, HashPair>;

  Othello();

  Othello(const Othello &) = default;

  Othello(Othello &&) = default;

  Othello &operator=(const Othello &) = default;

  Othello &operator=(Othello &&) = default;

  /**
   * Gets the current score as a pair, black score first, white score second
   * @return
   */
  [[nodiscard]] std::pair<int, int> score() const;

  [[nodiscard]] Captures captured(int x, int y, bool isBlack) const;

  void placePiece(int x, int y);

  [[nodiscard]] const BoardState &boardState() const { return boardState_; }

  [[nodiscard]] bool isBlackTurn() const { return blackTurn; }

  [[nodiscard]] const LegalMoves &legalMoves() const { return legalMoves_; }

private:
  void calculateLegalMoves();

  LegalMoves legalMoves_;
  BoardState boardState_;
  bool blackTurn = true;
};

bool operator==(const Othello &a, const Othello &b);

inline bool operator!=(const Othello &a, const Othello &b) { return !(a == b); }
