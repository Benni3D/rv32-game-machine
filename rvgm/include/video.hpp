#ifndef FILE_VIDEO_HPP
#define FILE_VIDEO_HPP
#include <thread>
#include <atomic>
#include "device.hpp"

struct SDL_Window;
struct SDL_Surface;

namespace rvgm {
   class VideoAdapter : public Device {
   private:
      SDL_Window* window;
      SDL_Surface* surface;
      std::thread thr;
      std::atomic_int status;
      std::atomic_bool redraw;
   public:
      DataPort<true> width, height, closing{};

      VideoAdapter(unsigned width, unsigned height);
      
      void bus_write8(std::uint32_t, std::uint8_t) noexcept override;
      std::uint8_t bus_read8(std::uint32_t) noexcept override;

      ~VideoAdapter();

      void register_subdevs(std::uint32_t addr, Bus& bus) override;
   private:
      void loop(unsigned w, unsigned h);
   };
}

#endif /* FILE_VIDEO_HPP */
