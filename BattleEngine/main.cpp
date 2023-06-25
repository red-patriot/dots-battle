#include "Screen.h"
#include "BattleEngine.h"
#include "BattlingDots.h"

#include <chrono>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace std::chrono_literals;
static const std::chrono::milliseconds loopTime = 250ms;

using CreateFunc = std::unique_ptr<battle::Dot> (*)();

int main() {
  battle::Engine engine{6, 6};
  battle::Screen screen{6, 6};

  std::string debugFilename = "C:\\Users\\bltan\\source\\repos\\DotsBattle\\x64\\Debug\\RandomActionDot.dll";
  HMODULE lib = LoadLibraryA(debugFilename.c_str());
  if (!lib) {
    return 1;
  }
  CreateFunc createDot = (CreateFunc)GetProcAddress(lib, "createInitialDot");

  engine.addNewPlayer(createDot());
  engine.addNewPlayer(createDot());

  screen.render(engine.getCurrentBoard());

  while (engine.isRunning()) {
    auto now = std::chrono::system_clock::now();
    engine.runOnce();
    screen.render(engine.getCurrentBoard());
    std::this_thread::sleep_until(now + loopTime);
  }

  return 0;
}
