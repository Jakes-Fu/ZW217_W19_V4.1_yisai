/*! @file ctrlstatusbar_data.h
@brief ���ļ�������statusbar_data�ؼ��Ķ���ӿں���������
@author xiaoqing.lu
@version 1.0
@date 12/23/2008
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 12/23/2008     Xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_StatusBar_data StatusBar_data

(����)

*******************************************************************************/

/*! @addtogroup statusbar_dataGrp StatusBar_data
@brief statusbar_dataģ��
@details ���ļ������⿪�ţ�����app��Ӧ�õ���guiwin.h�еķ�װ�ӿ�
@{
*******************************************************************************/

#ifndef _CTRL_STATUSBAR_DATA_H_
#define _CTRL_STATUSBAR_DATA_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guicommon.h"
#include "guifont.h"
#include "mmitheme_statusbar.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
/*! @typedef GUISTBDATA_ITEM_STATE_T
@brief ״̬���ؼ���item״̬
*/
typedef uint32 GUISTBDATA_ITEM_STATE_T;



/*! @typedef GUISTBDATA_APP_HANDLE_FUNC
@brief Item ����Ļص����������ͨ���˽ӿڴ򿪶�Ӧģ��ӿڵ�
*/
typedef BOOLEAN (*GUISTBDATA_APP_HANDLE_FUNC) (void);



/*! @enum GUISTBDATA_ICON_STYLE_E
@brief item��ʾ���
*/
typedef enum
{
    GUISTBDATA_ICON_NONE = 0,/*!< ��*/
    GUISTBDATA_ICON_NORMAL,/*!< ������ֻ��ʾ��ԴID*/
    GUISTBDATA_ICON_ANIM,/*!< ��ʾ����������ͼ��ʱ*/
    GUISTBDATA_ICON_WRE,/*!< wre��̨APͼ��,���ļ��м���ͼƬ*///add for wre_background by heng.xiao
    GUISTBDATA_ICON_MAX  /*!< ϵͳ����*/
} GUISTBDATA_ICON_STYLE_E;



/*! @struct GUISTBDATA_TEXT_T
@brief  �ı���Ϣ
*/
typedef struct
{
    BOOLEAN         is_display; /*!< �Ƿ���ʾ*/
    uint8           font_space; /*!< �ּ��*/
    GUI_FONT_T      font_type;  /*!< ����*/
    GUI_COLOR_T     font_color; /*!< ������ɫ*/
    GUI_ALIGN_E     align;			/*!< ���뷽ʽ*/
    wchar           wstr_ptr[MMISTATUSBAR_ITEM_TEXT_LEN];  /*!< ����buffer*/
    uint32          wstr_len;   /*!< ���ֳ���*/
} GUISTBDATA_TEXT_T;



/*! @struct GUISTBDATA_ICON_T
@brief item��ʾ��icon������Ϣ
*/
typedef struct
{
    BOOLEAN                     is_display; /*!< �Ƿ���ʾ*/
    uint32                      icon_num;   /*!< icon�������ֵΪMMISTATUSBAR_ITEM_ICON_FRAME*/
    uint32                      icon_array[MMISTATUSBAR_ITEM_ICON_FRAME]; /*!< icon����*/
    GUISTBDATA_ICON_STYLE_E     icon_type;  /*!< icon��ʾ����ǲ��Ƕ�̬��ʾ��*/
    BOOLEAN                     is_color_exchange;  /*!< ��ԴͼƬ�Ƿ�ʹ����ɫ�滻*/
    uint32                      color;              /*!< ��ɫ�滻ֵ*/
} GUISTBDATA_ICON_T;



/*! @struct GUISTATUSBAR_OWNER_DRAW_T
@brief �ؼ��Ի�ص�����������Ϣ
*/
typedef struct
{
    GUI_RECT_T                   display_rect;   /*!<��ʾ����*/
    MMI_HANDLE_T                 ctrl_handle;    /*!<�ؼ�handle*/
    GUI_LCD_DEV_INFO             lcd_dev;        /*!<lcd��Ϣ*/
} GUISTBDATA_OWNER_DRAW_T;



/*! @typedef GUISTATUSBAR_OWNER_DRAW_FUNC
@brief �ؼ��Ի�ص�����,Я������
*/
typedef void (*GUISTATUSBAR_OWNER_DRAW_FUNC) (
    GUISTBDATA_OWNER_DRAW_T *owner_draw_ptr
);



/*! @struct GUISTBDATA_ITEM_DATA_T
@brief item ������Ϣ
*/
typedef struct
{
    uint32                          cur_frame;          /*!< icon��ǰ֡*/
    GUI_RECT_T                      item_rect;          /*!< item����*/
    GUISTBDATA_TEXT_T               text_info;          /*!< ������Ϣ*/
    GUISTBDATA_ICON_T               icon_info;          /*!< icon��Ϣ*/
    GUI_BG_T                        bg_info;            /*!< bg��Ϣ*/
    GUISTBDATA_ITEM_STATE_T         item_state;         /*!< item״̬*/
    GUISTBDATA_APP_HANDLE_FUNC      app_handle_func;    /*!< item��Ӧ�Ŀ�ִ�й��ܽӿ�*/
    BOOLEAN 					    is_handle_callback; /*!< �Ƿ�ִ������Ĺ��ܽӿ�*/
    GUISTATUSBAR_OWNER_DRAW_FUNC    owner_draw_func;      /*!< owner draw function*/
} GUISTBDATA_ITEM_DATA_T;


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief initial the item data of status bar
@author Xiaoqing.Lu
@param item_max_num item [in] ��������û�����
@retval true/false
@return ��ʼ���ɹ���ʧ��
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_InitData (
    uint32 item_max_num
);


/***************************************************************************//*!
@brief Append item
@author Xiaoqing.Lu
@param item_data_ptr [in] item����ָ��
@retval true/false
@return �ɹ���ʧ��
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_AppendItemData (
    GUISTBDATA_ITEM_DATA_T *item_data_ptr
);


/***************************************************************************//*!
@brief ����item�Ƿ�ɼ�
@author xiaoqing.lu
@param item_index [in] ������
@param is_visible [in] �Ƿ�ɼ�
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemVisible (
    uint32   item_index,
    BOOLEAN  is_visible
);


/***************************************************************************//*!
@brief �ж�item�Ƿ�ɼ�
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemVisible (
    uint32   item_index
);

/***************************************************************************//*!
@brief ����item�Ƿ���Ч
@author xiaoqing.lu
@param item_index [in] ������
@param is_visible [in] �Ƿ���Ч
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemValid (
    uint32   item_index,
    BOOLEAN  is_valid
);

/***************************************************************************//*!
@brief ��ȡitem�Ƿ���Ч
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemValid (
    uint32   item_index
);

/***************************************************************************//*!
@brief ����disable״̬
@author xiaoqing.lu
@param item_index [in] ������
@param is_disable [in] �Ƿ�disable
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemDisable (
    uint32   item_index,
    BOOLEAN  is_disable
);


/***************************************************************************//*!
@brief �ж�disable
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemDisable (
    uint32   item_index
);


/***************************************************************************//*!
@brief set item handle function
@author xiaoqing.lu
@param item_index [in] ������
@param app_handle_func [in] ����ָ��
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemHandleFunc (
    uint32   item_index,
    GUISTBDATA_APP_HANDLE_FUNC app_handle_func
);


/***************************************************************************//*!
@brief set item handle function is valid or not
@author xiaoqing.lu
@param item_index [in] ������
@param is_valid [in] �Ƿ��ִ��
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemHandleFuncValid (
    uint32  item_index,
    BOOLEAN is_valid
);


/***************************************************************************//*!
@brief �ж��Ƿ��ִ��
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_ItemIsHandleFuncValid (
    uint32 item_index
);


/***************************************************************************//*!
@brief ����item icon
@author xiaoqing.lu
@param item_index [in] ������
@param icon_info_ptr [in] icon����
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIcon (
    uint32            item_index,
    const GUISTBDATA_ICON_T *icon_info_ptr
);


/***************************************************************************//*!
@brief ���icon����
@author xiaoqing.lu
@param item_index [in] ������
@param icon_info_ptr [in/out] icon����
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIcon (
    uint32            item_index,
    GUISTBDATA_ICON_T *icon_info_ptr
);


/***************************************************************************//*!
@brief ���õ�ǰitem frame
@author xiaoqing.lu
@param item_index [in] ������
@param cur_frame [in] ��ǰ֡
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconCurFrame (
    uint32 item_index,
    uint32 cur_frame
);


/***************************************************************************//*!
@brief ��ȡ��ǰ֡
@author xiaoqing.lu
@param item_index [in] ������
@param cur_frame_ptr [in/out] ��ǰ֡
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIconCurFrame (
    uint32 item_index,
    uint32 *cur_frame_ptr
);


/***************************************************************************//*!
@brief ������һ֡
@author xiaoqing.lu
@param item_index [in] ������
@param next_frame_ptr [in/out] ��һ֡
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIconNextFrame (
    uint32 item_index,
    uint32 *next_frame_ptr
);


/***************************************************************************//*!
@brief ����icon�Ƿ���Ч
@author xiaoqing.lu
@param item_index [in] ������
@param is_valid [in] �Ƿ���Ч
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconValid (
    uint32 item_index,
    BOOLEAN is_valid
);


/***************************************************************************//*!
@brief �ж�icon�Ƿ���Ч
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemIconValid (
    uint32 item_index
);


/***************************************************************************//*!
@brief ������ʾ���ͣ��Ƿ񶯻���
@author xiaoqing.lu
@param item_index [in] ������
@param icon_style icon [in] ��ʾ���ͣ��Ƿ񶯻���
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconStyle (
    uint32            item_index,
    GUISTBDATA_ICON_STYLE_E icon_style
);


/***************************************************************************//*!
@brief �����ʾ����
@author xiaoqing.lu
@param item_index [in] ������
@param icon_style_ptr icon [in/out] ��ʾ���ͣ��Ƿ񶯻���
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemIconStyle (
    uint32            item_index,
    GUISTBDATA_ICON_STYLE_E *icon_style_ptr
);


/***************************************************************************//*!
@brief ����icon id
@author xiaoqing.lu
@param item_index [in] ������
@param icon_id [in] icon��ԴID
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemIconId (
    uint32         item_index,
    MMI_IMAGE_ID_T icon_id
);


/***************************************************************************//*!
@brief ��������
@author xiaoqing.lu
@param item_index [in] ������
@param text_info_ptr [in] ������Ϣ
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemText (
    uint32            item_index,
    const GUISTBDATA_TEXT_T *text_info_ptr
);


/***************************************************************************//*!
@brief ���������Ϣ
@author xiaoqing.lu
@param item_index [in] ������
@param text_info_ptr [in/out] ������Ϣ
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_GetItemText (
    uint32            item_index,
    GUISTBDATA_TEXT_T *text_info_ptr
);


/***************************************************************************//*!
@brief ����text�Ƿ���ʾ
@author xiaoqing.lu
@param item_index [in] ������
@param is_valid [in] valid or not
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemTextValid (
    uint32 item_index,
    BOOLEAN is_valid
);


/***************************************************************************//*!
@brief �ж�text "valid or not?"
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemTextValid (
    uint32 item_index
);


/***************************************************************************//*!
@brief ����item rect
@author xiaoqing.lu
@param item_index [in] ������
@param rect [in] ����
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemRect (
    uint32     item_index,
    GUI_RECT_T rect
);


/***************************************************************************//*!
@brief ���item����
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC GUI_RECT_T CTRLSTBDATA_GetItemRect (
    uint32     item_index
);


/***************************************************************************//*!
@brief ���total item
@author xiaoqing.lu
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC uint32 CTRLSTBDATA_GetItemTotalNum (
    void
);


/***************************************************************************//*!
@brief ���item����
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿ�
*******************************************************************************/
PUBLIC GUISTBDATA_ITEM_DATA_T *CTRLSTBDATA_GetItemData (
    uint32 item_index
);


/***************************************************************************//*!
@brief set item is displayed by right side
@author xiaoqing.lu
@param item_index [in] ������
@param is_right_align [in] �Ƿ�����ʾ
@retval true/false
@return true/false
@note �ڲ��ӿڣ�������ǿ�����ʾ����϶��ǿ�����ʾ��û�е��������
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemRightAlign (
    uint32   item_index,
    BOOLEAN  is_right_align
);


/***************************************************************************//*!
@brief set istyle item is displayed by which side
@author xiyuan.ma
@param item_index [in] ������
@param align_type [in] ѡ��item��iphone style�£��ǿ�����ʾ��������ʾ���ǿ�����ʾ
@retval true/false
@return true/false
@note �ڲ��ӿڣ�iphone����£�item��������ʾλ�ÿ���ѡ�񣬿��󡢿��ҡ�����
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_SetItemCenterAlign (
    uint32   item_index,
    BOOLEAN  is_center_align
);


/***************************************************************************//*!
@brief judge item is displayed by right side or not
@author xiaoqing.lu
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿڣ�������ǿ�����ʾ����϶��ǿ�����ʾ��û�е��������
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemRightAlign (
    uint32   item_index
);


/***************************************************************************//*!
@brief judge item is displayed by right side��left side or center
@authorxiyuan.ma
@param item_index [in] ������
@retval true/false
@return true/false
@note �ڲ��ӿڣ�iphone����item����
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTBDATA_IsItemCenterAlign (
    uint32   item_index
);


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTABATA_SetItemBgInter (
    uint32            item_index,
    GUI_BG_T          *bg_info_ptr
);


/*****************************************************************************/
//  Description : set STATUSBAR owner draw callback function
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTABATA_SetOwnerDraw (
    uint32                          item_index,
    GUISTATUSBAR_OWNER_DRAW_FUNC    func
);


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _CTRL_STATUSBAR_DATA_H_


/*@}*/
