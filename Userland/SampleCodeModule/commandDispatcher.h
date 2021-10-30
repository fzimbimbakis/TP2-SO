#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

void commandSelector(char* buffer);
void pipeCommand(char* buffer, int idx, char isBackground);
#endif