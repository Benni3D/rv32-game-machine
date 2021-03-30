#include <cstdio>
#include "device.hpp"

namespace rvgm {
   void Device::bus_write16(std::uint32_t addr, std::uint16_t data) noexcept {
      bus_write8(addr + 0, data & 0xff);
      bus_write8(addr + 1, data >> 8);
   }
   void Device::bus_write32(std::uint32_t addr, std::uint32_t data) noexcept {
      bus_write16(addr + 0, data & 0xffff);
      bus_write16(addr + 2, data >> 16);
   }
   std::uint16_t Device::bus_read16(std::uint32_t addr) noexcept {
      return bus_read8(addr) | (static_cast<std::uint16_t>(bus_read8(addr + 1)) << 8);
   }
   std::uint32_t Device::bus_read32(std::uint32_t addr) noexcept {
      return bus_read16(addr) | (static_cast<std::uint32_t>(bus_read16(addr + 2)) << 16);
   }
   std::uint32_t RAM::load_file(const char* path) noexcept {
      std::FILE* file = std::fopen(path, "r");
      if (!file) return 0;
      const std::uint32_t n = std::fread(mem, 1, bus_size, file);
      std::fclose(file);
      return n;
   }

   SerialDevice::~SerialDevice() noexcept {
      if (in && in != stdin) std::fclose(in);
      if (out && out != stdout && out != in) std::fclose(out);
   }
   void SerialDevice::bus_write8(std::uint32_t, std::uint8_t data) noexcept {
      if (out) std::fwrite(&data, 1, 1, out);
   }
   std::uint8_t SerialDevice::bus_read8(std::uint32_t) noexcept {
      if (!in) return 0;
      std::uint8_t data;
      std::fread(&data, 1, 1, in);
      return data;
   }
}
