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
      board_(width, height),
      moveCounters_(width * height, 0) {
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
    } while (board_.getTeam(Coordinate{x, y}) != 0 && count < 100);

    if (count >= 100) {
      throw std::logic_error(std::format("Cannot find a space for new player on team {}",
                                         currentTeam_ + 1));
    }

    ++currentTeam_;
    board_.setSpace(Coordinate{x, y}, {.team = currentTeam_, .dot = std::move(newPlayer)});
  }

  void Engine::runOnce() {
    static std::vector<std::int32_t> xs = createIndices(board_.width());
    static std::vector<std::int32_t> ys = createIndices(board_.height());

    std::ranges::shuffle(xs, generator_);
    std::ranges::shuffle(ys, generator_);

    for (auto y = ys.begin(); y != ys.end(); ++y) {
      for (auto x = xs.begin(); x != xs.end(); ++x) {
        const Coordinate current{*x, *y};

        if (!hasMoved(current)) {
          markMoved(current);
          if (board_.getTeam(current)) {
            std::array<std::int32_t, 8> surrounding = getSurrounding(*x, *y);

            Board::Space& space = board_.getSpace(current);
            RunAction action = space.dot->run(space.team, surrounding, *x, *y);
            execute(std::move(action), current);
          }
        }
      }
    }

    ++turnCounter_;
  }

  const Board& Engine::getCurrentBoard() const noexcept {
    return board_;
  }

  std::array<std::int32_t, 8> Engine::getSurrounding(std::int32_t x, std::int32_t y) const {
    std::array<std::int32_t, 8> ret{
        board_.getTeam(Coordinate{x - 1, y - 1}),
        board_.getTeam(Coordinate{x, y - 1}),
        board_.getTeam(Coordinate{x + 1, y - 1}),
        board_.getTeam(Coordinate{x - 1, y}),
        board_.getTeam(Coordinate{x + 1, y}),
        board_.getTeam(Coordinate{x - 1, y + 1}),
        board_.getTeam(Coordinate{x, y + 1}),
        board_.getTeam(Coordinate{x + 1, y + 1})};
    return ret;
  }

  bool Engine::hasMoved(Coordinate space) const noexcept {
    return moveCounters_[board_.boardIndex(space)] == turnCounter_;
  }

  void Engine::markMoved(Coordinate space) noexcept {
    moveCounters_[board_.boardIndex(space)] = turnCounter_;
  }

  void Engine::execute(RunAction action, Coordinate space) {
    using std::swap;
    switch (action.type) {
      case RunAction::MOVE:
        auto newSpace = calculateMoveCoord(space, action.direction);
        if (board_.moveDot(space, newSpace)) {
          swap(moveCounters_[board_.boardIndex(space)],
               moveCounters_[board_.boardIndex(newSpace)]);
        }
        break;
      default:
        // IMPOSSIBLE?
        // TODO: Remove this case once all actions are implemeted
        throw std::exception{};
    }
  }
}  // namespace battle
