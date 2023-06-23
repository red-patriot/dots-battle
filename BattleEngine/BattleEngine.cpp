#include "BattleEngine.h"

#include <chrono>
#include <exception>
#include <format>
#include <random>

#include "EmptyDot.h"

namespace battle {

  Engine::Engine(std::uint32_t width, std::uint32_t height) :
      board_(width, height) { }

  bool Engine::isRunning() {
    return true;
  }

  void Engine::addNewPlayer(std::unique_ptr<Dot> newPlayer) {
    std::random_device rd;
    static std::mt19937 generator{rd()};
    static std::uniform_int_distribution<std::uint32_t> distWidth(0, board_.width() - 1);
    static std::uniform_int_distribution<std::uint32_t> distHeight(0, board_.height() - 1);

    std::uint32_t x = 0;
    std::uint32_t y = 0;

    int count = 0;
    do {
      x = distWidth(generator);
      y = distHeight(generator);
      ++count;
    } while (board_.getTeam(x, y) != 0 && count < 100);

    if (count >= 100) {
      throw std::logic_error(std::format("Cannot find a space for new player on team {}",
                                         currentTeam_ + 1));
    }

    ++currentTeam_;
    board_.setBox(x, y, {.team = currentTeam_, .square = std::move(newPlayer)});
  }

  void Engine::runOnce() {
    for (std::uint32_t y = 0; y < board_.height(); ++y) {
      for (std::uint32_t x = 0; x < board_.width(); ++x) {
        if (board_.getTeam(x, y)) {
          std::array<std::uint32_t, 8> surrounding = getSurrounding(x, y);

          Board::Box& current = board_.getBox(x, y);
          current.square->run(current.team, surrounding, x, y);
        }
      }
    }
  }

  const Board& Engine::getCurrentBoard() const noexcept {
    return board_;
  }

  std::array<std::uint32_t, 8> Engine::getSurrounding(std::uint32_t x, std::uint32_t y) const {
    std::array<std::uint32_t, 8> ret;
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
}  // namespace battle
