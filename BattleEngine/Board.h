#ifndef DOTS_BATTLE_BATTLE_ENGINE_BOARD_H
#define DOTS_BATTLE_BATTLE_ENGINE_BOARD_H

#include <cstdint>
#include <memory>
#include <vector>
#include <utility>

#include <Dot.h>

namespace battle {
  struct Coordinate {
    std::uint32_t x;
    std::uint32_t y;
  };

  class Board {
   public:
    struct Space {
      std::uint32_t team;
      std::unique_ptr<Dot> dot;
    };

    Board(std::uint32_t width, std::uint32_t height);

    std::uint32_t width() const noexcept { return width_; }
    std::uint32_t height() const noexcept { return height_; }

    const Space& getSpace(std::uint32_t x, std::uint32_t y) const noexcept;
    Space& getSpace(std::uint32_t x, std::uint32_t y);

    std::uint32_t getTeam(std::uint32_t x, std::uint32_t y) const noexcept;
    Dot* getDot(std::uint32_t x, std::uint32_t y) noexcept;
    Dot const* getDot(std::uint32_t x, std::uint32_t y) const noexcept;

    void setBox(std::uint32_t x, std::uint32_t y, Space box);

    void moveDot(Coordinate original, Direction direction);

   private:
    std::uint32_t width_;
    std::uint32_t height_;
    std::vector<Space> board_;
    Space outOfBounds_;
  };
}  // namespace battle

#endif
