#ifndef COMMMANDS_H
#define COMMMANDS_H

void testMMCommand();
void memInfoCommand();
int isCommand(char* command, char* buffer);
int isCommandWithArg(char* command, char* buffer);
void helpCommand();
void inforegCommand();
void printRegName(int index);
//void cpuidCommand();
void blockCommand(char* buffer);
void unblockCommand(char* buffer);
void killCommand(char* buffer);
void testScheduler();
void testPrio();
void getpidCommand();
void niceCommand(char * buffer);
void psCommand();
void loopCommand(char *buffer);

void getArguments(char* buffer, char* arg);
void getStrings(char * buffer, char * str1, char* str2);
void exceptionTestCommando(char * buffer);
void printmemCommand(char* buffer);
void dateCommand();
void semInfoCommand();
#include "testSem.h"
void testsemCommand(char * buffer);
#endif