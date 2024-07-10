#include "UniTime.h"

extern void SCI_Sleep(uint32 millisecs);

int usleep(useconds_t usec)
{
    SCI_Sleep(usec/1000);
    return 0;
}


