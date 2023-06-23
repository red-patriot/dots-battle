#ifndef DOTS_BATTLESQUARE_SQUARE_H
#define DOTS_BATTLESQUARE_SQUARE_H

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

namespace battle {
  enum class Direction : std::uint32_t {
    NW = 0,
    N = 1,
    NE = 2,
    E = 3,
    SE = 4,
    S = 5,
    SW = 6,
    W = 7
  };

  struct RunAction {
    enum class Type {
      WAIT,
      MOVE,
      ATTACK,
      REPLICATE
    };
    using enum Type;
    Type type;
    Direction direction;
    std::unique_ptr<class Dot> replicated;
  };

  class Dot {
   public:
    virtual ~Dot() = default;

    virtual std::string getName() const noexcept = 0;
    virtual void wasEliminated(std::uint32_t team, std::uint32_t killer,
                               std::uint32_t row, std::uint32_t column) noexcept = 0;

    virtual RunAction run(std::uint32_t team, std::array<std::uint32_t, 8> view,
                          std::uint32_t row, std::uint32_t column) noexcept = 0;
  };
}  // namespace battle

#endif
