
#ifndef _CONTEXTHANDLER_H
#define _CONTEXTHANDLER_H

#include <stdint.h>
#define READY 0
#define BLOCKED 1
#define DEAD 2
int64_t handler(int64_t newRIP);
#endif