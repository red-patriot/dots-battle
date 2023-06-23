#include "Board.h"

#include <exception>
#include <format>
#include <utility>

#include "EmptyDot.h"

namespace battle {
  Board::Board(std::uint32_t width, std::uint32_t height) :
      width_(width),
      height_(height),
      outOfBounds_{.team = 0, .square = std::make_unique<EmptyDot>()} {
    board_.reserve(width_ * height_);
    for (std::uint32_t i = 0; i < width_ * height_; ++i) {
      board_.emplace_back(Box{.team = 0, .square = std::make_unique<EmptyDot>()});
    }
  }

  const Board::Box& Board::getBox(std::uint32_t x, std::uint32_t y) const noexcept {
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
      return board_[y * width_ + x];
    }
    return outOfBounds_;
  }

  Board::Box& Board::getBox(std::uint32_t x, std::uint32_t y) {
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
      return board_[y * width_ + x];
    }
    throw std::domain_error(std::format("{}, {} is out of range for the current game board", x, y));
  }
  std::uint32_t Board::getTeam(std::uint32_t x, std::uint32_t y) const noexcept {
    return getBox(x, y).team;
  }
  Dot* Board::getSquare(std::uint32_t x, std::uint32_t y) noexcept {
    return getBox(x, y).square.get();
  }
  Dot const* Board::getSquare(std::uint32_t x, std::uint32_t y) const noexcept {
    return getBox(x, y).square.get();
  }
  void Board::setBox(std::uint32_t x, std::uint32_t y, Box box) {
    getBox(x, y) = std::move(box);
  }
}  // namespace battle
