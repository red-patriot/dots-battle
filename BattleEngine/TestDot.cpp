#include "TestDot.h"

std::string TestDot::getName() const noexcept {
  return "TEST";
}

void TestDot::wasEliminated(std::int32_t team, std::int32_t killer,
                               std::int32_t row, std::int32_t column) noexcept {
}

battle::RunAction TestDot::run(std::int32_t team, std::array<std::int32_t, 8> view,
                                  std::int32_t row, std::int32_t column) noexcept {
  return battle::RunAction{.type = battle::RunAction::WAIT};
}
