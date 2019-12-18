#pragma once

#include <array>
#include <utility>
#include <vector>
#include <unordered_map>

struct HashPair {
    template< class T1, class T2 >
    size_t operator()( const std::pair<T1, T2>& p ) const {
        auto hash1 = std::hash<T1>{}( p.first );
        auto hash2 = std::hash<T2>{}( p.second );
        return hash1 ^ hash2;
    }
};

class Othello {
public:
    enum class State {
        EMPTY, WHITE, BLACK
    };
    using BoardState = std::array<std::array<State, 8>, 8>;
    using Captures   = std::vector<std::pair<int, int>>;
    using LegalMoves = std::unordered_map<std::pair<int, int>, Captures, HashPair>;

    Othello();

    /**
     * Gets the current score as a pair, black score first, white score second
     * @return
     */
    [[nodiscard]] std::pair<int, int> score() const;

    [[nodiscard]] Captures captured( int x, int y, bool isBlack ) const;

    void placePiece( int x, int y, bool isBlack, const Captures& captures );

    [[nodiscard]] const BoardState& boardState() const { return boardState_; }

    [[nodiscard]] bool isBlackTurn() const { return blackTurn; }

    [[nodiscard]] LegalMoves legalMoves() const;

private:
    BoardState boardState_;
    bool       blackTurn = true;
};



