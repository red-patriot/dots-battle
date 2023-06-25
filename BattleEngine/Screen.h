#ifndef DOTS_BATTLE_BATTLE_ENGINE_SCREEN_H
#define DOTS_BATTLE_BATTLE_ENGINE_SCREEN_H

#include <span>
#include <cstdint>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/screen.hpp>

#include "Board.h"

namespace battle {
  class Screen {
   public:
    Screen(std::int32_t width, std::int32_t height);

    void render(const Board& board);

   private:
    static const std::array<ftxui::Color, 8> COLORS;

    std::int32_t width_;
    std::int32_t height_;

    ftxui::Screen screen_;

    void drawBattlefield(Coordinate URS, ftxui::Screen& drawArea, const Board& board);
  };
}  // namespace battle

#endif
