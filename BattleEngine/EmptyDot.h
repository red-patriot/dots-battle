#ifndef DOTS_BATTLE_BATTLE_ENGINE_EMPTY_SQUARE_H
#define DOTS_BATTLE_BATTLE_ENGINE_EMPTY_SQUARE_H

#include <Dot.h>

namespace battle {
  class EmptyDot : public Dot {
   public:
    std::string getName() const noexcept override;
    void wasEliminated(std::int32_t team, std::int32_t killer,
                       std::int32_t row, std::int32_t column) noexcept override;

    RunAction run(std::int32_t team, std::array<std::int32_t, 8> view,
                  std::int32_t row, std::int32_t column) noexcept override;
  };
}  // namespace battle

#endif
