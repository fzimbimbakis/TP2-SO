#include <naiveConsole.h>
#include <keyboard.h>

#define GREY 7

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;
static uint8_t shellSelector = 1;
static uint8_t * previousVideo = 0xB8F00;
//static uint8_t * previousVideo = video + (height-1)*width*2;

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintColor(const char * string, int color){
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintColorChar(string[i], color);
}

void ncPrintChar(char character)
{
	ncScroll();
	if(character=='\n') {			// Enter
		ncNewline();
		return;
	}else if(character=='\t'){		// Tab (switch shell)
		ncSwitchShell();
		return;
	}else if(character=='\b'){		// Backspace
		ncBackspace();
		return;
	}
	currentVideo[1]=GREY;
	*currentVideo = character;
	currentVideo += 2;
	currentVideo[1]=0xf0;
}

void ncPrintColorChar(char character,int color)
{
	ncScroll();
	*currentVideo++ = character;
	*currentVideo++= color;
}

void ncScroll(){
	if(shellSelector){
		if(currentVideo-video == (height/2)*width*2){
			ncNewline();
		}
	}else{
		if(currentVideo-video == height*width*2){
			ncNewline();
		}
	}
}

void ncSwitchShell(){
	uint8_t* aux = currentVideo;
	currentVideo[1]=GREY;
	currentVideo = previousVideo;
	currentVideo[1]=0xf0;
	previousVideo = aux;
	shellSelector=1-shellSelector;
}

void ncNewline(){
	currentVideo[1]=GREY;
	if(shellSelector){
		for(int i = 0; i<(height/2)-1; i++){
			for (int j=0; j<2*width-1; j++)
				video[width*2*i + j] = video[width*2*(i+1) + j];
		}
		for(int i=0; i<2*width; i++){
			if(i%2==0)
				video[(height-2)/2*width*2+i] = ' ';
			else
				video[(height-2)/2*width*2+i]=GREY;//restauro color de celda
		}
		
		currentVideo = video+(height-2)/2*width*2;
		currentVideo[1]=0xf0;
		return;
	}

	for(int i = (height+1)/2; i<height; i++){
		for (int j=0; j<2*width; j++)
			video[width*2*i + j] = video[width*2*(i+1) + j];
	}
	for(int i =0; i<2*width; i++){
		if(i%2==0)
			video[height*2*width-i] = ' ';
		else
			video[height*2*width-i] =GREY; //restauro color de celda
	}
	
	currentVideo = video + (height-1)*width*2;
	currentVideo[1]=0xf0;
}


void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncBackspace(){
	if((int)(currentVideo-video)%(width*2)==0) return;
	currentVideo[1]=GREY;
	currentVideo-=2;
	*currentVideo=' ';
	currentVideo[1]=0xf0;
}

void ncClear(){
	int i, mid = width;
	for (i = 0; i < height * width; i++){
		video[i * 2] = ' ';
		video[i*2 + 1]=GREY;
	}
	for(i=0; i<width*2; i+=2){
		video[24*width+i] = '-';
	}
	currentVideo = (shellSelector)?video+ ((height/2)-1)*width*2:video + (height-1)*width*2;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
