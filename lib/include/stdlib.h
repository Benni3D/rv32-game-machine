#ifndef FILE_STDLIB_H
#define FILE_STDLIB_H
#include <stdnoreturn.h>
#include <stddef.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RAND_MAX 32767

noreturn void abort(void);
noreturn void exit(int exit_code);

int atexit(void(*)(void));

void srand(unsigned seed);
int rand(void);

char* itoa(int value, char* str, int base);
char* utoa(unsigned value, char* str, int base);

#endif /* FILE_STDLIB_H */
