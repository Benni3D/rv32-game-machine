#ifndef FILE_BUS_HPP
#define FILE_BUS_HPP
#include <vector>
#include <memory>
#include "device.hpp"
#include "debug.hpp"

namespace rvgm {
   class Bus {
   private:
      struct Connection {
         std::uint32_t addr;
         std::shared_ptr<Device> dev;
         Connection(std::uint32_t a, std::shared_ptr<Device>&& d) noexcept : addr(a), dev(std::move(d)) {}
         bool is_mapped(std::uint32_t a, std::uint32_t n = 1) const noexcept {
            return a >= addr && (a + n) <= (addr + dev->bus_size);
         }
         Device* operator->() noexcept { return dev.get(); }
      };
      std::vector<Connection> devs{};
   public:

      void add_device(std::uint32_t addr, std::shared_ptr<Device>&& dev);

      void write8(std::uint32_t addr, std::uint8_t data);
      void write16(std::uint32_t addr, std::uint16_t data);
      void write32(std::uint32_t addr, std::uint32_t data);

      std::uint8_t read8(std::uint32_t addr);
      std::uint16_t read16(std::uint32_t addr);
      std::uint32_t read32(std::uint32_t addr);
   
      bool load_file(const char* path);
   };
}

#endif /* FILE_BUS_HPP */
