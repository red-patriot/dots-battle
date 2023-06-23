#ifndef DOTS_BATTLE_BATTLE_ENGINE_TEST_SQUARE_H
#define DOTS_BATTLE_BATTLE_ENGINE_TEST_SQUARE_H

#include <Dot.h>

class TestDot : public battle::Dot {
 public:
  std::string getName() const noexcept override;
  void wasEliminated(std::uint32_t team, std::uint32_t killer,
                     std::uint32_t row, std::uint32_t column) noexcept override;

  battle::RunAction run(std::uint32_t team, std::array<std::uint32_t, 8> view,
                        std::uint32_t row, std::uint32_t column) noexcept override;
};

#endif
