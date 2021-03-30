#ifndef FILE_CPU_HPP
#define FILE_CPU_HPP
#include "instr.hpp"
#include "regs.hpp"
#include "bus.hpp"

namespace rvgm {
   class Processor {
   public:
      std::shared_ptr<Bus> bus{};
      RegisterFile regs{};
      std::uint32_t pc{};
   public:
      Processor(std::shared_ptr<Bus> bus) noexcept : bus(std::move(bus)) {}
      void reset() noexcept;
      bool clk() noexcept;
   private:
      bool exec(Instruction instr);
   };
}

#endif /* FILE_CPU_HPP */
