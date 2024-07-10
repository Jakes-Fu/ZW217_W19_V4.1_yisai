/******************************************************************************
 ** File Name:       lcd_task.h                                               *
 ** Author:          Frank.Yang                                               *
 ** DATE:            04/25/2011                                               *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE              NAME             DESCRIPTION                            *
 ******************************************************************************/

 
#ifndef _LCD_TASK_H_
#define _LCD_TASK_H_

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#include "sci_api.h"
#include "dal_display.h"
#include "lcd_ctrl.h"
/******************************************************************************/
/* MACRO Define                                                               */
/******************************************************************************/

#define LCD_EVENT_WAIT_TIME           (20 * 1000)    //ms
#define LCD_EVENT_WAIT_QUIT_TIME      (20 * 1000)  //ms

/* event */
#define LCD_EVENT_NONE                0x00000000
#define LCD_EVENT_BLEND_OK            0x00000001
#define LCD_EVENT_INVALID_OK          0x00000002
#define LCD_EVENT_DONE                0x00000004
#define LCD_EVENT_ERROR               0x00000008

/*define error id*/ 
#define LCD_EID_NONE                  0

#define LCD_SUCCESS                   0
#define LCD_FAILED                    1
#define LCD_INVALID_STATUS            2  
#define LCD_MEMORY_ERROR              3
#define LCD_INVALID_ARGUMENT          4
#define LCD_FORMAT_UNSUPPORTED        5

#define LCD_STATUS_BUSY               6 
#define LCD_MEMORY_NOT_ENOUGH         7
#define LCD_MEMORY_MALLOC_FAILED      8
#define LCD_MEMORY_FREE_FAILED        9


#define LCD_UPDATE_MODE_INVALID       0
#define LCD_UPDATE_MODE_SYNC          1
#define LCD_UPDATE_MODE_ASYNC         2


typedef struct tag_DISPLAY_INVALIDATE_CONTEXT_T{

    DISPLAY_LAYER_INFO_T     layer_arr[DISPLAY_UPDATE_LAYER_NUM];
    uint32                   layer_num;
    DISPLAY_INVALIDATE_POS_T param;
}DISPLAY_INVALIDATE_CONTEXT_T;

typedef struct tag_DISPLAY_BLENDLAYER_CONTEXT_T{
    DISPLAY_BLEND_LAYER_T dst_layer;
    DISPLAY_BLEND_LAYER_T layer_arr[LAYER_SUPPORT_NUM];
    uint32                layer_num;
    DISPLAY_RECT_T        rect;

    DISPLAY_CALLBACK callback;
    uint32 param;

}DISPLAY_BLENDLAYER_CONTEXT_T;

typedef struct tag_DISPLAY_BLENDLAYER_INFO_T{
    uint8  *aux_mem_addr;
    uint16 lcd_width;
    uint16 lcd_height;
    DISPLAY_RECT_T *rect_ptr;    
}DISPLAY_BLENDLAYER_INFO_T;

typedef struct tag_DISPLAY_TASK_CONTEXT_T
{
    LCD_ID_E lcd_id;
    uint32 mode;
    DISPLAY_INVALIDATE_PARM_T *p_invalidate; // sync
    DISPLAY_BLENDLAYER_PARM_T *p_blend;
    
    DISPLAY_INVALIDATE_CONTEXT_T invalidate; // async
    DISPLAY_BLENDLAYER_CONTEXT_T blend;
    SCI_SEMAPHORE_PTR lcd_task_sm_ptr;
}DISPLAY_TASK_CONTEXT_T;


/*****************************************************************************/
//  Description: get current task context
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
DISPLAY_TASK_CONTEXT_T *LCD_GetTaskContext();
/*****************************************************************************/
//  Description: get lcd task control semaphor
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void LCD_GetAPISemaphore();
/*****************************************************************************/
//  Description: put lcd task control semaphor
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_PutAPISemaphore();
/*****************************************************************************/
//  Description: put lcd task control semaphor
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_PutTaskSemaphore(LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description: put lcd task control semaphor
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_PutTaskSemaphore(LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description: wait the DONE event and then clear it
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void LCD_WaitAndClearDoneStatus();
/*****************************************************************************/
//  Description: wait blend done event
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void LCD_WaitBlendDoneStatus();
/*****************************************************************************/
//  Description: get lcd task api semaphore
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void LCD_GetAPISemaphore();
#endif


