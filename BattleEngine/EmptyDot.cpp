#include "EmptyDot.h"

namespace battle {
  std::string EmptyDot::getName() const noexcept {
    return "";
  }
  void EmptyDot::wasEliminated(std::uint32_t team, std::uint32_t killer,
                                  std::uint32_t row, std::uint32_t column) noexcept {
  }
  RunAction EmptyDot::run(std::uint32_t team, std::array<std::uint32_t, 8> view, std::uint32_t row, std::uint32_t column) noexcept {
    return RunAction{.type = RunAction::WAIT};
  }
}  // namespace battle
