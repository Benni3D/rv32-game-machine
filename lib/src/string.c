#include <stdint.h>
#include "string.h"


size_t strlen(const char* s) {
   size_t i = 0;
   while (s[i]) ++i;
   return i;
}
size_t strnlen(const char* s, size_t n) {
   size_t i = 0;
   while (i < n && s[i]) ++i;
   return i;
}

void* memset(void* ptr, int ch, size_t num) {
   uint8_t* s = (uint8_t*)ptr;
   for (size_t i = 0; i < num; ++i)
      s[i] = (uint8_t)ch;
   return ptr;
}
void* memcpy(void* dest, const void* src, size_t num) {
   for (size_t i = 0; i < num; ++i) {
      ((uint8_t*)dest)[i] = ((const uint8_t*)src)[i];
   }
   return dest;
}
void* memmove(void* dest, const void* src, size_t num) {
   uint8_t* d = (uint8_t*)dest;
   const uint8_t* s = (const uint8_t*)src;
   if (d == s) return dest;
   else if (d < s) return memcpy(dest, src, num);
   for (size_t i = num; i != 0; --i) {
      ((uint8_t*)dest)[i - 1] = ((const uint8_t*)src)[i - 1];
   }
   return dest;
}
int memcmp(const void* p1, const void* p2, size_t num) {
   const uint8_t* s1 = (const uint8_t*)p1;
   const uint8_t* s2 = (const uint8_t*)p2;
   for (size_t i = 0; i < num; ++i) {
      const uint8_t diff = s1[i] - s2[i];
      if (diff) return diff;
   }
   return 0;
}
void* memchr(const void* ptr, int ch, size_t num) {
   uint8_t* s = (uint8_t*)ptr;
   for (size_t i = 0; i < num; ++i) {
      if (s[i] == (uint8_t)ch) return s;
   }
   return NULL;
}

char* strcpy(char* dest, const char* src) {
   size_t i = 0;
   do {
      dest[i] = src[i];
   } while (src[i++]);
   return dest;
}

char* strncpy(char* dest, const char* src, size_t num) {
   for (size_t i = 0; i < num; ++i) {
      dest[i] = src[i];
      if (!src[i]) break;
   }
   return dest;
}

char* strcat(char* dest, const char* src) {
   return strcpy(dest + strlen(dest), src);
}
char* strncat(char* dest, const char* src, size_t num) {
   return strncpy(dest + strlen(dest), src, num);
}


int strcmp(const char* s1, const char* s2) {
   for (size_t i = 0; ; ++i) {
      const char diff = s1[i] - s2[i];
      if (diff) return diff;
      else if (!s1[i]) break;
   }
   return 0;
}
int strncmp(const char* s1, const char* s2, size_t num) {
   for (size_t i = 0; i < num; ++i) {
      const char diff = s1[i] - s2[i];
      if (diff) return diff;
      else if (!s1[i]) break;
   }
   return 0;
}

char* strrvs(char* str) {
   return strnrvs(str, strlen(str));
}
char* strnrvs(char* str, size_t num) {
   size_t begin = 0, end = num - 1;
   char tmp;
   while (begin < end) {
      tmp = str[begin];
      str[begin] = str[end];
      str[end] = tmp;
      ++begin;
      --end;
   }
   return str;
}








