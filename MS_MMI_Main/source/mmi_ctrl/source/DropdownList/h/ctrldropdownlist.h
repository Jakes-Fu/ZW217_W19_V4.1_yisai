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
            GUIDROPDOWNLIST_SetRect(win_id, &rect);

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
            GUIDROPDOWNLIST_LoadItemArray(ctrl_id, item_arry, MMI_HELLO_DROPDOWNLIST_NUM);

            //���õ�ǰѡ�е�item
            GUIDROPDOWNLIST_SetSelectedItemId(ctrl_id, 0);
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
            index = GUIDROPDOWNLIST_GetSelectedItemId(ctrl_id);
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

-# @ref GUIDROPDOWNLIST_AppendItemArray
-# @ref GUIDROPDOWNLIST_SetCurItemIndex
-# @ref GUIDROPDOWNLIST_GetCurItemIndex
-# @ref GUIDROPDOWNLIST_GetCurItem
-# @ref GUIDROPDOWNLIST_CreateDyn

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


#ifndef _GUIDROPDOWNLIST_INTERNAL_H_
#define _GUIDROPDOWNLIST_INTERNAL_H_ 

/*--------------------------------------------------------------------------
                         Include Files                                    
--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "sci_types.h"
#include "mmk_app.h"
#include "guifont.h"
#include "mmitheme_dropdownlist.h"
#include "guilcd.h"
#include "ctrldropdownlist_export.h"

/*--------------------------------------------------------------------------
                         Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*----------------------------------------------------------------------------*/
/*                         Constant Variables                                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
// list type info
#define CTRL_DROPDOWNLIST_TYPE     (DROPDOWNLIST_GetType())
    
/*! @struct GUIDROPDOWNLIST_INIT_DATA_T
@brief �ؼ���ʼ����Ϣ
*/
typedef struct 
{
    CTRLBASE_INIT_PARAM_T           base_ctrl_param;

    uint16                          page_item_num; /*!< ÿҳ��ʾ���������������������ʾ����*/
    GUIDROPDOWNLIST_SHOW_TYPE_E     show_type; /*!< �������б�ʱ��ʾ��ʽ*/
    
}CTRLDROPDOWNLIST_INIT_DATA_T;

/*! @struct CTRLDROPDOWNLIST_OBJ_T
@brief �ؼ����ݽṹ
*/
typedef struct _dropdownlist_obj_tag
{
    CTRLBASE_OBJ_T                  base_ctrl;          /*!< �ؼ��������� */
    
    GUIDROPDOWNLIST_TYPE_T          ctrl_type_info;     /*!< �ؼ�������*/
    BOOLEAN                         is_focus;           /*!< �ؼ��Ƿ���Focus*/
//     BOOLEAN                         is_active;          /*!< �ؼ��Ƿ���active*/
    BOOLEAN                         is_ellipsis;        /*!< ����ʾ���µ�ʱ���Ƿ���ʾʡ�Ե�"...",ϵͳĬ�ϲ���ʾʡ�Ե�*/
    BOOLEAN                         is_has_button;      /*!< �Ƿ���button*/
    BOOLEAN                         is_disp_detail;     /*!< �Ƿ���ʾitem detail*/
    GUI_RECT_T                      orig_rect;          /*!< ��ʼʱ�� rect*/
    
    uint16					        page_item_num;      /*!< ÿҳ��ʾItem�����������ֵΪ�û����ã���Ĭ��ֵ*/
    uint16                          cur_page_item_num;  /*!< ��ǰ������ÿҳ��ʾ�����������ֵΪ��̬���������*/
    uint16					        total_item_num;     /*!< ����ITEM������*/
    uint16             		        max_item_num;	    /*!< ������������Item��*/
    uint16					        current_index;	    /*!< ��ǰ��ѡ�е�index*/
    BOOLEAN                         is_auto_size;       /*!< �Ƿ������Զ����´�С*/   
    MMI_STRING_T                    *all_item_ptr;	    /*!< ָ������item��ָ��*/
    MMI_STRING_T                    *all_item_detail_ptr; /*!< ��Ӧÿ��item������*/
    uint16                          *def_arry_ptr;      /*!< �������ʾ��������ֵ�Ǹ�����Чֵ��0-all_item_ptr, 1-all_item_detail_ptr*/
   
    MMI_DROPDOWNLIST_STYLE_T		style_info;         /*!< ��ʾ���*/
    GUIDROPDOWNLIST_SHOW_TYPE_E     show_type;          /*!< �������б�ʱ��ʾ��ʽ�����ϣ����£��Զ���Ӧ��*/
    GUIDROPDOWNLIST_STATUS_E        cur_status;	        /*!< ��ǰ״̬*/
    GUIDROPDOWNLIST_RECT_STYLE_E	rect_style;	        /*!< rect �߿���ʾ����*/
    GUIDROPDOWNLIST_BUTTON_STATUS_E button_status;      /*!< status of button*/ 

    BOOLEAN is_mid_softkey_open_list;                   /*!< �Ƿ��м�softkey����ʱ����������*/ 
    BOOLEAN is_web_key_open_list;                       /*!< �Ƿ�web������ʱ����������*/ 
    
    MMI_HANDLE_T                    edit_handle;        /*!< edit ctrl handle*/ 
    MMI_HANDLE_T                    list_handle;        /*!< list ctrl handle*/ 
    GUILIST_STATE_T                 list_state;         /*!< ��ǶList�ؼ���״̬*/
    MMI_WIN_ID_T                    win_id;             /*!< �ؼ��ĸ�����ID*/
}CTRLDROPDOWNLIST_OBJ_T;

// dropdownlist class for public or protected method
typedef struct _dropdownlist_vtbl_tag
{
    CTRLBASE_VTBL_T             base_ctrl_vtbl;
} CTRLDROPDOWNLIST_VTBL_T;

/*--------------------------------------------------------------------------
                         PUBLIC FUNCTION DECLARE                               
--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get list type
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T DROPDOWNLIST_GetType(void);

/*--------------------------------------------------------------------------
                        Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif // _GUIDROPDOWNLIST_INTERNAL_H_

/*@}*/
