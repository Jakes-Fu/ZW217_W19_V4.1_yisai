/****************************************************************************
** File Name:      mmiidle_hw_icon.c                                       *
** Author:                                                                 *
** Date:           12/19/2008                                              *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2008       apple.zhang         Create
** 
****************************************************************************/
//#include "std_header.h"
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
#define _MAIN_APP_C_  



/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#include "mmk_app.h"
#include "mmi_id.h"
#include "mmimp3_id.h"
#include "mmimp3_export.h"
#include "mmi_text.h"
#include "mmi_image.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmipb_export.h"
#include "mmipb_id.h"
#include "mmiidle_export.h"
#include "mmipub.h"
#include "mmidc_export.h"
#include "mmicc_export.h"
#include "mmi_position.h"
#include "mmimms_export.h"
#include "mmiacc_id.h"
#include "mmi_mainmenu_export.h"
#include "mmi_appmsg.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/                   
// typedef enum
// {
//     MMI_HARDWARE_ICON_1,
//     MMI_HARDWARE_ICON_2,
//     MMI_HARDWARE_ICON_3,  
//     MMI_HARDWARE_ICON_4,
// #ifndef MMI_FOUR_HW_ICON
//     MMI_HARDWARE_ICON_5,
// #endif
//     MMI_HARDWARE_ICON_MAX
// }MMI_HARDWARE_ICON_E;

typedef MMI_RESULT_E(*HDSHORTCUT_FUNC)(void);

/*
 使用硬图标虚拟物理按键时，在s_hw_icon_array内配置各图标要虚拟的物理键的键值即可，
 物理键值可见dal_keypad.h内定义(默认无效键值为0).
*/
#ifdef TOUCH_PANEL_HWICON_SUPPORT_VIRTUAL_2KEY
//虚拟两键配置
LOCAL MMI_HW_ICON_T s_hw_icon_array[MMI_HARDWARE_ICON_MAX] =
{
    {
        {
            MMI_HARDWARE_ICON_1_LEFT,
            MMI_HARDWARE_ICON_TOP, 
            MMI_HARDWARE_ICON_1_RIGHT, 
            MMI_HARDWARE_ICON_BOTTOM
        },
        PNULL, 
        SCI_VK_MENU_SELECT
    },
    {
        {
            MMI_HARDWARE_ICON_2_LEFT,
            MMI_HARDWARE_ICON_TOP, 
            MMI_HARDWARE_ICON_2_RIGHT, 
            MMI_HARDWARE_ICON_BOTTOM
        },
        PNULL, 
        SCI_VK_MENU_CANCEL,
    },
};
#else
LOCAL MMI_HW_ICON_T s_hw_icon_array[MMI_HARDWARE_ICON_MAX] =
{
    {
        {MMI_HARDWARE_ICON_1_LEFT,
         MMI_HARDWARE_ICON_TOP, 
         MMI_HARDWARE_ICON_1_RIGHT, 
         MMI_HARDWARE_ICON_BOTTOM},
        PNULL, 
        0, //SCI_VK_MENU_SELECT
    },
    {
        {MMI_HARDWARE_ICON_2_LEFT,
         MMI_HARDWARE_ICON_TOP, 
         MMI_HARDWARE_ICON_2_RIGHT, 
         MMI_HARDWARE_ICON_BOTTOM},
        PNULL, 
        0, //SCI_VK_WEB
    },
    {
        {MMI_HARDWARE_ICON_3_LEFT,
         MMI_HARDWARE_ICON_TOP, 
         MMI_HARDWARE_ICON_3_RIGHT, 
         MMI_HARDWARE_ICON_BOTTOM},
        PNULL, 
        0, //SCI_VK_MENU_CANCEL
    },
    {
        {MMI_HARDWARE_ICON_4_LEFT,
         MMI_HARDWARE_ICON_TOP, 
         MMI_HARDWARE_ICON_4_RIGHT, 
         MMI_HARDWARE_ICON_BOTTOM},
        PNULL, 
        0,
    },
#if (MMI_HARDWARE_ICON_MAX == 5 )
    {
        {MMI_HARDWARE_ICON_5_LEFT,
         MMI_HARDWARE_ICON_TOP, 
         MMI_HARDWARE_ICON_5_RIGHT, 
         MMI_HARDWARE_ICON_BOTTOM},
        PNULL, 
        0,
    },
#endif
};
#endif
/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Is Prohibit Handle HW Icon
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsProhibitHandleHWIcon(void);
/*****************************************************************************/
//  Description : execute the hardware icon function
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void  Handle_HW_Icon_Exec_Func(void);
/*****************************************************************************/
//  Description : handle hardware Mp3 icon process msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconMp3ProcessMsg(
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             );
/*****************************************************************************/
//  Description : handle hardware PB icon process msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconPBProcessMsg(
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );
/*****************************************************************************/
//  Description : handle hardware icon prompt msg
//  Global resource dependence : 
//  Author: Allen.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconPromptWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            );
/*****************************************************************************/
//  Description : handle hardware mainmenu icon process msg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconMainmenuProcessMsg(
                                                  MMI_MESSAGE_ID_E msg_id,
                                                  DPARAM param
                                                  );
/*****************************************************************************/
//  Description : handle hardware Dial icon process msg
//  Global resource dependence : 
//  Author: Xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconDialProcessMsg(
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              );
/*****************************************************************************/
//  Description : handle hardware camera icon process msg
//  Global resource dependence : 
//  Author: Xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconCameraProcessMsg(
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              );
/*****************************************************************************/
//  Description : handle hardware message icon process msg
//  Global resource dependence : 
//  Author: xiaoqingllu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconMessageProcessMsg(
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 );
/*****************************************************************************/
//  Description : handle hardware Mpeg4 icon process msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconMpeg4ProcessMsg(
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             );
/*****************************************************************************/
//  Description : enter camera win
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void EnterCameraWin(void);
/*****************************************************************************/
//  Description : enter message win
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void EnterMessageWin(void);
/*****************************************************************************/
//  Description : open icon prompt win
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenHWIconPromptWin(void);


#ifdef TOUCH_PANEL_HWICON_SUPPORT_MAINMENU_PB_SMS_CAMERA_MP4
LOCAL const HDSHORTCUT_FUNC s_func_arr[] = 
{
    {
        HandleHWIconMainmenuProcessMsg,        
    },
    {
        HandleHWIconPBProcessMsg,        
    },
    {
        HandleHWIconMessageProcessMsg,        
    },
    {
        HandleHWIconCameraProcessMsg,       
    },
    {
        HandleHWIconMpeg4ProcessMsg,       
    }       
};
#endif

#ifdef TOUCH_PANEL_HWICON_SUPPORT_MAINMENU_SMS_PB_DIAL_MP3
LOCAL const HDSHORTCUT_FUNC s_func_arr[] = 
{
        HandleHWIconMainmenuProcessMsg,   /*lint !e64*/     
        HandleHWIconMessageProcessMsg,   /*lint !e64*/     
        HandleHWIconPBProcessMsg,       /*lint !e64*/ 
        HandleHWIconDialProcessMsg,    /*lint !e64*/   
        HandleHWIconMp3ProcessMsg, /*lint !e64*/      
};
#endif

#ifdef TOUCH_PANEL_HWICON_SUPPORT_DIAL_PB_SMS_MAINMENU
LOCAL const HDSHORTCUT_FUNC s_func_arr[] = 
{
    {
        HandleHWIconDialProcessMsg,        
    },
    {
        HandleHWIconPBProcessMsg,        
    },
    {
        HandleHWIconMessageProcessMsg,        
    },
    {
        HandleHWIconMainmenuProcessMsg,       
    }       
};
#endif

LOCAL HDSHORTCUT_FUNC s_cur_hw_icon_func = HandleHWIconMainmenuProcessMsg;/*lint !e64*/
/**---------------------------------------------------------------------------*
 **                         difination                                        *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : start Hard icon for tp
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_StartHdShortCutBarIcon(void)
{
#ifdef TOUCH_PANEL_SUPPORT   
    int8 i = 0;    
    MMK_SetTPHardwareIconIsEnable(TRUE, MMI_HARDWARE_ICON_MAX);

    for (i = 0; i < MMI_HARDWARE_ICON_MAX; i++)
    {
#if (defined TOUCH_PANEL_HWICON_SUPPORT_MAINMENU_PB_SMS_CAMERA_MP4) || (defined TOUCH_PANEL_HWICON_SUPPORT_MAINMENU_SMS_PB_DIAL_MP3) || (defined TOUCH_PANEL_HWICON_SUPPORT_DIAL_PB_SMS_MAINMENU)
        s_hw_icon_array[i].ProcessMsg = s_func_arr[i];/*lint !e64*/
#endif
        MMK_SetTPHardwareIcon(TRUE, &s_hw_icon_array[i], i);
    }  
#endif
}

/*****************************************************************************/
//  Description : Is Prohibit Handle HW Icon
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsProhibitHandleHWIcon(void)
{
    if (
           MMK_IsOpenWin(MMIPUB_QUERY_WIN_ID)
        || MMK_IsOpenWin(MMIUDISK_UPCC_USING_WIN_ID)
        || MMK_IsOpenWin(MMIPUB_ALERT_WIN_ID)
        || MMK_IsOpenWin(MMIUDISK_IS_USING_WIN_ID)
        || MMK_IsOpenWin(MMIUDISK_STOP_WAIT_WIN_ID)
        || MMK_IsOpenWin(MMIPB_WAITING_ALERT_WIN_ID)
        #ifdef BLUETOOTH_SUPPORT
        || MMIAPIBT_IsSendingOrReceiving()
        #endif
        || MMIAPIMMS_IsSendingOrRecving()    
        || MMK_IsOpenWin(MMIKL_CLOCK_DISP_WIN_ID)
        )
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Is Prohibit Handle HW Icon
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsProhibitHandleHWIcon(void)
{
    return IsProhibitHandleHWIcon();
}

/*****************************************************************************/
//  Description : execute the hardware icon function
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void  Handle_HW_Icon_Exec_Func(void)
{    

    MMI_STRING_T        wait_text = {0};
    
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
	#ifdef JAVA_SUPPORT
    if (MMIAPIJAVA_IsJavaRuning())
    {
        //SCI_TRACE_LOW:"Handle_HW_Icon_Exec_Func : s_cur_hw_icon_func = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_HWICON_355_112_2_18_2_24_9_65,(uint8*)"d", s_cur_hw_icon_func);
 #ifdef JAVA_SUPPORT_SUN          
        MMIAPIJAVA_Shutdown(0);
 #endif
 #ifdef JAVA_SUPPORT_IA       
        MMIAPIJAVA_ExitJAVA();
 #endif
        MMK_ReturnIdleWin();
        if (PNULL != s_cur_hw_icon_func)
        {
            s_cur_hw_icon_func(); 
        }        
        s_cur_hw_icon_func = PNULL;
    }
    else
	#endif
    {    
        //SCI_TRACE_LOW:"Handle_HW_Icon_Exec_Func : s_cur_hw_icon_func = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_HWICON_372_112_2_18_2_24_9_66,(uint8*)"d", s_cur_hw_icon_func);
        MMK_ReturnIdleWin();
        s_cur_hw_icon_func(); 
        s_cur_hw_icon_func = PNULL;
    }
} 

/*****************************************************************************/
//  Description : handle hardware Mp3 icon process msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconMp3ProcessMsg(
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    if (IsProhibitHandleHWIcon())
    {
        return TRUE;
    }

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        MMIAPIMP3_OpenMp3Player();
    }
    else if (!MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))  

    {
        OpenHWIconPromptWin();
        s_cur_hw_icon_func = MMIAPIMP3_OpenMp3Player;/*lint !e64*/
    }
#endif
    return TRUE;
}
/*****************************************************************************/
//  Description : handle hardware Mpeg4 icon process msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconMpeg4ProcessMsg(
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION 
    if (IsProhibitHandleHWIcon())
    {
        return TRUE;
    }    

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        MMIAPIVP_MainEntry();
    }
    else if (!MMIAPIVP_IsOpenPlayerWin())   
    {
        OpenHWIconPromptWin();
        s_cur_hw_icon_func = MMIAPIVP_MainEntry;/*lint !e64*/
    }
#endif
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : handle hardware PB icon process msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconPBProcessMsg(
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_HANDLE_T win_handle = MMK_GetFocusParentWinHandle();
    //SCI_TRACE_LOW:"HandleHWIconPBProcessMsg enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_HWICON_445_112_2_18_2_24_10_67,(uint8*)"");
    if (IsProhibitHandleHWIcon())
    {
        return TRUE;
    }

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        //SCI_TRACE_LOW:"HandleHWIconPBProcessMsg forcus"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_HWICON_453_112_2_18_2_24_10_68,(uint8*)"");
        MMIAPIPB_OpenListAllGroupWin();
      
    }
    else if((NULL == win_handle) || win_handle != MMK_ConvertIdToHandle(MMIPB_MAIN_WIN_ID))        
    {
        OpenHWIconPromptWin();
        s_cur_hw_icon_func = MMIAPIPB_OpenListAllGroupWin;/*lint !e64*/
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : handle hardware icon prompt msg
//  Global resource dependence : 
//  Author: Allen.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconPromptWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //SCI_TRACE_LOW:"HandleHWIconPromptWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_HWICON_477_112_2_18_2_24_10_69,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        Handle_HW_Icon_Exec_Func();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(MMIHW_QUERY_WIN_ID);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);    
}

/*****************************************************************************/
//  Description : handle hardware mainmenu icon process msg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconMainmenuProcessMsg(
                                                  MMI_MESSAGE_ID_E msg_id,
                                                  DPARAM param
                                                  )
{
    if (IsProhibitHandleHWIcon())
    {
        return MMI_RESULT_TRUE;
    }

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        MMIAPIMENU_CreatMainMenu();
    }
    else if (!MMK_IsFocusWin(MAIN_MAINMENU_WIN_ID))
    {
        OpenHWIconPromptWin();
        s_cur_hw_icon_func = MMIAPIMENU_CreatMainMenu;/*lint !e64*/
    }

    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : handle hardware Dial icon process msg
//  Global resource dependence : 
//  Author: Xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconDialProcessMsg(
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              )
{
    if (IsProhibitHandleHWIcon())
    {
        return TRUE;
    }

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        MMIAPIIDLE_EnterDialWin();
    }
    else if (!MMK_IsFocusWin(MMIIDLE_DIAL_WIN_ID))
    {
        OpenHWIconPromptWin();
        s_cur_hw_icon_func = MMIAPIIDLE_EnterDialWin;/*lint !e64*/
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : enter camera win
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void EnterCameraWin(void)
{
    MMIAPIDC_OpenPhotoWin();
}
/*****************************************************************************/
//  Description : handle hardware camera icon process msg
//  Global resource dependence : 
//  Author: Xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconCameraProcessMsg(
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              )
{
    if (IsProhibitHandleHWIcon())
    {
        return TRUE;
    }

#ifdef CAMERA_SUPPORT
    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        EnterCameraWin();

    }
    else if (!MMIAPIDC_IsOpened())
    {
        OpenHWIconPromptWin();
        s_cur_hw_icon_func = EnterCameraWin;/*lint !e64*/
    }
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : enter message win
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void EnterMessageWin(void)
{
    MMIAPISMS_OpenMessageMainMenuWin();
}

/*****************************************************************************/
//  Description : handle hardware message icon process msg
//  Global resource dependence : 
//  Author: xiaoqingllu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHWIconMessageProcessMsg(
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 )
{
    if (IsProhibitHandleHWIcon())
    {
        return TRUE;
    }
    
    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        EnterMessageWin();
    }    
    else if (!MMIAPISMS_IsInSMSWin())
    {
        OpenHWIconPromptWin();
        s_cur_hw_icon_func = EnterMessageWin;/*lint !e64*/
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : open icon prompt win
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenHWIconPromptWin(void)
{
    MMI_WIN_ID_T    query_win_id = MMIHW_QUERY_WIN_ID;
    
    if (!MMK_IsFocusWin(query_win_id))
    {
        MMIPUB_CloseQuerytWin(&query_win_id);        
        MMIPUB_OpenQueryWinByTextId(STXT_EXIT,IMAGE_PUBWIN_QUERY,&query_win_id,HandleHWIconPromptWinMsg);
    }    

}
#endif
