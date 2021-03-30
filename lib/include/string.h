#ifndef FILE_STRING_H
#define FILE_STRING_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char*);
size_t strnlen(const char*, size_t);

void* memset(void*, int, size_t);
void* memcpy(void*, const void*, size_t);
void* memmove(void*, const void*, size_t);
void* memchr(const void*, int, size_t);
int memcmp(const void*, const void*, size_t);

char* strcpy(char*, const char*);
char* strcat(char*, const char*);
char* strncpy(char*, const char*, size_t);
char* strncat(char*, const char*, size_t);

int strcmp(const char*, const char*);
int strncmp(const char*, const char*, size_t);

char* strrvs(char*);
char* strnrvs(char*, size_t);

#ifdef __cplusplus
}
#endif

#endif /* FILE_STRING_H */
