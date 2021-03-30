#ifndef FILE_STDIO_H
#define FILE_STDIO_H
#include <stddef.h>
#include <stdarg.h>

int putchar(int);
int puts(const char*);

int vprintf(const char*, va_list);
int vsprintf(char*, const char*, va_list);
int vsnprintf(char*, size_t, const char*, va_list);

int printf(const char*, ...);
int sprintf(char*, const char*, ...);
int snprintf(char*, size_t, const char*, ...);

#endif /* FILE_STDIO_H */
