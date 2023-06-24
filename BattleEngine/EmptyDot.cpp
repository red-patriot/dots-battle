#include "EmptyDot.h"

namespace battle {
  std::string EmptyDot::getName() const noexcept {
    return "";
  }
  void EmptyDot::wasEliminated(std::int32_t team, std::int32_t killer,
                                  std::int32_t row, std::int32_t column) noexcept {
  }
  RunAction EmptyDot::run(std::int32_t team, std::array<std::int32_t, 8> view, std::int32_t row, std::int32_t column) noexcept {
    return RunAction{.type = RunAction::WAIT};
  }
}  // namespace battle
