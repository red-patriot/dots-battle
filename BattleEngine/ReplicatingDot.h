#ifndef DOTS_BATTLE_BATTLE_ENGINE_REPLICATING_DOT_H
#define DOTS_BATTLE_BATTLE_ENGINE_REPLICATING_DOT_H

#include <Dot.h>

#include "BattleEngine.h"

namespace battle {
  /** A class which holds a dot trying to replicate until the process is complete. */
  class ReplicatingDot : public Dot {
   public:
    ReplicatingDot(std::unique_ptr<Dot> parent,
                   std::unique_ptr<Dot> child,
                   Direction replicateDir,
                   Engine* engine) :
        parent_(std::move(parent)),
        child_(std::move(child)),
        replicateDir_(replicateDir),
        engine_(engine) { }

    std::string getName() const noexcept override {
      return parent_->getName();
    }
    void wasEliminated(std::int32_t team, std::int32_t killer,
                       std::int32_t row, std::int32_t column) noexcept override {
      parent_->wasEliminated(team, killer, row, column);
    }

    battle::RunAction run(std::int32_t team, std::array<std::int32_t, 8> view,
                          std::int32_t row, std::int32_t column) noexcept override {
      --countdown_;
      if (countdown_ == 0) {
        engine_->doReplicate(std::move(parent_), Coordinate{row, column},
                             std::move(child_), replicateDir_);
      }
      return RunAction{.type = RunAction::WAIT};
    }

   private:
    int countdown_ = 100;  // Turns to replicate

    std::unique_ptr<Dot> parent_;
    std::unique_ptr<Dot> child_;
    Direction replicateDir_;
    Engine* engine_;
  };
}  // namespace battle

#endif
