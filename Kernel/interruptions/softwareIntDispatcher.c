#include <naiveConsole.h>
#include <interrupts.h>
#include <keyboard.h>
#define RED 4
typedef int (*EntryPoint)(unsigned int, unsigned int, unsigned int);

void write(unsigned int fd, const char * buffer, unsigned int count);
void read(unsigned int fd, char * buffer, unsigned int count);
int accessClock(unsigned int mode);
EntryPoint functionPtrs[] = {&write, &read, &accessClock};

int int_80(unsigned int arg1, unsigned int arg2, unsigned int arg3, int sysCall){
    functionPtrs[sysCall](arg1, arg2, arg3);
    return;
}

void write(unsigned int fd, const char * buffer, unsigned int count){       
    if(fd==1){      // STDOUT
        for (int i = 0; i < count && buffer[i]; i++)            
        {
            ncPrintChar(buffer[i]);
        }
    }
    if(fd==2){      // STDERR
        for (int i = 0; i < count && buffer[i]; i++)
        {
            ncPrintColorChar(buffer[i], RED);
        }
    }
}

void read(unsigned int fd, char * buffer, unsigned int count){      
    if(fd==0){      // STDIN
        cleanBuffer();
        while(getEndBuffer()<count){
		    _hlt();
        }
        char * inBuffer = getBuffer();
        int i;
        for (i = 0; i < count ; i++)
        {
            buffer[i] = inBuffer[i];
        }
        
        cleanBuffer();
    }
}