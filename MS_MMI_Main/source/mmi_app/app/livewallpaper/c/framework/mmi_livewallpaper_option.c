/*****************************************************************************
** File Name:      mmi_option.c                                      *
** Author:         danmin.xu                                                 *
** Date:                                                                     *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                      *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**                                   *
******************************************************************************/
#define _MMI_LIVEWALLPAPER_OPTION_C_
#include "mmi_livewallpaper_option.h"
#include "mmiset_wintab.h"
#include "mmiset_internal.h"
#include "mmilivewallpaper_export.h"

#define  BAR_WIDTH    (MMI_MAINSCREEN_WIDTH*2/3)
#define  BAR_HEIGHT   6

#define CTRLNUM_PER_FORM   4   //一个子FORM控件最多包含的子控件
#define MAXNUM_PER_MENU    10  //OPTION菜单最多允许的设置项个数

//控件ID 
enum
{
    OPTION_CTRL_ID_START = (MMI_MODULE_COMMON << 16),
    OPTION_SELECT_LIST_CTRL_ID,    //列表窗口列表控件ID
    OPTION_FORM_CTRL_ID,
    OPTION_FORM_CHIRD_CTRL_ID
};

//////////////////////////////////////////////////////////////
//静态变量
LOCAL MMI_CTRL_ID_T ctrl_group[MAXNUM_PER_MENU];
LOCAL uint16 ctrl_count=0;    //主CTRL个数
LOCAL MMI_HANDLE_T cur_label_id=0;  //当前选中的LABEL控件ID
LOCAL OPTIONLIST *pmenu=PNULL;

/*****************************************************************************/
//  Description : OPTION列表窗口回调函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOptionListWinMsg(
								  MMI_WIN_ID_T win_id, 
								  MMI_MESSAGE_ID_E msg_id, 
								  DPARAM param
								  );

/*****************************************************************************/
//  Description : RADIO 选择列表窗口回调函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectListWinMsg(
										  MMI_WIN_ID_T win_id, 
										  MMI_MESSAGE_ID_E msg_id, 
										  DPARAM param
										  );

//OPTION列表窗口
WINDOW_TABLE(MMI_LIVE_OPTION_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleOptionListWinMsg),
    WIN_ID(MMILIVEWALLPAPER_OPTION_WIN_ID),  //这里暂时先拿这个ID顶一下，后面重新定义个
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,OPTION_FORM_CTRL_ID), //列表FORM控件

    END_WIN
};

//RADIO选择列表窗口
WINDOW_TABLE( MMI_LIVE_SELECT_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSelectListWinMsg ),    
    WIN_ID( MMILIVEWALLPAPER_RADIO_WIN_ID ),  //这里暂时先拿这个ID顶一下，后面重新定义个
    WIN_TITLE( TXT_NULL ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, OPTION_SELECT_LIST_CTRL_ID),
    END_WIN
};

/*****************************************************************************/
//  Description : 创建OPTION窗口
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMI_CreateOptionWin(OPTIONLIST *plist)
{
    //用结构指针作为窗口参数传入
    MMK_CreateWin((uint32*)MMI_LIVE_OPTION_WIN_TAB,(ADD_DATA)plist);
}

/*****************************************************************************/
//  Description : 创建一个FORM子控件
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateFormChildCtrl(
							   MMI_WIN_ID_T win_id, 
							   MMI_HANDLE_T child_handle,
							   GUIFORM_LAYOUT_E layout_type
							   )
{
    GUIFORM_INIT_DATA_T form_init = {0};
    GUIFORM_DYNA_CHILD_T child_ctrl={0};

    form_init.layout_type = layout_type;    

    child_ctrl.guid = SPRD_GUI_FORM_ID;
    child_ctrl.is_bg = FALSE;
    child_ctrl.is_get_active = TRUE;
    child_ctrl.init_data_ptr = &form_init;
    child_ctrl.child_handle = child_handle;

    GUIFORM_CreatDynaChildCtrl(win_id, OPTION_FORM_CTRL_ID, &child_ctrl);
    GUIFORM_SetStyle(child_handle,GUIFORM_STYLE_UNIT);
}

/*****************************************************************************/
//  Description : 创建一个LABEL子控件
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateLabelChildCtrl(
								MMI_WIN_ID_T win_id,      //窗口ID
								MMI_HANDLE_T form_handle, //其父控件FORM的HANDLE
								MMI_HANDLE_T child_handle,//给此LABEL的HANDLE 
								MMI_TEXT_ID_T text_id,    //要显示的字符串ID
								BOOLEAN  is_get_active,   //是否会ACTIVE
								GUI_FONT_T font           //显示字号
								)
{
    GUILABEL_INIT_DATA_T text_label_init = {0};
    GUIFORM_DYNA_CHILD_T child_ctrl={0};
    MMI_STRING_T  text={0};

    text_label_init.align = GUILABEL_ALIGN_LEFT;
    child_ctrl.guid = SPRD_GUI_LABEL_ID;
    child_ctrl.is_bg = FALSE;
    child_ctrl.is_get_active = is_get_active;
    child_ctrl.init_data_ptr = &text_label_init;
    child_ctrl.child_handle = child_handle;

    GUIFORM_CreatDynaChildCtrl(win_id, form_handle, &child_ctrl);

    MMI_GetLabelTextByLang(text_id, &text);			
	
    if(text.wstr_len > 0 && (PNULL !=text.wstr_ptr))
    {
        GUILABEL_SetText(child_ctrl.child_handle, &text, FALSE);
        GUILABEL_SetFont(child_ctrl.child_handle, font, MMI_WHITE_COLOR);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateItem(MMI_WIN_ID_T win_id,ListItem *plistitem, uint8 index)
{
    MMI_HANDLE_T  child_handle=OPTION_FORM_CHIRD_CTRL_ID+index*CTRLNUM_PER_FORM;

    switch(plistitem->type)
    {
        case TYPE_OPEN_SET:
            CreateFormChildCtrl(win_id,child_handle,GUIFORM_LAYOUT_ORDER);

            //创建属于“子FOMR控件A”的“LABEL控件”		
            CreateLabelChildCtrl(win_id,child_handle,child_handle+1,
            plistitem->set_name_id,TRUE,MMI_DEFAULT_TEXT_FONT);
            ctrl_group[ctrl_count]=child_handle+1;
            ctrl_count++;
            break;

        case TYPE_SELECT_LIST:
        {
            uint16 i=0;
            SelectList *pselect=(SelectList *)plistitem->pitem;

            i=pselect->GetSetting();

            CreateFormChildCtrl(win_id,child_handle,GUIFORM_LAYOUT_ORDER);

            //显示设置名称
            CreateLabelChildCtrl(win_id,child_handle,child_handle+1,
            plistitem->set_name_id,FALSE,MMI_DEFAULT_TEXT_FONT);

            //显示设置值
            CreateLabelChildCtrl(win_id,child_handle,child_handle+2,
            pselect->item_text_id[i],TRUE,MMI_DEFAULT_TEXT_FONT);//SONG_FONT_14);
            ctrl_group[ctrl_count]=child_handle+2;
            ctrl_count++;
        }
        break;

        default:
            break;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UpdateSetParam(SelectList *plist)
{
    MMI_STRING_T  text={0};
    uint16 i=0;

    i=plist->GetSetting();

    MMI_GetLabelTextByLang(plist->item_text_id[plist->GetSetting()], &text);
    GUILABEL_SetText(cur_label_id, &text, FALSE);       
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectListWinMsg(
										   MMI_WIN_ID_T    win_id, 
										   MMI_MESSAGE_ID_E msg_id, 
										   DPARAM           param
										   )
{
    MMI_RESULT_E recode   =  MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = OPTION_SELECT_LIST_CTRL_ID;

    ListItem *plistitem=MMK_GetWinAddDataPtr(win_id);
    SelectList *plist=plistitem->pitem;
    static uint16 cur_idx=0;
	
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW: 
        {
            uint16 i=0;
            MMI_STRING_T title_sting = {0};

            MMI_GetLabelTextByLang(plistitem->set_name_id, &title_sting);        
            GUIWIN_SetTitleText(win_id, title_sting.wstr_ptr, title_sting.wstr_len, FALSE);
            GUILIST_SetMaxItem(ctrl_id, plist->num, FALSE );
            cur_idx=plist->GetSetting();

            for (i=0;i<plist->num;i++)
            {
                MMIAPISET_AppendListItemByTextIdExt(plist->item_text_id[i],
                TXT_COMMON_OK, TXT_NULL, STXT_RETURN, 
                ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
            }

            GUILIST_SetSelectedItem(ctrl_id, cur_idx, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_idx);
            GUILIST_SetOwnSofterKey(ctrl_id,TRUE);

            MMK_SetAtvCtrl(win_id,ctrl_id);
            //GUILIST_MakeListPopup(ctrl_id);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            uint16  index =0;

            index = GUILIST_GetCurItemIndex(ctrl_id);
            if(cur_idx!=index)
            {
                plist->SaveSetting(index);

                GUILIST_SetSelectedItem(ctrl_id, index, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, index);
                GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
                UpdateSetParam(plist);
            }
            MMK_CloseWin( win_id );
        }
        break;
        case MSG_APP_RED:
            MMIAPISET_ResumePreLivewallpaper();
            return FALSE;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );      
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;  
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOptionListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{	
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id;
    uint8 i=0;

    pmenu=(OPTIONLIST *)MMK_GetWinAddDataPtr(win_id);			

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            ctrl_count=0;

            //设置窗口名称
            GUIWIN_SetTitleTextId(win_id,pmenu->win_title_id,TRUE);

            for (i=0;i<pmenu->item_num;i++)
            {
                CreateItem(win_id,&(pmenu->plistitem[i]),i);
            }

            MMK_SetAtvCtrl(win_id, ctrl_group[0]); //第一个子控件
        }
        break;

        case MSG_CLOSE_WINDOW:
            break;	

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {			
            if (MSG_CTL_PENOK == msg_id)
            {
                ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            }
            else
            {
                ctrl_id= MMK_GetActiveCtrlId(win_id);
            }

            for (i=0;i<pmenu->item_num;i++)
            {
                if (ctrl_group[i]==ctrl_id)
                {
                    switch(pmenu->plistitem[i].type)
                    {
                    case TYPE_OPEN_SET:
                        {
                            OpenSet *pOpen=(OpenSet *)pmenu->plistitem[i].pitem;
                            pOpen->OpenSetFun();
                        }
                        break;
                    case TYPE_SELECT_LIST:
                        cur_label_id=ctrl_id;
                        MMK_CreatePubListWin((uint32*)MMI_LIVE_SELECT_LIST_WIN_TAB,(ADD_DATA)&(pmenu->plistitem[i]));
                        break;
                    default:
                        break;
                    }
                }
            }			
        }
        break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMIAPILIVEWALLPAPER_Stop();
            MMIAPILIVEWALLPAPER_Destroy(FALSE);
            MMIAPILIVEWALLPAPER_Init();            
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_RED:
            MMIAPISET_ResumePreLivewallpaper();
            return FALSE;		
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}



/*Edit by script, ignore 2 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
