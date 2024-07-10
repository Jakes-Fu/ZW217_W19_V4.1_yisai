/*****************************************************************************
** File Name:      guicombox.c                                               *
** Author:                                                                   *
** Date:             09/18/2012                                               *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME                                 DESCRIPTION                               *
** 09/2012     xiyuan.ma & nan.ji             Creat
******************************************************************************/
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "guicombox.h"
#include "ctrlcombox_export.h"
//#include "mmitheme_list.h"
#include "mmi_gui_trc.h"
#include "guistring.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Convert Show Type
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL CTRLCOMBOX_SHOW_TYPE_E ConvertShowType (
                                              GUICOMBOX_SHOW_TYPE_E expand_direction
                                              );

/*****************************************************************************/
// 	Description : Convert Edit Type
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL void ConvertEditType (
                            const GUICOMBOX_TEXT_TYPE_T *src_edit_type_ptr,
                            CTRLCOMBOX_TEXT_TYPE_T *dest_edit_type_ptr
                            );

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Convert Show Type
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL CTRLCOMBOX_SHOW_TYPE_E ConvertShowType (
                                              GUICOMBOX_SHOW_TYPE_E expand_direction
                                              )
{
    CTRLCOMBOX_SHOW_TYPE_E show_type = CTRLCOMBOX_SHOW_DOWNWARD;
    
    switch (expand_direction)
    {
    case GUICOMBOX_SHOW_DOWNWARD:
        show_type = CTRLCOMBOX_SHOW_DOWNWARD;
        break;
        
    case GUICOMBOX_SHOW_UPWARD:
        show_type = CTRLCOMBOX_SHOW_UPWARD;
        break;
        
    case GUICOMBOX_SHOW_AUTOSET:
        show_type = CTRLCOMBOX_SHOW_AUTOSET;
        break;
        
    case GUICOMBOX_SHOW_MAX:
        show_type = CTRLCOMBOX_SHOW_MAX;
        break;
        
    default:
        SCI_TRACE_LOW ("ConvertShowType unknow type");
        break;
    }
    
    return show_type;
}

/*****************************************************************************/
// 	Description : Convert Edit Type
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL void ConvertEditType (
                            const GUICOMBOX_TEXT_TYPE_T *src_edit_type_ptr,
                            CTRLCOMBOX_TEXT_TYPE_T *dest_edit_type_ptr
                            )
{
    if (PNULL != dest_edit_type_ptr
        && PNULL != src_edit_type_ptr)
    {
        dest_edit_type_ptr->edit_type = src_edit_type_ptr->edit_type;
        dest_edit_type_ptr->editable_max_listitem_num =  src_edit_type_ptr->editable_max_listitem_num;
        dest_edit_type_ptr->editable_max_text_num = src_edit_type_ptr->editable_max_text_num;
    }
    
    return;
}

/*****************************************************************************/
// 	Description : GUICOMBOX SetExpandDirection
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_SetExpandDirection (
                                              MMI_CTRL_ID_T    ctrl_id,
                                              GUICOMBOX_SHOW_TYPE_E expand_direction
                                              )
{
    CTRLCOMBOX_SHOW_TYPE_E show_type = CTRLCOMBOX_SHOW_DOWNWARD;
    
    show_type = ConvertShowType (expand_direction);
    
    return CTRLCOMBOX_SetExpandDirection (ctrl_id, show_type);
}

/*****************************************************************************/
// 	Description : GUICOMBOX AddItemDetail
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_AddItemDetail (
                                         MMI_CTRL_ID_T ctrl_id,
                                         const GUICOMBOX_DETAIL_STR_DATA_T *data_ptr
                                         )
{
    BOOLEAN             result = FALSE;
    GUILIST_ITEM_T      item = {0};
    MMI_HANDLE_T        list_handle = 0;    
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    if (PNULL != data_ptr)
    {
        list_handle = GUICOMBOX_GetInlineListHandle(ctrl_id);
        item.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
        item.item_data_ptr = &item_data;
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = data_ptr->str.wstr_ptr;
        item_data.item_content[0].item_data.text_buffer.wstr_len = data_ptr->str.wstr_len;
        
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = data_ptr->detail_str.wstr_ptr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = data_ptr->detail_str.wstr_len;
        
        result = GUILIST_AppendItem(list_handle,&item);
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : GUICOMBOX AddItemDetailArray
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_AddItemDetailArray (
                                              MMI_CTRL_ID_T ctrl_id,
                                              const GUICOMBOX_DETAIL_STR_DATA_T *data_ptr,
                                              uint16 array_size
                                              )
{
    uint16    i = 0;
    BOOLEAN   result = FALSE;
    
    if (array_size > 0
        && PNULL != data_ptr)
    {
        for (i = 0; i < array_size; i++)
        {
            result = GUICOMBOX_AddItemDetail (ctrl_id, &data_ptr[i]);
            
            if (!result)
            {
                SCI_TRACE_LOW ("CTRLCOMBOX_AddItemArray  AppendItem failed");
                break;
            }
        }
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : GUICOMBOX  InsertItem
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_InsertItem (
                                      MMI_CTRL_ID_T ctrl_id,
                                      const GUILIST_ITEM_T*   item_ptr,
                                      uint16 pos
                                      )
{
    BOOLEAN        result = FALSE;
    MMI_HANDLE_T   list_handle = 0;    
    
    if (PNULL != item_ptr)
    {
        list_handle = GUICOMBOX_GetInlineListHandle(ctrl_id);
        result = GUILIST_InsertItem (list_handle, item_ptr, pos);
    }
    return result;
}

/*****************************************************************************/
// 	Description : GUICOMBOX  GetItem
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC const GUILIST_ITEM_T* GUICOMBOX_GetItem(
                                               MMI_CTRL_ID_T ctrl_id,
                                               uint16 index
                                               )
{
    return GUILIST_GetItemPtrByIndex (GUICOMBOX_GetInlineListHandle(ctrl_id), index);
}

/*****************************************************************************/
// 	Description : GUICOMBOX SetEditType
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_SetEditType (
                                       MMI_CTRL_ID_T ctrl_id,
                                       const GUICOMBOX_TEXT_TYPE_T *edit_type_ptr
                                       )
{
    CTRLCOMBOX_TEXT_TYPE_T edit_type = {0};
    
    ConvertEditType (edit_type_ptr, &edit_type);
    
    return CTRLCOMBOX_SetEditType (ctrl_id, &edit_type);
}


