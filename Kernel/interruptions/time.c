#include <time.h>
#include <stdint.h>
#include "interrupts.h"
static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
    //ncPrintDec(ticks);
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void sleep(int seconds){
	unsigned long intialTicks = ticks;
	for (int i = 1; i <= seconds; i++)
	{
		while(ticks<(intialTicks+18*i))
            _hlt();

	}
}
