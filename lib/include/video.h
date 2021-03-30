#ifndef FILE_VIDEO_H
#define FILE_VIDEO_H
#include <stdint.h>

#define vidmem ((volatile uint32_t*)0x20000000)
#define vidcfg ((const volatile uint32_t*)0x80001000)

inline static uint32_t video_width(void) { return vidcfg[0]; }
inline static uint32_t video_height(void) { return vidcfg[1]; }
inline static uint32_t video_close_requested(void) { return vidcfg[2]; }

inline static uint32_t argb(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
   return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
inline static void draw(int x, int y, uint32_t argb) {
   vidmem[y * video_width() + x] = argb;
}

void fill_rect(int x, int y, int w, int h, uint32_t argb);

#endif /* FILE_VIDEO_H */
