#include "Board.h"

#include <exception>
#include <format>
#include <utility>

#include "EmptyDot.h"

namespace battle {
  Board::Board(std::int32_t width, std::int32_t height) :
      width_(width),
      height_(height),
      outOfBounds_{.team = 0, .dot = std::make_unique<EmptyDot>()} {
    board_.reserve(width_ * height_);
    for (std::int32_t i = 0; i < width_ * height_; ++i) {
      board_.emplace_back(Space{.team = 0, .dot = std::make_unique<EmptyDot>()});
    }
  }

  const Board::Space& Board::getSpace(Coordinate coord) const noexcept {
    const auto& [x, y] = coord;
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
      return board_[boardIndex(coord)];
    }
    return outOfBounds_;
  }

  Board::Space& Board::getSpace(Coordinate coord) {
    const auto& [x, y] = coord;
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
      return board_[boardIndex(coord)];
    }
    throw std::domain_error(std::format("{}, {} is out of range for the current game board", x, y));
  }
  std::int32_t Board::getTeam(Coordinate coord) const noexcept {
    return getSpace(coord).team;
  }
  Dot* Board::getDot(Coordinate coord) noexcept {
    return getSpace(coord).dot.get();
  }
  Dot const* Board::getDot(Coordinate coord) const noexcept {
    return getSpace(coord).dot.get();
  }
  Board::Space Board::removeDot(Coordinate coord) {
    using std::swap;
    Space ret = {.team = 0,
                 .dot = std::make_unique<EmptyDot>()};
    swap(ret, getSpace(coord));
    return ret;
  }
  bool Board::setSpace(Coordinate coord, Space box) {
    const auto& [x, y] = coord;
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
      getSpace(coord) = std::move(box);
      return true;
    }
    return false;
  }
  bool Board::moveDot(Coordinate from, Coordinate to) {
    // Move can only move to an unoccupied coord in the board bounds,
    // if a coord is full or out of bounds, ignore a move to it
    if ((to.y >= 0 && to.y < height_ && to.x >= 0 && to.x < width_) &&
        !getTeam(to)) {
      using std::swap;
      swap(getSpace(from), getSpace(to));
      return true;
    }
    return false;
  }
  size_t Board::boardIndex(Coordinate space) const noexcept {
    return space.y * width_ + space.x;
  }
}  // namespace battle
