#include "lib.h"
#include "commands.h"


void commandSelector(char* buffer){
    if(isCommand("help", buffer)){
        helpCommand();
    }else if(isCommand("inforeg", buffer)){
        inforegCommand();
    }else if(isCommandWithArg("printmem", buffer)){
        printmemCommand(buffer);
    }else if(isCommand("date", buffer)){
        dateCommand();
    }else if(isCommandWithArg("exceptiontest",buffer)){
        exceptionTestCommando(buffer);
    }else if(isCommand("cpuid", buffer)){
        cpuidCommand();
    }
    else{
        printer("Command not found");
        printf("\n");
    }

}