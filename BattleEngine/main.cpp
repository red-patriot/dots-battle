#include "Screen.h"
#include "BattleEngine.h"
#include "BattlingDots.h"

#include <chrono>

using namespace std::chrono_literals;
static const std::chrono::milliseconds loopTime = 250ms;

int main() {
  battle::Engine engine{6, 6};
  battle::Screen screen{6, 6};

  engine.addNewPlayer(std::make_unique<battle::test::Test1>());
  engine.addNewPlayer(std::make_unique<battle::test::Test1>());
  engine.addNewPlayer(std::make_unique<battle::test::Test1>());
  engine.addNewPlayer(std::make_unique<battle::test::Test1>());
  screen.render(engine.getCurrentBoard());

  while (engine.isRunning()) {
    auto now = std::chrono::system_clock::now();
    engine.runOnce();
    screen.render(engine.getCurrentBoard());
    std::this_thread::sleep_until(now + loopTime);
  }

  return 0;
}
