/*****************************************************************************
** File Name:      mmiset_wintab.c                                           *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng     Creat
******************************************************************************/

#define _MMISET_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "guiiconlist.h"
#include "mmi_nv.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "tb_dal.h"
#include "guilabel.h"
#include "guimsgbox.h"
#include "mmiset_wintab.h"
#include "mmialarm_export.h"
#include "mmk_timer.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmi_appmsg.h"

#include "mmiidle_subwintab.h"
#include "mmi_textfun.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiset_display.h"
#include "mmiset_func.h"
#include "mmiset_set_ring.h"
#include "mmiset_text.h"
#include "mmiset_image.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
#include "mmk_tp.h"

#include "guibutton.h"
#include "mmiset_nv.h"
#include "mmipub.h"
#include "tp_export.h"

//#include "mmiset.h"
#include "mmiset_internal.h"
#include "mmiim.h"
#include "mn_api.h"
#include "guirichtext.h"
#ifdef MMI_DUALMODE_ENABLE
#ifndef WIN32
#include "tb_hal.h"
#endif
#endif
#include "mn_type.h"//@$discuss,此类头文件的增加是否需要宏控制？增加对VT的控制，增加软双模
#include "mn_api.h"
#include "mmi_mainmenu_export.h"
//#include "mmipb_wintab.h"
#include "mmipb_export.h"
#include "guitext.h"
//#include "freq_cfg.h"
#include "mmk_msg.h"
#include "mmi_resource.h"
#include "guires.h"
#include "mmiset_shortcut.h"
#include "guiedit.h"
#include "mmidm_export.h"
#include "guidropdownlist.h"
#include "mn_api.h"
#include "mmicom_time.h"

#include "mmicc_text.h"
#ifdef MMI_OCR_ENABLE
#include "mmiocr_export.h"
#endif
#include "mmipb_text.h"
#include "guiform.h"
//#include "mmiim_im_switching.h"

#include "guictrl_api.h"
#ifdef QQ_SUPPORT
#include "mmiqq_export.h"
#endif
#include "guisetlist.h"
#include "guiownerdraw.h"
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
#include "sci_api.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif


#include "guistatusbar.h"


#include "mmiocr_export.h"
#include "mmiidle_statusbar.h"
//#include "mmiset_mainmenu.h"
#include "mmiset_mainmenu_internal.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMI_BUTTON_MARGIN (20)

//#define MULTI_FILE_THEME_SUPPORT //支持放置在SD卡的主题文件
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

LOCAL GUI_LCD_DEV_INFO s_set_button_layer_info = {0,UILAYER_NULL_HANDLE};
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/





/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : to handle the ring setting main menu window message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRingMainMenuWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );



/*****************************************************************************/
//  Description : to handle select call ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectCallRingWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : to handle select fixed call ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectFixedCallRingWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    );

/*****************************************************************************/
//  Description : to handle select message ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectMsgRingWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );



/*****************************************************************************/
//  Description : to handle select fixed msg ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectFixedMsgRingWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   );
/*****************************************************************************/
//  Description : 添加消息铃声radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendMsgRingItem(
                             MN_DUAL_SYS_E  dual_sys,
                             MMI_CTRL_ID_T   ctrl_id
                             );



/*****************************************************************************/
//  Description : to open ring setting menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void EnterRingSettingMenu( void);



/*****************************************************************************/
//  Description :铃声设置中所有label的设置
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void EnvSetRingParam(MMI_WIN_ID_T    win_id);


/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
#ifndef MMI_ENVSET_MINI_SUPPORT
// the window for ring main menu
WINDOW_TABLE( MMISET_RING_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleRingMainMenuWindow ),    
    WIN_ID( MMISET_RING_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_ENV_RING_FORM_CTRL_ID),
    END_WIN
};
#endif
// the window for ring main menu
WINDOW_TABLE( MMISET_ICON_RING_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleRingMainMenuWindow ),    
    WIN_ID( MMISET_RING_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
#ifdef DPHONE_SUPPORT 
    CREATE_MENU_CTRL(MENU_ICON_SET_RING_SELECT, MMISET_RING_MENU_CTRL_ID),
#else    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_ENV_RING_FORM_CTRL_ID),
#endif 
    END_WIN
};

// the window for select call ring
WINDOW_TABLE( MMISET_RING_SELECT_CALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectCallRingWindow ),    
        WIN_ID( MMISET_RING_SELECT_CALL_WIN_ID ),
        WIN_TITLE( TXT_SET_RING_SELECT_CALL ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_RING_SELECT_CALL_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};

// the window for select fixed call ring
WINDOW_TABLE( MMISET_RING_SELECT_FIXED_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectFixedCallRingWindow ),    
        WIN_ID( MMISET_RING_SELECT_FIXED_WIN_ID ),
        WIN_TITLE( TXT_PB_RING_PHONE ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_RING_SELECT_FIXED_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),

        END_WIN
};


//the window for select fixed msg ring
WINDOW_TABLE( MMISET_MSG_RING_SELECT_FIXED_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectFixedMsgRingWindow ),    
        WIN_ID( MMISET_MSG_RING_SELECT_FIXED_WIN_ID ),
        WIN_TITLE( TXT_RING_FIXED ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID),
//#ifdef MMI_PDA_SUPPORT//bug71574
//        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
//#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
//#endif
        
        END_WIN
};

// the window for select message ring
WINDOW_TABLE( MMISET_RING_SELECT_MSG_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectMsgRingWindow ),    
        WIN_ID( MMISET_RING_SELECT_MSG_WIN_ID ),
        WIN_TITLE( TXT_SET_RING_SELECT_MSG ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_RING_SELECT_MSG_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to handle the ring setting main menu window message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
#ifdef DPHONE_SUPPORT 
LOCAL MMI_RESULT_E  HandleRingMainMenuWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_MENU_GROUP_ID_T          group_id   =   0;
    MMI_MENU_ID_T                menu_id    =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MN_DUAL_SYS_E               dual_sys    =   MN_DUAL_SYS_MAX;
#endif
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_RING_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_RING_MENU_CTRL_ID,&group_id,&menu_id);
        
#if defined(MMI_MULTI_SIM_SYS_SINGLE)
        switch (menu_id)
        {
        case ID_SET_RING_SELECT_CALL:
            MMIAPISET_OpenFixCallRingWin(MN_DUAL_SYS_1, 0,0);
            break;
            
        case ID_SET_RING_SELECT_MSG:
            MMK_CreatePubListWin((uint32*)MMISET_MSG_RING_SELECT_FIXED_WIN_TAB,PNULL);
            break;
        default:
            //SCI_PASSERT(0,("HandleRingMainMenuWindow menu_id = %d",menu_id));
            break;
        }
#else //MMI_MULTI_SIM_SYS_SINGLE
        if(menu_id >= ID_SET_RING_SIM1_SELECT_CALL && menu_id < ID_SET_RING_SIM_MAX_SELECT_CALL)
        {
            dual_sys = MN_DUAL_SYS_1 + (menu_id - ID_SET_RING_SIM1_SELECT_CALL);
            MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_CALL_WIN_TAB, (ADD_DATA)dual_sys);
        }
        else if(menu_id >= ID_SET_RING_SIM1_SELECT_MSG && menu_id < ID_SET_RING_SIM_MAX_SELECT_MSG)
        {
            dual_sys = MN_DUAL_SYS_1 + (menu_id - ID_SET_RING_SIM1_SELECT_MSG);
            MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_MSG_WIN_TAB, (ADD_DATA)dual_sys);
        }    
#endif //MMI_MULTI_SIM_SYS_SINGLE

        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#else
LOCAL MMI_RESULT_E  HandleRingMainMenuWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                 recode     =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                ctrl_id    =   0;
    uint16                       offset     =   0;
    //MN_DUAL_SYS_E               dual_sys    =   MN_DUAL_SYS_MAX;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        EnvSetRingParam(win_id);
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMISET_ENV_RING_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
        MMK_SetAtvCtrl(win_id, MMISET_ENV_CALL_RING_LABEL_CTRL_ID+1);
        GUIWIN_SetTitleTextId(win_id,TXT_SET_RING_SET,FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint8  i  =  0;
            MMIENVSET_ADD_DATA_T *add_data_ptr = (MMIENVSET_ADD_DATA_T*)SCI_ALLOC_APP(sizeof(MMIENVSET_ADD_DATA_T));
            if(MSG_CTL_PENOK == msg_id)
            {
                ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            }
            else
            {
                ctrl_id = MMK_GetActiveCtrlId(win_id);
            }
            offset = ctrl_id - MMISET_ENV_CALL_RING_LABEL_CTRL_ID;
            add_data_ptr->s_dual_sys= MN_DUAL_SYS_1 + offset/4;
            add_data_ptr->ctrl_id  = ctrl_id;
            for(i=0; i<MMI_DUAL_SYS_MAX; i++)
            {
                if((4*i+1) == offset)
                {
                    MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_CALL_WIN_TAB, (ADD_DATA)add_data_ptr);
                }
                else if((4*i+3) == offset)
                {
                    MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_MSG_WIN_TAB, (ADD_DATA)add_data_ptr);
                }
            }
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }    
    return recode;
}
#endif
/*****************************************************************************/
//  Description : to handle select call ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectCallRingWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    uint16                      cur_selection    = 0;
    uint32                      music_types      = 0;
    MMI_RESULT_E                result           = MMI_RESULT_TRUE;
    MMISET_CALL_RING_T          ring_info        = {MMISET_CALL_RING_FIXED, 0, 0};
    MMISET_CALL_MORE_RING_T     select_ring_info = {0};
    MULTIM_SELECT_RETURN_T      *win_param       = NULL;
    MMIENVSET_ADD_DATA_T*       add_data_ptr     =  (MMIENVSET_ADD_DATA_T*) MMK_GetWinAddDataPtr(win_id); 
    MMI_CTRL_ID_T               ctrl_id          = MMISET_RING_SELECT_CALL_CTRL_ID;
    MMIFMM_SELECT_INFO_T        select_info = {MMIFMM_MUSIC_ALL,0,0,0,PNULL,0,-1};
    MMI_STRING_T                ring                                   = {0};
    wchar                       file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      *ring_id_ptr = PNULL;
    uint32 text_id[MMISET_RING_FIXED_DEFAUT_NUM] =
    {TXT_COMMON_RING_RING1,TXT_COMMON_RING_RING2
#ifndef MMI_LOW_MEMORY_RING
    ,TXT_RING3,TXT_RING4,
    TXT_RING5,TXT_RING6,TXT_RING7,TXT_RING8
#endif
    };
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:     
        GUILIST_SetMaxItem(MMISET_RING_SELECT_CALL_CTRL_ID,MMISET_CALL_RING_MAX_CLASS, FALSE );//max item 2
        MMISET_AppendNVCallRingCalssItem(add_data_ptr->s_dual_sys, ctrl_id);/*lint !e64*/
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:        
        ring_info = MMIAPIENVSET_GetCallRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());/*lint !e64*/
        cur_selection = (uint16)ring_info.call_ring_type;     
        GUILIST_SetSelectedItem(MMISET_RING_SELECT_CALL_CTRL_ID,cur_selection,TRUE);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        switch (cur_selection)
        {
        case 0://固定铃声
            ring_info = MMIAPIENVSET_GetCallRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());/*lint !e64*/
            MMIAPISET_OpenFixCallRingWin(add_data_ptr->s_dual_sys, win_id, ring_info.fix_ring_index);/*lint !e64*/
            break;
            
        case 1:            
            ring_info = MMIAPISET_GetCallRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());  /*lint !e64*/ 
            //允许的来电铃声类型中不包括AMR         
			music_types = MMIFMM_MUSIC_ALL;//MMIFMM_MUSIC_MP3|MMIFMM_MUSIC_WMA|MMIFMM_MUSIC_MID|MMIFMM_MUSIC_ACC|MMIFMM_MUSIC_M4A|MMIFMM_MUSIC_WAV|MMIFMM_MUSIC_MIDI|MMIFMM_MUSIC_AMR;
#ifdef DRM_SUPPORT
             music_types |= MMIFMM_DRM_ALL; 
#endif
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = music_types;
            select_info.win_id = win_id;
            select_info.ring_vol = MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_CALL);
            
            //if volume is 0, play the music with volume 3
            if (0 == select_info.ring_vol)
            {
                select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
            }

            if(MMISET_CALL_RING_MORE_RING == ring_info.call_ring_type)
            {
                select_info.ticked_file_wstr = ring_info.more_ring_info.name_wstr;
                select_info.ticked_file_len= ring_info.more_ring_info.name_wstr_len;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectMusicWin(&select_info);
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len= 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectMusicWin(&select_info);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
           		limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
            break;
            
        default:
            //SCI_PASSERT(FALSE,("HandleSelectCallRingWindow: cur_selection %d is error!",cur_selection));
            break;
        }
        break;
        
        case MSG_MULTIM_SELECTED_RETURN :          
            win_param = (MULTIM_SELECT_RETURN_T *)param;
            select_ring_info.name_wstr_len = (uint16)MIN(win_param->file_name_len,MMISET_CALL_RING_NAME_MAX_LEN);
            
            MMIAPICOM_Wstrncpy( select_ring_info.name_wstr, win_param->file_name_ptr, select_ring_info.name_wstr_len );  
            
            if(MMIAPIENVSET_SetMoreCallRingInfo(add_data_ptr->s_dual_sys,select_ring_info,MMIENVSET_GetCurModeId()))/*lint !e64*/
            { //append set call ring list
                MMISET_AppendCallRingCalssItem(add_data_ptr->s_dual_sys, ctrl_id);/*lint !e64*/

                MMIAPIFMM_SplitFullPath(select_ring_info.name_wstr, select_ring_info.name_wstr_len,
                NULL, NULL, NULL, NULL,
                file_name, &ring.wstr_len);

                ring.wstr_ptr = file_name;
                GUILABEL_SetText(add_data_ptr->ctrl_id, &ring, FALSE);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }

            MMK_CloseWin( win_id );      
            //MMK_SendMsg(MMISET_RING_SELECT_CALL_WIN_ID,MSG_SET_SELECT_FIX_CALL_RING_RETURN,PNULL);
            break;

        case MSG_SET_SELECT_FIX_CALL_RING_RETURN:
            ring_id_ptr = (uint16 *)param;
            MMIAPIENVSET_SetFixedCallRingId(add_data_ptr->s_dual_sys,*ring_id_ptr,MMIENVSET_GetCurModeId());
            GUILABEL_SetTextById(add_data_ptr->ctrl_id, text_id[(*ring_id_ptr)-1], FALSE);
            MMK_CloseWin( win_id ); 
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );      
            break;
            
        case MSG_CLOSE_WINDOW:    
            MMK_FreeWinAddData(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : open select fixed ring window for phone book
//  Global resource dependence : 
//  Author:kelly.li
//  Note:             
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFixCallRingWin(
                                         MN_DUAL_SYS_E dual_sys,
                                         MMI_WIN_ID_T    win_id,
                                         uint16       selected_id
                                         )
{
    MMISET_SELECT_WIN_PARAM_T   *win_param_ptr  =   NULL;
    
    
    win_param_ptr = SCI_ALLOC_APP(sizeof(MMISET_SELECT_WIN_PARAM_T));
   // SCI_ASSERT(NULL != win_param_ptr);
    if(PNULL == win_param_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_OpenFixCallRingWin"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WINTAB_614_112_2_18_2_52_23_377,(uint8*)"");
        return;
    } 
    SCI_MEMSET(win_param_ptr, 0, sizeof(MMISET_SELECT_WIN_PARAM_T));

    win_param_ptr->selected_ring_id = selected_id;
    win_param_ptr->win_id = win_id;
    win_param_ptr->dual_sys = dual_sys;

    MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_FIXED_WIN_TAB,(ADD_DATA)win_param_ptr);

}

/*****************************************************************************/
//  Description : to handle select fixed msg ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectFixedMsgRingWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    uint32          dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		/* pause background sound */
		MMISRVAUD_ReqVirtualHandle("FIX MSG SELECT RING", MMISRVAUD_PRI_NORMAL);
        GUILIST_SetMaxItem(MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID,MMISET_MAX_RING_ID, FALSE );//max item 13
        AppendMsgRingItem(dual_sys,MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID);
        break;
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        break;
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP: 
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE:
#ifndef MMI_PDA_SUPPORT
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        MMISET_PlayRingByListItem(MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID,MMISET_RING_TYPE_MSG);
#endif
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //如果是list单击, 直接返回; 只响应双击
        if ( MMK_IsSingleClick(msg_id, param))
        {
#ifdef MMI_PDA_SUPPORT
            MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
            MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
            MMISET_PlayRingByListItem(MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID,MMISET_RING_TYPE_MSG);
#endif
            break;
        }
        else
        {
            if ( MSG_CTL_PENOK == msg_id && PNULL != param )
            {
                MMI_NOTIFY_T* notify_ptr = (MMI_NOTIFY_T*)param;
                if(notify_ptr->src_id == MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID)
                {
                    break;
                }
            }

        }
        
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        MMISET_SetMsgRingByRadioItem(dual_sys, MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID); /*lint !e64*/      
        MMK_CloseWin( win_id );
        //update msg ring type list
        //  MMK_SendMsg(MMISET_RING_SELECT_MSG_WIN_ID,MSG_FULL_PAINT,PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_GET_FOCUS:
        //MMIDEFAULT_EnableKeyRing(FALSE);
        //MMIDEFAULT_EnableTpRing(FALSE);    
        break;
        
    case MSG_LOSE_FOCUS:
        //stop ring
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);        
        break;
        
    case MSG_CLOSE_WINDOW:
        //stop ring
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
		/* resume background sound */
        MMISRVAUD_FreeVirtualHandle("FIX MSG SELECT RING");
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : to handle select fixed call ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectFixedCallRingWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{
    uint16                      cur_selection   =   0;
    uint16                      ring_id         =   0;
    MMI_RESULT_E                result          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_RING_SELECT_FIXED_CTRL_ID;
    MMISET_SELECT_WIN_PARAM_T   *win_param      =   NULL;
    
    win_param = (MMISET_SELECT_WIN_PARAM_T *)MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(PNULL != win_param);
    if(PNULL == win_param)
    {
        //SCI_TRACE_LOW:"mmisetting_assert HandleSelectFixedCallRingWindow"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WINTAB_754_112_2_18_2_52_23_378,(uint8*)"");
        return MMI_RESULT_FALSE;
    }     
    //SCI_TRACE_LOW:"[MMISET]:HandleSelectFixedCallRingWindow, msg = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WINTAB_757_112_2_18_2_52_23_379,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		/* pause background sound */  /*Bug 163146 后台播放音频，来电铃声设置,暂停后台播放*/
        MMISRVAUD_ReqVirtualHandle("FIX CALL SELECT RING", MMISRVAUD_PRI_NORMAL);  //bug163146
        GUILIST_SetMaxItem(ctrl_id,MMISET_RING_FIXED_DEFAUT_NUM, FALSE );//max item 8
        MMISET_AppendFixedCallRingItem(win_param->dual_sys, ctrl_id,win_param);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        break;       
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:

    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP: 
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE: 
 
#ifndef MMI_PDA_SUPPORT         
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        MMISET_PlayRingByListItem(ctrl_id,MMISET_RING_TYPE_CALL);
#endif 

        break;     
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //如果是list单击, 直接返回; 只响应双击
        if ( MMK_IsSingleClick(msg_id, param))
        {
#ifdef MMI_PDA_SUPPORT
            MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
            MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
            MMISET_PlayRingByListItem(ctrl_id,MMISET_RING_TYPE_CALL);
#endif
            break;
        }
        else
        {
            if ( MSG_CTL_PENOK == msg_id && PNULL != param )
            {
                MMI_NOTIFY_T* notify_ptr = (MMI_NOTIFY_T*)param;
                if(notify_ptr->src_id == MMISET_RING_SELECT_FIXED_CTRL_ID)
                {
                    break;
                }
            }

        }

        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        ring_id = (uint16)(cur_selection +1);
        
        if ( 0 != win_param->win_id)
        {
            MMK_SendMsg(win_param->win_id,MSG_SET_SELECT_FIX_CALL_RING_RETURN,(void*)&ring_id);
        }
        else
        {
            MMIAPIENVSET_SetFixedCallRingId(win_param->dual_sys,ring_id,MMIENVSET_GetCurModeId());
        }
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_GET_FOCUS:
        //MMIDEFAULT_EnableKeyRing(FALSE);
        //MMIDEFAULT_EnableTpRing(FALSE);       
        break;
        
    case MSG_LOSE_FOCUS:
        //stop ring
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);        
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);
        break;
        
    case MSG_CLOSE_WINDOW:
        //stop ring
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        /* resume background sound */ /*Bug 163146 后台播放音频，结束来电铃声设置,恢复后台播放*/
        MMISRVAUD_FreeVirtualHandle("FIX CALL SELECT RING");  //bug163146
        SCI_FREE(win_param);
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : to handle select message ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectMsgRingWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    uint16                  cur_selection       = 0;
    MMI_RESULT_E            result              = MMI_RESULT_TRUE;
    MMISET_MSG_RING_T       ring_info           = {MMISET_MSG_RING_FIXED, 0, 0};
    MMISET_CALL_MORE_RING_T select_ring_info    ={0};
    MULTIM_SELECT_RETURN_T  *win_param          = NULL;
    MMIENVSET_ADD_DATA_T*       add_data_ptr     =  (MMIENVSET_ADD_DATA_T*) MMK_GetWinAddDataPtr(win_id); 
    MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MUSIC_ALL,0,0,0,PNULL,0,-1};
    MMI_STRING_T                ring                                   = {0};
    wchar                       file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint32                      ring_id                                = 0;
    uint32 text_id[MMISET_MSG_FIXED_RING_MAX_ITEM] =
    {
        TXT_COMMON_RING_RING1,TXT_COMMON_RING_RING2
#ifndef MMI_LOW_MEMORY_RING
        ,TXT_RING3,TXT_RING4,
        TXT_RING5,TXT_RING6,
#endif
    };

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_RING_SELECT_MSG_CTRL_ID,MMISET_MSG_RING_MAX_CLASS, FALSE );//max item 2
        MMISET_AppendMsgRingClassItem(add_data_ptr->s_dual_sys, MMISET_RING_SELECT_MSG_CTRL_ID);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,MMISET_RING_SELECT_MSG_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        ring_info = MMIAPISET_GetMsgRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());/*lint !e64*/
        cur_selection = (uint16)ring_info.msg_ring_type;     
        GUILIST_SetSelectedItem(MMISET_RING_SELECT_MSG_CTRL_ID,cur_selection,TRUE);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(MMISET_RING_SELECT_MSG_CTRL_ID);
        switch (cur_selection)
        {
        case 0://固定铃声  
            MMK_CreatePubListWin((uint32*)MMISET_MSG_RING_SELECT_FIXED_WIN_TAB, (ADD_DATA)add_data_ptr->s_dual_sys);
            break;
            
        case 1:
            select_info.ring_vol = MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_MSG);
            //if volume is 0, play the music with volume 3
            if (0 == select_info.ring_vol)
            {
                select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
            }
            ring_info = MMIAPISET_GetMsgRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());/*lint !e64*/           
#ifdef DRM_SUPPORT
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
            if(MMISET_MSG_RING_MORE_RING == ring_info.msg_ring_type)
            {
                select_info.ticked_file_wstr = ring_info.more_ring_info.name_wstr;
                select_info.ticked_file_len= ring_info.more_ring_info.name_wstr_len;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectMusicWin(&select_info);
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len= 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectMusicWin(&select_info);
            }
#ifdef DRM_SUPPORT
            {
                 MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                 limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
           		 limit_value.is_rights_valid = 1;
                 MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
            break;
            
        default:
            //SCI_PASSERT(FALSE,("HandleSelectCallRingWindow: cur_selection %d is error!",cur_selection));
            break;
        }
        break;        
        
        case MSG_MULTIM_SELECTED_RETURN:
            win_param = (MULTIM_SELECT_RETURN_T *)param;
            select_ring_info.name_wstr_len = (uint16)MIN(win_param->file_name_len,MMISET_CALL_RING_NAME_MAX_LEN);
            MMIAPICOM_Wstrncpy( select_ring_info.name_wstr, win_param->file_name_ptr, select_ring_info.name_wstr_len );                 
            if(!MMISET_SetMoreMsgRingInfo(add_data_ptr->s_dual_sys, MMISET_RING_SELECT_MSG_CTRL_ID,select_ring_info))/*lint !e64*/
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            else
            {
                MMIAPIFMM_SplitFullPath(select_ring_info.name_wstr, select_ring_info.name_wstr_len,
                                NULL, NULL, NULL, NULL,
                                file_name, &ring.wstr_len);

                ring.wstr_ptr = file_name;
                GUILABEL_SetText(add_data_ptr->ctrl_id, &ring, FALSE);
            }
            MMK_CloseWin( win_id ); 
            break;
            
        case MSG_SET_SELECT_FIX_MSG_RING_RETURN:
            ring_id = (uint32)param;
            GUILABEL_SetTextById(add_data_ptr->ctrl_id, text_id[ring_id-1], FALSE);
            MMK_CloseWin( win_id );
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );      
            break;
            
        case MSG_CLOSE_WINDOW:
            MMK_FreeWinAddData(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 添加消息铃声radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
   
LOCAL void AppendMsgRingItem(
                             MN_DUAL_SYS_E  dual_sys,
                             MMI_CTRL_ID_T   ctrl_id
                             )
{
    #define LIST_STR_LEN    20 
    uint8               ring_vol      = 0;
    uint8               ring_id       = 0;
    uint16              cur_selection = 0;
    MMISET_MSG_RING_T   msg_ring_info = {MMISET_MSG_RING_FIXED, 0, 0};

    GUILIST_RemoveAllItems(ctrl_id);
    MMIAPISET_AppendFixedItems(
                                    TXT_RING,
                                    MMISET_MSG_FIXED_RING_MAX_ITEM,
                                    TXT_COMMON_OK,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    ctrl_id,
                                    GUIITEM_STYLE_ONE_LINE_RADIO
                                    );
    //get selected msg ring info
    msg_ring_info = MMIAPISET_GetMsgRingInfo(dual_sys,MMIENVSET_GetCurModeId());
    
    if (MMISET_MSG_RING_FIXED == msg_ring_info.msg_ring_type)
    {
        cur_selection = (uint16)(msg_ring_info.fix_ring_index - 1);
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
    }
    else
    {
        GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    }
    
    ring_id = (uint8)(cur_selection + 1);   
    
    //get ring volume
    ring_vol = MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_MSG);
    
    //if volume is 0, play the music with volume 3
    if (0 == ring_vol)
    {
        ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
    }
    
    //play ring
    MMIAPISET_PlayCallRingByVol(ring_vol,ring_id, 1, MMISET_RING_TYPE_MSG, PNULL);
}


/*****************************************************************************/
//  Description : 更新闹钟的设置
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_UpdateAlarmSet(void)
{
#if 0	/*时间更新统一处理*/
    //update alarm event
    MMIALM_UpdateEvents();
    
    //set alarm icon in idle
    if (MMIALM_HasActiveEvent())
    {
        MAIN_SetIdleAlarmState(TRUE);
    }
#endif
}






/*****************************************************************************/
//  Description : to open ring setting menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void EnterRingSettingMenu( void)
{
    MMK_CreateWin((uint32 *)MMISET_ICON_RING_MENU_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : to enter ring setting menu
//  Global resource dependence : 
//  Author:xiangjie.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_EnterRingSettingMenu( void)
{
    EnterRingSettingMenu(); 
}


/*****************************************************************************/
//  Description :铃声设置中所有label的设置
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void EnvSetRingParam(MMI_WIN_ID_T    win_id)
{
    uint16 loop = 0;
    MMISET_CALL_RING_T  call_ring_info ={0};
    MMISET_MSG_RING_T   msg_ring_info = {0};
    GUILABEL_INIT_DATA_T text_label_init = {0};
    GUIFORM_DYNA_CHILD_T  child_data = {0};
    GUIFORM_DYNA_CHILD_T  child_form_data = {0};
    GUIFORM_INIT_DATA_T     init_data = {0};
    MN_DUAL_SYS_E               dual_sys    =   MN_DUAL_SYS_MAX;
    MMI_STRING_T                ring                                   = {0};
    wchar                       file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};

    uint32 text_id[MMISET_RING_FIXED_DEFAUT_NUM] =
    {TXT_COMMON_RING_RING1,TXT_COMMON_RING_RING2
#ifndef MMI_LOW_MEMORY_RING
    ,TXT_RING3,TXT_RING4,
    TXT_RING5,TXT_RING6,TXT_RING7,TXT_RING8
#endif
    };
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    uint32 call_ring_title_id[]= {TXT_SET_RING_SELECT_CALL};
    uint32 msg_ring_title_id[] = {TXT_SET_RING_SELECT_MSG};
#else
    uint32 call_ring_title_id[]=
    {
        TXT_SET_RING_SIM1_SELECT_CALL,
        TXT_SET_RING_SIM2_SELECT_CALL,
        TXT_SET_RING_SIM3_SELECT_CALL,
        TXT_SET_RING_SIM4_SELECT_CALL
    };
    uint32 msg_ring_title_id[]=
    {
        TXT_SET_RING_SIM1_SELECT_MSG,
        TXT_SET_RING_SIM2_SELECT_MSG,
        TXT_SET_RING_SIM3_SELECT_MSG,
        TXT_SET_RING_SIM4_SELECT_MSG
    };
#endif
    for(loop = MN_DUAL_SYS_1; loop < 4 * MMI_DUAL_SYS_MAX; loop++)
    {
        if(0 == loop%2) //当loop = 0，2，4，6...时，创建一个可focus的子form
        {
            child_form_data.child_handle = MMISET_ENV_RING_CHILD_FORM_CTRL_ID + loop/2;
            child_form_data.guid = SPRD_GUI_FORM_ID;
            child_form_data.is_get_active = TRUE;
            child_form_data.is_bg       =   FALSE;
            init_data.layout_type = GUIFORM_LAYOUT_ORDER;
            child_form_data.init_data_ptr = &init_data;
            GUIFORM_CreatDynaChildCtrl(win_id, MMISET_ENV_RING_FORM_CTRL_ID, &child_form_data);
            GUIFORM_SetStyle(child_form_data.child_handle,GUIFORM_STYLE_UNIT);
        }
        //开始创建子form的子label：分为可active和不可active两种
        child_data.child_handle = MMISET_ENV_CALL_RING_LABEL_CTRL_ID + loop;
        child_data.guid = SPRD_GUI_LABEL_ID;
        if(0 == loop%2)//单数位置创建不可active的label
        {
            child_data.is_get_active = FALSE;
            text_label_init.align = GUILABEL_ALIGN_LEFT;
        }
        else if(1 == loop%2) //双数位置创建可active的label控件
        {
            child_data.is_get_active = TRUE;
            text_label_init.align = GUILABEL_ALIGN_DEFAULT;
        }
        child_data.is_bg       =   FALSE;
        child_data.init_data_ptr = &text_label_init;
        GUIFORM_CreatDynaChildCtrl(win_id, child_form_data.child_handle, &child_data);
        if(0 == loop%4)//0,4,8...的相对位置放着sim1,sim2..来电铃声的title
        {
            GUILABEL_SetTextById(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, call_ring_title_id[loop/4], FALSE);
        }
        else if(2 == loop%4)//2,6,10...的相对位置放着sim1,sim2..信息铃声的title
        {
            GUILABEL_SetTextById(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, msg_ring_title_id[loop/4], FALSE);
        } 
        
        dual_sys = MN_DUAL_SYS_1 + loop/4;
        call_ring_info = MMIAPISET_GetCallRingInfo(dual_sys,MMIENVSET_GetCurModeId());
        msg_ring_info = MMIAPISET_GetMsgRingInfo(dual_sys,MMIENVSET_GetCurModeId());
        
        if(1 == loop%4)//1，5，9...的相对位置放着sim1,sim2..来电铃声的信息
        {
            if(MMISET_CALL_RING_FIXED == call_ring_info.call_ring_type)
            {
                GUILABEL_SetTextById(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, text_id[call_ring_info.fix_ring_index - 1], FALSE);  
            }
            else if(MMISET_CALL_RING_MORE_RING == call_ring_info.call_ring_type)
            {
                MMIAPIFMM_SplitFullPath(call_ring_info.more_ring_info.name_wstr, call_ring_info.more_ring_info.name_wstr_len,
                                        NULL, NULL, NULL, NULL,
                                        file_name, &ring.wstr_len);
                
                ring.wstr_ptr = file_name;
                GUILABEL_SetText(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, &ring, FALSE);
            }
        }
        else if(3 == loop%4)//1，5，9...的相对位置放着sim1,sim2..信息铃声的信息
        {
            if(MMISET_MSG_RING_FIXED == msg_ring_info.msg_ring_type)
            {
                GUILABEL_SetTextById(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, text_id[msg_ring_info.fix_ring_index - 1], FALSE);  
            }
            else if(MMISET_MSG_RING_MORE_RING == msg_ring_info.msg_ring_type)
            {
                MMIAPIFMM_SplitFullPath(msg_ring_info.more_ring_info.name_wstr, msg_ring_info.more_ring_info.name_wstr_len,
                                        NULL, NULL, NULL, NULL,
                                        file_name, &ring.wstr_len);
                
                ring.wstr_ptr = file_name;
                GUILABEL_SetText(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, &ring, FALSE);
            }
        }
    }
}
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
/*****************************************************************************/
//  Description : create button layer.
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateButtonLayer(MMI_WIN_ID_T win_id)
{
    UILAYER_CREATE_T   create_info = {0};
    UILAYER_RESULT_E  result = UILAYER_RESULT_SUCCESS;
    uint16 layer_width = 0;
    uint16 layer_height = 0;
    
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);
    /*创建BUTTON使用的层*/
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = MAX(layer_width,layer_height);//因为只有第一次时初始化，之后可能进行横竖屏转换导致部分区域没有在layer内
    create_info.height          = MAX(layer_width,layer_height);//故设置layer的区域为横竖屏区域的并集。CR163796
    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = FALSE;   

    result = UILAYER_CreateLayer(&create_info, &s_set_button_layer_info);
    if(UILAYER_RESULT_SUCCESS == result)
    {
        UILAYER_APPEND_BLT_T  append_blt = {0};
        UILAYER_Clear(&s_set_button_layer_info);
        append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_set_button_layer_info;
        UILAYER_AppendBltLayer(&append_blt);         
        return TRUE;
    }
    else
    {
        s_set_button_layer_info.block_id = 0;
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : CreateButton
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void CreateButton(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id, GUI_BOTH_RECT_T both_rect)
{
    GUIBUTTON_INIT_DATA_T   init_data = {0};
    MMI_HANDLE_T handle = 0;
    GUI_FONT_ALL_T font_all = {0};
    GUI_BG_T btn_bg = {0};
    
    init_data.both_rect = both_rect;
    init_data.bg.bg_type = GUI_BG_IMG;
#ifdef MMI_PDA_SUPPORT                
    init_data.bg.img_id = IMAGE_PUBWIN_BUTTON_PDA;//IMAGE_COMMON_PICVIEW_BUTTON;
#else
    init_data.bg.img_id = IMAGE_SOFTKEY_BG;
#endif

    handle = GUIBUTTON_CreateDynamic(MMK_GetFocusWinId(), ctrl_id, &init_data);
    GUIBUTTON_SetTextId(handle, text_id);

    btn_bg.bg_type = GUI_BG_IMG;
#if defined(MMIPUBWIN_PDASTYLE_SUPPORT) || defined(GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT) || defined(GUIPOPMENU_POP_CONTEXT_SUPPORT) || defined(GUIF_EDIT_PDASTYLE) || defined(GUIF_SOFTKEY_PDASTYLE)
    btn_bg.img_id = IMAGE_PUBWIN_BUTTON_HL_PDA;    
#else
    btn_bg.img_id = IMAGE_SOFTKEY_BG_DOWN;    
#endif
    GUIBUTTON_SetPressedBg(handle, &btn_bg);
    GUIBUTTON_SetRunSheen(handle, FALSE);

#ifdef MMI_PDA_SUPPORT
        font_all.color = MMI_BLACK_COLOR;
#else
        font_all.color = MMI_WHITE_COLOR;
#endif
    font_all.font = MMI_DEFAULT_NORMAL_FONT;
    GUIBUTTON_SetFont(handle, &font_all);

}

/*****************************************************************************/
//  Description : MMIAPISET_CreatButtonLayer供静态墙纸和屏保或其它模块使用 
//  Global resource dependence :必须跟Destroy成对使用
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreatSetButton(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ok_ctrl_id,GUIBUTTON_CALLBACK_FUNC ok_call_back,GUIBUTTON_CALLBACK_FUNC cancel_call_back)
{
    MMI_CTRL_ID_T cancel_ctrl_id = MMISET_SET_CANCEL_BUTTON;
    uint16    button_icon_width = 0;
    uint16    button_icon_height = 0;
    uint16 layer_width = 0;
    uint16 layer_height = 0;
    GUI_RECT_T  rc = {0};
    GUI_BOTH_RECT_T  both_rect = {0};

#ifdef MMI_PDA_SUPPORT            
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_PUBWIN_BUTTON_PDA, win_id);
#else
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_SOFTKEY_BG, win_id);
#endif
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);

    if(layer_width < layer_height)
    {
    button_icon_width = layer_width/3 - 4;

    rc.left = ((layer_width/2) - button_icon_width)/2;
    rc.right = rc.left + button_icon_width;
    rc.bottom = layer_height - MMI_BUTTON_MARGIN;
    rc.top = rc.bottom - button_icon_height;
    both_rect.v_rect = rc;
        
        button_icon_width = layer_height/3 - 4;

        rc.left = ((layer_height/2) - button_icon_width)/2;
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_width - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
    both_rect.h_rect = rc;
    }
    else
    {
        button_icon_width = layer_width/3 - 4;

        rc.left = ((layer_width/2) - button_icon_width)/2;
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_height - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
        both_rect.h_rect = rc;
        
        button_icon_width = layer_height/3 - 4;

        rc.left = ((layer_height/2) - button_icon_width)/2;
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_width - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
        both_rect.v_rect = rc;
    }
    CreateButton(win_id, ok_ctrl_id, TXT_COMMON_OK, both_rect);
   IGUICTRL_SetState(MMK_GetCtrlPtr(ok_ctrl_id), GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
    if(layer_width < layer_height)
    {
        button_icon_width = layer_width/3 - 4;
    rc.left = ((layer_width/2) - button_icon_width)/2 + (layer_width/2);
    rc.right = rc.left + button_icon_width;
    rc.bottom = layer_height - MMI_BUTTON_MARGIN;
    rc.top = rc.bottom - button_icon_height;
    both_rect.v_rect = rc;
       
        button_icon_width = layer_height/3 - 4;
        rc.left = ((layer_height/2) - button_icon_width)/2 + (layer_height/2);
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_width - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
    both_rect.h_rect = rc;
    }
    else
    {
        button_icon_width = layer_width/3 - 4;
        rc.left = ((layer_width/2) - button_icon_width)/2 + (layer_width/2);
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_height - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
        both_rect.h_rect = rc;
       
        button_icon_width = layer_height/3 - 4;
        rc.left = ((layer_height/2) - button_icon_width)/2 + (layer_height/2);
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_width - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
        both_rect.v_rect = rc;
    }
    CreateButton(win_id, cancel_ctrl_id, STXT_CANCEL, both_rect);
    IGUICTRL_SetState(MMK_GetCtrlPtr(cancel_ctrl_id), GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
    GUIBUTTON_SetCallBackFunc(cancel_ctrl_id, cancel_call_back);    
    GUIBUTTON_SetCallBackFunc(ok_ctrl_id, ok_call_back);
}
/*****************************************************************************/
//  Description : 创建button显示的层，并设置控件到层上,与
//                    MMIAPISET_DestroySetButtonLayer成对调用
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateSetButtonLayer(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ok_ctrl_id)
{
    MMI_CTRL_ID_T cancel_ctrl_id = MMISET_SET_CANCEL_BUTTON;
    
    if(CreateButtonLayer(win_id))
    {
        MMK_SetCtrlLcdDevInfo(ok_ctrl_id, &s_set_button_layer_info);
        MMK_SetCtrlLcdDevInfo(cancel_ctrl_id, &s_set_button_layer_info);

    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO* MMIAPISET_GetButtonLayer()
{
    return &s_set_button_layer_info;
}
/*****************************************************************************/
//  Description : 当创建了这两个button的控件失去焦点时调用
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_DestroySetButtonLayer(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ok_ctrl_id)
{
    MMI_CTRL_ID_T cancel_ctrl_id = MMISET_SET_CANCEL_BUTTON;
    GUI_LCD_DEV_INFO default_lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    UILAYER_ReleaseLayer(&s_set_button_layer_info); 
    s_set_button_layer_info.lcd_id = 0;   
    s_set_button_layer_info.block_id = UILAYER_NULL_HANDLE;   
    MMK_SetCtrlLcdDevInfo(ok_ctrl_id, &default_lcd_dev);
    MMK_SetCtrlLcdDevInfo(cancel_ctrl_id, &default_lcd_dev);
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:juan.wu
//	Note: 
/*****************************************************************************/
PUBLIC  MMI_RESULT_E MMIAPISET_SetOkButtonCallback(void)
{
    return (MMI_RESULT_E)MMK_SendMsg(MMK_GetFocusWinHandle(), MSG_APP_OK, PNULL);
}

/*****************************************************************************/
//  Description : SetCancelButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_SetCancelButtonCallback(void)
{
    return (MMI_RESULT_E)MMK_CloseWin(MMK_GetFocusWinHandle());
}

/*****************************************************************************/
//  Description : MMISET_AppendButtonLayer
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 这个button layer在full paint的时候要append一下
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendSetButtonLayer(void)
{
    UILAYER_APPEND_BLT_T  append_blt = {0};
    
    if(UILAYER_IsLayerActive(&s_set_button_layer_info))
    {
        
        UILAYER_Clear(&s_set_button_layer_info);
        append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_set_button_layer_info;
        UILAYER_AppendBltLayer(&append_blt);         
    }
}

/*****************************************************************************/
//  Description : MMISET_AppendButtonLayer
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 这个button layer在button update时，clear一下层，变成透明
/*****************************************************************************/
PUBLIC void MMIAPISET_ClearButtonLayer(GUI_RECT_T update_rect)
{    
	if (UILAYER_IsLayerActive(&s_set_button_layer_info))
	{
		UILAYER_ClearRect(&s_set_button_layer_info, update_rect);
	}
}
#endif

/*****************************************************************************/
//  Description : handle flight mode win msg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleFlightmodeWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint32 i = 0;

            MMISET_StopPSApplication();

            MMIAPISET_SetFlyMode(TRUE);
            MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);

            for( i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                MAIN_SetIdleRoamingState(i, FALSE);
                MAIN_SetIdleForwardState(i, FALSE);
            }

            MMIAPIPHONE_DeactivePsAccordingToSetting();

            MMIPUB_CloseQuerytWin(PNULL);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);    
}

/*Edit by script, ignore 6 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
