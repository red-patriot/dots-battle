#ifndef DOTS_BATTLE_BATTLE_ENGINE_BATTLE_ENGINE_H
#define DOTS_BATTLE_BATTLE_ENGINE_BATTLE_ENGINE_H

#include <cstdint>
#include <memory>
#include <random>
#include <span>
#include <vector>
#include <utility>

#include <Dot.h>

#include "Board.h"

namespace battle {
  struct WinnerData {
    std::int32_t team;
    std::string teamName;
  };

  class Engine {
   public:
    Engine(std::int32_t width, std::int32_t height);

    bool isRunning() const noexcept;

    void addNewPlayer(std::unique_ptr<Dot> newPlayer);
    void runOnce();


    const Board& getCurrentBoard() const noexcept;
    WinnerData getWinner() const noexcept;

   private:
    Board board_;
    std::uint8_t turnCounter_ = -1;
    std::vector<std::uint8_t> moveCounters_;
    std::vector<std::int32_t> teamControl_;
    std::int32_t currentTeam_ = 0;
    std::mt19937 generator_;

    std::array<std::int32_t, 8> getSurrounding(std::int32_t x, std::int32_t y) const;
    bool hasMoved(Coordinate space) const noexcept;
    void markMoved(Coordinate space) noexcept;

    void execute(RunAction action, Coordinate space);

    void doMove(Coordinate space, Coordinate newSpace);
    void doAttack(Coordinate attacker, Coordinate target);
    void doReplicate(Coordinate parent, std::unique_ptr<Dot> replicated, Coordinate birthplace);
  };
}  // namespace battle

#endif
