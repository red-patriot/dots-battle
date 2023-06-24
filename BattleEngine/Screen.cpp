#include "Screen.h"

#include <iostream>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

namespace battle {

  Screen::Screen(std::int32_t width, std::int32_t height) :
      width_(width),
      height_(height),
      screen_(width_, height_) { }

  void Screen::render(const Board& board) {
    static const std::array<ftxui::Color, 8> COLORS = {ftxui::Color::Black,
                                                       ftxui::Color::Blue,
                                                       ftxui::Color::Red,
                                                       ftxui::Color::Green,
                                                       ftxui::Color::Yellow,
                                                       ftxui::Color::Magenta,
                                                       ftxui::Color::Cyan,
                                                       ftxui::Color::LightSlateGrey};

    auto reset = screen_.ResetPosition();
    screen_.Clear();

    for (std::int32_t y = 0; y < height_; ++y) {
      for (std::int32_t x = 0; x < width_; ++x) {
        std::int32_t team = board.getTeam(Coordinate{x, y});
        if (team) {
          ftxui::Color color = COLORS[team];
          auto& px = screen_.PixelAt(x, y);
          px.character = "O";
          px.foreground_color = color;
        } else {
          ftxui::Color color = ftxui::Color::White;
          auto& px = screen_.PixelAt(x, y);
          px.character = "+";
          px.foreground_color = color;
        }
      }
    }

    std::cout << reset;
    screen_.Print();
  }

}  // namespace battle
