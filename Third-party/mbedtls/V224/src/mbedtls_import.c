#include "dal_time.h"
#include "mbedtls/mbedtls_import.h"

//modified by cheng.du of sprd
unsigned int  HAL_time(void)
{
    uint32 seconds = 0;
    seconds = TM_GetTotalSeconds() + 315504000; //TM_GetTotalSeconds is from 1980.1.1 .  315504000 is seconds from 1970.1.1 to 1980.1.1
    return seconds;
}


