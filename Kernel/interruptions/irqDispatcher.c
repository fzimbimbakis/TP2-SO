// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include "contextHandler.h"

static void int_20();
static void int_21();
typedef void (*EntryPoint)();
extern void timer_handler();
EntryPoint functionPtr[] = {&int_20, &int_21};

void irqDispatcher(uint64_t irq) {
	functionPtr[irq]();
	return;
}

void int_20() {
	handler();
}

void int_21(){
	keyboard_handler();
}
