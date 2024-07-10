/*! @file guidropdownlist.h
@brief  ���ļ�������droplist�ؼ��Ķ���ӿں��������� 
@author yelianna
@version 1.0
@date 04/04/2005  
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 04/04/2005     yelianna         Create
*******************************************************************************/

/*! @page page_DropdownList ������(DropdownList)

-# @ref sec_dropdownlist_function
-# @ref sec_dropdownlist_def
-# @ref sec_dropdownlist_example
-# @ref sec_dropdownlist_function_list
-# @ref sec_dropdownlist_app

@section sec_dropdownlist_function �ؼ�����

- Dropdownlist��������ؼ�����������������Ϣ������һ�𣬵��������ʱ����������Ϣ���б����ʽ��ʾ����������һҳʱ����������ʾ���������û����Բ�����������ʾҳ��֮������ݡ�
- ������ķ����3D����ƽ�������֣����ı���Ϣ��������Ϣ�ȣ�������ͨ���ӿڽ������á�
- ������ؼ�չ��ʱ������ʾ���б�ֻ���ı���Ϣ����ÿ��������ı�����ʽ��ʾ�����ģ���֧����ʾ������Ϣ��
- ������ؼ���Ŀ����Ϊͬ����Ϣ�ṩһ��������ʾ�ļ��ϣ�DataSet����������������ʱ������ʾ�ļ����б��б�ѡ�еĵ�ǰ���ˣ���������һ������ѡ��Ŀؼ���

@section sec_dropdownlist_def �궨�����

@code
#define CREATE_DROPDOWNLIST_CTRL(PAGE_ITEM_NUM, _TYPE_, _CTRL_ID_) \
        MMK_CREATE_DROPDOWNLIST, PAGE_ITEM_NUM, _TYPE_, _CTRL_ID_
@endcode
- ���ڴ���һ��������ؼ�
    - _TYPE_ ��������ʾ�����ͣ��������ϻ�����չ����ʾ�б�Ҳ��������Ӧ
    - PAGE_ITEM_NUM ÿҳ��ʾ���б�����
    - _CTRL_ID_ �ؼ�ID��ΪMMI_CONTROL_ID_Eö������

@section sec_dropdownlist_example �ؼ�ʹ��ʾ��

���벿�ַ���sample_test_dropdownlist.c�ļ��С�

�������ȴ���һ�����Կؼ��Ĵ��ڣ�����Table����ΪMMI_TEST_DROPDOWNLIST_WIN_TAB��
@code
WINDOW_TABLE( MMI_TEST_DROPDOWNLIST_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_DROPDOWNLIST_WIN_ID ),
    WIN_FUNC((uint32)HandleTestDropdownlistWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_DROPDOWNLIST),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
CREATE_DROPDOWNLIST_CTRL(3,GUIDNLIST_SHOW_DOWNWARD, MMI_TEST_DROPDOWNLIST_CTRL_ID),
    END_WIN
};
@endcode
Ȼ��Ϊ�������Ӵ�������
@code
LOCAL MMI_RESULT_E HandleTestDropdownlistWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_DROPDOWNLIST_CTRL_ID;
    GUI_RECT_T rect =  {40, 60, 219, 85}
                       3,

                       switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_DROPDOWNLIST_ITEM_T item_arry[MMI_HELLO_DROPDOWNLIST_NUM] = {0};
            uint32 i = 0;

            //����dropdownlist acitve
            MMK_SetAtvCtrl(win_id, ctrl_id);
            CTRLDROPDOWNLIST_SetRect(win_id, &rect);

            for (i = 0; i < MMI_HELLO_DROPDOWNLIST_NUM; i++)
            {
                wchar  wstr[16] = {0};
                char   str[16]  = {0};
                uint16 str_len  = 0;

                str_len = sprintf(str, "test item %d", i);

                MMI_STRNTOWSTR(wstr, 16, str, 16, str_len);

                MMI_WSTRNCPY(item_arry[i].wstr, DROPDOWNLIST_STRING_MAX_NUM, wstr, str_len, str_len);

                item_arry[i].wstr_len = str_len;
            }

            //����item
            CTRLDROPDOWNLIST_LoadItemArray(ctrl_id, item_arry, MMI_HELLO_DROPDOWNLIST_NUM);

            //���õ�ǰѡ�е�item
            CTRLDROPDOWNLIST_SetSelectedItemId(ctrl_id, 0);
        }
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16 index = 0;

            //��õ�ǰ������
            index = CTRLDROPDOWNLIST_GetSelectedItemId(ctrl_id);
        }
        break;
    case MSG_CTL_CANCEL:
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
@endcode

@section sec_dropdownlist_function_list �ؼ��ӿڽ���

-# @ref CTRLDROPDOWNLIST_AppendItemArray
-# @ref CTRLDROPDOWNLIST_SetCurItemIndex
-# @ref CTRLDROPDOWNLIST_GetCurItemIndex
-# @ref CTRLDROPDOWNLIST_GetCurItem
-# @ref CTRLDROPDOWNLIST_CreateDyn

@section sec_dropdownlist_app �ؼ�Ӧ��

���ճ̴��������У�����ʹ��������������Ϊ�ճ��������͵�ѡ��ؼ�������ͼ��ʾ��
@image html dropdownlist_app_example.jpg 
ͼ 2 33 �ؼ���ʾ��ͼ

�������������м������ճ����ѵ����ͣ�����������һ�Ρ�����ÿ�����ѡ�����ÿ�����ѡ�����ÿ�����ѡ�����ÿ�����ѡ����������ѡ������ݡ�
*******************************************************************************/

/*! @addtogroup  dropdownlistGrp DropdownList 
@brief dropdownlist �ؼ�
@details dropdownlist �ؼ�
@{
*******************************************************************************/


#ifndef _GUIDROPDOWNLIST_H_
#define _GUIDROPDOWNLIST_H_ 

/*--------------------------------------------------------------------------
                         Include Files                                    
--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "sci_types.h"
#include "ctrldropdownlist_export.h"


/*--------------------------------------------------------------------------
                         Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*--------------------------------------------------------------------------
                         MACRO DEFINITION                                 
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                         TYPE AND STRUCT                                
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                         PUBLIC FUNCTION DECLARE                               
--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ���ÿؼ�������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param win_handle [in] �ؼ��ĸ�����ID����
@param type_info_ptr [in] ��ʼ����Ϣ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define  GUIDROPDOWNLIST_SetCtrlType CTRLDROPDOWNLIST_SetCtrlType

/***************************************************************************//*!
@brief ��ȡ�ؼ�������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param win_handle [in] �ؼ��ĸ�����ID����
@retval 
@return 
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetCtrlType CTRLDROPDOWNLIST_GetCtrlType

/***************************************************************************//*!
@brief 
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param win_handle [in] �ؼ��ĸ�����ID����
@retval 
@return 
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetInlineEditHandle CTRLDROPDOWNLIST_GetInlineEditHandle

/***************************************************************************//*!
@brief ��ȡ�ؼ�������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param win_handle [in] �ؼ��ĸ�����ID����
@retval 
@return 
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetButton CTRLDROPDOWNLIST_SetButton

/***************************************************************************//*!
@brief ��̬�����ӿ�
@author yelianna
@param ctrl_id [in] �ؼ�ID
@param win_handle [in] �ؼ��ĸ�����ID����
@param init_ptr [in] ��ʼ����Ϣ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_CreateDyn CTRLDROPDOWNLIST_CreateDyn

/***************************************************************************//*!
@brief ����������
@author yelianna
@param ctrl_id [in] �ؼ�ID
@param style [in] �ؼ�����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetRectStyle CTRLDROPDOWNLIST_SetRectStyle

/***************************************************************************//*!
@brief ���ÿؼ���Ƕlist״̬
@author Lianxiang.Zhou
@param ctrl_id [in] ������ؼ�ID
@param state [in] list״̬
@param is_true [in] ����orȡ������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetListState CTRLDROPDOWNLIST_SetListState

/***************************************************************************//*!
@brief ���item����
@author fen.xie
@param ctrl_id [in] �ؼ�ID
@retval ���ؿؼ�item����
@return ���ؿؼ�item����
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetTotalItemNum CTRLDROPDOWNLIST_GetTotalItemNum
                                                         
/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:fen.xie
//Note:return current selected item
/*****************************************************************************/
#define GUIDROPDOWNLIST_GetMaxtemNum CTRLDROPDOWNLIST_GetMaxtemNum

/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:fen.xie
//Note:return current selected item
/*****************************************************************************/
#define GUIDROPDOWNLIST_IsFocus CTRLDROPDOWNLIST_IsFocus

/***************************************************************************//*!
@brief append item
@author yelianna
@param ctrl_id [in] �ؼ�ID
@param item_array_ptr [in] ITEM����
@param total_num [in] ����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_AppendItemArray CTRLDROPDOWNLIST_AppendItemArray

/***************************************************************************//*!
@brief append item
@author jin.wang
@param ctrl_id [in] �ؼ�ID
@param item_ptr [in] ITEM��Ϣ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_AppendItem CTRLDROPDOWNLIST_AppendItem

/*****************************************************************************//*!
@brief �����ַ���,���dropdownlist item ������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_ptr [in] ITEM text string
@param item_index [in] ITEM index
@retval true/false
@return ���سɹ���ʧ��
@note 
*****************************************************************************/
#define GUIDROPDOWNLIST_AddItemDetail CTRLDROPDOWNLIST_AddItemDetail

/***************************************************************************//*!
@brief append item
@author jin.wang
@param ctrl_id [in] �ؼ�ID
@param text_id [in] ITEM text id
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_AppendItemById CTRLDROPDOWNLIST_AppendItemById

/*****************************************************************************//*!
@brief �����ַ���,���dropdownlist item ������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param text_id [in] ITEM text id
@param item_index [in] ITEM index
@retval true/false
@return ���سɹ���ʧ��
@note 
*****************************************************************************/
#define GUIDROPDOWNLIST_AddItemDetailById CTRLDROPDOWNLIST_AddItemDetailById

/*****************************************************************************//*!
@brief �����Ƿ���ʾitem �����飬�����ʾ�������ʾ˫���б�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_disp_detail [in] �Ƿ���ʾ��Ĭ�ϲ���ʾ
@retval true/false
@return ���سɹ���ʧ��
@note 
*****************************************************************************/
#define GUIDROPDOWNLIST_SetDispItemDetail CTRLDROPDOWNLIST_SetDispItemDetail

/***************************************************************************//*!
@brief set max item
@author jin.wang
@param ctrl_id [in] �ؼ�ID
@param max_item_num [in] item num
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetMaxItem CTRLDROPDOWNLIST_SetMaxItem

/***************************************************************************//*!
@brief ��ñ�ѡ����
@author yelianna
@param ctrl_id [in] �ؼ�ID
@retval item�ṹ
@return ���ر�ѡ�����������Ϣ
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetCurItem CTRLDROPDOWNLIST_GetCurItem

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:yelianna
//Note:return current selected item
/*****************************************************************************/
#define GUIDROPDOWNLIST_GetItem CTRLDROPDOWNLIST_GetItem

/***************************************************************************//*!
@brief ���õ�ǰ��ITEM
@author yelianna
@param ctrl_id [in]�ؼ�ID
@param index [in]�ؼ�������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetCurItemIndex CTRLDROPDOWNLIST_SetCurItemIndex


/***************************************************************************//*!
@brief ��õ�ǰitem������
@author yelianna
@param ctrl_id [in]�ؼ�ID
@retval uint16
@return ���ص�ǰ������
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetCurItemIndex CTRLDROPDOWNLIST_GetCurItemIndex


/***************************************************************************//*!
@brief �����Ƿ���ʾʡ�Ե�
@author yelianna
@param ctrl_id [in]�ؼ�ID
@param is_ellipsis [in]�Ƿ���ʾʡ�Ժ�
@retval true/false
@return ���سɹ���ʧ��
@note ���ؼ�item���ı������ؼ�����ʱ���Ƿ�ʹ��ʡ�Ժ�
*******************************************************************************/
#define GUIDROPDOWNLIST_SetEllipsis CTRLDROPDOWNLIST_SetEllipsis

/***************************************************************************//*!
@brief ���ÿؼ�����
@author Jassmine
@param ctrl_handle [in]�ؼ����
@param rect_ptr ��[in]������
@retval void
@return void
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetRect CTRLDROPDOWNLIST_SetRect

/*****************************************************************************/
//  Description : set list control background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_SetListBg CTRLDROPDOWNLIST_SetListBg

/***************************************************************************//*!
@brief ���ÿؼ��Ƿ�ɼ�
@author Jassmine
@param ctrl_id    [in] �ؼ�id
@param is_visible [in] �Ƿ�ɼ�
@param is_update  [in] �Ƿ�����ˢ��
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetVisible CTRLDROPDOWNLIST_SetVisible

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_OpenList CTRLDROPDOWNLIST_OpenList

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_SetPageNum CTRLDROPDOWNLIST_SetPageNum

/***************************************************************************//*!
@brief ���ÿؼ��ı߿�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param border_ptr [in] �ؼ��߿���Ϣ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetBorder CTRLDROPDOWNLIST_SetBorder

/*****************************************************************************/
//  Description : �����ַ���,���dropdownlist
//  Global resource dependence : 
//  Author: jin.wang
//  Note:��ӳɹ�����TRUE��ʧ�ܷ���FALSE
/*****************************************************************************/
#define GUIDROPDOWNLIST_AppendItemExt CTRLDROPDOWNLIST_AppendItemExt

/*****************************************************************************/
// 	Description : close dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_CloseList CTRLDROPDOWNLIST_CloseList

/*****************************************************************************/
//  Description : ������Чֵ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_SetValidItemValue CTRLDROPDOWNLIST_SetValidItemValue

/*****************************************************************************/
//Description : �����м�softkey��web���Ƿ��list��Ĭ�϶�Ϊtrue
//Global resource dependence : 
//Author: xiaoqing.lu
//Note: �������óɹ���ʧ��
/*****************************************************************************/
#define GUIDROPDOWNLIST_SetOpenListMethod CTRLDROPDOWNLIST_SetOpenListMethod

/***************************************************************************//*!
@brief ��ȡ�ؼ�������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@retval 
@return 
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetCtrlStatus CTRLDROPDOWNLIST_GetCtrlStatus

/*--------------------------------------------------------------------------
                        Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif

/*@}*/


