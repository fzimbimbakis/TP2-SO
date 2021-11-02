// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "lib.h"
#include "commands.h"
#include "sysCall.h"
#include "phylo.h"


void commandSelector(char *buffer) {
    if (isCommand("help", buffer)) {
        helpCommand();
    } else if (isCommand("inforeg", buffer)) {
        inforegCommand();
    } else if (isCommandWithArg("printmem", buffer)) {
        printmemCommand(buffer);
    } else if (isCommand("date", buffer)) {
        dateCommand();
    } else if (isCommand("testmm", buffer)) {
        testMMCommand();
    } else if (isCommand("mem", buffer)) {
        memInfoCommand();
    } else if (isCommandWithArg("block", buffer)) {
        blockCommand(buffer);
    } else if (isCommandWithArg("unblock", buffer)) {
        unblockCommand(buffer);
    } else if (isCommandWithArg("kill", buffer)) {
        killCommand(buffer);
    } else if (isCommand("testsch", buffer)) {
        testScheduler();
    } else if (isCommand("getpid", buffer)) {
        getpidCommand();
    } else if (isCommandWithArg("nice", buffer)) {
        niceCommand(buffer);
    } else if (isCommandWithArg("loop", buffer)) {
        loopCommand(buffer);
    } else if (isCommand("testprio", buffer)) {
        testPrio();
    } else if (isCommand("ps", buffer)) {
        psCommand();
    } else if (isCommandWithArg("testsem", buffer)) {
        testsemCommand(buffer);
    } else if (isCommand("seminfo", buffer)) {
        semInfoCommand(buffer);
    } else if (isCommand("filter", buffer)) {
        filterCommand();
    } else if (isCommand("cat", buffer)) {
        catCommand();
    } else if (isCommand("wc", buffer)) {
        wcCommand();
    } else if (isCommand("pipeinfo", buffer)) {
        pipeInfoCommand();
    } else if (isCommand("phylo", buffer)) {
        phyloCommand();
    } else {

        printf(" Command not found");
        printf("\n");
    }

}

void process1(char *buffer, int fd, char *sem) {

    sysDupPipe(0, fd);
    commandSelector(buffer);
    if (sem != 0) {
        post_sem(sem);
    }
    exit();
}

void process2(char *buffer, int fd, char *sem) {
    sysDupPipe(1, fd);
    commandSelector(buffer);
    if (sem != 0) {
        post_sem(sem);
    }
    exit();
}

static char semForPipesID = 1;

void pipeCommand(char *buffer, int idx) {
    buffer[idx] = 0;
    buffer[idx - 1] = 0;
    int pipes[2];

    sysPipe(pipes);

    char *semID = alloc(6 * sizeof(char));
    char *pipe = "pipe";
    for (int i = 0; i < 4; ++i) {
        semID[i] = pipe[i];
    }
    semID[4] = semForPipesID++;
    semID[5] = 0;
    create_sem(semID, 0);


    newPipedProcess((uint64_t) & process1, 0, buffer + idx + 2, pipes[0], semID);
    newPipedProcess((uint64_t) & process2, 0, buffer, pipes[1], semID);

    wait_sem(semID);
    wait_sem(semID);
    close_sem(semID);
    free(semID);
    sysClosePipe(pipes[0]);
    sysClosePipe(pipes[1]);
}

void bgPipedProcess(char *buffer, int pipeIdx) {
    pipeCommand(buffer, pipeIdx);
    free(buffer);
    exit();
}

void bgCommand(char *buffer) {
    commandSelector(buffer);
    free(buffer);
    exit();
}

void backgroundCommand(char *buffer, int pipeIdx, int isPiped) {
    int length = strlen(buffer);
    char *myBuffer = alloc(length + 1);
    for (int i = 0; i < length; ++i) {
        myBuffer[i] = buffer[i];
    }
    myBuffer[length + 1] = 0;
    if (isPiped) {
        newPipedProcess((uint64_t) & bgPipedProcess, 1, myBuffer, pipeIdx, 0);
    } else {
        newBufferProcess((uint64_t) & bgCommand, 1, myBuffer);
    }

}
