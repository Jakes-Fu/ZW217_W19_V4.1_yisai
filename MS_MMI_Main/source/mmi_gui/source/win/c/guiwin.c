/*****************************************************************************
** File Name:      guiwin.c                                                  *
** Author:                                                                   *
** Date:           01/05/2009                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe gui win                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/05/2009     liqing.peng      Create
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guisoftkey.h"
#include "guilcd.h"
#include "guifont.h"
#include "guictrl_api.h"
#include "mmk_app.h"
#include "guiwin.h"
#include "mmi_theme.h"
#include "mmi_default.h"
#include "mmi_autotest.h"
#include "mmitheme_softkey.h"
#include "guititle_internal.h"
#include "mmitheme_title.h"
#include "guitab.h"
#include "../../mmi_ctrl/source/Title/h/ctrltitle.h"
#include "guistatusbar.h"
#include "guistatusbar_data.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set pubwin window title text id
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: �˴��ĵ����ƻ���ϵͳ�Ĳ�ṹ��pclint������ע�͹�(lianxiang)
/*****************************************************************************/
extern void MMIPUB_SetWinTitleTextId(
                                     MMI_WIN_ID_T     win_id,
                                     MMI_TEXT_ID_T    text_id,
                                     BOOLEAN          is_fresh
                                     );/*lint -esym(526,MMIPUB_SetWinTitleTextId) */

/*****************************************************************************/
//  Description : set pubwin window title text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: �˴��ĵ����ƻ���ϵͳ�Ĳ�ṹ��pclint������ע�͹�(lianxiang)
/*****************************************************************************/
extern void MMIPUB_SetWinTitleText(
                                   MMI_WIN_ID_T     win_id,
                                   MMI_STRING_T     *text_ptr,
                                   BOOLEAN          is_fresh
                                   );/*lint -esym(526,MMIPUB_SetWinTitleText) */

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note: ��������ת��
/*****************************************************************************/
LOCAL GUISTBDATA_ICON_STYLE_E ConvertWINTypeToGUI(
                                                  GUIWIN_STBDATA_ICON_STYLE_E win_type
                                                  );


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note: ��������ת��
/*****************************************************************************/
LOCAL GUIWIN_STBDATA_ICON_STYLE_E ConvertGUITypeToWIN(
                                                      GUISTBDATA_ICON_STYLE_E gui_type
                                                      );

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ��̬����״̬���ؼ�
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_CreateSoftkeyDyn(
                                       MMI_HANDLE_T win_handle,
                                       MMI_TEXT_ID_T left_text_id,
                                       MMI_TEXT_ID_T mid_text_id,
                                       MMI_TEXT_ID_T right_text_id
                                       )
{
    MMI_CTRL_ID_T ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    
    return GUISOFTKEY_CreateCtrlDyn(win_handle, ctrl_id, left_text_id, mid_text_id, right_text_id);
}

/*****************************************************************************/
// 	Description : ����softkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyButtonTextPtr(
                                              MMI_HANDLE_T    win_handle,
                                              MMI_STRING_T    str_info,   // the left softkey id
                                              GUISOFTKEY_INDEX button_num,
                                              BOOLEAN         is_need_update       // whether update
                                              )
{
    return GUISOFTKEY_SetButtonTextPtr(win_handle, MMITHEME_GetSoftkeyCtrlId(), str_info, button_num, is_need_update);
}

/*****************************************************************************/
// 	Description : get softkey id
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetSoftkeyTextId(
                                       MMI_HANDLE_T    win_handle,    // ���ھ��
                                       MMI_TEXT_ID_T   *left_id_ptr,   //in:may PNULL
                                       MMI_TEXT_ID_T   *middle_id_ptr, //in:may PNULL
                                       MMI_TEXT_ID_T   *right_id_ptr   //in:may PNULL
                                       )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		result = GUISOFTKEY_GetTextId(win_handle,ctrl_id,
                        left_id_ptr,middle_id_ptr,right_id_ptr);
	}

	return (result);
}

/*****************************************************************************/
// 	Description : ����softkey button�ϵ�IconͼƬID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Liqing.Peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SeSoftkeytButtonIconId(
                                             MMI_HANDLE_T    win_handle,    // ���ھ��
                                             MMI_IMAGE_ID_T  icon_id,
                                             GUISOFTKEY_INDEX button_num,
                                             BOOLEAN         is_need_update       // whether update
                                             )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
        result = GUISOFTKEY_SetButtonIconId(
            win_handle,
            ctrl_id,
            icon_id,
            button_num,
            is_need_update       // whether update
            );
	}

	return result;
}

#ifdef GUIF_SCROLLKEY
/*****************************************************************************/
// 	Description : ����scrollkey button�ϵ�IconͼƬID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Liqing.Peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetScrollkeytButtonIconId(
                                             MMI_HANDLE_T        win_handle,    // ���ھ��
                                             MMI_IMAGE_ID_T      icon_id,
                                             MMI_TEXT_ID_T       text_id,
                                             GUISCROLLSKIN_INDEX skin_idx,
                                             GUISCROLLKEY_INDEX  button_num,
                                             BOOLEAN             is_need_update       // whether update
                                             )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinScrollkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
        result = GUISCROLLKEY_SetButtonIconId(
            win_handle,
            ctrl_id,
            icon_id,
            text_id,
            skin_idx,
            button_num,
            is_need_update       // whether update
            );
	}

	return result;
}
#endif

/*****************************************************************************/
// 	Description : ����softkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SeSoftkeytButtonTextId(
                                             MMI_HANDLE_T    win_handle,    // ���ھ��
                                             MMI_IMAGE_ID_T  text_id,
                                             GUISOFTKEY_INDEX button_num,
                                             BOOLEAN         is_need_update       // whether update
                                             )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
        result = GUISOFTKEY_SetButtonTextId(
            win_handle,
            ctrl_id,
            text_id,
            button_num,
            is_need_update       // whether update
            );
	}

	return result;
}

/*****************************************************************************/
// 	Description : ����softkey��ͼƬID,����֪�ؼ��Ƿ�����ˢ��
//	Global resource dependence : 
//  Author:Liqing.Peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyTextId(
                                       MMI_HANDLE_T    win_handle,    // ���ھ��
                                       MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                       MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id
                                       MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                       BOOLEAN         is_need_update // whether update
                                       )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		result = GUISOFTKEY_SetTextId(win_handle, ctrl_id,
									  leftsoft_id, middlesoft_id, rightsoft_id, 
									  is_need_update);
        
        MMIWWW_TraceAutoTestNotifySoftkey(leftsoft_id, middlesoft_id, rightsoft_id);
	}

	return result;
}

/*****************************************************************************/
// 	Description : ����ˢ��Softkey�ؼ�
//	Global resource dependence : 
//  Author:Liqing.Peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_UpdateSoftkey(
                                    MMI_HANDLE_T    win_handle
                                    )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		GUISOFTKEY_Update(win_handle, ctrl_id);

		result = TRUE;
	}

	return result;
}

/*****************************************************************************/
// Description : ����Softkey��Style
// Global resource dependence : 
// Author: liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyStyle(
                                      MMI_HANDLE_T      win_handle,
                                      GUI_COLOR_T	    font_color,
                                      uint32		    bg_type,
                                      GUI_COLOR_T	    bg_color,
                                      MMI_IMAGE_ID_T    bg_img
                                      )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		GUISOFTKEY_SetStyle(win_handle, ctrl_id, font_color, bg_type, bg_color, bg_img);

		result = TRUE;
	}

	return result;
}

//++++CR242519
/*****************************************************************************/
// Description : ����Softkey��Pressed Style
// Global resource dependence : 
// Author: liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyPressedStyle(
                                      MMI_HANDLE_T      win_handle,
                                      GUI_COLOR_T	    font_color,
                                      uint32		    bg_type,
                                      GUI_COLOR_T	    bg_color,
                                      MMI_IMAGE_ID_T    bg_img
                                      )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		GUISOFTKEY_SetPressedStyle(win_handle, ctrl_id, font_color, bg_type, bg_color, bg_img);

		result = TRUE;
	}

	return result;
}
//++++CR242519

/*****************************************************************************/
// Description : ����Softkey��bg Style
// Global resource dependence : 
// Author: liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyBgStyle(
                                        MMI_HANDLE_T      win_handle,
                                        GUI_SOFTKEY_BG_TYPE_T bg_type
                                        )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		GUISOFTKEY_SetBgType(win_handle, ctrl_id, bg_type);

		result = TRUE;
	}

	return result;
}

/*****************************************************************************/
//  Description : ����softkey common�ı���
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_SetSoftkeyCommonBg(
                                      MMI_HANDLE_T  win_handle,
                                      GUI_BG_T      *bg_ptr
                                      )
{
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
        GUIAPICTRL_SetBg(ctrl_id,bg_ptr);
    }
}

/*****************************************************************************/
// Description : �����������ɫ
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftKeyfont(
                                     MMI_HANDLE_T    win_handle,
                                     GUI_COLOR_T     *color_ptr,
                                     GUI_FONT_T      *font_ptr
                                     )
{
    BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

    if (0 != ctrl_id)
    {
        result = TRUE;
        GUISOFTKEY_Setfont(win_handle, ctrl_id, color_ptr, font_ptr );
    }

    return result;
}

/*****************************************************************************/
// Description : ����softkey��״̬
// Global resource dependence : 
// Author:liqing.peng
// Note: ���������Ҫ����ĳ��Softkey��Tp long��Ϣ,��ôis_long_press����ΪTrue
/*****************************************************************************/
PUBLIC BOOLEAN  GUIWIN_SetSoftkeyBtnState(
                                          MMI_HANDLE_T	 win_handle,
                                          uint16         button_index,
                                          BOOLEAN        is_gray,
                                          BOOLEAN		 is_long_press
                                          )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		result = GUISOFTKEY_SetButtonState(win_handle, ctrl_id, button_index, is_gray, is_long_press);
	}

	return result;
}

/*****************************************************************************/
// Description : ����softkey�Ƿ����
// Global resource dependence : 
// Author:liqing.peng
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  GUIWIN_SetSoftkeyBtnGray(
                                          MMI_HANDLE_T	 win_handle,
                                          uint16         button_index,
                                          BOOLEAN        is_gray
                                          )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		result = GUISOFTKEY_SetButtonGray(win_handle, ctrl_id, button_index, is_gray);
	}

	return result;
}

/*****************************************************************************/
// 	Description : whether softkey response to tp msg
//	Global resource dependence : 
//  Author:Liqing.Peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsSoftkeyTPEnable(
                                        MMI_HANDLE_T   win_handle
                                        )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		result = GUISOFTKEY_IsTPEnable(win_handle, ctrl_id);
	}

    return result;
}


/*****************************************************************************/
// 	Description : ����Softkey�Ƿ���Ӧ������Ϣ
//	Global resource dependence : 
//  Author:Liqing.Peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyTPEnable(
                                         MMI_HANDLE_T   win_handle,
                                         BOOLEAN        sk_tp_enable
                                         )
{
	BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
	{
		result = GUISOFTKEY_SetTPEnable(win_handle, ctrl_id, sk_tp_enable);
	}

    return result;
}

/*****************************************************************************/
//  Description : �����Ƿ�ɼ�
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyVisible(
                                        MMI_HANDLE_T    win_handle,
                                        BOOLEAN         is_visible
                                        )
{
    BOOLEAN			result = FALSE;
	MMI_CTRL_ID_T   ctrl_id = 0;

	ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

	if (0 != ctrl_id)
    {
        result = GUISOFTKEY_SetVisible(win_handle, ctrl_id, is_visible);
    }
    return result;
}

/*****************************************************************************/
//  Description : ��ȡSoftkey�Ƿ�ɼ�
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetSoftkeyVisible(
                                        MMI_HANDLE_T    win_handle
                                        )
{
    BOOLEAN			result = FALSE;
    MMI_CTRL_ID_T   ctrl_id = 0;

    ctrl_id = MMK_GetWinSoftkeyCtrlId(win_handle);

    if (0 != ctrl_id)
    {
        result = GUISOFTKEY_GetVisible(win_handle, ctrl_id);
    }
    return result;
}

// status bar
/*****************************************************************************/
//  Description : initial the item data of status bar
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note: ��ʼ��״̬��������,����ϵͳ��״̬����������ͬ,�����ڿ���ʱ��ʼ��
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_InitStbData(
                                  uint32 item_max_num
                                  )
{
    return GUISTBDATA_InitData(item_max_num);
}

/*****************************************************************************/
//  Description : ����״̬������
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbVisible(
                                 MMI_HANDLE_T win_handle,
                                 BOOLEAN      is_visible
                                 )
{
    IGUICTRL_T* ctrl_ptr = (IGUICTRL_T*)MMK_GetCtrlPtrByWin( win_handle, MMITHEME_GetStatusBarCtrlId()); 
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_SetState((IGUICTRL_T *)ctrl_ptr,GUICTRL_STATE_INVISIBLE, is_visible);
    }
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note: ��������ת��
/*****************************************************************************/
LOCAL GUISTBDATA_ICON_STYLE_E ConvertWINTypeToGUI(
                                                  GUIWIN_STBDATA_ICON_STYLE_E win_type
                                                  )
{
    GUISTBDATA_ICON_STYLE_E gui_type = GUISTBDATA_ICON_NONE;

    switch(win_type)
    {
    case GUIWIN_STBDATA_ICON_NONE:
        gui_type = GUISTBDATA_ICON_NONE;
        break;
    case GUIWIN_STBDATA_ICON_ANIM:
        gui_type = GUISTBDATA_ICON_ANIM;
        break;
    case GUIWIN_STBDATA_ICON_NORMAL:
        gui_type = GUISTBDATA_ICON_NORMAL;
        break;
    case GUIWIN_STBDATA_ICON_MAX:
        gui_type = GUISTBDATA_ICON_MAX;
        break;
#if 1//def WRE_SUPPORT//add for wre_background by heng.xiao
    case GUIWIN_STBDATA_ICON_WRE:
        gui_type = GUISTBDATA_ICON_WRE;
        break;
#endif
    default:
        gui_type = GUISTBDATA_ICON_MAX;
        break;
    }

    return gui_type;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note: ��������ת��
/*****************************************************************************/
LOCAL GUIWIN_STBDATA_ICON_STYLE_E ConvertGUITypeToWIN(
                                                      GUISTBDATA_ICON_STYLE_E gui_type
                                                      )
{
    GUIWIN_STBDATA_ICON_STYLE_E win_type = GUIWIN_STBDATA_ICON_NONE;
    
    switch(gui_type)
    {
    case GUISTBDATA_ICON_NONE:
        win_type = GUIWIN_STBDATA_ICON_NONE;
        break;
    case GUISTBDATA_ICON_ANIM:
        win_type = GUIWIN_STBDATA_ICON_ANIM;
        break;
    case GUISTBDATA_ICON_NORMAL:
        win_type = GUIWIN_STBDATA_ICON_NORMAL;
        break;
#if 1//def WRE_SUPPORT//add for wre_background by heng.xiao
    case GUISTBDATA_ICON_WRE:
        win_type = GUIWIN_STBDATA_ICON_WRE;
        break;
#endif
    case GUISTBDATA_ICON_MAX:
        win_type = GUIWIN_STBDATA_ICON_MAX;
        break;
    default:
        win_type = GUIWIN_STBDATA_ICON_MAX;
        break;
    }
    
    return win_type;
}

/*****************************************************************************/
//  Description : Append item
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note: ���Item������Ϣ
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_AppendStbItemData(
                                        GUIWIN_STBDATA_ITEM_DATA_T *item_data_ptr
                                        )
{
    uint32 i = 0;
    GUISTBDATA_ITEM_DATA_T item_data = {0};

    if (PNULL == item_data_ptr)
    {
        return FALSE;
    }

    // item other info
    item_data.app_handle_func = item_data_ptr->app_handle_func;
    item_data.cur_frame = item_data_ptr->cur_frame;
    item_data.is_handle_callback = item_data_ptr->is_handle_callback;
    // icon info
    for (i = 0; i < MMISTATUSBAR_ITEM_ICON_FRAME; i++)
    {
        item_data.icon_info.icon_array[i] = item_data_ptr->icon_info.icon_array[i];
    }
    item_data.icon_info.icon_num = item_data_ptr->icon_info.icon_num;
    item_data.icon_info.icon_type = ConvertWINTypeToGUI(item_data_ptr->icon_info.icon_type);
    item_data.icon_info.is_display = item_data_ptr->icon_info.is_display;

    // text info
    item_data.text_info.align = item_data_ptr->text_info.align;
    item_data.text_info.font_color = item_data_ptr->text_info.font_color;
    item_data.text_info.font_type = item_data_ptr->text_info.font_type;
    item_data.text_info.is_display = item_data_ptr->text_info.is_display;
    item_data.text_info.wstr_len = item_data_ptr->text_info.wstr_len;    
    item_data.owner_draw_func = item_data_ptr->owner_draw_func;  //add owner_draw_func init  //wre_background
    SCI_MEMCPY(item_data.text_info.wstr_ptr, item_data_ptr->text_info.wstr_ptr, 
        sizeof(wchar)*MIN(item_data_ptr->text_info.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN));

    // call func of gui data module
    return GUISTBDATA_AppendItemData(&item_data);
}

/*****************************************************************************/
//  Description : set item is visible or not
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ����item�Ƿ�ɼ�
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemVisible(
                                        uint32   item_index,
                                        BOOLEAN  is_visible
                                        )
{
    return GUISTBDATA_SetItemVisible(item_index, is_visible);
}


/*****************************************************************************/
//  Description : charge the item is visible or not
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: �ж�item�Ƿ�ɼ�
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemVisible(
                                       uint32   item_index
                                       )
{
    return GUISTBDATA_IsItemVisible(item_index);
}

/***************************************************************************//*!
@brief ����item�Ƿ���Ч
@author xiaoqing.lu
@param item_index [in] ������
@param is_visible [in] �Ƿ���Ч
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemValid(
                                        uint32   item_index,
                                        BOOLEAN  is_valid
                                        )
{
    return GUISTBDATA_SetItemValid(item_index, is_valid);
}


/***************************************************************************//*!
@brief ��ȡitem�Ƿ���Ч
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemValid(
                                       uint32   item_index
                                       )
{
    return GUISTBDATA_IsItemValid(item_index);
}

/*****************************************************************************/
//  Description : set item is displayed by right side
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ������ǿ�����ʾ����϶��ǿ�����ʾ��û�е��������
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemRightAlign(
                                           uint32   item_index,
                                           BOOLEAN  is_right_align
                                           )
{
    return GUISTBDATA_SetItemRightAlign(item_index, is_right_align);
}

/*****************************************************************************/
//  Description : set item is displayed by right side
//  Global resource dependence : 
//  Author:xiyuan.ma
//  Note: status item ������ʾ
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemCenterAlign(
                                           uint32   item_index,
                                           BOOLEAN  is_center_align
                                           )
{
    return GUISTBDATA_SetItemCenterAlign(item_index, is_center_align);
}

/*****************************************************************************/
//  Description : judge item is displayed by right side or not
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ������ǿ�����ʾ����϶��ǿ�����ʾ��û�е��������
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemRightAlign(
                                          uint32 item_index
                                          )
{
    return GUISTBDATA_IsItemRightAlign(item_index);
}

/*****************************************************************************/
//  Description : judge item is displayed by right side or not
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ������ǿ�����ʾ����϶��ǿ�����ʾ��û�е��������
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemCenterAlign(
                                          uint32 item_index
                                          )
{
    return GUISTBDATA_IsItemCenterAlign(item_index);
}

/***************************************************************************//*!
@brief ����disable״̬
@author xiaoqing.lu
@param item_index [in] ������
@param is_disable [in] �Ƿ�disable
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemDisable(
                                        uint32   item_index,
                                        BOOLEAN  is_disable
                                        )
{
    return GUISTBDATA_SetItemDisable(item_index, is_disable);
}


/***************************************************************************//*!
@brief �ж�disable
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemDisable(
                                       uint32   item_index
                                       )
{
    return GUISTBDATA_IsItemDisable(item_index);
}

/*****************************************************************************/
//  Description : set item handle function is valid or not
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ����״̬��Item�����Ӧ�ýӿ�ָ��,���紥�ʵ����ʱ���ܹ�ִ�и�Ӧ��
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemHandleFunc(
                                           uint32   item_index,
                                           GUIWIN_STBDATA_APP_HANDLE_FUNC app_handle_func
                                           )
{
    return GUISTBDATA_SetItemHandleFunc(item_index, (GUISTBDATA_APP_HANDLE_FUNC)app_handle_func);
}

/*****************************************************************************/
//  Description : set item handle function is valid or not
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: �����Ƿ���Ӧ��,���ΪFalse�򲻻ᴦ��
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetItemHandleFuncValid(
                                             uint32  item_index,
                                             BOOLEAN is_valid
                                             )
{
    return GUISTBDATA_SetItemHandleFuncValid(item_index, is_valid);
}

/*****************************************************************************/
//  Description : get item handle function is valid or not
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:  ��ȡӦ�ô���ӿڵ���Ч��
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_StbItemIsHandleFuncValid(
                                               uint32 item_index
                                               )
{
    return GUISTBDATA_ItemIsHandleFuncValid(item_index);
}

/*****************************************************************************/
//  Description : ����Item�Ķ�Ӧ��ʾ��ͼƬ
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ÿ��item�����ʾMMISTATUSBAR_ITEM_ICON_FRAME��Icon,�ڿؼ�����������ʽ����
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIcon(
                                     uint32            item_index,
                                     const GUIWIN_STBDATA_ICON_T *icon_info_ptr
                                     )
{
    uint32 i = 0;
    GUISTBDATA_ICON_T icon_info = {0};

    if (PNULL == icon_info_ptr)
    {
        return FALSE;
    }

    icon_info.icon_num = icon_info_ptr->icon_num;

    for (i = 0; i < MMISTATUSBAR_ITEM_ICON_FRAME; i++)
    {
        icon_info.icon_array[i] = icon_info_ptr->icon_array[i];
    }

    icon_info.icon_type = ConvertWINTypeToGUI(icon_info_ptr->icon_type);
    icon_info.is_display = icon_info_ptr->is_display;
    
    return GUISTBDATA_SetItemIcon(item_index, (GUISTBDATA_ICON_T*)&icon_info);
}

/*****************************************************************************/
//  Description : ��ȡ�ؼ���ĳһ���ͼƬ��Ϣ
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemIcon(
                                     uint32            item_index,
                                     GUIWIN_STBDATA_ICON_T *icon_info_ptr
                                     )
{
    return GUISTBDATA_GetItemIcon(item_index, (GUISTBDATA_ICON_T*)icon_info_ptr);
}

/*****************************************************************************/
//  Description : ����ͼƬIcon��ʾ����,����ʾΪ��̬ͼƬ,Ҳ����ʾΪ��̬ͼƬ
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ��̬ʱֻ��ʾ�����еĵ�һ��ͼƬ,��̬ʱ��ʾ����������ͼƬ�Զ�����ʾ
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconStyle(
                                          uint32            item_index,
                                          GUIWIN_STBDATA_ICON_STYLE_E icon_style
                                          )
{
    return GUISTBDATA_SetItemIconStyle(item_index, ConvertWINTypeToGUI(icon_style));
}

/*****************************************************************************/
//  Description : ��ȡItem��ͼƬ��ʾ״̬,��̬��̬
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemIconStyle(
                                          uint32            item_index,
                                          GUIWIN_STBDATA_ICON_STYLE_E *icon_style_ptr
                                          )
{
    BOOLEAN result = FALSE;
    GUISTBDATA_ICON_STYLE_E icon_type = GUISTBDATA_ICON_NONE;

    result = GUISTBDATA_GetItemIconStyle(item_index, &icon_type);

    *icon_style_ptr = ConvertGUITypeToWIN(icon_type);

    return result;
}
/*****************************************************************************/
//  Description : ����Item��Icon
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:�ýӿں�GUIWIN_SetStbItemIcon������������ֻ����item��icon����ĵ�һ��icon,
//       ���ؼ���ʾ��̬icon��ʱ����ʾ��һ��ͼƬ
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconId(
                                       uint32         item_index,
                                       MMI_IMAGE_ID_T icon_id
                                       )
{
    return GUISTBDATA_SetItemIconId(item_index, icon_id);
}
/*****************************************************************************/
//  Description : ���ÿؼ�Item�����ĵ�ǰ��ʾ��ͼƬ
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ��̬��ʾicon��ʱ��,�ؼ���Ҫ֪����ǰ��ʾ��icon�����±�
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconCurFrame(
                                             uint32 item_index,
                                             uint32 cur_frame
                                             )
{
    return GUISTBDATA_SetItemIconCurFrame(item_index, cur_frame);
}

/*****************************************************************************/
//  Description : ��ȡicon���鵱ǰ��ʾ�������±�
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemIconCurFrame(
                                             uint32 item_index,
                                             uint32 *cur_frame_ptr
                                             )
{
    return GUISTBDATA_GetItemIconCurFrame(item_index, cur_frame_ptr);
}

/*****************************************************************************/
//  Description : ����Icon�Ƿ���Ч,������Ϊ��Ч��ʱ��,item��Ӧ��״̬����ʾͼƬ
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ״̬���ȿ�����ʾͼƬҲ������ʾ�ı�,���߶���ʾ,������Ϊ��Ч��ʱ��,
//          ����ʾͼƬ
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconValid(
                                          uint32 item_index,
                                          BOOLEAN is_valid
                                          )
{
    return GUISTBDATA_SetItemIconValid(item_index, is_valid);
}

/*****************************************************************************/
//  Description : �ж�item��icon�Ƿ���Ч
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_StbIsItemIconValid(
                                         uint32 item_index
                                         )
{
    return GUISTBDATA_IsItemIconValid(item_index);
}

/*****************************************************************************/
//  Description : ����item��ʾ���ַ���
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:״̬���ȿ�����ʾͼƬҲ������ʾ�ı�,���߶���ʾ,������Ϊ��Ч��ʱ��,
//          ����ʾ�ı�
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemText(
                                     uint32            item_index,
                                     const GUIWIN_STBDATA_TEXT_T *text_info_ptr
                                     )
{
    GUISTBDATA_TEXT_T text_info = {0};

    if (PNULL == text_info_ptr)
    {
        return FALSE;
    }

    text_info.align = text_info_ptr->align;
    text_info.font_color = text_info_ptr->font_color;
    text_info.font_type = text_info_ptr->font_type;
    text_info.is_display = text_info_ptr->is_display;
    text_info.wstr_len = text_info_ptr->wstr_len;

    SCI_MEMCPY(text_info.wstr_ptr, text_info_ptr->wstr_ptr, sizeof(wchar)*MIN(text_info_ptr->wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN));

    return GUISTBDATA_SetItemText(item_index, (GUISTBDATA_TEXT_T*)&text_info);
}

/*****************************************************************************/
//  Description : ��ȡitem���ַ���
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemText(
                                     uint32            item_index,
                                     GUIWIN_STBDATA_TEXT_T *text_info_ptr
                                     )
{
    return GUISTBDATA_GetItemText(item_index, (GUISTBDATA_TEXT_T*)text_info_ptr);
}

/*****************************************************************************/
//  Description : ����item���ı��Ƿ���Ч
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemTextValid(
                                          uint32 item_index,
                                          BOOLEAN is_valid
                                          )
{
    return GUISTBDATA_SetItemTextValid(item_index, is_valid);
}

/*****************************************************************************/
//  Description : �ж��ı��Ƿ���Ч
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemTextValid(
                                         uint32 item_index
                                         )
{
    return GUISTBDATA_IsItemTextValid(item_index);
}

/*****************************************************************************/
//  Description : ����Item��rect
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemRect(
                                     uint32     item_index,
                                     GUI_RECT_T rect
                                     )
{
    return GUISTBDATA_SetItemRect(item_index, rect);
}

//PUBLIC BOOLEAN GUIWIN_SetStbIstyleItemRect(
//                                     uint32     item_index,
//                                     GUI_RECT_T rect
//                                     )
//{
//    return GUISTBDATA_SetIstyleItemRect(item_index, rect);
//}
/*****************************************************************************/
//  Description : ���item��rect
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T GUIWIN_GetStbItemRect(
                                        uint32 item_index
                                        )
{
    return GUISTBDATA_GetItemRect(item_index);
}

/*****************************************************************************/
//  Description : ��ȡitem������
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint32 GUIWIN_GetStbItemTotalNum(void)
{
    return GUISTBDATA_GetItemTotalNum();
}

/*****************************************************************************/
//  Description : ����״̬���Ƿ���ʾ����
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ��ʱ��״̬����ֱ����ʾ��lcd��,������Ҫ��ʾ�Լ��ı���,����idle�������˵�����
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbBgIsDesplay(
                                     MMI_HANDLE_T win_handle,
                                     BOOLEAN is_desplay_bg
                                     )
{
    GUISTATUSBAR_SetBgIsDesplay(win_handle, is_desplay_bg);
}

/*****************************************************************************/
//  Description : ����״̬���Ƿ�����û������ı�����Ϣ
//  Global resource dependence : 
//  Author:xiyuan.ma
//  Note:            TURE, ���
/*****************************************************************************/
PUBLIC void GUIWIN_IsClearCustomerBg(
                                     MMI_HANDLE_T win_handle,
                                     BOOLEAN clear_customer_bg
                                     )
{
    GUISTATUSBAR_ClearCustomerBg(win_handle, clear_customer_bg);
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbState(
                               MMI_HANDLE_T win_handle,
                               uint32 ctrl_state,
                               BOOLEAN is_set
                               )
{
    GUISTATUSBAR_SetState(win_handle, ctrl_state, is_set);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbState(
                                  MMI_HANDLE_T win_handle,
                                  uint32 ctrl_state
                                  )
{
    return GUISTATUSBAR_GetState(win_handle, ctrl_state);
}

/*****************************************************************************/
//  Description : ���ÿؼ��ı���ͼƬID
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbBgImageId(
                                   MMI_HANDLE_T win_handle,
                                   MMI_IMAGE_ID_T image_id
                                   )
{
    GUISTATUSBAR_SetBgImageId(win_handle, image_id);
}

/*****************************************************************************/
//  Description : ���ÿؼ��ı���
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbBg(
                            MMI_HANDLE_T win_handle,
                            GUI_BG_T  bg_info
                            )
{
    GUISTATUSBAR_SetBg(win_handle, bg_info);
}

/*****************************************************************************/
//  Description : ���ÿؼ��ı���
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbCommonBg(
                                  MMI_HANDLE_T  win_handle,
                                  GUI_BG_T      *bg_ptr
                                  )
{
    if (PNULL != bg_ptr)
    {
        GUISTATUSBAR_SetCommonBg(win_handle,*bg_ptr);
    }
}

/*****************************************************************************/
//  Description : set statusbar both rect
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbBothRect(
                                     MMI_HANDLE_T      win_handle,
                                     GUI_BOTH_RECT_T   *both_rect_ptr
                                     )
{
    BOOLEAN result = FALSE;
    MMI_HANDLE_T stb_handle = MMK_GetCtrlHandleByWin(win_handle, MMITHEME_GetStatusBarCtrlId());

    if (0 != stb_handle)
    {
        result = GUIAPICTRL_SetBothRect(stb_handle, both_rect_ptr);
    }

    return result;
}

/***************************************************************************//*!
@brief post messages of status bar
@author xiaoqing.lu
@param win_handle [in] �ؼ��ĸ�����handle��id
@param is_full_paint [in] �Ƿ�ˢ����������
@retval true/false
@return �ɹ���ʧ��
@note ���ؼ�����ʾ�Լ��ı���ʱ����Ҫˢ�´��ڵı�����is_full_paint=true��
*******************************************************************************/
PUBLIC MMI_RESULT_E GUIWIN_ReDrawStbCtrl(
                                         MMI_HANDLE_T win_handle,
                                         BOOLEAN is_full_paint
                                         )
{
    return GUISTATUSBAR_UpdateCtrl(win_handle, is_full_paint);
}

/*****************************************************************************/
//  Description : update icon only, not display background
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ֻˢ�¿ؼ���icon,��ˢ�¿ؼ�����,���ؼ�������Ч��ʱ�����Ӧ���Լ�ˢ��
//          ������ʱ����øýӿ�ˢ�¿ؼ�,�ýӿڲ�������ڷ���Fullpaint��Ϣ
/*****************************************************************************/
PUBLIC MMI_RESULT_E GUIWIN_UpdateStbIcon(void)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    
    if(MMK_IsIncludeStatusBar(MMK_GetFocusWinId())
        && MMIDEFAULT_IsBacklightOn())
    {
        result = GUISTATUSBAR_UpdateCtrl(MMK_GetFocusWinId(), FALSE);
    }

    return result;
}

/*****************************************************************************/
//  Description : ˢ��״̬��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: ˢ��״̬��������item,��������Ч��ʱ��ᷢ��fullpaint������,�����ô���
//          ˢ�±���
/*****************************************************************************/
PUBLIC MMI_RESULT_E GUIWIN_UpdateStb(void)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
    if( FALSE == GUISTATUSBAR_IsDropDown(MMK_GetFocusWinId()))
#endif
    {
        if(MMK_IsIncludeStatusBar(MMK_GetFocusWinId())
            && MMIDEFAULT_IsBacklightOn())
        {
            result = GUISTATUSBAR_UpdateCtrl(MMK_GetFocusWinId(), TRUE);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : ��̬����״̬���ؼ�
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_CreateStbDyn(
                                   MMI_HANDLE_T win_handle
                                   )
{
    return GUISTATUSBAR_CreateCtrlDyn(win_handle);
}

/*****************************************************************************/
//  Description : ��̬����״̬���ؼ����û������Զ���ؼ�������
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_CustomStbCreate(
                                      MMI_HANDLE_T win_handle,
                                      GUI_BOTH_RECT_T *rect_ptr
                                      )
{
    return GUISTATUSBAR_CustomCreate(win_handle, rect_ptr);
}

/*****************************************************************************/
//  Description : �û���ȡstatusbar�Ĳ���Ϣ
//  Global resource dependence : 
//  Author:xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_GetStatusbarLayer(
									 MMI_HANDLE_T win_handle,
									 GUI_LCD_DEV_INFO *dev_info_ptr
									 )
{
    GUISTATUSBAR_GetCtrlLayer(win_handle, dev_info_ptr);
}

#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbDropDownState(
                                       MMI_HANDLE_T win_handle,
                                       BOOLEAN enable
                                       )
{
    GUISTATUSBAR_SetDropDownState(win_handle, enable);
}

/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbDropDown(
                                    MMI_HANDLE_T win_handle
                                    )
{
    return GUISTATUSBAR_IsDropDown(win_handle);
}

/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbPressed(
                                 MMI_HANDLE_T win_handle,
                                 BOOLEAN is_true
                                 )
{
    GUISTATUSBAR_SetPressed(win_handle, is_true);
}

/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbPressed(
                                   MMI_HANDLE_T win_handle
                                   )
{
    return GUISTATUSBAR_IsPressed(win_handle);
}

#endif

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleText(
                                   MMI_HANDLE_T		win_handle,
                                   const wchar		*wstr_ptr, //[IN] string pointer
                                   uint16			wstr_len, //[IN] string length, <=16
                                   BOOLEAN			is_need_update //[IN] is flash
                                   )
{
    BOOLEAN		 result  = FALSE;
    MMI_HANDLE_T title_handle = 0;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    if ( MMK_GetWinDisplayStyleState( win_handle, WS_HAS_PROMPT_WIN ) )
    {
        MMI_STRING_T string = {0};
        string.wstr_ptr = (wchar *)wstr_ptr;
        string.wstr_len = wstr_len;

        MMIPUB_SetWinTitleText( win_handle, &string, is_need_update );
        result = TRUE;
    }
    else
    {
        title_handle = MMK_GetWinTitleHandle(win_handle);
        if (0 != title_handle)
        {
            result = GUITITLE_SetText(win_handle, title_handle, wstr_ptr, wstr_len, is_need_update);
        }
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : set the window title string, base string id
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleTextId(
                                     MMI_HANDLE_T	  win_handle,
                                     MMI_TEXT_ID_T   text_id, //[IN] the string id
                                     BOOLEAN         is_need_update //[IN] is flash
                                     )
{
    BOOLEAN		 result  = FALSE;
    MMI_HANDLE_T title_handle = 0;

    win_handle = MMK_ConvertIdToHandle( win_handle );
    
    if ( MMK_GetWinDisplayStyleState( win_handle, WS_HAS_PROMPT_WIN ) )
    {
        MMIPUB_SetWinTitleTextId( win_handle, text_id, is_need_update );
        result = TRUE;
    }
    else
    {
        title_handle = MMK_GetWinTitleHandle(win_handle);

        if (0 != title_handle)
        {
            result = GUITITLE_SetTextId(win_handle, title_handle, text_id, is_need_update);
        }
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : set the window title display style
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note: ���ñ�����ı��Ű淽ʽ
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleTextAlign(
                                        MMI_HANDLE_T			win_handle,
                                        GUI_ALIGN_E	        align
                                        )
{
    BOOLEAN         result  = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetTextAlignType(win_handle, title_handle, align);
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : set the window title display style
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note: ���ñ���ؼ��Ĺ�����ʽ,Ĭ�ϴ������ҹ���,��ο�GUITITLE_SCROLL_TYPE_E����
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleTextScroll(
                                         MMI_HANDLE_T			    win_handle,
                                         GUITITLE_SCROLL_TYPE_E	scroll_type
                                         )
{
    BOOLEAN         result  = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetTextScrollType(win_handle, title_handle, scroll_type);
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : ����title���Ƿ���arrow
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note: ���ñ���ؼ��ϵ�����ͼ���Ƿ���Ҫ��ʾ
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleRollArrow(    
                                        MMI_HANDLE_T   win_handle,
                                        BOOLEAN        is_need_title_arrow  //�Ƿ���Ҫtitle�ϵ�arrowͼ��
                                        )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetRollArrow(win_handle, title_handle, is_need_title_arrow);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : ����title���Ƿ���arrow
//	Global resource dependence : 
//  Author: Liqing.peng
//	Note: ���ñ���ؼ�������ͼ���״̬
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleRollArrowStatus(    
                                              MMI_HANDLE_T               win_handle,
                                              GUITITLE_BUTTON_INDEX_T   index,
                                              GUITITLE_BUTTON_STATE_E  status
                                              )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetRollArrowStatus(win_handle, title_handle, index, status);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : ���title��arrow״̬
//	Global resource dependence : 
//  Author: Liqing.peng
//	Note: ��ñ���ͼ���״̬
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E GUIWIN_GetTitleRollArrowStatus(    
                                                             MMI_HANDLE_T               win_handle,
                                                             GUITITLE_BUTTON_INDEX_T   index
                                                             )
{
    MMI_HANDLE_T title_handle = 0;
    GUITITLE_BUTTON_STATE_E         state = TITLE_BUTTON_NORMAL;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        state = GUITITLE_GetRollArrowStatus(win_handle, title_handle, index);
    }
    
    return state;
}

/*****************************************************************************/
// 	Description : ����title���Ƿ���button
//	Global resource dependence : 
//  Author: xiaoqinglu
//	Note: ���ñ���ؼ��ϵ�����ͼ���Ƿ���Ҫ��ʾ
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleNeedButton(    
                                         MMI_HANDLE_T   win_handle,
                                         BOOLEAN        is_need_button//�Ƿ���Ҫtitle�ϵ�buttonͼ��
                                         )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetNeedButton(win_handle, title_handle, is_need_button);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : ����title���Ƿ���button
//	Global resource dependence : 
//  Author: xiaoqinglu
//	Note: ���ñ���ؼ�������ͼ���״̬
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonStatus(    
                                           MMI_HANDLE_T               win_handle,
                                           GUITITLE_BUTTON_INDEX_T   index,
                                           GUITITLE_BUTTON_STATE_E  status
                                           )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetButtonStatus(win_handle, title_handle, index, status);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : ���title��button״̬
//	Global resource dependence : 
//  Author: xiaoqinglu
//	Note: ��ñ���ͼ���״̬
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E GUIWIN_GetTitleButtonStatus(    
                                                           MMI_HANDLE_T               win_handle,
                                                           GUITITLE_BUTTON_INDEX_T   index
                                                           )
{
    MMI_HANDLE_T title_handle = 0;
    GUITITLE_BUTTON_STATE_E         state = TITLE_BUTTON_NORMAL;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        state = GUITITLE_GetButtonStatus(win_handle, title_handle, index);
    }
    
    return state;
}

/*****************************************************************************/
// 	Description : ����title���½��б�������Ϣ
//	Global resource dependence : 
//  Author: Liqing.peng
//	Note: ���ñ���ؼ���ʾ�ĸ����ַ���
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleSubText(    
                                      MMI_HANDLE_T   win_handle,
                                      BOOLEAN        is_need_sub_text,   //�Ƿ���Ҫ�б�������Ϣ
                                      const wchar*	 wstr_ptr,		//[IN] string pointer
                                      uint16		 wstr_len,		//[IN] string length
                                      BOOLEAN        is_need_update        //�Ƿ�������ʾ
                                      )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetSubText(win_handle, title_handle, is_need_sub_text, wstr_ptr, wstr_len, is_need_update);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : ����title���½��б�������Ϣ����
//	Global resource dependence : 
//  Author: Liqing.peng
//	Note: ���ñ���ؼ���ʾ�ĸ����ַ���
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleSubTextParam(
                                           MMI_HANDLE_T     win_handle,
                                           GUI_RECT_T       *text_rect_ptr, // ���ַ���������
                                           GUI_FONT_T       *font_ptr,      // ���ַ�������
                                           GUI_ALIGN_E      *align_ptr      // align type
                                           )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T title_handle = 0;
   
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetSubTextParam(win_handle,
                    title_handle,
                    text_rect_ptr,
                    font_ptr,
                    align_ptr);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : ���sub text����ʾ��Ϣ
//	Global resource dependence : 
//  Author: Liqing.peng
//	Note: Clear����ؼ��ĸ����ַ���
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_ClearTitleSubTextInfo(    
                                            MMI_HANDLE_T   win_handle
                                            )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_ClearSubTextInfo(win_handle, title_handle);
    }
    return result;
}
/*****************************************************************************/
// 	Description : update the window title
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void GUIWIN_UpdateTitle(
                               MMI_HANDLE_T win_handle
                               )
{
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        GUITITLE_Update(win_handle, title_handle);
    }
}


/*****************************************************************************/
// 	Description : set window title rect
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleRect(
                                   MMI_HANDLE_T win_handle,
                                   GUI_RECT_T	rect
                                   )
{
    MMI_HANDLE_T title_handle = 0;
    BOOLEAN         result = FALSE;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetRect(win_handle, title_handle, rect);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : set window title both rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleBothRect(
                                       MMI_HANDLE_T     win_handle,
                                       GUI_BOTH_RECT_T  *rect_ptr
                                       )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    //get control id
    title_handle = MMK_GetWinTitleHandle(win_handle);
    if (0 != title_handle)
    {
        result = GUIAPICTRL_SetBothRect(title_handle,rect_ptr);
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : set the window title background
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleBackground(
                                         MMI_HANDLE_T			win_handle,
                                         MMI_IMAGE_ID_T		bg_image,
                                         GUI_COLOR_T			bg_color
                                         )
{
    BOOLEAN         result  = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetBackground(win_handle, title_handle, bg_image, bg_color);
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : create title dynamic
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_CreateTitleDyna(
                                      MMI_HANDLE_T	win_handle,
                                      MMI_TEXT_ID_T	text_id
                                      )
{
    BOOLEAN					result = FALSE;
    MMI_HANDLE_T title_handle = 0;

    title_handle = MMITHEME_GetTitleCtrlId();

    result = GUITITLE_CreateCtrlDyn(win_handle, title_handle, text_id);

    return (result);
}

/*****************************************************************************/
// 	Description : set window title icon
//	Global resource dependence : 
//  Author:Liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleIcon(
                                   MMI_HANDLE_T     win_handle,
                                   MMI_IMAGE_ID_T   icon_id
                                   )
{
    MMI_HANDLE_T title_handle = 0;
    BOOLEAN         result = FALSE;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 != title_handle)
    {
        result = GUITITLE_SetIcon(win_handle, title_handle, icon_id);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : set title font color
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleFontColor(
                                        MMI_HANDLE_T     win_handle,
                                        GUI_COLOR_T      font_color
                                        )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    
    //get control id
    title_handle = MMK_GetWinTitleHandle(win_handle);
    if (0 != title_handle)
    {
        GUITITLE_SetFontColor(win_handle,title_handle,font_color);
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : Set Title Visible
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleIsVisible(    
                                        MMI_HANDLE_T win_handle,
                                        BOOLEAN is_visible,
                                        BOOLEAN is_update
                                        )
{
    BOOLEAN result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    CTRLTITLE_OBJ_T *title_ptr = PNULL;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 == title_handle)
    {
        win_handle = MMK_GetParentWinHandle(win_handle);
        title_handle = MMK_GetWinTitleHandle(win_handle);
    }

    if (0 != title_handle)
    {
        title_ptr = GUITITLE_GetTitlePtr(win_handle, title_handle);

        result = GUITITLE_SetIsVisible(title_ptr, is_visible, is_update);
    }
    
    return result;
}

/***************************************************************************//*!
@brief ��ȡtitle������ɫ
@author qi.liu1
@param      win_handle [in]   ���ھ��
@param      font_color [in]   ������ɫ
@return ��ǰ����title����ɫ
@note   �˺�����װ��GUIWIN_GetTitleColor�����ڻ�ȡ��ǰtitle������ɫ
*******************************************************************************/
PUBLIC GUI_COLOR_T GUIWIN_GetTitleColor(
                                        MMI_HANDLE_T win_handle
                                        )
{
    GUI_COLOR_T title_color = 0xffff;
    MMI_HANDLE_T title_handle = 0;
    CTRLTITLE_OBJ_T *title_ptr = PNULL;

    title_handle = MMK_GetWinTitleHandle(win_handle);

    if (0 == title_handle)
    {
        win_handle = MMK_GetParentWinHandle(win_handle);
        title_handle = MMK_GetWinTitleHandle(win_handle);
    }

    if (0 != title_handle)
    {
        title_ptr = GUITITLE_GetTitlePtr(win_handle, title_handle);
        if(PNULL != title_ptr)
        {
            title_color = title_ptr->str_style.font_color;
        }
    }

    return title_color;
}

/*****************************************************************************/
// 	Description : Set Istyle Icon state
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonState(    
                                          MMI_HANDLE_T win_handle,
                                          GUITITLE_BUTTON_INDEX_T icon_index,
                                          GUITITLE_BUTTON_STATE_E icon_state,
                                          BOOLEAN is_need_update
                                          )
{
    BOOLEAN result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    CTRLTITLE_OBJ_T *title_ptr = PNULL;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 == title_handle)
    {
        win_handle = MMK_GetParentWinHandle(win_handle);
        title_handle = MMK_GetWinTitleHandle(win_handle);
    }

    if (0 != title_handle)
    {
        title_ptr = GUITITLE_GetTitlePtr(win_handle, title_handle);

        result = GUITITLE_SetButtonState(title_ptr, icon_index, icon_state, is_need_update);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : Set Istyle Icon Text
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonTextId(    
                                           MMI_HANDLE_T win_handle,
                                           GUITITLE_BUTTON_INDEX_T icon_index,
                                           MMI_TEXT_ID_T text_id,
                                           BOOLEAN is_need_update
                                           )
{
    BOOLEAN result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    CTRLTITLE_OBJ_T *title_ptr = PNULL;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 == title_handle)
    {
        win_handle = MMK_GetParentWinHandle(win_handle);
        title_handle = MMK_GetWinTitleHandle(win_handle);
    }

    if (0 != title_handle)
    {
        title_ptr = GUITITLE_GetTitlePtr(win_handle, title_handle);

        result = GUITITLE_SetButtonTextId(title_ptr, icon_index, text_id, is_need_update);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : Set Istyle Icon Text By Ptr
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonText(    
                                         MMI_HANDLE_T win_handle,
                                         GUITITLE_BUTTON_INDEX_T icon_index,
                                         MMI_STRING_T *str_ptr,
                                         BOOLEAN is_need_update
                                         )
{
    BOOLEAN result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    CTRLTITLE_OBJ_T *title_ptr = PNULL;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 == title_handle)
    {
        win_handle = MMK_GetParentWinHandle(win_handle);
        title_handle = MMK_GetWinTitleHandle(win_handle);
    }

    if (0 != title_handle)
    {
        title_ptr = GUITITLE_GetTitlePtr(win_handle, title_handle);

        result = GUITITLE_SetButtonTextByPtr(title_ptr, icon_index, str_ptr, is_need_update);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : Set Istyle Icon Image
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonImage(    
                                          MMI_HANDLE_T win_handle,
                                          GUITITLE_BUTTON_INDEX_T icon_index,
                                          MMI_IMAGE_ID_T image_id,
                                          MMI_IMAGE_ID_T press_image_id,
                                          BOOLEAN is_need_update
                                          )
{
    BOOLEAN result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    CTRLTITLE_OBJ_T *title_ptr = PNULL;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 == title_handle)
    {
        win_handle = MMK_GetParentWinHandle(win_handle);
        title_handle = MMK_GetWinTitleHandle(win_handle);
    }

    if (0 != title_handle)
    {
        title_ptr = GUITITLE_GetTitlePtr(win_handle, title_handle);

        result = GUITITLE_SetButtonImage(title_ptr, icon_index, image_id, press_image_id, is_need_update);
    }

    return result;
}

/*****************************************************************************/
// 	Description : Set Istyle Icon Msg
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleButtonHandleMsgInfo(    
                                                  MMI_HANDLE_T win_handle,
                                                  GUITITLE_BUTTON_INDEX_T icon_index,
                                                  MMI_MESSAGE_ID_E button_msg_id,
                                                  MMI_CTRL_ID_T handle_msg_ctrl_id
                                                  )
{
    BOOLEAN result = FALSE;
    MMI_HANDLE_T title_handle = 0;
    CTRLTITLE_OBJ_T *title_ptr = PNULL;
    
    title_handle = MMK_GetWinTitleHandle(win_handle);
    
    if (0 == title_handle)
    {
        win_handle = MMK_GetParentWinHandle(win_handle);
        title_handle = MMK_GetWinTitleHandle(win_handle);
    }

    if (0 != title_handle)
    {
        title_ptr = GUITITLE_GetTitlePtr(win_handle, title_handle);

        result = GUITITLE_SetButtonHandleMsgInfo(title_ptr, icon_index, button_msg_id, handle_msg_ctrl_id);
    }
    
    return result;
}

/*****************************************************************************/
//  Description : ����item��bg
//  Global resource dependence : 
//  Author:xiyuan.ma 2012.1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemBg(
                                   uint32     item_index,
                                   GUI_BG_T   *bg_info_ptr
                                   )
{
    return GUISTATUSBAR_SetItemBg(item_index, bg_info_ptr);
}

/*****************************************************************************/
//  Description : set STATUSBAR owner draw callback function
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbOwnerDraw(
                                   uint32                          item_index,
                                   GUIWIN_STBDATA_OWNER_DRAW_FUNC  func
                                   )
{
    GUISTABATA_SetOwnerDraw(item_index, func);
}


