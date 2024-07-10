/*******************************************************************************
 ** FileName:      mmifm_win32_simu.c                                          *
 ** Author:        baokun.yin                                                  *
 ** Date:          2008/12/17                                                  *
 ** CopyRight:     2008, Spreadtrum, Incoporated, All right reserved            *
 ** Description:   define FM simu interface                                    *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2008/12/17     baokun.yin             Created                              *
 *******************************************************************************/

#define MMIFM_WIN32_SIMU_C

#include "std_header.h"
#include "mmifm_win32_simu.h"

#ifdef __cplusplus
	extern "C"
	{
#endif
#define FM_FREQ_RANGE_HIGH 1080
#define FM_FREQ_RANGE_LOW  875
static uint16 s_fm_autoscan_freq_tab[30] = {885, 890, 895, 900, 905, 910, 915, 920, 925, 930,
                                            935, 940, 945, 950, 955, 960, 965, 970, 975, 990,
                                            995, 1000, 1005, 1015, 1020, 1025, 1030, 1035, 1040, 1045,
                                            };
LOCAL FM_SINGLE_CHANNEL_INFO_T s_single_channel_info = {885,	100, FALSE, FM_CHANNEL_SPEAKER, FM_VOL_LEVEL_3};
/******************************************************************************/
// Description: set FM scan level
// Author:     liqing.peng
// Input:      
// Output:     
// Return:     
// Note: 
/******************************************************************************/
uint32 FM_SetScanLevel(FM_SCANLEVEL_E level)
{
    return 0;
}

/*****************************************************************************/
//  Description:    This function is used to Check if FM has init.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN FM_IsInit(void)
{
	return TRUE;
}


/*****************************************************************************/
//  Description:    This function is used to seek radio station automatically.
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_AutoSeek(
              uint16 	freq,
		FM_SCAN_DIR_E	seek_dir,
		FM_SCAN_MODE_E mode,			// 0 -> Search for next/previous station; 1 -> Auto scan
		FM_ALL_CHANNEL_INFO_T  *all_channel_info
		)
{
    if(FM_SCAN_DOWN == seek_dir)
    {
        s_fm_autoscan_freq_tab[0] = freq - 5;
    }
    else
    {
        s_fm_autoscan_freq_tab[0] = freq + 5;
    }
    if(s_fm_autoscan_freq_tab[0] > FM_FREQ_RANGE_HIGH)
    {
        
        s_fm_autoscan_freq_tab[0] = FM_FREQ_RANGE_HIGH;
    }
    else if(s_fm_autoscan_freq_tab[0] < FM_FREQ_RANGE_LOW)
    {
        s_fm_autoscan_freq_tab[0] = FM_FREQ_RANGE_LOW;
    }
    all_channel_info->auto_freq_tab_ptr = s_fm_autoscan_freq_tab;
    return 0;
}

/******************************************************************************/
// Description: init FM 
// Author:     liqing.peng
// Input:      
// Output:     
// Return:     
// Note: 
/******************************************************************************/
uint32 FM_Init(void)
{
    return 0;
}

/******************************************************************************/
// Description: open FM 
// Author:     liqing.peng
// Input:      
// Output:     
// Return:     
// Note: 
/******************************************************************************/
uint32 FM_Open(void)
{
    return 0;
}

/*****************************************************************************/
//  Description:    This function is used to seek radio station manually.
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
uint32 FM_ManualSeek (
              uint16 freq,
              FM_SCAN_DIR_E seek_dir,
              uint8 steps, 
              FM_SINGLE_CHANNEL_INFO_T *single_channel_info)
{
    if(PNULL == single_channel_info)
    {
        return 0;
    }
    if(FM_SCAN_DOWN == seek_dir)
    {
        s_single_channel_info.freq =  freq;
    }
    else
    {
        s_single_channel_info.freq =  freq;
    }
    if(s_single_channel_info.freq > FM_FREQ_RANGE_HIGH)
    {
        
        s_single_channel_info.freq = FM_FREQ_RANGE_HIGH;
    }
    else if(s_single_channel_info.freq < FM_FREQ_RANGE_LOW)
    {
        s_single_channel_info.freq = FM_FREQ_RANGE_LOW;
    }
    *single_channel_info = s_single_channel_info;
    return 0;
}

/******************************************************************************/
// Description: close FM
// Author:     liqing.peng
// Input:      
// Output:     
// Return:     
// Note: 
/******************************************************************************/
uint32 FM_Close(void)
{
    return 0;
}

/******************************************************************************/
// Description: stop FM
// Author:     liqing.peng
// Input:      
// Output:     
// Return:     
// Note: 
/******************************************************************************/
uint32 FM_PlayStop(void)
{
    return 0;
}
#ifdef __cplusplus
    }
#endif


