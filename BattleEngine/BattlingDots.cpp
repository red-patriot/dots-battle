#include "BattlingDots.h"

#include <random>

namespace battle::test {
  std::string Test1::getName() const noexcept {
    return "Random";
  }
  void Test1::wasEliminated(std::int32_t team, std::int32_t killer, std::int32_t row, std::int32_t column) noexcept {
    return;
  }
  battle::RunAction Test1::run(std::int32_t team, std::array<std::int32_t, 8> view, std::int32_t row, std::int32_t column) noexcept {
    static std::random_device rd;
    static std::mt19937 generator{rd()};
    static std::uniform_int_distribution actDist(0, 3);
    static std::uniform_int_distribution dirDist(0, 7);

    battle::RunAction::Type action = static_cast<::battle::RunAction::Type>(actDist(generator));
    battle::Direction direction = static_cast<::battle::Direction>(dirDist(generator));
    switch (action) {
      case battle::RunAction::Type::WAIT:
        return battle::RunAction{.type = battle::RunAction::WAIT};
        break;
      case battle::RunAction::Type::MOVE:
        return battle::RunAction{.type = battle::RunAction::MOVE,
                                 .direction = direction};
      case battle::RunAction::Type::ATTACK:
        return battle::RunAction{.type = battle::RunAction::ATTACK,
                                 .direction = direction};
      case battle::RunAction::Type::REPLICATE:
        return battle::RunAction{.type = battle::RunAction::REPLICATE,
                                 .direction = direction,
                                 .replicated = std::make_unique<Test1>()};
        break;
      default:
        break;
    }
  }

  std::string Test2::getName() const noexcept {
    return "Multiply";
  }
  void Test2::wasEliminated(std::int32_t team, std::int32_t killer, std::int32_t row, std::int32_t column) noexcept {
  }
  battle::RunAction Test2::run(std::int32_t team, std::array<std::int32_t, 8> view, std::int32_t row, std::int32_t column) noexcept {
    return battle::RunAction();
  }

  std::string Test3::getName() const noexcept {
    return "Always Attack";
  }
  void Test3::wasEliminated(std::int32_t team, std::int32_t killer, std::int32_t row, std::int32_t column) noexcept {
  }
  battle::RunAction Test3::run(std::int32_t team, std::array<std::int32_t, 8> view, std::int32_t row, std::int32_t column) noexcept {
    return battle::RunAction();
  }
}  // namespace battle::test
