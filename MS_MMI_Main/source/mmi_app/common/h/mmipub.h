/*****************************************************************************
** File Name:      mmipub.h                                                  *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2007       Jassmine              Creat
******************************************************************************/

#ifndef _MMIPUB_H_
#define _MMIPUB_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmitheme_pubwin.h"
#include "mmi_anim.h"
#include "guiedit.h"
#include "guilistbox.h"
#include "guiform.h"
#include "guilabel.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
        
typedef PROCESSMSG_FUNC MMIPUB_HANDLE_FUNC;
//pub tips dis style
typedef enum
{
    MMIPUB_TIPS_DIS_TOP,        //top
    MMIPUB_TIPS_DIS_MIDDLE,     //middle
    MMIPUB_TIPS_DIS_BOTTOM,     //bottom
    MMIPUB_TIPS_DIS_CUSTOM,     //custom,�ݲ�֧�֣�����󣬻ᰴ��BOTTOM�Դ�
    MMIPUB_TIPS_DIS_MAX
} MMIPUB_TIPS_DIS_STYLE_E;

//pubwin softkey style
typedef enum
{
    MMIPUB_SOFTKEY_NONE,        //none
    MMIPUB_SOFTKEY_ONE,         //one:exit
    MMIPUB_SOFTKEY_OKEXIT,      //two:ok exit
    MMIPUB_SOFTKEY_OKCANCEL,    //two:ok cancel
    MMIPUB_SOFTKEY_CUSTOMER,    //customer
    MMIPUB_SOFTKEY_DIALOG_DISMISS,//not common style
    MMIPUB_SOFTKEY_DIALOG_ALARM,
    MMIPUB_SOFTKEY_MAX
} MMIPUB_SOFTKEY_STYLE_E;

#define MMIPUB_TEXT_MAX 3

//////////////////////////////////////////////////////////////////////////
// added by feng.xiao for NEWMS00180461
typedef struct _MMIPUB_PROGRESS_IMG_T
{
    MMI_IMAGE_ID_T  m_bg_img_id;
    MMI_IMAGE_ID_T  m_ft_img_id;
} MMIPUB_PROGRESS_IMG_T;

//pubwin infomation
typedef struct
{
    uint32                  user_data;                  //pubwin create from this,just identifier
    PROCESSMSG_FUNC         user_func;                  //�û�����Ļص�
    uint8                   update_delay_timer_id;      //�ӳ�ˢ�µ�timer id
    uint8                   timer_id;                   //timer id
    uint32                  time_period;                //time period
    MMI_IMAGE_ID_T          prompt_image_id;            //prompt image id
    MMI_ANIM_ID_T           wait_anim_id;               //wait anim id
    MMIPUB_PROGRESS_IMG_T   prg_img_info;               //added by feng.xiao for NEWMS00180461

    MMI_TEXT_ID_T           softkey_id[GUISOFTKEY_BUTTON_NUM];  //softkey id
    BOOLEAN                 is_softkey_gray[GUISOFTKEY_BUTTON_NUM];  //softkey id
    BOOLEAN                 is_handle_long[GUISOFTKEY_BUTTON_NUM];   //�Ƿ���penlong��Ϣ
    MMIPUB_SOFTKEY_STYLE_E  softkey_style;                      //softkey style
    
    MMITHEME_PUBWIN_T       pubwin_theme;               //public window theme
    MMI_WIN_ID_T            win_id;
    MMI_HANDLE_T            win_handle;
    MMI_HANDLE_T            ctrl_handle;
    MMI_MESSAGE_ID_E        user_msg_id;             //user's msg

    MMI_STRING_T            title_string;

    MMI_STRING_T            string[MMIPUB_TEXT_MAX];
    uint16                  line_num;
    BOOLEAN                 is_layout; // �Ƿ񲼾ֹ�

    GUI_RECT_T              bg_rect;        // PUBWIN������
    GUI_RECT_T              alert_rect;     // ����ͼ��
                                            // MMIPUBWIN_PROGRESS: ��������λ��
    GUI_RECT_T              title_rect;     // title����
    GUI_RECT_T              client_rect;    // �ͻ���

    GUI_LCD_DEV_INFO        pubwin_bg_dev; // Ӧ�ö�㣬��ʾ����ͼƬ����Ϣ����
    GUI_LCD_DEV_INFO        pubwin_mask_dev; // Ӧ�ö�㣬��ʾ�ɲ�
    BOOLEAN                 is_3danim_invalid;
    uint32                  type;    
    BOOLEAN                 is_waitwin_update_delay;//wait win �Ƿ���Ҫ�ӳ�ˢ��
    
    ADD_DATA			    extra_data_ptr;//Add extra info ptr
} MMIPUB_INFO_T;

//pub waiting win param
typedef struct
{
    MMI_STRING_T            wait_string;                //string
    MMI_TEXT_ID_T           wait_id;                    //��0��ʹ��text id, ����ʹ��string
    PROCESSMSG_FUNC         user_func;                  //�û��ص�����, ����Ϊ��
    MMI_WIN_ID_T            win_id;                     //�û�ָ���Ĵ���id, ��Ϊ0��ʹ��Ĭ�ϵ�id
    MMI_HANDLE_T            applet_handle;              //���ڵĸ�Ӧ�þ��, Ϊ0��ʹ��Ĭ�ϵ�first applet handle
} MMIPUB_WAITING_PARAM_T;

//pub list win infomation
typedef struct
{
    GUILIST_TYPE_E          list_type;
    
} MMIPUB_LIST_PARAM_T;

//pub form win infomation
typedef struct
{
    GUIFORM_LAYOUT_E        layout_type;
} MMIPUB_FORM_PARAM_T;

//pub edit win infomation
typedef struct
{
    //����ĳЩ��label��edit����
    MMI_CTRL_ID_T           label_id;
    GUILABEL_ALIGN_E        label_align;
    
    GUIEDIT_TYPE_E          edit_type;
    GUIEDIT_INIT_ADD_U      add_data;
    uint16                  max_len;
} MMIPUB_EDIT_PARAM_T;

typedef union
{
    MMIPUB_LIST_PARAM_T list_param;
    MMIPUB_EDIT_PARAM_T edit_param;
    MMIPUB_FORM_PARAM_T form_param;
}MMIPUB_CTRL_PARAM_U;

typedef struct  
{
    ADD_DATA                add_data_ptr;
    PROCESSMSG_FUNC         user_func;                  //�û��ص�����, ����Ϊ��
    MMI_WIN_ID_T            win_id;                     //�û�ָ���Ĵ���id, ��Ϊ0��ʹ��Ĭ�ϵ�id
    MMI_CTRL_ID_T           ctrl_id;
    MMI_HANDLE_T            applet_handle;              //���ڵĸ�Ӧ�þ��, Ϊ0��ʹ��Ĭ�ϵ�first applet handle
    MMI_TEXT_ID_T           title_id;
    MMI_TEXT_ID_T           left_sk_id;
    MMI_TEXT_ID_T           middle_sk_id;
    MMI_TEXT_ID_T           right_sk_id;
    CAF_GUID_T              guid;
    MMIPUB_CTRL_PARAM_U     ctrl_param;
}MMIPUB_PARAM_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*
#define MMIPUB_OpenAlertWinByTextId(param1,param2,param3,param4,param5,param6,param7,param8)	\
	MMIPUB_OpenAlertSuccessWin(param2)

//#define MMIPUB_OpenAlertWinByTextId(param1,param2,param3,param4,param5,param6,param7,param8)	\
//	MMIPUB_OpenAlertWinByTextId0(param2, param4)

//#define MMIPUB_OpenAlertWinByTextId(param1,param2,param3,param4,param5,param6,param7,param8)	\
//	MMIPUB_OpenAlertWinByTextId1(param1,param2,param3,param4,param5,param6,param7,param8)
*/

/*****************************************************************************/
//  Description : open success window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertSuccessWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : open fail window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertFailWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : open warning window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWarningWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : open waiting window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWaitingWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : creat and open alert window by text id
//                ��text1��ֵ, text2ֵΪ��ʱ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWinByTextId(
                                        uint32                     *time_period_ptr,   //time period,default is 2s
                                        MMI_TEXT_ID_T              text1_id,           //text 1,no default TXT_NULL
                                        MMI_TEXT_ID_T              text2_id,           //text 2,no default TXT_NULL
                                        MMI_IMAGE_ID_T             image_id,           //alert image id
                                        MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                        MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                        MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                        MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                        );

/*****************************************************************************/
//  Description : creat and open alert window by text id
//                ��text1��ֵ, text2ֵΪ��ʱ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWinByTextIdEx(
                                          MMI_HANDLE_T               applet_handle,
                                          uint32                     *time_period_ptr,   //time period,default is 2s
                                          MMI_TEXT_ID_T              text1_id,           //text 1,no default TXT_NULL
                                          MMI_TEXT_ID_T              text2_id,           //text 2,no default TXT_NULL
                                          MMI_IMAGE_ID_T             image_id,           //alert image id
                                          MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                          MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                          MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                          MMIPUB_HANDLE_FUNC         handle_func,        //handle function
                                          uint32                     user_data           //user_data
                                          );

/*****************************************************************************/
//  Description : creat and open alert window by text pointer
//                ��text1��ֵ, text2ֵΪ��ʱ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWinByTextPtr(
                                         uint32                    *time_period_ptr,   //time period,default is 3s
                                         MMI_STRING_T              *text1_ptr,         //text 1
                                         MMI_STRING_T              *text2_ptr,         //text 2
                                         MMI_IMAGE_ID_T            image_id,           //alert image id
                                         MMI_WIN_ID_T              *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                         MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                         MMIPUB_SOFTKEY_STYLE_E    softkey_style,      //softkey style
                                         MMIPUB_HANDLE_FUNC        handle_func         //handle function
                                         );

/*****************************************************************************/
//  Description : creat and open alert window by text pointer
//                ��text1��ֵ, text2ֵΪ��ʱ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWinByTextPtrEx(
                                           MMI_HANDLE_T              applet_handle,
                                           uint32                    *time_period_ptr,   //time period,default is 3s
                                           MMI_STRING_T              *text1_ptr,         //text 1
                                           MMI_STRING_T              *text2_ptr,         //text 2
                                           MMI_IMAGE_ID_T            image_id,           //alert image id
                                           MMI_WIN_ID_T              *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                           MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                           MMIPUB_SOFTKEY_STYLE_E    softkey_style,      //softkey style
                                           MMIPUB_HANDLE_FUNC        handle_func,        //handle function
                                           uint32                    user_data           //user_data
                                           );

/*****************************************************************************/
//  Description : handle alert window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_HandleAlertWinMsg(
                                             MMI_WIN_ID_T      win_id, 
                                             MMI_MESSAGE_ID_E  msg_id, 
                                             DPARAM            param
                                             );

/*****************************************************************************/
//  Description : get alert window timer id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPUB_GetAlertTimerId(
                             MMI_WIN_ID_T      win_id
                             );

/*****************************************************************************/
//  Description : set alert window text display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetAlertWinTextByPtr(
                                 MMI_WIN_ID_T       win_id,
                                 MMI_STRING_T       *text1_ptr,         //text 1
                                 MMI_STRING_T       *text2_ptr,         //text 2
                                 BOOLEAN            is_fresh
                                 );



/*****************************************************************************/
//  Description : set wait window text 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_UpdateWaitWinText(
                           BOOLEAN          is_fresh,   //is fresh
                           uint8            text_num,   //text number
                           MMI_STRING_T     *text1_ptr, //text 1
                           MMI_STRING_T     *text2_ptr, //text 2
                           MMI_STRING_T     *text3_ptr, //text 3
                           MMI_WIN_ID_T     win_id
                           );                                 

/*****************************************************************************/
//  Description : set pubwin window softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
// ���idΪ0�����޸�ԭ����
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinSoftkey(
                                 MMI_WIN_ID_T          win_id,
                                 MMI_TEXT_ID_T         left_sk_id,
                                 MMI_TEXT_ID_T         right_sk_id,
                                 BOOLEAN               is_fresh
                                 );


/*****************************************************************************/
//  Description : set pubwin window softkey
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
// ���idΪ0�����޸�ԭ����
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinSoftkeyEx(
                                   MMI_WIN_ID_T          win_id,
                                   MMI_TEXT_ID_T         left_sk_id,
                                   MMI_TEXT_ID_T         middle_sk_id,
                                   MMI_TEXT_ID_T         right_sk_id,
                                   BOOLEAN               is_fresh
                                   );

/*****************************************************************************/
//  Description : get pubwin window softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_GetWinSoftkey(
                                 MMI_WIN_ID_T          win_id,
                                 MMI_TEXT_ID_T         *left_sk_id_ptr,
                                 MMI_TEXT_ID_T         *middle_sk_id_ptr,
                                 MMI_TEXT_ID_T         *right_sk_id_ptr
                                 );

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
/*****************************************************************************/
//  Description : set pubwin window softkey gray
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinSoftkeyGray(
                                     MMI_WIN_ID_T    win_id,
                                     BOOLEAN         is_left_gray,
                                     BOOLEAN         is_middle_gray,
                                     BOOLEAN         is_right_gray,
                                     BOOLEAN         is_fresh
                                     );

/*****************************************************************************/
//  Description : get pubwin window softkey gray
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_GetWinSoftkeyGray(
                                     MMI_WIN_ID_T    win_id,
                                     BOOLEAN         *is_left_gray_ptr,
                                     BOOLEAN         *is_middle_gray_ptr,
                                     BOOLEAN         *is_right_gray_ptr
                                     );

/*****************************************************************************/
//  Description : set pubwin window softkey hanlde long
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinSoftkeyHandleLong(
                                           MMI_WIN_ID_T     win_id,
                                           uint16           button_index,
                                           BOOLEAN          is_left_gray
                                           );
#endif

/*****************************************************************************/
//  Description : set pubwin window title text id
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinTitleTextId(
                                     MMI_WIN_ID_T     win_id,
                                     MMI_TEXT_ID_T    text_id,
                                     BOOLEAN          is_fresh
                                     );

/*****************************************************************************/
//  Description : set pubwin window title text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinTitleText(
                                   MMI_WIN_ID_T     win_id,
                                   MMI_STRING_T     *text_ptr,
                                   BOOLEAN          is_fresh
                                   );

/*****************************************************************************/
//  Description : close alert public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseAlertWin(void);

/*****************************************************************************/
//  Description : close alert public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseAlertWinEx( 
                                      MMI_HANDLE_T  applet_handle
                                      );

/*****************************************************************************/
//  Description : creat and open query window by text id
//                ��text1��ֵ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinByTextId(
                                        MMI_TEXT_ID_T              text_id,           //text 1,no default TXT_NULL
                                        MMI_IMAGE_ID_T             image_id,           //alert image id
                                        MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                        MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                        );

/*****************************************************************************/
//  Description : creat and open query window by text id
//                ��text1��ֵ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinByTextIdEx(
                                          MMI_HANDLE_T               applet_handle,
                                          MMI_TEXT_ID_T              text_id,           //text 1,no default TXT_NULL
                                          MMI_IMAGE_ID_T             image_id,           //alert image id
                                          MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                          MMIPUB_HANDLE_FUNC         handle_func,         //handle function
                                          uint32                     user_data           //user_data
                                          );

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//                ��text1��ֵ, text2ֵΪ��ʱ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinByTextPtr(
                                         MMI_STRING_T               *text1_ptr,          //text 1,no default TXT_NULL
                                         MMI_STRING_T               *text2_ptr,          //text 1,no default TXT_NULL
                                         MMI_IMAGE_ID_T             image_id,           //alert image id
                                         MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                         MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                         );

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//                ��text1��ֵ, text2ֵΪ��ʱ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinByTextPtrEx(
                                           MMI_HANDLE_T               applet_handle,
                                           MMI_STRING_T               *text1_ptr,          //text 1,no default TXT_NULL
                                           MMI_STRING_T               *text2_ptr,          //text 1,no default TXT_NULL
                                           MMI_IMAGE_ID_T             image_id,           //alert image id
                                           MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                           MMIPUB_HANDLE_FUNC         handle_func,         //handle function
                                           uint32                     user_data           //user_data
                                           );

/*****************************************************************************/
//  Description : handle query window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_HandleQueryWinMsg(
                                             MMI_WIN_ID_T      win_id, 
                                             MMI_MESSAGE_ID_E  msg_id, 
                                             DPARAM            param
                                             );

/*****************************************************************************/
//  Description : close query public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseQuerytWin(
                                     MMI_WIN_ID_T  *win_id_ptr
                                     );

/*****************************************************************************/
//  Description : close query public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseQuerytWinEx(
                                       MMI_HANDLE_T  applet_handle,
                                       MMI_WIN_ID_T  *win_id_ptr
                                       );

/*****************************************************************************/
//  Description : creat and open wait window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenWaitWin(
                               uint8                   text_num,       //text number
                               MMI_STRING_T            *text1_ptr,     //text 1
                               MMI_STRING_T            *text2_ptr,     //text 2
                               MMI_STRING_T            *text3_ptr,     //text 3
                               MMI_WIN_ID_T            win_id,         //win id
                               MMI_IMAGE_ID_T          image_id,       //image id
                               MMI_ANIM_ID_T           anim_id,        //anim id
                               MMI_WIN_PRIORITY_E      win_priority,   //priority
                               MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                               MMIPUB_HANDLE_FUNC      handle_func     //handle function
                               );

/*****************************************************************************/
//  Description : creat and open wait window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenWaitWinEx(
                                 MMI_HANDLE_T            applet_handle,
                                 uint8                   text_num,       //text number
                                 MMI_STRING_T            *text1_ptr,     //text 1
                                 MMI_STRING_T            *text2_ptr,     //text 2
                                 MMI_STRING_T            *text3_ptr,     //text 3
                                 MMI_WIN_ID_T            win_id,         //win id
                                 MMI_IMAGE_ID_T          image_id,       //image id
                                 MMI_ANIM_ID_T           anim_id,        //anim id
                                 MMI_WIN_PRIORITY_E      win_priority,   //priority
                                 MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                                 MMIPUB_HANDLE_FUNC      handle_func,    //handle function
                                 uint32                  user_data       //user_data
                                 );

/*****************************************************************************/
//  Description : handle wait window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_HandleWaitWinMsg(
                                            MMI_WIN_ID_T      win_id, 
                                            MMI_MESSAGE_ID_E  msg_id,
                                            DPARAM            param
                                            );

/*****************************************************************************/
//  Description : set wait window text 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWaitWinText(
                           BOOLEAN          is_fresh,   //is fresh
                           uint8            text_num,   //text number
                           MMI_STRING_T     *text1_ptr, //text 1
                           MMI_STRING_T     *text2_ptr, //text 2
                           MMI_STRING_T     *text3_ptr, //text 3
                           MMI_WIN_ID_T     win_id
                           );

/*****************************************************************************/
//  Description : close wait public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseWaitWin(
                                   MMI_WIN_ID_T    win_id
                                   );

/*****************************************************************************/
//  Description : close wait public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseWaitWinEx(
                                     MMI_HANDLE_T    applet_handle,
                                     MMI_WIN_ID_T    win_id
                                     );


/*****************************************************************************/
//  Description : creat and open progress window
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenProgressWinEx(
                        MMI_HANDLE_T                applet_handle,
                        MMI_STRING_T                *text1_str_ptr,     //text 1
                        MMI_WIN_ID_T                *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                        MMI_WIN_PRIORITY_E          *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                        MMIPUB_SOFTKEY_STYLE_E      softkey_style,      //softkey style
                        MMIPUB_HANDLE_FUNC          handle_func         //handle function
                        );
/*****************************************************************************/
//  Description : creat and open progress window by text id
//                ��text1��ֵ, text2ֵΪ��ʱ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: J
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenProgressWinByTextId(
                                 MMI_TEXT_ID_T              text1_id,           //text 1,no default TXT_NULL
                                 MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                 MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                 MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                 MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                 );

/*****************************************************************************/
//  Description : creat and open progress window by text pointer
//                ��text1��ֵ, text2ֵΪ��ʱ, ���text1�ı��ܳ�, �Զ�������ʾ,
//                �����ʾ3��
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenProgressWinByTextPtr(
                                  MMI_STRING_T              *text1_ptr,         //text 1
                                  MMI_WIN_ID_T              *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                  MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                  MMIPUB_SOFTKEY_STYLE_E    softkey_style,      //softkey style
                                  MMIPUB_HANDLE_FUNC        handle_func         //handle function
                                  );

/*****************************************************************************/
//  Description : handle progress window message
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_HandleProgressWinMsg(
                                      MMI_WIN_ID_T      win_id, 
                                      MMI_MESSAGE_ID_E  msg_id, 
                                      DPARAM            param
                                      );

/*****************************************************************************/
//  Description : set progress win total size
//                
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetProgressTotalSize(MMI_WIN_ID_T *win_id_ptr, uint32 total_size);

/*****************************************************************************/
//  Description : update progress
//                
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_UpdateProgressBar(MMI_WIN_ID_T *win_id_ptr, uint32 finished_size);

/*****************************************************************************/
//  Description : update progress
//                
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_UpdateProgressBarEx(MMI_WIN_ID_T *win_id_ptr, uint32 finished_size, BOOLEAN is_flash );

/*****************************************************************************/
//  Description : set progress bar text display
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetProgressBarTextByPtr(
                                        MMI_WIN_ID_T       win_id,
                                        MMI_STRING_T       *text1_ptr,         //text 1
                                        BOOLEAN            is_fresh
                                        );
                                        
/*****************************************************************************/
//  Description : close progress windows
//                
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_CloseProgressWin(MMI_WIN_ID_T *win_id_ptr);

/*****************************************************************************/
//  Description : Interface for customer to open query win start app
//  Global resource dependence : 
//  Author: peng.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinStartApp( uint32 guid );
/*****************************************************************************/
//  Description : Interface for customer to open query win start app
//  Global resource dependence : 
//  Author: peng.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenStartAppWaitWin( uint32 guid );
/*****************************************************************************/
//  Description : Interface for customer to open query win no file exist
//  Global resource dependence : 
//  Author: peng.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinFilesNoExist(void);

/*****************************************************************************/
//  Description : creat and open query window by text id
//  Global resource dependence : 
//  Author: aoke
//  Note: ʹ��TEXT�ؼ���ʾ�����ı�������ͼƬ
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryTextWinByTextId(
                                            MMI_TEXT_ID_T              text_id,           //text 1,no default TXT_NULL
                                            MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_TEXT_WIN_ID
                                            MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                            );

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryTextWinByTextPtr(
                                             MMI_STRING_T               *text_ptr,          //text 1,no default TXT_NULL
                                             MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_QUERY_TEXT_WIN_ID
                                             MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                             );

/*****************************************************************************/
//  Description : creat and open alert text window by text id
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertTextWinByTextId(
                                            uint32                     *time_period_ptr,   //time period,default is 2s
                                            MMI_TEXT_ID_T              text_id,           //text 1,no default TXT_NULL
                                            MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_TEXT_WIN_ID
                                            MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                            MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                            MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                            );

/*****************************************************************************/
//  Description : creat and open alert text window by text id
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertTextWinByTextIdEx(
                                          MMI_HANDLE_T               applet_handle,
                                          uint32                     *time_period_ptr,   //time period,default is 2s
                                          MMI_TEXT_ID_T              text_id,           //text,no default TXT_NULL
                                          MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_TEXT_WIN_ID
                                          MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                          MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                          MMIPUB_HANDLE_FUNC         handle_func,        //handle function
                                          uint32                     user_data           //user_data
                                          );

/*****************************************************************************/
//  Description : creat and open alert window by text pointer
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertTextWinByTextPtr(
                                             uint32                    *time_period_ptr,   //time period,default is 2s
                                             MMI_STRING_T              *text_ptr,         //text
                                             MMI_WIN_ID_T              *win_id_ptr,        //win id,default is MMIPUB_ALERT_TEXT_WIN_ID
                                             MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                             MMIPUB_SOFTKEY_STYLE_E    softkey_style,      //softkey style
                                             MMIPUB_HANDLE_FUNC        handle_func         //handle function
                                             );

/*****************************************************************************/
//  Description : close alert public windows, free memory
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseAlertTextWin(void);

/*****************************************************************************/
//  Description : close query public windows, free memory
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseQuerytTextWin(
                                         MMI_WIN_ID_T  *win_id_ptr
                                         );

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
/*****************************************************************************/
//  Description : open waiting win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:android���ȴ�����
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_OpenWaitingWin(
                                     MMIPUB_WAITING_PARAM_T* param
                                     );
#endif

/*****************************************************************************/
//  Description : open pubwin win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIPUB_OpenWin(
                                   MMIPUB_PARAM_T* param
                                   );

/*****************************************************************************/
//  Description : open edit win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIPUB_OpenEditWin(
//                                   MMIPUB_EDIT_PARAM_T* param
//                                   );

/*****************************************************************************/
//  Description : open list win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIPUB_OpenListWin(
//                                   MMIPUB_LIST_PARAM_T* param
//                                   );

/*****************************************************************************/
//  Description : open form win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIPUB_OpenFormWin(
//                                   MMIPUB_FORM_PARAM_T* param
//                                   );

/*****************************************************************************/
//  Description : set wait window text 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWaitWinTextEx(
                                  BOOLEAN          is_fresh,   //is fresh
                                  uint8            text_num_index,   //text number index
                                  MMI_STRING_T     *text_ptr, //text
                                  MMI_WIN_ID_T     win_id
                                  );


/*****************************************************************************/
//    Description : ��ʾ��ʾ��Ϣ�ַ���
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_DisplayTipsPtr(MMI_WIN_ID_T win_id, MMI_STRING_T* Text_Ptr);

/*****************************************************************************/
//    Description : ��ʾ��ʾ��Ϣ�ַ���
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_DisplayTipsID(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//    Description : �����ʾ��Ϣ
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_ClearTips(void);

/*****************************************************************************/
//    Description : ��ʾ��ʾ��Ϣ�ַ��� By Text ID
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_DisplayTipsIDEx(
    MMI_WIN_ID_T    *win_id_ptr,        //may NULL
    MMI_TEXT_ID_T   text_id, 
    MMIPUB_TIPS_DIS_STYLE_E dis_style,
    uint32          *time_out_ptr
    );

/*****************************************************************************/
//    Description : ��ʾ��ʾ��Ϣ�ַ��� By Text ptr
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_DisplayTipsPtrEx(
    MMI_WIN_ID_T    *win_id_ptr,    //may NULL
    MMI_STRING_T    *Text_Ptr, 
    MMIPUB_TIPS_DIS_STYLE_E dis_style, 
    uint32          *time_out_ptr
    );

/*****************************************************************************/
//    Description : �����Ƿ���ʾ�ı�־λ
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetDisTipsFlag(BOOLEAN is_dis);

/*****************************************************************************/
//    Description : ��ȡ�Ƿ���ʾ�ı�־λ
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_GetDisTipsFlag(void);

/*****************************************************************************/
//    Description : ��ʼ��ʾTips
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_StartDisTips(MMI_WIN_ID_T win_id, uint32 *time_out_ptr);

/*****************************************************************************/
//    Description : ֹͣTips��ʾ��ʱ��
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_StopDisTipsTimer(void);

/*****************************************************************************/
//  Description : set pub win add data
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_SetWinAddData(
                                    MMI_HANDLE_T win_handle,
                                    uint32       user_data
                                    );

/*****************************************************************************/
//  Description : get pub win add data
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMIPUB_INFO_T* MMIPUB_GetWinAddDataPtr(
                                              MMI_HANDLE_T win_handle
                                              );

/*****************************************************************************/
//  Description : open temp wait win, when lose_focus close
//  Global resource dependence : 
//  Author: bin.ji
//  Note: �����ô˺��������ȴ����ڣ���ǰ��ʹ�ͷŴ����ڴ�
/*****************************************************************************/
PUBLIC void MMIPUB_OpenTempWaitWin(void);

/*****************************************************************************/
//  Description : open temp wait win, and then close it.
//  Global resource dependence : 
//  Author: bin.ji
//  Note: Pop up a temp win and then close it immediatlly, normlly, it's used when some window need a
//          MSG_GET_FOCUS msg.
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAndCloseTempWin(void);

/*****************************************************************************/
//  Description : get pubwin layer
//  Global resource dependence : 
//  Author: andrew
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_GetPubWinLayer(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO* lcd_dev_info);

/*****************************************************************************/
//  Description : Set Pub Win type as MMIPUBWIN_LIST
//  Global resource dependence : 
//  Author: sam.Hua
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_SetPubListWinType(MMIPUB_INFO_T* 	win_info_ptr);	

/*****************************************************************************/
//  Description : set wait pubwin window is update delay
//  Global resource dependence : 
//  Author: hongjun.jia
//  Note: TRUE ����֧���ӳ�ˢ�£�FALSE ����֧���ӳ�ˢ��
/*****************************************************************************/
PUBLIC void MMIPUB_SetWaitWinIsupdatedelay(
                                 MMI_WIN_ID_T          win_id,
                                 BOOLEAN               is_updatedelay
                                 );

/*****************************************************************************/
//  Description : com alert note win
//  Parameter: [In] win Id
//             [In] text_id
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchPUB_Alert_Note_Win(MMI_WIN_ID_T    win_id, MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : creat and open alarm dialog window
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlarmDialogWin(
                               uint8                   text_num,       //text number
                               MMI_STRING_T            *text1_ptr,     //text 1
                               MMI_STRING_T            *text2_ptr,     //text 2
                               MMI_STRING_T            *text3_ptr,     //text 3
                               MMI_WIN_ID_T            win_id,         //win id
                               MMI_IMAGE_ID_T          image_id,       //image id
                               MMI_ANIM_ID_T           anim_id,        //anim id
                               MMI_WIN_PRIORITY_E      win_priority,   //priority
                               MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                               MMIPUB_HANDLE_FUNC      handle_func     //handle function
                               );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
