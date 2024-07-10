/******************************************************************************
** File Name:      mmiwidget.c                                                   *
** Author:                                                                    *
** Date:           06/2007                                                    *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe button                       *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME               DESCRIPTION                              *
** 06/2007        bin.ji     Create                                   *                              *
******************************************************************************/
#ifndef _MMIGRID_UIDLE_SE_H_
#define _MMIGRID_UIDLE_SE_H_ 

#ifdef PDA_UI_SUPPORT_U_IDLE
/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmiwidget.h"
#include "u_launcher_internal.h"


/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/
#define UIDLE_CLASSIC_SE_SUPPORT  
#define UIDLE_SLOPE_SE_SUPPORT  
#define UIDLE_BOXROT_SE_SUPPORT  
#define UIDLE_BOOKPAGE_SE_SUPPORT  
#define UIDLE_MIDROTATE_SE_SUPPORT  
#define UIDLE_SMALLITEMS_SE_SUPPORT  


#define MMI_IDLE_FLING_MAX_VELOCITY        (800)  

typedef enum
{
    UIDLE_STYLE_CLASSIC,    
    UIDLE_STYLE_SLOPE,  		
    UIDLE_STYLE_BOXROT,		
    UIDLE_STYLE_BOOKPAGE,	
    UIDLE_STYLE_MIDROTATE,	
    UIDLE_STYLE_SMALLITEMS,	
    UIDLE_STYLE_RANDOM,
	UIDLE_STYLE_MAX
}UIDLE_STYLE_E;

typedef struct
{
    MMIGRID_ITEM_TYPE_E     item_type;   
    GUI_LCD_DEV_INFO        item_dev;
}MMIGRID_U_ITEM_INFO_T;

typedef struct 
{    
    MMIGRID_U_ITEM_INFO_T  item_info[MMI_IDLE_GRID_PAGE_CELL_NUM];
    GO_MAINMENU_PAGE_INFO_T page_info;
}MMIGRID_U_PAGE_INFO_T;

/*****************************************************************************/
//  Description: handle page u idle tp move.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUIdle_HandleUIdleSlideTpMov(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description: Handle u Idle slide tp up. 
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIUIdle_HandleUIdleSlideTpUp(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description: free u Idle action.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMIUIDLE_FreeUIdleAction(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description: is uidle effect close or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUIdle_IsOpen(void);

/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
#endif
