#ifndef _CPU_H_
#define _CPU_H_
void cpuFun();
void cpuidNotValid();
void processorVersionInfo(int ebx, int ecx, int edx);
void processorExtendedFeatures(int ebx, int ecx, int edx);
#endif