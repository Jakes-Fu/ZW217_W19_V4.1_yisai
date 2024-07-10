/*! @file guiwin.h
@brief   ���ļ�������gui win����ӿںͽṹ��
@author  liqing.peng
@version  1.0
@date    01/05/2009 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note    01/05/2009     liqing.peng		 Create
*******************************************************************************/

/*! @page page_Win Win

(����)

*******************************************************************************/

/*! @addtogroup winGrp Win
@brief   ���ļ�������gui win����ӿںͽṹ��
@details ���ļ��Դ��ڿؼ�Ԫ�ؽ��з�װ(��״̬����softkey��title)
@{
*******************************************************************************/

#ifndef _GUI_WIN_H_
#define _GUI_WIN_H_ 

/*--------------------------------------------------------------------------*/
/*                         Include Files                                    */
/*--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "guilcd.h"
#include "guictrl.h"
#include "guistatusbar.h"
#include "guistatusbar_data.h"
#include "guititle.h"
#include "guisoftkey.h"
#ifdef GUIF_SCROLLKEY
#include "guiscrollkey.h"
#endif
#include "ctrltitle_export.h"
/*--------------------------------------------------------------------------*/
/*                         Compiler Flag                                    */
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                 */
/*--------------------------------------------------------------------------*/
        
/*--------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                  */
/*--------------------------------------------------------------------------*/

/*! @typedef GUIWIN_STBDATA_APP_HANDLE_FUNC
@brief   Item ����Ļص����������ͨ���˽ӿڴ򿪶�Ӧģ��ӿڵ�
@note    �紥�ʵ��item�ǿ���ͨ���˽ӿ�ָ����Ӧ�ĺ������Ե��ýӿ�
*/
typedef BOOLEAN(*GUIWIN_STBDATA_APP_HANDLE_FUNC)(void);

                                                  
/*! @enum  GUIWIN_STBDATA_ICON_STYLE_E 
@brief  item show style
@note   ״̬����ʾ���͵�ö��
*/
typedef enum
{
  GUIWIN_STBDATA_ICON_NONE = GUISTBDATA_ICON_NONE,     /*!<��icon��ʾ*/
  GUIWIN_STBDATA_ICON_NORMAL = GUISTBDATA_ICON_NORMAL, /*!<icon��ʾΪһ��ͼƬ*/
  GUIWIN_STBDATA_ICON_ANIM = GUISTBDATA_ICON_ANIM,     /*!<icon��ʾΪ����*/
  GUIWIN_STBDATA_ICON_WRE = GUISTBDATA_ICON_WRE,        /*!< wre��̨APͼ��,���ļ��м���ͼƬ*/ //add for wre_background by heng.xiao
  GUIWIN_STBDATA_ICON_MAX = GUISTBDATA_ICON_MAX        
}GUIWIN_STBDATA_ICON_STYLE_E;

/*! @struct GUIWIN_STBDATA_TEXT_T
@brief  �ı���Ϣ
@note   ״̬���ı���ʾ�������ݽṹ��״̬���ȿ�����ʾͼƬҲ������ʾ�ı�,
        ���߶���ʾ,������Ϊ��Ч��ʱ�򣬲���ʾ�ı�
*/
typedef struct
{
    BOOLEAN         is_display;   /*!<�Ƿ���ʾ*/                         
    GUI_FONT_T      font_type;    /*!<������*/
    GUI_COLOR_T     font_color;   /*!<������ɫ*/
    GUI_ALIGN_E     align;        /*!<���з�ʽ*/
    wchar           wstr_ptr[MMISTATUSBAR_ITEM_TEXT_LEN]; /*!<�ַ�������*/
    uint32          wstr_len;     /*!<�ַ�������*/
} GUIWIN_STBDATA_TEXT_T;

/*! @struct GUIWIN_STBDATA_ICON_T
@brief ͼ����Ϣ
@note  ״̬����icon״̬���ݽṹ
*/
typedef struct
{
    BOOLEAN                     is_display; /*!<�Ƿ���ʾ*/
    uint32                      icon_num;   /*!<icon����Ŀ*/
    uint32                      icon_array[MMISTATUSBAR_ITEM_ICON_FRAME];/*!<�ַ�������*/
    GUIWIN_STBDATA_ICON_STYLE_E icon_type;  /*!<icon����*/
} GUIWIN_STBDATA_ICON_T;

/*! @struct GUISTATUSBAR_OWNER_DRAW_T
@brief �ؼ��Ի�ص�����������Ϣ
*/
typedef struct 
{
    GUI_RECT_T                   display_rect;   /*!<��ʾ����*/
    MMI_HANDLE_T                 ctrl_handle;    /*!<�ؼ�handle*/
    GUI_LCD_DEV_INFO             lcd_dev;        /*!<lcd��Ϣ*/
} GUIWIN_STADATA_OWNER_DRAW_T;

/*! @typedef GUISTATUSBAR_OWNER_DRAW_FUNC
@brief �ؼ��Ի�ص�����,Я������
*/
typedef void (*GUIWIN_STBDATA_OWNER_DRAW_FUNC)(
                                               GUIWIN_STADATA_OWNER_DRAW_T *owner_draw_ptr
                                               );

/*! @struct GUIWIN_STBDATA_ITEM_DATA_T
@brief item ���ݽṹ
@note  ״̬����item��״̬���ݽṹ��item�������ı���ͼƬ
*/
typedef struct
{
    uint32                          cur_frame;         /*!<��ǰ����*/
    GUIWIN_STBDATA_TEXT_T           text_info;         /*!<�ַ���Ϣ*/
    GUIWIN_STBDATA_ICON_T           icon_info;         /*!<icon��Ϣ*/
	GUI_BG_T                        bg_info;           /*!<������Ϣ*/
    GUIWIN_STBDATA_APP_HANDLE_FUNC  app_handle_func;   /*!<Ӧ�ûص�����*/
    BOOLEAN 					    is_handle_callback;/*!<�Ƿ���ûص�*/
	GUIWIN_STBDATA_OWNER_DRAW_FUNC  owner_draw_func;   /*!< owner draw function*/
}GUIWIN_STBDATA_ITEM_DATA_T;

/*--------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                              */
/*--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ��̬����softkey�ؼ�
@author xiaoqing.lu
@param    win_handle    [in]   ���ھ��
@param    left_text_id  [in]   ��������ı�id
@param    mid_text_id   [in]   ��������ı�id
@param    right_text_id [in]   ��������ı�id
@retval TRUE  ���óɹ�
@retval FALSE ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��guisoftkey.c�е�GUISOFTKEY_CreateCtrlDyn���������ڶ�̬����
        softkey�����ú�text_id 
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_CreateSoftkeyDyn(
                                       MMI_HANDLE_T  win_handle,
                                       MMI_TEXT_ID_T left_text_id,
                                       MMI_TEXT_ID_T mid_text_id,
                                       MMI_TEXT_ID_T right_text_id
                                       );


/***************************************************************************//*!
@brief ����softkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
@author Great.Tian
@param      win_handle        [in] ���ھ��
@param      str_info          [in] Ҫ���õ��ı���Ϣ
@param      button_num        [in] Ҫ���õ�buttom������
@param      is_need_update    [in] �Ƿ�����ˢ��(��ΪTRUE����������оֲ�ˢ�£�ΪFALSE
                                   �����յ�ˢ����Ϣʱ�Ž���ˢ��
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��guisoftkey.c�е�GUISOFTKEY_SetButtonTextPtr����������ʱ������
        ����softkey�ϵ��ı�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyButtonTextPtr(
                                              MMI_HANDLE_T     win_handle,
                                              MMI_STRING_T     str_info,   
                                              GUISOFTKEY_INDEX button_num,
                                              BOOLEAN          is_need_update       // whether update
                                              );


/***************************************************************************//*!
@brief get softkey text id
@author Jassmine
@param      win_handle        [in] ���ھ��
@param      left_id_ptr       [in/out] �������text idָ��(����Ϊ��)
@param      middle_id_ptr   [in/out] �������text idָ��(����Ϊ��)
@param      right_id_ptr      [in/out] �������text idָ��(����Ϊ��)
@retval TRUE    ��ȡ�ɹ�
@retval FALSE   ��ȡʧ��
@return �Ƿ��ȡ�ɹ�
@note   �˺�����װ��guisoftkey.c�е�GUISOFTKEY_GetTextId�����ڻ�ȡsoftkey�ϵ��ı�
        id
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetSoftkeyTextId(
                                       MMI_HANDLE_T    win_handle,    // ���ھ��
                                       MMI_TEXT_ID_T   *left_id_ptr,   //in:may PNULL
                                       MMI_TEXT_ID_T   *middle_id_ptr, //in:may PNULL
                                       MMI_TEXT_ID_T   *right_id_ptr   //in:may PNULL
                                       );


/***************************************************************************//*!
@brief ����softkey button�ϵ�IconͼƬID,����֪�ؼ��Ƿ�����ˢ��
@author Liqing.Peng
@param      win_handle         [in] ���ھ��
@param      icon_id            [in] iconͼƬ��Դid
@param      button_num         [in] Ҫ���õ��������
@param      is_need_update     [in] �Ƿ�����ˢ��
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��guisoftkey.c�е�GUISOFTKEY_SetButtonIconId�����������������
        ��iconͼƬ���䷽����������������ı�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SeSoftkeytButtonIconId(
                                             MMI_HANDLE_T    win_handle,    // ���ھ��
                                             MMI_IMAGE_ID_T  icon_id,
                                             GUISOFTKEY_INDEX button_num,
                                             BOOLEAN         is_need_update       // whether update
                                             );

#ifdef GUIF_SCROLLKEY
PUBLIC BOOLEAN GUIWIN_SetScrollkeytButtonIconId(
                                             MMI_HANDLE_T        win_handle,    // ���ھ��
                                             MMI_IMAGE_ID_T      icon_id,
                                             MMI_TEXT_ID_T       text_id,
                                             GUISCROLLSKIN_INDEX skin_idx,
                                             GUISCROLLKEY_INDEX  button_num,
                                             BOOLEAN             is_need_update       // whether update
                                             );
#endif

/***************************************************************************//*!
@brief ����softkey button���ı�ID,����֪�ؼ��Ƿ�����ˢ��
@author Great.Tian
@param      win_handle       [in] ���ھ��
@param      text_id          [in] �ı���Դid
@param      button_num       [in] Ҫ���õ��������
@param      is_need_update   [in] �Ƿ�����ˢ��
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��guisoftkey.c�е�GUISOFTKEY_SetButtonTextId����������������ı�
        ��GUIWIN_SetSoftkeyButtonTextPtr���������ڣ�GUIWIN_SetSoftkeyButtonTextPtr
        ������ı�������MMI_STRING_T�����˺�����������ı���Դ��id
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SeSoftkeytButtonTextId(
                                             MMI_HANDLE_T    win_handle,    // ���ھ��
                                             MMI_IMAGE_ID_T  text_id,
                                             GUISOFTKEY_INDEX button_num,
                                             BOOLEAN         is_need_update       // whether update
                                             );


/***************************************************************************//*!
@brief  ����softkey �ı�ID,����֪�ؼ��Ƿ�����ˢ��
@author Liqing.Peng
@param      win_handle     [in] ���ھ��
@param      leftsoft_id    [in]   �������text id
@param      middlesoft_id  [in]   �������text id
@param      rightsoft_id   [in]   �������text_id
@param      is_need_update [in]   �Ƿ�����ˢ��
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��guisoftkey.c �е�GUISOFTKEY_SetTextId�������������С������
        ���ı���ͨ����ȡ�ı���Դid������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyTextId(
                                       MMI_HANDLE_T    win_handle,    // ���ھ��
                                       MMI_TEXT_ID_T   leftsoft_id,   // ������ı�id
                                       MMI_TEXT_ID_T   middlesoft_id, // ������ı�id
                                       MMI_TEXT_ID_T   rightsoft_id,  // t������ı�id
                                       BOOLEAN         is_need_update // �Ƿ�����ˢ��
                                       );


/***************************************************************************//*!
@brief ����ˢ��Softkey�ؼ�
@author Liqing.Peng
@param    win_handle   [in]   ���ھ��
@retval TRUE    ˢ�³ɹ�
@retval FALSE   ˢ��ʧ��
@return �Ƿ�ˢ�³ɹ�
@note   �˺�����װ��guisoftkey.c �е�GUISOFTKEY_Update����������Ҫʱˢ��softkey��
        ���ڶ�sotkey���Խ���һϵ�г�ʼ�������ˢ����ʾ����
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_UpdateSoftkey(
                                    MMI_HANDLE_T    win_handle
                                    );


/***************************************************************************//*!
@brief ����Softkey��Style
@author liqing.peng
@param      win_handle [in]   ���ھ��
@param      font_color [in]   ������ɫ
@param      bg_type    [in]   �������
@param      bg_color   [in]   ������ɫ
@param      bg_img     [in]   ����ͼƬid
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��guisoftley.c �е�GUISOFTKEY_SetStyle����������softkey�ķ��
        ��Ҫ�б����������������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyStyle(
                                      MMI_HANDLE_T      win_handle,
                                      GUI_COLOR_T	    font_color,
                                      uint32		    bg_type,
                                      GUI_COLOR_T	    bg_color,
                                      MMI_IMAGE_ID_T    bg_img
                                      );


//++++CR242519
/***************************************************************************//*!
@brief ����Softkey pressʱ��Style
@author liqing.peng
@param      win_handle [in]   ���ھ��
@param      font_color [in]   ������ɫ
@param      bg_type    [in]   �������
@param      bg_color   [in]   ������ɫ
@param      bg_img     [in]   ����ͼƬid
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��guisoftley.c �е�GUISOFTKEY_SetStyle����������softkey�ķ��
        ��Ҫ�б����������������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyPressedStyle(
                                      MMI_HANDLE_T      win_handle,
                                      GUI_COLOR_T	    font_color,
                                      uint32		    bg_type,
                                      GUI_COLOR_T	    bg_color,
                                      MMI_IMAGE_ID_T    bg_img
                                      );

//++++CR242519

/***************************************************************************//*!
@brief ����Softkey��bg Style
@author liqing.peng
@param      win_handle [in]   ���ھ��
@param      bg_type    [in]   �������(0���ʾ����ΪNULL��1��ʾΪͼƬ��2��ʾֻ��
                              ��ɫ)
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��guisoftley.c �е�GUISOFTKEY_SetBgType����������softkey�ı���
        ���
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyBgStyle(
                                        MMI_HANDLE_T          win_handle,
                                        GUI_SOFTKEY_BG_TYPE_T bg_type
                                        );


/***************************************************************************//*!
@brief ����softkey common�ı���
@author xiaoqing.lu
@param      win_handle [in]   ���ھ��
@param      bg_ptr     [in]   �ؼ����еı������ݽṹ
@retval none
@return none
@note   �˺�����װ��guictrl_api.c �е�GUIAPICTRL_SetBg����ӵ���softkey�ؼ�����
        ����ϵ����ñ�������������softkey����
*******************************************************************************/
PUBLIC void GUIWIN_SetSoftkeyCommonBg(
                                      MMI_HANDLE_T  win_handle,
                                      GUI_BG_T      *bg_ptr
                                      );


/***************************************************************************//*!
@brief ����softkey��״̬
@author Lianxiang
@param      win_handle    [in]  ���ھ��
@param      button_index  [in]  Ҫ���õ�button����
@param      is_gray       [in]  �Ƿ�һ�
@param      is_long_press [in]  �Ƿ񳤰�
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note   ���������Ҫ����ĳ��Softkey��Tp long��Ϣ,��ôis_long_press����ΪTrue
*******************************************************************************/
PUBLIC BOOLEAN  GUIWIN_SetSoftkeyBtnState(
                                          MMI_HANDLE_T	 win_handle,
                                          uint16         button_index,
                                          BOOLEAN        is_gray,
                                          BOOLEAN		 is_long_press
                                          );

/***************************************************************************//*!
@brief ����softkey��״̬
@author Lianxiang
@param      win_handle    [in]  ���ھ��
@param      button_index  [in]  Ҫ���õ�button����
@param      is_gray       [in]  �Ƿ�һ�
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note
*******************************************************************************/
PUBLIC BOOLEAN  GUIWIN_SetSoftkeyBtnGray(
                                          MMI_HANDLE_T	 win_handle,
                                          uint16         button_index,
                                          BOOLEAN        is_gray
                                          );


/***************************************************************************//*!
@brief �ж�����Ƿ���Դ���tp��Ϣ
@author Liqing.Peng
@param      win_handle  [in]  ���ھ��
@retval TRUE    ������Ӧtp��Ϣ
@retval FALSE   ������Ӧtp��Ϣ
@return �Ƿ���Դ���tp��Ϣ
@note   �˺�����װ��guisoftkey�е�GUISOFTKEY_IsTPEnable�������ж�����Ƿ���Դ���
        tp��Ϣ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsSoftkeyTPEnable(
                                        MMI_HANDLE_T   win_handle
                                        );


/***************************************************************************//*!
@brief ����Softkey�Ƿ���Ӧ������Ϣ
@author Liqing.Peng
@param      win_handle    [in]  ���ھ��
@param      sk_tp_enable  [in]  �Ƿ������Ӧtp��Ϣ��TRUE���ԣ�FALSE������
@retval TRUE    ������Ӧtp��Ϣ
@retval FALSE   ������Ӧtp��Ϣ
@return �Ƿ������Ӧtp��Ϣ
@note   �˺�����װ��guisoftkey�е�GUISOFTKEY_SetTPEnable�����Զ�softkey�Ƿ�����Ӧ
        tp��Ϣ��������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyTPEnable(
                                         MMI_HANDLE_T   win_handle,
                                         BOOLEAN        sk_tp_enable
                                         );


/***************************************************************************//*!
@brief �����������ɫ
@author xiaoqing.lu
@param       win_handle [in]   ���ھ��
@param       color_ptr  [in]  ��ɫֵ(uint16)
@param       font_ptr   [in]   ��������
@retval TRUE     ���óɹ�
@retval FALSE    ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��GUISOFTKEY_Setfont����������softkey�ϵ��ı��������ɫ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftKeyfont(
                                     MMI_HANDLE_T    win_handle,
                                     GUI_COLOR_T     *color_ptr,
                                     GUI_FONT_T      *font_ptr
                                     );

// status bar
/***************************************************************************//*!
@brief  ��ʼ��item���ݺ�״̬��
@author Lianxiang
@param      item_max_num [in]   ���item��
@retval TRUE    ��ʼ���ɹ�
@retval FALSE   ��ʼ��ʧ��
@return �Ƿ��ʼ���ɹ�
@note   �˺�����װ��GUISTBDATA_InitData����ʼ��״̬��������,����ϵͳ��״̬����
        ������ͬ,�����ڿ���ʱ��ʼ��
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_InitStbData(
                                  uint32 item_max_num
                                  );


/***************************************************************************//*!
@brief ����item
@author Xiaoqing.Lu
@param       item_data_ptr  [in]  item������Ϣ
@retval TRUE     ��ӳɹ�
@retval FALSE    ���ʧ��
@return �Ƿ���ӳɹ�
@note   �˺�����װ��GUISTBDATA_AppendItemData�����Item������Ϣ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_AppendStbItemData(
                                        GUIWIN_STBDATA_ITEM_DATA_T *item_data_ptr
                                        );


/***************************************************************************//*!
@brief ����item�Ƿ�ɼ�
@author xiaoqing.lu
@param       item_index [in]   item����
@param       is_visible [in]   �Ƿ�ɼ�
@retval TRUE     ���óɹ�
@retval FALSE    ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��GUISTBDATA_SetItemVisible������item�Ƿ�ɼ�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemVisible(
                                        uint32   item_index,
                                        BOOLEAN  is_visible
                                        );


/***************************************************************************//*!
@brief �ж�item�Ƿ�ɼ�
@author xiaoqing.lu
@param       item_index [in]   Ҫ�жϵ�item����
@retval TRUE     �ɼ�
@retval FALSE    ���ɼ�
@return �Ƿ�ɼ�
@note �˺�����װ��GUISTBDATA_IsItemVisible�� �ж�item�Ƿ�ɼ�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemVisible(
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
PUBLIC BOOLEAN GUIWIN_SetStbItemValid(
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
PUBLIC BOOLEAN GUIWIN_IsStbItemValid(
                                       uint32   item_index
                                       );


/***************************************************************************//*!
@brief ����״̬��item�����Ӧ�ýӿ�ָ��
@author xiaoqing.lu
@param       item_index      [in]   item����
@param       app_handle_func [in]   ָ����Ӧ�ӿڵ�ָ��
@retval TRUE     ���óɹ�
@retval FALSE    ����ʧ��
@return �Ƿ����óɹ�
@note   �˺�����װ��GUISTBDATA_SetItemHandleFunc������״̬��Item�����Ӧ�ýӿ�ָ��,
        ���紥�ʵ����ʱ���ܹ�ִ�и�Ӧ��
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemHandleFunc(
                                           uint32   item_index,
                                           GUIWIN_STBDATA_APP_HANDLE_FUNC app_handle_func
                                           );


/***************************************************************************//*!
@brief ����item�Ƿ�����ʾ
@author xiaoqing.lu
@param       item_index     [in]   item����
@param       is_right_align [in]   �Ƿ��Ҷ���
@retval TRUE     ���óɹ�
@retval FALSE    ����ʧ��
@return �Ƿ����óɹ�
@note   ������ǿ�����ʾ����϶��ǿ�����ʾ��û�е��������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemRightAlign(
                                           uint32   item_index,
                                           BOOLEAN  is_right_align
                                           );

/***************************************************************************//*!
@brief ����item�Ƿ������ʾ
@author xiyuan.ma
@param       item_index     [in]   item����
@param       is_right_align [in]   �Ƿ��Ҷ���
@retval TRUE     ���óɹ�
@retval FALSE    ����ʧ��
@return �Ƿ����óɹ�
@note   �Ƿ������ʾ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemCenterAlign(
                                           uint32   item_index,
                                           BOOLEAN  is_center_align
                                           );
/***************************************************************************//*!
@brief �ж�item�Ƿ�����ʾ
@author xiaoqing.lu
@param       item_index [in]   item����
@retval TRUE     ������ʾ
@retval FALSE    ������ʾ
@return �Ƿ��Ҷ���
@note   �˺�����װ��GUISTBDATA_IsItemRightAlign�������ж�״̬��item�Ƿ��Ҷ���
        ������ǿ�����ʾ����϶��ǿ�����ʾ��û�е��������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemRightAlign(
                                          uint32 item_index
                                          );
/***************************************************************************//*!
@brief �ж�item�Ƿ������ʾ
@author xiaoqing.lu
@param       item_index [in]   item����
@retval TRUE     ������ʾ
@retval FALSE    ������ʾ
@return �Ƿ��Ҷ���
@note   �Ƿ������ʾ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemCenterAlign(
                                          uint32 item_index
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
PUBLIC BOOLEAN GUIWIN_SetStbItemDisable(
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
PUBLIC BOOLEAN GUIWIN_IsStbItemDisable(
                                       uint32   item_index
                                       );

/***************************************************************************//*!
@brief ����item��Ӧ��Ӧ�ýӿ��Ƿ���Ч
@author xiaoqing.lu
@param       item_index [in]   item����
@param       is_valid   [in]     �Ƿ���Ч TRUE��ʾitem����ýӿڴ���Ӧ��
@retval TRUE     ���óɹ�
@retval FALSE    ����ʧ��
@return �����Ƿ�ɹ�
@note   �˺�����װ��GUISTBDATA_SetItemHandleFuncValid�������Ƿ���Ӧ��,
        ���ΪFalse�򲻻ᴦ��
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetItemHandleFuncValid(
                                             uint32  item_index,
                                             BOOLEAN is_valid
                                             );


/***************************************************************************//*!
@brief �ж�item������Ľӿ��Ƿ���Ч
@author xiaoqing.lu
@param      item_index  [in]  item����
@retval TRUE    ����ӿ�
@retval FALSE   ������ӿ�
@return �Ƿ���ӿ�
@note   �˺�����װ��GUISTBDATA_SetItemHandleFuncValid����ȡӦ�ô���ӿڵ���Ч��
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_StbItemIsHandleFuncValid(
                                               uint32 item_index
                                               );


/***************************************************************************//*!
@brief ����Item�Ķ�Ӧ��ʾ��icon
@author xiaoqing.lu
@param      item_index    [in]   item����
@param      icon_info_ptr [in]   ״̬��icon״̬���ݽṹ
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��
@return �Ƿ����óɹ�
@note �˺�����װ��GUISTBDATA_SetItemIcon��ÿ��item�����ʾ
      MMISTATUSBAR_ITEM_ICON_FRAME��Icon,�ڿؼ�����������ʽ����
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIcon(
                                     uint32                      item_index,
                                     const GUIWIN_STBDATA_ICON_T *icon_info_ptr
                                     );

/*****************************************************************************/
//  Description : ����״̬������
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbVisible(
                                 MMI_HANDLE_T win_handle,
                                 BOOLEAN      is_visible
                                 );

/***************************************************************************//*!
@brief ��ȡ�ؼ���ĳһ���icon��Ϣ
@author xiaoqing.lu
@param      item_index    [in]     item����
@param      icon_info_ptr [in/out]   ״̬��icon״̬���ݽṹ
@retval TRUE    ��ȡ�ɹ�
@return FALSE   ��ȡʧ��
@note �˺�����װ��GUISTBDATA_GetItemIcon�����Ի�ȡitem�е�icon״̬
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemIcon(
                                     uint32                item_index,
                                     GUIWIN_STBDATA_ICON_T *icon_info_ptr
                                     );


/***************************************************************************//*!
@brief ����ͼƬIcon��ʾ����,����ʾΪ��̬ͼƬ,Ҳ����ʾΪ��̬ͼƬ
@author xiaoqing.lu
@param      item_index [in]   item����
@param      icon_style [in]   icon��ʾ���
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��  
@return �Ƿ����óɹ�
@note �˺�����װ��GUISTBDATA_SetItemIconStyle����̬ʱֻ��ʾ�����еĵ�һ��ͼƬ,
      ��̬ʱ��ʾ����������ͼƬ�Զ�����ʾ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconStyle(
                                          uint32                      item_index,
                                          GUIWIN_STBDATA_ICON_STYLE_E icon_style
                                          );


/***************************************************************************//*!
@brief ��ȡItem��ͼƬ��ʾ״̬,��̬��̬
@author xiaoqing.lu
@param       item_index    [in]    item����
@param      icon_style_ptr [in/out]   icon״̬ 
@retval TRUE    ��ȡ�ɹ�
@return FALSE   ��ȡʧ��
@return �Ƿ��ȡ�ɹ�
@note   �˺�����װ��GUISTBDATA_GetItemIconStyle�����Ի�ȡitem��icon״̬
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemIconStyle(
                                          uint32            item_index,
                                          GUIWIN_STBDATA_ICON_STYLE_E *icon_style_ptr
                                          );


/***************************************************************************//*!
@brief ����Item��IconͼƬ
@author xiaoqing.lu
@param     item_index  [in]    item����
@param      icon_id    [in]   icon��ͼƬid
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��  
@return �Ƿ����óɹ�
@note   �˺�����װ��GUISTBDATA_SetItemIconId���ýӿں�GUIWIN_SetStbItemIcon��
        ����������ֻ����item��icon����ĵ�һ��icon,���ؼ���ʾ��̬icon��ʱ����
        ʾ��һ��ͼƬ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconId(
                                       uint32         item_index,
                                       MMI_IMAGE_ID_T icon_id
                                       );


/***************************************************************************//*!
@brief ���ÿؼ�Item�����ĵ�ǰ��ʾ��ͼƬ
@author xiaoqing.lu
@param      item_index  [in]   item����
@param      cur_frame   [in]  ��ǰ��ʾ��ͼƬ
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��  
@return �Ƿ����óɹ�
@note   �˺�����װ��GUISTBDATA_SetItemIconCurFrame����̬��ʾicon��ʱ��,�ؼ���Ҫ
        ֪����ǰ��ʾ��icon�����±�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconCurFrame(
                                             uint32 item_index,
                                             uint32 cur_frame
                                             );


/***************************************************************************//*!
@brief ��ȡicon���鵱ǰ��ʾ�������±�
@author xiaoqing.lu
@param       item_index    [in]    item����
@param      cur_frame_ptr  [in/out]   ��ǰ��ʾ��ͼƬ
@retval TRUE    ��ȡ�ɹ�
@return FALSE   ��ȡʧ��
@return �Ƿ��ȡ�ɹ�
@note   �˺�����װ��GUISTBDATA_GetItemIconCurFrame�����Ի�ȡ��̬��ʾ��icon��ǰ
        ��ʾ��ͼƬ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemIconCurFrame(
                                             uint32 item_index,
                                             uint32 *cur_frame_ptr
                                             );


/***************************************************************************//*!
@brief ����Icon�Ƿ���Ч,������Ϊ��Ч��ʱ��,item��Ӧ��״̬����ʾͼƬ
@author xiaoqing.lu
@param      item_index [in]   item����
@param      is_valid   [in]   �Ƿ���Ч
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ��  
@return �Ƿ����óɹ�
@note   �˺�����װ��GUISTBDATA_SetItemIconValid״̬���ȿ�����ʾͼƬҲ������ʾ��
        ��,���߶���ʾ,������Ϊ��Ч��ʱ��,����ʾͼƬ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemIconValid(
                                          uint32 item_index,
                                          BOOLEAN is_valid
                                          );


/***************************************************************************//*!
@brief �ж�item��icon�Ƿ���Ч
@author xiaoqing.lu
@param      item_index [in]   item����
@retval TRUE    ��Ч
@retval FALSE   ��Ч
@return �Ƿ���Ч
@note   �˺�����װ��GUISTBDATA_IsItemIconValid���ж�icon�Ƿ���Ч
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_StbIsItemIconValid(
                                         uint32 item_index
                                         );


/***************************************************************************//*!
@brief  ����item��ʾ���ַ���
@author xiaoqing.lu
@param      item_index    [in]   item����
@param      text_info_ptr [in]   ״̬���ı���ʾ����
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUISTBDATA_SetItemText��״̬���ȿ�����ʾͼƬҲ������ʾ�ı�,
        ���߶���ʾ,������Ϊ��Ч��ʱ��,����ʾ�ı�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemText(
                                     uint32            item_index,
                                     const GUIWIN_STBDATA_TEXT_T *text_info_ptr
                                     );


/***************************************************************************//*!
@brief ��ȡitem���ַ���
@author xiaoqing.lu
@param       item_index    [in]   item����
@param      text_info_ptr  [in/out]   ��ȡ��item�ַ���
@retval TRUE    ��ȡ�ɹ�
@return FALSE   ��ȡʧ��
@return �Ƿ��ȡ�ɹ�
@note   �˺�����װ��GUISTBDATA_GetItemText�����Ի�ȡitem���ַ���
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbItemText(
                                     uint32            item_index,
                                     GUIWIN_STBDATA_TEXT_T *text_info_ptr
                                     );


/***************************************************************************//*!
@brief ����item���ı��Ƿ���Ч
@author xiaoqing.lu
@param      item_index [in]   item����
@param      is_valid   [in]   �ı��Ƿ���Ч
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUISTBDATA_SetItemTextValid������item���ı��Ƿ���Ч
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemTextValid(
                                          uint32 item_index,
                                          BOOLEAN is_valid
                                          );


/***************************************************************************//*!
@brief �ж��ı��Ƿ���Ч
@author xiaoqing.lu
@param      item_index [in]   item����
@retval TRUE    ��Ч
@retval FALSE   ��Ч
@return �Ƿ���Ч
@note   �˺�����װ��GUISTBDATA_IsItemTextValid���ж��ı��Ƿ���Ч
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbItemTextValid(
                                         uint32 item_index
                                         );


/***************************************************************************//*!
@brief ����Item��rect
@author xiaoqing.lu
@param      item_index [in]   item����
@param      rect       [in]   ���õ�����
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUISTBDATA_SetItemRect����������item�ڴ����е�����
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemRect(
                                     uint32     item_index,
                                     GUI_RECT_T rect
                                     );


/***************************************************************************//*!
@brief ���item��rect
@author xiaoqing.lu
@param      item_index [in]   item����
@retval TRUE    ��ȡ�ɹ�
@return FALSE   ��ȡʧ��
@return �Ƿ��ȡ�ɹ�
@note   �˺�����װ��GUISTBDATA_GetItemRect�����ڻ�ȡitem������
*******************************************************************************/
PUBLIC GUI_RECT_T GUIWIN_GetStbItemRect(
                                        uint32 item_index
                                        );


/***************************************************************************//*!
@brief ��ȡitem������
@author xiaoqing.lu
@retval item����
@return item����
@note   �˺�����װ��GUISTBDATA_GetItemTotalNum�����ڻ�ȡ״̬��item����
*******************************************************************************/
PUBLIC uint32 GUIWIN_GetStbItemTotalNum(void);


/***************************************************************************//*!
@brief ����״̬���Ƿ���ʾ����
@author xiaoqing.lu
@param      win_handle     [in]  ���ھ��
@param      is_desplay_bg  [in]   �����Ƿ���ʾ����(TRUE��ʾ)
@retval void
@return void
@note   �˺�����װ��GUISTATUSBAR_SetBgIsDesplay��ʱ��״̬����ֱ����ʾ��lcd��,
        ������Ҫ��ʾ�Լ��ı���,����idle�������˵�����
*******************************************************************************/
PUBLIC void GUIWIN_SetStbBgIsDesplay(
                                     MMI_HANDLE_T win_handle,
                                     BOOLEAN is_desplay_bg
                                     );


/***************************************************************************//*!
@brief          ����״̬���Ƿ�����û�����
@author       xiyuan.ma
@param       win_handle              [in]  ���ھ��
@param       clear_customer_bg   [in]   �����Ƿ�����û�����(TRUE ���)
@retval void
@return void
@note
*******************************************************************************/
PUBLIC void GUIWIN_IsClearCustomerBg(
                                     MMI_HANDLE_T win_handle,
                                     BOOLEAN clear_customer_bg
                                     );

/***************************************************************************//*!
@brief ����״̬����״̬
@author xiaoqing.lu
@param      win_handle [in]   ���ھ��
@param      ctrl_state [in]   �ؼ�״ֵ̬(uint32)
@param      is_set     [in]      ״̬�Ƿ񼤻�(TRUE�򼤻����õ�״̬)
@retval void
@return void
@note   �˺�����װ��GUISTATUSBAR_SetState����״̬������״̬���ã�����һ��״ֵ̬
        ctrl_state�󣬻���Ҫ�жϸ�״̬�Ƿ񼤻�(is_set)
*******************************************************************************/
PUBLIC void GUIWIN_SetStbState(
                               MMI_HANDLE_T win_handle,
                               uint32 ctrl_state,
                               BOOLEAN is_set
                               );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetStbState(
                                  MMI_HANDLE_T win_handle,
                                  uint32 ctrl_state
                                  );

/***************************************************************************//*!
@brief ���ÿؼ��ı���ͼƬID
@author xiaoqing.lu
@param      win_handle [in]   ���ھ��
@param      image_id   [in]   ͼƬ��Դid
@retval void
@return void 
@note   �˺�����װ��GUISTATUSBAR_SetBgImageId���������ñ���ͼƬ
*******************************************************************************/
PUBLIC void GUIWIN_SetStbBgImageId(
                                   MMI_HANDLE_T win_handle,
                                   MMI_IMAGE_ID_T image_id
                                   );


/***************************************************************************//*!
@brief  ���ÿؼ��ı�����ɫ
@author xiaoqing.lu
@param      win_handle [in]   ���ھ��
@param      bg_info    [in]   �����������ݽṹ(�б������ͣ���״��ͼƬid����ɫ)
@retval void
@return void
@note   �˺�����װ��GUISTATUSBAR_SetBg����������״̬���ı�����������
*******************************************************************************/
PUBLIC void GUIWIN_SetStbBg(
                            MMI_HANDLE_T win_handle,
                            GUI_BG_T  bg_info
                            );


/***************************************************************************//*!
@brief  ֻˢ��icon����ˢϴ����
@author xiaoqing.lu
@retval MMI_RESULT_FALSE    ˢ��ʧ��
@retval MMI_RESULT_TRUE     ˢ�³ɹ�
@return ˢ�½���Ƿ�ɹ�
@note   �˺�����װ��GUISTATUSBAR_UpdateCtrlֻˢ�¿ؼ���icon,��ˢ�¿ؼ�����,���ؼ�
        ������Ч��ʱ�����Ӧ���Լ�ˢ�±�����ʱ����øýӿ�ˢ�¿ؼ�,�ýӿڲ������
        �ڷ���Fullpaint��Ϣ
*******************************************************************************/
PUBLIC MMI_RESULT_E GUIWIN_UpdateStbIcon(void);


/***************************************************************************//*!
@brief ����״̬���ĺ�������ʾ����
@author hua.fang
@param      win_handle    [in]   ���ھ��
@param      both_rect_ptr [in]   ���������Ե�����
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   ���ú�������ʾ������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbBothRect(
                                     MMI_HANDLE_T      win_handle,
                                     GUI_BOTH_RECT_T   *both_rect_ptr
                                     );


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
                                         );

/***************************************************************************//*!
@brief  ˢ��״̬��
@author xiaoqing.lu
@retval MMI_RESULT_FALSE    ˢ��ʧ��
@retval MMI_RESULT_TRUE     ˢ�³ɹ�
@return ˢ�½���Ƿ�ɹ�
@note   ˢ��״̬��������item,��������Ч��ʱ��ᷢ��fullpaint������,�����ô���
        ˢ�±���
*******************************************************************************/
PUBLIC MMI_RESULT_E GUIWIN_UpdateStb(void);


/***************************************************************************//*!
@brief  ��̬����״̬���ؼ�
@author xiaoqing.lu
@param       win_handle [in]   ���ھ��
@retval TRUE     �����ɹ�
@retval FALSE    ����ʧ�� 
@return �Ƿ񴴽��ɹ�
@note   �˺�����װ��GUISTATUSBAR_CreateCtrlDyn�����ڶ�̬����״̬���ؼ�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_CreateStbDyn(
                                   MMI_HANDLE_T win_handle
                                   );


/***************************************************************************//*!
@brief  ��̬����״̬���ؼ����û������Զ���ؼ�������
@author xiaoqing.lu
@param      win_handle [in]   ���ھ��
@param      rect_ptr   [in]   �û��Լ����õ���ʾ����
@retval TRUE     �����ɹ�
@retval FALSE    ����ʧ�� 
@return �Ƿ񴴽��ɹ�
@note   �˺�����װ��GUISTATUSBAR_CustomCreate����GUIWIN_CreateStbDyn���������ڣ�
        �˺��������Լ�����״̬��������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_CustomStbCreate(
                                      MMI_HANDLE_T win_handle,
                                      GUI_BOTH_RECT_T *rect_ptr
                                      );


/***************************************************************************//*!
@brief  ����STATUSBAR�ؼ�item�Ի�ص�����
@author xiyuan.ma
@param  item_index     [in] item index
@param  func           [in] �Ի�ص�����
@return 
@note 
*******************************************************************************/
PUBLIC void GUIWIN_SetStbOwnerDraw(
                                   uint32                          item_index,
                                   GUIWIN_STBDATA_OWNER_DRAW_FUNC  func
                                   );
								   
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
                                       );

/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbDropDown(
                                    MMI_HANDLE_T win_handle
                                    );

/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC void GUIWIN_SetStbPressed(
                                 MMI_HANDLE_T win_handle,
                                 BOOLEAN is_true
                                 );

/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_IsStbPressed(
                                   MMI_HANDLE_T win_handle
                                   );
#endif

/***************************************************************************//*!
@brief ���ô���title�ַ���
@author Liqing.peng
@param      win_handle     [in]   ���ھ��
@param      wstr_ptr       [in]   �ַ���ָ��
@param      wstr_len       [in]   �ַ�������(<=16)
@param      is_need_update [in]   �Ƿ�����ˢ��(TRUE�������ú�����ˢ��title��)
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUITITLE_SetText��ͨ��������ַ����ͳ�������title���ı�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleText(
                                   MMI_HANDLE_T		win_handle,
                                   const wchar		*wstr_ptr, //[IN] �ַ���ָ��
                                   uint16			wstr_len, //[IN] �ַ�������, <=16
                                   BOOLEAN			is_need_update //[IN] �Ƿ�����ˢ��
                                   );


/***************************************************************************//*!
@brief ���ô���title�ַ����������ַ�����Դid
@author Liqing.peng
@param      win_handle      [in]  ���ھ��
@param      text_id         [in]   �ַ�����Դid
@param      is_need_update  [in]  �Ƿ�����ˢ��(TRUE�������ú�����ˢ��title��)
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUITITLE_SetTextId��ͨ��������ַ�����Դid������title�ı�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleTextId(
                                     MMI_HANDLE_T	 win_handle,
                                     MMI_TEXT_ID_T   text_id, //[IN] �ַ�����Դid
                                     BOOLEAN         is_need_update //[IN] �Ƿ�����ˢ��
                                     );


/***************************************************************************//*!
@brief ���ô���title�Ű�
@author Liqing.peng
@param      win_handle [in]   ���ھ��
@param      align      [in]   �ַ����Ű淽ʽ
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUITITLE_SetTextAlignType�����ñ�����ı��Ű淽ʽ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleTextAlign(
                                        MMI_HANDLE_T		win_handle,
                                        GUI_ALIGN_E	        align
                                        );


/***************************************************************************//*!
@brief  ���ñ���ؼ��Ĺ�����ʽ
@author Liqing.peng
@param      win_handle  [in]   ���ھ��
@param      scroll_type [in]   ������ʽ
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUITITLE_SetTextScrollType�����ñ���ؼ��Ĺ�����ʽ,Ĭ��
        �������ҹ���,��ο�GUITITLE_SCROLL_TYPE_E����
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleTextScroll(
                                         MMI_HANDLE_T			win_handle,
                                         GUITITLE_SCROLL_TYPE_E	scroll_type
                                         );


/***************************************************************************//*!
@brief ����title���Ƿ���arrow
@author Liqing.peng
@param      win_handle          [in]   ���ھ��
@param      is_need_title_arrow [in]   �Ƿ���Ҫtitle�ϵ�arrowͼ��    
@retval TRUE    ���óɹ���title��Ҫ��ʾarrow
@retval FALSE   ����ʧ�ܣ�title����Ҫ��ʾarrow
@return title�Ƿ���Ҫarrow
@note   �˺�����װ��GUITITLE_SetRollArrow�����ñ���ؼ��ϵ�����ͼ���Ƿ���Ҫ��ʾ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleRollArrow(    
                                        MMI_HANDLE_T   win_handle,
                                        BOOLEAN        is_need_title_arrow  //�Ƿ���Ҫtitle�ϵ�arrowͼ��
                                        );


/***************************************************************************//*!
@brief  ����title��arrow��״̬
@author Liqing.peng
@param      win_handle [in]   ���ھ��
@param      index      [in]   title arrow������(0��ʾ��arrow��1��ʾ��arrow)
@param      status     [in]   ���õ�arrow״̬
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUITITLE_SetRollArrowStatus�����ñ���ؼ�������ͼ���״̬
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleRollArrowStatus(    
                                              MMI_HANDLE_T               win_handle,
                                              GUITITLE_BUTTON_INDEX_T   index,
                                              GUITITLE_BUTTON_STATE_E  status
                                              );


/***************************************************************************//*!
@brief ���title��arrow״̬
@author Liqing.peng
@param      win_handle [in]   ���ھ��
@param      index      [in]   title arrow����(0��ʾ��arrow��1��ʾ��arrow)
@retval TITLE_BUTTON_NORMAL    ����������������Ӧtp���ʵ��
@retval TITLE_BUTTON_INVISIBLE ���ɼ�
@retval TITLE_BUTTON_GRAYED    �һ���������Ӧtp���ʵ��
@return title button��״̬
@note   �˺�����װ��GUITITLE_GetRollArrowStatus����ñ���ͼ���״̬
*******************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E GUIWIN_GetTitleRollArrowStatus(    
                                                             MMI_HANDLE_T               win_handle,
                                                             GUITITLE_BUTTON_INDEX_T   index
                                                             );


/*****************************************************************************/
// 	Description : ����title���Ƿ���button
//	Global resource dependence : 
//  Author: xiaoqinglu
//	Note: ���ñ���ؼ��ϵ�����ͼ���Ƿ���Ҫ��ʾ
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleNeedButton(    
                                         MMI_HANDLE_T   win_handle,
                                         BOOLEAN        is_need_button//�Ƿ���Ҫtitle�ϵ�buttonͼ��
                                         );

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
                                           );

/*****************************************************************************/
// 	Description : ���title��button״̬
//	Global resource dependence : 
//  Author: xiaoqinglu
//	Note: ��ñ���ͼ���״̬
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E GUIWIN_GetTitleButtonStatus(    
                                                           MMI_HANDLE_T               win_handle,
                                                           GUITITLE_BUTTON_INDEX_T   index
                                                           );

/***************************************************************************//*!
@brief ����title���½��б�������Ϣ
@author Liqing.peng
@param      win_handle       [in]   ���ھ��
@param      is_need_sub_text [in]   �Ƿ���Ҫ�б�������Ϣ
@param      wstr_ptr         [in]     ������ַ���
@param      wstr_len         [in]   �ַ�������
@param      is_need_update   [in]   �Ƿ�����ˢ����ʾ
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUITITLE_SetSubText�����ñ���ؼ���ʾ�ĸ����ַ���
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleSubText(    
                                      MMI_HANDLE_T   win_handle,
                                      BOOLEAN        is_need_sub_text,   //�Ƿ���Ҫ�б�������Ϣ
                                      const wchar*	 wstr_ptr,		//[IN] string pointer
                                      uint16		 wstr_len,		//[IN] string length
                                      BOOLEAN        is_need_update        //�Ƿ�������ʾ
                                      );


/***************************************************************************//*!
@brief  ����title���½��б�������Ϣ����
@author Liqing.peng
@param      win_handle    [in]   ���ھ��
@param      text_rect_ptr [in]   ���ַ���������
@param      font_ptr      [in]   ���ַ�������
@param      align_ptr     [in]   �Ű淽ʽ
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUITITLE_SetSubTextParam�����ñ���ؼ���ʾ�ĸ����ַ����ķ��
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleSubTextParam(
                                           MMI_HANDLE_T     win_handle,
                                           GUI_RECT_T       *text_rect_ptr, // ���ַ���������
                                           GUI_FONT_T       *font_ptr,      // ���ַ�������
                                           GUI_ALIGN_E      *align_ptr      // align type
                                           );


/***************************************************************************//*!
@brief ���sub text����ʾ��Ϣ
@author Liqing.peng
@param      win_handle [in]   ���ھ��
@retval TRUE    sub text�����
@retval FALSE   ���ʧ��
@return �Ƿ�����ɹ�
@note   �˺�����װ��GUITITLE_ClearSubTextInfo��Clear����ؼ��ĸ����ַ���
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_ClearTitleSubTextInfo(    
                                            MMI_HANDLE_T   win_handle
                                            );


/***************************************************************************//*!
@brief ����title
@author Liqing.peng
@param       win_handle [in]   ���ھ��
@retval TRUE     ���³ɹ�
@retval FALSE    ����ʧ��  
@return �Ƿ���³ɹ�
@note   �˺�����װ��GUITITLE_Update������title��������ʾ
*******************************************************************************/
PUBLIC void GUIWIN_UpdateTitle(
                               MMI_HANDLE_T win_handle
                               );


/***************************************************************************//*!
@brief  ����title ��ʾ������
@author Liqing.peng
@param      win_handle [in]   ���ھ��
@param      rect       [in]   ���õ�����
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   �˺�����װ��GUITITLE_SetRect������title ��ʾ������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleRect(
                                   MMI_HANDLE_T win_handle,
                                   GUI_RECT_T	rect
                                   );


/***************************************************************************//*!
@brief  ����title�ؼ��ĺ�������ʾ����
@author Jassmine
@param      win_handle [in]   ���ھ��
@param      rect_ptr   [in]   ����������
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ�
@note   ����title�ؼ��ĺ�������ʾ����
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleBothRect(
                                       MMI_HANDLE_T     win_handle,
                                       GUI_BOTH_RECT_T  *rect_ptr
                                       );


/***************************************************************************//*!
@brief ����title�ؼ��ı���
@author Liqing.peng
@param      win_handle [in]   ���ھ��
@param      bg_image   [in]   ����ͼƬid
@param      bg_color   [in]   ������ɫ
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ� 
@note   �˺�����װ��GUITITLE_SetBackground������title�ؼ��ı���
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleBackground(
                                         MMI_HANDLE_T		win_handle,
                                         MMI_IMAGE_ID_T		bg_image,
                                         GUI_COLOR_T		bg_color
                                         );


/***************************************************************************//*!
@brief ��̬����title�ؼ�
@author Liqing.peng
@param      win_handle [in]   ���ھ��
@param      text_id    [in]   �ַ�����Դid
@retval TRUE    �����ɹ�
@retval FALSE   ����ʧ��
@return �Ƿ񴴽��ɹ�
@note   �˺�����װ��GUITITLE_CreateCtrlDyn����̬����title�ؼ�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_CreateTitleDyna(
                                      MMI_HANDLE_T	win_handle,
                                      MMI_TEXT_ID_T	text_id
                                      );


/***************************************************************************//*!
@brief ����title��icon
@author Liqing.peng
@param      win_handle [in]   ���ھ��
@param      icon_id    [in]   iconͼƬ��Դid
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ� 
@note   �˺�����װ��GUITITLE_SetIcon����������title��icon��ע�⵱title�ַ����ܳ�
        ���ҹ�����ʾʱ��title�����Ҹ���45���ؿ�ȿ�����ʾiconͼƬ����ʱͼƬ���
        ���ܳ���45���أ�������ɸ�������
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleIcon(
                                   MMI_HANDLE_T     win_handle,
                                   MMI_IMAGE_ID_T   icon_id
                                   );


/***************************************************************************//*!
@brief ����title������ɫ
@author Jassmine
@param      win_handle [in]   ���ھ��
@param      font_color [in]   ������ɫ
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ� 
@note   �˺�����װ��GUITITLE_SetFontColor����������title������ɫ
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetTitleFontColor(
                                        MMI_HANDLE_T     win_handle,
                                        GUI_COLOR_T      font_color
                                        );

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
                                        );

/***************************************************************************//*!
@brief  �����Ƿ�ɼ�
@author xiaoqing.lu
@param      win_handle [in]   ���ھ��
@param      is_visible [in]   �Ƿ�ɼ�
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ� 
@note   �˺�����װ��GUISOFTKEY_SetVisible����������Ƿ�ɼ�
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetSoftkeyVisible(
                                        MMI_HANDLE_T    win_handle,
                                        BOOLEAN         is_visible
                                        );

/***************************************************************************//*!
@brief  ��ȡSoftkey�Ƿ�ɼ�
@author xiaoqing.lu
@param      win_handle [in]   ���ھ��
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ�ɼ�
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUIWIN_GetSoftkeyVisible(
                                        MMI_HANDLE_T    win_handle
                                        );

/***************************************************************************//*!
@brief  ���ÿؼ��ı���
@author xiaoqing.lu
@param      win_handle [in]   ���ھ��
@param      bg_ptr     [in]   �����������ݽṹ
@retval TRUE    ���óɹ�
@retval FALSE   ����ʧ�� 
@return �Ƿ����óɹ� 
@note   �˺�����װ��GUISTATUSBAR_SetCommonBg������״̬���ؼ�����
*******************************************************************************/
PUBLIC void GUIWIN_SetStbCommonBg(
                                  MMI_HANDLE_T  win_handle,
                                  GUI_BG_T      *bg_ptr
                                  );

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
                                        );

/*****************************************************************************/
//  Description : ����item��bg
//  Global resource dependence : 
//  Author:xiyuan.ma 2012.1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIWIN_SetStbItemBg(
                                   uint32     item_index,
                                   GUI_BG_T   *bg_info_ptr
                                   );

/*****************************************************************************//*!
@brief  ��ȡ״̬����LCD DEV INFO
@author xiyuan.ma
@param  win_handle       [in]   ���ھ��
@param  dev_info_ptr     [in]   LCD DEV INFO  
@note   �˽ӿ����ڰ����û���ȡ״̬����LCD��Ϣ
/*****************************************************************************/
PUBLIC void GUIWIN_GetStatusbarLayer(
									 MMI_HANDLE_T win_handle,
									 GUI_LCD_DEV_INFO *dev_info_ptr
									 );

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
                                          );

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
                                           );

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
                                         );

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
                                          );

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
                                                  );

/*--------------------------------------------------------------------------*/
/*                         Compiler Flag                                    */
/*--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _GUI_WIN_H_


/*@}*/
