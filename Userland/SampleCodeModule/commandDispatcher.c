#include "lib.h"
#include "commands.h"
#include "sysCall.h"



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
    else if(isCommandWithArg("loop",buffer)){
        loopCommand(buffer);
    }
    else if(isCommand("testprio",buffer)){
        testPrio();
    }
    else if(isCommand("ps",buffer)){
        psCommand();
    } else if(isCommandWithArg("testsem", buffer)){
        testsemCommand(buffer);
    }else if(isCommand("seminfo", buffer)){
        semInfoCommand(buffer);
    }else if(isCommand("filter", buffer)){
        filterCommand();
    }else if(isCommand("cat", buffer)){
        catCommand();
    } else if(isCommand("wc", buffer)){
        wcCommand();
    }
    else{
//        printer(buffer);
        printf(" Command not found");
        printf("\n");
    }

}

void process1(char * buffer,int fd, char * sem){
//    printf("P1\n");
    sysDupPipe(0, fd);
//    char * aux = alloc(sizeof(char)*9);
//    sysRead(0, aux, 8);
//    buffer[7]=0;
//    printf(aux);
    commandSelector(buffer);
    if(sem!=0) {
        post_sem(sem);
    }
    exit();
}

void process2(char * buffer,int fd, char* sem){
//    printf("P2\n");
//    printf("      %d       ", fd);
//    printf("A\n");
//    printf(buffer);
//    printf("B\n");
    sysDupPipe(1, fd);
    commandSelector(buffer);
    if(sem!=0) {
//        printf("POST\n");   // TODO: Pongo UN PRINTF y salta una EXCEPTION aiycvuaihbcoivyagoubdjfsd
        post_sem(sem);
    }
    exit();
}

static char semForPipesID = 1;
void pipeCommand(char* buffer, int idx, char isBackground){
    buffer[idx]=0;
    buffer[idx-1]='\n';
    int pipes[2];
//    printf("Antes de sysPipe\n");
    sysPipe(pipes);
//    printf("pipe 1:%d pipe 2: %d\n", pipes[0], pipes[1]);
//    printf("Despues de sysPipe\n");
//    if(!isBackground){
        char * semID = alloc(6* sizeof(char));
        semID[0] = 'p';
        semID[1] = 'i';
        semID[2] = 'p';
        semID[3] = 'e';
        semID[4] = semForPipesID++;
        semID[5] = 0;
        create_sem(semID,0);


        newPipedProcess(&process1, 0, buffer, pipes[0], semID);
        newPipedProcess(&process2, 0, buffer+idx+2, pipes[1], semID);

        wait_sem(semID);
        wait_sem(semID);
        close_sem(semID);
        free(semID);
        sysClosePipe(pipes[0]);
        sysClosePipe(pipes[1]);
//    } else{
//        newPipedProcess(&process1, 0, buffer, pipes[0], 0); // No se como. Cambia el stdin o out
//        newPipedProcess(&process2, 0, buffer+idx+2, pipes[1], 0); // No se como. Cambia el stdin o out
//    }

}