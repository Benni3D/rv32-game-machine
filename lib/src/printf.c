#include "stdlib.h"
#include "stdio.h"

typedef void(*printer_t)(void*, char, int);

static int printf_impl(void*, printer_t, int, const char*, va_list);

static void printch(void* data, char ch, int i) {
   (void)data;
   (void)i;
   putchar(ch);
}
static void print_str(void* data, char ch, int i) {
   ((char*)data)[i] = ch;
}

int vprintf(const char* fmt, va_list ap) {
   return printf_impl(NULL, printch, -1, fmt, ap);
}
int vsprintf(char* buf, const char* fmt, va_list ap) {
   return printf_impl(buf, print_str, -1, fmt, ap);
}
int vsnprintf(char* buf, size_t num, const char* fmt, va_list ap) {
   return printf_impl(buf, print_str, num, fmt, ap);
}

int printf(const char* fmt, ...) {
   va_list ap;
   va_start(ap, fmt);

   const int n = printf_impl(NULL, printch, -1, fmt, ap);
   
   va_end(ap);
   return n;
}
int sprintf(char* buf, const char* fmt, ...) {
   va_list ap;
   va_start(ap, fmt);

   const int n = printf_impl(buf, print_str, -1, fmt, ap);

   va_end(ap);
   return n;
}
int snprintf(char* buf, size_t num, const char* fmt, ...) {
   va_list ap;
   va_start(ap, fmt);

   const int n = printf_impl(buf, print_str, num, fmt, ap);

   va_end(ap);
   return n;
}

static int prints(printer_t p, void* data, int idx, const char* s) {
   int i;
   for (i = 0; s[i]; ++i) p(data, s[i], i + idx);
   return i;
}

static int printf_impl(void* data, printer_t printer, int num, const char* fmt, va_list ap) {
   char buf[32];
   int written = 0;

   for (size_t i = 0; fmt[i]; ++i) {
      char ch = fmt[i];
      if (ch != '%') { printer(data, ch, written++); continue; }
      ch = fmt[i];
      switch (ch) {
      case 'd':
      case 'i':
         itoa(va_arg(ap, int), buf, 10);
         written += prints(printer, data, written, buf);
         break;
      case 'x':
         utoa(va_arg(ap, unsigned), buf, 16);
         written += prints(printer, data, written, buf);
         break;
      case 's': written += prints(printer, data, written, va_arg(ap, const char*)); break;
      case 'c': printer(data, va_arg(ap, int), written++); break;
      

      case '%':  printer(data, '%', written++); break;
      case '\0': return written;
      default:
         printer(data, '%', written++);
         printer(data, ch,  written++);
         break;
      }
   }

   return written;
}
