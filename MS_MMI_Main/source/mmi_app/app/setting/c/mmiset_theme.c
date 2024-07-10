/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_WALLPAPER_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_display.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmiset_nv.h"
#include "mmiset_image.h"
#include "mmiudisk_export.h"
#include "mmi_resource.h"
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

#define     MMISET_FILE_THEME_NUM_MAX               6

LOCAL MMISET_THEME_TYPE_E s_current_theme_type = MMISET_THEME_DEFAULT_TYPE;
LOCAL uint16   s_current_static_theme_id = 0; //当前选中的主题
LOCAL const wchar s_mmisetting_theme_dir[]           = { 't', 'h', 'e', 'm', 'e' ,0 };



                                                
#ifdef MULTI_THEME_SUPPORT
/*****************************************************************************/
//  Description : to handle set wallpaper
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetThemeWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );
/*****************************************************************************/
//  Description : to handle set wallpaper
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleThemePreviewWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
WINDOW_TABLE( MMISET_SET_THEME_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetThemeWindow ),    
        WIN_ID( MMISET_SET_THEME_WIN_ID ),
        WIN_TITLE( TXT_SET_THEME ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_THEME_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};

WINDOW_TABLE( MMISET_SET_THEME_PREVIEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleThemePreviewWindow ),    
        WIN_ID( MMISET_SET_THEME_PREVIEW_WIN_ID ),        
        //        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif                                                
/*****************************************************************************/
//  Description : append file theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendFileTheme(const wchar * device_name, uint32 name_len)
{
#ifdef MULTI_FILE_THEME_SUPPORT       
    FILEARRAY_DATA_T            f_array_data = {0};
    MMIFMM_FILTER_T             filter = {{"*.thm"}};
    wchar   full_path_name[FMM_SEARCH_FILENAME_MAXLEN+1] = {0};
    uint16  full_path_len = FMM_SEARCH_FILENAME_MAXLEN+1;
    wchar   file_name[FMM_SEARCH_FILENAME_MAXLEN+1] = {0};
    uint16  file_name_len = FMM_SEARCH_FILENAME_MAXLEN;
    uint32 file_theme_num = 0;    
    uint16  i = 0;
    FILEARRAY file_array = NULL;       
    wchar * name_data_ptr = PNULL;
    uint32 total_index = 0;
    
    /* 获得theme路径 */
    file_array = MMIAPIFILEARRAY_Create();
    MMIAPIFMM_CombineFullPath(
        device_name, 
        name_len,
        s_mmisetting_theme_dir, 
        MMIAPICOM_Wstrlen(s_mmisetting_theme_dir),
        PNULL, 
        NULL,
        full_path_name,
        &full_path_len
        );
    MMIAPIFMM_SearchFileInPath(full_path_name, full_path_len, &filter, TRUE, FUNC_FIND_FILE, file_array, 0, 0);
    /* 获得动态theme个数 */
    file_theme_num = (uint16)MMIAPIFILEARRAY_GetArraySize(file_array);
    
    if(MMISET_THEME_FILE_TYPE == MMIAPISET_GetCurrentThemeType())
    {
        MMIAPISET_GetFileThemeContent(full_path_name);
    }
    for (i = 0; i < file_theme_num; i ++)
    {
        SCI_MEMSET(&f_array_data, 0, sizeof(f_array_data));
        if(MMIAPIFILEARRAY_Read(file_array, i, &f_array_data))
        {
            if (FILEARRAY_TYPE_FILE == f_array_data.type)
            {
                SCI_MEMSET(file_name,0x00,sizeof(file_name));
                /* 获得显示的名字 */
                MMIAPIFMM_SplitFullPath(f_array_data.filename, f_array_data.name_len, PNULL, NULL, PNULL, NULL, file_name, &file_name_len);
                /* 记录全路径在list中 */
                name_data_ptr = SCI_ALLOC_APP((f_array_data.name_len + 1) * sizeof(wchar));
                //SCI_ASSERT(PNULL != name_data_ptr);
		   if(PNULL == name_data_ptr)
		   {
		        //SCI_TRACE_LOW:"mmisetting_assert AppendFileTheme"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_THEME_143_112_2_18_2_52_13_345,(uint8*)"");
			 MMIAPIFILEARRAY_Destroy(&file_array);	
		        return;
		    } 
                MMIAPICOM_Wstrncpy(name_data_ptr, f_array_data.filename, f_array_data.name_len);
                name_data_ptr[f_array_data.name_len] = 0;            
                MMIAPISET_AppendListItemByTextBuffer(file_name, file_name_len, (uint32)name_data_ptr, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, MMISET_SET_THEME_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);
                /* 判断是否是当前的文件theme，如果是则高亮。 */
                if(MMISET_THEME_FILE_TYPE == MMIAPISET_GetCurrentThemeType())
                {
                    if(0 == MMIAPICOM_Wstrcmp(full_path_name, f_array_data.filename))
                    {
                        total_index = GUILIST_GetTotalItemNum(MMISET_SET_THEME_CTRL_ID);
                        GUILIST_SetCurItemIndex( MMISET_SET_THEME_CTRL_ID, total_index - 1 );              
                        GUILIST_SetSelectedItem( MMISET_SET_THEME_CTRL_ID, total_index - 1, TRUE );
                    }
                }
            }  
        }
    }
    MMIAPIFILEARRAY_Destroy(&file_array);
#endif
}

/*****************************************************************************/
//  Description : get file theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetFileThemeContent(wchar * file_name_ptr)
{
#ifdef MULTI_THEME_SUPPORT       
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_SUCCESS;
    
    MMINV_READ(MMINV_SET_CUR_FILE_THEME, file_name_ptr, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {           
        *file_name_ptr = 0;
        MMINV_WRITE(MMINV_SET_CUR_FILE_THEME, "");
    }    
#endif
}
/*****************************************************************************/
//  Description : get current theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetCurrentStaticThemeID(void)
{   
    return s_current_static_theme_id;    
}

#ifdef MULTI_THEME_SUPPORT
/*****************************************************************************/
//  Description : ini current theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_IniCurrentTheme(void)
{
    wchar   full_path_name[FMM_SEARCH_FILENAME_MAXLEN+1] = {0};    
    uint16                      index           =   0;
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_SUCCESS;
    
    /* 初始化静态theme索引值 */
    MMINV_READ(MMINV_SET_CUR_STATIC_THEME, &index, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {   
        index   =   MMISET_DEFAULT_THEME_TYPE;
        MMINV_WRITE(MMINV_SET_CUR_STATIC_THEME, &index);
    }    
    s_current_static_theme_id = index;    
    
    /* 初始化theme类型 */
    MMINV_READ(MMINV_SET_THEME_TYPE, &s_current_theme_type, return_value);    
    if (MN_RETURN_SUCCESS != return_value)
    {           
        if (MMISET_DEFAULT_THEME_TYPE)/*lint !e774 !e506*/
        {
            s_current_theme_type   =   (uint32)MMISET_THEME_STATIC_TYPE;/*lint !e64*/
        }
        else
        {
            s_current_theme_type   =   (uint32)MMISET_THEME_DEFAULT_TYPE;/*lint !e64*/
        }
        MMINV_WRITE(MMINV_SET_THEME_TYPE, &s_current_theme_type);
    }        
    
    switch(s_current_theme_type)
    {
    case MMISET_THEME_DEFAULT_TYPE:    
        MMISET_SetCurrentTheme(0, PNULL);
        break;
    case MMISET_THEME_STATIC_TYPE:
        MMISET_SetCurrentTheme(s_current_static_theme_id, PNULL);
        break;
    case MMISET_THEME_FILE_TYPE:        
        if(!MMIAPIUDISK_UdiskIsRun())
        {
            MMIAPISET_GetFileThemeContent(full_path_name);
            if(MMIAPIFMM_IsFileExist(full_path_name, MMIAPICOM_Wstrlen(full_path_name)))
            {
                MMISET_SetCurrentTheme(0, full_path_name);
            }
            else
            {
                MMISET_SetCurrentTheme(0, PNULL);
            }
        }
        else
        {
            /* udisk使用中，只能使用default资源。 */
            MMIRES_UpdateStaticThemeRes(0);
        }
        break;
    default:
	//SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_IniCurrentTheme"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_THEME_256_112_2_18_2_52_13_346,(uint8*)"");
       // SCI_ASSERT(0);
        break;
    }  
}



/*****************************************************************************/
//  Description : to handle set theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetThemeWindow(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    uint16          cur_selection   =   0;
    uint32          res_num = 0;
    uint32          i = 0;
    uint32          total = 0;
    wchar * name_ptr = PNULL;
    MMI_STRING_T itemStr={0};
    uint32 text_id[]=
    {
        TXT_SET_THEME_1,
        TXT_SET_THEME_2
    };
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_SETTHEME, FALSE);
        res_num = MMIRES_GetStaticThemeTotalNum();        
        GUILIST_SetMaxItem(MMISET_SET_THEME_CTRL_ID,(uint16)(res_num + 1 + MMISET_FILE_THEME_NUM_MAX), FALSE );//max item 2
        MMIAPISET_AppendListItemByTextIdExt( TXT_SET_THEME_1,TXT_COMMON_OK, STXT_PREVIEW, STXT_RETURN, MMISET_SET_THEME_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO  );
        MMIAPISET_AppendListItemByTextIdExt( TXT_SET_THEME_2, TXT_COMMON_OK, STXT_PREVIEW, STXT_RETURN, MMISET_SET_THEME_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO );
        
#ifdef MULTI_FILE_THEME_SUPPORT  
        if(!(MMIAPIUDISK_UdiskIsRun() || MMIAPIUdisk_VUARTIsRun()))
        {     
            AppendFileTheme(MMIFILE_DEVICE_UDISK, MMIFILE_DEVICE_UDISK_LEN);
            AppendFileTheme(MMIFILE_DEVICE_SDCARD, MMIFILE_DEVICE_SDCARD_LEN);
        }
#endif
        /* 判断是否是当前的静态theme，如果是则高亮。 */
        if((MMISET_THEME_STATIC_TYPE == MMIAPISET_GetCurrentThemeType()) || (MMISET_THEME_DEFAULT_TYPE == MMIAPISET_GetCurrentThemeType()) )
        {
            cur_selection = MMIAPISET_GetCurrentStaticThemeID(); 
            GUILIST_SetCurItemIndex( MMISET_SET_THEME_CTRL_ID, cur_selection);              
            GUILIST_SetSelectedItem( MMISET_SET_THEME_CTRL_ID, cur_selection, TRUE);
        }
        
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_THEME_CTRL_ID);       
        break;
        
    case MSG_FULL_PAINT:
        //设置选中
        //cur_selection = MMIAPISET_GetCurrentTheme();        
        //GUILIST_SetSelectedItem( MMISET_SET_THEME_CTRL_ID, cur_selection, TRUE );
        
        //设置阴影条所在位置
        //cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_THEME_CTRL_ID);
        //GUILIST_SetCurItemIndex( MMISET_SET_THEME_CTRL_ID,cur_selection );  
        cur_selection = MMIAPISET_GetCurrentStaticThemeID();
        GUILIST_SetSelectedItem( MMISET_SET_THEME_CTRL_ID, cur_selection, TRUE);
        GUILIST_SetOwnSofterKey(MMISET_SET_THEME_CTRL_ID,TRUE);
        //GUIWIN_SetSoftkeyTextId(win_id, STXT_OK,STXT_PREVIEW,STXT_RETURN,FALSE);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_THEME_CTRL_ID);
        /* 只有静态的theme可以预览 */
        if(cur_selection <= MMIRES_GetStaticThemeTotalNum())
        {
            MMK_CreateWin((uint32*)MMISET_SET_THEME_PREVIEW_WIN_TAB,(ADD_DATA)cur_selection);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:        
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:    
    case MSG_APP_OK:
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_THEME_CTRL_ID); 
        if(cur_selection <= MMIRES_GetStaticThemeTotalNum())
        {
            /* 选中了静态theme */
            MMISET_SetCurrentTheme(cur_selection, PNULL);    
        }
        else
        {
            /* 选中了文件动态theme */
            GUILIST_GetItemData(MMISET_SET_THEME_CTRL_ID, cur_selection, (uint32*)(&name_ptr));
            //MMIAPIFILEARRAY_Read(s_file_array, cur_selection - MMIRES_GetStaticThemeTotalNum() - 1, &f_array_data);
            MMISET_SetCurrentTheme(cur_selection, name_ptr);    
        }
        //success!       
        MMI_GetLabelTextByLang(text_id[cur_selection],&itemStr);
        GUILABEL_SetText(MMISET_DISPLAY_THEME_LABEL_EDIT_CTRL_ID, &itemStr, FALSE);

        MMK_CloseWin( win_id );
        break;  
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:        
        MMK_CloseWin( win_id );      
        break;
    case MSG_CLOSE_WINDOW:
        total = GUILIST_GetTotalItemNum(MMISET_SET_THEME_CTRL_ID);
        for(i = 0; i < total; i ++)
        {
            GUILIST_GetItemData(MMISET_SET_THEME_CTRL_ID, i, (uint32*)(&name_ptr));
            if(PNULL != name_ptr)
            {
                SCI_FREE(name_ptr);
            }
        }
        //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_SETTHEME, TRUE);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : get current theme type
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 这个函数不能被resource直接调用判断，
//  因为在某些时候（udisk使用中），虽说记录的是文件theme,但实际上只能使用default资源。
/*****************************************************************************/
PUBLIC MMISET_THEME_TYPE_E MMIAPISET_GetCurrentThemeType(void)
{
    return s_current_theme_type;
}

/*****************************************************************************/
//  Description : set current theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetCurrentTheme(uint16   cur_selection, const wchar * theme_file_name)
{
    // MN_RETURN_RESULT_E          return_value = MN_RETURN_SUCCESS;
#ifndef MMI_PDA_SUPPORT 
    MMISET_WALLPAPER_SET_T      wallpaper_info = {0};
#endif
    
    if(PNULL == theme_file_name)
    {        
        MMINV_WRITE(MMINV_SET_CUR_STATIC_THEME, &cur_selection);
        s_current_static_theme_id = cur_selection;
        if(0 == cur_selection)
        {
            s_current_theme_type = MMISET_THEME_DEFAULT_TYPE;
        }
        else
        {
            s_current_theme_type = MMISET_THEME_STATIC_TYPE;
        }        
        
        MMINV_WRITE(MMINV_SET_THEME_TYPE, &s_current_theme_type);        
        
        //设置背景没有备份,需要重新备份
        MMITHEME_SetBgLayerChanged();       
        MMIRES_UpdateStaticThemeRes(cur_selection);
    }
    else
    {           
        MMINV_WRITE(MMINV_SET_CUR_FILE_THEME, theme_file_name);/*lint !e605*/
        s_current_static_theme_id = 0;
        s_current_theme_type = MMISET_THEME_FILE_TYPE;
        
        MMINV_WRITE(MMINV_SET_THEME_TYPE, &s_current_theme_type);
        
        //设置背景没有备份,需要重新备份
        MMITHEME_SetBgLayerChanged();        
        MMIRES_UpdateFileThemeRes(theme_file_name);
    }

#ifndef MMI_PDA_SUPPORT  
    //update wallpaper
#if defined MMI_WALLPAPER_SUPPORT
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    if ((MMISET_WALLPAPER_FIXED_PIC == wallpaper_info.wallpaper_type) &&
        (MMISET_WALLPAPER_DEFAULT_ID == wallpaper_info.wallpaper_id))//wallpaper is default
    {
        MMIAPIIDLE_SetWallpaper(TRUE);/*lint !e64*/
    }
#endif
#endif    
}

/*****************************************************************************/
//  Description : to handle set theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleThemePreviewWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E        recode                                  =   MMI_RESULT_TRUE;
#ifndef MMI_PDA_SUPPORT
    uint16              cur_selection                           =   0;  
    uint32              theme_mainmenu[MMISET_THEME_MAX_NUM]    =   {IMAGE_SET_THEME1, IMAGE_SET_THEME2};

    GUI_POINT_T         dis_point                               =   {0};
    GUI_LCD_DEV_INFO    lcd_dev_info                            =   {GUI_LCD_0, GUI_BLOCK_0};
    MMI_STRING_T itemStr={0};
    uint32 text_id[]=
    {
        TXT_SET_THEME_1,
        TXT_SET_THEME_2
    };
    wchar * name_ptr = PNULL;
    lcd_dev_info.lcd_id     =   GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id   =   GUI_BLOCK_MAIN;

    //add items to listbox    
    
    switch (msg_id)
    {        
    case MSG_FULL_PAINT:
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_THEME_CTRL_ID);
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            theme_mainmenu[cur_selection],
            &lcd_dev_info);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,TXT_NULL,STXT_RETURN,FALSE);
        break;     
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:        
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:    
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_THEME_CTRL_ID);        
        if(cur_selection <= MMIRES_GetStaticThemeTotalNum())
        {
            MMISET_SetCurrentTheme(cur_selection, PNULL);    
        }
        else
        {
            GUILIST_GetItemData(MMISET_SET_THEME_CTRL_ID, cur_selection, (uint32*)(&name_ptr));
            //MMIAPIFILEARRAY_Read(s_file_array, cur_selection - MMIRES_GetStaticThemeTotalNum() - 1, &f_array_data);
            MMISET_SetCurrentTheme(cur_selection, name_ptr);    
        }
        MMI_GetLabelTextByLang(text_id[cur_selection],&itemStr);
        GUILABEL_SetText(MMISET_DISPLAY_THEME_LABEL_EDIT_CTRL_ID, &itemStr, FALSE);

        MMK_CloseWin(MMISET_SET_THEME_WIN_ID);
        MMK_CloseWin( win_id );
        break;  
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:       
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
#endif
    return recode;
}
#else
/*****************************************************************************/
//  Description : get current theme type
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 这个函数不能被resource直接调用判断，
//  因为在某些时候（udisk使用中），虽说记录的是文件theme,但实际上只能使用default资源。
/*****************************************************************************/
PUBLIC MMISET_THEME_TYPE_E MMIAPISET_GetCurrentThemeType(void)
{
    return MMISET_THEME_DEFAULT_TYPE;
}
#endif




/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
// 
/*****************************************************************************/

PUBLIC void MMISET_CreateThemePubWin(void)
{
#ifdef MULTI_THEME_SUPPORT
            MMK_CreatePubListWin((uint32*)MMISET_SET_THEME_WIN_TAB,PNULL);
#endif    
}




/*Edit by script, ignore 2 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
