#include "lib.h"
#include "cpu.h"

typedef struct cpuFeatures{
    int bit;
    char* name;
    char reg;
}cpuFeatures;

static cpuFeatures feature[] = {
    {1<<25, "SSE", 'd'}, 
    {1<<26, "SSE2", 'd'},
    {1<<0, "SSE3", 'c'},
    {1<<19, "SSE41", 'c'},
    {1<<20, "SSE42", 'c'},
    {1<<25, "AES", 'c'},
    {1<<28, "AVX", 'c'},
    {1<<29, "F16C", 'c'},
    {1<<1, "PCLMULQDQ", 'c'},
    {1<<12, "FMA", 'c'}};


static cpuFeatures extended[] = {
    {1<<5,"AVX2",'b'},
    {1<<9,"VAES",'c'},
    {1<<10,"VPCLMULQDQ", 'c'}
};


void cpuidNotValid(){
    printf("CPUID not enabled\n");
}

void processorVersionInfo(int ebx, int ecx, int edx){
    printf("CPU Version Information features:\n");
    auxiliar(feature, ebx, ecx, edx, 10);
}

void auxiliar(cpuFeatures features[], int ebx, int ecx, int edx, int cant){
    for (int i = 0; i < cant; i++){
    printf("Press ANY key to view next feature\n");
    getChar();
    
        switch (features[i].reg)
        {
        case 'b':
            printf("%s is %s\n", features[i].name, (ebx&features[i].bit)!=0? "supported":"not supported");
            break;
        case 'c':
            printf("%s is %s\n", features[i].name, (ecx&features[i].bit)!=0? "supported":"not supported");
            break;
        case 'd':
            printf("%s is %s\n", features[i].name, (edx&features[i].bit)!=0? "supported":"not supported");
            break;
        
        default:
            break;
        }
    }
}

void processorExtendedFeatures(int ebx, int ecx, int edx){
    printf("Cpu extended features:\n");
    auxiliar(extended, ebx,ecx,edx, 3);
}