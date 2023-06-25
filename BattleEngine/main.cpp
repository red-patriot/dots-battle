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

  std::string debugFilename = "C:\\Users\\bltan\\source\\repos\\DotsBattle\\x64\\Debug\\RandomActionDot.dll";
  
  auto player1 = battle::loadPlayer(debugFilename);
  engine.addNewPlayer(std::move(player1));

  screen.render(engine.getCurrentBoard());

  while (engine.isRunning()) {
    auto now = std::chrono::system_clock::now();
    engine.runOnce();
    screen.render(engine.getCurrentBoard());
    std::this_thread::sleep_until(now + loopTime);
  }

  return 0;
}
