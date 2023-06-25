#include "RandomActionDot.h"

#include <random>

std::string RandomActionDot::getName() const noexcept {
  return "Random";
}
void RandomActionDot::wasEliminated(std::int32_t team, std::int32_t killer, std::int32_t row, std::int32_t column) noexcept {
  return;
}
battle::RunAction RandomActionDot::run(std::int32_t team, std::array<std::int32_t, 8> view, std::int32_t row, std::int32_t column) noexcept {
  static std::random_device rd;
  static std::mt19937 generator{rd()};
  static std::uniform_int_distribution actDist(0, 3);
  static std::uniform_int_distribution dirDist(0, 7);

  battle::RunAction::Type action = static_cast<battle::RunAction::Type>(actDist(generator));
  battle::Direction direction = static_cast<battle::Direction>(dirDist(generator));

  if (action != battle::RunAction::ATTACK && view[direction]) {
    action = battle::RunAction::WAIT;
  }
  switch (action) {
    case battle::RunAction::WAIT:
      return battle::RunAction{.type = battle::RunAction::WAIT};
      break;
    case battle::RunAction::MOVE:
      return battle::RunAction{.type = battle::RunAction::MOVE,
                               .direction = direction};
    case battle::RunAction::ATTACK:
      return battle::RunAction{.type = battle::RunAction::ATTACK,
                               .direction = direction};
    case battle::RunAction::REPLICATE:
      return battle::RunAction{.type = battle::RunAction::REPLICATE,
                               .direction = direction,
                               .replicated = std::make_unique<RandomActionDot>()};
      break;
  }
}

std::unique_ptr<class battle::Dot> createInitialDot() noexcept {
  return std::make_unique<RandomActionDot>();
}
