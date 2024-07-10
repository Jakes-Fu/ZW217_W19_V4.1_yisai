/*****************************************************************************
** File Name:      mmiebook_bookshelf.c                                      *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe ebook module book shelf     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2011     ying.xu              Creat
******************************************************************************/
#include "mmi_app_ebook_trc.h"
#ifdef EBOOK_SUPPORT
#include "tb_dal.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmipub.h"
#include "tb_dal.h"
#include "mmidisplay_data.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "guifont.h"

#include "mmifmm_export.h"
#include "mmibt_export.h"
#include "mmi_appmsg.h"
#include "guimenu.h"
#include "mmifmm_id.h"
#include "mmiudisk_export.h"
#include "mmi_mainmenu_export.h"
#include "guitab.h"
#include "mmicc_export.h"
#include "mmimp3_export.h"
#include "mmikl_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "guirichtext.h"
#include "guimenu.h"
#include "guiedit.h"
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#endif
#ifdef MMS_SUPPORT
#include "mmimms_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmiset_text.h"

#include "mmiebook_menutable.h"
#include "mmiebook_image.h"
#include "mmiebook_id.h"
#include "mmiebook_text.h"
#include "mmiebook_file.h"
//#include "mmiebook.h"
#include "mmiebook_internal.h"
#include "mmiebook_nv.h"
#include "mmiebook_id.h"    
#include "mmigrid.h"
#include "mmiebook_bookshelf.h"
#include "mmiebook_image.h"
#ifdef MMIEBOOK_BOOKSHELF_SUPPORT  
#include "guires.h"
#include "guibutton.h"
#include "mmiebook_position.h"  
#endif
#define MMIEBOOK_BUTTON_MARGIN_WIDTH      10

/*****************************************************************************/
//  Description : send text file by MMS
//  Global resource dependence :
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void EbookSendMMS(
                        MMIEBOOK_FMM_CALLBACK_INFO* fmm_callback_info
                        );

/*****************************************************************************/
//  Description : send text file by blue tooth
//  Global resource dependence : g_textfile_all_info
//   Author: kelly.li
//  Note:   
/*****************************************************************************/
LOCAL void SendTxtFileByBt(MMIEBOOK_FMM_CALLBACK_INFO* fmm_callback_info);

/*****************************************************************************/
//  Description : enablel menu item
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void EnableTxtListOptMenuItem(
                                    MMI_WIN_ID_T                win_id,
                                    MMIEBOOK_FMM_CALLBACK_INFO  *fmm_callback_info_ptr
                                    );

/*****************************************************************************/
//  Description : 电子书列表选项
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTxtListOptWinMsg (
                                            MMI_WIN_ID_T win_id,            
                                            MMI_MESSAGE_ID_E msg_id,        
                                            DPARAM param
                                            );

/*****************************************************************************/
//  Description : 最近阅读的电子书列表选项
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLatestListOptWinMsg(
                                              MMI_WIN_ID_T win_id,            
                                              MMI_MESSAGE_ID_E msg_id,        
                                              DPARAM param
                                              );

#ifdef MMIEBOOK_BOOKSHELF_SUPPORT                                                
LOCAL MMI_RESULT_E  HandleLatestListPopupMenuWinMsg(
                                                    MMI_WIN_ID_T win_id,            
                                                    MMI_MESSAGE_ID_E msg_id,        
                                                    DPARAM param
                                                    );
#endif                                                     
/*****************************************************************************/
//  Description : 最近阅读的电子书列表
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLatestListWinMsg(
                                           MMI_WIN_ID_T win_id,            
                                           MMI_MESSAGE_ID_E msg_id,        
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description : enablel Latest List Opt menu item
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void EnableLatestListOptMenuItem(
                                       MMI_WIN_ID_T win_id
                                       );
#if !defined MMIEBOOK_BOOKSHELF_SUPPORT
/*****************************************************************************/
//  Description : add latest books to list ctrl
//  Global resource dependence : 
//  Author:  zhaohui
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void AppendLatestListItemData(
                                    MMI_CTRL_ID_T  ctrl_id,
                                    uint16 index
                                    );
#endif
/*****************************************************************************/
//  Description : handle ok key when in txt list opt win
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void HandleTxtListOptWinOKKey(  MMIEBOOK_FMM_CALLBACK_INFO* fmm_callback_info ); 

/*****************************************************************************/
//  Description : add latest books to list ctrl
//  Global resource dependence : 
//  Author:  zhaohui
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void InitLatestListCtrl(void);

/*****************************************************************************/
//  Description : alloc s_ebook_latest_file_info_ptr
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN CreateLatestListPtr( void );

/*****************************************************************************/
//  Description : free s_ebook_latest_file_info_ptr
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void DestroyLatestListPtr( void );

/*****************************************************************************/
//  Description : 往列表头添加数据
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN AddLatestListItem(void);

/*****************************************************************************/
//  Description : 删除选择的项
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void DeleteLatestListItem(
                                uint16 cur_select//IN: 书签列表窗口中的位置
                                );

/*****************************************************************************/
//  Description : 删除所有item
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void CleanLatestListItem(void);

/*****************************************************************************/
//  Description : 读取最近阅读的电子书信息
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN ReadLatestList(void);

/*****************************************************************************/
//  Description : 及时保存相关详情项信息
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN SaveLatestList(void);

/*****************************************************************************/
//  Description : 用最近一次打开的电子书信息来初始化s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->存在
/*****************************************************************************/
LOCAL BOOLEAN InitFileBriefFromLatest( uint16 cur_selection );


LOCAL   MMIEBOOK_LATEST_LIST_T*    s_ebook_latest_list_ptr =PNULL;//存储了最近10本电子书的信息
LOCAL  MMIEBOOK_BOOKSHELF_T  book_shelf = {0};

#ifdef MMIEBOOK_BOOKSHELF_SUPPORT
WINDOW_TABLE( MMIEBOOK_LATEST_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLatestListWinMsg ),    
        WIN_ID(MMIEBOOK_LATEST_LIST_WIN_ID),
        WIN_STATUSBAR,
#ifdef MMI_PDA_SUPPORT
        CREATE_BUTTON_CTRL(IMAGE_ISTYLE_EBOOK_TITLE_RETURN,MMIEBOOK_RETURN_BUTTON_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_ISTYLE_EBOOK_TITLE_MENU,MMIEBOOK_MENU_BUTTON_CTRL_ID),
#endif
        END_WIN
};
#else
WINDOW_TABLE( MMIEBOOK_LATEST_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleLatestListWinMsg ),    
        WIN_ID(MMIEBOOK_LATEST_LIST_WIN_ID),
        
        WIN_TITLE( TXT_EBOOK_LATEST_BOOKS ),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif    
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIEBOOK_LATEST_LIST_CTRL_ID),
        END_WIN
};
#endif

WINDOW_TABLE( MMIEBOOK_LATEST_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLatestListOptWinMsg),
        WIN_ID(MMIEBOOK_LATEST_LIST_OPT_WIN_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_HIDE_STATUS,
#endif
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EBOOK_LATEST_LIST_OPT,MMIEBOOK_LATEST_LIST_OPT_CTRL_ID),
#ifndef MMI_PDA_SUPPORT    
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#endif
        END_WIN
};


WINDOW_TABLE( MMIEBOOK_TXT_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTxtListOptWinMsg),
        WIN_ID(MMIEBOOK_TXT_LIST_OPT_WIN_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_HIDE_STATUS,
#endif
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EBOOK_TXT_LIST_OPT,MMIEBOOK_TXT_LIST_OPT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};

#ifdef MMIEBOOK_BOOKSHELF_SUPPORT    
WINDOW_TABLE( MMIEBOOK_LATEST_LIST_POPUP_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleLatestListPopupMenuWinMsg),
        WIN_ID(MMIEBOOK_LATEST_LIST_POPUP_WIN_ID),
        WIN_STYLE(WS_HAS_TRANSPARENT),//yaye.jiang for cr:NEWMS00115565
        END_WIN
};
#endif   

/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence : 
//  Author:maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T      text_id,       //插入的节点的文本
                             MMI_IMAGE_ID_T     image_id,       //插入的节点的文本
                             uint32             node_id,        //节点编号
                             uint32             parent_node_id, //父节点编号
                             uint16             index           //位置
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;    
    node_item.select_icon_id = image_id;
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}  

/*****************************************************************************/
//  Description : send text file by MMS
//  Global resource dependence :
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void EbookSendMMS(
                        MMIEBOOK_FMM_CALLBACK_INFO* fmm_callback_info
                        )
{
#ifdef MMS_SUPPORT
    MMIFILE_FILE_INFO_T     file_info = {0};
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
    wchar              full_name_wstr[MMIEBOOK_FULLNAME_MAX_LEN + 1] = {0};   //文件名字,ucs
    uint16             full_name_len = 0;
    
    //SCI_TRACE_LOW:"MMIEBOOK: FmmSendMMS."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_BOOKSHELF_334_112_2_18_2_15_8_0,(uint8*)"");
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }
    
    
    MMI_WSTRNCPY(full_name_wstr,MMIEBOOK_FULLNAME_MAX_LEN,fmm_callback_info->list_data_ptr->pathname,
        fmm_callback_info->list_data_ptr->pathname_len,fmm_callback_info->list_data_ptr->pathname_len); 
    
    full_name_len = fmm_callback_info->list_data_ptr->pathname_len;
    *(full_name_wstr + full_name_len) = MMIFILE_SLASH;
    full_name_len = full_name_len + 1;
    
    MMI_WSTRNCPY(full_name_wstr + full_name_len,MMIEBOOK_FULLNAME_MAX_LEN - full_name_len,fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->filename_ptr,
        fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->filename_len,fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->filename_len); 
    
    full_name_len = full_name_len + fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->filename_len;
    
    
    MMIAPIFMM_GetFileInfoFormFullPath(full_name_wstr,full_name_len,&file_info);
    
    
    file_type = fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->type;
    
#ifdef DRM_SUPPORT
    if(MMIFMM_FILE_TYPE_DRM == file_type)
    {
        //只要不能发送，则立马提示返回
        if (!MMIAPIDRM_IsCanForward(PNULL, SFS_INVALID_HANDLE, full_name_wstr))
        {
            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);               
            return;
        }
        file_type = MMIAPIDRM_GetDRMFileMimeType(SFS_INVALID_HANDLE , full_name_wstr);
    }
#endif	
    
    if (MMIFMM_FILE_TYPE_NORMAL != file_type && MMIFMM_FILE_TYPE_FOLDER != file_type)
    {
        if (   MMIFMM_FILE_TYPE_PICTURE == file_type
            || MMIFMM_FILE_TYPE_MUSIC == file_type
            || MMIFMM_FILE_TYPE_MOVIE == file_type)
        {
            MMIAPIMMS_EditMMSFromFile(&file_info,(MMIMULTIM_FILE_TYPE_E)file_type);/*lint !e64*/
        }    
        else
        {
            file_type = MMIFMM_FILE_TYPE_NORMAL;
            MMIAPIMMS_EditMMSFromFile(&file_info,(MMIMULTIM_FILE_TYPE_E)file_type);/*lint !e64*/
        }
    }		
    else				
    {
        // 提示格式不支持
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
    }
#endif
}

/*****************************************************************************/
//  Description : send text file by blue tooth
//  Global resource dependence : g_textfile_all_info
//   Author: kelly.li
//  Note:   
/*****************************************************************************/
LOCAL void SendTxtFileByBt(MMIEBOOK_FMM_CALLBACK_INFO* fmm_callback_info)
{
#ifdef BLUETOOTH_SUPPORT
    wchar              full_name_wstr[MMIEBOOK_FULLNAME_MAX_LEN + 1] = {0};   //文件名字,ucs
    uint16             full_name_len = 0;
    MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};
	
    MMI_WSTRNCPY(full_name_wstr,MMIEBOOK_FULLNAME_MAX_LEN,fmm_callback_info->list_data_ptr->pathname,
        fmm_callback_info->list_data_ptr->pathname_len,fmm_callback_info->list_data_ptr->pathname_len); 
    full_name_len = fmm_callback_info->list_data_ptr->pathname_len;
    *(full_name_wstr + full_name_len) = MMIFILE_SLASH;
    full_name_len = full_name_len + 1;
    MMI_WSTRNCPY(full_name_wstr + full_name_len,MMIEBOOK_FULLNAME_MAX_LEN - full_name_len,fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->filename_ptr,
        fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->filename_len,fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->filename_len); 
    full_name_len = full_name_len + fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->filename_len;
    
#ifdef DRM_SUPPORT
    if(MMIAPIDRM_IsDRMFile(SFS_INVALID_HANDLE , full_name_wstr))  
    {
        //只要不能发送，则立马提示返回
        if (!MMIAPIDRM_IsCanForward(PNULL, SFS_INVALID_HANDLE, full_name_wstr))
        {
            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);               
            return;
        }
    }
    
#endif
    MMIAPICOM_Wstrncpy(send_file_info.filepath_name,full_name_wstr,full_name_len);/*lint !e605*/
    send_file_info.filepath_len = full_name_len;
    send_file_info.file_size = fmm_callback_info->list_data_ptr->data[fmm_callback_info->index]->file_size;
    send_file_info.is_temp_file = FALSE;
    MMIAPIBT_SendMultiFile(&send_file_info,1);
#endif
}

//通过显示区域和显示截取区域，计算显示范围
//预置条件，显示区域等于图片大小，裁减区域小于显示区域
LOCAL GUI_RECT_T ConvertDisplayRect2ImageRect(GUI_RECT_T *img_rect, GUI_RECT_T* display_rect, GUI_RECT_T *clip_rect)
{
    GUI_RECT_T rect = {0};
    int16 left_offset = clip_rect->left - display_rect->left;
    int16 top_offset = clip_rect->top - display_rect->top;
    int16 right_offset = clip_rect->right - display_rect->right;
    int16 bottom_offset = clip_rect->bottom - display_rect->bottom;
    rect.left = img_rect->left + left_offset;
    rect.top = img_rect->top + top_offset;
    rect.right = img_rect->right + right_offset;    
    rect.bottom = img_rect->bottom + bottom_offset;
    return rect;
}

#ifdef MMIEBOOK_BOOKSHELF_SUPPORT
/*****************************************************************************/
//  Description : 获得电子书书架显示区域
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetBookshelfRect(MMIEBOOK_BOOKSHELF_T *p_bookshelf)
{
    GUI_RECT_T return_rect = MMITHEME_GetFullScreenRect();
    return_rect.top += MMI_STATUSBAR_HEIGHT ;
    return return_rect;
}

//去掉书架title的区域
LOCAL GUI_RECT_T GetBookshelfLayerRect(MMIEBOOK_BOOKSHELF_T *p_bookshelf)
{
    GUI_RECT_T return_rect = MMITHEME_GetFullScreenRect();
    return_rect.top += MMI_STATUSBAR_HEIGHT +  MMIEBOOK_TITLE_BAR_HEIGHT;
    return return_rect;
}

//获得书架上一层中各本书的区域
LOCAL int32 GetBookRectOnLayer(MMIEBOOK_BOOKSHELF_T *p_bookshelf,
                               GUI_RECT_T   *p_layer_rect, GUI_RECT_T *p_book_rect, int32 num)
{
    uint16 img_width = 0;
    uint16 img_height = 0;
    uint16 x_interval = 0;
    MMI_IMAGE_ID_T book_img_id = IMAGE_EBOOK_LOCAL;
    GUI_RECT_T layer_rect = *p_layer_rect;
    
    MMI_IMAGE_ID_T board_img_id = IMAGE_EBOOK_BOOKCASE_BORAD;
    
    //计算书在书架隔板上的偏移
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  board_img_id,  book_shelf.grid.display.win_id);
    layer_rect.bottom -= img_height*2/3;
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  book_img_id,  p_bookshelf->grid.display.win_id);
    
    x_interval = (p_bookshelf->grid.display.layout_rect.right - 
        p_bookshelf->grid.display.layout_rect.left + 1 - img_width*num) / (num + 1);
    
    layer_rect.top = layer_rect.bottom - img_height + 1;
    
    GUI_CreateMatrixRect(&layer_rect,x_interval,0,x_interval,0,1,num,p_book_rect);       
    return 0;
}

LOCAL int32 DrawBook(int32 index, GUI_RECT_T *p_rect, GUI_LCD_DEV_INFO *lcd_dev_info_ptr)
{
    GUI_RECT_T book_shelf_rect = book_shelf.rect;
    GUI_RECT_T display_rect = *p_rect;
    GUI_RECT_T clip_rect = *p_rect;
    GUI_RECT_T img_rect = {0};  
    uint16 img_width = 0;
    uint16 img_height = 0;
    
    int32 i = 0;
    MMI_IMAGE_ID_T img[]={                    
#ifdef MMI_LOW_MEMORY_EBOOK   
            IMAGE_EBOOK_BOOK1,
            IMAGE_EBOOK_BOOK1,
            IMAGE_EBOOK_BOOK1,
#else
        IMAGE_EBOOK_BOOK1,
            IMAGE_EBOOK_BOOK2,
            IMAGE_EBOOK_BOOK3,
#endif

#ifdef MMI_LOW_MEMORY_EBOOK   
            IMAGE_EBOOK_BOOK1,
            IMAGE_EBOOK_BOOK1,
            IMAGE_EBOOK_BOOK1,
#else
            IMAGE_EBOOK_BOOK3,
            IMAGE_EBOOK_BOOK1,
            IMAGE_EBOOK_BOOK2,
#endif            

#ifdef MMI_LOW_MEMORY_EBOOK   
            IMAGE_EBOOK_BOOK1,
            IMAGE_EBOOK_BOOK1,
            IMAGE_EBOOK_BOOK1,
#else
            IMAGE_EBOOK_BOOK2,
            IMAGE_EBOOK_BOOK3,
            IMAGE_EBOOK_BOOK1,
#endif            
            
    };
    const GUI_LCD_DEV_INFO *p_lcd_dev_info = 0;
    MMI_IMAGE_ID_T   img_id = IMAGE_EBOOK_LOCAL;    
    
    if(!GUI_IntersectRect( &clip_rect, display_rect, book_shelf_rect))
    {
        return -1;
    }
    
    if(index != 0)
    {   
        i = index % 9;   //通过时间获得1个伪随机数
        img_id = img[i];
    }
    if(UILAYER_IsLayerActive(lcd_dev_info_ptr))
    {
        p_lcd_dev_info = lcd_dev_info_ptr;
    }
    else
    {
        p_lcd_dev_info = MMITHEME_GetDefaultLcdDev();
    }
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  img_id,  book_shelf.grid.display.win_id);    
    img_rect = GUI_CreateRect( 0, 0 ,  img_width - 1, img_height - 1 );
    img_rect = ConvertDisplayRect2ImageRect( &img_rect, &display_rect, &clip_rect);
    
    GUIRES_DisplayImg(PNULL,
        &clip_rect,               /*lint !e605*/
        &img_rect,               /*lint !e605*/
        book_shelf.grid.display.win_id,
        img_id,
        p_lcd_dev_info);

/*
     if(index == book_shelf.focused_book_index)
     {
         LCD_DrawRect(p_lcd_dev_info, clip_rect, MMI_GREEN_COLOR);
     }   
*/
     return 0;
}

LOCAL int32 DrawBookName(int32 index, GUI_RECT_T *p_rect, GUI_LCD_DEV_INFO *lcd_dev_info_ptr)
{
    const GUI_LCD_DEV_INFO *p_lcd_dev_info = 0;
    GUISTR_STYLE_T   style={0};
    MMI_STRING_T     str={0};
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN  | GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    wchar			file_name[MMIEBOOK_TXT_NAME_MAX_LEN+1] = {0};
    uint16			file_name_len = 0;
    GUI_RECT_T book_shelf_rect = book_shelf.rect;
    GUI_RECT_T display_rect = *p_rect;
    GUI_RECT_T clip_rect = *p_rect;
    int16           x_margin = 0;

    x_margin = (display_rect.right - display_rect.left + 1)/8;
    display_rect.left += x_margin;
    display_rect.right -= x_margin;
    if(!GUI_IntersectRect( &clip_rect, display_rect, book_shelf_rect))
    {
        return -1;
    }
    style.font = MMIEBOOK_NAME_FONT;
    style.font_color =0x3900;//RGB(59, 35, 0);    
    style.align = ALIGN_HVMIDDLE;
    
    if(index == 0)
    {
        MMI_GetLabelTextByLang(TXT_EBOOK_LOCAL_FILE, &str);
    }
    else
    {
        if (MMIEBOOK_LATEST_LIST_MAX_NUM >= index)
        {
            SCI_MEMSET(file_name,0,(MMIEBOOK_TXT_NAME_MAX_LEN+1)*sizeof(wchar));
            MMIAPIFMM_SplitFullPath(s_ebook_latest_list_ptr->item_brief[index-1].full_name_wstr, 
                s_ebook_latest_list_ptr->item_brief[index-1].full_name_len,
                NULL, NULL, NULL, NULL,
                file_name, &file_name_len);
            str.wstr_ptr=file_name;
            str.wstr_len=file_name_len;
        }
    }
    display_rect.top += (display_rect.bottom - display_rect.top + 1)/2;
    if(UILAYER_IsLayerActive(lcd_dev_info_ptr))
    {
        p_lcd_dev_info = lcd_dev_info_ptr;
    }
    else
    {
        p_lcd_dev_info = MMITHEME_GetDefaultLcdDev();
    }
    GUISTR_DrawTextToLCDInRect(p_lcd_dev_info,
        &display_rect,
        &clip_rect,
        &str,
        &style,
        str_state,
        GUISTR_TEXT_DIR_AUTO
        );    
   return 0;
}


LOCAL int32 DrawBookshelfLayerBoard(int32 index, GUI_RECT_T *p_rect, GUI_LCD_DEV_INFO *lcd_dev_info_ptr)
{
    GUI_LCD_DEV_INFO *p_lcd_dev_info = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_EBOOK_BOOKCASE_BORAD;
    uint16 img_width = 0;    
    uint16 img_heigth = 0;
    GUI_RECT_T book_shelf_rect = book_shelf.rect;
    GUI_RECT_T display_rect = *p_rect;
    GUI_RECT_T clip_rect = *p_rect;
    GUI_RECT_T img_rect = {0};  
    
    GUIRES_GetImgWidthHeight(&img_width, &img_heigth,  img_id,  book_shelf.grid.display.win_id);    
    display_rect.top = display_rect.bottom - img_heigth + 1;
    
    if(!GUI_IntersectRect( &clip_rect, display_rect, book_shelf_rect))
    {
        return -1;
    }
    
    if(UILAYER_IsLayerActive(lcd_dev_info_ptr))
    {
        p_lcd_dev_info = lcd_dev_info_ptr;
    }
    else
    {
        p_lcd_dev_info = MMITHEME_GetDefaultLcdDev();   /*lint !e605 */
    }
    
    img_rect = GUI_CreateRect( 0, 0 ,  img_width - 1, img_heigth - 1 );
    img_rect = ConvertDisplayRect2ImageRect( &img_rect, &display_rect, &clip_rect);
    
    GUIRES_DisplayImg(PNULL,
        &clip_rect,               /*lint !e605*/
        &img_rect,               /*lint !e605*/
        book_shelf.grid.display.win_id,
        img_id,
        p_lcd_dev_info);    
    return 0;
}

LOCAL int32 DrawBookshelfLayerBook(int32 index, GUI_RECT_T *p_rect, GUI_LCD_DEV_INFO *lcd_dev_info_ptr)
{
    int32 i= 0;
    GUI_RECT_T book_rect[10] = {0};
    int32 book_start_index = 0;
    int16 per_layer=0;
    if(!MMITHEME_IsMainScreenLandscape ())
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_V;
    }
    else
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_H;
    }
    book_start_index=index*per_layer;
    GetBookRectOnLayer(&book_shelf, p_rect, book_rect, per_layer);
    for(i = 0; i < per_layer; i++ )
    {    
        if(book_start_index + i >= book_shelf.book_num + 1)
        {
            return 1;
        }        
        DrawBook(book_start_index + i, &book_rect[i], lcd_dev_info_ptr);
        DrawBookName(book_start_index + i, &book_rect[i], lcd_dev_info_ptr);
    }
    return 0;   
}


LOCAL int32 MMIEBOOK_DrawBookshelfLayer(int32 index, GUI_RECT_T *p_rect, GUI_LCD_DEV_INFO *lcd_dev_info_ptr)
{
    GUI_LCD_DEV_INFO *p_lcd_dev_info = 0;
    
    if(UILAYER_IsLayerActive(lcd_dev_info_ptr))
    {
        p_lcd_dev_info = lcd_dev_info_ptr;        
        UILAYER_ClearRect(lcd_dev_info_ptr, *p_rect);
    }
    else
    {
        p_lcd_dev_info = MMITHEME_GetDefaultLcdDev(); /*lint !e605 */
    }
    
    DrawBookshelfLayerBoard(index,p_rect,p_lcd_dev_info);   
    DrawBookshelfLayerBook(index,p_rect,p_lcd_dev_info);
    return 0;    
}


LOCAL int32 DrawBookshelfBackground(MMIEBOOK_BOOKSHELF_T *p_shelf)
{
    GUI_RECT_T rect = {0};    
    GUI_RECT_T title_rect_v = MMIEBOOK_TITLE_RECT_V;
    GUI_RECT_T title_rect_h = MMIEBOOK_TITLE_RECT_H;
    GUI_RECT_T title_rect={0};
    GUISTR_STYLE_T   style={0};
    MMI_STRING_T str={0};
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS;
    GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    if(!MMITHEME_IsMainScreenLandscape ())
    {
        title_rect = title_rect_v;
    }
    else
    {
        title_rect = title_rect_h;
    }
    
    rect = GetBookshelfRect(p_shelf);
    title_rect.top= rect.top;
    title_rect.bottom = title_rect.top + MMIEBOOK_TITLE_BAR_HEIGHT - 1;
    
    GUIRES_DisplayImg(PNULL,
        &rect,               /*lint !e605*/
        PNULL,               /*lint !e605*/
        MMIEBOOK_LATEST_LIST_WIN_ID,
        IMAGE_EBOOK_BOOKCASE_BG,
        &lcd_dev_info);
    
    style.align = ALIGN_HVMIDDLE;
    style.font = MMIEBOOK_TITTLE_FONT;
    style.font_color = 0xffff;
    style.char_space = 0;
    
    MMI_GetLabelTextByLang(TXT_COMM_EBOOK, &str);
    GUISTR_DrawTextToLCDInRect(&lcd_dev_info,
        &title_rect,
        &title_rect,
        &str,
        &style,
        str_state,
        GUISTR_TEXT_DIR_AUTO
        );
    return 0;    
}

LOCAL int32 GetBookIndex(MMIEBOOK_BOOKSHELF_T *p_shelf, GUI_POINT_T point)
{
    int32 layer_index = 0;
    GUI_RECT_T layer_rect={0};
    GUI_RECT_T book_rect[10] = {0};
    int32 i = 0;
    int16 per_layer=0;
    if(!MMITHEME_IsMainScreenLandscape ())
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_V;
    }
    else
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_H;
    }
    if(!GUI_PointIsInRect( point, p_shelf->rect))
    {
        return -1;
    }
    layer_index = MMIGRID_GetPointLocation(&p_shelf->grid, point);
    layer_rect = MMIGRID_GetOneGridRect(&p_shelf->grid, layer_index);    
    GetBookRectOnLayer(p_shelf, &layer_rect, book_rect, per_layer );
    for(i = 0; i< per_layer; i++)
    {
        if(GUI_PointIsInRect( point, book_rect[i]))
        {
            return i + layer_index*per_layer;
        }
    }
    return -1;
}

/*****************************************************************************/
//  Description : Set PDA Button
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL void SetPDAButtonParam(
                             MMI_WIN_ID_T   win_id  //in:
                             )
{
    GUI_RECT_T option_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T return_rect = MMITHEME_GetFullScreenRect();
    uint16 image_height = 0;
    uint16 image_width = 0;
    GUI_BG_T bg = {0};
    MMI_STRING_T button_str = {0};
    uint16 button_str_width = 0;
    uint16 display_width = 0;
    GUISTR_STYLE_T str_style = {0}; /*lint !e64*/
    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;
    GUI_ALIGN_E align_style = ALIGN_HVMIDDLE;
    MMI_TITLE_STYLE_T title_style = {0};
    GUI_FONT_ALL_T font_all = {0};
    
    MMITHEME_GetTitleStyle(&title_style);
    
    str_style.align = align_style;
    str_style.font = title_style.button_font_type;
    
    font_all.color = title_style.font_color;
    font_all.font = title_style.button_font_type;
    
    //return
    GUIRES_GetImgWidthHeight(&image_width,&image_height,IMAGE_ISTYLE_EBOOK_TITLE_RETURN,win_id);
    
    MMITHEME_GetResText(STXT_RETURN, win_id, &button_str);
    
    button_str_width = GUISTR_GetStringWidth(&str_style, &button_str, str_state);
    
    display_width = image_width + button_str_width;
    
    option_rect.top = MMI_STATUSBAR_HEIGHT;
    option_rect.left += MMIEBOOK_BUTTON_MARGIN_WIDTH;
    option_rect.right = option_rect.left + display_width - 1;
    option_rect.bottom = MMIEBOOK_TITLE_BOTTOM;
    
    option_rect = GUI_GetCenterRect(option_rect, display_width, image_height);
    
    GUIBUTTON_SetRect(MMIEBOOK_RETURN_BUTTON_CTRL_ID, &option_rect);
    
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_ISTYLE_EBOOK_TITLE_RETURN_DOWN;
    
    GUIBUTTON_SetPressedBg(MMIEBOOK_RETURN_BUTTON_CTRL_ID, &bg);
    
    GUIBUTTON_SetRunSheen(MMIEBOOK_RETURN_BUTTON_CTRL_ID, FALSE);
    
    GUIBUTTON_SetTextId(MMIEBOOK_RETURN_BUTTON_CTRL_ID, STXT_RETURN);
    
    GUIBUTTON_SetFont(MMIEBOOK_RETURN_BUTTON_CTRL_ID, &font_all);
    
    //Clear
    GUIRES_GetImgWidthHeight(&image_width,&image_height,IMAGE_ISTYLE_EBOOK_TITLE_MENU,win_id);
    
    MMITHEME_GetResText(TXT_COMMON_CLEAR, win_id, &button_str);
    
    button_str_width = GUISTR_GetStringWidth(&str_style, &button_str, str_state);
    
    display_width = image_width + button_str_width;
    
    return_rect.top = MMI_STATUSBAR_HEIGHT;
    return_rect.right -= MMIEBOOK_BUTTON_MARGIN_WIDTH;
    return_rect.left = return_rect.right - display_width + 1;
    return_rect.bottom = MMIEBOOK_TITLE_BOTTOM;
    
    return_rect = GUI_GetCenterRect(return_rect, display_width, image_height);
    
    GUIBUTTON_SetRect(MMIEBOOK_MENU_BUTTON_CTRL_ID, &return_rect);
    
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_ISTYLE_EBOOK_TITLE_MENU_DOWN;
    
    GUIBUTTON_SetPressedBg(MMIEBOOK_MENU_BUTTON_CTRL_ID, &bg);
    
    GUIBUTTON_SetRunSheen(MMIEBOOK_MENU_BUTTON_CTRL_ID, FALSE);
    
    GUIBUTTON_SetTextId(MMIEBOOK_MENU_BUTTON_CTRL_ID, TXT_COMMON_CLEAR);
    
    GUIBUTTON_SetFont(MMIEBOOK_MENU_BUTTON_CTRL_ID, &font_all);
}
#endif
//init book shelf
LOCAL int32 ResetBookshelf(MMIEBOOK_BOOKSHELF_T *p_bookshelf,int32 book_num)
{
    int16 per_layer=0;
    int16 per_screen=0;
    if(!MMITHEME_IsMainScreenLandscape ())
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_V;
        per_screen=MMIEBOOK_BOOKSHELF_LAYER_PER_SCREEN_V;
    }
    else
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_H;
        per_screen=MMIEBOOK_BOOKSHELF_LAYER_PER_SCREEN_H;
    }
    p_bookshelf->book_num = book_num;
    p_bookshelf->layer_num = (1 + p_bookshelf->book_num + per_layer - 1 )
         /per_layer;
    if(p_bookshelf->layer_num < per_screen)
    {
         p_bookshelf->layer_num = per_screen;
    }
    p_bookshelf->grid.row = p_bookshelf->layer_num;    
    
    if(p_bookshelf->layer_num > per_screen)
    {
        p_bookshelf->grid.display.layout_rect.bottom = p_bookshelf->grid.display.layout_rect.top 
            + p_bookshelf->grid.grid_height * p_bookshelf->grid.row;            

    }
    else
    {
        p_bookshelf->grid.display.layout_rect = p_bookshelf->grid.rect;
        p_bookshelf->grid.display.display_rect = p_bookshelf->grid.rect;
    }

    return 0;
}

//open a book on bookshelf
LOCAL int32 OpenBook(int32 index, MMI_HANDLE_T win_id)            
{
    if(0==index)
    {
        uint32 data=MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
        BOOLEAN is_need_privacy_protection =TRUE;
        if(data==0)
        {
            is_need_privacy_protection=FALSE;
        }
        MMIEBOOK_OpenExplorerWinforEbook(FALSE, is_need_privacy_protection);
#ifndef MMI_TASK_MANAGER
        MMK_CloseWin( win_id );  
#endif                
    }
    else if(-1!=index && index <= (int32)s_ebook_latest_list_ptr->total_item_num)
    {
        //初始化s_ebook_file_brief
        MMIEBOOK_CreateFileBrief();    
        if (InitFileBriefFromLatest(index-1))
        {
            //MMK_CreateEbookWin((uint32*)MMIEBOOK_ENTER_TXT_CONTENT_WIN_TAB, PNULL);//资源管理器中入口
            MMIEBOOK_OpenTxtContentWin(PNULL);
#ifndef MMI_TASK_MANAGER
            MMK_CloseWin( win_id );  
#endif                
        }
        else//该电子书已不存在
        {
            MMIEBOOK_DestroyFileBrief();
            MMIPUB_OpenAlertWarningWin(TXT_EBOOK_FILE_NOT_EXIST);
        }  
    }
    return 0;             
}

//init book shelf
LOCAL int32 InitBookshelf(MMIEBOOK_BOOKSHELF_T *p_bookshelf,
                          MMI_WIN_ID_T win_id,
                          int32 book_num
                          )
{
    int16 per_layer=0;
    int16 per_screen=0;
    if(!MMITHEME_IsMainScreenLandscape ())
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_V;
        per_screen=MMIEBOOK_BOOKSHELF_LAYER_PER_SCREEN_V;
    }
    else
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_H;
        per_screen=MMIEBOOK_BOOKSHELF_LAYER_PER_SCREEN_H;
    }
    SCI_MEMSET(p_bookshelf, 0, sizeof(MMIEBOOK_BOOKSHELF_T));
    
    p_bookshelf->book_num = book_num;
    //p_bookshelf->book_num = 7;
    
    //p_bookshelf->layer_num = (1 + MMIEBOOK_LATEST_LIST_MAX_NUM + MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER - 1)
   //     /MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER;
   
     p_bookshelf->layer_num = (1 + p_bookshelf->book_num + per_layer - 1 )
         /per_layer;
    if(p_bookshelf->layer_num < per_screen)
    {
        p_bookshelf->layer_num = per_screen;
    }
    p_bookshelf->grid.row = p_bookshelf->layer_num;
    p_bookshelf->grid.col = 1;
    
    p_bookshelf->rect = GetBookshelfLayerRect(p_bookshelf);
    p_bookshelf->grid.rect =  p_bookshelf->rect;
    
    p_bookshelf->grid.display.win_id = win_id;
    p_bookshelf->grid.display.display_rect = p_bookshelf->rect;
    
    p_bookshelf->grid.grid_height =  (p_bookshelf->grid.display.display_rect.bottom - 
        p_bookshelf->grid.display.display_rect.top + 1) 
        / per_screen;
    
    p_bookshelf->grid.grid_width  =  (p_bookshelf->grid.display.display_rect.right - 
        p_bookshelf->grid.display.display_rect.left + 1) ;
    p_bookshelf->grid.grid_x_interval = 0;
    p_bookshelf->grid.grid_y_interval = 0;
    
    p_bookshelf->grid.display.layout_rect = p_bookshelf->grid.display.display_rect;    
    if(p_bookshelf->layer_num > per_screen)
    {
        p_bookshelf->grid.display.layout_rect.bottom = p_bookshelf->grid.display.layout_rect.top 
            + p_bookshelf->grid.grid_height * p_bookshelf->grid.row;
    }
    p_bookshelf->grid.display.draw = MMIEBOOK_DrawBookshelfLayer;
    p_bookshelf->grid.slide.allowed_slide_direction = MMI_SLIDE_DIRECTION_Y;
    
    p_bookshelf->grid.track.max_num = sizeof(p_bookshelf->grid.track.point)
        /sizeof(&p_bookshelf->grid.track.point[0]);
    
    MMIGRID_CreateDisplayLayer(&p_bookshelf->grid.display);
    MMIGRID_AppendDisplayLayer(&p_bookshelf->grid.display);
    return 0;
}

//hanlde 5d dicrection key msg
LOCAL MMI_RESULT_E HandleLatestListWinKeyMsg(
                                    MMI_WIN_ID_T win_id,            
                                    MMI_MESSAGE_ID_E msg_id,        
                                    DPARAM param
                                )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    int16 per_layer=0;
    if(!MMITHEME_IsMainScreenLandscape ())
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_V;
    }
    else
    {
        per_layer=MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_H;
    }
    
    if(book_shelf.focused_book_index == -1)
    {
        book_shelf.focused_book_index = 0;
        MMIGRID_HandleGridMsg(&book_shelf.grid,win_id, MSG_FULL_PAINT,  param );
        return result;
    }
    
    switch (msg_id)
    {
    case MSG_APP_UP:
        if(book_shelf.focused_book_index - per_layer >= 0)
        {
             book_shelf.focused_book_index -= per_layer;
        }
        break;
    case MSG_APP_DOWN:
        if(book_shelf.focused_book_index + per_layer  <= book_shelf.book_num)
        {
            book_shelf.focused_book_index += per_layer;
        }
        break;
    case MSG_APP_LEFT:      
        if(book_shelf.focused_book_index - 1 >=
            book_shelf.focused_book_index/per_layer*per_layer)
        {
            book_shelf.focused_book_index -= 1;
        }
        break;
    case MSG_APP_RIGHT: 
        if(book_shelf.focused_book_index + 1 <= per_layer - 1 +
            book_shelf.focused_book_index/per_layer*per_layer)
        {
            book_shelf.focused_book_index += 1;    
        }
        break;
        
    case MSG_APP_WEB:  
        OpenBook(book_shelf.focused_book_index, win_id);
        return result;
        //break;
        
    default:
        break;
    }
    if(book_shelf.focused_book_index < 0) 
    {
        book_shelf.focused_book_index = 0;
    }    
    else if(book_shelf.focused_book_index > book_shelf.book_num)
    {
        book_shelf.focused_book_index = book_shelf.book_num;
    }     
    
    MMIGRID_HandleGridMsg(&book_shelf.grid,win_id, msg_id,  param );
    MMIGRID_HandleGridMsg(&book_shelf.grid,win_id, MSG_FULL_PAINT,  param );  
    return result;
}

/*****************************************************************************/
//  Description : 最近阅读的电子书列表
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLatestListWinMsg(
                                           MMI_WIN_ID_T win_id,            
                                           MMI_MESSAGE_ID_E msg_id,        
                                           DPARAM param
                                           )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
#ifndef MMI_PDA_SUPPORT
    static BOOLEAN  is_clear=FALSE;
    static uint16  cur_select=0;
#endif
    UNUSED_PARAM(param);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_TRACE_LOW("ebook_time:HandleLatestListWinMsg MSG_OPEN_WINDOW start=%d",SCI_GetTickCount());
        CreateLatestListPtr();
        ReadLatestList();
        //MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        //MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);
#ifdef MMI_PDA_SUPPORT
        SetPDAButtonParam(win_id);
        if (!MMITHEME_IsIstyle())
        {
            GUIBUTTON_SetVisible(MMIEBOOK_RETURN_BUTTON_CTRL_ID, FALSE, FALSE);
            GUIBUTTON_SetVisible(MMIEBOOK_MENU_BUTTON_CTRL_ID, FALSE, FALSE);
        }
#if defined(MMI_ISTYLE_SUPPORT)		
        else
        {
            if( PNULL != s_ebook_latest_list_ptr
                && 0 < s_ebook_latest_list_ptr->total_item_num )
            {
                GUIBUTTON_SetVisible(MMIEBOOK_MENU_BUTTON_CTRL_ID, TRUE, FALSE);
            }
            else
            {
                GUIBUTTON_SetVisible(MMIEBOOK_MENU_BUTTON_CTRL_ID, FALSE, FALSE);
            }
        }
#endif		
#endif
        InitBookshelf(&book_shelf,win_id,s_ebook_latest_list_ptr->total_item_num);
       
        if (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetPreTiggerMode())
        {
            book_shelf.focused_book_index = 0;
        }
        else
        {
            book_shelf.focused_book_index = -1;
        }
#ifndef MMI_PDA_SUPPORT
        is_clear=FALSE;
#endif
        SCI_TRACE_LOW ("ebook_time:HandleLatestListWinMsg MSG_OPEN_WINDOW end=%d",SCI_GetTickCount());
        break;
    case MSG_FULL_PAINT:
        if (PNULL == s_ebook_latest_list_ptr)
        {
            //SCI_ASSERT(0);
            break;
        }
        SCI_TRACE_LOW("ebook_time:HandleLatestListWinMsg MSG_FULL_PAINT start=%d",SCI_GetTickCount());
        DrawBookshelfBackground(&book_shelf);
        SCI_TRACE_LOW("ebook_time:HandleLatestListWinMsg MSG_FULL_PAINT end=%d",SCI_GetTickCount());
        break;
    case MSG_GET_FOCUS:
        if (PNULL == s_ebook_latest_list_ptr)
        {
            if(!CreateLatestListPtr())
            {
                MMK_CloseWin( win_id);   
                break;            
            }
        }
        
        ReadLatestList();
        ResetBookshelf(&book_shelf, s_ebook_latest_list_ptr->total_item_num);
        if (!MMITHEME_IsIstyle())
        {
            GUIBUTTON_SetVisible(MMIEBOOK_RETURN_BUTTON_CTRL_ID, FALSE, FALSE);
            GUIBUTTON_SetVisible(MMIEBOOK_MENU_BUTTON_CTRL_ID, FALSE, FALSE);
        }
#if defined(MMI_ISTYLE_SUPPORT)		
        else
        {
            GUIBUTTON_SetVisible(MMIEBOOK_RETURN_BUTTON_CTRL_ID, TRUE, FALSE);
            
            if( PNULL != s_ebook_latest_list_ptr
                && 0 < s_ebook_latest_list_ptr->total_item_num )
            {
                GUIBUTTON_SetVisible(MMIEBOOK_MENU_BUTTON_CTRL_ID, TRUE, FALSE);
            }
            else
            {
                GUIBUTTON_SetVisible(MMIEBOOK_MENU_BUTTON_CTRL_ID, FALSE, FALSE);
            }
        }
#endif		
        if (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetPreTiggerMode())
        {
            book_shelf.focused_book_index = 0;
        }
        else
        {
            book_shelf.focused_book_index = -1;
        }
        MMIGRID_AppendDisplayLayer(&book_shelf.grid.display);
        break;
        
    case MSG_LCD_SWITCH:
#ifdef MMI_PDA_SUPPORT
        SetPDAButtonParam(win_id);
#endif
        MMIGRID_DestroyDisplayLayer(&book_shelf.grid.display);
        InitBookshelf(&book_shelf,win_id,s_ebook_latest_list_ptr->total_item_num);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (MMIEBOOK_MENU_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
        {
            MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CLEAR, IMAGE_PUBWIN_QUERY, PNULL, PNULL);//清空
#ifndef MMI_PDA_SUPPORT
        is_clear=TRUE;
#endif
        }
        else if (MMIEBOOK_RETURN_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
        {
            MMK_PostMsg(win_id, MSG_APP_CANCEL, param, PNULL);
        }   
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_PROMPTWIN_OK:
#ifndef MMI_PDA_SUPPORT
        if(is_clear)
        {
            MMIPUB_CloseQuerytWin(PNULL);
            CleanLatestListItem();
            GUILIST_RemoveAllItems(MMIEBOOK_LATEST_LIST_CTRL_ID);
            MMIPUB_OpenAlertSuccessWin(TXT_DELETED);//@yongwei.he cr94087
        }
        else
        {
            DeleteLatestListItem(cur_select);
            MMIPUB_CloseQuerytWin(PNULL);
        }
        is_clear=FALSE;
#else
        MMIPUB_CloseQuerytWin(PNULL);
        CleanLatestListItem();
        GUILIST_RemoveAllItems(MMIEBOOK_LATEST_LIST_CTRL_ID);
        MMIPUB_OpenAlertSuccessWin(TXT_DELETED);//@yongwei.he cr94087
#endif
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
#ifndef MMI_PDA_SUPPORT
        is_clear=FALSE;
#endif
        break;

    case MSG_APP_UP:  
    case MSG_APP_DOWN:    
    case MSG_APP_LEFT: 
    case MSG_APP_RIGHT:    
    case MSG_APP_WEB:    
        //HandleLatestListWinKeyMsg(win_id, msg_id, param);
       break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)MMIEBOOK_LATEST_LIST_OPT_WIN_TAB,PNULL);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        book_shelf.tp_state = msg_id;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if(book_shelf.tp_state  != MSG_TP_PRESS_DOWN)
        {          
            break;
        }
        if(book_shelf.grid.slide.slide_state != MMK_TP_SLIDE_NONE)
        {            
            break;
        }
        {
            GUI_POINT_T point;
            int32 index=-1;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            index = GetBookIndex(&book_shelf, point);
            OpenBook(index, win_id);
            book_shelf.tp_state = msg_id;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        book_shelf.tp_state = msg_id;        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        if(book_shelf.tp_state  != MSG_TP_PRESS_DOWN)
        {        
            book_shelf.tp_state = msg_id;
            break;
        }
        if(book_shelf.grid.slide.slide_state != MMK_TP_SLIDE_NONE)
        {          
            book_shelf.tp_state = msg_id;
            break;
        }
        {
            GUI_POINT_T point;
            int32 index=-1;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            index = GetBookIndex(&book_shelf, point);
            if(index>0 && index <= (int32)s_ebook_latest_list_ptr->total_item_num)
            {
#ifdef MMI_PDA_SUPPORT
                MMK_CreateWin((uint32 *)MMIEBOOK_LATEST_LIST_POPUP_WIN_TAB,(ADD_DATA)(index-1));
#else
                cur_select=index-1;
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE, IMAGE_PUBWIN_QUERY, PNULL, PNULL);                
#endif
            }

        }
        book_shelf.tp_state = msg_id;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifndef MMI_TASK_MANAGER
        MMK_CloseWin( win_id );      
#else
        MMIAPIEBOOK_StopApplet(FALSE);
#endif
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIGRID_DestroyDisplayLayer(&book_shelf.grid.display);
        DestroyLatestListPtr();
#ifdef MMI_TASK_MANAGER
        //MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
        MMIAPIEBOOK_StopApplet(FALSE);
#endif
#ifndef MMI_PDA_SUPPORT
        is_clear=FALSE;
#endif
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    //do not hanglde key msg 
    if(!(MSG_APP_UP <= msg_id && msg_id <= MSG_APP_RIGHT)
        && MSG_APP_WEB != msg_id)
    {
         MMIGRID_HandleGridMsg(&book_shelf.grid,win_id, msg_id,  param );
    }
    return result;
}
#else

/*****************************************************************************/
//  Description : add latest books to list ctrl
//  Global resource dependence : 
//  Author:  zhaohui
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void AppendLatestListItemData(
                                    MMI_CTRL_ID_T  ctrl_id,
                                    uint16 index
                                    )
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    wchar			file_name[MMIEBOOK_TXT_NAME_MAX_LEN+1] = {0};
    uint16			file_name_len = 0;
    
    //SCI_ASSERT(PNULL != s_ebook_latest_list_ptr);/*assert verified*/
    if (PNULL == s_ebook_latest_list_ptr)
    {
        return;
    }
    item_t.item_style                        = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr                     = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    //在列表框里面列出所有书签名
    item_data.softkey_id[0] = STXT_OPTION;
    if (0 == s_ebook_latest_list_ptr->total_item_num)
    {
        item_data.softkey_id[1] = TXT_NULL;
    }
#ifdef TTS_SUPPORT
#if defined( MMI_GUI_STYLE_MINISCREEN)    
    else//mini界面中间softkey使用图片替换文本
    {
        item_data.softkey_id[1] =(MMI_IMAGE_ID_T)IMAGE_COMMON_SOFTKEY_OK_ICON;//TXT_NULL;
    }
#else
	else
    {
        item_data.softkey_id[1] = TXT_READ;//TXT_NULL;
    }
#endif 
#endif
    item_data.softkey_id[2] = STXT_RETURN;
    item_data.item_content[0].item_data.image_id = IMAGE_EBOOK_BOOKMARK_LIST;//需要更换
    
    MMIAPIFMM_SplitFullPath(s_ebook_latest_list_ptr->item_brief[index].full_name_wstr, s_ebook_latest_list_ptr->item_brief[index].full_name_len,
        NULL, NULL, NULL, NULL,
        file_name, &file_name_len);
    
    item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;
    
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_name;
    
    //     MMI_WSTRNCPY( item_data.item_content[1].item_data.text_buffer.wstr,
    //         item_data.item_content[1].item_data.text_buffer.wstr_len,
    //         file_name,
    //         item_data.item_content[1].item_data.text_buffer.wstr_len,
    //         item_data.item_content[1].item_data.text_buffer.wstr_len);
    
    GUILIST_SetItemData( ctrl_id, &item_data, index );    
}
/*****************************************************************************/
//  Description : 最近阅读的电子书列表
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLatestListWinMsg(
                                           MMI_WIN_ID_T win_id,            
                                           MMI_MESSAGE_ID_E msg_id,        
                                           DPARAM param
                                           )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    
    UNUSED_PARAM(param);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateLatestListPtr();
        ReadLatestList();
        InitLatestListCtrl();
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        MMK_SetAtvCtrl(win_id,MMIEBOOK_LATEST_LIST_CTRL_ID);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            //SCI_ASSERT( PNULL != need_item_data_ptr );/*assert verified*/
            if (PNULL == need_item_data_ptr)
            {
                break;
            }
            index = need_item_data_ptr->item_index;
            AppendLatestListItemData(MMIEBOOK_LATEST_LIST_CTRL_ID,index);
        }
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        if( 0 != s_ebook_latest_list_ptr->total_item_num )
        {
            //初始化s_ebook_file_brief
            MMIEBOOK_CreateFileBrief();    
            if (InitFileBriefFromLatest(GUILIST_GetCurItemIndex(MMIEBOOK_LATEST_LIST_CTRL_ID)))
            {
                //MMK_CreateEbookWin((uint32*)MMIEBOOK_ENTER_TXT_CONTENT_WIN_TAB, PNULL);//资源管理器中入口               
                MMIEBOOK_OpenTxtContentWin(PNULL);
                MMK_CloseWin( win_id );
            }
            else//该电子书已不存在
            {
                MMIEBOOK_DestroyFileBrief();
                MMIPUB_OpenAlertWarningWin(TXT_EBOOK_FILE_NOT_EXIST);
            }     
        }   
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)MMIEBOOK_LATEST_LIST_OPT_WIN_TAB,PNULL);
        break;
        
    case MSG_CTL_LIST_LONGOK:
        if( 0 != s_ebook_latest_list_ptr->total_item_num )
        {
            uint32 index = (uint32)GUILIST_GetCurItemIndex(MMIEBOOK_LATEST_LIST_CTRL_ID);
            MMK_CreateWin((uint32 *)MMIEBOOK_LATEST_LIST_OPT_WIN_TAB,(ADD_DATA)index);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_CLOSE_WINDOW:
        DestroyLatestListPtr();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
#endif
/*****************************************************************************/
//  Description : add latest books to list ctrl
//  Global resource dependence : 
//  Author:  zhaohui
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void InitLatestListCtrl(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIEBOOK_LATEST_LIST_CTRL_ID;
    uint32 i = 0;
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
    GUILIST_SetMaxItem( ctrl_id, s_ebook_latest_list_ptr->total_item_num, TRUE); //max item 10
    
    item_t.item_style                        = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr                     = &item_data;
    
    for ( i = 0; i < s_ebook_latest_list_ptr->total_item_num; i++ )
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }       
    
    GUILIST_SetCurItemIndex(ctrl_id,0);
}

/*****************************************************************************/
//  Description : alloc s_ebook_latest_file_info_ptr
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN CreateLatestListPtr( void )
{
    if (PNULL == s_ebook_latest_list_ptr)
    {
        s_ebook_latest_list_ptr = (MMIEBOOK_LATEST_LIST_T*)SCI_ALLOCA(sizeof(MMIEBOOK_LATEST_LIST_T));
        if(s_ebook_latest_list_ptr == PNULL)
        {
            return FALSE;
        }
    }
    
    SCI_MEMSET(s_ebook_latest_list_ptr, 0, sizeof(MMIEBOOK_LATEST_LIST_T));
    return TRUE;
}

/*****************************************************************************/
//  Description : free s_ebook_latest_file_info_ptr
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void DestroyLatestListPtr( void )
{
    if (PNULL != s_ebook_latest_list_ptr)
    {
        SCI_FREE(s_ebook_latest_list_ptr);
        s_ebook_latest_list_ptr=PNULL;
    }
}

/*****************************************************************************/
//  Description : 往列表头添加数据
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN AddLatestListItem(void)
{
    uint16      i = 0;
    BOOLEAN     is_find = FALSE;	
    MMIEBOOK_FILE_BRIEF_T* s_ebook_file_brief = MMIEBOOK_GetFileBrief();
    
    //SCI_ASSERT(PNULL != s_ebook_latest_list_ptr);/*assert verified*/
    if (PNULL == s_ebook_latest_list_ptr)
    {
        return  FALSE;
    }
    
    if(s_ebook_latest_list_ptr->total_item_num > MMIEBOOK_LATEST_LIST_MAX_NUM)
    {
        SCI_MEMSET(s_ebook_latest_list_ptr,0,sizeof(MMIEBOOK_LATEST_LIST_T));
    }
    
    //判断是否该书已存在与列表中
    for (i = 0; i < s_ebook_latest_list_ptr->total_item_num; i++)
    {
        if (0 == MMIAPICOM_CompareTwoWstr(s_ebook_file_brief->full_name_wstr, s_ebook_file_brief->full_name_len, 
            s_ebook_latest_list_ptr->item_brief[i].full_name_wstr,s_ebook_latest_list_ptr->item_brief[i].full_name_len))
        {
            is_find = TRUE;//相等
            break;
        }
    }
    
    if (is_find)//列表中有相同的文件,则将其提到列表头,但不增加项数
    {
        SCI_MEMSET(&s_ebook_latest_list_ptr->item_brief[i],0,sizeof(MMIEBOOK_FILE_BRIEF_T));//此句是否可有可无
        memmove( &s_ebook_latest_list_ptr->item_brief[1],
            &s_ebook_latest_list_ptr->item_brief[0],
            ( i ) * sizeof(MMIEBOOK_FILE_BRIEF_T) );   	
    }
    else
    {
        memmove( &s_ebook_latest_list_ptr->item_brief[1],
            &s_ebook_latest_list_ptr->item_brief[0],
            ( MMIEBOOK_LATEST_LIST_MAX_NUM - 1) * sizeof(MMIEBOOK_FILE_BRIEF_T) );
        if (s_ebook_latest_list_ptr->total_item_num < MMIEBOOK_LATEST_LIST_MAX_NUM)
        {
            s_ebook_latest_list_ptr->total_item_num += 1;	
        }
    }
    
    MMI_MEMCPY(&s_ebook_latest_list_ptr->item_brief[0],sizeof(MMIEBOOK_FILE_BRIEF_T),
        s_ebook_file_brief,sizeof(MMIEBOOK_FILE_BRIEF_T),sizeof(MMIEBOOK_FILE_BRIEF_T));
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 删除选择的项
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void DeleteLatestListItem(
                                uint16 cur_select//IN: 书签列表窗口中的位置
                                )                                 
{  
    wchar bookmark_file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    
    //SCI_ASSERT(PNULL != s_ebook_latest_list_ptr);/*assert verified*/
    //SCI_PASSERT(cur_select < s_ebook_latest_list_ptr->total_item_num,   /*assert verified*/
        //("MMIEBOOK_DeleteBookmark: cur_select is %d,s_ebook_latest_list_ptr->total_item_num is %d",cur_select,s_ebook_latest_list_ptr->total_item_num));/*assert verified*/		

    if (PNULL == s_ebook_latest_list_ptr || cur_select >= s_ebook_latest_list_ptr->total_item_num)
    {
        return;
    }
    
    MMIEBOOK_GetBookmarkFileName(s_ebook_latest_list_ptr->item_brief[cur_select].full_name_wstr,bookmark_file_full_path);
    MMIAPIFMM_DeleteFile(bookmark_file_full_path, PNULL);//NEWMS00131378
    //将要删除的书签清0
    SCI_MEMSET(&s_ebook_latest_list_ptr->item_brief[cur_select], 0, sizeof(MMIEBOOK_FILE_BRIEF_T));
    
    //将要删除位置之后(包括)的item项前移
    memmove( &s_ebook_latest_list_ptr->item_brief[cur_select],
        &s_ebook_latest_list_ptr->item_brief[cur_select + 1],
        ( s_ebook_latest_list_ptr->total_item_num - 1 - cur_select ) * sizeof(MMIEBOOK_FILE_BRIEF_T) );	
    
    s_ebook_latest_list_ptr->total_item_num --;
    
    //将原先最后一个item占用的信息清零
    SCI_MEMSET(&s_ebook_latest_list_ptr->item_brief[s_ebook_latest_list_ptr->total_item_num], 0, sizeof(MMIEBOOK_FILE_BRIEF_T));
    
    SaveLatestList();
}

/*****************************************************************************/
//  Description : 删除所有item
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void CleanLatestListItem(void)
{
    //SCI_ASSERT(PNULL != s_ebook_latest_list_ptr);/*assert verified*/
    if(PNULL != s_ebook_latest_list_ptr)
    {
        SCI_MEMSET(s_ebook_latest_list_ptr, 0, sizeof(MMIEBOOK_LATEST_LIST_T));
        
        SaveLatestList();
    }
}

/*****************************************************************************/
//  Description : 读取最近阅读的电子书信息
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN ReadLatestList(void)
{
    BOOLEAN read_ok = FALSE;
    int32  i = 0;
    
    //SCI_ASSERT(!MMIEBOOK_IsLatestListFileOpen());/*assert verified*/
    //SCI_ASSERT(PNULL != s_ebook_latest_list_ptr);/*assert verified*/
    if (PNULL == s_ebook_latest_list_ptr || MMIEBOOK_IsLatestListFileOpen())
    {
        return FALSE;
    }
    SCI_TRACE_LOW("ebook_time:ReadLatestList start=%d",SCI_GetTickCount());
    for (i = MMI_DEVICE_NUM -1; i >= MMI_DEVICE_UDISK; i--)//sd优先
    {
        if (!MMIEBOOK_IsLatestListFileOK((MMIFILE_DEVICE_E)i))
        {
            MMIEBOOK_CreateLatestListFile((MMIFILE_DEVICE_E)i);
        }
        
        if (MMIEBOOK_OpenLatestListFile((MMIFILE_DEVICE_E)i, "r+"))
        {
            read_ok = MMIEBOOK_ReadLatestListFile(
                MMIEBOOK_GetLatestListFileHandle(),
                MMIEBOOK_LATEST_LIST_FILE_INFO,
                s_ebook_latest_list_ptr
                );
            
            MMIEBOOK_CloseLatestListFile();
            
            if ((read_ok)&&
                (s_ebook_latest_list_ptr->total_item_num <= MMIEBOOK_LATEST_LIST_MAX_NUM)//读出数据合理
                )
            {
                return read_ok;
            }
            else
            {
                MMIEBOOK_DeleteLatestListFile((MMIFILE_DEVICE_E)i);
                read_ok=FALSE;
            }
        }
    }
    SCI_TRACE_LOW("ebook_time:ReadLatestList end=%d",SCI_GetTickCount());
    //SCI_TRACE_LOW:"mmiebook_wintab.c ReadLatestList read_ok is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_BOOKSHELF_1753_112_2_18_2_15_11_1,(uint8*)"d",read_ok);
    return read_ok;
}

/*****************************************************************************/
//  Description : 及时保存相关详情项信息
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN SaveLatestList(void)//保存最近阅读的电子书信息
{
    BOOLEAN write_ok = TRUE;
    int32 i = 0;
    
    //SCI_ASSERT(!MMIEBOOK_IsLatestListFileOpen());/*assert verified*/
    //SCI_ASSERT(PNULL != s_ebook_latest_list_ptr);/*assert verified*/
    if (PNULL == s_ebook_latest_list_ptr || MMIEBOOK_IsLatestListFileOpen())
    {
        return FALSE;
    }
    
    for (i = MMI_DEVICE_NUM -1; i >= MMI_DEVICE_UDISK; i--)//sd优先
    {
        if (!MMIEBOOK_IsLatestListFileOK((MMIFILE_DEVICE_E)i))
        {
            MMIEBOOK_CreateLatestListFile((MMIFILE_DEVICE_E)i);
        }
        
        if (MMIEBOOK_OpenLatestListFile((MMIFILE_DEVICE_E)i, "r+"))
        {
            write_ok = MMIEBOOK_WriteLatestListFile(
                MMIEBOOK_GetLatestListFileHandle(),
                MMIEBOOK_LATEST_LIST_FILE_INFO,
                s_ebook_latest_list_ptr
                );
            
            MMIEBOOK_CloseLatestListFile();
            
            if (write_ok)
            {
                return write_ok;
            }
            else
            {
                MMIEBOOK_DeleteLatestListFile((MMIFILE_DEVICE_E)i);
            }
        }
    }
    
    return write_ok;
}

/*****************************************************************************/
//  Description : 用最近一次打开的电子书信息来初始化s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->存在
/*****************************************************************************/
LOCAL BOOLEAN InitFileBriefFromLatest( uint16 cur_selection )
{
    BOOLEAN result = FALSE;
    MMIEBOOK_FILE_BRIEF_T* s_ebook_file_brief = MMIEBOOK_GetFileBrief();
    
    //SCI_ASSERT(PNULL != s_ebook_latest_list_ptr);/*assert verified*/ 
    if(PNULL == s_ebook_latest_list_ptr)
    {
        return result;
    }
    
    if (0 != s_ebook_latest_list_ptr->total_item_num)//没有阅读过电子书
    {
        //判断该文件是否还存在？
        if (MMIEBOOK_IsFileExist(s_ebook_latest_list_ptr->item_brief[cur_selection].full_name_wstr,s_ebook_latest_list_ptr->item_brief[cur_selection].full_name_len,(uint32)(s_ebook_latest_list_ptr->item_brief[cur_selection].file_size)))
        {
            //给s_ebook_file_brief赋值
            MMI_MEMCPY(s_ebook_file_brief,sizeof(MMIEBOOK_FILE_BRIEF_T),
                &s_ebook_latest_list_ptr->item_brief[cur_selection],sizeof(MMIEBOOK_FILE_BRIEF_T),sizeof(MMIEBOOK_FILE_BRIEF_T));      
            result = TRUE;
        }    
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 在列表中寻找最近一次打开的电子书
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_FindLastOpenedBookinList( void )
{
    BOOLEAN     result = FALSE;
    
    CreateLatestListPtr();
    if (!ReadLatestList())
    {
        DestroyLatestListPtr();
        return result;
    }
    
    result = InitFileBriefFromLatest(0);//最近的一本,入参为零
    
    DestroyLatestListPtr();
    
    return result;
}

/*****************************************************************************/
//  Description : 在列表中寻找最近一次打开的电子书
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetLastOpenedBookPath( wchar *p_path_name, uint16 *p_path_name_len)
{
    BOOLEAN     result = FALSE;
    wchar file_name[MMIEBOOK_FULLNAME_MAX_LEN + 1] = {0};
    uint16 file_name_len = 0;
    wchar               dir_name[MMIEBOOK_TXT_NAME_MAX_LEN+1] = {0};
    uint16              dir_name_len = 0;
    
    CreateLatestListPtr();
    if (!ReadLatestList())
    {
        DestroyLatestListPtr();
        return result;
    }
    
    file_name_len = MMIEBOOK_FULLNAME_MAX_LEN;
    MMIAPIFMM_SplitFullPath(s_ebook_latest_list_ptr->item_brief[0].full_name_wstr, s_ebook_latest_list_ptr->item_brief[0].full_name_len,
        PNULL, PNULL, dir_name, &dir_name_len, 
        file_name, &file_name_len);          
    
    MMIEBOOK_GetFileFullPathUCS2(//该电子书所在路径
        s_ebook_latest_list_ptr->item_brief[0].file_dev,
        dir_name,
        dir_name_len,
        PNULL,
        0,
        p_path_name,
        p_path_name_len
        );
#if defined( MMI_GUI_STYLE_MINISCREEN)      
    MMIAPIFMM_ShowTxtContent(s_ebook_latest_list_ptr->item_brief[0].full_name_wstr);
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : 打开电子书时,更新最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:v
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_UpdateLatestList(void)
{
    BOOLEAN result = TRUE;
    
    if(!CreateLatestListPtr())
    {
        return FALSE;
    }
    if (!ReadLatestList())
    {
        DestroyLatestListPtr();
        result = FALSE;
        return result;
    }
    AddLatestListItem();
    if (!SaveLatestList())
    {
        result = FALSE;
    }
    DestroyLatestListPtr();
    return result;
}

/*****************************************************************************/
//  Description : 电子书列表选项窗口的处理函数
//  Global resource dependence : g_textfile_all_info
//   Author: songbin.zeng
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTxtListOptWinMsg (
                                            MMI_WIN_ID_T win_id,            
                                            MMI_MESSAGE_ID_E msg_id,        
                                            DPARAM param
                                            )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMIEBOOK_FMM_CALLBACK_INFO* fmm_callback_info_ptr = PNULL;
    
    fmm_callback_info_ptr = (MMIEBOOK_FMM_CALLBACK_INFO*)MMK_GetWinAddDataPtr(win_id);
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        break;
        
    case MSG_OPEN_WINDOW:
        EnableTxtListOptMenuItem(win_id,fmm_callback_info_ptr);
        MMK_SetAtvCtrl(win_id, MMIEBOOK_TXT_LIST_OPT_CTRL_ID);
        break;
        
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        HandleTxtListOptWinOKKey(fmm_callback_info_ptr);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != fmm_callback_info_ptr)
        {
            SCI_FREE(fmm_callback_info_ptr);
        }
        break;
        
    case MSG_APP_LEFT:      
    case MSG_APP_RIGHT:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : enablel menu item
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
#ifndef MMI_PDA_SUPPORT
LOCAL void EnableTxtListOptMenuItem(
                                    MMI_WIN_ID_T                win_id,
                                    MMIEBOOK_FMM_CALLBACK_INFO  *fmm_callback_info_ptr
                                    )
{
    if((0 == fmm_callback_info_ptr->list_data_ptr->file_num)&&
        (0 == fmm_callback_info_ptr->list_data_ptr->folder_num))//当前目录为空
    {
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DETAIL, TRUE);//详情(灰显)
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_CURRENT_BOOKS, FALSE);//最近阅读的书籍
#ifdef BLUETOOTH_SUPPORT
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDBT, TRUE);//蓝牙发送(灰显)
#endif
#ifdef MMS_SUPPORT
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDMMS, TRUE);//彩信发送(灰显)
#endif
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DELETE, TRUE);//删除(灰显)
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_ADVANCE_OPT, ID_EBOOK_LIST_MAKEABLE, TRUE);//标记(灰显)
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_RENAME, TRUE);//重命名(灰显)
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_ADVANCE_OPT, FALSE);//高级
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_COPY, TRUE);//复制(灰显)
    }
    else 
    {
        MMIFMM_FOLDER_TYPE_E    folder_type = MMIFMM_FOLDER_TYPE_NORMAL;
        
        folder_type =  MMIAPIFMM_GetFolderType(fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->filename_ptr, 
            fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->filename_len, FALSE);
        
        if (MMIFMM_FILE_TYPE_FOLDER == fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->type)//当前项为文件夹            
        {
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DETAIL, FALSE);//详情
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_CURRENT_BOOKS, FALSE);//最近阅读的书籍
#ifdef BLUETOOTH_SUPPORT
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDBT, TRUE);//蓝牙发送(灰显)
#endif
#ifdef MMS_SUPPORT
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDMMS, TRUE);//彩信发送(灰显)
#endif
            // 对于根目录下的4个默认文件夹(ebook,audio,video,photos)，不可删除
            if ((1 == fmm_callback_info_ptr->list_data_ptr->path_depth)&&//根目录
                (MMIFMM_FOLDER_TYPE_NORMAL != folder_type))//默认文件夹
            {
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DELETE, TRUE);//删除(灰显)           
            }
            else//普通文件夹
            {
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DELETE, FALSE);//删除   
            }
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_ADVANCE_OPT, ID_EBOOK_LIST_MAKEABLE, FALSE);//标记
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_RENAME, TRUE);//重命名(灰显)
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_ADVANCE_OPT, FALSE);//高级
        }
        else//当前项为电子书文件
        {
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DETAIL, FALSE);//详情
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_CURRENT_BOOKS, FALSE);//最近阅读的书籍
#ifdef BLUETOOTH_SUPPORT
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDBT, FALSE);//蓝牙发送
#endif
#ifdef MMS_SUPPORT
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDMMS, FALSE);//彩信发送(灰显)
#endif
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DELETE, FALSE);//删除
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_ADVANCE_OPT, ID_EBOOK_LIST_MAKEABLE, FALSE);//标记
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_RENAME, FALSE);//设置
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_ADVANCE_OPT, FALSE);//高级
        }
        
        switch (fmm_callback_info_ptr->list_data_ptr->checked)
        {
        case MMIFMM_FILE_STATE_ALL_CHECKED:
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DETAIL, TRUE);//详情
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARK, TRUE);//标记(灰显)
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARK, FALSE);//取消标记
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARKALL, TRUE);//标记所有(灰显)
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARKALL, FALSE);//取消所有标记
#ifdef BLUETOOTH_SUPPORT
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDBT, TRUE);//蓝牙发送(灰显)
#endif
#ifdef MMS_SUPPORT
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDMMS, TRUE);//彩信发送(灰显)
#endif            
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_RENAME, TRUE);//重命名(灰显)
            break;
            
        case MMIFMM_FILE_STATE_CHECKED:
            if (MMIFMM_FILE_STATE_CHECKED == fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->state)
            {
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARK, TRUE);//标记(灰显)
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARK, FALSE);//取消标记
            }
            else
            {
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARK, FALSE);//标记
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARK, TRUE);//取消标记(灰显)
            }
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DETAIL, TRUE);//详情
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARKALL, FALSE);//标记所有
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARKALL, FALSE);//取消所有标记
            if(MMIFMM_FILE_TYPE_FOLDER !=fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->type)
            {
#ifdef BLUETOOTH_SUPPORT
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDBT, TRUE);//蓝牙发送(灰显)
#endif
#ifdef MMS_SUPPORT
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDMMS, TRUE);//彩信发送(灰显)
#endif
            }            
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_RENAME, TRUE);//重命名(灰显)
            break;
            
        case MMIFMM_FILE_STATE_UNCHECKED:
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_DETAIL, FALSE);//详情
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARK, FALSE);//标记
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARK, TRUE);//取消标记(灰显)
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARKALL, FALSE);//标记所有
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARKALL, TRUE);//取消所有标记(灰显)
            if(MMIFMM_FILE_TYPE_FOLDER !=fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->type)
            {
#ifdef BLUETOOTH_SUPPORT
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDBT, FALSE);//蓝牙发送(灰显)
#endif
#ifdef MMS_SUPPORT
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_SENDMMS, FALSE);//彩信发送(灰显)
#endif
            }            
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_RENAME, FALSE);//重命名(灰显)
            break;
            
        default:
            //SCI_PASSERT(0,(("EnableTxtListOptMenuItem list_data_ptr->checked = %d"),fmm_callback_info_ptr->list_data_ptr->checked));/*assert verified*/
            //SCI_TRACE_LOW:"EnableTxtListOptMenuItem list_data_ptr->checked = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_BOOKSHELF_2117_112_2_18_2_15_12_2,(uint8*)"d",fmm_callback_info_ptr->list_data_ptr->checked);
            break;    
        }
    }
}
#else
LOCAL void EnableTxtListOptMenuItem(
                                    MMI_WIN_ID_T                win_id,
                                    MMIEBOOK_FMM_CALLBACK_INFO  *fmm_callback_info_ptr
                                    )
{
    if((0 == fmm_callback_info_ptr->list_data_ptr->file_num)&&
        (0 == fmm_callback_info_ptr->list_data_ptr->folder_num))//当前目录为空
    {
        GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_LIST_MAKEABLE, TRUE);//标记(灰显)
    }
    else 
    {
        MMIFMM_FOLDER_TYPE_E    folder_type = MMIFMM_FOLDER_TYPE_NORMAL;
        
        folder_type =  MMIAPIFMM_GetFolderType(fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->filename_ptr, 
            fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->filename_len, FALSE);
        
        if (MMIFMM_FILE_TYPE_FOLDER == fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->type)//当前项为文件夹            
        {
            
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_LIST_MAKEABLE, FALSE);//标记
            
        }
        else//当前项为电子书文件
        {
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_TXT_LIST_OPT, ID_EBOOK_LIST_MAKEABLE, FALSE);//标记
        }
        
        switch (fmm_callback_info_ptr->list_data_ptr->checked)
        {
        case MMIFMM_FILE_STATE_ALL_CHECKED:
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARK, TRUE);//标记(灰显)
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARK, FALSE);//取消标记
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARKALL, TRUE);//标记所有(灰显)
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARKALL, FALSE);//取消所有标记
            break;
            
        case MMIFMM_FILE_STATE_CHECKED:
            if (MMIFMM_FILE_STATE_CHECKED == fmm_callback_info_ptr->list_data_ptr->data[fmm_callback_info_ptr->index]->state)
            {
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARK, TRUE);//标记(灰显)
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARK, FALSE);//取消标记
            }
            else
            {
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARK, FALSE);//标记
                GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARK, TRUE);//取消标记(灰显)
            }
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARKALL, FALSE);//标记所有
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARKALL, FALSE);//取消所有标记
            
            break;
            
        case MMIFMM_FILE_STATE_UNCHECKED:
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARK, FALSE);//标记
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARK, TRUE);//取消标记(灰显)
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_MARKALL, FALSE);//标记所有
            GUIMENU_SetItemGrayed(MMIEBOOK_TXT_LIST_OPT_CTRL_ID,MENU_EBOOK_LIST_MARK_OPT, ID_EBOOK_LIST_CANCEL_MARKALL, TRUE);//取消所有标记(灰显)
            break;
            
        default:
            //SCI_PASSERT(0,(("EnableTxtListOptMenuItem list_data_ptr->checked = %d"),fmm_callback_info_ptr->list_data_ptr->checked));/*assert verified*/
            //SCI_TRACE_LOW:"EnableTxtListOptMenuItem list_data_ptr->checked = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_BOOKSHELF_2185_112_2_18_2_15_12_3,(uint8*)"d",fmm_callback_info_ptr->list_data_ptr->checked);
            break;    
        }
    }
}
#endif


/*****************************************************************************/
//  Description : 最近阅读的电子书列表选项
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLatestListOptWinMsg(
                                              MMI_WIN_ID_T win_id,            
                                              MMI_MESSAGE_ID_E msg_id,        
                                              DPARAM param
                                              )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T     group_id = 0; 
    MMI_MENU_ID_T           menu_id  = 0;
    uint16                  cur_selection = 0;
    
    UNUSED_PARAM(param);
    
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        break;
        
    case MSG_OPEN_WINDOW:
        EnableLatestListOptMenuItem(win_id);
        MMK_SetAtvCtrl(win_id, MMIEBOOK_LATEST_LIST_OPT_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(MMIEBOOK_LATEST_LIST_OPT_CTRL_ID, &group_id, &menu_id);
        switch (menu_id)
        {
            
        case ID_EBOOK_DELETE:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case ID_EBOOK_CLEAR:
            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,PNULL,PNULL);//清空
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );      
            break;
            
        case MSG_CLOSE_WINDOW:
            break;
            
        case MSG_APP_LEFT:      
        case MSG_APP_RIGHT:
            break;
            
        case MSG_PROMPTWIN_OK:
            MMIPUB_CloseQuerytWin(PNULL);
            GUIMENU_GetId(MMIEBOOK_LATEST_LIST_OPT_CTRL_ID, &group_id, &menu_id);
            switch (menu_id)
            {
            case ID_EBOOK_DELETE:
                cur_selection = GUILIST_GetCurItemIndex(MMIEBOOK_LATEST_LIST_CTRL_ID); 
                //SCI_PASSERT(cur_selection < s_ebook_latest_list_ptr->total_item_num,     /*assert verified*/   
                   // ("HandleLatestListOptWinMsg: cur_selection is %d,ebook_latest_list_ptr->total_item_num is %d",cur_selection,s_ebook_latest_list_ptr->total_item_num)); /*assert verified*/           
                if (s_ebook_latest_list_ptr == PNULL || cur_selection >= s_ebook_latest_list_ptr->total_item_num)
                {
                    break;
                }
                
                DeleteLatestListItem(cur_selection);
                GUILIST_RemoveItem(MMIEBOOK_LATEST_LIST_CTRL_ID, cur_selection);
                
                {   
                    MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                    MMK_CloseWin( win_id );
                }
                if (0 == s_ebook_latest_list_ptr->total_item_num)
                {
                    GUIWIN_SetSoftkeyTextId(MMIEBOOK_LATEST_LIST_WIN_ID, STXT_OPTION, PNULL, STXT_RETURN, FALSE);
                }
                break;
                
            case ID_EBOOK_CLEAR:
                CleanLatestListItem();
                GUILIST_RemoveAllItems(MMIEBOOK_LATEST_LIST_CTRL_ID);
                //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);//@yongwei.he cr94087
                MMK_CloseWin( win_id );
                GUIWIN_SetSoftkeyTextId(MMIEBOOK_LATEST_LIST_WIN_ID, STXT_OPTION, PNULL, STXT_RETURN, FALSE);
                break;
                
            default:
                break;
            }
            break;
            
            case MSG_PROMPTWIN_CANCEL:
                MMIPUB_CloseQuerytWin(PNULL);
                break;
                
            default:
                result = MMI_RESULT_FALSE;
                break;
    }
    
    return (result);
                                              }
                                              
#ifdef MMIEBOOK_BOOKSHELF_SUPPORT
LOCAL MMI_RESULT_E  HandleLatestListPopupMenuWinMsg(
  MMI_WIN_ID_T win_id,            
  MMI_MESSAGE_ID_E msg_id,        
  DPARAM param
  )
{
  MMI_RESULT_E            result = MMI_RESULT_TRUE;
  //MMI_MENU_GROUP_ID_T     group_id = 0; 
  //MMI_MENU_ID_T           menu_id  = 0;
  uint32                  cur_selection =  (uint32)MMK_GetWinAddDataPtr(win_id);
  MMI_CTRL_ID_T           ctrl_id = MMIEBOOK_LATEST_LIST_OPT_CTRL_ID;  
  UNUSED_PARAM(param);
  
  switch (msg_id)
  {
  case MSG_FULL_PAINT:
      break;
      
  case MSG_OPEN_WINDOW:
      {
          MMI_STRING_T kstring = {0};
          uint32       i = 0;
          wchar			file_name[MMIEBOOK_TXT_NAME_MAX_LEN+1] = {0};
          uint16			file_name_len = 0;
          MMI_IMAGE_ID_T image_id = 0;
          //creat dynamic menu
          GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP_AUTO);
          
          MMIAPIFMM_SplitFullPath(s_ebook_latest_list_ptr->item_brief[cur_selection].full_name_wstr, s_ebook_latest_list_ptr->item_brief[cur_selection].full_name_len,
              NULL, NULL, NULL, NULL,
              file_name, &file_name_len);
          
          kstring.wstr_ptr = file_name;
          kstring.wstr_len = file_name_len;
          
          GUIMENU_SetMenuTitle(&kstring,ctrl_id);   
          
#ifdef MMI_PDA_SUPPORT
          image_id = IMAGE_COMMON_OPTMENU_DELETE;
#else
          image_id = 0;
#endif
          InsertNodeByLabel(
              ctrl_id,//ctrl_id
              TXT_DELETE,//text_ptr 
              image_id,
              ID_EBOOK_DELETE & 0x00ff ,//node_id
              0, //parent_node_id
              i++//index
              );
      }
      MMK_SetAtvCtrl(win_id, ctrl_id);
      break;
      
  case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
  case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
  case MSG_CTL_OK:
  case MSG_APP_WEB:
  case MSG_CTL_MIDSK:
      {
          uint32 item_id = GUIMENU_GetCurNodeId(ctrl_id) + (MMI_MODULE_EBOOK << 16);         
          switch(item_id)
          {                
          case ID_EBOOK_DELETE:
              DeleteLatestListItem(cur_selection);
              GUILIST_RemoveItem(ctrl_id, cur_selection);
              
              break;                
              
          default:
              break;
          }
          MMK_CloseWin( win_id );      
      }
      break;
      
  case MSG_APP_CANCEL:
  case MSG_CTL_CANCEL:
      MMK_CloseWin( win_id );      
      break;
      
  case MSG_CLOSE_WINDOW:
      break;
      
  default:
      result = MMI_RESULT_FALSE;
      break;
  }
  
  return (result);
}
#endif

/*****************************************************************************/
//  Description : enablel Latest List Opt menu item
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void EnableLatestListOptMenuItem(
  MMI_WIN_ID_T win_id
  )
{
    //SCI_ASSERT(PNULL != s_ebook_latest_list_ptr); /*assert verified*/
    if (PNULL == s_ebook_latest_list_ptr)
    {
        return;
    }
  
    if( 0 == s_ebook_latest_list_ptr->total_item_num )
    {
        GUIMENU_SetItemGrayed(MMIEBOOK_LATEST_LIST_OPT_CTRL_ID,MENU_EBOOK_LATEST_LIST_OPT, ID_EBOOK_DELETE, TRUE);//删除
        GUIMENU_SetItemGrayed(MMIEBOOK_LATEST_LIST_OPT_CTRL_ID,MENU_EBOOK_LATEST_LIST_OPT, ID_EBOOK_CLEAR, TRUE);//清空
    }
    else
    {
        GUIMENU_SetItemGrayed(MMIEBOOK_LATEST_LIST_OPT_CTRL_ID,MENU_EBOOK_LATEST_LIST_OPT, ID_EBOOK_DELETE, FALSE);//删除
        GUIMENU_SetItemGrayed(MMIEBOOK_LATEST_LIST_OPT_CTRL_ID,MENU_EBOOK_LATEST_LIST_OPT, ID_EBOOK_CLEAR, FALSE);//清空
    }   
}

/*****************************************************************************/
//  Description : 资源管理中响应按键的callback函数
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_OpenPopmenuCallBack(MMIFMM_DATA_INFO_T *list_data_ptr,uint32 index)
{
  MMIEBOOK_FMM_CALLBACK_INFO* fmm_callback_info_ptr = (MMIEBOOK_FMM_CALLBACK_INFO*)SCI_ALLOC_APP(sizeof(MMIEBOOK_FMM_CALLBACK_INFO));
  SCI_MEMSET(fmm_callback_info_ptr, 0, sizeof(MMIEBOOK_FMM_CALLBACK_INFO));      
  fmm_callback_info_ptr->list_data_ptr = list_data_ptr;
  fmm_callback_info_ptr->index = index;
  
  //打开popmenu
  MMK_CreateWin((uint32 *)MMIEBOOK_TXT_LIST_OPT_WIN_TAB,(ADD_DATA)fmm_callback_info_ptr);
}

/*****************************************************************************/
//  Description : handle ok key when in txt list opt win
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void HandleTxtListOptWinOKKey( MMIEBOOK_FMM_CALLBACK_INFO* fmm_callback_info ) 
{
  MMI_WIN_ID_T            win_id = MMIEBOOK_TXT_LIST_OPT_WIN_ID;
  MMI_MENU_GROUP_ID_T     group_id = 0; 
  MMI_MENU_ID_T           menu_id  = 0;
  
  GUIMENU_GetId(MMIEBOOK_TXT_LIST_OPT_CTRL_ID, &group_id, &menu_id);
  switch (menu_id)
  {
  case ID_EBOOK_DETAIL://详情
      MMIAPIFMM_HandleMenuOpt(MMIFMM_DETAIL_OPT);
      MMK_CloseWin( win_id );
      break;
      
  case ID_EBOOK_RENAME://重命名
      MMIAPIFMM_HandleMenuOpt(MMIFMM_RENAME_OPT);
      MMK_CloseWin( win_id );
      break;   

  case ID_EBOOK_COPY:
      MMIAPIFMM_HandleMenuOpt(MMIFMM_COPY_OPT);
      MMK_CloseWin( win_id );
      break;   
      
  case ID_EBOOK_CURRENT_BOOKS://最近阅读的电子书
#ifndef MMI_TASK_MANAGER
      MMK_CreateEbookWin((uint32*)MMIEBOOK_LATEST_LIST_WIN_TAB, PNULL); 
#else//@fen.xie
      if (MMK_IsOpenWin(MMIEBOOK_LATEST_LIST_WIN_ID))
      {
          MMK_WinGrabFocus(MMIEBOOK_LATEST_LIST_WIN_ID);
      }
      else
      {
        MMK_CreateEbookWin((uint32*)MMIEBOOK_LATEST_LIST_WIN_TAB, PNULL); 
      }
#endif/*#ifndef MMI_TASK_MANAGER*/        
      MMK_CloseWin(win_id);
      break;
      
      //#ifdef MMI_BLUETOOTH          
  case ID_EBOOK_SENDBT://蓝牙发送
      SendTxtFileByBt(fmm_callback_info);
      MMK_CloseWin( win_id );
      break;
      //#endif
  case ID_EBOOK_SENDMMS:
      EbookSendMMS(fmm_callback_info);
      MMK_CloseWin( win_id );
      break;
      
  case ID_EBOOK_DELETE://删除
      MMIAPIFMM_HandleMenuOpt(MMIFMM_DELETE_OPT);
      MMK_CloseWin( win_id );
      break;
      
  case ID_EBOOK_CODING_TYPE://编码方式
      MMIEBOOK_OpenCodeTypeWin(PNULL);
      MMK_CloseWin( win_id );
      break;     
      
  case ID_EBOOK_LIST_MARK://标记
      MMIAPIFMM_HandleMenuOpt(MMIFMM_SUB_MARK_OPT);
      MMK_CloseWin(win_id);
      break;
      
  case ID_EBOOK_LIST_CANCEL_MARK://取消标记
      MMIAPIFMM_HandleMenuOpt(MMIFMM_CANCEL_MARK_OPT);
      MMK_CloseWin(win_id);
      break;
      
  case ID_EBOOK_LIST_MARKALL://全部标记
      MMIAPIFMM_HandleMenuOpt(MMIFMM_MARK_ALL_OPT);
      MMK_CloseWin(win_id);
      break;
      
  case ID_EBOOK_LIST_CANCEL_MARKALL://取消全部标记
      MMIAPIFMM_HandleMenuOpt(MMIFMM_CANCEL_ALL_MARK_OPT);
      MMK_CloseWin(win_id);
      break;
      
#ifdef TTS_SUPPORT
  case ID_EBOOK_SET_TTS_OPEN_CLOSE:
      //MMK_CreatePubListWin((uint32*)MMIEBOOK_SET_TTS_OPEN_CLOSE_WIN_TAB,PNULL);
      MMIEBOOK_OpenTTSOpenCloseWin(PNULL);
      MMK_CloseWin( win_id );
      break;
#ifdef EBOOK_TTS_SELF_VOLUME  
  case ID_EBOOK_SET_TTS_VOL:
      //MMK_CreateWin((uint32*)MMIEBOOK_SET_TTS_VOL_WIN_TAB,PNULL);
      MMIEBOOK_OpenTTSVolumeSetWin(PNULL);
      MMK_CloseWin( win_id );
      break;
#endif
#endif
  case ID_EBOOK_AUTO_READ://自动浏览设置 
      MMIEBOOK_OpenAutoReadSetWin(PNULL);
      MMK_CloseWin( win_id );
      break;
      
  default:
      break;
  }
}

PUBLIC MMI_HANDLE_T MMIEBOOK_OpenBookListWin(ADD_DATA add_data_ptr)
{
    return MMK_CreateEbookWin((uint32*)MMIEBOOK_LATEST_LIST_WIN_TAB,add_data_ptr);
}
#endif


/*Edit by script, ignore 4 case. Thu Apr 26 19:00:53 2012*/ //IGNORE9527


/*Edit by script, ignore 5 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
