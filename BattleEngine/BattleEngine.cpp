#include "BattleEngine.h"

#include <algorithm>
#include <chrono>
#include <exception>
#include <format>
#include <random>

#include "EmptyDot.h"
#include "EngineUtility.h"

namespace battle {

  Engine::Engine(std::int32_t width, std::int32_t height) :
      board_(width, height) {
    std::random_device rd;
    generator_ = std::mt19937{rd()};
  }

  bool Engine::isRunning() {
    return true;
  }

  void Engine::addNewPlayer(std::unique_ptr<Dot> newPlayer) {
    static std::uniform_int_distribution<std::int32_t> distWidth(0, board_.width() - 1);
    static std::uniform_int_distribution<std::int32_t> distHeight(0, board_.height() - 1);

    std::int32_t x = 0;
    std::int32_t y = 0;

    int count = 0;
    do {
      x = distWidth(generator_);
      y = distHeight(generator_);
      ++count;
    } while (board_.getTeam(x, y) != 0 && count < 100);

    if (count >= 100) {
      throw std::logic_error(std::format("Cannot find a space for new player on team {}",
                                         currentTeam_ + 1));
    }

    ++currentTeam_;
    board_.setBox(x, y, {.team = currentTeam_, .dot = std::move(newPlayer)});
  }

  void Engine::runOnce() {
    static std::vector<std::int32_t> xs = createIndices(board_.width());
    static std::vector<std::int32_t> ys = createIndices(board_.height());

    std::ranges::shuffle(xs, generator_);
    std::ranges::shuffle(ys, generator_);

    for (auto y = ys.begin(); y != ys.end(); ++y) {
      for (auto x = xs.begin(); x != xs.end(); ++x) {
        if (board_.getTeam(*x, *y)) {
          std::array<std::int32_t, 8> surrounding = getSurrounding(*x, *y);

          Board::Space& current = board_.getSpace(*x, *y);
          RunAction action = current.dot->run(current.team, surrounding, *x, *y);
          execute(std::move(action), Coordinate{*x, *y});
        }
      }
    }
  }

  const Board& Engine::getCurrentBoard() const noexcept {
    return board_;
  }

  std::array<std::int32_t, 8> Engine::getSurrounding(std::int32_t x, std::int32_t y) const {
    std::array<std::int32_t, 8> ret;
    ret[0] = board_.getTeam(x - 1, y - 1);
    ret[1] = board_.getTeam(x, y - 1);
    ret[2] = board_.getTeam(x + 1, y - 1);
    ret[3] = board_.getTeam(x - 1, y);
    ret[4] = board_.getTeam(x + 1, y);
    ret[5] = board_.getTeam(x - 1, y + 1);
    ret[6] = board_.getTeam(x, y + 1);
    ret[7] = board_.getTeam(x + 1, y + 1);
    return ret;
  }

  void Engine::execute(RunAction action, Coordinate space) {
    switch (action.type) {
      case RunAction::MOVE:
        auto newSpace = calculateMoveCoord(space, action.direction);
        board_.moveDot(space, newSpace);
        break;
      default:
        // IMPOSSIBLE?
        // TODO: Remove this case once all actions are implemeted
        throw std::exception{};
    }
  }
}  // namespace battle
