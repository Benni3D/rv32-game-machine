#ifndef FILE_REGS_HPP
#define FILE_REGS_HPP
#include <cstdint>
#include <memory>
#include <array>


namespace rvgm::detail {
   class RegisterBase {
   public:
      virtual ~RegisterBase() = default;

      RegisterBase& operator=(std::uint32_t val) noexcept { return write(val), *this; }
      [[nodiscard]]
      operator std::uint32_t() const noexcept { return read(); }

      virtual void write(std::uint32_t) noexcept = 0;
      [[nodiscard]]
      virtual std::uint32_t read() const noexcept = 0;
   };
   class ZeroRegister : public RegisterBase {
   public:
      void write(std::uint32_t) noexcept final {}
      [[nodiscard]]
      std::uint32_t read() const noexcept final { return 0; }
   };
   class Register : public RegisterBase {
   private:
      std::uint32_t value{};
   public:
      void write(std::uint32_t v) noexcept final { value = v; }
      [[nodiscard]]
      std::uint32_t read() const noexcept final { return value; }
   };
}

namespace rvgm {
   class RegisterFile {
   private:
      std::array<std::unique_ptr<detail::RegisterBase>, 32> regs{};
   public:
      RegisterFile() {
         regs[0] = std::make_unique<detail::ZeroRegister>();
         for (std::uint8_t i = 1; i < regs.size(); ++i)
            regs[i] = std::make_unique<detail::Register>();
      }
      RegisterFile(const RegisterFile&) = delete;
      RegisterFile(RegisterFile&&) = delete;
      ~RegisterFile() = default;

      RegisterFile& operator=(const RegisterFile&) = delete;
      RegisterFile& operator=(RegisterFile&&) = delete;

      [[nodiscard]]
      auto& operator[](std::uint8_t i) { return *regs[i]; }
      [[nodiscard]]
      std::uint32_t operator[](std::uint8_t i) const { return regs[i]->read(); }

      void reset() noexcept {
         for (std::uint8_t i = 1; i < regs.size(); ++i)
            *regs[i] = 0;
      }
   };
}

#endif /* FILE_REGS_HPP */
