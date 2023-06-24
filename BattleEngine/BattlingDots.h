#ifndef DOTS_BATTLE_BATTLE_ENGINE_BATTLING_DOTS_H
#define DOTS_BATTLE_BATTLE_ENGINE_BATTLING_DOTS_H

#include <Dot.h>

namespace battle::test {
  class Test1 : public Dot {
   public:
    std::string getName() const noexcept override;
    void wasEliminated(std::uint32_t team, std::uint32_t killer,
                       std::uint32_t row, std::uint32_t column) noexcept override;

    ::battle::RunAction run(std::uint32_t team, std::array<std::uint32_t, 8> view,
                            std::uint32_t row, std::uint32_t column) noexcept override;
  };

  class Test2 : public Dot {
   public:
    std::string getName() const noexcept override;
    void wasEliminated(std::uint32_t team, std::uint32_t killer,
                       std::uint32_t row, std::uint32_t column) noexcept override;

    ::battle::RunAction run(std::uint32_t team, std::array<std::uint32_t, 8> view,
                            std::uint32_t row, std::uint32_t column) noexcept override;
  };

  class Test3 : public Dot {
   public:
    std::string getName() const noexcept override;
    void wasEliminated(std::uint32_t team, std::uint32_t killer,
                       std::uint32_t row, std::uint32_t column) noexcept override;

    ::battle::RunAction run(std::uint32_t team, std::array<std::uint32_t, 8> view,
                            std::uint32_t row, std::uint32_t column) noexcept override;
  };
}  // namespace battle::test

#endif
