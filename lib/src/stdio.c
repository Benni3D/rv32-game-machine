#include "stdio.h"

extern void debug(char);

int putchar(int ch) {
   debug(ch);
   return ch;
}
int puts(const char* s) {
   int i = 0;
   while (s[i]) putchar(s[i++]);
   putchar('\n');
   return i;
}

