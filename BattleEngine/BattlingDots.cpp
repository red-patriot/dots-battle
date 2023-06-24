#include "BattlingDots.h"
#include "BattlingDots.h"

#include <random>

namespace battle::test {
  std::string Test1::getName() const noexcept {
    return "Random";
  }
  void Test1::wasEliminated(std::uint32_t team, std::uint32_t killer, std::uint32_t row, std::uint32_t column) noexcept {
    return;
  }
  battle::RunAction Test1::run(std::uint32_t team, std::array<std::uint32_t, 8> view, std::uint32_t row, std::uint32_t column) noexcept {
    static std::random_device rd;
    static std::mt19937 generator{rd()};
    static std::uniform_int_distribution actDist(0, 3);
    static std::uniform_int_distribution dirDist(0, 7);

    battle::RunAction::Type action = static_cast<::battle::RunAction::Type>(actDist(generator));
    battle::Direction direction = static_cast<::battle::Direction>(dirDist(generator));
    switch (action) {
      case ::battle::RunAction::WAIT:
        return ::battle::RunAction{.type = action};
      case ::battle::RunAction::MOVE:
        return ::battle::RunAction{.type = action,
                                   .direction = direction};
      default:
        return ::battle::RunAction{.type = ::battle::RunAction::WAIT};
    }
  }

  std::string Test2::getName() const noexcept {
    return "Multiply";
  }
  void Test2::wasEliminated(std::uint32_t team, std::uint32_t killer, std::uint32_t row, std::uint32_t column) noexcept {
  }
  battle::RunAction Test2::run(std::uint32_t team, std::array<std::uint32_t, 8> view, std::uint32_t row, std::uint32_t column) noexcept {
    return battle::RunAction();
  }

  std::string Test3::getName() const noexcept {
    return "Always Attack";
  }
  void Test3::wasEliminated(std::uint32_t team, std::uint32_t killer, std::uint32_t row, std::uint32_t column) noexcept {
  }
  battle::RunAction Test3::run(std::uint32_t team, std::array<std::uint32_t, 8> view, std::uint32_t row, std::uint32_t column) noexcept {
    return battle::RunAction();
  }
}  // namespace battle::test
