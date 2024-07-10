
#include "zdt_common.h"

extern BOOLEAN MMIAPIDC_HandleHallInt(BOOLEAN is_high);

uint32 ZDT_HALL_PostEvent(uint8 is_high)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_HALL_INT,NULL,is_high);
    return 0;
}

BOOLEAN  MMIZDT_ZW31_HanldeHallInt(uint32 len)
{
    #ifdef  CAMERA_SUPPORT  //yangyu add for compile error
    if(len == 0)
    {
        MMIAPIDC_HandleHallInt(FALSE);
    }
    else
    {
        MMIAPIDC_HandleHallInt(TRUE);
    }
    #endif
    return TRUE;
}

