#ifndef DOTS_BATTLE_BATTLE_ENGINE_BOARD_H
#define DOTS_BATTLE_BATTLE_ENGINE_BOARD_H

#include <cstdint>
#include <memory>
#include <vector>
#include <utility>

#include <Dot.h>

namespace battle {
  struct Coordinate {
    std::int32_t x;
    std::int32_t y;
  };

  class Board {
   public:
    struct Space {
      std::int32_t team;
      std::unique_ptr<Dot> dot;
    };

    Board(std::int32_t width, std::int32_t height);

    std::int32_t width() const noexcept { return width_; }
    std::int32_t height() const noexcept { return height_; }

    const Space& getSpace(std::int32_t x, std::int32_t y) const noexcept;
    Space& getSpace(std::int32_t x, std::int32_t y);

    std::int32_t getTeam(std::int32_t x, std::int32_t y) const noexcept;
    Dot* getDot(std::int32_t x, std::int32_t y) noexcept;
    Dot const* getDot(std::int32_t x, std::int32_t y) const noexcept;

    void setBox(std::int32_t x, std::int32_t y, Space box);

    void moveDot(Coordinate from, Coordinate to);

   private:
    std::int32_t width_;
    std::int32_t height_;
    std::vector<Space> board_;
    Space outOfBounds_;
  };
}  // namespace battle

#endif
