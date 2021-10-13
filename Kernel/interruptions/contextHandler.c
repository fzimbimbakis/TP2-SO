#include "contextHandler.h"
#include <stdint.h>
struct proces{
    char state;
    int returnValue;
}typedef proces;
//
//static* proces;

int64_t handler(int64_t newRIP){
    return newRIP;
}
