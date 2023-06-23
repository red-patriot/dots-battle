#include "TestDot.h"

std::string TestDot::getName() const noexcept {
  return "TEST";
}

void TestDot::wasEliminated(std::uint32_t team, std::uint32_t killer,
                               std::uint32_t row, std::uint32_t column) noexcept {
}

battle::RunAction TestDot::run(std::uint32_t team, std::array<std::uint32_t, 8> view,
                                  std::uint32_t row, std::uint32_t column) noexcept {
  return battle::RunAction{.type = battle::RunAction::WAIT};
}
