/*****************************************************************************
** File Name:               mmiap_spectrum.h                                 *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#ifdef MMIAP_SPECTRUM_SUPPORT
#ifndef _MMIAP_SPECTRUM_H_
#define _MMIAP_SPECTRUM_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"

#include "mmisrv.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIMP3_SPECTRUM_TIME            50  //频谱显示间隔
#define MMIMP3_SPEC_COLOR               0x0031
#define MMIMP3_FREQ_NUM_MAX             32      //频点数
#define MMIMP3_FREQ_VALUE_MAX           128     //峰值
#define MMIMP3_SPECTRUM_MARGIN         (1)

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
typedef struct
{
    BOOLEAN is_check_win_focus; //窗口是否在焦点状态
    BOOLEAN is_show_spectrum; //显示频谱
}MMIAP_SPECTRUM_WIN_CALLBACK_PARAM_T;

typedef struct
{
 uint16    spec_top_line[MMIMP3_FREQ_NUM_MAX + 1]; //频率图位置

 BOOLEAN       is_spec_open;                 //是否已开启
 uint8         timer_id;              //频率图刷新timer
}MMIAP_SPECTRUM_INFO_T;

typedef struct
{
    GUI_LCD_DEV_INFO lcd_dev_info;
    uint8 margin;
    GUI_COLOR_T color;
    GUI_RECT_T rect; //spectrum rect
    uint32 max_freq_num;//频点数
    MMISRV_HANDLE_T audio_handle;
}MMIAP_DISPLAY_SPECTRUM_T;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Start Mp3 spectrum.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_StartSpectrum(
                    void
                    );
                    
/*****************************************************************************/
//  Description : Stop Mp3 spectrum.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_StopSpectrum(
                    BOOLEAN is_clear
                    );
                    
/*****************************************************************************/
//  Description : Is Spectrum Open
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsSpectrumOpen(void);

/*****************************************************************************/
//  Description : Display mp3 Spectrum image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplaySpectrum(
    MMI_CTRL_ID_T ctrl_id //in
);

/*****************************************************************************/
//  Description : clear mp3 spec.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearSpectrumInfo(void);

/*****************************************************************************/
//  Description : destroy spectrum ino
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroySpectrumInfo(
    void
);

/*****************************************************************************/
//  Description : init spectrum info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitSpectrumInfo(
    void
);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif

#endif //MMIAP_SPECTRUM_SUPPORT

