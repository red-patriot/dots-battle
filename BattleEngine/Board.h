#ifndef DOTS_BATTLE_BATTLE_ENGINE_BOARD_H
#define DOTS_BATTLE_BATTLE_ENGINE_BOARD_H

#include <cstdint>
#include <memory>
#include <vector>
#include <utility>

#include <Dot.h>

namespace battle {
  class Board {
   public:
    struct Box {
      std::uint32_t team;
      std::unique_ptr<Dot> square;
    };

    Board(std::uint32_t width, std::uint32_t height);

    std::uint32_t width() const noexcept { return width_; }
    std::uint32_t height() const noexcept { return height_; }

    const Box& getBox(std::uint32_t x, std::uint32_t y) const noexcept;
    Box& getBox(std::uint32_t x, std::uint32_t y);

    std::uint32_t getTeam(std::uint32_t x, std::uint32_t y) const noexcept;
    Dot* getSquare(std::uint32_t x, std::uint32_t y) noexcept;
    Dot const* getSquare(std::uint32_t x, std::uint32_t y) const noexcept;

    void setBox(std::uint32_t x, std::uint32_t y, Box box);

   private:
    std::uint32_t width_;
    std::uint32_t height_;
    std::vector<Box> board_;
    Box outOfBounds_;
  };
}  // namespace battle

#endif
