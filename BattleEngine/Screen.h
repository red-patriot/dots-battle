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
    std::int32_t width_;
    std::int32_t height_;

    ftxui::Screen screen_;
    
  };
}  // namespace battle

#endif
