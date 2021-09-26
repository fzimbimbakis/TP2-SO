#include <time.h>
#include <naiveConsole.h>
static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void waiting(int seconds){
	unsigned long intialTicks = ticks;
	for (int i = 1; i <= seconds; i++)
	{
		while(ticks!=(intialTicks+18*i));
		ncPrintChar(' ');
		ncPrintDec(seconds-i);
	}
	
	return;
}
