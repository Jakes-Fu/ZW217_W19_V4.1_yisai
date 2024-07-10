#include "zdt_app.h"

#ifdef ZDT_PLAT_SHB_SUPPORT
int SHB_API_BatStatus_Post(uint8 level,uint8 percent)
{
    uint16 bat_status = 0;
    bat_status = level;
    bat_status = bat_status << 8;
    bat_status += percent;
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SHB_BATSTAUTS,NULL,bat_status);
    return 0;
}

int SHB_API_LBS_Change_Post(void)
{
    //基站发生变化
    if(g_is_shb_power_off_ing == TRUE)
    {
        return 0;
    }
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SHB_LBS_CHANGE,NULL,0);
    return 0;
}
#endif

