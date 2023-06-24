#ifndef DOTS_BATTLE_BATTLE_ENGINE_ENGINE_UTILITY_H
#define DOTS_BATTLE_BATTLE_ENGINE_ENGINE_UTILITY_H

#include <algorithm>
#include <cstdint>
#include <vector>

#include <Dot.h>

#include "Board.h"

namespace battle {
  inline std::vector<std::int32_t> createIndices(std::int32_t length) {
    std::vector<std::int32_t> ret(length);
    std::ranges::generate(ret, [n = 0]() mutable { return n++; });
    return ret;
  }

  inline Coordinate calculateMoveCoord(Coordinate from, Direction dir) {
    switch (dir) {
      case Direction::NW:
        return Coordinate{.x = from.x - 1,
                          .y = from.y - 1};
      case Direction::N:
        return Coordinate{.x = from.x,
                          .y = from.y - 1};
      case Direction::NE:
        return Coordinate{.x = from.x + 1,
                          .y = from.y - 1};
      case Direction::E:
        return Coordinate{.x = from.x + 1,
                          .y = from.y};
      case Direction::SE:
        return Coordinate{.x = from.x + 1,
                          .y = from.y + 1};
      case Direction::S:
        return Coordinate{.x = from.x,
                          .y = from.y + 1};
      case Direction::SW:
        return Coordinate{.x = from.x - 1,
                          .y = from.y + 1};
      case Direction::W:
        return Coordinate{.x = from.x - 1,
                          .y = from.y};
    }
  }

}  // namespace battle

#endif
