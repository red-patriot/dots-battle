#include "Screen.h"

#include <iostream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

namespace battle {

  const std::array<ftxui::Color, 16> Screen::COLORS = {ftxui::Color::Black,
                                                       ftxui::Color::Blue,
                                                       ftxui::Color::Red,
                                                       ftxui::Color::Green,
                                                       ftxui::Color::Yellow,
                                                       ftxui::Color::Magenta,
                                                       ftxui::Color::Cyan,
                                                       ftxui::Color::LightSlateGrey,
                                                       ftxui::Color::SkyBlue1,
                                                       ftxui::Color::DarkSeaGreen,
                                                       ftxui::Color::DarkKhaki,
                                                       ftxui::Color::DarkTurquoise,
                                                       ftxui::Color::DarkOrange,
                                                       ftxui::Color::DarkGoldenrod,
                                                       ftxui::Color::DarkViolet,
                                                       ftxui::Color::Pink1};

  Screen::Screen(std::int32_t width, std::int32_t height) :
      width_(width),
      height_(height),
      screen_(width_, height_) {
  }

  void Screen::doPlayerSelection(std::function<std::string(std::string)> loaderFunc) {
    std::string dllFileName;
    std::string errorText;
    int players = 0;
    std::vector<ftxui::Element> currentPlayers;
    bool shouldBattle = false;

    ftxui::InputOption opt;
    opt.on_enter = [&]() {
      try {
        if (players >= COLORS.size() - 1) {
          errorText = std::format("Error: the maximum number of players is {}", COLORS.size());
          return;
        }
        auto name = loaderFunc(dllFileName);
        errorText = "";
        ++players;
        currentPlayers.push_back(ftxui::text(name + " (O)") | ftxui::bgcolor(COLORS[players]));
      } catch (...) {
        errorText = "Error loading player from: " + dllFileName;
      }
    };

    ftxui::Component dllInput = ftxui::Input(&dllFileName, "player", opt);
    ftxui::Component goButton = ftxui::Button("Battle", [&]() { shouldBattle = true; });

    ftxui::Component comp = ftxui::Container::Vertical({dllInput,
                                                        goButton});

    auto renderer = ftxui::Renderer(comp, [&]() {
      return ftxui::vbox({ftxui::text("Player Selection"),
                          ftxui::separator(),
                          dllInput->Render(),
                          ftxui::separator(),
                          ftxui::text("Players:"),
                          ftxui::vbox(currentPlayers) | ftxui::border,
                          ftxui::hbox({ftxui::text("") | ftxui::flex,
                                       goButton->Render()}),
                          ftxui::separator(),
                          ftxui::paragraph(errorText)}) |
             ftxui::border;
    });

    auto selectionScreen = ftxui::ScreenInteractive::FitComponent();

    ftxui::Loop loop(&selectionScreen, renderer);
    while (!shouldBattle) {
      loop.RunOnce();
    }

    auto reset = selectionScreen.ResetPosition();
    selectionScreen.Clear();
    std::cout << reset;
    selectionScreen.Print();
    reset = selectionScreen.ResetPosition();
    std::cout << reset;
  }

  void Screen::render(const Board& board, const std::vector<std::int32_t>& teamControls) {
    auto reset = screen_.ResetPosition();
    drawBattlefield({0, 0}, screen_, board);

    std::cout << reset;
    screen_.Print();
  }

  void Screen::displayGameOver(std::int32_t team, const std::string& teamName) {
    auto reset = screen_.ResetPosition();
    std::cout << reset;
    screen_.Clear();
    screen_.Print();
    reset = screen_.ResetPosition();
    std::cout << reset;

    ftxui::Element doc = ftxui::vbox({ftxui::text("GAME OVER!") | ftxui::center,
                                      ftxui::text("WINNER:") | ftxui::center,
                                      ftxui::text(std::format("{} - {}", team, teamName)) |
                                          ftxui::color(COLORS[team]) | ftxui::center}) |
                         ftxui::borderDouble;
    screen_ = ftxui::Screen::Create(ftxui::Dimension::Fit(doc), ftxui::Dimension::Fit(doc));
    ftxui::Render(screen_, doc);
    screen_.Print();
  }

  void Screen::drawBattlefield(Coordinate offset, ftxui::Screen& drawArea, const Board& board) {
    for (std::int32_t y = 0; y < height_; ++y) {
      for (std::int32_t x = 0; x < width_; ++x) {
        std::int32_t team = board.getTeam(Coordinate{x, y});
        if (team) {
          ftxui::Color color = COLORS[team];
          auto& px = screen_.PixelAt(x + offset.x, y + offset.y);
          px.character = "O";
          px.foreground_color = color;
        } else {
          ftxui::Color color = ftxui::Color::White;
          auto& px = screen_.PixelAt(x + offset.x, y + offset.y);
          px.character = "+";
          px.foreground_color = color;
        }
      }
    }
  }

}  // namespace battle
