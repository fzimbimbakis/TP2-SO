#ifndef COMMMANDS_H
#define COMMMANDS_H


int isCommand(char* command, char* buffer);
int isCommandWithArg(char* command, char* buffer);
void helpCommand();
void inforegCommand();
void printRegName(int index);
void cpuidCommand();

void getArguments(char* buffer, char* arg);
void exceptionTestCommando(char * buffer);
void printmemCommand(char* buffer);
void dateCommand();


#endif