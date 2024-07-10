/***************************************************************************
** File Name:      mmikm_wintab.c                                          *
** Author:                                                                 *
** Date:           07/14/2011                                               *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME              DESCRIPTION                          *
** 07/2011           tonny.chen        Create                              *
**																		   *
****************************************************************************/
/**------------------------------------------------------------------------*/
/**                         Include Files                                  */
/**------------------------------------------------------------------------*/
#include "mmi_app_kmovie_trc.h"
#include "mmk_type.h"
#include "guires.h"
#include "window_parse.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "mmipub.h"
#include "guistring.h"
#include "mmi_textfun.h"
#include "guiform.h"
//#include "guilabel.h"
#include "guiownerdraw.h"
#include "mmicom_string.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "guitext.h"
#include "guiiconlist.h"
#include "guilistbox.h"
#include "gui_ucs2b_converter.h"
#include "mmisd_export.h"
#include "mmikm_menutable.h"
#include "mmikm_id.h"
#include "mmikm_text.h"
#include "mmikm_internal.h"
#include "mmikm_export.h"
#include "mmikm_playlist.h"
#include "mmikm_text.h"
#include "mmifmm_interface.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define MMIKM_MAIN_MENU_ITEM_NUM           19
#define MMIKM_MAIN_MENU_ITEM_MAX_NUM       20

#define TYPE_NUM_STR_MAX_LEN               10
#define MMIKM_MAIN_MENU_ITEM_STR_MAX_LEN   GUILIST_STRING_MAX_NUM  //60, if 20 have issue 

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
LOCAL MMIKM_TYPE_E  s_selectd_km_type_e = MMIKM_TYPE_ACTION;
LOCAL uint16        s_selectd_delet_file_indx = 0;
LOCAL BOOLEAN   s_entry_last_played=FALSE;
/*****************************************************************************/
//  Description : load km type infor list control
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMovieTypeInfor(
                             uint16         type_num_index,  //in:索引
                             MMIKM_FILE_ITEM_INFO_T *file_item_info_ptr //out
                             );

/*****************************************************************************/
//  Description : to handle km main win menu list popmenu window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainWinPopWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : km main win
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         );

/*****************************************************************************/
//  Description : km show movie type information window  msg
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDispTypeInforWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         );
/*****************************************************************************/
//  Description : to handle km main win menu list popmenu window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDispTypeWinPopWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : disp more information win msg
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDispMoreInforWinMsg(
                                                 MMI_WIN_ID_T     win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM           param
                                         );

/*****************************************************************************/
// 	Description : create km app main window dynamic menu list 
//	Global resource dependence : 
//  Author:tonny.chen
//	Note: 
/*****************************************************************************/
LOCAL void CreateKMMainWinMenuList(
                    MMI_WIN_ID_T             win_id, //IN: list menu's window id
                    MMI_CTRL_ID_T            ctrl_id //IN: list menu's id
                               );

/*****************************************************************************/
//  Description : to create km main menu list popmenu window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CreateMainWinPopMenu(void);

/*****************************************************************************/
//  Description : to create km show movie type information window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CreateDispTypeInforWin(void);

/*****************************************************************************/
//  Description : to create km show movie type information window popmenu window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CreateDispTypeWinPopMenu(void);

/*****************************************************************************/
//  Description : to create km show more information window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CreateDispMoreInforWin(void);

/*****************************************************************************/
//  Description : create searching file wait win
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void  MainWinPopWinOpenMenuHandle(void);

/*****************************************************************************/
//  Description : to handle disp type pda mark opt window
//  Global resource dependence : 
//  Author:chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  DispTypePdaMarkOptHandle(
                                          MMI_CTRL_ID_T        list_ctrl_id, 
                                          MMI_MENU_ID_T    pop_menu_id
                                          );

/*****************************************************************************/
//  Description : SetDispTypeInfoOptMenuGreyStatus
//  Global resource dependence : 
//  Author:chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL void  SetDispTypeInfoOptMenuGreyStatus(void);
/*****************************************************************************/
//  Description : entryplaywin
//  Global resource dependence : 
//  Author:jinju.ma
//  Note: 
/*****************************************************************************/
LOCAL void DispPlayKmovieWin(void);

                 
#ifdef MMI_PDA_SUPPORT    
/*****************************************************************************/
//  Description : to handle km disp type pda popmenu window
//  Global resource dependence : 
//  Author:chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDispTypePdaWinPopWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : to create km show movie type information window pda popmenu window
//  Global resource dependence : 
//  Author:chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL void CreateDispTypeWinPdaPopMenu(void);
#endif

//mmikm main menu window table 
WINDOW_TABLE(MMIKM_MAIN_WIN_TAB) = 
{ 
    WIN_FUNC((uint32)HandleMainWinMsg),    
    WIN_ID(MMIKM_MAIN_WIN_ID),
    WIN_TITLE(TXT_KM),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN    
        
};

//main memu list popup menu wintab 
WINDOW_TABLE( MMIKM_MAIN_WIN_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMainWinPopWinMsg),//handle function
    WIN_ID(MMIKM_MAIN_WIN_POPMENU_WIN_ID), //win id
    WIN_STYLE( WS_HAS_TRANSPARENT), //style
    CREATE_POPMENU_CTRL(MMIKM_MENU_MAIN_WIN_POP, MMIKM_MAIN_WIN_POPMENU_CTRL_ID), //group id, ctrl id
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//mmikm display movie type infor window table 
WINDOW_TABLE(MMIKM_DISP_TYPE_INFOR_WIN_TAB) = 
{ 
    WIN_FUNC((uint32)HandleDispTypeInforWinMsg),    
    WIN_ID(MMIKM_DISP_TYPE_INFOR_WIN_ID),
    WIN_TITLE(TXT_KM),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIKM_FORM_CTRL_ID),   
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID, MMIKM_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIKM_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID),
#endif
    END_WIN       
};

//mmikm display movie type infor popup menu window table 
WINDOW_TABLE(MMIKM_DISP_TYPE_WIN_POPMENU_WIN_TAB) = 
{ 
    WIN_FUNC((uint32)HandleDispTypeWinPopWinMsg),    
    WIN_ID(MMIKM_DISP_TYPE_WIN_POPMENU_WIN_ID), //win id
    WIN_STYLE( WS_HAS_TRANSPARENT), //style
    CREATE_POPMENU_CTRL(MMIKM_DISP_TYPE_WIN_POP, MMIKM_DISP_TYPE_WIN_POPMENU_CTRL_ID), //group id, ctrl id
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN     
};

#ifdef MMI_PDA_SUPPORT    
//mmikm display movie type infor pda popup menu window table 
WINDOW_TABLE(MMIKM_DISP_TYPE_PDA_POPMENU_WIN_TAB) = 
{ 
    WIN_FUNC((uint32)HandleDispTypePdaWinPopWinMsg),    
    WIN_ID(MMIKM_DISP_TYPE_PDA_POPMENU_WIN_ID), //win id
    WIN_STYLE( WS_HAS_TRANSPARENT), //style
    CREATE_POPMENU_CTRL(MMIKM_DISP_TYPE_PDA_POPMENU, MMIKM_DISP_TYPE_PDA_POPMENU_CTRL_ID), //group id, ctrl id
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN     
};
#endif

//mmikm display movie more information window table 
WINDOW_TABLE(MMIKM_DISP_MORE_INFOR_WIN_TAB) = 
{ 
    WIN_FUNC((uint32)HandleDispMoreInforWinMsg),    
    WIN_ID(MMIKM_DISP_MORE_INFOR_WIN_ID), //win id
    WIN_TITLE(TXT_KM_MOVIE_INFO),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIKM_DISP_MORE_INFOR_CTRL_ID),
#ifdef MMI_KINGMOVIE_MINI_SUPPORT
    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER, MMIKM_DISP_THUMBNAIL_FILENAME_CTRL_ID, MMIKM_DISP_MORE_INFOR_CTRL_ID),
#else
    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS, MMIKM_DISP_THUMBNAIL_FILENAME_CTRL_ID, MMIKM_DISP_MORE_INFOR_CTRL_ID),
#endif
    CHILD_ANIM_CTRL(FALSE,FALSE, MMIKM_DISP_THUMBNAIL_CTRL_ID, MMIKM_DISP_THUMBNAIL_FILENAME_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE,MMIKM_DISP_FILENAME_CTRL_ID, MMIKM_DISP_THUMBNAIL_FILENAME_CTRL_ID),

    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER, MMIKM_DISP_DETAIL_INFOR_CTRL_ID, MMIKM_DISP_MORE_INFOR_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE, MMIKM_DISP_FILENAME_EXT_INFOR_CTRL_ID, MMIKM_DISP_DETAIL_INFOR_CTRL_ID),
    //CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE, MMIKM_DISP_TIME_INFOR_CTRL_ID, MMIKM_DISP_DETAIL_INFOR_CTRL_ID),
    //CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE, MMIKM_DISP_YEAR_INFOR_CTRL_ID, MMIKM_DISP_DETAIL_INFOR_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE, MMIKM_DISP_TIME_INFOR_CTRL_ID, MMIKM_DISP_DETAIL_INFOR_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE, MMIKM_DISP_YEAR_INFOR_CTRL_ID, MMIKM_DISP_DETAIL_INFOR_CTRL_ID),
     CHILD_TEXT_CTRL(FALSE, MMIKM_DISP_ACTOR_INFOR_CTRL_ID, MMIKM_DISP_DETAIL_INFOR_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE, MMIKM_DISP_SYNOP_INFOR_CTRL_ID, MMIKM_DISP_DETAIL_INFOR_CTRL_ID),
    
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN     
};

/*****************************************************************************/
// 	Description :  gui form dispaly animal content
//	Global resource dependence : 
//  Author:tonny.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN ShowFormAnimCtrl(MMIKM_FILE_ITEM_INFO_T *file_item_info_ptr)
{
    BOOLEAN recode = FALSE;
    uint16 width = 0;
    uint16 height = 0;
    GUIANIM_CTRL_INFO_T         control_info = {0};
    //GUIANIM_CTRL_T              guianim_ctrl_info = {0};
    GUIANIM_FILE_INFO_T         file_info = {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    uint16  thumbnail_file_full_name_len=0;
    wchar   thumbnail_file_full_name[FMM_SEARCH_FILENAME_MAXLEN+1]={0};


    
    //SCI_TRACE_LOW:"[KM]ShowFormAnimCtrl entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_298_112_2_18_2_31_23_89,(uint8*)"");
    if(NULL == file_item_info_ptr)
    {
        //SCI_TRACE_LOW:"[KM]ShowFormAnimCtrl file_item_info_ptr = NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_301_112_2_18_2_31_23_90,(uint8*)"");
        return recode;
    }
    GetThumbnailImageByFileName(file_item_info_ptr->file_full_path_name,file_item_info_ptr->file_info_name,thumbnail_file_full_name,&thumbnail_file_full_name_len);
   if(MMIAPIFMM_IsFileExist(thumbnail_file_full_name,thumbnail_file_full_name_len))//cr141075
    {
    GUIANIM_GetImgWidthHeight(&width, &height,
                                                        thumbnail_file_full_name, 
                                                                thumbnail_file_full_name_len);
    }
    //SCI_TRACE_LOW:"[KM]ShowFormAnimCtrl width = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_311_112_2_18_2_31_23_91,(uint8*)"d", width);
    //SCI_TRACE_LOW:"[KM]ShowFormAnimCtrl height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_312_112_2_18_2_31_23_92,(uint8*)"d", height);
    if(width > KM_FILE_INFO_THUMBNAIL_MAX_W)
    {
        width = KM_FILE_INFO_THUMBNAIL_MAX_W;  
    }
    if(height > KM_FILE_INFO_THUMBNAIL_MAX_H)
    {
        height = KM_FILE_INFO_THUMBNAIL_MAX_H; 
    }
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    child_width.add_data = width;
    GUIFORM_SetChildWidth(MMIKM_DISP_THUMBNAIL_FILENAME_CTRL_ID, MMIKM_DISP_THUMBNAIL_CTRL_ID, &child_width);

    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    child_height.add_data = height;
    GUIFORM_SetChildHeight(MMIKM_DISP_THUMBNAIL_FILENAME_CTRL_ID, MMIKM_DISP_THUMBNAIL_CTRL_ID, &child_height);
    //set anim 

    file_info.full_path_wstr_ptr = thumbnail_file_full_name;
    file_info.full_path_wstr_len = MMIAPICOM_Wstrlen(thumbnail_file_full_name);  


    //guianim_ctrl_info.img_height = height + KM_FILE_INFO_THUMBNAIL_ENLARGE_SIZE;
    //guianim_ctrl_info.img_width = width + KM_FILE_INFO_THUMBNAIL_ENLARGE_SIZE;
    
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIKM_DISP_THUMBNAIL_CTRL_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_LEFT_TOP;
    GUIANIM_SetParam(&control_info, PNULL, &file_info, &display_info);

    //GUIANIM_SetImgWidthHeight(MMIKM_DISP_THUMBNAIL_CTRL_ID, width + KM_FILE_INFO_THUMBNAIL_ENLARGE_SIZE, height + KM_FILE_INFO_THUMBNAIL_ENLARGE_SIZE);


    recode = TRUE;
    return recode;
    
}

/*****************************************************************************/
// 	Description :  gui form dispaly lable content
//	Global resource dependence : 
//  Author:tonny.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN ShowFormLableCtrl(MMIKM_FILE_ITEM_INFO_T *file_item_info_ptr)
{
    uint16 text_len = 0;
    uint16 char_infor_len = 0;
    BOOLEAN recode = FALSE;
    wchar *actor_wchar = NULL;
    wchar *synop_wchar = NULL;
    MMI_STRING_T  string_text = {0};
    
    wchar text_buf[KM_FILE_INFO_SYNOPSIS_LEN_MAX + KM_FILE_INFO_YEAR_LEN_MAX] = {0};

    KMV_INFORMATION_T kmv_infor_t={0};
    uint8                       file_info_actor[KM_FILE_INFO_ACTOR_LEN_MAX+1] = {0};
    uint8                       file_info_synop[KM_FILE_INFO_SYNOPSIS_LEN_MAX+1] = {0};


#if 0//#ifdef KM_DEBUG_SWITCH_ENABLE
    char char_temp[KM_FILE_INFO_SYNOPSIS_LEN_MAX + KM_FILE_INFO_YEAR_LEN_MAX +1] = {0}; 
#endif
    
    if(NULL == file_item_info_ptr)
    {
        return recode;
    }

    //set file name infor text  
    recode = GUITEXT_SetString(MMIKM_DISP_FILENAME_CTRL_ID, file_item_info_ptr->file_info_name, 
                      file_item_info_ptr->file_info_name_len, FALSE);

    //set file name ext infor text
    MMI_GetLabelTextByLang(TXT_INPUT_FILE_NAME, &string_text);
    MMIAPICOM_Wstrncpy(text_buf, string_text.wstr_ptr, string_text.wstr_len);

    MMIAPICOM_Wstrncat(text_buf, file_item_info_ptr->file_info_name, file_item_info_ptr->file_info_name_len);

#if 0//#ifdef KM_DEBUG_SWITCH_ENABLE
    MMIAPICOM_WstrToStr(text_buf, char_temp);
#endif

    text_len = MMIAPICOM_Wstrlen(text_buf); 
    GUITEXT_SetString(MMIKM_DISP_FILENAME_EXT_INFOR_CTRL_ID, text_buf, text_len, FALSE);                

    //set time infor text
    SCI_MEMSET(text_buf, 0x00, KM_FILE_INFO_SYNOPSIS_LEN_MAX + KM_FILE_INFO_YEAR_LEN_MAX);
    MMI_GetLabelTextByLang(TXT_KM_TIME, &string_text);
    MMIAPICOM_Wstrncpy(text_buf, string_text.wstr_ptr, string_text.wstr_len);

    MMIAPICOM_Wstrncat(text_buf, file_item_info_ptr->file_info_time, MMIAPICOM_Wstrlen(file_item_info_ptr->file_info_time));

    MMI_GetLabelTextByLang(TXT_KM_MINS, &string_text);
    MMIAPICOM_Wstrncat(text_buf, string_text.wstr_ptr, string_text.wstr_len);
    
    //lable_text.wstr_ptr = text_buf;
    //lable_text.wstr_len = MMIAPICOM_Wstrlen(text_buf);   
    //GUILABEL_SetText(MMIKM_DISP_TIME_INFOR_CTRL_ID, &lable_text, FALSE);
    GUITEXT_SetString(MMIKM_DISP_TIME_INFOR_CTRL_ID, text_buf, MMIAPICOM_Wstrlen(text_buf), FALSE);  
    

    //set year infor text
    SCI_MEMSET(text_buf, 0x00, KM_FILE_INFO_SYNOPSIS_LEN_MAX + KM_FILE_INFO_YEAR_LEN_MAX);
    MMI_GetLabelTextByLang(TXT_KM_INFO_YEAR, &string_text);
    MMIAPICOM_Wstrncpy(text_buf, string_text.wstr_ptr, string_text.wstr_len);

    MMIAPICOM_Wstrncat(text_buf, file_item_info_ptr->file_info_year, MMIAPICOM_Wstrlen(file_item_info_ptr->file_info_year));

    //lable_text.wstr_ptr = text_buf;
    //lable_text.wstr_len = MMIAPICOM_Wstrlen(text_buf);   
    //GUILABEL_SetText(MMIKM_DISP_YEAR_INFOR_CTRL_ID, &lable_text, FALSE);
    GUITEXT_SetString(MMIKM_DISP_YEAR_INFOR_CTRL_ID, text_buf, MMIAPICOM_Wstrlen(text_buf), FALSE);  


     
    MMIAPIKM_CkeckSupportfile((wchar*)file_item_info_ptr->file_full_path_name);

    //get kmv file content    //smv_support
    if(MMIAPISM_IsSupportfile())
    {
      SCI_TRACE_LOW("[KM] ShowFormLableCtrl THIS IS SMV FILE");
      MMIKM_GetSmvFileContent(file_item_info_ptr->file_full_path_name, &kmv_infor_t);
    }
    else
    {
        SCI_TRACE_LOW("[KM] ShowFormLableCtrl THIS IS HMV FILE");
        MMIKM_GetKmvFileContent(file_item_info_ptr->file_full_path_name, &kmv_infor_t); 
    }

                              

 //get file actor
    if(kmv_infor_t.item_actor.item_is_true)
    {
        //utf8 编码
        if(kmv_infor_t.item_actor.item_size > KM_FILE_INFO_ACTOR_LEN_MAX)
        {
            kmv_infor_t.item_actor.item_size = KM_FILE_INFO_ACTOR_LEN_MAX;
        }
        //SCI_TRACE_LOW:"[KM] GetFileItemInfor:actor len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_438_112_2_18_2_31_23_94,(uint8*)"d", kmv_infor_t.item_actor.item_size);
        strncpy((char*)file_info_actor, (char *)kmv_infor_t.item_actor.item_buffer_ptr, kmv_infor_t.item_actor.item_size);
    }
    //get file synop
    if(kmv_infor_t.item_synop.item_is_true)
    {
        //utf8 编码
        if(kmv_infor_t.item_synop.item_size > KM_FILE_INFO_SYNOPSIS_LEN_MAX)
        {
            kmv_infor_t.item_synop.item_size = KM_FILE_INFO_SYNOPSIS_LEN_MAX;
        }
        //SCI_TRACE_LOW:"[KM] GetFileItemInfor:synop len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_449_112_2_18_2_31_23_95,(uint8*)"d", kmv_infor_t.item_synop.item_size);
        strncpy((char*)file_info_synop, (char *)kmv_infor_t.item_synop.item_buffer_ptr, kmv_infor_t.item_synop.item_size);
    }
    
    //set actor infor text
    SCI_MEMSET(text_buf, 0x00, KM_FILE_INFO_SYNOPSIS_LEN_MAX + KM_FILE_INFO_YEAR_LEN_MAX);
    MMI_GetLabelTextByLang(TXT_KM_INFO_STARRING, &string_text);
    MMIAPICOM_Wstrncpy(text_buf, string_text.wstr_ptr, string_text.wstr_len);


    char_infor_len = strlen((char *)file_info_actor);


    //SCI_TRACE_LOW:"[KM]ShowFormLableCtrl acotr len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_462_112_2_18_2_31_23_96,(uint8*)"d", char_infor_len);
    if(char_infor_len > 0)
    {
        text_len = KM_FILE_INFO_ACTOR_LEN_MAX + 1;
        actor_wchar = SCI_ALLOC_APP(sizeof(wchar)*text_len);
        if(NULL != actor_wchar)
        {
           SCI_MEMSET(actor_wchar, 0x00, sizeof(wchar)*text_len); 
           text_len = GUI_UTF8ToWstr(actor_wchar, 
                           (uint32)text_len, 
                            (const uint8 *)file_info_actor,
                           (uint32)char_infor_len);

            MMIAPICOM_Wstrncat(text_buf, actor_wchar, text_len);
            SCI_FREE(actor_wchar);
            actor_wchar = NULL;   
        }
    }

    text_len = MMIAPICOM_Wstrlen(text_buf); 
    //SCI_TRACE_LOW:"[KM]ShowFormLableCtrl final acotr len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_482_112_2_18_2_31_23_97,(uint8*)"d", text_len);
    GUITEXT_SetString(MMIKM_DISP_ACTOR_INFOR_CTRL_ID, text_buf, text_len, FALSE);  
    
    //set synop infor text
    SCI_MEMSET(text_buf, 0x00, KM_FILE_INFO_SYNOPSIS_LEN_MAX + KM_FILE_INFO_YEAR_LEN_MAX);
    MMI_GetLabelTextByLang(TXT_KM_INFO_SYNOPSIS, &string_text);
    MMIAPICOM_Wstrncpy(text_buf, string_text.wstr_ptr, string_text.wstr_len);


     char_infor_len = strlen((char*)file_info_synop);

    //SCI_TRACE_LOW:"[KM]ShowFormLableCtrl synop len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_493_112_2_18_2_31_23_98,(uint8*)"d", char_infor_len);
    if(char_infor_len > 0)
    {
        text_len = KM_FILE_INFO_SYNOPSIS_LEN_MAX + 1;
        synop_wchar = SCI_ALLOC_APP(sizeof(wchar)*text_len);
        if(NULL != synop_wchar)
        {
            SCI_MEMSET(synop_wchar, 0x00, sizeof(wchar)*text_len);
            text_len = GUI_UTF8ToWstr(synop_wchar, 
                            (uint32)text_len, 
                             (const uint8 *)file_info_synop,
                                     (uint32)char_infor_len);
             
            text_len = MMIAPICOM_Wstrlen(synop_wchar); 
            MMIAPICOM_Wstrncat(text_buf, synop_wchar, text_len);
             SCI_FREE(synop_wchar);
             synop_wchar = NULL;   
        }
    }
    text_len = MMIAPICOM_Wstrlen(text_buf); 
    GUITEXT_SetString(MMIKM_DISP_SYNOP_INFOR_CTRL_ID, text_buf, text_len, FALSE);  
    
    if(PNULL != kmv_infor_t.data_buf_ptr)
    {
        SCI_FREE(kmv_infor_t.data_buf_ptr);
        kmv_infor_t.data_buf_ptr = PNULL;
    }
    recode = TRUE;
    return recode;
}

/*****************************************************************************/
//  Description : disp more information win msg
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDispMoreInforWinMsg(
                                                 MMI_WIN_ID_T     win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM           param
                                         )
{
    uint16 cur_select_list = 0;
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id =  MMIKM_DISP_MORE_INFOR_CTRL_ID;
    
    MMIKM_FILE_ITEM_INFO_T km_file_item_info = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        cur_select_list = GUILIST_GetCurItemIndex(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID);
        if(!GetMovieTypeInfor(cur_select_list, &km_file_item_info) )
        {
            break;
        }
        GUIFORM_SetStyle(ctrl_id, GUIFORM_STYLE_NORMAL);
        ShowFormAnimCtrl(&km_file_item_info);
        ShowFormLableCtrl(&km_file_item_info);
        MMK_SetAtvCtrl(win_id, ctrl_id);        
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  搜素本地播放列表等待窗口处理函数
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandlePlaylistSearchWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;      

    static KM_PLAYLIST_TYPE_E   locallist_type = KM_PLAYLIST_DEFAULT;
#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT 
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
#else
    MMIFILE_DEVICE_E file_dev = MMIKM_GetPlayListDevice();
#endif
    MMIFILE_DEVICE_E* file_dev_ptr = PNULL;
    MMIFILE_ERROR_E   file_error = SFS_ERROR_NONE;
#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT        
    static MMIFILE_DEVICE_E search_file_dev = MMI_DEVICE_UDISK;
#endif
    //SCI_TRACE_LOW:"HandlePlaylistSearchWaitingWin entry!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_600_112_2_18_2_31_24_99,(uint8*)"");
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);        
        break;

    case MSG_CTL_CANCEL: 
    case MSG_APP_CANCEL:  
 
        MMIAPIFMM_SearchFileStop();
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        MMIPUB_CloseWaitWin(win_id); 
        MMIAPIKM_FileArrayDestroy();
        if(!MMK_IsOpenWin(MMIKM_MAIN_WIN_ID))
        {
            MMIKM_ReleaseMemoForPlaylistInfo();  
        }
        else
        {
            MMIKM_ReadFile();
        }
           
        break;

    case MSG_APP_RED:
        MMIAPIFMM_SearchFileStop();
        MMIAPIKM_FileArrayDestroy();
        MMIKM_ReleaseMemoForPlaylistInfo();
        result = MMI_RESULT_FALSE;
        break;

    case MSG_KM_LOADING_BEGIN:
        file_dev_ptr = (MMIFILE_DEVICE_E*)param;
        if(PNULL != file_dev_ptr && file_dev == *file_dev_ptr)
        {
            if(!MMIKM_PlayList_BeginLoad(win_id, MSG_KM_LOADING_BEGIN,locallist_type,file_dev)) //开始加载默认路径默认播放列表
            {   
                MMK_PostMsg(win_id,MSG_KM_LOADING_END,PNULL,PNULL);  
            }
        }
        else
        {
#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT        
            if(search_file_dev == MMI_DEVICE_UDISK)
            {
                search_file_dev = MMI_DEVICE_SDCARD;
                if(!MMIAPISD_GetStatus(search_file_dev))
                {
                    search_file_dev = MMI_DEVICE_SDCARD_1;
                }
            }
            else if(search_file_dev == MMI_DEVICE_SDCARD)
            {
                search_file_dev = MMI_DEVICE_SDCARD_1;
            }  
            
            if((MMI_DEVICE_SDCARD == search_file_dev) && MMIAPISD_GetStatus(search_file_dev))
            {
                if(!MMIKM_PlayList_BeginLoad(win_id, MSG_KM_LOADING_BEGIN,locallist_type,search_file_dev)) //开始加载sd卡默认播放列表
                {   
                    MMK_PostMsg(win_id,MSG_KM_LOADING_END,PNULL,PNULL);
                }
            }
            else if((MMI_DEVICE_SDCARD_1 == search_file_dev) && MMIAPISD_GetStatus(search_file_dev))
            {
                if(!MMIKM_PlayList_BeginLoad(win_id, MSG_KM_LOADING_END,locallist_type,search_file_dev)) //开始加载sd2卡默认播放列表
                {   
                    MMK_PostMsg(win_id,MSG_KM_LOADING_END,PNULL,PNULL);
                }
            }
            else
            {
                MMK_PostMsg(win_id,MSG_KM_LOADING_END,PNULL,PNULL);
            }           
#else  
            MMIFILE_DEVICE_E dev2 = MMI_DEVICE_SYSTEM;
            
            if(file_dev == MMI_DEVICE_SDCARD)
            {
                dev2 = MMI_DEVICE_SDCARD_1;
            }
            else if(file_dev == MMI_DEVICE_SDCARD_1)
            {
                dev2 = MMI_DEVICE_SDCARD;
            }
            if(dev2 != MMI_DEVICE_SYSTEM && MMIAPIFMM_GetDeviceTypeStatus(dev2))
            {
                if(!MMIKM_PlayList_BeginLoad(win_id, MSG_KM_LOADING_END,locallist_type,dev2)) //开始加载其他sd卡默认播放列表
                {   
                    MMK_PostMsg(win_id,MSG_KM_LOADING_END,PNULL,PNULL);
                }
            }
            else
            {
                MMK_PostMsg(win_id,MSG_KM_LOADING_END,PNULL,PNULL);
            }
#endif              
        }        
        break;

    case MSG_KM_LOADING_END:
        {
            MMIKM_DoubleCardFileArrayCombine();
#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT            
            MMIAPIKM_UdiskTcardFileArrayCombine();
            search_file_dev = MMI_DEVICE_UDISK;
#endif            
            file_error = MMIKM_PlayList_EndLoad(locallist_type); //默认播放列表加载完毕
            if (SFS_ERROR_NO_SPACE == file_error)
            {       
                if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD) || !MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD_1))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SD_CARD_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_DISK_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                }
                MMIPUB_CloseWaitWin(win_id); 
                break; 
            }
            MMIKM_PlayList_AllItem_Load();
        }
        break;

    case MSG_KM_LOADING_SAVE:
        {
            MMIKM_LOAD_SAVE_INFO_T* save_info_ptr = (MMIKM_LOAD_SAVE_INFO_T*)param;
            MMIKM_LOAD_SAVE_INFO_T save_info = {0};
            uint32  item_file_size=0;
            //SCI_TRACE_LOW:"HandlePlaylistSearchWaitingWin MSG_KM_LOADING_SAVE!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_731_112_2_18_2_31_24_100,(uint8*)"");
            save_info.cur_list_index = save_info_ptr->cur_list_index;
            save_info.list_num = save_info_ptr->list_num;
            save_info.list_type = save_info_ptr->list_type;
            MMIKM_PlayListGetItem(save_info.list_type, &save_info,&item_file_size);//andy.he_cr122288 
            
            if(save_info.cur_list_index < (save_info.list_num - 1) && save_info.list_num !=0 &&(save_info.cur_list_index<KM_FILE_INFO_NUM_MAX-1))
            {
              save_info.list_num=  MMIKM_GetPlaylistNum(KM_PLAYLIST_DEFAULT); 
              if(item_file_size != 0)
                {
                      save_info.cur_list_index++;
                } 
                MMIAPICOM_OtherTaskToMMI(win_id,MSG_KM_LOADING_SAVE, &save_info, sizeof(MMIKM_LOAD_SAVE_INFO_T));
            }
            else
            {
		        MMIAPIKM_FileArrayDestroy();
                if(!MMK_IsOpenWin(MMIKM_MAIN_WIN_ID))
                {
#ifdef MMI_KEY_LOCK_SUPPORT
					if(MMIAPIKL_IsPhoneLocked())
					{
						MMIKM_CreateMainWin();
						MMIAPIKL_LockPhone();
					}
					else
#endif
					{
                     	MMIKM_CreateMainWin();
					}
                } 
                else
                {   //full paint, update date
                    MMK_DestroyDynaCtrl(MMIKM_MAIN_WIN_MENU_CTRL_ID);
                    //创建窗口的菜单
                    CreateKMMainWinMenuList(MMIKM_MAIN_WIN_ID, MMIKM_MAIN_WIN_MENU_CTRL_ID);
                }
                MMIPUB_CloseWaitWin(win_id);
            }
        }

        break;
   
    case MSG_GET_FOCUS:
       
        break;

    case MSG_CLOSE_WINDOW:
        if(!MMK_IsOpenWin(MMIKM_MAIN_WIN_ID))
        {
             MMIKM_ReleaseMemoForPlaylistInfo();
        }
        MMIAPIKM_FileArrayDestroy();
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : 删除选中的文件和列表信息
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeltSelectdFileInfor(uint16* cur_indx_ptr, uint16 delete_num)
{
    
    BOOLEAN recode = FALSE;
    MMIKM_TYPE_E km_type_enum = MMIKM_TYPE_MAX;

    km_type_enum = s_selectd_km_type_e; 

    if(MMIKM_DeltMovieTpyeInfor(cur_indx_ptr, delete_num, km_type_enum) )
    {
        //s_selectd_delet_file_indx = cur_indx - 1;
        MMK_PostMsg(MMIKM_DISP_TYPE_INFOR_WIN_ID, MSG_KM_UPDATE_LIST, NULL, NULL);  

        MMK_PostMsg(MMIKM_MAIN_WIN_ID, MSG_KM_UPDATE_MAIN_WIN, PNULL, PNULL);
        recode = TRUE;
    }
    return recode;
    
}
/*****************************************************************************/
//  Description : open wait win
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void KMOpenWaitWin(MMIPUB_HANDLE_FUNC  func)
{
    MMI_STRING_T wait_text = {0};
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,
        &wait_text,
        PNULL,
        PNULL,
        MMIKM_WAITING_WIN_ID,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_ONE,
        (MMIPUB_HANDLE_FUNC)func);
}

/*****************************************************************************/
//  Description : HandleDelFileQueryWin
//  Global resource dependence :                                
//      Author: tonny.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeltFileQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint16 delete_num = 0;
    uint16 cur_index[KM_FILE_INFO_NUM_MAX] = {0};
   
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
          
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            KMOpenWaitWin(PNULL);            
            if(GUILIST_GetListStateById(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,GUILIST_STATE_ENABLE_MARK))
            {
                delete_num = GUILIST_GetSelectedItemIndex(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,cur_index,KM_FILE_INFO_NUM_MAX);
                GUILIST_SetTextListMarkable(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,FALSE);
            }
            else
            {
                delete_num = 1;
                cur_index[0] = GUILIST_GetCurItemIndex(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID);
            }                   
            DeltSelectdFileInfor(cur_index, delete_num);  
#ifdef MMI_PDA_SUPPORT
			GUIFORM_SetChildDisplay(MMIKM_FORM_CTRL_ID,MMICOMMON_SOFTKEY_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
#endif
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            MMK_CloseWin(MMIKM_WAITING_WIN_ID);
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }

    return result;
}

/*****************************************************************************/
//  Description : to delete select file in list 
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeltSelectdFile(void)
{
    MMI_WIN_ID_T  query_win_id = MMIKM_QUERY_WIN_ID;
    MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id, HandleDeltFileQueryWin);
    return TRUE;
}

/*****************************************************************************/
//  Description : to handle km main win menu list popmenu window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDispTypeWinPopWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    uint16    cur_select_list_id = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIKM_DISP_TYPE_WIN_POPMENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T pop_menu_id = 0;
    MMIKM_FILE_ITEM_INFO_T km_file_item_info = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        SetDispTypeInfoOptMenuGreyStatus();
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&pop_menu_id);
        cur_select_list_id = GUILIST_GetCurItemIndex(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID);
        GetMovieTypeInfor(cur_select_list_id, &km_file_item_info);
        switch(pop_menu_id)
        {
            case ID_PLAY:
                DispPlayKmovieWin();
				// {
// #ifdef KM_DEBUG_SWITCH_ENABLE
//                     uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
//                     MMIAPICOM_WstrToStr((const wchar*)km_file_item_info.file_full_path_name, char_filename);
// #endif
//                     if(km_file_item_info.is_valid && 0 != MMIAPICOM_Wstrlen(km_file_item_info.file_full_path_name))
//                     {
//                         //MMIAPIKM_SetSupportfileFlag(TRUE);
//                         MMIAPIKM_CkeckSupportfile(km_file_item_info.file_full_path_name);//smv_support
// #ifdef VIDEO_PLAYER_SUPPORT
// #ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
//                 MMIAPIVP_MiniFunction_PlayVideo(km_file_item_info.file_full_path_name,
//                                            km_file_item_info.file_full_path_len);
// #else
//                         MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_FILE,km_file_item_info.file_full_path_name,
//                                            km_file_item_info.file_full_path_len, PNULL);
// #endif
// #endif
//                         MMIKM_SetLastPlayInfor(km_file_item_info.file_full_path_name, 
//                                                km_file_item_info.file_full_path_len, 0);
//                                                
//                     }
//                 }
                break;
            case ID_DELETE:
                DeltSelectdFile();
                
                break;
            case ID_MORE_INFO:
                CreateDispMoreInforWin();
                break;

            case ID_KM_SUB_MARK:
            case ID_KM_CANCEL_MARK:
            case ID_KM_MARK_ALL:
            case ID_KM_CANCEL_ALL_MARK:
                DispTypePdaMarkOptHandle(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID, pop_menu_id);
                break;
                
            default:
                break;
        }
        MMK_CloseWin(win_id);
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

/*****************************************************************************/
//  Description : to handle km main win menu list popmenu window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainWinPopWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIKM_MAIN_WIN_POPMENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T pop_menu_id = 0;
    MMI_MENU_ID_T selecte_main_menu_id = 0;
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        selecte_main_menu_id = GUIMENU_GetCurNodeId(MMIKM_MAIN_WIN_MENU_CTRL_ID);
        GUIMENU_GetId(ctrl_id,&group_id,&pop_menu_id);
        switch (pop_menu_id)
        {
            case ID_KM_OPEN:
                {
                    if(ID_LAST_PLAYED == selecte_main_menu_id)
                    {
                         MMIKM_PLAY_INFO_T km_play_info = {0};
                         if(MMIKM_GetLastPlayInfor(&km_play_info))
                         {
                              MMIAPIKM_SetLastPlayOpenWin(TRUE);  
                              //MMIAPIKM_SetSupportfileFlag(TRUE);
                              MMIAPIKM_CkeckSupportfile(km_play_info.cur_file_full_path_name);//smv_support

#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
                MMIAPIVP_MiniFunction_PlayVideo(km_play_info.cur_file_full_path_name,
                                                 km_play_info.cur_file_full_path_len);
#else
                              MMIAPIVP_PlayVideoFromVideoPlayerEx(VP_ENTRY_FROM_FILE,km_play_info.cur_file_full_path_name,
                                                 km_play_info.cur_file_full_path_len, PNULL,MMIAPIKM_GetLastPorcesstime(),MMIKM_SetLastPlayInfor);
#endif
#endif
                              MMIKM_SetLastPlayInfor(km_play_info.cur_file_full_path_name, km_play_info.cur_file_full_path_len, 0);
                                                    
                         }
                    }
                    else
                    {
                        //创建影片某种类型显示窗口
                        s_selectd_km_type_e = selecte_main_menu_id - ID_TYPE_ACTION;
                        CreateDispTypeInforWin();
                    }
                    MMK_CloseWin(win_id);
                }
                break;
            case ID_KM_UPDATE_LIST:
                {
                    s_selectd_km_type_e = selecte_main_menu_id - ID_TYPE_ACTION;
                    MMIAPIKM_SetSupportfileFlag(TRUE);
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
                    MMIAPIKM_SetHmvfileFlag(TRUE);
#endif
                    MMIKM_CreateSearchWaitWin();
                    MMK_CloseWin(win_id);
                }
                break;
            default:
                break;
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

/*****************************************************************************/
//  Description : km main win
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    lcd_dev_info.lcd_id		= GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id	= GUI_BLOCK_MAIN;
    
     
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //创建窗口的菜单
        CreateKMMainWinMenuList(win_id, MMIKM_MAIN_WIN_MENU_CTRL_ID);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_KM_UPDATE_MAIN_WIN:
        //full paint, update date
        MMK_DestroyDynaCtrl(MMIKM_MAIN_WIN_MENU_CTRL_ID);
        //创建窗口的菜单
        CreateKMMainWinMenuList(win_id, MMIKM_MAIN_WIN_MENU_CTRL_ID);  
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        MainWinPopWinOpenMenuHandle();
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
        CreateMainWinPopMenu();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIAPIKM_SetSupportfileFlag(FALSE);
        MMIKM_ReleaseMemoForPlaylistInfo();
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPIKM_FileArrayDestroy();
        MMIAPIKM_SetSupportfileFlag(FALSE);
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
        MMIAPIKM_SetHmvfileFlag(FALSE);
#endif
        MMIKM_ReleaseMemoForPlaylistInfo();
        MMK_DestroyDynaCtrl(MMIKM_MAIN_WIN_MENU_CTRL_ID);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : append km type list one item
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendTypeInforItem(
                                BOOLEAN         is_text,        //in:是否添加text data
                                uint16          item_index,     //in:item索引
                                uint16          content_index,  //in:item内容索引,text or anim etc.
                                MMI_CTRL_ID_T   ctrl_id         //in:control id
                                )
{
    BOOLEAN             result = FALSE;
    
    char symbol_char[TYPE_NUM_STR_MAX_LEN] = {0};
    wchar symbol_wchar[TYPE_NUM_STR_MAX_LEN] = {0};
    wchar               first_line_wstr[KM_FILE_INFO_NAME_LEN_MAX] = {0};
    wchar               second_line_wstr[MMIKM_MAIN_MENU_ITEM_STR_MAX_LEN] = {0};
    MMI_STRING_T        text_string = {0};    
    GUILIST_ITEM_DATA_T item_data = {0};
    GUIANIM_FILE_INFO_T anim_path = {0};
    MMIKM_FILE_ITEM_INFO_T km_file_item_info = {0};  
    uint16  thumbnail_file_full_name_len=0;
    wchar   thumbnail_file_full_name[FMM_SEARCH_FILENAME_MAXLEN+1]={0};


    
    if( !GetMovieTypeInfor(item_index, &km_file_item_info) )
    {
        return result;
    }
    
    //set softkey
    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[2] = STXT_RETURN;

    //anim
    item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
	item_data.item_content[0].item_data.anim_path_ptr = &anim_path;


        GetThumbnailImageByFileName(km_file_item_info.file_full_path_name,km_file_item_info.file_info_name,thumbnail_file_full_name,&thumbnail_file_full_name_len);
	anim_path.full_path_wstr_ptr = thumbnail_file_full_name;
	anim_path.full_path_wstr_len = thumbnail_file_full_name_len;
    
    //text1,file name and show year
    //added file name infor
    MMIAPICOM_Wstrncpy(first_line_wstr, km_file_item_info.file_info_name, km_file_item_info.file_info_name_len);

    //added "("
    sprintf(symbol_char, "(");
    MMIAPICOM_StrToWstr((const uint8*)symbol_char, symbol_wchar);
    MMIAPICOM_Wstrcat(first_line_wstr, symbol_wchar);

    //added show year infor
    MMIAPICOM_Wstrcat(first_line_wstr, km_file_item_info.file_info_year);

    //added ")"
    memset(symbol_char, 0x00, TYPE_NUM_STR_MAX_LEN);
    SCI_MEMSET(symbol_wchar, 0x00, TYPE_NUM_STR_MAX_LEN);
    sprintf(symbol_char, ")");
    MMIAPICOM_StrToWstr((const uint8*)symbol_char, symbol_wchar);
    MMIAPICOM_Wstrcat(first_line_wstr, symbol_wchar);

    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;  
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = first_line_wstr;
    item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen(first_line_wstr);

    //text2 show time 
    //time:
    MMI_GetLabelTextByLang(TXT_KM_TIME, &text_string);
    MMIAPICOM_Wstrncpy(second_line_wstr, text_string.wstr_ptr, text_string.wstr_len);

    //time infor
    MMIAPICOM_Wstrcat(second_line_wstr, km_file_item_info.file_info_time);

    //mins
    MMI_GetLabelTextByLang(TXT_KM_MINS, &text_string);
    MMIAPICOM_Wstrncat(second_line_wstr, text_string.wstr_ptr, text_string.wstr_len);
   
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = second_line_wstr;
    item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen(second_line_wstr);

    if(is_text)
    {
        //add item text etc.
        GUILIST_SetItemData( ctrl_id, &item_data, item_index );  
    }
    else
    {   //add item anim
        GUILIST_SetItemContent(ctrl_id, &item_data.item_content[content_index],
                          item_index, content_index);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : load km type infor list control
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
LOCAL void SetListWinTitleTextId(
                             MMI_WIN_ID_T   win_id, //in:
                             MMI_CTRL_ID_T  ctrl_id, //in:
                             MMIKM_TYPE_E km_type_enum
                             )
{
    MMI_TEXT_ID_T text_id = TXT_KM_TYPE_ACTION;
    switch(km_type_enum)
    {
        case MMIKM_TYPE_ACTION:
            text_id = TXT_KM_TYPE_ACTION;
            break;
        case MMIKM_TYPE_ADVENTURE:
            text_id = TXT_KM_TYPE_ADVENTURE;
            break;
        case MMIKM_TYPE_ANIMATION:
            text_id = TXT_KM_TYPE_ANIMATION;
            break;
        case MMIKM_TYPE_COMEDY:
            text_id = TXT_KM_TYPE_COMEDY;
            break;
        case MMIKM_TYPE_DOCUMENTARY:
            text_id = TXT_KM_TYPE_DOCUMENTARY;
            break;
        case MMIKM_TYPE_DRAMA:
            text_id = TXT_KM_TYPE_DRAMA;
            break;
        case MMIKM_TYPE_FAMILY:
            text_id = TXT_KM_TYPE_FAMILY;
            break;
        case MMIKM_TYPE_FANTASY:
            text_id = TXT_KM_TYPE_FANTASY;
            break;
        case MMIKM_TYPE_HORROR:
            text_id = TXT_KM_TYPE_HORROR;
            break;
        case MMIKM_TYPE_MUSIC:
            text_id = TXT_KM_TYPE_MUSIC;
            break;
        case MMIKM_TYPE_MYSTERY:
            text_id = TXT_KM_TYPE_MYSTERY;
            break;
        case MMIKM_TYPE_PERFORMING:
            text_id = TXT_KM_TYPE_PERFORMING;
            break;
        case MMIKM_TYPE_ROMANCE:
            text_id = TXT_KM_TYPE_ROMANCE;
            break;
        case MMIKM_TYPE_SCIFI:
            text_id = TXT_KM_TYPE_SCIFI;
            break;
        case MMIKM_TYPE_SPORTS:
            text_id = TXT_KM_TYPE_SPORTS;
            break;
        case MMIKM_TYPE_SUSPENSE:
            text_id = TXT_KM_TYPE_SUSPENSE;
            break;
        case MMIKM_TYPE_WAR:
            text_id = TXT_KM_TYPE_WAR;
            break;
        case MMIKM_TYPE_WESTERN:
            text_id = TXT_KM_TYPE_WESTERN;
            break;
        case MMIKM_TYPE_OTHER:
            text_id = TXT_KM_TYPE_OTHER;
            break;
        default:
            break;
    }
    //set title
    GUIWIN_SetTitleTextId(win_id, text_id,TRUE);
}

/*****************************************************************************/
//  Description : load km type infor list control
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMovieTypeInfor(
                             uint16         type_num_index,  //in:索引
                             MMIKM_FILE_ITEM_INFO_T *file_item_info_ptr //out
                             )
{
    BOOLEAN recode = FALSE;
    MMIKM_TYPE_E km_type_enum = MMIKM_TYPE_MAX;

    if(PNULL == file_item_info_ptr)
    {
        return recode;
    }
    
    km_type_enum = s_selectd_km_type_e; 
    
    if(MMIKM_GetMovieTpyeInfor(type_num_index, km_type_enum, file_item_info_ptr) )
    {
        recode = TRUE;
    }
    return recode;
    
}

/*****************************************************************************/
//  Description : Init the control of the list window of all item lists.
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void InitTypeInforWinListCtrl(MMI_WIN_ID_T   win_id, //in:
                                        MMI_CTRL_ID_T  ctrl_id //in:
                                      )
{
    uint16  i =0;
    uint16 total_num = 0;
    
    GUILIST_ITEM_T       item = {0};
    MMI_HANDLE_T    list_handle = 0;
    
    MMIKM_TYPE_E km_type_enum = MMIKM_TYPE_MAX;

    list_handle = MMK_ConvertIdToHandle(ctrl_id);
   
    if(GUILIST_GetTotalItemNum(ctrl_id))
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }

    km_type_enum = s_selectd_km_type_e; 
    total_num = MMIKM_GetMovieTypeNum(km_type_enum);
    //dynamic item data
    GUILIST_SetMaxItem( ctrl_id, total_num, TRUE);
    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
    GUILIST_SetTitleStyle(ctrl_id, GUILIST_TITLE_ONLY);
    //set title
    SetListWinTitleTextId(win_id, ctrl_id, km_type_enum);
    
    item.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2;
    for(i=0; i< total_num; i++)
    {
        GUILIST_AppendItem(list_handle, &item);
    }
    
}

/*****************************************************************************/
//  Description : km show movie type information window  msg
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDispTypeInforWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         )
{
   
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    MMI_CTRL_ID_T  ctrl_id = MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID; 
    GUILIST_NEED_ITEM_DATA_T    *need_item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = PNULL;
#ifdef MMI_PDA_SUPPORT	
	GUIFORM_CHILD_HEIGHT_T child_height = {0};
	GUIFORM_CHILD_DISPLAY_E   display_type = GUIFORM_CHILD_DISP_HIDE;
 #endif  
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:          
#ifdef MMI_PDA_SUPPORT
		GUIFORM_IsSlide(MMIKM_FORM_CTRL_ID, FALSE);
		GUIFORM_SetChildDisplay(MMIKM_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
		GUIFORM_PermitChildBg(MMIKM_FORM_CTRL_ID, FALSE);	
		GUIFORM_PermitChildBorder(MMIKM_FORM_CTRL_ID, FALSE);
		child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
		GUIFORM_SetChildHeight(MMIKM_FORM_CTRL_ID, MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID, &child_height);
#endif
		InitTypeInforWinListCtrl(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        //add item text etc.
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        if( PNULL == need_item_data_ptr )
        {
            //SCI_TRACE_LOW:" PNULL == need_item_data_ptr "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_1418_112_2_18_2_31_25_101,(uint8*)"");
            break;
        }
        
        AppendTypeInforItem(TRUE, need_item_data_ptr->item_index, 0, ctrl_id);
        break;
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        //add item anim
        need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
        if (PNULL == need_item_content_ptr)
        {
            //SCI_TRACE_LOW:" PNULL == need_item_content_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_1429_112_2_18_2_31_25_102,(uint8*)"");
            break;
        }
        AppendTypeInforItem(FALSE, need_item_content_ptr->item_index, need_item_content_ptr->item_content_index, ctrl_id);
        break;
        
    case MSG_KM_UPDATE_LIST:
        InitTypeInforWinListCtrl(win_id,ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0);
        //GUILIST_SetCurItemIndex(ctrl_id, s_selectd_delet_file_indx);       
        break;

    case MSG_CTL_LIST_MARK_ITEM:
        if (0 == GUILIST_GetSelectedItemIndex(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,PNULL,0))
        {
#ifndef MMI_PDA_SUPPORT
            GUILIST_SetTextListMarkable(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,FALSE);
#endif
        }
		else if (MMIKM_GetMovieTypeNum(s_selectd_km_type_e) == GUILIST_GetSelectedItemIndex(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,PNULL,0))
		{
#ifdef MMI_PDA_SUPPORT
             GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK, TXT_DELETE, STXT_CANCEL, TRUE);
		     MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#endif
		}
		else
		{
#ifdef MMI_PDA_SUPPORT
             GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL,TRUE);
			 MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#endif
		}
        break;
         
    //case MSG_CTL_ICONLIST_APPEND_ITEM:
        //break;
        
    case MSG_CTL_ICONLIST_APPEND_ICON:
        break;
        
    case MSG_FULL_PAINT:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifndef MMI_PDA_SUPPORT 
		if(!MMIKM_IsLastPlayInfor())
	    {
            MMK_PostMsg(MMIKM_MAIN_WIN_ID, MSG_KM_UPDATE_MAIN_WIN, PNULL, PNULL);
	    }
		DispPlayKmovieWin();
#else
        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
        {
            if(MMIKM_GetMovieTypeNum(s_selectd_km_type_e) == GUILIST_GetSelectedItemIndex(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,PNULL,0))//如果已经全部选中
            {
                //GUILIST_SetTextListMarkable(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,FALSE);
                GUILIST_SetAllSelected(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,FALSE);
                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL,TRUE);
            }
            else//全部标记
            {
                GUILIST_SetAllSelected(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,TRUE);         
                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK, TXT_DELETE, STXT_CANCEL, TRUE);
            }
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
        {
            GUIFORM_SetChildDisplay(MMIKM_FORM_CTRL_ID,MMICOMMON_SOFTKEY_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
            GUILIST_SetTextListMarkable(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,FALSE);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
        {		
            if(0 == GUILIST_GetSelectedItemIndex(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID, PNULL, 0))
            {
                MMIPUB_OpenAlertWarningWin( TXT_PLS_SELECT_ONE_ITEM);
            }
            else
            {
                DeltSelectdFile();
                
            }
        }
		else
        {
    		if(!MMIKM_IsLastPlayInfor())
    	    {
                MMK_PostMsg(MMIKM_MAIN_WIN_ID, MSG_KM_UPDATE_MAIN_WIN, PNULL, PNULL);
    	    }
            DispPlayKmovieWin();
        }
#endif
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
		GUIFORM_GetChildDisplay(MMIKM_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
        if(GUIFORM_CHILD_DISP_HIDE == display_type)
		{
			CreateDispTypeWinPopMenu();
		}
        break;

    case MSG_APP_MENU:
		GUIFORM_GetChildDisplay(MMIKM_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
        if(GUIFORM_CHILD_DISP_HIDE == display_type)
		{		
			CreateDispTypeWinPdaPopMenu();
		}
        break;

#endif //MMI_PDA_SUPPORT
    case MSG_CTL_OK:
    case MSG_APP_OK:
        CreateDispTypeWinPopMenu();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : create km app main window dynamic menu list 
//	Global resource dependence : 
//  Author:tonny.chen
//	Note: 
/*****************************************************************************/
LOCAL void CreateKMMainWinMenuList(
                    MMI_WIN_ID_T             win_id, //IN: list menu's window id
                    MMI_CTRL_ID_T            ctrl_id //IN: list menu's id
                               )
{
    uint16                  i = 0;
    MMI_STRING_T      kstring = {0};
    MMI_STRING_T      item_string = {0, MMIKM_MAIN_MENU_ITEM_STR_MAX_LEN};
    MMI_TEXT_ID_T     text_id = TXT_KM;
    uint32            node_id = 0;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    BOOLEAN is_lastplay = MMIKM_IsLastPlayInfor();
    char type_num_char[TYPE_NUM_STR_MAX_LEN] = {0};
    wchar type_num_string[TYPE_NUM_STR_MAX_LEN*2 + 1] = {0};
    
    //movie main menu list
    uint32 main_win_menu_list[MMIKM_MAIN_MENU_ITEM_NUM][2] ={
                                      {ID_TYPE_ACTION,      TXT_KM_TYPE_ACTION},
                                      {ID_TYPE_ADVENTURE,   TXT_KM_TYPE_ADVENTURE},
                                      {ID_TYPE_ANIMATION,   TXT_KM_TYPE_ANIMATION},
                                      {ID_TYPE_COMEDY,      TXT_KM_TYPE_COMEDY},
                                      {ID_TYPE_DOCUMENTARY, TXT_KM_TYPE_DOCUMENTARY},
                                      {ID_TYPE_DRAMA,       TXT_KM_TYPE_DRAMA},
                                      {ID_TYPE_FAMILY,      TXT_KM_TYPE_FAMILY},
                                      {ID_TYPE_FANTASY,     TXT_KM_TYPE_FANTASY},
                                      {ID_TYPE_HORROR,      TXT_KM_TYPE_HORROR},
                                      {ID_TYPE_MUSIC,       TXT_KM_TYPE_MUSIC},
                                      {ID_TYPE_MYSTERY,     TXT_KM_TYPE_MYSTERY},
                                      {ID_TYPE_PERFORMING,  TXT_KM_TYPE_PERFORMING},
                                      {ID_TYPE_ROMANCE,     TXT_KM_TYPE_ROMANCE},
                                      {ID_TYPE_SCIFI,       TXT_KM_TYPE_SCIFI},
                                      {ID_TYPE_SPORTS,      TXT_KM_TYPE_SPORTS},
                                      {ID_TYPE_SUSPENSE,    TXT_KM_TYPE_SUSPENSE},
                                      {ID_TYPE_WAR,         TXT_KM_TYPE_WAR},
                                      {ID_TYPE_WESTERN,     TXT_KM_TYPE_WESTERN},
                                      {ID_TYPE_OTHER,       TXT_KM_TYPE_OTHER},
                                   };
    uint32 main_win_menu_list_ext[MMIKM_MAIN_MENU_ITEM_MAX_NUM][2] ={
                                      {ID_LAST_PLAYED,      TXT_KM_LAST_PLAYED},  
                                      {ID_TYPE_ACTION,      TXT_KM_TYPE_ACTION},
                                      {ID_TYPE_ADVENTURE,   TXT_KM_TYPE_ADVENTURE},
                                      {ID_TYPE_ANIMATION,   TXT_KM_TYPE_ANIMATION},
                                      {ID_TYPE_COMEDY,      TXT_KM_TYPE_COMEDY},
                                      {ID_TYPE_DOCUMENTARY, TXT_KM_TYPE_DOCUMENTARY},
                                      {ID_TYPE_DRAMA,       TXT_KM_TYPE_DRAMA},
                                      {ID_TYPE_FAMILY,      TXT_KM_TYPE_FAMILY},
                                      {ID_TYPE_FANTASY,     TXT_KM_TYPE_FANTASY},
                                      {ID_TYPE_HORROR,      TXT_KM_TYPE_HORROR},
                                      {ID_TYPE_MUSIC,       TXT_KM_TYPE_MUSIC},
                                      {ID_TYPE_MYSTERY,     TXT_KM_TYPE_MYSTERY},
                                      {ID_TYPE_PERFORMING,  TXT_KM_TYPE_PERFORMING},
                                      {ID_TYPE_ROMANCE,     TXT_KM_TYPE_ROMANCE},
                                      {ID_TYPE_SCIFI,       TXT_KM_TYPE_SCIFI},
                                      {ID_TYPE_SPORTS,      TXT_KM_TYPE_SPORTS},
                                      {ID_TYPE_SUSPENSE,    TXT_KM_TYPE_SUSPENSE},
                                      {ID_TYPE_WAR,         TXT_KM_TYPE_WAR},
                                      {ID_TYPE_WESTERN,     TXT_KM_TYPE_WESTERN},
                                      {ID_TYPE_OTHER,       TXT_KM_TYPE_OTHER},
                                   };
    
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    item_string.wstr_ptr = SCI_ALLOC_APP(sizeof(wchar)*MMIKM_MAIN_MENU_ITEM_STR_MAX_LEN);
    if(PNULL == item_string.wstr_ptr)
    {
        //SCI_TRACE_LOW:"CreateKMMainWinMenuList alloc fail !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_1587_112_2_18_2_31_25_103,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"CreateKMMainWinMenuList is_lastplay = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_1591_112_2_18_2_31_25_104,(uint8*)"d", is_lastplay);
    
    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect,win_id,ctrl_id,GUIMENU_STYLE_THIRD);
    //GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT //MINI
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OPTION, IMAGE_NULL, STXT_RETURN);
#else
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OPTION, TXT_NULL, STXT_RETURN);
#endif

    MMK_SetAtvCtrl(win_id, ctrl_id);

    //set menu title
    MMI_GetLabelTextByLang(text_id, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);

    //set main win menu items 
    if(!is_lastplay)
    {
        for(i = 0; i < MMIKM_MAIN_MENU_ITEM_NUM; i++)
        {
            //get item info from menu_main
            node_id = main_win_menu_list[i][0];
            text_id = main_win_menu_list[i][1];
            
            memset(type_num_char, 0x00, TYPE_NUM_STR_MAX_LEN);
            SCI_MEMSET(type_num_string, 0x00, TYPE_NUM_STR_MAX_LEN*2 + 1);
            //SCI_MEMSET(&kstring, 0x00, sizeof(MMI_STRING_T));
            item_string.wstr_len = 0;
            SCI_MEMSET(item_string.wstr_ptr, 0x00, MMIKM_MAIN_MENU_ITEM_STR_MAX_LEN);
            
            MMI_GetLabelTextByLang(text_id, &kstring);
            
            sprintf(type_num_char, "(%d)", MMIKM_GetMovieTypeNum((MMIKM_TYPE_E)i));
            MMIAPICOM_StrToWstr((const uint8*)type_num_char, type_num_string);
            
            MMIAPICOM_Wstrncpy(item_string.wstr_ptr, kstring.wstr_ptr, kstring.wstr_len);
            MMIAPICOM_Wstrncat(item_string.wstr_ptr, type_num_string, MMIAPICOM_Wstrlen(type_num_string));
            
            item_string.wstr_len = MMIAPICOM_Wstrlen(type_num_string) + kstring.wstr_len;
            
            node_item.item_text_ptr = &item_string;
#ifdef MMI_PDA_SUPPORT
            node_item.select_icon_id = IMAGE_COMMON_MOVIE_UDISK_ICON;
#endif
            GUIMENU_InsertNode(i, node_id, 0, &node_item, ctrl_id);
        }
    }
    else
    {   //have last play infor item 
        for(i = 0; i < MMIKM_MAIN_MENU_ITEM_MAX_NUM; i++)
        {
            //get item info from menu_main
            node_id = main_win_menu_list_ext[i][0];
            text_id = main_win_menu_list_ext[i][1];
            //last play item
            if(0 == i)
            {
                MMI_GetLabelTextByLang(text_id, &kstring);
                node_item.item_text_ptr = &kstring;
            }
            else
            {
                //other item
                memset(type_num_char, 0x00, TYPE_NUM_STR_MAX_LEN);
                SCI_MEMSET(type_num_string, 0x00, TYPE_NUM_STR_MAX_LEN*2 +1 );
                
                item_string.wstr_len = 0;
                SCI_MEMSET(item_string.wstr_ptr, 0x00, MMIKM_MAIN_MENU_ITEM_STR_MAX_LEN);

                MMI_GetLabelTextByLang(text_id, &kstring);

                sprintf(type_num_char, "(%d)", MMIKM_GetMovieTypeNum((i-1)));
                MMIAPICOM_StrToWstr((const uint8*)type_num_char, type_num_string);

                MMIAPICOM_Wstrncpy(item_string.wstr_ptr, kstring.wstr_ptr, kstring.wstr_len);
                MMIAPICOM_Wstrncat(item_string.wstr_ptr, type_num_string, MMIAPICOM_Wstrlen(type_num_string));

                item_string.wstr_len = MMIAPICOM_Wstrlen(type_num_string) + kstring.wstr_len;

                node_item.item_text_ptr = &item_string;

            }
#ifdef MMI_PDA_SUPPORT
            node_item.select_icon_id = IMAGE_COMMON_MOVIE_UDISK_ICON;
#endif
            
            GUIMENU_InsertNode(i, node_id, 0, &node_item, ctrl_id);
        }
    }

    if(item_string.wstr_ptr != PNULL)
    {
        SCI_FREE(item_string.wstr_ptr);
        item_string.wstr_ptr = PNULL;
        
    }
   
}

/*****************************************************************************/
//  Description : create searching file wait win
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void  MMIKM_CreateSearchWaitWin(void)
{
    MMI_STRING_T prompt_str = {0};
#ifdef MMI_KING_MOVIE_IN_UDISK_SUPPORT    
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
#else
    MMIFILE_DEVICE_E file_dev = MMIKM_GetPlayListDevice();
#endif

    if(!MMIKM_MemSetForPlaylistInfo())
    {
        //SCI_TRACE_LOW:"MMIKM_MemSetForPlaylistInfo fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_1697_112_2_18_2_31_26_105,(uint8*)"");
        return;
    }
    MMIAPIKM_SetSupportfileFlag(TRUE);
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    MMIAPIKM_SetHmvfileFlag(TRUE);
#endif
    
    MMI_GetLabelTextByLang(TXT_COMMON_UPDATING, &prompt_str);
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMIKM_SEACH_PLAYLIST_WAIT_WIN_ID,
             PNULL,ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePlaylistSearchWaitingWin);

    MMIAPIKM_FileArrayDestroy();
    MMK_PostMsg(MMIKM_SEACH_PLAYLIST_WAIT_WIN_ID,MSG_KM_LOADING_BEGIN,&file_dev,sizeof(MMIFILE_DEVICE_E));
}

/*****************************************************************************/
//  Description : create main window
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void  MMIKM_CreateMainWin(void)
{
    //创建窗口
    if(!MMK_IsOpenWin(MMIKM_MAIN_WIN_ID))
    {
        MMK_CreateWin((uint32*)MMIKM_MAIN_WIN_TAB, NULL);
    }
}

/*****************************************************************************/
//  Description : to create km main win menu list popmenu window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CreateMainWinPopMenu(void)
{
    MMK_CreateWin((uint32 *)MMIKM_MAIN_WIN_POPMENU_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : to create km show movie type information window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CreateDispTypeInforWin(void)
{
    MMK_CreateWin((uint32 *)MMIKM_DISP_TYPE_INFOR_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : to create km show movie type information window popmenu window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CreateDispTypeWinPopMenu(void)
{
    MMK_CreateWin((uint32 *)MMIKM_DISP_TYPE_WIN_POPMENU_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : to create km show more information window
//  Global resource dependence : 
//  Author:tonny.chen
//  Note: 
/*****************************************************************************/
LOCAL void CreateDispMoreInforWin(void)
{
    MMK_CreateWin((uint32 *)MMIKM_DISP_MORE_INFOR_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : create searching file wait win
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void  MainWinPopWinOpenMenuHandle(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIKM_MAIN_WIN_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T pop_menu_id = 0;
    MMI_MENU_ID_T selecte_main_menu_id = 0;
    
    selecte_main_menu_id = GUIMENU_GetCurNodeId(MMIKM_MAIN_WIN_MENU_CTRL_ID);
    GUIMENU_GetId(ctrl_id,&group_id,&pop_menu_id);
    if(ID_LAST_PLAYED == selecte_main_menu_id)
    {
         MMIKM_PLAY_INFO_T km_play_info = {0};
         if(MMIKM_GetLastPlayInfor(&km_play_info))
         {  
                
              //SCI_TRACE_LOW:"[MMIKM] MainWinPopWinOpenMenuHandle :%d"
              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_WINTAB_1781_112_2_18_2_31_26_106,(uint8*)"d", km_play_info.played_time);
              MMIAPIKM_SetLastPlayOpenWin(TRUE);
              //MMIAPIKM_SetSupportfileFlag(TRUE);
              MMIAPIKM_CkeckSupportfile(km_play_info.cur_file_full_path_name);//smv_support
#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
                MMIAPIVP_MiniFunction_PlayVideo(km_play_info.cur_file_full_path_name,
                                 km_play_info.cur_file_full_path_len);
#else
              MMIAPIVP_PlayVideoFromVideoPlayerEx(VP_ENTRY_FROM_FILE,km_play_info.cur_file_full_path_name,
                                 km_play_info.cur_file_full_path_len, PNULL,MMIAPIKM_GetLastPorcesstime(),MMIKM_SetLastPlayInfor);
#endif
#endif
              MMIKM_SetLastPlayInfor(km_play_info.cur_file_full_path_name, km_play_info.cur_file_full_path_len, 0);
                                    
         }
    }
    else
    {
        //创建影片某种类型显示窗口
        s_selectd_km_type_e = selecte_main_menu_id - ID_TYPE_ACTION;
        CreateDispTypeInforWin();
    }
}

/*****************************************************************************/
//  Description : to handle disp type pda mark opt window
//  Global resource dependence : 
//  Author:chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  DispTypePdaMarkOptHandle(
                                          MMI_CTRL_ID_T        list_ctrl_id, 
                                          MMI_MENU_ID_T    pop_menu_id
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIKM_TYPE_E km_type_enum = MMIKM_TYPE_MAX;
    uint16 total_num = 0;
    uint16 cur_file_selection = 0;

    km_type_enum = s_selectd_km_type_e; 
    total_num = MMIKM_GetMovieTypeNum(km_type_enum);
    cur_file_selection = GUILIST_GetCurItemIndex(list_ctrl_id);
    
    switch(pop_menu_id)
    {
        case ID_KM_SUB_MARK:
            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id, total_num);
            GUILIST_SetSelectedItem(list_ctrl_id,cur_file_selection,TRUE);
#ifdef MMI_PDA_SUPPORT
			GUISOFTKEY_SetTextId(MMIKM_DISP_TYPE_INFOR_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL,FALSE);
			GUIFORM_SetChildDisplay(MMIKM_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
#endif
			break;
            
        case ID_KM_CANCEL_MARK:
            GUILIST_SetSelectedItem(list_ctrl_id,cur_file_selection,FALSE);
            if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
            {
                GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);	
            }
            break;

        case ID_KM_MARK_ALL:
            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id, total_num);
            GUILIST_SetAllSelected(list_ctrl_id,TRUE);
#ifdef MMI_PDA_SUPPORT
			GUISOFTKEY_SetTextId(MMIKM_DISP_TYPE_INFOR_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK, TXT_DELETE, STXT_CANCEL,FALSE);
			GUIFORM_SetChildDisplay(MMIKM_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
#endif
			break;
            
        case ID_KM_CANCEL_ALL_MARK:
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            break;            
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }	 
    return recode;
}

/*****************************************************************************/
//  Description : SetDispTypeInfoOptMenuGreyStatus
//  Global resource dependence : 
//  Author:chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL void  SetDispTypeInfoOptMenuGreyStatus(void)
{
#ifdef MMI_PDA_SUPPORT
    MMI_CTRL_ID_T   list_ctrl_id    = MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID;
    MMI_MENU_GROUP_ID_T  mark_group_id = MMIKM_DISP_TYPE_PDA_POPMENU;
    MMI_MENU_GROUP_ID_T  sub_mark_group_id = MMIKM_DISP_TYPE_MARK_OPT;
    MMI_WIN_ID_T win_id = MMIKM_DISP_TYPE_PDA_POPMENU_WIN_ID;
    uint16          cur_file_selection = GUILIST_GetCurItemIndex(list_ctrl_id);
    uint16          total_num       = 0;
    uint16          selected_num    = 0;
    const GUILIST_ITEM_T  *list_item_ptr   = PNULL;
    MMIKM_TYPE_E km_type_enum = MMIKM_TYPE_MAX;

    km_type_enum = s_selectd_km_type_e; 
    total_num = MMIKM_GetMovieTypeNum(km_type_enum);
    if(0 < total_num)
    {
        MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_KM_MARK, FALSE);
        selected_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);        
        list_item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id,cur_file_selection);         
        if(GUILIST_GetListStateById(list_ctrl_id,GUILIST_STATE_ENABLE_MARK))
        {
            //long ok menu
            MMIAPICOM_EnableGrayed(MMIKM_DISP_TYPE_WIN_POPMENU_WIN_ID, MMIKM_DISP_TYPE_WIN_POP, ID_DELETE, FALSE);
            MMIAPICOM_EnableGrayed(MMIKM_DISP_TYPE_WIN_POPMENU_WIN_ID, MMIKM_DISP_TYPE_WIN_POP, ID_MORE_INFO, TRUE);
            if(PNULL != list_item_ptr && GUILIST_GetItemState(list_item_ptr,GUIITEM_STATE_SELECTED) )
            {
                //mark
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_SUB_MARK, TRUE);
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_MARK, FALSE);                 
                if(total_num == selected_num)
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_MARK_ALL, TRUE);
                }
                else
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_MARK_ALL, FALSE);
                }
                if (0 == selected_num)
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, TRUE);
                }
                else
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, FALSE);
                }
            }
            else
            {
                //mark
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_SUB_MARK, FALSE);
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_MARK, TRUE);
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_MARK_ALL, FALSE);
                if (0 == selected_num)
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, TRUE);                    
                }
                else
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, FALSE);
                }
            }
        }
        else
        {
            //mark
            MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_SUB_MARK, FALSE);
            MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_MARK, TRUE);
            MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_MARK_ALL, FALSE);
            MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, TRUE);

            //long ok menu
            MMIAPICOM_EnableGrayed(MMIKM_DISP_TYPE_WIN_POPMENU_WIN_ID, MMIKM_DISP_TYPE_WIN_POP, ID_DELETE, FALSE);
            MMIAPICOM_EnableGrayed(MMIKM_DISP_TYPE_WIN_POPMENU_WIN_ID, MMIKM_DISP_TYPE_WIN_POP, ID_MORE_INFO, FALSE);
        }
    }
    else
    {
        MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_KM_MARK, TRUE);
        //long ok menu
        MMIAPICOM_EnableGrayed(MMIKM_DISP_TYPE_WIN_POPMENU_WIN_ID, MMIKM_DISP_TYPE_WIN_POP, ID_DELETE, TRUE);
        MMIAPICOM_EnableGrayed(MMIKM_DISP_TYPE_WIN_POPMENU_WIN_ID, MMIKM_DISP_TYPE_WIN_POP, ID_MORE_INFO, TRUE);
    }
#else
    MMI_CTRL_ID_T   list_ctrl_id    = MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID;
    MMI_MENU_GROUP_ID_T  mark_group_id = MMIKM_DISP_TYPE_WIN_POP;
    MMI_MENU_GROUP_ID_T  sub_mark_group_id = MMIKM_DISP_TYPE_MARK_OPT;
    MMI_WIN_ID_T win_id = MMIKM_DISP_TYPE_WIN_POPMENU_WIN_ID;
    uint16          cur_file_selection = GUILIST_GetCurItemIndex(list_ctrl_id);
    uint16          total_num       = 0;
    uint16          selected_num    = 0;
    const GUILIST_ITEM_T  *list_item_ptr   = PNULL;
    MMIKM_TYPE_E km_type_enum = MMIKM_TYPE_MAX;

    km_type_enum = s_selectd_km_type_e; 
    total_num = MMIKM_GetMovieTypeNum(km_type_enum);
    if(0 < total_num)
    {
        MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_KM_MARK, FALSE);
        selected_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);        
        list_item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id,cur_file_selection);         
        if(GUILIST_GetListStateById(list_ctrl_id,GUILIST_STATE_ENABLE_MARK))
        {
            MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_PLAY, TRUE);
            MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_KM_MARK, FALSE);
            MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_DELETE, FALSE);
            MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_MORE_INFO, TRUE);
            if(PNULL != list_item_ptr && GUILIST_GetItemState(list_item_ptr,GUIITEM_STATE_SELECTED) )
            {
                //mark
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_SUB_MARK, TRUE);
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_MARK, FALSE);                 
                if(total_num == selected_num)
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_MARK_ALL, TRUE);
                }
                else
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_MARK_ALL, FALSE);
                }
                if (0 == selected_num)
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, TRUE);
                }
                else
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, FALSE);
                }
            }
            else
            {
                //mark
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_SUB_MARK, FALSE);
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_MARK, TRUE);
                MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_MARK_ALL, FALSE);
                if (0 == selected_num)
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, TRUE);                    
                }
                else
                {
                    MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, FALSE);
                }
            }
        }
        else
        {
            //mark
            MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_SUB_MARK, FALSE);
            MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_MARK, TRUE);
            MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_MARK_ALL, FALSE);
            MMIAPICOM_EnableGrayed(win_id, sub_mark_group_id, ID_KM_CANCEL_ALL_MARK, TRUE);

            MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_PLAY, FALSE);
            MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_KM_MARK, FALSE);
            MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_DELETE, FALSE);
            MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_MORE_INFO, FALSE);
        }
    }
    else
    {
        MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_PLAY, TRUE);
        MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_KM_MARK, TRUE);
        MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_DELETE, TRUE);
        MMIAPICOM_EnableGrayed(win_id, mark_group_id, ID_MORE_INFO, TRUE);
    }
#endif
}

/*****************************************************************************/
//  Description :设置是否进入上次播放模式
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKM_SetLastPlayOpenWin(BOOL isopened)
{
    s_entry_last_played=isopened;
}
/*****************************************************************************/
//  Description : 是否以<<上次播放>> 形式打开 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKM_IsLastPlayOpenWin(void)
{
    return s_entry_last_played;
}

#ifdef MMI_PDA_SUPPORT    
/*****************************************************************************/
//  Description : to handle km disp type pda popmenu window
//  Global resource dependence : 
//  Author:chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDispTypePdaWinPopWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIKM_DISP_TYPE_PDA_POPMENU_CTRL_ID;
    MMI_CTRL_ID_T list_ctrl_id = MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T pop_menu_id = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetDispTypeInfoOptMenuGreyStatus();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
//        GUIMENU_GetId(ctrl_id,&group_id,&pop_menu_id);
//        DispTypePdaMarkOptHandle(list_ctrl_id, pop_menu_id);
        GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
        GUILIST_SetMaxSelectedItem(list_ctrl_id, MMIKM_GetMovieTypeNum(s_selectd_km_type_e));
		GUISOFTKEY_SetTextId(MMIKM_DISP_TYPE_INFOR_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL,FALSE);
		GUIFORM_SetChildDisplay(MMIKM_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        MMK_CloseWin(win_id);
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

/*****************************************************************************/
//  Description : to create km show movie type information window pda popmenu window
//  Global resource dependence : 
//  Author:chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL void CreateDispTypeWinPdaPopMenu(void)
{
    MMK_CreateWin((uint32 *)MMIKM_DISP_TYPE_PDA_POPMENU_WIN_TAB,PNULL);
}
#endif
/*****************************************************************************/
//  Description : entryplaywin
//  Global resource dependence : 
//  Author:jinju.ma
//  Note: 
/*****************************************************************************/
LOCAL void DispPlayKmovieWin(void)
{
	uint16    cur_select_list_id = 0;
	MMIKM_FILE_ITEM_INFO_T km_file_item_info = {0};
#ifdef KM_DEBUG_SWITCH_ENABLE
	uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
#endif

	cur_select_list_id = GUILIST_GetCurItemIndex(MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID);
	GetMovieTypeInfor(cur_select_list_id, &km_file_item_info);
#ifdef KM_DEBUG_SWITCH_ENABLE
	MMIAPICOM_WstrToStr((const wchar*)km_file_item_info.file_full_path_name, char_filename);
#endif
    if(km_file_item_info.is_valid && 0 != MMIAPICOM_Wstrlen(km_file_item_info.file_full_path_name))
    {
        //MMIAPIKM_SetSupportfileFlag(TRUE);
        MMIAPIKM_CkeckSupportfile(km_file_item_info.file_full_path_name);//smv_support
#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
        MMIAPIVP_MiniFunction_PlayVideo(km_file_item_info.file_full_path_name,
                           km_file_item_info.file_full_path_len);
#else
        MMIAPIVP_PlayVideoFromVideoPlayerEx(VP_ENTRY_FROM_FILE,km_file_item_info.file_full_path_name,
                           km_file_item_info.file_full_path_len, PNULL,0,MMIKM_SetLastPlayInfor);
#endif
#endif
        MMIKM_SetLastPlayInfor(km_file_item_info.file_full_path_name, 
                               km_file_item_info.file_full_path_len, 0);
	}                                       
}

/*****************************************************************************/
//  Description : MMIAPIKM_IsKMmainWinOpen
//  Global resource dependence : 
//  Author:Glen.Li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKM_IsKMmainWinOpen(void)
{
    return MMK_IsOpenWin(MMIKM_MAIN_WIN_ID);	
}

/*Edit by script, ignore 6 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
