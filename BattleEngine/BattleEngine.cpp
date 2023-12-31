#include "BattleEngine.h"

#include <algorithm>
#include <chrono>
#include <exception>
#include <format>
#include <random>

#include "EmptyDot.h"
#include "EngineUtility.h"
#include "ReplicatingDot.h"

namespace battle {

  Engine::Engine(std::int32_t width, std::int32_t height) :
      board_(width, height),
      moveCounters_(width * height, 0) {
    std::random_device rd;
    generator_ = std::mt19937{rd()};
    teamControl_.push_back(0);
  }

  bool Engine::isRunning() const noexcept {
    auto teams = std::ranges::count_if(teamControl_, [](std::int32_t control) -> bool {
      return control > 0;
    });
    return teams >= 2;
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
    teamControl_.push_back(1);
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

  const std::vector<std::int32_t>& Engine::getTeamControls() const noexcept {
    return teamControl_;
  }

  WinnerData Engine::getWinner() const noexcept {
    if (isRunning()) {
      return {};
    }
    WinnerData ret;

    for (auto y = 0; y != board_.height(); ++y) {
      for (auto x = 0; x != board_.width(); ++x) {
        if (board_.getTeam(Coordinate{x, y})) {
          ret.team = board_.getTeam(Coordinate{x, y});
          ret.teamName = board_.getDot(Coordinate{x, y})->getName();
          return ret;
        }
      }
    }
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
    auto& [x, y] = space;
    if (y >= 0 && y < board_.height() && x >= 0 && x < board_.width()) {
      return moveCounters_[board_.boardIndex(space)] == turnCounter_;
    }
    return true;
  }

  void Engine::markMoved(Coordinate space) noexcept {
    auto& [x, y] = space;
    if (y >= 0 && y < board_.height() && x >= 0 && x < board_.width()) {
      moveCounters_[board_.boardIndex(space)] = turnCounter_;
    }
  }

  void Engine::execute(RunAction action, Coordinate space) {
    switch (action.type) {
      case RunAction::WAIT:
        markMoved(space);
        break;
      case RunAction::MOVE:
        {
          auto newSpace = calculateMoveCoord(space, action.direction);
          doMove(space, newSpace);
          break;
        }
      case RunAction::ATTACK:
        {
          auto target = calculateMoveCoord(space, action.direction);
          doAttack(space, target);
          break;
        }
      case RunAction::REPLICATE:
        {
          startReplicate(space, std::move(action.replicated), action.direction);
          break;
        }
      default:
        // IMPOSSIBLE?
        // TODO: Remove this case once all actions are implemented
        throw std::exception{};
    }
  }

  void Engine::doMove(Coordinate from, Coordinate to) {
    using std::swap;
    if (board_.inBounds(to) && board_.moveDot(from, to)) {
      swap(moveCounters_[board_.boardIndex(from)],
           moveCounters_[board_.boardIndex(to)]);
    }
  }

  void Engine::doAttack(Coordinate attacker, Coordinate target) {
    if (board_.getTeam(target) && board_.getTeam(attacker) != board_.getTeam(target)) {
      auto targetDot = board_.getDot(target);
      targetDot->wasEliminated(board_.getTeam(target),
                               board_.getTeam(attacker), target.x, target.y);
      --teamControl_[board_.getTeam(target)];
      board_.setSpace(target, {.team = 0, .dot = std::make_unique<EmptyDot>()});
    }
    doMove(attacker, target);
  }

  void Engine::startReplicate(Coordinate location,
                              std::unique_ptr<Dot> child, Direction dir) {
    auto parent = board_.removeDot(location);
    std::unique_ptr<Dot> parentWrapper = std::make_unique<ReplicatingDot>(std::move(parent.dot),
                                                                          this);
    board_.setSpace(location, {.team = parent.team, .dot = std::move(parentWrapper)});
    auto birthplace = calculateMoveCoord(location, dir);
    if (!board_.getTeam(birthplace) && board_.inBounds(birthplace)) {
      std::unique_ptr<Dot> childWrapper = std::make_unique<ReplicatingDot>(std::move(child),
                                                                           this);
      if (board_.setSpace(birthplace, {.team = board_.getTeam(location),
                                       .dot = std::move(childWrapper)})) {
        ++teamControl_[board_.getTeam(location)];
      }

      markMoved(birthplace);
    }
    markMoved(location);
  }

  void Engine::finishReplicate(std::unique_ptr<Dot> parent, Coordinate location) {
    board_.setSpace(location, {.team = board_.getTeam(location),
                               .dot = std::move(parent)});
  }

}  // namespace battle
