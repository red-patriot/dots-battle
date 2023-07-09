#include "Screen.h"

#include <iostream>
#include <string>

#include <nfd.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

using namespace ftxui;

namespace battle {

  const std::array<ftxui::Color, 16> Screen::COLORS = {Color::Black,
                                                       Color::Blue,
                                                       Color::Red,
                                                       Color::Green,
                                                       Color::Yellow,
                                                       Color::Magenta,
                                                       Color::Cyan,
                                                       Color::LightSlateGrey,
                                                       Color::SkyBlue1,
                                                       Color::DarkSeaGreen,
                                                       Color::DarkKhaki,
                                                       Color::DarkTurquoise,
                                                       Color::DarkOrange,
                                                       Color::DarkGoldenrod,
                                                       Color::DarkViolet,
                                                       Color::Pink1};

  Screen::Screen(std::int32_t width, std::int32_t height) :
      width_(width),
      height_(height),
      screen_(width_, height_) { }

  void Screen::doPlayerSelection(std::function<std::string(std::string)> loaderFunc) {
    std::string dllFileName;
    std::string errorText;
    int players = 0;
    std::vector<Element> currentPlayers;
    bool shouldBattle = false;

    ftxui::InputOption opt;
    auto doLoad = [&]() {
      try {
        if (players >= COLORS.size() - 1) {
          errorText = std::format("Error: the maximum number of players is {}", COLORS.size());
          return;
        }
        auto name = loaderFunc(dllFileName);
        errorText = "";
        ++players;
        currentPlayers.push_back(ftxui::text(name) | ftxui::bgcolor(COLORS[players]));
      } catch (...) {
        errorText = "Error loading player from: " + dllFileName;
      }
    };
    opt.on_enter = doLoad;

    ftxui::Component dllInput = ftxui::Input(&dllFileName, "player", opt);
    ftxui::Component goButton = ftxui::Button("Battle", [&]() { shouldBattle = true; });
    ftxui::Component loadButton = ftxui::Button(
        L"\u2BA9 \u2B24", [&]() {
          nfdchar_t* fileName = nullptr;
          nfdresult_t result = NFD_OpenDialog("dll", nullptr, &fileName);

          switch (result) {
            case NFD_OKAY:
              dllFileName = std::string(fileName);
              doLoad();
              break;
            case NFD_CANCEL:
            case NFD_ERROR:
              break;
          }
        },
        ButtonOption::Ascii());

    ftxui::Component comp = Container::Vertical({dllInput,
                                                        goButton,
                                                        loadButton});

    auto renderer = Renderer(comp, [&]() {
      return vbox({text("Player Selection"),
                          separator(),
                          hbox({dllInput->Render() | size(WIDTH, EQUAL, 50),
                                loadButton->Render()}),
                          separator(),
                          text("Players:"),
                          vbox(currentPlayers) | border | size(HEIGHT, EQUAL, COLORS.size() + 2),
                          hbox({ftxui::text("") | flex,
                                goButton->Render()}),
                          separator(),
                          paragraph(errorText)}) |
             border;
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
    Elements stats;
    stats.push_back(separator());
    std::int32_t winner = *std::ranges::max_element(teamControls);
    double boardSize = width_ * height_;
    for (size_t i = 1; i < teamControls.size(); ++i) {
      stats.push_back(ftxui::hbox(
                          {text(teamControls[i] == winner ? L"\u2B50" : L" ") | size(WIDTH, EQUAL, 2),
                           text(std::format("{:3} dots ", teamControls[i])),
                           ftxui::gauge(teamControls[i] / boardSize) | size(WIDTH, EQUAL, 20)}) |
                      color(COLORS[i]));
    }

    Element doc = vbox({text("Dots Battle"),
                        separator(),
                        hbox({window(text(""), text("")) |
                                  size(WIDTH, EQUAL, width_ + 2) |
                                  size(HEIGHT, EQUAL, height_ + 2),
                              vbox(stats)})}) |
                  border;

    screen_ = ftxui::Screen::Create(Dimension::Fit(doc));
    ftxui::Render(screen_, doc);
    // KLUDGE: I can't find a better way to do this, but I wish there was a better
    // way than using hardcoded indicies.
    ftxui::Render(screen_, doc);
    drawBattlefield({2, 4}, screen_, board);

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
          px.character = to_string(L"\u2B24");
          px.foreground_color = color;
        } else {
          ftxui::Color color = ftxui::Color::White;
          auto& px = screen_.PixelAt(x + offset.x, y + offset.y);
          px.character = to_string(L"\u00B7");
          px.foreground_color = color;
        }
      }
    }

    ftxui::Element e;
  }

}  // namespace battle
