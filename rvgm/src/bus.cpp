#include <cstdio>
#include "bus.hpp"

namespace rvgm {
   void Bus::add_device(std::uint32_t addr, std::shared_ptr<Device>&& dev) {
      dev->register_subdevs(addr, *this);
      devs.emplace_back(addr, std::move(dev));
   }
   void Bus::write8(std::uint32_t addr, std::uint8_t data) {
#if DEBUG
      std::fprintf(stderr, "write8(0x%08x, 0x%02x);\n", addr, data);
#endif

      for (auto& dev : devs) {
         if (dev.is_mapped(addr)) {
            dev->bus_write8(addr - dev.addr, data);
            break;
         }
      }
   }

   void Bus::write16(std::uint32_t addr, std::uint16_t data) {
#if DEBUG
      std::fprintf(stderr, "write16(0x%08x, 0x%04x);\n", addr, data);
#endif
      for (auto& dev : devs) {
         const std::uint32_t a = addr - dev.addr;
         if (dev.is_mapped(addr, 2)) {
            dev->bus_write16(a, data);
            break;
         }
         else if (dev.is_mapped(addr)) {
            dev->bus_write8(a, data & 0xff);
            write8(addr + 1, data >> 8);
            break;
         }
      }
   }

   void Bus::write32(std::uint32_t addr, std::uint32_t data) {
#if DEBUG
      std::fprintf(stderr, "write32(0x%08x, 0x%08x);\n", addr, data);
#endif
      for (auto& dev : devs) {
         const std::uint32_t a = addr - dev.addr;
         if (dev.is_mapped(addr, 4)) {
            dev->bus_write32(a, data);
            break;
         }
         else if (dev.is_mapped(addr, 3)) {
            dev->bus_write16(a, data & 0xffff);
            dev->bus_write8(a + 2, (data >> 16) & 0xff);
            write8(addr + 3, data >> 24);
            break;
         }
         else if (dev.is_mapped(addr, 2)) {
            dev->bus_write16(a, data & 0xffff);
            write16(addr + 2, data >> 16);
            break;
         }
         else if (dev.is_mapped(addr)) {
            dev->bus_write8(a, data & 0xff);
            write8(addr + 1, (data >> 8) & 0xff);
            write16(addr + 2, data >> 16);
            break;
         }
      }
   }

   std::uint8_t Bus::read8(std::uint32_t addr) {
      std::uint8_t data = 0;
      for (auto& dev : devs) {
         if (dev.is_mapped(addr)) { data = dev->bus_read8(addr - dev.addr); break; }
      }
#if DEBUG
      std::fprintf(stderr, "read8(0x%08x) -> 0x%02x\n", addr, data);
#endif
      return data;
   }

   std::uint16_t Bus::read16(std::uint32_t addr) {
      std::uint16_t data = 0;
      for (auto& dev : devs) {
         const std::uint32_t a = addr - dev.addr;
         if (dev.is_mapped(addr, 2)) { data = dev->bus_read16(a); break; }
         else if (dev.is_mapped(addr)) { data = dev->bus_read8(a) | (static_cast<std::uint16_t>(read8(addr + 1)) << 8); break; }
      }
#if DEBUG
      std::fprintf(stderr, "read16(0x%08x) -> 0x%04x\n", addr, data);
#endif
      return data;
   }

   std::uint32_t Bus::read32(std::uint32_t addr) {
      std::uint32_t data = 0;
      for (auto& dev : devs) {
         const std::uint32_t a = addr - dev.addr;
         if (dev.is_mapped(addr, 4)) { data = dev->bus_read32(a); break; }
         else if (dev.is_mapped(addr, 3)) {
            data |= dev->bus_read16(a);
            data |= static_cast<std::uint32_t>(dev->bus_read8(a + 2)) << 16;
            data |= static_cast<std::uint32_t>(read8(addr + 3)) << 24;
            break;
         }
         else if (dev.is_mapped(addr, 2)) { data = dev->bus_read16(a) | (static_cast<std::uint32_t>(read16(addr + 2)) << 16); break; }
         else if (dev.is_mapped(addr)) {
            data |= dev->bus_read8(a);
            data |= static_cast<std::uint16_t>(read8(addr + 1)) << 8;
            data |= static_cast<std::uint32_t>(read16(addr + 2)) << 16;
            break;
         }
      }
#if DEBUG
      std::fprintf(stderr, "read32(0x%08x) -> 0x%08x\n", addr, data);
#endif
      return data;
   }


   bool Bus::load_file(const char* path) {
      std::FILE* file = std::fopen(path, "rb");
      if (!file) return false;
      std::fseek(file, 0, SEEK_END);
      const long sz = std::ftell(file);
      std::fseek(file, 0, SEEK_SET);

      uint8_t* buffer = new uint8_t[sz];
      std::fread(buffer, 1, sz, file);
      std::fclose(file);

      std::uint32_t i = 0;
      while (i < sz) {
         write8(i, buffer[i]);
         ++i;
      }
      delete[] buffer;
      return true;
   }

}
