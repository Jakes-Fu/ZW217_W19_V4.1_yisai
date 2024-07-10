/*****************************************************************************
** File Name:      mmidc_conflict.h                                          *
** Author:                                                                   *
** Date:           22/05/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to between dc and app manager           *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2012       robert.wang       Create                                    *
******************************************************************************/
#ifndef _MMIDC_CONFLICT_H_
#define _MMIDC_CONFLICT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//typedef MMI_RESULT_E (* PROCESSMSG_FUNC)(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
typedef BOOLEAN (*APP_CANCEL_FUNC)(DPARAM param);


typedef BOOLEAN (*MMIDC_APP_CHECK_FUNC)(void);
typedef void (*MMIDC_APP_CLOSE_FUNC)(void** param, uint32* param_size);

typedef void (*MMIDC_APP_START_FUNC)(void** param, uint32* param_size);

typedef enum
{
    MMIDC_APP_WIFI,
    MMIDC_APP_BROWSER,        
    MMIDC_APP_EMAIL,    
    MMIDC_APP_EBOOK,
    MMIDC_APP_DYNAMIC_MAINMENU,    
    MMIDC_APP_PIC_VIEWER,
    MMIDC_APP_AUDIO_PLAYER,
    MMIDC_APP_MMS,        
    MMIDC_APP_BLUETOOTH,        
    MMIDC_APP_MOBILE_VIDEO,
    MMIDC_APP_DL,
    MMIDC_APP_UZONE,
    MMIDC_APP_QBTHEME,    
    MMIDC_APP_MAX
}MMIDC_APP_E;

typedef enum
{
    MMIDC_APP_TYPE_NONE,
    MMIDC_APP_TYPE_RESTORE,
    MMIDC_APP_TYPE_CLOSE,
    MMIDC_APP_TYPE_QUERY,
    MMIDC_APP_TYPE_MAX
}MMIDC_APP_TYPE_E;

typedef struct
{
    MMIDC_APP_E  app_id;
    MMIDC_APP_TYPE_E do_type;
    MMI_TEXT_ID_T app_name_id;
    MMIDC_APP_CHECK_FUNC check_func_ptr;
    MMIDC_APP_CLOSE_FUNC close_func_ptr;
    MMIDC_APP_START_FUNC start_func_ptr;
    void* param;
    uint32 param_size;
    BOOLEAN is_running;
}MMIDC_APP_ITEM_T;

typedef struct
{
    uint32 total_num;
    MMIDC_APP_ITEM_T *array_ptr;
}MMIDC_APP_LIST_T;

typedef struct
{
    PROCESSMSG_FUNC ok_func;
    DPARAM          param;
    uint32          size_of_param;
}MMIDC_APP_OK_FUNC_T;

typedef struct
{
    APP_CANCEL_FUNC  cancel_func;
    DPARAM          param;
    uint32          size_of_param;    
}MMIDC_APP_CANCEL_FUNC_T;

typedef struct 
{
    MMIDC_APP_OK_FUNC_T ok_func_info;
    MMIDC_APP_CANCEL_FUNC_T cancel_func_info;
    MMIDC_APP_LIST_T app_list_info;
}MMIDC_APP_CONFLICT_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : check applet conflict
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CheckAppConflict(
                                       const MMIDC_APP_CONFLICT_T* conflict_ptr //in
                                       );

/*****************************************************************************/
//  Description : restore 2m sensor environment
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_Restore2MSensorEnvironment(BOOLEAN is_check);

/*****************************************************************************/
//  Description : get dc app total number
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetDCAppTotalNumber(void);

/*****************************************************************************/
//  Description : get dc app array
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_APP_ITEM_T* MMIDC_GetDCAppArrayPtr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


