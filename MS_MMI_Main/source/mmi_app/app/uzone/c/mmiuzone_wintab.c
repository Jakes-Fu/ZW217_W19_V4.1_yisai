/*****************************************************************************
** File Name:      mmiuzone_wintab.c                                         *
** Author:         Jessica                                               *
** Date:           21/12/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe uzone wintab                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011      Jessica              Creat
******************************************************************************/

#include "mmi_app_uzone_trc.h"
#ifdef MMIUZONE_SUPPORT

#define _MMIUZONE_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#ifdef WIN32 
#include "std_header.h"
#endif

#include "window_parse.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiuzone.h"
#include "mmiuzone_internal.h"
#include "mmiuzone_id.h"
#include "mmiuzone_position.h"
#include "mmiuzone_text.h"
#include "mmiuzone_image.h"
#include "mmiuzone_display.h"
#include "mmiuzone_se.h"
#include "mmiuzone_nv.h"
#include "mmiuzone_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                              STRUCT DEFINITION                        *
**--------------------------------------------------------------------------*/
typedef struct 
{
    CAF_APPLET_T caf_applet;
    uint32  user_data;
}MMIUZONE_APPLET_T;   //vt applet定义


/**--------------------------------------------------------------------------*
**                         EXTERN VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : the process message function of the Uzone
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  UzoneApplet_HandleEvent(    
            IAPPLET_T*          iapplet_ptr,
            MMI_MESSAGE_ID_E    msg_id, 
            DPARAM              param
            );

//LOCAL MMIUZONE_APPLET_T *s_uzone_applet_instance = PNULL; 

//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmiuzone_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_UZONE_APPLET_ID), sizeof(MMIUZONE_APPLET_T), UzoneApplet_HandleEvent , 
        IMAGE_SECMENU_ICON_MULTIM_UZONE, TXT_UZONE},
};

//applet的信息
LOCAL CAF_STATIC_APPLET_INFO_T s_mmiuzone_applet_info = 
{
    s_mmiuzone_applet_array,
    ARR_SIZE(s_mmiuzone_applet_array)
};

LOCAL MMIUZONE_NV_INFO_T s_uzone_nv_info = {0};


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//Description : Check UZone Running Environment
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckUzoneRunningEnvironment(void);


/*****************************************************************************/
//Description : start uzone applet
//Global resource dependence :none
//Author:
//Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartUzoneApplet(void);


/*****************************************************************************/
//Description : close uzone applet
//Global resource dependence :none
//Author:
//Note: 
/*****************************************************************************/
LOCAL void CloseUzoneApplet(void);

/*****************************************************************************/
//Description : save nv infor
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL void SaveNV(void);


/*****************************************************************************/
//Description : init se for uzone
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL void InitUzoneSE(void);


/*****************************************************************************/
//Description : handle open window msg
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleOpenWindow(MMI_WIN_ID_T        win_id);


/*****************************************************************************/
//Description : handle get focus msg
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleGetFocus(void);


/*****************************************************************************/
//Description : handle lose focus msg
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL void HandleLoseFocus(void);

/*****************************************************************************/
//Description : handle LCD switch
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLCDSwitch(void);


/*****************************************************************************/
//Description : handle full paint msg
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleFullPaint(void);


/*****************************************************************************/
//Description : handle full paint msg
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL void HandleFullPaintFinish(void);


/*****************************************************************************/
// 	Description : handle uzone main win msg
//	Global resource dependence :
//   Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUzoneMainWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);


/**--------------------------------------------------------------------------*
**                         WINTAB DEFINITION                                *
**--------------------------------------------------------------------------*/

WINDOW_TABLE(MMIUZONE_MAIN_TAB) = 
{
    WIN_FUNC((uint32)HandleUzoneMainWinMsg), 
    WIN_ID(MMIUZONE_MAIN_WIN_ID),
    WIN_STYLE(WS_DISABLE_COMMON_BG),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
#if defined(MMI_PDA_SUPPORT)
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_90),
#endif

    WIN_HIDE_STATUS,
    END_WIN
};


/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DEFINE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register uzone module applet info
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_UZONE, &s_mmiuzone_applet_info);
}

/*****************************************************************************/
//  Description : Entry function of uzone
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUZONE_Entry(void)
{
    BOOLEAN  ret = FALSE;

    //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_Entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_209_112_2_18_3_4_27_55,(uint8*)"");
    
    ret = CheckUzoneRunningEnvironment();
    if (!ret)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_Entry return fail for error environment!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_214_112_2_18_3_4_27_56,(uint8*)"");
        return ret;
    }

    return StartUzoneApplet();
}


/*****************************************************************************/
//  Description : set nv info
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_SetNVInfo(MMIUZONE_NV_INFO_T nv_info)
{
    s_uzone_nv_info = nv_info;
    SEAPI_SetCurItemIndex(s_uzone_nv_info.cur_item_index);     
}

/*****************************************************************************/
//  Description : get current item index
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIUZONE_GetCurItemIndex(void)
{
    return s_uzone_nv_info.cur_item_index;
}

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//Description : Check UZone Running Environment
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckUzoneRunningEnvironment(void)
{
    return TRUE;
}

/*****************************************************************************/
//Description : start uzone applet
//Global resource dependence :none
//Author:
//Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartUzoneApplet(void)
{
    BOOLEAN ret = FALSE;
    uint32 applet_id = SPRD_UZONE_APPLET_ID;
    MMI_HANDLE_T applet_handle = 0;
    
    MMI_APPLET_START_T start ={0};
    MMIUZONE_APPLET_T* applet_ptr = (MMIUZONE_APPLET_T*)MMK_GetAppletPtr(applet_id);
    
    if (PNULL == applet_ptr)
    {
        start.guid = applet_id;

        MMIPUB_OpenTempWaitWin();
        
        applet_handle = MMK_StartApplet( &start );
        if (PNULL != applet_handle)
        {
            ret = TRUE;
        }
    }
    else
    {
        ret = TRUE;
        MMK_GrabWinFocusByApplet(applet_id);
    }

    //SCI_TRACE_LOW:"[MMIUZONE] StartUzoneApplet ret = %d,applet_ptr=0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_273_112_2_18_3_4_27_57,(uint8*)"dd",ret, applet_ptr);

    return ret;
}

/*****************************************************************************/
//Description : close uzone applet
//Global resource dependence :none
//Author:
//Note: 
/*****************************************************************************/
LOCAL void CloseUzoneApplet(void)
{
    MMIUZONE_APPLET_T* applet_ptr = (MMIUZONE_APPLET_T*)MMK_GetAppletPtr( SPRD_UZONE_APPLET_ID );
    
    if( PNULL != applet_ptr )
    {
        SaveNV();
        MMIUZONE_Destroy();
        MMK_CloseApplet(applet_ptr->caf_applet.app_handle);
    }

    //SCI_TRACE_LOW:"[MMIUZONE] CloseUzoneApplet applet_ptr=0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_293_112_2_18_3_4_27_58,(uint8*)"d", applet_ptr);
}

/*****************************************************************************/
//Description : save nv infor
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL void SaveNV(void)
{
    s_uzone_nv_info.cur_item_index = SEAPI_GetCurItemIndex();
    MMINV_WRITE(MMINV_UZONE_INFO, &s_uzone_nv_info);
    //SCI_TRACE_LOW:"[MMIUZONE] SaveNV (%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_304_112_2_18_3_4_27_59,(uint8*)"d", s_uzone_nv_info.cur_item_index);

}

/*****************************************************************************/
//Description : init se for uzone
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL void InitUzoneSE(void)
{
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();

    if(PNULL == running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] InitUzoneSE - Invalid param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_318_112_2_18_3_4_27_60,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"[MMIUZONE] InitUzoneSe- current item = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_322_112_2_18_3_4_27_61,(uint8*)"d", s_uzone_nv_info.cur_item_index);

    SEAPI_SetItemNumber(running_info_ptr->item_num);
    SEAPI_SetCurItemIndex(s_uzone_nv_info.cur_item_index);
    SEAPI_SetSrcLayerHandle(&running_info_ptr->page_srclayer_dev_info,
                            &running_info_ptr->icon_srclayer_dev_info,
                            &running_info_ptr->str_srclayer_dev_info,
                            &running_info_ptr->str_shadow_dev_info);
                            
    SEAPI_SetDestLayerHandle(&running_info_ptr->fglayer_dev_info);
    SEAPI_SetItemCallback(MMIUZONE_ItemAction);


    SEAPI_SetStarLayerHandle(&running_info_ptr->bigstarlayer_dev_info, 
                                &running_info_ptr->smallstarlayer_dev_info);
    SEAPI_SetDrawItemCb(MMIUZONE_DrawFocusItem);
    SEAPI_SetFocusItemLayerHandle(&running_info_ptr->focusitemlayer_dev_info);

    
}

/*****************************************************************************/
//Description : handle open window msg
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleOpenWindow(MMI_WIN_ID_T        win_id)
{
    //SCI_TRACE_LOW:"[MMIUZONE] HandleOpenWindow"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_337_112_2_18_3_4_27_62,(uint8*)"");
    
    if(!MMIUZONE_Init(win_id))
    {
        //SCI_TRACE_LOW:"[MMIUZONE] HandleOpenWindow - MMIUZONE_Init fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_341_112_2_18_3_4_27_63,(uint8*)"");
        return FALSE;
    }
    
    if(!MMIUZONE_FillLayers())
    {
        //SCI_TRACE_LOW:"[MMIUZONE] HandleOpenWindow - MMIUZONE_FillLayers fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_347_112_2_18_3_4_27_64,(uint8*)"");
        return FALSE;
    }
    
    InitUzoneSE();
    
    MMIUZONE_DrawWindow();
    return TRUE;
}

/*****************************************************************************/
//Description : handle get focus msg
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleGetFocus(void)
{
    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();
    if(PNULL == running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] HandleGetFocus - Invalid param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_366_112_2_18_3_4_27_65,(uint8*)"");
        return FALSE;
    }

    running_info_ptr->is_landscape = MMK_IsWindowLandscape(running_info_ptr->main_win_id);
    
    //SCI_TRACE_LOW:"[MMIUZONE] HandleGetFocusWindow- current item = %d, landsape (%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_373_112_2_18_3_4_27_66,(uint8*)"dd", s_uzone_nv_info.cur_item_index, running_info_ptr->is_landscape);
    
    if(!MMIUZONE_CreateLayers())
    {
        //SCI_TRACE_LOW:"[MMIUZONE] HandleGetFocus - MMIUZONE_CreateLayers fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_377_112_2_18_3_4_27_67,(uint8*)"");
        return FALSE;
    }
    
    if(!MMIUZONE_FillLayers())
    {
        //SCI_TRACE_LOW:"[MMIUZONE] HandleGetFocus - MMIUZONE_FillLayers fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_383_112_2_18_3_4_27_68,(uint8*)"");
        return FALSE;
    }
    
    InitUzoneSE();
    MMIUZONE_DrawWindow();
    return TRUE;
}

/*****************************************************************************/
//Description : handle lose focus msg
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL void HandleLoseFocus(void)
{
    s_uzone_nv_info.cur_item_index = SEAPI_GetCurItemIndex();
    //SCI_TRACE_LOW:"[MMIUZONE] HandleLoseFocusWindow- current item = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_399_112_2_18_3_4_27_69,(uint8*)"d", s_uzone_nv_info.cur_item_index);
    MMIUZONE_ExistDraw();
//    MMIUZONE_ReleaseLayers();
}


/*****************************************************************************/
//Description : handle full paint msg
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleFullPaint(void)
{
    GUI_LCD_DEV_INFO main_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0};

    MMIUZONE_RUNNING_INFO* running_info_ptr = MMIUZONE_GetRunningInfo();

    if(PNULL == running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] HandleFullPaint - Invalid param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_461_112_2_18_3_4_27_74,(uint8*)"");
        return FALSE;
    }

    if(!MMK_IsFocusWin(running_info_ptr->main_win_id))
    {   
        //SCI_TRACE_LOW:"[MMIUZONE] HandleFullPaint -is not focus window"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_467_112_2_18_3_4_27_75,(uint8*)"");

        MMIUZONE_DrawBgToMainLayer();        

    }
    else
        //SCI_TRACE_LOW:"[MMIUZONE] HandleFullPaint -is focus window"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_493_112_2_18_3_4_28_78,(uint8*)"");

    return TRUE;

}

/*****************************************************************************/
//Description : the process message function of the uzone
//Global resource dependence : 
//Author:
//Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  UzoneApplet_HandleEvent(    
                                            IAPPLET_T*          iapplet_ptr, //in
                                            MMI_MESSAGE_ID_E    msg_id, //in
                                            DPARAM              param //in
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIUZONE_APPLET_T* applet_ptr = (MMIUZONE_APPLET_T*)iapplet_ptr;

    //SCI_TRACE_LOW:"[MMIUZONE] UzoneApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_536_112_2_18_3_4_28_81,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_APPLET_START:
        {
            MMI_WINDOW_TABLE_CREATE_T win_table = {0};

            if(!MMIUZONE_Create(applet_ptr->caf_applet.app_handle))
            {
                //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_Create return fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_546_112_2_18_3_4_28_82,(uint8*)"");
                return MMI_RESULT_FALSE;
            }

            win_table.applet_handle = applet_ptr->caf_applet.app_handle;
            win_table.win_table_ptr = MMIUZONE_MAIN_TAB;
            win_table.add_data_ptr = param;
            
            MMK_CreateWinTable( &win_table );
        }
        break;
        
    case MSG_APPLET_STOP:
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}


/*****************************************************************************/
// 	Description : handle uzone main win msg
//	Global resource dependence :
//   Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUzoneMainWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E result      = MMI_RESULT_TRUE;
    MMI_RESULT_E result2 = MMI_RESULT_TRUE;
    BOOLEAN res = TRUE;

    //SCI_TRACE_LOW:"[MMIUZONE] HandleUzoneMainWinMsg msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_WINTAB_584_112_2_18_3_4_28_83,(uint8*)"d",msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            res = HandleOpenWindow(win_id);
            if(!res)
            {
                MMIPUB_OpenAlertWarningWin(STXT_UZONE_OPEN_FAIL);
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            res = HandleFullPaint();
            if(!res)
            {
                MMIPUB_OpenAlertWarningWin(STXT_UZONE_OPEN_FAIL);
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_GET_FOCUS:
            res = HandleGetFocus();
            if(!res)
            {
                MMIPUB_OpenAlertWarningWin(STXT_UZONE_OPEN_FAIL);
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            MMIUZONE_ExistDraw();
            CloseUzoneApplet();
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    if(res)
    {
        result2 = SEAPI_HandleMsg(win_id, msg_id, param);
    }
    
    //单独处理lose focus
    if(MSG_LOSE_FOCUS == msg_id)
    {
       HandleLoseFocus();
       result = MMI_RESULT_TRUE;
    }
    
    return (result || result2);
}

/*****************************************************************************/
//Description : exit uzone
//Global resource dependence :
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_Exit(void)
{
    SCI_TRACE_LOW("[MMIUZONE] MMIUZONE_Exit");
    
    if (MMIAPIUZONE_IsOpened())
    {
        MMK_SendMsg(MMIUZONE_MAIN_WIN_ID, MSG_CLOSE_WINDOW, PNULL);
    }
}
#endif //MMIUZONE_SUPPORT
