#include <SDL2/SDL.h>
#include "video.hpp"
#include "bus.hpp"

namespace rvgm {
   template<class T>
   static std::shared_ptr<T> no_delete_ptr(T* ptr) {
      struct deleter {
         void operator()(T*) const noexcept {}
      };
      return std::shared_ptr<T>(ptr, deleter{});
   }
   VideoAdapter::VideoAdapter(unsigned width, unsigned height) : Device(width*height*4), width(width), height(height) {
      thr = std::thread(&VideoAdapter::loop, this, width, height);
      while (status == 0);
      if (status == -1) {
         // TODO: error handling
      }
   }

   VideoAdapter::~VideoAdapter() {
      status = 0;
      thr.join();
   }

   void VideoAdapter::bus_write8(std::uint32_t addr, std::uint8_t data) noexcept {
      SDL_LockSurface(surface);
      //printf("writing(%08x);\n", addr);
      std::uint8_t* pixels = static_cast<std::uint8_t*>(surface->pixels);
      pixels[addr] = data;
      redraw = 1;
      SDL_UnlockSurface(surface);
   }
   std::uint8_t VideoAdapter::bus_read8(std::uint32_t addr) noexcept {
      SDL_LockSurface(surface);
      const std::uint8_t px = (static_cast<std::uint8_t*>(surface->pixels))[addr];
      SDL_UnlockSurface(surface);
      return px;
   }


   void VideoAdapter::register_subdevs(std::uint32_t, Bus& bus) {
      bus.add_device(0x8000'1000, no_delete_ptr(&width));
      bus.add_device(0x8000'1004, no_delete_ptr(&height));
      bus.add_device(0x8000'1008, no_delete_ptr(&closing));
   }

   void VideoAdapter::loop(unsigned w, unsigned h) {
      auto print_error = []{
         std::fprintf(stderr, "Failed to initialize video: %s\n", SDL_GetError());
      };
      if (SDL_Init(SDL_INIT_VIDEO) != 0) { print_error(); status = -1; return; }
      surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
      if (!surface) { print_error(); status = -1; return; }
      window = SDL_CreateWindow(
            "RISC-V Game Machine",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            w, h,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
      );
      if (!window) { print_error(); status = -1; return; }
   
      status = 1;

      while (status == 1) {
         // handle events
         SDL_Event e;
         SDL_PumpEvents();
         if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) closing.value = 1;
         }

         // copy screen buffer
         if (redraw) {
            SDL_BlitScaled(surface, NULL, SDL_GetWindowSurface(window), NULL);
            SDL_UpdateWindowSurface(window);
            redraw = 0;
         }
      }
      SDL_FreeSurface(surface);
      SDL_DestroyWindow(window);
      SDL_Quit();
   }
}
