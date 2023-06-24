#include "Screen.h"
#include "BattleEngine.h"
#include "BattlingDots.h"

int main() {
  battle::Engine engine{6, 4};
  battle::Screen screen{6, 4};

  engine.addNewPlayer(std::make_unique<battle::test::Test1>());

  while (engine.isRunning()) {
    engine.runOnce();
    screen.render(engine.getCurrentBoard());
  }

  return 0;
}
