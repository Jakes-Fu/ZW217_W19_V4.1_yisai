/*! @file ctrldropdownlist_export.h
@brief  ���ļ�������droplist�ؼ��Ķ���ӿں��������� 
@author xiaoqing.lu
@version 1.0
@date 25/07/2012  
@remarks Copyright: 2012 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 25/07/2012     xiaoqing.lu         Create
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


#ifndef _CTRLDROPDOWNLIST_H_
#define _CTRLDROPDOWNLIST_H_ 

/*--------------------------------------------------------------------------
                         Include Files                                    
--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "sci_types.h"
#include "mmk_app.h"
#include "mmitheme_dropdownlist.h"
#include "ctrllist_export.h"

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
typedef struct _dropdownlist_obj_tag* CTRL_DROPDOWNLIST_HANDLE_T;

#define DROPDOWNLIST_STRING_MAX_NUM  56  //max string length of item 

/*--------------------------------------------------------------------------
                         TYPE AND STRUCT                                
--------------------------------------------------------------------------*/
typedef enum
{
    GUIDNLIST_VALID_VALUE_0,
    GUIDNLIST_VALID_VALUE_1,
    GUIDNLIST_VALID_VALUE_NONE,
    GUIDNLIST_VALID_VALUE_MAX
}GUIDROPDOWNLIST_VALID_VALUE_E;

/*! @enum GUIDROPDOWNLIST_RECT_STYLE_E
@brief DropDownList�ı߿���
*/
typedef enum
{ 
    GUIDNLIST_NONE_TYPE,/*!< none*/
    GUIDNLIST_RECT_SOLID, /*!< 3DЧ��*/
    GUIDNLIST_RECT_PLANE  /*!< ƽ��Ч��*/
}GUIDROPDOWNLIST_RECT_STYLE_E;


/*! @enum GUIDROPDOWNLIST_POSITION_E
@brief ���ʲ����ؼ�ʱ�����������λ��
*/
typedef enum
{
    GUIDNLIST_TEXT_RECT = 0,/*!< �ı�����*/
    GUIDNLIST_BUTTON_RECT,  /*!< ��ť����*/
    GUIDNLIST_LIST_RECT,		/*!< ����������*/
    GUIDNLIST_OTHER
}GUIDROPDOWNLIST_POSITION_E;


/*! @enum GUIDROPDOWNLIST_STATUS_E
@brief �����ؼ�ʱ��״̬
*/
typedef enum
{ 
    GUIDNLIST_OPEN_STATUS = 0,	/*!< �������ڴ�״̬*/
    GUIDNLIST_CLOSE_STATUS,		/*!< ��������δ��״̬*/
    GUIDNLIST_ERROR_STATUS /*!< ����״̬*/
}GUIDROPDOWNLIST_STATUS_E;


/*! @enum GUIDROPDOWNLIST_BUTTON_STATUS_E
@brief �ؼ���ť��״̬
*/
typedef enum
{
    GUIDNLIST_BUTTON_PRESSED = 0,/*!< ����״̬*/
    GUIDNLIST_BUTTON_RELEASED,	/*!< ����״̬*/
    GUIDNLIST_BUTTON_ERROR /*!< ����״̬*/
}GUIDROPDOWNLIST_BUTTON_STATUS_E;


/*! @enum GUIDROPDOWNLIST_SHOW_TYPE_E
@brief �������б�ʱ��ʾ��ʽ
*/
typedef enum
{
    GUIDNLIST_SHOW_DOWNWARD = 0,/*!< ���´��б��*/
    GUIDNLIST_SHOW_UPWARD,	    /*!< ���ϴ��б��*/
    GUIDNLIST_SHOW_AUTOSET, /*!< ���ݿؼ���λ������Ӧ*/
    GUIDNLIST_SHOW_MAX
}GUIDROPDOWNLIST_SHOW_TYPE_E;

/*! @enum GUIDROPDOWNLIST_TYPE_E
@brief �ؼ���ʱ�ķ�ʽ
*/
typedef enum
{
    GUIDNLIST_NORMAL = 0,           /*!< һ���������*/
    GUIDNLIST_EDITABLE_TEXT,	    /*!< �ɱ༭��������(Text)*/
    GUIDNLIST_EDITABLE_NUMLIST,	    /*!< �ɱ༭��������(number list)*/
    GUIDNLIST_EDITABLE_PHONENUM,	/*!< �ɱ༭��������(phone number)*/

    GUIDNLIST_STYLE_MAX
}GUIDROPDOWNLIST_TYPE_E;

/*! @struct GUIDROPDOWNLIST_TYPE_T
@brief ���ÿؼ�������
*/
typedef struct 
{
    GUIDROPDOWNLIST_TYPE_E     ctrl_type; /*!< ���ÿؼ�������*/

    uint32 editable_max_text_num;
    uint32 editable_max_listitem_num;

}GUIDROPDOWNLIST_TYPE_T;

/*! @struct GUIDROPDOWNLIST_INIT_DATA_T
@brief �ؼ���ʼ����Ϣ
*/
typedef struct 
{
    GUI_BOTH_RECT_T                 both_rect;/*!< �ؼ��ĺ���������*/
    uint16                          page_item_num; /*!< ÿҳ��ʾ���������������������ʾ����*/
    GUIDROPDOWNLIST_SHOW_TYPE_E     show_type; /*!< �������б�ʱ��ʾ��ʽ*/
    
}GUIDROPDOWNLIST_INIT_DATA_T;

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
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetCtrlType(
                                           MMI_HANDLE_T  win_handle,    //window id
                                           MMI_CTRL_ID_T ctrl_id,       // control id
                                           const GUIDROPDOWNLIST_TYPE_T *type_info_ptr
                                           );

/***************************************************************************//*!
@brief ��ȡ�ؼ�������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param win_handle [in] �ؼ��ĸ�����ID����
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC GUIDROPDOWNLIST_TYPE_T CTRLDROPDOWNLIST_GetCtrlType(
                                                          MMI_HANDLE_T  win_handle,    //window id
                                                          MMI_CTRL_ID_T ctrl_id
                                                          );

/***************************************************************************//*!
@brief 
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param win_handle [in] �ؼ��ĸ�����ID����
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC MMI_HANDLE_T CTRLDROPDOWNLIST_GetInlineEditHandle(
                                                        MMI_HANDLE_T  win_handle,    //window id
                                                        MMI_CTRL_ID_T ctrl_id
                                                        );

/***************************************************************************//*!
@brief ��ȡ�ؼ�������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param win_handle [in] �ؼ��ĸ�����ID����
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC void CTRLDROPDOWNLIST_SetButton(
                                      MMI_HANDLE_T  win_handle,    //window id
                                      MMI_CTRL_ID_T ctrl_id,
                                      BOOLEAN is_show_button
                                      );

/***************************************************************************//*!
@brief ��̬�����ӿ�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param win_handle [in] �ؼ��ĸ�����ID����
@param init_ptr [in] ��ʼ����Ϣ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_CreateDyn(
                                         MMI_CTRL_ID_T ctrl_id, // control id
                                         MMI_HANDLE_T  win_handle,//window id
                                         GUIDROPDOWNLIST_INIT_DATA_T *init_ptr
                                         );

/***************************************************************************//*!
@brief ����������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param style [in] �ؼ�����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetRectStyle( 
                                            MMI_CTRL_ID_T                ctrl_id, // control id
                                            GUIDROPDOWNLIST_RECT_STYLE_E style
                                            );

/***************************************************************************//*!
@brief ���ÿؼ���Ƕlist״̬
@author xiaoqing.lu
@param ctrl_id [in] ������ؼ�ID
@param state [in] list״̬
@param is_true [in] ����orȡ������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetListState(
                                            MMI_CTRL_ID_T  ctrl_id,
                                            GUILIST_STATE_T  state,
                                            BOOLEAN is_true
                                            );
                                            

/***************************************************************************//*!
@brief ���item����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@retval ���ؿؼ�item����
@return ���ؿؼ�item����
@note 
*******************************************************************************/
PUBLIC uint16 CTRLDROPDOWNLIST_GetTotalItemNum(
                                              MMI_CTRL_ID_T ctrl_id
                                              );
                                                         
/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
PUBLIC uint16 CTRLDROPDOWNLIST_GetMaxtemNum( //return total items
                                           MMI_CTRL_ID_T ctrl_id  // IN// control id
                                           );

/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_IsFocus( //return total items
                                       MMI_CTRL_ID_T ctrl_id  // IN// control id
                                       );

/***************************************************************************//*!
@brief append item
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_array_ptr [in] ITEM����
@param total_num [in] ����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItemArray( 
                                               MMI_CTRL_ID_T ctrl_id, // control id
                                               MMI_STRING_T *item_array_ptr,//load item array
                                               uint16 array_size //item array total num
                                               );

/***************************************************************************//*!
@brief append item
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_ptr [in] ITEM��Ϣ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItem( 
                                          MMI_CTRL_ID_T ctrl_id, // control id
                                          const MMI_STRING_T *item_ptr //item info
                                          );

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
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AddItemDetail( 
                                             MMI_CTRL_ID_T ctrl_id, // control id
                                             uint16 item_index,
                                             const MMI_STRING_T *item_ptr //item info
                                             );

/***************************************************************************//*!
@brief append item
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param text_id [in] ITEM text id
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItemById( 
                                              MMI_CTRL_ID_T ctrl_id, // control id
                                              MMI_TEXT_ID_T text_id //text id
                                              );

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
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AddItemDetailById( 
                                                 MMI_CTRL_ID_T ctrl_id, // control id
                                                 uint16 item_index,
                                                 MMI_TEXT_ID_T text_id //item info
                                                 );

/*****************************************************************************//*!
@brief �����Ƿ���ʾitem �����飬�����ʾ�������ʾ˫���б�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_disp_detail [in] �Ƿ���ʾ��Ĭ�ϲ���ʾ
@retval true/false
@return ���سɹ���ʧ��
@note 
*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetDispItemDetail(
                                                 MMI_CTRL_ID_T ctrl_id,
                                                 BOOLEAN       is_disp_detail       
                                                 );

/***************************************************************************//*!
@brief set max item
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param max_item_num [in] item num
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetMaxItem(
                                          MMI_CTRL_ID_T ctrl_id, // control id
                                          uint16 max_item_num
                                          );

/***************************************************************************//*!
@brief ��ñ�ѡ����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@retval item�ṹ
@return ���ر�ѡ�����������Ϣ
@note 
*******************************************************************************/
PUBLIC MMI_STRING_T CTRLDROPDOWNLIST_GetCurItem( //return selected check item number
                                               MMI_CTRL_ID_T ctrl_id  // IN// control id
                                               );


/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
PUBLIC MMI_STRING_T CTRLDROPDOWNLIST_GetItem( //return selected check item number
                                            MMI_CTRL_ID_T ctrl_id,  // IN// control id
                                            uint16 item_index
                                            );

/***************************************************************************//*!
@brief ���õ�ǰ��ITEM
@author xiaoqing.lu
@param ctrl_id [in]�ؼ�ID
@param index [in]�ؼ�������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetCurItemIndex(
                                               MMI_CTRL_ID_T ctrl_id,
                                               uint16        index        
                                               );


/***************************************************************************//*!
@brief ��õ�ǰitem������
@author xiaoqing.lu
@param ctrl_id [in]�ؼ�ID
@retval uint16
@return ���ص�ǰ������
@note 
*******************************************************************************/
PUBLIC uint16 CTRLDROPDOWNLIST_GetCurItemIndex( 
                                              MMI_CTRL_ID_T ctrl_id  // IN// control id
                                              );


/***************************************************************************//*!
@brief �����Ƿ���ʾʡ�Ե�
@author xiaoqing.lu
@param ctrl_id [in]�ؼ�ID
@param is_ellipsis [in]�Ƿ���ʾʡ�Ժ�
@retval true/false
@return ���سɹ���ʧ��
@note ���ؼ�item���ı������ؼ�����ʱ���Ƿ�ʹ��ʡ�Ժ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetEllipsis(
                                           MMI_CTRL_ID_T ctrl_id,
                                           BOOLEAN       is_ellipsis       
                                           );


/***************************************************************************//*!
@brief ���ÿؼ�����
@author xiaoqing.lu
@param ctrl_handle [in]�ؼ����
@param rect_ptr ��[in]������
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLDROPDOWNLIST_SetRect(
                                    MMI_HANDLE_T    ctrl_handle,
                                    GUI_RECT_T      *rect_ptr
                                    );

/*****************************************************************************/
//  Description : set list control background
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDROPDOWNLIST_SetListBg(
                                      MMI_HANDLE_T    ctrl_handle,
                                      GUI_BG_T      *bg_ptr
                                      );

/***************************************************************************//*!
@brief ���ÿؼ��Ƿ�ɼ�
@author xiaoqing.lu
@param ctrl_id    [in] �ؼ�id
@param is_visible [in] �Ƿ�ɼ�
@param is_update  [in] �Ƿ�����ˢ��
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetVisible(
                                          MMI_CTRL_ID_T     ctrl_id,
                                          BOOLEAN           is_visible,
                                          BOOLEAN           is_update
                                          );

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_OpenList(
                                        MMI_HANDLE_T  win_handle,    //window id
                                        MMI_CTRL_ID_T ctrl_id
                                        );

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetPageNum(
                                          MMI_HANDLE_T  win_handle,    //window id
                                          MMI_CTRL_ID_T ctrl_id,
                                          uint16 page_item_num
                                          );

/***************************************************************************//*!
@brief ���ÿؼ��ı߿�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param border_ptr [in] �ؼ��߿���Ϣ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetBorder(
                                         MMI_CTRL_ID_T ctrl_id,       // control id
                                         GUI_BORDER_T  *border_ptr
                                         );

/*****************************************************************************/
//  Description : �����ַ���,���dropdownlist
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:��ӳɹ�����TRUE��ʧ�ܷ���FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItemExt( 
                                             MMI_CTRL_ID_T ctrl_id, // control id
                                             const MMI_STRING_T *item_ptr, //item info
                                             const MMI_STRING_T *item_detail_ptr //item info
                                             );

/*****************************************************************************/
// 	Description : close dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_CloseList(
                                         MMI_HANDLE_T  win_handle,    //window id
                                         MMI_CTRL_ID_T ctrl_id
                                         );

/*****************************************************************************/
//  Description : ������Чֵ
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetValidItemValue( 
                                                 MMI_CTRL_ID_T      ctrl_id, // control id
                                                 uint16 item_index,
                                                 GUIDROPDOWNLIST_VALID_VALUE_E index
                                                 );

/*****************************************************************************/
//Description : �����м�softkey��web���Ƿ��list��Ĭ�϶�Ϊtrue
//Global resource dependence : 
//Author: xiaoqing.lu
//Note: �������óɹ���ʧ��
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetOpenListMethod(
                                                 MMI_CTRL_ID_T ctrl_id,
                                                 BOOLEAN is_mid_softkey_open_list, /*!< �Ƿ��м�softkey����ʱ����������*/ 
                                                 BOOLEAN is_web_key_open_list      /*!< �Ƿ�web������ʱ����������*/ 
                                                 );

/***************************************************************************//*!
@brief ��ȡ�ؼ�������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC GUIDROPDOWNLIST_STATUS_E CTRLDROPDOWNLIST_GetCtrlStatus(
                                                               MMI_CTRL_ID_T ctrl_id
                                                               );

/*--------------------------------------------------------------------------
                        Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif // _CTRLDROPDOWNLIST_H_

/*@}*/


