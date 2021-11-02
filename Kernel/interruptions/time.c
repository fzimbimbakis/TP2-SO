// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <time.h>
#include <stdint.h>
#include "interrupts.h"

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
    //ncPrintDec(ticks);
}


void sleep(int seconds) {
    unsigned long intialTicks = ticks;
    for (int i = 1; i <= seconds; i++) {
        while (ticks < (intialTicks + 18 * i))
            _hlt();

    }
}
