#include "BattleEngine.h"

#include <algorithm>
#include <chrono>
#include <exception>
#include <format>
#include <random>

#include "EmptyDot.h"

namespace battle {

  Engine::Engine(std::uint32_t width, std::uint32_t height) :
      board_(width, height) { 
    std::random_device rd;
    generator_ = std::mt19937{rd()};
  }

  bool Engine::isRunning() {
    return true;
  }

  void Engine::addNewPlayer(std::unique_ptr<Dot> newPlayer) {
    static std::uniform_int_distribution<std::uint32_t> distWidth(0, board_.width() - 1);
    static std::uniform_int_distribution<std::uint32_t> distHeight(0, board_.height() - 1);

    std::uint32_t x = 0;
    std::uint32_t y = 0;

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

  std::vector<std::uint32_t> createIndices(std::uint32_t length) {
    std::vector<std::uint32_t> ret(length);
    std::ranges::generate(ret, [n = 0]() mutable { return n++; });
    return ret;
  }

  void Engine::runOnce() {
    static std::vector<std::uint32_t> xs = createIndices(board_.width());
    static std::vector<std::uint32_t> ys = createIndices(board_.height());

    std::ranges::shuffle(xs, generator_);
    std::ranges::shuffle(ys, generator_);

    for (auto y = ys.begin(); y != ys.end(); ++y) {
      for (auto x = xs.begin(); x != xs.end(); ++x) {
        if (board_.getTeam(*x, *y)) {
          std::array<std::uint32_t, 8> surrounding = getSurrounding(*x, *y);

          Board::Space& current = board_.getSpace(*x, *y);
          auto action = current.dot->run(current.team, surrounding, *x, *y);
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
  void Engine::execute(RunAction action, Coordinate space) {
    switch (action.type) {
    }
  }
}  // namespace battle
