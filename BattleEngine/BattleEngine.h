#ifndef DOTS_BATTLE_BATTLE_ENGINE_BATTLE_ENGINE_H
#define DOTS_BATTLE_BATTLE_ENGINE_BATTLE_ENGINE_H

#include <cstdint>
#include <memory>
#include <span>
#include <vector>
#include <utility>

#include <Dot.h>

#include "Board.h"

namespace battle {
  class Engine {
   public:
    Engine(std::uint32_t width, std::uint32_t height);

    bool isRunning();

    void addNewPlayer(std::unique_ptr<Dot> newPlayer);
    void runOnce();

    const Board& getCurrentBoard() const noexcept;

   private:
    Board board_;

    std::uint32_t currentTeam_ = 0;

    std::array<std::uint32_t, 8> getSurrounding(std::uint32_t x, std::uint32_t y) const;
  };
}  // namespace battle

#endif
