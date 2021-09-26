
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#define RED 4

#include <lib.h>
#include <naiveConsole.h>

static void printRegisters();
static void zero_division();
static void invalid_opcode();

void exceptionDispatcher(int exception, int ExcDir) {
	ncNewline();
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if(exception == INVALID_OPCODE_ID)
		invalid_opcode();
	
	ncPrintColor("Exception at: ", RED);
	ncPrintHex(ExcDir);
	ncNewline();
}

static void zero_division() {
	ncPrintColor("Error: Division by zero Exception.", RED);
	ncNewline();
}

static void invalid_opcode(){
	ncPrintColor("Error: Invalid Opcode Exception.", RED);
	ncNewline();
}

static char * registersNames[] = {"RAX", "RBX", "RCX", "RDX", "RBP", "RSI", "RDI", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};
void printRegName(int index){
	ncPrintChar(' ');
	ncPrintColor(registersNames[14-index], RED);
	ncPrint(":");
}

void printEOE(){
	ncNewline();
	ncPrint("Both shell will reset in 10");
}