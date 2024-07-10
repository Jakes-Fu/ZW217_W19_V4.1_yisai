/****************************************************************************
** File Name:      mmicom_panel.h                                           *
** Author:                                                                  *
** Date:           2010/7/27                                                *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 7/2010        aoke.hu            Create                                  *
**                                                                          *
****************************************************************************/
#ifndef MMICOM_PANEL_H
#define MMICOM_PANEL_H

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/

#include "sci_types.h"
#include "guibutton.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             90
#elif MAINLCD_SIZE_240X320
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             134
#elif MAINLCD_SIZE_240X400
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             134
#elif MAINLCD_SIZE_320X480
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             180
#else
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             134
#endif

#ifndef UI_MULTILAYER_SUPPORT
#define MMICOM_PLAYER_PANEL_CREATE_WIN             0x01
#endif
/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
typedef enum
{
    MMICOM_PANEL_VOLUME,
    MMICOM_PANEL_BRIGHTNESS,
    MMICOM_PANEL_TYPE_MAX
}MMICOM_PANEL_TYPE_E;

typedef enum
{
    MMICOM_PANEL_OPEN_NONE,
    MMICOM_PANEL_OPEN_INC,
    MMICOM_PANEL_OPEN_DEC,
    MMICOM_PANEL_OPEN_VALUE_SWITCH,     //��ʱ�������Ǿ����л�
    MMICOM_PANEL_OPEN_MAX
}MMICOM_PANEL_OPEN_TYPE_E;

typedef struct  
{
    uint32              cur_value;        //��ǰֵ��
    uint32              last_value;      //last_value�ڻָ�����ʱ�ã�cur_valueΪ0ʱ��Ч
}MMICOM_PANEL_CALLBACK_T;

typedef void (*SETVALUE_CALLBACK)(MMICOM_PANEL_CALLBACK_T *para_ptr);

typedef struct  
{
    int16                       x;              //��������x����, Ԥ�ø�����Ĭ���㷨
    int16                       y;              //��������y����, Ԥ�ø�����Ĭ���㷨
    MMICOM_PANEL_TYPE_E         panel_type;    //�����������
    MMICOM_PANEL_OPEN_TYPE_E    open_type;
    uint32                      cur_value;      //��ǰֵ,�����Ļ���Ϊ0
    uint32                      min_value;      //��Сֵ,�������Ļ�,��Ϊ0
    uint32                      max_value;      //���ֵ
    uint32                      last_value;      //last_value�ڻָ�����ʱ�ã�cur_valueΪ0ʱ��Ч
    MMI_STRING_T                title_str;       //�����⣻  ��ʾЧ��Ϊ��"������"��������/���ȵ���
    uint32                      createwin_style; //��ֵΪMMICOM_PLAYER_PANEL_CREATE_WINʱ����ʾ������һ���´��ڣ�Ĭ�ϴ����Ӵ��ڣ����ȥͼ�����������
    SETVALUE_CALLBACK           set_callback;   //���ں���
}MMICOM_PANEL_INFO_T;
 
/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open panel
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: Ĭ�Ͼ��У� 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_OpenPanelChildWin(MMI_WIN_ID_T win_id,
                                           MMICOM_PANEL_INFO_T *panel_info_ptr);

/*****************************************************************************/
//  Description :close panel
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_ClosePanelChildWin(void);

/*****************************************************************************/
//  Description : get panel is opened or not
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsPanelOpened(void);

/*****************************************************************************/
//  Description : get panel type
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC MMICOM_PANEL_TYPE_E MMIAPICOM_GetPanelType(void);

/*****************************************************************************/
//  Description : update panel
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_UpdatePanel(void);

/*****************************************************************************/
//  Description : display volume icon
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_DisplayVolIcon( MMI_WIN_ID_T win_id,  
                                     uint8 volume, 
                                     GUIBUTTON_OWNER_DRAW_T    *owner_draw_ptr);

/*****************************************************************************/
//  Description : switch value
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: value��0���0���л���    ������Ǿ����л�
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_SwitchPanelValue(MMICOM_PANEL_TYPE_E  panel_type);

#endif  // MMICOM_PANEL_H
