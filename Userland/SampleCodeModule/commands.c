#include "commands.h"
#include "lib.h"
#include "sysCall.h"
#include "exceptionTests.h"
#include "readMemory.h"
#include "cpu.h"

#define SECONDS 0
#define MINUTES 2
#define HOURS 4
#define DAY 7
#define MONTH 8
#define YEAR 9

int isCommand(char* command, char* buffer){ //1 si son iguales, 0 si no
	int comLen=strlen(command);

    if(comLen > strlen(buffer))
		return 0;

    int i;
	for(i=0; i < comLen ; i++){
		if(buffer[i]== '\n' || command[i]!=buffer[i]) 
			return 0;
	}
	if (buffer[i] == '\n')
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
        if(buffer[i]=='\n'){
            arg[j]=0; //final del string
            flag=0;
        }else
            arg[j]=buffer[i]; //copio el string
    }
}

#define commandsQuantity 5
void helpCommand(){
    static char * strings[][2] = {
        {"help: display every command available\n","&> help\n"},
        {"inforeg: print every register with its value\n","&> inforeg\n"},
        {"printmem: 32bytes from the direction passed by argument\n","&> printmem [DIRECTION](hexa)\n"},
        {"date: show real time live\n","&> date\n"},
        {"exceptiontest: Test exception routines. 0: Division by 0. 6: Invalid operation code.\n","&> exceptiontest [Exception ID]\n"}
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
	printer(registersNames[14-index]);
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

            printer(numToStr(direction+i,16));

            printf(": %s   ",numToStr(value,16));
        }
        printf("\n");
    }else{
        printer("Direccion invalida");
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
            printer("Exception ID is not valid");
            printf("\n");
            break;
    }
    
}

void cpuidCommand(){
    cpuFun();
}
