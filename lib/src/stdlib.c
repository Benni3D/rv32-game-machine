#include <stdint.h>
#include "string.h"
#include "stdlib.h"

/*
static uint32_t next;

void srand(unsigned seed) {
   next = seed;
}

int rand(void) {
   next = next * 1103515245 + 12345;
   return (unsigned)(next >> 16) & 15;
}
*/

static const volatile uint32_t* const random_port = (const volatile uint32_t*)0x80000008;
void srand(unsigned seed) { (void)seed; }
int rand(void) { return *random_port; }



static const char chars[] = "0123456789abcdefghijklmnopqrstuvwxyz";
char* itoa(int val, char* buf, int base) {
   if (base < 2 || base > 36) return NULL;
   if (!val) {
      buf[0] = '0';
      buf[1] = '\0';
      return buf;
   }
   size_t i = 0;
   int negative = 0;
   if (val < 0) negative = 1, val = -val;
   while (val) {
      buf[i++] = chars[val % base];
      val /= base;
   }
   if (negative) buf[i++] = '-';
   buf[i] = '\0';
   return strnrvs(buf, i);
}

char* utoa(unsigned val, char* buf, int base) {
   if (base < 2 || base > 36) return NULL;
   if (!val) {
      buf[0] = '0';
      buf[1] = '\0';
      return buf;
   }
   size_t i = 0;
   while (val) {
      buf[i++] = chars[val % base];
      val /= base;
   }
   buf[i] = '\0';
   return strnrvs(buf, i);
}

