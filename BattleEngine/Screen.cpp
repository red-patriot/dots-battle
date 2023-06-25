#include "Screen.h"

#include <iostream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

namespace battle {

  const std::array<ftxui::Color, 8> Screen::COLORS = {ftxui::Color::Black,
                                                      ftxui::Color::Blue,
                                                      ftxui::Color::Red,
                                                      ftxui::Color::Green,
                                                      ftxui::Color::Yellow,
                                                      ftxui::Color::Magenta,
                                                      ftxui::Color::Cyan,
                                                      ftxui::Color::LightSlateGrey};

  Screen::Screen(std::int32_t width, std::int32_t height) :
      width_(width),
      height_(height),
      screen_(width_, height_) {
  }

  void Screen::doPlayerSelection(std::function<void(std::string)> loaderFunc) {
    std::string dllFileName;
    std::string errorText;
    bool shouldBattle = false;
    ftxui::InputOption opt;
    opt.on_enter = [&]() {
      try {
        loaderFunc(dllFileName);
        errorText = "";
      } catch (...) {
        errorText = "Error loading player from: " + dllFileName;
      }
    };

    ftxui::Component dllInput = ftxui::Input(&dllFileName, "player", opt);
    ftxui::Component goButton = ftxui::Button("Battle", [&]() { shouldBattle = true; });

    ftxui::Component comp = ftxui::Container::Vertical({dllInput,
                                                        goButton});

    auto renderer = ftxui::Renderer(comp, [&]() {
      return ftxui::vbox({ftxui::text("Player Selction"),
                          ftxui::separator(),
                          dllInput->Render(),
                          goButton->Render(),
                          ftxui::separator(),
                          ftxui::paragraph(errorText)}) |
             ftxui::border;
    });

    auto selectionScreen = ftxui::ScreenInteractive::FitComponent();

    ftxui::Loop loop(&selectionScreen, renderer);
    while (!shouldBattle) {
      loop.RunOnce();
    }
  }

  void Screen::render(const Board& board) {
    auto reset = screen_.ResetPosition();
    drawBattlefield({0, 0}, screen_, board);

    std::cout << reset;
    screen_.Print();
  }

  void Screen::drawBattlefield(Coordinate URS, ftxui::Screen& drawArea, const Board& board) {
    for (std::int32_t y = URS.y; y < height_ + URS.y; ++y) {
      for (std::int32_t x = URS.x; x < width_ + URS.x; ++x) {
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
  }

}  // namespace battle
