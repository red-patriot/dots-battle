#include "Board.h"

#include <exception>
#include <format>
#include <utility>

#include "EmptyDot.h"

namespace battle {
  Board::Board(std::uint32_t width, std::uint32_t height) :
      width_(width),
      height_(height),
      outOfBounds_{.team = 0, .dot = std::make_unique<EmptyDot>()} {
    board_.reserve(width_ * height_);
    for (std::uint32_t i = 0; i < width_ * height_; ++i) {
      board_.emplace_back(Space{.team = 0, .dot = std::make_unique<EmptyDot>()});
    }
  }

  const Board::Space& Board::getSpace(std::uint32_t x, std::uint32_t y) const noexcept {
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
      return board_[y * width_ + x];
    }
    return outOfBounds_;
  }

  Board::Space& Board::getSpace(std::uint32_t x, std::uint32_t y) {
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
      return board_[y * width_ + x];
    }
    throw std::domain_error(std::format("{}, {} is out of range for the current game board", x, y));
  }
  std::uint32_t Board::getTeam(std::uint32_t x, std::uint32_t y) const noexcept {
    return getSpace(x, y).team;
  }
  Dot* Board::getDot(std::uint32_t x, std::uint32_t y) noexcept {
    return getSpace(x, y).dot.get();
  }
  Dot const* Board::getDot(std::uint32_t x, std::uint32_t y) const noexcept {
    return getSpace(x, y).dot.get();
  }
  void Board::setBox(std::uint32_t x, std::uint32_t y, Space box) {
    getSpace(x, y) = std::move(box);
  }
}  // namespace battle
