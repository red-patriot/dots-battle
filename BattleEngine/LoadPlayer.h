#ifndef DOTS_BATTLE_BATTLE_ENGINE_LOAD_PLAYER_H
#define DOTS_BATTLE_BATTLE_ENGINE_LOAD_PLAYER_H

#include <memory>
#include <string>

#include <Dot.h>

namespace battle {
  std::unique_ptr<Dot> loadPlayer(const std::string& playerDLL);
}

#endif
