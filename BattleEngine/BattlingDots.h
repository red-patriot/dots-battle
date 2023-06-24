#ifndef DOTS_BATTLE_BATTLE_ENGINE_BATTLING_DOTS_H
#define DOTS_BATTLE_BATTLE_ENGINE_BATTLING_DOTS_H

#include <Dot.h>

namespace battle::test {
  class Test1 : public Dot {
   public:
    std::string getName() const noexcept override;
    void wasEliminated(std::int32_t team, std::int32_t killer,
                       std::int32_t row, std::int32_t column) noexcept override;

    RunAction run(std::int32_t team, std::array<std::int32_t, 8> view,
                  std::int32_t row, std::int32_t column) noexcept override;
  };

  class Test2 : public Dot {
   public:
    std::string getName() const noexcept override;
    void wasEliminated(std::int32_t team, std::int32_t killer,
                       std::int32_t row, std::int32_t column) noexcept override;

    RunAction run(std::int32_t team, std::array<std::int32_t, 8> view,
                  std::int32_t row, std::int32_t column) noexcept override;
  };

  class Test3 : public Dot {
   public:
    std::string getName() const noexcept override;
    void wasEliminated(std::int32_t team, std::int32_t killer,
                       std::int32_t row, std::int32_t column) noexcept override;

    RunAction run(std::int32_t team, std::array<std::int32_t, 8> view,
                  std::int32_t row, std::int32_t column) noexcept override;
  };
}  // namespace battle::test

#endif
