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
    }else if(isCommandWithArg("testmm", buffer)){
        testMMCommand(buffer);
    } else if(isCommand("mem", buffer)){
        memInfoCommand();
    }else if(isCommandWithArg("block", buffer)){
        blockCommand(buffer);
    }
    else if(isCommandWithArg("unblock", buffer)){
        unblockCommand(buffer);
    }
    else if(isCommandWithArg("kill", buffer)){
        killCommand(buffer);
    }
    else if(isCommand("testsch",buffer)){
        testScheduler();
    }
    else if(isCommand("getpid",buffer)){
        getpidCommand();
    }
    else if(isCommandWithArg("nice",buffer)){
        niceCommand(buffer);
    }
    else if(isCommand("testprio",buffer)){
        testPrio();
    }
    else{
        printer("Command not found");
        printf("\n");
    }

}