#include "Screen.h"
#include "BattleEngine.h"
#include "TestDot.h"

int main() {
  battle::Engine engine{6, 4};
  battle::Screen screen{6, 4};

  engine.addNewPlayer(std::make_unique<TestDot>());
  engine.addNewPlayer(std::make_unique<TestDot>());
  engine.addNewPlayer(std::make_unique<TestDot>());

  while (engine.isRunning()) {
    engine.runOnce();
    screen.render(engine.getCurrentBoard());
  }

  return 0;
}
