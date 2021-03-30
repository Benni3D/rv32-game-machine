#ifndef FILE_DEVICE_HPP
#define FILE_DEVICE_HPP
#include <cstdint>
#include <cstdlib>
#include <utility>
#include <cstdio>
#include <ctime>

namespace rvgm {
   class Bus;
   class Device {
   public:
      const std::uint32_t bus_size;

      constexpr Device(std::uint32_t sz) noexcept : bus_size(sz) {}
      Device(const Device&) = delete;
      constexpr Device(Device&& dv) noexcept : bus_size(dv.bus_size) {}
      virtual ~Device() = default;

      Device& operator=(const Device&) = delete;
      Device& operator=(Device&&) = delete;

      virtual void bus_write8(std::uint32_t addr, std::uint8_t data) noexcept = 0;
      virtual void bus_write16(std::uint32_t addr, std::uint16_t data) noexcept;
      virtual void bus_write32(std::uint32_t addr, std::uint32_t data) noexcept;

      virtual std::uint8_t bus_read8(std::uint32_t addr) noexcept = 0;
      virtual std::uint16_t bus_read16(std::uint32_t addr) noexcept;
      virtual std::uint32_t bus_read32(std::uint32_t addr) noexcept;
   
      virtual void register_subdevs(std::uint32_t, Bus&) {}
   };

   class RAM : public Device {
   protected:
      std::uint8_t* mem;
   public:
      RAM(std::uint32_t sz) : Device(sz), mem(new std::uint8_t[sz]) {}
      RAM(RAM&& ram) noexcept : Device(std::move(ram)), mem(ram.mem) { ram.mem = nullptr; }
      ~RAM() noexcept { delete[] mem; }
      
      void bus_write8(std::uint32_t addr, std::uint8_t data) noexcept override { mem[addr] = data; }
      std::uint8_t bus_read8(std::uint32_t addr) noexcept override { return mem[addr]; }

      std::uint32_t load_file(const char* path) noexcept;
   };

   class ROM : public RAM {
   public:
      ROM(const char* path, std::uint32_t sz) : RAM(sz) { load_file(path); }

      void bus_write8(std::uint32_t, std::uint8_t) noexcept override {}
   };

   class SerialDevice : public Device {
   private:
      FILE* in;
      FILE* out;
   public:
      SerialDevice(FILE* file) noexcept : Device(1), in(file), out(file) {}
      SerialDevice(FILE* in, FILE* out) noexcept : Device(1), in(in), out(out) {}
      ~SerialDevice() noexcept;

      void bus_write8(std::uint32_t, std::uint8_t) noexcept override;
      std::uint8_t bus_read8(std::uint32_t) noexcept override;
   };


   template<bool rdonly = false>
   class DataPort : public Device {
   public:
      std::uint32_t value;

      DataPort(std::uint32_t v = 0) noexcept : Device(4), value(v) {}

      void bus_write8(std::uint32_t a, std::uint8_t data) noexcept override {
         if constexpr (!rdonly) {
            value &= ~(0xff << (a * 8));
            value |= (uint32_t)data << (a * 8);
         }
      }
      void bus_write16(std::uint32_t a, std::uint16_t data) noexcept override {
         if constexpr (!rdonly) {
            value &= ~(0xffff << (a * 8));
            value |= (uint32_t)data << (a * 8);
         }
      }
      void bus_write32(std::uint32_t a, std::uint32_t data) noexcept override {
         if constexpr (!rdonly) {
            value &= ~(0xffff'ffff << (a * 8));
            value |= (uint32_t)data << (a * 8);
         }
      }

      std::uint8_t bus_read8(std::uint32_t a) noexcept override { return (value >> (a * 8)) & 0xff; }
      std::uint16_t bus_read16(std::uint32_t a) noexcept override { return (value >> (a * 8)) & 0xffff; }
      std::uint32_t bus_read32(std::uint32_t a) noexcept override { return value >> (a*8); }
   };

   class DynamicPort : public Device {
   public:
      DynamicPort() noexcept : Device(4) {}

      // Implement write
      void bus_write8(std::uint32_t, std::uint8_t) noexcept override {}
      void bus_write16(std::uint32_t, std::uint16_t) noexcept override {}
      void bus_write32(std::uint32_t, std::uint32_t) noexcept override {}

      
      std::uint8_t bus_read8(std::uint32_t a) noexcept override { return (get() >> (a * 8)) & 0xff; }
      std::uint16_t bus_read16(std::uint32_t a) noexcept override { return (get() >> (a * 8)) & 0xffff; }
      std::uint32_t bus_read32(std::uint32_t a) noexcept override { return get() >> (a*8); }

      virtual std::uint32_t get() const noexcept = 0;
   };

   class TimePort : public DynamicPort {
   public:
      std::uint32_t get() const noexcept override { return static_cast<std::uint32_t>(std::time(NULL)); }
   };
   class RandomPort : public DynamicPort {
   public:
      // Implement seeding
      std::uint32_t get() const noexcept override { return static_cast<std::uint32_t>(rand()); }
   };

}

#endif /* FILE_DEVICE_HPP */
