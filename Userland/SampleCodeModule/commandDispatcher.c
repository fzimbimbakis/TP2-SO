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
    } else if(isCommandWithArg("testsem", buffer)){
        testsemCommand(buffer);
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
    commandSelector(buffer);
    if(sem!=0)
    post_sem(sem);
    exit();
}

void process2(char * buffer,int fd, char* sem){
//    printf("P2\n");
    sysDupPipe(1, fd);
    commandSelector(buffer);
    if(sem!=0)
    post_sem(sem);
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
    if(!isBackground){
        char * semID = alloc(2* sizeof(char));
        semID[0] = semForPipesID++;
        semID[1] = 0;
        create_sem(semID,0);
//        printf("antes de newPipedProcess\n");
//        printf("AAAA pipe 1:%d pipe 2: %d\n", pipes[0], pipes[1]);
        newPipedProcess(&process1, 0, buffer, pipes[0], semID);
        newPipedProcess(&process2, 0, buffer+idx+2, pipes[1], semID);
//        printf("despues de newPipedProcess\n");
        wait_sem(semID);
        wait_sem(semID);
        free(semID);
    } else{
        newPipedProcess(&process1, 0, buffer, pipes[0], 0); // No se como. Cambia el stdin o out
        newPipedProcess(&process2, 0, buffer+idx+1, pipes[1], 0); // No se como. Cambia el stdin o out
    }

}