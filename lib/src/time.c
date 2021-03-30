#include "time.h"

static const volatile time_t* time_port = (const volatile time_t*)0x80000004;

time_t time(time_t* arg) {
   const time_t time = *time_port;
   if (arg) *arg = time;
   return time;
}

