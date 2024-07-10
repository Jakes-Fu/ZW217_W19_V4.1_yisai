/***************************************************************************
** File Name:      ctrlbutton_export.h                                     *
** Author:         hua.fang                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _CTRLBUTTON_EXPORT_H_
#define _CTRLBUTTON_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "guictrl.h"
#include "guifont.h"

#include "ctrlbase_export.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
/*! @enum GUIBUTTON_TYPE_E
@brief button����
*/
typedef enum
{
    GUIBUTTON_TYPE_NORMAL,  /*!<Ĭ��*/
    GUIBUTTON_TYPE_RADIO,   /*!<��ѡ*/
    GUIBUTTON_TYPE_CHECK,   /*!<��ѡ*/
    GUIBUTTON_TYPE_MAX,     /*!<����λ*/
} GUIBUTTON_TYPE_E;

/*! @enum GUIBUTTON_STYLE_E
@brief buttonͼ��ʱ����ʾ���
*/
typedef enum
{
    GUIBUTTON_STYLE_DEFAULT,    /*!<Ĭ�ϣ�ͼ���ص�*/
    GUIBUTTON_STYLE_UIDT,       /*!<��ͼ����*/
    GUIBUTTON_STYLE_LIRTM,       /*!<��ͼ����(ͼ�ľ���)*/
    GUIBUTTON_STYLE_LIRTL,       /*!<��ͼ����(ͼ�ľ���)*/
    GUIBUTTON_STYLE_MAX
} GUIBUTTON_STYLE_E;

/*! @enum GUIBUTTON_OWNDRAW_STATE_E
@brief button�Ի�ʱ��״̬
*/
typedef enum
{
    GUIBUTTON_OWNDRAW_NONE,    /*!<none*/
    GUIBUTTON_OWNDRAW_TP_DOWN, /*!<tp dow*/
    GUIBUTTON_OWNDRAW_TP_MOVE, /*!<tp move*/
    GUIBUTTON_OWNDRAW_TP_UP,   /*!<tp up*/
    GUIBUTTON_OWNDRAW_MAX
} GUIBUTTON_OWNDRAW_STATE_E;

/*! @enum GUIBUTTON_ANIM_TYPE_E
@brief button�İ�����Ч����
*/
typedef enum
{
    GUIBUTTON_NONE_ANIM,            /*!<�޶�����Ч*/
    GUIBUTTON_RUNSHEEN_ANIM,    /*!<runsheen ������Ч*/
    GUIBUTTON_POPBLINK_ANIM,     /*!<popblink ������Ч*/
    GUIBUTTON_LOAD_MAX               /*!<����λ*/
} GUIBUTTON_EFFECT_TYPE_E;

/*! @typedef GUIBUTTON_CALLBACK_FUNC
@brief �ؼ��ص�����,��Я������
*/
typedef MMI_RESULT_E (*GUIBUTTON_CALLBACK_FUNC)(void);

/*! @typedef GUIBUTTON_CALLBACK_FUNC_EXT
@brief �ؼ��ص�����,��Я������
*/
typedef MMI_RESULT_E (*GUIBUTTON_CALLBACK_FUNC_EXT)(MMI_HANDLE_T ctrl_handle);

/*! @struct GUIBUTTON_INIT_DATA_T
@brief button�ؼ���ʼ������
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<��������*/
    GUI_BG_T            bg;         /*!<�ɿ�״̬����*/
} GUIBUTTON_INIT_DATA_T;

/*! @struct GUIBUTTON_OWNER_DRAW_T
@brief �ؼ��Ի�ص�����������Ϣ
*/
typedef struct 
{
    int16               display_x;      /*!<������ʾx����*/
    int16               display_y;      /*!<������ʾy����*/
    int16               fg_display_x;   /*!<ǰ����ʾx����*/
    int16               fg_display_y;   /*!<ǰ����ʾy����*/
    GUI_RECT_T          display_rect;   /*!<��ʾ����*/
    MMI_HANDLE_T        ctrl_handle;    /*!<�ؼ�handle*/
    GUIBUTTON_OWNDRAW_STATE_E draw_state;
    GUI_LCD_DEV_INFO    lcd_dev;        /*!<lcd��Ϣ*/
} GUIBUTTON_OWNER_DRAW_T;

/*! @typedef GUIBUTTON_OWNER_DRAW_FUNC
@brief �ؼ��Ի�ص�����,Я������
*/
typedef void (*GUIBUTTON_OWNER_DRAW_FUNC)(
                                          GUIBUTTON_OWNER_DRAW_T    *owner_draw_ptr
                                          );

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ��̬����button�ؼ�
@author Jassmine
@param win_id   [in] ����id
@param ctrl_id  [in] �ؼ�id
@param init_ptr [in] ��ʼ������
@return �ؼ�handle
@note 
*******************************************************************************/
PUBLIC MMI_HANDLE_T CTRLBUTTON_CreateDynamic(
                                            MMI_WIN_ID_T            win_id,     //in:
                                            MMI_CTRL_ID_T           ctrl_id,    //in:
                                            GUIBUTTON_INIT_DATA_T   *init_ptr   //in:
                                            );

/***************************************************************************//*!
@brief ����button�ؼ���ʾ���
@author hua.fang
@param ctrl_id [in] �ؼ�id
@param style  [in] ��ʾ���
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetStyle(
                                  MMI_CTRL_ID_T         ctrl_id,        //in
                                  GUIBUTTON_STYLE_E     style           //in:
                                  );

/***************************************************************************//*!
@brief ����button�ؼ�����,���ɿ�״̬������Ϣ
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param bg_ptr  [in] ����,����Ϊ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetBg(
                               MMI_CTRL_ID_T    ctrl_id,    //in
                               GUI_BG_T         *bg_ptr     //in:may PNULL
                               );

/***************************************************************************//*!
@brief ����button�ؼ�ǰ��,���ɿ�״̬ǰ����Ϣ
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param fg_ptr  [in] ǰ����Ϣ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFg(
                               MMI_CTRL_ID_T    ctrl_id,    //in
                               GUI_BG_T         *fg_ptr     //in:may PNULL
                               );

/***************************************************************************//*!
@brief ����button�ؼ����±�����Ϣ
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param bg_ptr  [in] ���±�����Ϣ,����Ϊ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetPressedBg(
                                      MMI_CTRL_ID_T    ctrl_id,    //in
                                      GUI_BG_T         *bg_ptr     //in:may PNULL
                                      );

/***************************************************************************//*!
@brief ����button�ؼ�����ǰ����Ϣ
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param fg_ptr  [in] ����ǰ����Ϣ,����Ϊ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetPressedFg(
                                      MMI_CTRL_ID_T    ctrl_id,    //in
                                      GUI_BG_T         *fg_ptr     //in:may PNULL
                                      );

/***************************************************************************//*!
@brief ����button�ؼ��Ƿ�һ�,�Լ��һ�������Ϣ
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param is_gray   [in] �Ƿ�һ�
@param gray_ptr  [in] �һ�������Ϣ
@param is_update [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetGrayed(
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   BOOLEAN          is_gray,    //in:
                                   GUI_BG_T         *gray_ptr,  //in:
                                   BOOLEAN          is_update   //in:
                                   );

/***************************************************************************//*!
@brief ����button�ؼ��Ƿ�һ�,�Լ��һ�ǰ����Ϣ
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param is_gray   [in] �Ƿ�һ�
@param gray_ptr  [in] �һ�ǰ����Ϣ
@param is_update [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetGrayedFg(
                                     MMI_CTRL_ID_T      ctrl_id,    //in:
                                     BOOLEAN            is_gray,    //in:
                                     GUI_BG_T           *gray_ptr,  //in:
                                     BOOLEAN            is_update   //in:
                                     );

/***************************************************************************//*!
@brief ����button�ؼ��ı�id
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param text_id [in] �ı�id
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetTextId(
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   MMI_TEXT_ID_T    text_id     //in
                                   );

/***************************************************************************//*!
@brief ����button�ؼ��ı���Ϣ
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param str_ptr [in] �ı�����
@param str_len [in] �ı�����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetText(
                                 MMI_CTRL_ID_T  ctrl_id,    //in:
                                 wchar          *str_ptr,   //in:
                                 uint16         str_len     //in:
                                 );

/***************************************************************************//*!
@brief ����button�ؼ��ı����з�ʽ
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param align   [in] ���з�ʽ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetTextAlign(
                                      MMI_CTRL_ID_T     ctrl_id,//in:
                                      GUI_ALIGN_E       align   //in
                                      );

/*****************************************************************************/
//  Description : set button font color
//  Parameter: [In] ctrl_id : button control id
//             [In] font_color : button font color
//             [Out] None
//             [Return] result : TRUE:secceed , FALSE : failed
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFontColor(
                                      MMI_CTRL_ID_T     ctrl_id,//in:
                                      CAF_COLOR_T       font_color//in
                                      );

/***************************************************************************//*!
@brief ����button�ؼ��ص�����,��Я������
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param func    [in] �ص�����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetCallBackFunc(
                                         MMI_CTRL_ID_T              ctrl_id,
                                         GUIBUTTON_CALLBACK_FUNC    func
                                         );

/***************************************************************************//*!
@brief ����button�ؼ��ص�����,��Я������
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param func    [in] �ص�����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetCallBackFuncExt(
                                            MMI_CTRL_ID_T              ctrl_id,
                                            GUIBUTTON_CALLBACK_FUNC_EXT    func
                                            );

/***************************************************************************//*!
@brief ����button�ؼ��Ƿ���tp long��Ϣ
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param is_handle [in] �Ƿ���
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetHandleLong(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       BOOLEAN          is_handle
                                       );

/***************************************************************************//*!
@brief ����button�ؼ��Ƿ��͸��ʾ����
@author Jassmine
@param ctrl_id        [in] �ؼ�id
@param is_transparent [in] �Ƿ��͸��ʾ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetTransparent(
                                        MMI_CTRL_ID_T   ctrl_id,
                                        BOOLEAN         is_transparent
                                        );

/***************************************************************************//*!
@brief ����button�ؼ��Ƿ�ʹ�ù���Ч��
@author Jassmine
@param ctrl_id     [in] �ؼ�id
@param is_runsheen [in] �Ƿ�ʹ�ù���Ч��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetRunSheen(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     BOOLEAN        is_runsheen
                                     );

/***************************************************************************//*!
@brief ����button�ؼ�����
@author Jassmine
@param ctrl_id  [in] �ؼ�id
@param rect_ptr [in] ����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetRect(
                                MMI_CTRL_ID_T   ctrl_id,    //in:
                                GUI_RECT_T      *rect_ptr   //in:
                                );

/***************************************************************************//*!
@brief ����button�ؼ��Ƿ�������ʾ�߿�
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param is_permit [in] �Ƿ���ʾ�߿�
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_PermitBorder(
                                      MMI_CTRL_ID_T     ctrl_id,
                                      BOOLEAN           is_permit
                                      );

/***************************************************************************//*!
@brief ����button�ؼ�����
@author Jassmine
@param ctrl_id  [in] �ؼ�id
@param font_ptr [in] ������Ϣ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFont(
                                 MMI_CTRL_ID_T      ctrl_id,    //in:
                                 GUI_FONT_ALL_T     *font_ptr   //in:
                                 );

/***************************************************************************//*!
@brief ����button�ؼ��Ƿ�ɼ�
@author Jassmine
@param ctrl_id    [in] �ؼ�id
@param is_visible [in] �Ƿ�ɼ�
@param is_update  [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetVisible(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    BOOLEAN         is_visible,
                                    BOOLEAN         is_update
                                    );

/***************************************************************************//*!
@brief ����button�ؼ��Ի�ص�����
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param func    [in] �Ի�ص�����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC void CTRLBUTTON_SetOwnerDraw(
                                   MMI_CTRL_ID_T                ctrl_id,
                                   GUIBUTTON_OWNER_DRAW_FUNC    func
                                   );

/***************************************************************************//*!
@brief ˢ��button�ؼ�
@author Jassmine
@param ctrl_id [in] �ؼ�id
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_Update(
                                MMI_CTRL_ID_T   ctrl_id
                                );

/*****************************************************************************/
//  Description : update button text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_UpdateText(
                                    MMI_CTRL_ID_T   ctrl_id
                                    );

/***************************************************************************//*!
@brief �Ƿ��ֹ�Զ�ˢ��
@author hua.fang
@param ctrl_id [in] �ؼ�id
@param is_forbid_paint [in] �Ƿ��ֹ�Զ�ˢ��
@return 
@note 
*******************************************************************************/
PUBLIC void CTRLBUTTON_SetForbidPaint(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     BOOLEAN        is_forbid_paint
                                     );

/*****************************************************************************//*!
@brief  ����button��border
@author Sam.hua
@param  ctrl_id    [in] �ؼ�id
@param  border_ptr [in] border�Ĳ���ָ��
@param  is_update  [in] �Ƿ�ˢ��
@return TRUE�����óɹ� FALSE������ʧ��
@note
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetBorder(
                                   MMI_CTRL_ID_T ctrl_id,    //in:
                                   GUI_BORDER_T  *border_ptr,
                                   BOOLEAN       is_update
                                   );

/*****************************************************************************//*!
@brief  ����button pop��������ز���
@author xiyuan.ma
@param  ctrl_id      [in] �ؼ�id
@param  pop_info_ptr [in] pop�����Ĳ���ָ��
@return TRUE�����óɹ� FALSE������ʧ��
@note   button��pop��������Ĭ��ֵ���û�����ͨ���˽ӿ��������ЩĬ��ֵ���ﵽ���Ч��
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetPopAnimInfo(
                                        MMI_CTRL_ID_T ctrl_id,
                                        int32*        total_frame_ptr,    
                                        int32*        radius_ptr,         
                                        uint32*       filled_color_ptr 
                                        );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _CTRLBUTTON_EXPORT_H_
