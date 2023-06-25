#ifndef DOTS_BATTLE_RANDOM_ACTION_DOT_RANDOM_ACTION_DOT_H
#define DOTS_BATTLE_RANDOM_ACTION_DOT_RANDOM_ACTION_DOT_H

#include <Dot.h>

#include "RandomActionDotExports.h"

extern "C" std::unique_ptr<class battle::Dot> RANDOMDOT_API __stdcall createInitialDot() noexcept;

class RANDOMDOT_API RandomActionDot : public battle::Dot {
 public:
  std::string getName() const noexcept override;
  void wasEliminated(std::int32_t team, std::int32_t killer,
                     std::int32_t row, std::int32_t column) noexcept override;

  battle::RunAction run(std::int32_t team, std::array<std::int32_t, 8> view,
                        std::int32_t row, std::int32_t column) noexcept override;
};

#endif
