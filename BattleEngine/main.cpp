#include "Screen.h"
#include "BattleEngine.h"

#include <chrono>
#include <string>

#include "LoadPlayer.h"

using namespace std::chrono_literals;
static const std::chrono::milliseconds loopTime = 50ms;

int main() {
  battle::Engine engine{24, 12};
  battle::Screen screen{24, 12};

  screen.doPlayerSelection([&](const std::string& filename) {
    auto player = battle::loadPlayer(filename);
    auto name = player->getName();
    engine.addNewPlayer(std::move(player));
    return name;
  });

  screen.render(engine.getCurrentBoard(), engine.getTeamControls());

  while (engine.isRunning()) {
    auto now = std::chrono::system_clock::now();
    engine.runOnce();
    screen.render(engine.getCurrentBoard(), engine.getTeamControls());
    std::this_thread::sleep_until(now + loopTime);
  }

  auto winner = engine.getWinner();
  screen.displayGameOver(winner.team, winner.teamName);

  return 0;
}
