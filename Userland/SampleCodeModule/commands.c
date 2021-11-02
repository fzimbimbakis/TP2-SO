#include "commands.h"
#include "lib.h"
#include "sysCall.h"
#include "exceptionTests.h"
#include "readMemory.h"

#include "testMM.h"
#include "testScheduler.h"

#define SECONDS 0
#define MINUTES 2
#define HOURS 4
#define DAY 7
#define MONTH 8
#define YEAR 9

int isCommand(char* command, char* buffer){ //1 si son iguales, 0 si no
	int comLen=strlen(command);
//command:date
//buffer: date
    if(comLen > strlen(buffer))
		return 0;

    int i;
	for(i=0; i < comLen ; i++){
		if(buffer[i]== '\n' || command[i]!=buffer[i]) 
			return 0;
	}
	if (buffer[i] == '\n' || buffer[i] == 0)
		return 1;
	return 0;
}

int isCommandWithArg(char* command, char* buffer){ //1 si son iguales, 0 si no
	int comLen=strlen(command);

	if(comLen > strlen(buffer))
		return 0;

	int i;
	for(i=0; i < comLen ; i++){
		if(buffer[i]== ' ' || command[i]!=buffer[i]) //si es el espacio, no coincide con el comando
			return 0;
	}
	if (buffer[i] == ' ') //si cuando termino de leer el comando y viene el espacio en buffer -> son iguales
		return 1;
	return 0;
}

void getArguments(char* buffer, char* arg){
    int buffLen=strlen(buffer);
    int i;
    for(i=0;buffer[i]!= ' ';i++);

    while(buffer[i]==' ')
        i++;
    int flag=1;

    for(int j=0; j < MAX_BUFFER && i < buffLen && flag; j++, i++ ){
        if(buffer[i]=='\n' || buffer[i]==0){
            arg[j]=0; //final del string
            flag=0;
        }else
            arg[j]=buffer[i]; //copio el string
    }
}

#define commandsQuantity 21
void helpCommand(){
    static char * strings[][2] = {
        {"help: display every command available\n","&> help\n"},
        {"inforeg: print every register with its value\n","&> inforeg\n"},
        {"printmem: 32bytes from the direction passed by argument\n","&> printmem [DIRECTION](hexa)\n"},
        {"date: show real time live\n","&> date\n"},
        {"testmm: Runs a Memory Manager test.\n", "&> testmm\n"},
        {"testsch: Runs a Scheduler test.\n", "&> testsch\n"},
        {"testprio: Runs a Scheduler priority test.\n", "&> testprio\n"},
        {"testsem: Runs a semaphore test.\nThis command can run with or without semaphores to appreciate the difference.\n0: Run test with semaphores.\n1: Run test without semaphores\n", "&> testsem [mode]\n"},
        {"mem: Shows (Units: Bytes):\n- Total memory\n- Taken memory\n- Free memory\n", "&> meminfo\n"},
        {"ps: Prints every running process information.\n", "&> ps\n"},
        {"block: Sets a process state to BLOCKED.\n", "&> block [Process PID]\n"},
        {"unblock: Sets a process state to READY.\n", "&> unblock [Process PID]\n"},
        {"kill: Stops a process.\n", "&> kill [Process PID]\n"},
        {"getpid: Prints actual process PID.\n", "&> getpid\n"},
        {"nice: Change a process priority.\n", "&> nice [Process PID] [New Priority]\n"},
        {"loop: Prints the process pid every N SECONDS.\n(Important: It can only be stopped with a kill.)\n", "&> loop [N SECONDS]\n"},
        {"filter: Receives standard input and only prints vocals.\nThe command stops when a ';' is received.\n", "&> filter\n"},
        {"cat: Receives standard input and prints it.\nThe command stops when a ';' is received.\n", "&> cat\n"},
        {"wc: Receives standar input, prints it and when it receives a ';' it prints the lines of the input.\n", "&> wc\n"},
        {"seminfo: Prints all semaphores information.\n", "&> seminfo\n"},
        {"pipeinfo: Prints all semaphores information.\n", "&> pipeinfo\n"},
        {"phylo: Dinning phylosofers problem.\n", "&> phylo\n"},
        {"Run command in background.\n", "&> .[command]"},
        {"Pipe two commands.\n", "&> [command 1] - [command 2]"}
    };
    static int i;
    for (i = 0; i < commandsQuantity; i++)
    {
        printf("@ ");
        printf(strings[i][0]);
        printf("Syntax: ");
        printf(strings[i][1]);
        if(i!=commandsQuantity-1){
        printf("Press ANY key to see next command.\n");
        getChar();
        putChar('\n');
        }
    }
    printf("End of help.\n");
}


static char * registersNames[] = {"RAX", "RBX", "RCX", "RDX", "RBP", "RSI", "RDI", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};
void printRegName(int index){
	putChar(' ');
	printf(registersNames[14-index]);
	printf(":");
    cleanBuffer();
}

void inforegCommand(){
    printRegs();    
    printf("\n");
}


void printmemCommand(char* buffer){
    char arg[MAX_BUFFER];
    
    getArguments(buffer,arg);
    long int direction=hexToDec(arg);
    if(direction>=0){
        for(int i=0;i<32;i+=4){
            //imprimo 8 bloques de 4bytes al ser una arquitectura de 64 bits
            unsigned int value=readDirection(direction+i);
            unsigned int low=value>>16;
            value=value<<16;
            value+=low;

            printf(numToStr(direction+i,16));

            printf(": %s   ",numToStr(value,16));
        }
        printf("\n");
    }else{
        printf("Direccion invalida");
        printf("\n");
    }   
    
   

}


void dateCommand(){
    printHex(sysTime(HOURS));
    printf(":");
    printHex(sysTime(MINUTES));
    printf(":");
    printHex(sysTime(SECONDS));
    printf(" of ");
    printHex(sysTime(DAY));
    printf("/");
    printHex(sysTime(MONTH));
    printf("/");
    printHex(sysTime(YEAR));
    printf("\n");
    putChar(';');

}

void exceptionTestCommando(char * buffer){
    char arg[MAX_BUFFER];
    
    getArguments(buffer,arg);
    int num=strToNum(arg);
    switch (num){
        case 0:
            divTest();
            break;
        case 6:
            opCodeTest();
            break;
        default:
            printf("Exception ID is not valid");
            printf("\n");
            break;
    }
    
}

void memInfoCommand(){      // TODO: Agregar debilidad al informe. Si le pasamos mem a un pipe como el que escribe en el pipe. Se rompe
    unsigned * array = memInfo();
    printf("Total memory: ");
    printDec(array[0]);
    printf(" Taken memory: ");
    printDec(array[1]);
    printf(" Free memory " );
    printDec(array[2]);
    putChar('\n');
    free(array);
}

void testMMCommand(){
            test_mm();
}

 void blockCommand(char* buffer){
        char arg[MAX_BUFFER];

        getArguments(buffer,arg);
        uint32_t pid=strToNum(arg);
        if(block(pid)==-1){
            printf("Invalid PID");
            printf("\n");
        }

}

void unblockCommand(char* buffer){
    char arg[MAX_BUFFER];

    getArguments(buffer,arg);
    uint32_t pid=strToNum(arg);
    if(unblock(pid)==-1){
        printf("Invalid PID");
        printf("\n");
    }

}

 void killCommand(char* buffer){
        char arg[MAX_BUFFER];

        getArguments(buffer,arg);
        uint32_t pid=strToNum(arg);
        if(kill(pid)==-1){
            printf("Invalid PID");
            printf("\n");
        }
}

void testScheduler(){
    test_processes();
}

void testPrio(){
    test_prio();
}

void getpidCommand(){
    uint32_t pid= getpid();
    printf("PID = %d\n", pid);
}

void getStrings(char * buffer, char * str1, char* str2){
    int buffLen=strlen(buffer);
    int i;
    for(i=0;buffer[i]!= ' ';i++){
        str1[i]=buffer[i];
    }
    str1[i]=0;

    while(buffer[i]==' ')
        i++;
    int flag=1;

    for(int j=0; j < MAX_BUFFER && i < buffLen && flag; j++, i++ ){
        if(buffer[i]=='\n'){
            str2[j]=0; //final del string
            flag=0;
        }else
            str2[j]=buffer[i]; //copio el string
    }
}

void niceCommand(char * buffer){
    char arg[MAX_BUFFER];

    getArguments(buffer,arg);

    char arg1[MAX_BUFFER];
    char arg2[MAX_BUFFER];

    getStrings(arg, arg1, arg2);
    uint32_t pid=strToNum(arg1);

    char priority = strToNum(arg2);

    if(nice(pid, priority)==-1){
        printf("Invalid PID");
            printf("\n");
    }

}

void psCommand(){
    ps();
}


void loopCommand(char *buffer){
    char arg[MAX_BUFFER];

    getArguments(buffer,arg);
    int seconds=strToNum(arg);

    while(1){
        sleep(seconds);
        printf("pid: %d\n", getpid());
    }
}

void testsemCommand(char * buffer){
    char arg[MAX_BUFFER];

    getArguments(buffer,arg);
    int num=strToNum(arg);
    switch (num){
        case 0:;
            test_sync();
            break;
        case 1:   ;
            test_no_sync();
            break;


        default:
            printf("Test ID is not valid");
            printf("\n");
            break;
    }
}

void filterCommand(){
    char c;

        while((c=getChar())!=';'){
                if(!(c=='a' || c=='e' || c=='i' || c=='o' || c=='u')){
                    putChar(c);
                }
        }
    putChar(';');
        printf("\nEnd filter.\n");
        return;

}
void catCommand(){
    char c;
    char* buffer = alloc(256 * sizeof(char ));
    int index=0;

        while((c=getChar())!=';'){
            buffer[index++]=c;
            putChar(c);
            if(c=='\n'){
//                buffer[index++]=c;
                buffer[index++]=0;
                index=0;
                printf(buffer);
            }
        }
    putChar(';');
        printf("\nEnd cat.\n");
        return;

}
void wcCommand(){
    char c;
    int lines=0;
    while((c=getChar())!=';'){
        putChar(c);
        if(c=='\n')
            lines++;
    }
    putChar(';');
    printf("\nwc: Input has %d lines.\n", lines);
    return;
}


void semInfoCommand(){
    int qty;
    sem_info_wrapper * info = sysSemInfo(&qty);
    for (int i = 0; i < qty; ++i) {
        printf("ID: %s Value: %d Process pids blocked: ", info[i].id, info[i].value);
        for (int j = 0; j < info[i].nPids; ++j) {
            printf("%d ", info[i].pids[j]);
        }
        putChar('\n');
        free(info[i].pids);
        free(info[i].id);
    }
    free(info);

}

void pipeInfoCommand(){
    int qty;
//    int * pipes = alloc(2* sizeof(int));
//    sysPipe(pipes);
    pipe_info_wrapper * info = sysInfoPipe(&qty);
    for (int i = 0; i < qty; ++i) {
        printf("ID: %d Process pids blocked: ", info[i].id);
        if(info[i].semR!=0){
            for (int j = 0; j < info[i].semR->nPids; ++j) {
                printf("%d ", info[i].semR->pids[j]);
            }
            free(info[i].semR->pids);
            free(info[i].semR->id);
            free(info[i].semR);
        }
        if(info[i].semW!=0) {
            for (int j = 0; j < info[i].semW->nPids; ++j) {
                printf("%d ", info[i].semW->pids[j]);
            }
            free(info[i].semW->pids);
            free(info[i].semW->id);
            free(info[i].semW);
        }
        putChar('\n');
    }
    free(info);
}

