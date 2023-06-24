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

  const Board::Space& Board::getSpace(std::int32_t x, std::int32_t y) const noexcept {
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
      return board_[y * width_ + x];
    }
    return outOfBounds_;
  }

  Board::Space& Board::getSpace(std::int32_t x, std::int32_t y) {
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
      return board_[y * width_ + x];
    }
    throw std::domain_error(std::format("{}, {} is out of range for the current game board", x, y));
  }
  std::int32_t Board::getTeam(std::int32_t x, std::int32_t y) const noexcept {
    return getSpace(x, y).team;
  }
  Dot* Board::getDot(std::int32_t x, std::int32_t y) noexcept {
    return getSpace(x, y).dot.get();
  }
  Dot const* Board::getDot(std::int32_t x, std::int32_t y) const noexcept {
    return getSpace(x, y).dot.get();
  }
  void Board::setBox(std::int32_t x, std::int32_t y, Space box) {
    getSpace(x, y) = std::move(box);
  }
  void Board::moveDot(Coordinate from, Coordinate to) {
    // Move can only move to an unoccupied space in the board bounds,
    // if a space is full or out of bounds, ignore a move to it
    if ((to.y >= 0 && to.y < height_ && to.x >= 0 && to.x < width_) &&
        !getTeam(to.x, to.y)) {
      using std::swap;
      swap(getSpace(from.x, from.y), getSpace(to.x, to.y));
    }
    return;
  }
}  // namespace battle
