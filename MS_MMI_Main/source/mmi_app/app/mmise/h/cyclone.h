/****************************************************************************
** File Name:      cyclone.h                                                	                    *
** Author:           jila                                                      		            *
** Date:           2011.05.11                                             	                            *
** Description:    This file is used to describe the tyd  public applications          *
****************************************************************************
**                         Important Edit History                         	 	*
** ------------------------------------------------------------------------*
** DATE           	        NAME             DESCRIPTION                       	*
** 2011.05.15        	mmimenu_cyclone.c         	
** 
****************************************************************************/
#ifndef _CYCLONE_H_
#define _CYCLONE_H_ 
 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_appmsg.h"
#include "mmise_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif 


#ifdef WIN32
#define ANIM_ROLL_OFF             (180)
#else
#define ANIM_ROLL_OFF    		 (225*3)
#endif


#ifdef MAINLCD_SIZE_240X320
//#define ICON_WIDTH	(48)
//#define ICON_HEIGHT	(48)
//#define ICON_NUM	(10)
#define FENG_HEIGHT	(240)
#define FENG_ROUND	(100)
#elif defined MAINLCD_SIZE_240X400
//#define ICON_WIDTH	(44)
//#define ICON_HEIGHT	(44)
//#define ICON_NUM	(128)
#define FENG_HEIGHT	(300)
#define FENG_ROUND	(100)
#elif defined MAINLCD_SIZE_320X480
//#define ICON_WIDTH	(48)
//#define ICON_HEIGHT	(48)
//#define ICON_NUM	(128)
#define FENG_HEIGHT	(360)
#define FENG_ROUND	(260)
#elif defined MAINLCD_SIZE_176X220
//#define ICON_WIDTH	(48)
//#define ICON_HEIGHT	(48)
//#define ICON_NUM	(128)
#define FENG_HEIGHT	(180)
#define FENG_ROUND	(130)
#endif

#define CYCLONE_SIN_BASE   65535

typedef struct CYCLONE_ANIM_PARAM_Tag
{
    int  start_angle;
    int  stop_angle;
    int  cur_angle;
    int  cur_frame_offset_angle;
    int  calc_angle; 
    int  total_frame;
    int  cur_frame;
    int  direction;
}CYCLONE_ANIM_PARAM_T;

typedef enum
{
    STATE_SCROLL_NONE,
    STATE_SCROLL_READY,
    STATE_SCROLLING,
    STATE_SCROLL_ANIM,
    STATE_SCROLL_MAX,
}SCROLL_STATE_E;


typedef struct CYCLONE_SCROLL_PARAM_TAG
{
    int32    scroll_angle;
    int32    anim_angle;
    int32   last_time_stop_angle;
    int32    inertia_angle;
    int32    catch_angle;
    int32   catch_start_angle;
    int32    catch_stop_angle;
    int32    inertia_frame;
    int32    total_catch_frame;
    int32    cur_catch_frame;
    uint16 cur_select_pos_index;
    uint16 cur_item_index;
    SCROLL_STATE_E tp_state;
}CYCLONE_SCROLL_PARAM_T;

PUBLIC void MMICYCLONE_DualBuff_Reset(uint8 * buffer1,uint8 *buffer2);

PUBLIC uint8* MMICYCLONE_DualBuff_GetWriteBuff(void);

#ifdef   __cplusplus
    }
#endif

#endif //
