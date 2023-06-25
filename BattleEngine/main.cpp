#include "Screen.h"
#include "BattleEngine.h"

#include <chrono>
#include <string>

#include "LoadPlayer.h"

using namespace std::chrono_literals;
static const std::chrono::milliseconds loopTime = 150ms;

int main() {
  battle::Engine engine{6, 6};
  battle::Screen screen{6, 6};

  screen.doPlayerSelection([&](const std::string& filename) {
    auto player = battle::loadPlayer(filename);
    auto name = player->getName();
    engine.addNewPlayer(std::move(player));
    return name;
  });

  screen.render(engine.getCurrentBoard());

  while (engine.isRunning()) {
    auto now = std::chrono::system_clock::now();
    engine.runOnce();
    screen.render(engine.getCurrentBoard());
    std::this_thread::sleep_until(now + loopTime);
  }

  auto winner = engine.getWinner();
  screen.displayGameOver(winner.team, winner.teamName);

  return 0;
}
