#include "Screen.h"
#include "BattleEngine.h"
#include "BattlingDots.h"

#include <chrono>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "LoadPlayer.h"

using namespace std::chrono_literals;
static const std::chrono::milliseconds loopTime = 250ms;

int main() {
  battle::Engine engine{6, 6};
  battle::Screen screen{6, 6};

  screen.doPlayerSelection([&](const std::string& filename) {
    auto player = battle::loadPlayer(filename);
    engine.addNewPlayer(std::move(player));
  });

  screen.render(engine.getCurrentBoard());

  while (engine.isRunning()) {
    auto now = std::chrono::system_clock::now();
    engine.runOnce();
    screen.render(engine.getCurrentBoard());
    std::this_thread::sleep_until(now + loopTime);
  }

  return 0;
}
