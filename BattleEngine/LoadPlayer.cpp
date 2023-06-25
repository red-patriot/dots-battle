#include "LoadPlayer.h"

#include <exception>
#include <format>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using CreateFunction = std::unique_ptr<battle::Dot> (*)();

namespace battle {
  std::unique_ptr<Dot> battle::loadPlayer(const std::string& playerDLL) {
    HMODULE dllModule = LoadLibraryA(playerDLL.c_str());
    if (!dllModule) {
      throw std::logic_error(std::format("Failed to load {} with error {}",
                                         playerDLL, GetLastError()));
    }

    CreateFunction createDot = (CreateFunction)GetProcAddress(dllModule, "createInitialDot");
    if (!createDot) {
      FreeLibrary(dllModule);
      throw std::logic_error(std::format("'createInitialDot' is not defined, {} has been unloaded",
                                         playerDLL));
    }
    return createDot();
  }
}  // namespace battle
