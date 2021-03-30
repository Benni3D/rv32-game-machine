#include <stddef.h>
#include <stdint.h>
#include "stdlib.h"
#include "stdio.h"
#include "video.h"

void debug(char);

int main(void) {
   char buf[16];
   puts("Hello World");

   int w = vidcfg[0], h = vidcfg[1];

   while (!vidcfg[2]) {
      const uint32_t c = argb(rand(), rand(), rand(), 0xff);
      for (int y = 0; y < h; ++y) {
         for (int x = 0; x < w; ++x) {
            draw(x, y, c);
         }
      }
      putchar('.');
      putchar('\n');
   }

   puts("end");
   return 0;

}
