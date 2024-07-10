/******************************************************************************
**  File Name:      mmiatv_wintab.h                                             *
**  Author:         xin.li                                                    *
**  Date:           2010/05                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about nv                  *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  05/2010       xin.li           Create.                                    *
******************************************************************************/
#ifdef ATV_SUPPORT
#ifndef _MMIATV_WINTAB_H_
#define _MMIATV_WINTAB_H_


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmiatv_fsm.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIATV_CHANNEL_MAX_NUM          15		//频道最大个数
        
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIATV_SCREEN_NORMAL,
    MMIATV_SCREEN_FULLSCREEN,
    MMIATV_SCREEN_MODE_MAX
} MMIATV_SCREEN_MODE_TYPE_E;

typedef enum
{
    MMIATV_GET_FOCUS,
    MMIATV_LOSE_FOCUS,
    MMIATV_INTERRUPT_MODE_MAX
} MMIATV_INTERRUPT_MODE_TYPE_E;

typedef struct MMIATV_INFO_T_tag 
{
    
    FSMSRV_HANDLE              atv_handle;
    uint32                  current_index;

    uint8                   systimer_id;
    uint8                   record_timer_id;
    uint32                  systimer_type;
    uint16                  service_num_count;
    uint16                  common_tip_count;
    uint16                  volume_count;
    uint16                  brightness_count;
    
    BOOLEAN                 is_need_start_search;
//    uint32                  search_total_num;
//    uint32                  search_cur_num;

    MMIATV_SCREEN_MODE_TYPE_E screen_mode;
	BOOLEAN                   is_display_panel;
	uint8                     panel_timer_id;
	BOOLEAN                   is_bright_panel;
	MMI_TEXT_ID_T				text_id;
	MMI_TEXT_ID_T				always_text_id;
    //MMIATV_STATUS_TYPE_E      play_status;
} MMIATV_INFO_T;

typedef struct MMIATV_SAVE_PROG_ID_T_Tag
{
    uint32                  service_id;
} MMIATV_SAVE_PROG_T;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/   
/*****************************************************************************/
// 	Description : Register atv win id
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_RegWinIdNameArr(void);

/*****************************************************************************/
// 	Description : open atv player main win
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_OpenATVMainWin(void);

/*****************************************************************************/
// 	Description : whether atv is open
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsATVOpen(void);

/*****************************************************************************/
// 	Description : exit atv
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_Exit(void);


/*****************************************************************************/
// 	Description : set current screen mode
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
// PUBLIC void MMIATV_SetScreenMode(MMIATV_SCREEN_MODE_TYPE_E mode);

/*****************************************************************************/
// 	Description : update control button state
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_UpdateButtonStatus(BOOLEAN is_refresh_all);

/*****************************************************************************/
// 	Description : 设置主播放窗口是否允许旋屏
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_EnableLcdSwitch(BOOLEAN is_enable);

/*****************************************************************************/
// 	Description : open alert win by text_id
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_OpenCommonAlertWin(MMI_TEXT_ID_T     text_id,
                                      MMI_IMAGE_ID_T    image_id);

/*****************************************************************************/
// 	Description : display service search progress
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplaySearchProgress(uint32 cur_num, uint32 total_num);

/*****************************************************************************/
// 	Description : clear search progress
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearSearchProgress(void);

/*****************************************************************************/
// 	Description : display service number
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplayServiceNum(void);

/*****************************************************************************/
// 	Description : clear service number
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearServiceNum(BOOLEAN is_focus);

/*****************************************************************************/
// 	Description : display common tip by string
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplayCommonTipByStr(MMI_STRING_T *text_str_ptr);

/*****************************************************************************/
// 	Description : display common tip
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplayCommonTip(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
// 	Description : display common tip all the time
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplayCommonTipAlways(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
// 	Description : clear common tip rect
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearCommonTip(void);

        
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* _MMIATV_WINTAB_H_ */
#endif /* ATV_SUPPORT */


