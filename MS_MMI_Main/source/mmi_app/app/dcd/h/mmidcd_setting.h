/****************************************************************************
** File Name:      mmidcd_setting.h                                        *
** Author:         jun.hu                                                  *
** Date:           03/02/2010                                              *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE            NAME             DESCRIPTION                            *
** 03/02/2010      jun.hu            create
** 
****************************************************************************/

#ifndef _MMIDCD_SETTING_H
#define _MMIDCD_SETTING_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"
#include "dal_time.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
 //MS00216980 cheney
#define    MMIDCD_SETTING_DEFAULT_INDEX               3
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIDCD_CONNECT_MODE_ALWAYS = 0,
    MMIDCD_CONNECT_MODE_NEVER,
    MMIDCD_CONNECT_MODE_OFF_WHEN_CRUISE,

    NUM_OF_MMIDCD_CONNECT_MODE

} MMIDCD_CONNECT_MODE_E;

typedef enum
{
    MMIDCD_SCROLL_TIME_SLOW = 3,
    MMIDCD_SCROLL_TIME_MIDDLE = 2,
    MMIDCD_SCROLL_TIME_FAST = 1,

    NUM_OF_MMIDCD_SCROLL_TIME = 3

} MMIDCD_SCROLL_TIME_E;

typedef enum
{
    MMIDCD_CONTENT_TYPE_NONE = 0,
    MMIDCD_CONTENT_TYPE_IMAGE,
    MMIDCD_CONTENT_TYPE_AUDIO,
    MMIDCD_CONTENT_TYPE_MOVIE,
    MMIDCD_CONTENT_TYPE_MAX,

}MMIDCD_CONTENT_TYPE_E;

typedef enum
{
    MMIDCD_MEMORY_ON_PHONE = 0,
    MMIDCD_MEMORY_ON_DISK,
    MMIDCD_MEMORY_TYPE_MAX,
}MMIDCD_MEMORY_TYPE_E;

typedef struct MMIDCD_NV_SETTING_T_tag  
{
    BOOLEAN                     is_dcd_open;
    //BOOLEAN                     is_idle_show;
    //BOOLEAN                       is_auto_play;
    BOOLEAN                     is_auto_update;
    BOOLEAN                     is_show_readed; // 显示已读条目
    BOOLEAN                     is_dcd_update_log_on;
    uint8                       simindex[MN_DUAL_SYS_MAX];   // sim 卡对应的选的index, 目前暂不支持双卡分开的情况...
    MN_DUAL_SYS_E               sim_slected;
    MMIDCD_MEMORY_TYPE_E        memory_type;
    MMIDCD_CONNECT_MODE_E       connect_mode;
    MMIDCD_SCROLL_TIME_E        scroll_time;
    SCI_TM_T	                dcd_update_time;
} MMIDCD_NV_SETTING_T;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : read setting info from nv
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMIDCD_NV_SETTING_T MMIDCD_SetSettingDefault(void);
/*****************************************************************************/
//  Description : init dcd setting 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_InitSetting(void);
/*****************************************************************************/
//  Description : Get mms net setting index 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIDCD_Setting_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description : Set mms net setting index  
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_Setting_SetNetSettingIndex(
                                              MN_DUAL_SYS_E dual_sys,
                                              uint8         index
                                              );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIDCD_SETTING_H*/


