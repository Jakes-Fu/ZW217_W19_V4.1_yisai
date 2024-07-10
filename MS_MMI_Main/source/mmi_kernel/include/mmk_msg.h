/******************************************************************************
** File Name:      mmk_msg.h                                                  *
** Author:                                                                    *
** Date:           03/11/2003                                                    *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe the data struct of           *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2003       Louis.wei         Create                                        *
******************************************************************************/

/*!
 *  \addtogroup mmk_message_group_label
 *  @{
 */


/*! 
 *  \file     mmk_msg.h
 *  \date    2006
 *  \author louis.wei
 *  \brief     This file is used to describe mmi message.
 */

/*!
 * mmk_msg.h会被mmk_app.c包含, 如果用户修改需要重新编译mmk_app.c, @liqing.peng, 08-06-26
 */


#ifndef  _MMK_MSG_H_    
#define _MMK_MSG_H_   

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/ 
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/ 
#include "mmk_type.h"
#include "mmi_signal.h"
#include "mmi_osbridge.h"        

/*----------------------------------------------------------------------------*/ 
/*                         Type Declaration                                   */
/*----------------------------------------------------------------------------*/ 
//0x00~0xff 内部控件消息, 设计成熟的通知消息, 可以拿到caf.h中
/*
   notify消息开放的原则:
   1. 该消息设计的比较合理
   2. 该消息对应的notify消息结构, 必须包含CAF_NOTIFY_T的基类结构, 并定义在对应控件的cafctrlxxx.h中
*/
//common notify
#define     MSG_NOTIFY_NONE                             0x00                //空消息

#define     MSG_NOTIFY_RESIZE                           0x01
#define     MSG_NOTIFY_UPDATE                           0x02
#define     MSG_NOTIFY_GET_ACTIVE                       0x03
#define     MSG_NOTIFY_LOSE_ACTIVE                      0x04
#define     MSG_NOTIFY_SELECT_CHANGE                    0x05
#define     MSG_NOTIFY_SET_SOFTKEY                      0x06

//clipboard menu notify
#define     MSG_NOTIFY_CLIPBRD_CUT                      0x10
#define     MSG_NOTIFY_CLIPBRD_COPY                     0x11
#define     MSG_NOTIFY_CLIPBRD_PASTE                    0x12
#define     MSG_NOTIFY_CLIPBRD_SELECTALL                0x13
#define     MSG_NOTIFY_CLIPBRD_MENU_CLOSED              0x14
#define     MSG_NOTIFY_CLIPBRD_SELECT                   0x15
#define     MSG_NOTIFY_BEGIN_SLIDER                     0x16
#define     MSG_NOTIFY_MOVE_SLIDER                      0x17
#define     MSG_NOTIFY_END_SLIDER                       0x18

// menu
#define     MSG_NOTIFY_MENU_SET_STATUS                  0x20
#define     MSG_NOTIFY_GRID_SHORTCUT2IDLE               0x21                //add for grid idle.
#define     MSG_NOTIFY_QBTHEME_SHORTCUT2IDLE            0x22                //add for qbtheme idle.
#define     MSG_NOTIFY_MENU_INFO						0x23                //for menu

// form
#define     MSG_NOTIFY_HIGHLIGHT                        0x27

//edit
#define     MSG_NOTIFY_LINE_CHANGE                      0x30

// anim
#define     MSG_CTL_ANIM_GET_DATA_CNF                   0x37

// status bar
#define     MSG_CTL_STATUSBAR_UPDATE                    0x40

// setting list
#define     MSG_CTL_SETLIST_SET                         0x47

//pub win
#define     MSG_PUBMENU_OPEN                            0x50                //打开
#define     MSG_PUBMENU_OK                              0x51                //确认
#define     MSG_PUBMENU_CANCEL                          0x52                //返回
#define     MSG_PUBMENU_CLOSE                           0x53                //关闭, 占位, 暂时不发

#define     MSG_PUBEDIT_OPEN                            0x54                //打开
#define     MSG_PUBEDIT_OK                              0x55                //确认
#define     MSG_PUBEDIT_CANCEL                          0x56                //返回
#define     MSG_PUBEDIT_CLOSE                           0x57                //关闭, 占位, 暂时不发

#define     MSG_PUBTEXT_OPEN                            0x58                //打开
#define     MSG_PUBTEXT_OK                              0x59                //确认
#define     MSG_PUBTEXT_CANCEL                          0x5a                //返回
#define     MSG_PUBTEXT_CLOSE                           0x5b                //关闭, 占位, 暂时不发

#define     MSG_PUBRICHTEXT_OPEN                        0x5c                //打开
#define     MSG_PUBRICHTEXT_OK                          0x5d                //确认
#define     MSG_PUBRICHTEXT_CANCEL                      0x5e                //返回
#define     MSG_PUBRICHTEXT_CLOSE                       0x5f                //关闭, 占位, 暂时不发

#define     MSG_PUBLIST_OPEN                            0x60                //打开
#define     MSG_PUBLIST_OK                              0x61                //确认
#define     MSG_PUBLIST_CANCEL                          0x62                //返回
#define     MSG_PUBLIST_CLOSE                           0x63                //关闭, 占位, 暂时不发

#define     MSG_ALERTWIN_CLOSED                         0x64
#define     MSG_TIPSWIN_CLOSED                          0x65
#define     MSG_PROMPTWIN_OK                            MSG_MESSAGEBOX_OK
#define     MSG_PROMPTWIN_CANCEL                        MSG_MESSAGEBOX_CANCEL

//im notify    
#define     MSG_NOTIFY_IM_START                         0x70
#define     MSG_NOTIFY_IM_COMMIT                        MSG_NOTIFY_IM_START
#define     MSG_NOTIFY_IM_BACKSPACE                     (MSG_NOTIFY_IM_START+1)
#define     MSG_NOTIFY_IM_PAINT                         (MSG_NOTIFY_IM_START+2)
#define     MSG_NOTIFY_IM_SWICTH                        (MSG_NOTIFY_IM_START+3)
#define     MSG_NOTIFY_IM_SWICTH_MODE                   (MSG_NOTIFY_IM_START+4)
#define     MSG_NOTIFY_IM_CHANGE_RECT                   (MSG_NOTIFY_IM_START+5)
#define     MSG_NOTIFY_IM_REFRESH_SOFTKEY               (MSG_NOTIFY_IM_START+6)
#define     MSG_NOTIFY_IM_LONG_DEL                      (MSG_NOTIFY_IM_START+7)
#define     MSG_NOTIFY_IM_HIDE                          (MSG_NOTIFY_IM_START+8)
#define     MSG_NOTIFY_IM_SETCURSOR                     (MSG_NOTIFY_IM_START+9)
#define     MSG_NOTIFY_IM_GETCURSOR                     (MSG_NOTIFY_IM_START+10)
#define     MSG_NOTIFY_IM_SET_HIGHLIGHT                 (MSG_NOTIFY_IM_START+11)

//输入法的一个特殊消息
#define     MSG_T9_HANDWRITING_ENTRY                    0x80
#define     MSG_T9_HANDWRITING_EXIT                     0x81
#define     MSG_T9_SET_PUNCTUATION                      0x82
#define     MSG_T9_SET_INPUTMETHOD                      0x83
#define     MSG_T9_PANEL_OPERATION                      0x84              // 触笔软键盘应用处理


#define     MSG_SK_SKB_ICON_PRESSED                     0x90              //当点击softkey bar 中的图标，发送此消息，参数是index

#ifdef MEDIA_DEMO_SUPPORT
#define     MSG_MEDIA_DEMO_APP_UP                       0xa0
#define     MSG_MEDIA_DEMO_APP_DOWN                     0xa1
#define     MSG_MEDIA_DEMO_SHOW_DATE                    0xa2
#endif

// tp move notify message
#define     MSG_NOTIFY_TPMOVE_LEFT_SIDE                 0xb0            // tp移动到lcd左侧
#define     MSG_NOTIFY_TPMOVE_RIGHT_SIDE                0xb1            // tp移动到lcd右侧
#define     MSG_NOTIFY_TPMOVE_TOP_SIDE                  0xb2            // tp移动到lcd上侧
#define     MSG_NOTIFY_TPMOVE_END_SIDE                  0xb3            // tp移动到lcd下侧
#define     MSG_NOTIFY_TPCLICK_LEFT_ICON                0xb4            // tp移动到item slide 第一个图标
#define     MSG_NOTIFY_TPCLICK_RIGHT_ICON               0xb5            // tp移动到item slide 第二个图标

//仅仅保持兼容
#define     MSG_CTL_OK                                  MSG_NOTIFY_OK
#define     MSG_CTL_CANCEL                              MSG_NOTIFY_CANCEL
#define     MSG_CTL_PENOK                               MSG_NOTIFY_PENOK
#define     MSG_CTL_MIDSK                               MSG_NOTIFY_MIDSK
#define     MSG_CTL_PENLONGOK                           MSG_NOTIFY_PENLONGOK

#define     MSG_CTL_LIST_PREPAGE                        MSG_NOTIFY_LIST_PREPAGE
#define     MSG_CTL_LIST_NXTPAGE                        MSG_NOTIFY_LIST_NXTPAGE
#define     MSG_CTL_LIST_NEED_ITEM_DATA                 MSG_NOTIFY_LIST_NEED_ITEM_DATA
#define     MSG_CTL_LIST_NEED_ITEM_CONTENT              MSG_NOTIFY_LIST_NEED_ITEM_CONTENT
#define     MSG_CTL_LIST_ITEM_CONTENT_CLICK             MSG_NOTIFY_LIST_ITEM_CONTENT_CLICK
#define     MSG_CTL_LIST_MOVEBOTTOM                     MSG_NOTIFY_LIST_MOVEBOTTOM        
#define     MSG_CTL_LIST_MOVETOP                        MSG_NOTIFY_LIST_MOVETOP
#define     MSG_CTL_LIST_MOVEDOWN                       MSG_NOTIFY_LIST_MOVEDOWN        
#define     MSG_CTL_LIST_MOVEUP                         MSG_NOTIFY_LIST_MOVEUP  
#define     MSG_CTL_LIST_SELECTEDALL                    MSG_NOTIFY_LIST_SELECTEDALL
#define     MSG_CTL_LIST_MARK_ITEM                      MSG_NOTIFY_LIST_MARK_ITEM
#define     MSG_CTL_LIST_SLIDE_STATE_CHANGE             MSG_NOTIFY_LIST_SLIDE_STATE_CHANGE
#define     MSG_CTL_LIST_LONGOK                         MSG_NOTIFY_LIST_LONGOK
#define     MSG_CTL_LIST_LONGPRESS                      MSG_NOTIFY_LIST_LONGOK  //"长按"msg
#define     MSG_CTL_LIST_CHECK_CONTENT                  MSG_NOTIFY_LIST_CHECK_CONTENT
#define     MSG_CTL_LIST_ADD_CONTEN                     MSG_NOTIFY_LIST_ADD_CONTEN
#define     MSG_CTL_LIST_DEL_CONTEN                     MSG_NOTIFY_LIST_DEL_CONTEN
#define     MSG_CTL_LIST_SET_SELECT						MSG_NOTIFY_LIST_SET_SELECT
#define     MSG_CTL_LIST_SELECT_QUERY					MSG_NOTIFY_LIST_SELECT_QUERY
#define     MSG_CTL_LIST_CHECKED						MSG_NOTIFY_LIST_CHECKED // radio or check type, button checked

#define     MSG_CTL_PRGBOX_MOVEUP_ONE                   MSG_NOTIFY_PRGBOX_MOVEUP_ONE
#define     MSG_CTL_PRGBOX_MOVEDOWN_ONE                 MSG_NOTIFY_PRGBOX_MOVEDOWN_ONE
#define     MSG_CTL_PRGBOX_MOVEUP_PAGE                  MSG_NOTIFY_PRGBOX_MOVEUP_PAGE
#define     MSG_CTL_PRGBOX_MOVEDOWN_PAGE                MSG_NOTIFY_PRGBOX_MOVEDOWN_PAGE
#define     MSG_CTL_PRGBOX_MOVE                         MSG_NOTIFY_PRGBOX_MOVE
#define     MSG_CTL_PRGBOX_CLICK                        MSG_NOTIFY_PRGBOX_CLICK

#define     MSG_CTL_ANIM_DISPLAY_IND                    MSG_NOTIFY_ANIM_DISPLAY_IND
#define     MSG_CTL_ANIM_GIF_UPDATE                     MSG_NOTIFY_ANIM_GIF_UPDATE

#define     MSG_CTL_ICONLIST_APPEND_TEXT                MSG_NOTIFY_ICONLIST_APPEND_TEXT
#define     MSG_CTL_ICONLIST_APPEND_ICON                MSG_NOTIFY_ICONLIST_APPEND_ICON
#define     MSG_CTL_ICONLIST_ICON_NUM                   MSG_NOTIFY_ICONLIST_ICON_NUM
#define     MSG_CTL_ICONLIST_MARK_ITEM                  MSG_NOTIFY_ICONLIST_MARK_ITEM
#define     MSG_CTL_ICONFOLDER_APPEND_ICON              MSG_NOTIFY_ICONFOLDER_APPEND_ICON

#define     MSG_CTL_TAB_SWITCH                          MSG_NOTIFY_TAB_SWITCH
#define     MSG_CTL_TAB_START                           MSG_NOTIFY_TAB_START
#define     MSG_CTL_TAB_END                             MSG_NOTIFY_TAB_END
#define     MSG_CTL_TAB_NEED_ITEM_CONTENT               MSG_NOTIFY_TAB_NEED_ITEM_CONTENT  

#define     MSG_CTL_EDITBOX_UPDATE_STRNUM               MSG_NOTIFY_EDITBOX_UPDATE_STRNUM

#define     MSG_CTL_DROPDOWNLIST_SELECTED               MSG_NOTIFY_DROPDOWNLIST_SELECTED
#define     MSG_CTL_DROPDOWNLIST_PRE_PROCESS            MSG_NOTIFY_DROPDOWNLIST_PRE_PROCESS

#define     MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY         MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_KEY
#define     MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP          MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_TP

// title
#define     MSG_CTL_TITLE_MOVE_NEXT                     MSG_NOTIFY_TITLE_MOVE_NEXT
#define     MSG_CTL_TITLE_MOVE_PREV                     MSG_NOTIFY_TITLE_MOVE_PREV

// tips msg
#define     MSG_CTL_TIPS_NEED_DATA                      MSG_NOTIFY_TIPS_NEED_DATA

// setting list
#define     MSG_CTL_SETLIST_SWITCH                      MSG_NOTIFY_SETLIST_SWITCH
#define     MSG_CTL_SETLIST_OPENWIN                     MSG_NOTIFY_SETLIST_OPENWIN

//button tp press msg
#define     MSG_CTL_BUTTON_DOWN                         MSG_NOTIFY_BUTTON_DOWN              
#define     MSG_CTL_BUTTON_UP                           MSG_NOTIFY_BUTTON_UP                
//仅仅保持兼容

// slider notify msg
#define     MSG_CTRL_SLIDER_CHANGED                     MSG_NOTIFY_SLIDER_CHANGED

// slider notify msg
#define     MSG_CTRL_LETTER_CHANGED                     MSG_NOTIFY_LETTER_CHANGED

#define     MSG_APP_UP                                  MSG_KEYDOWN_UP
#define     MSG_APP_DOWN                                MSG_KEYDOWN_DOWN
#define     MSG_APP_LEFT                                MSG_KEYDOWN_LEFT
#define     MSG_APP_RIGHT                               MSG_KEYDOWN_RIGHT
#define     MSG_APP_GREEN                               MSG_KEYDOWN_GREEN
#define     MSG_APP_CAMERA                              MSG_KEYDOWN_CAMERA
#define     MSG_APP_1                                   MSG_KEYDOWN_1
#define     MSG_APP_2                                   MSG_KEYDOWN_2
#define     MSG_APP_3                                   MSG_KEYDOWN_3
#define     MSG_APP_4                                   MSG_KEYDOWN_4
#define     MSG_APP_5                                   MSG_KEYDOWN_5
#define     MSG_APP_6                                   MSG_KEYDOWN_6
#define     MSG_APP_7                                   MSG_KEYDOWN_7
#define     MSG_APP_8                                   MSG_KEYDOWN_8
#define     MSG_APP_9                                   MSG_KEYDOWN_9
#define     MSG_APP_0                                   MSG_KEYDOWN_0
#define     MSG_APP_HASH                                MSG_KEYDOWN_HASH
#define     MSG_APP_STAR                                MSG_KEYDOWN_STAR
#define     MSG_APP_OK                                  MSG_KEYDOWN_OK
#define     MSG_APP_MENU                                MSG_KEYDOWN_MENU
#define     MSG_APP_CANCEL                              MSG_KEYDOWN_CANCEL
#define     MSG_APP_RED                                 MSG_KEYDOWN_RED
#define     MSG_APP_UPSIDE                              MSG_KEYDOWN_UPSIDE
#define     MSG_APP_DOWNSIDE                            MSG_KEYDOWN_DOWNSIDE

#define     MSG_APP_WEB                                 MSG_KEYDOWN_WEB    
#define     MSG_APP_FLIP                                MSG_KEYDOWN_FLIP    
#define     MSG_APP_HEADSET_BUTTON                      MSG_KEYDOWN_HEADSET_BUTTON    
#define     MSG_APP_HEADSET_DETECT                      MSG_KEYDOWN_HEADSET_DETECT    

#define     MSG_APP_AT                                  MSG_KEYDOWN_AT
#define     MSG_APP_VIDEO_TEL                           MSG_KEYDOWN_VIDEO_TEL
#define     MSG_APP_PLAYANDSTOP                         MSG_KEYDOWN_PLAYANDSTOP
#define     MSG_APP_FORWARD                             MSG_KEYDOWN_FORWARD
#define     MSG_APP_BACKWARD                            MSG_KEYDOWN_BACKWARD

#define     MSG_APP_Q                                   MSG_KEYDOWN_Q
#define     MSG_APP_W                                   MSG_KEYDOWN_W
#define     MSG_APP_E                                   MSG_KEYDOWN_E
#define     MSG_APP_R                                   MSG_KEYDOWN_R
#define     MSG_APP_T                                   MSG_KEYDOWN_T
#define     MSG_APP_Y                                   MSG_KEYDOWN_Y
#define     MSG_APP_U                                   MSG_KEYDOWN_U
#define     MSG_APP_I                                   MSG_KEYDOWN_I
#define     MSG_APP_O                                   MSG_KEYDOWN_O
#define     MSG_APP_P                                   MSG_KEYDOWN_P
#define     MSG_APP_A                                   MSG_KEYDOWN_A
#define     MSG_APP_S                                   MSG_KEYDOWN_S
#define     MSG_APP_D                                   MSG_KEYDOWN_D
#define     MSG_APP_F                                   MSG_KEYDOWN_F
#define     MSG_APP_G                                   MSG_KEYDOWN_G
#define     MSG_APP_H                                   MSG_KEYDOWN_H
#define     MSG_APP_J                                   MSG_KEYDOWN_J
#define     MSG_APP_K                                   MSG_KEYDOWN_K
#define     MSG_APP_L                                   MSG_KEYDOWN_L
#define     MSG_APP_DEL                                 MSG_KEYDOWN_DEL
#define     MSG_APP_Z                                   MSG_KEYDOWN_Z
#define     MSG_APP_X                                   MSG_KEYDOWN_X
#define     MSG_APP_C                                   MSG_KEYDOWN_C
#define     MSG_APP_V                                   MSG_KEYDOWN_V
#define     MSG_APP_B                                   MSG_KEYDOWN_B
#define     MSG_APP_N                                   MSG_KEYDOWN_N
#define     MSG_APP_M                                   MSG_KEYDOWN_M
#define     MSG_APP_COMMA                               MSG_KEYDOWN_COMMA
#define     MSG_APP_PERIOD                              MSG_KEYDOWN_PERIOD
#define     MSG_APP_ENTER                               MSG_KEYDOWN_ENTER
#define     MSG_APP_FN                                  MSG_KEYDOWN_FN
#define     MSG_APP_SHIFT                               MSG_KEYDOWN_SHIFT
#define     MSG_APP_AT_QWERTY                           MSG_KEYDOWN_AT_QWERTY
#define     MSG_APP_SPACE                               MSG_KEYDOWN_SPACE
#define     MSG_APP_AND                                 MSG_KEYDOWN_AND
#define     MSG_APP_QUESTION                            MSG_KEYDOWN_QUESTION
#define     MSG_APP_CTRL                                MSG_KEYDOWN_CTRL

#define     MSG_APP_PLUS                                MSG_KEYDOWN_PLUS 
#define     MSG_APP_LPARENTHESIS                        MSG_KEYDOWN_LPARENTHESIS 
#define     MSG_APP_RPARENTHESIS                        MSG_KEYDOWN_RPARENTHESIS 
#define     MSG_APP_MINUS                               MSG_KEYDOWN_MINUS 
#define     MSG_APP_DQUOTES                             MSG_KEYDOWN_DQUOTES 
#define     MSG_APP_SEMICOLON                           MSG_KEYDOWN_SEMICOLON 
#define     MSG_APP_COLON                               MSG_KEYDOWN_COLON 
#define     MSG_APP_SLASH                               MSG_KEYDOWN_SLASH 
#define     MSG_APP_SAND                                MSG_KEYDOWN_SAND 
#define     MSG_APP_EXCLAMATION                         MSG_KEYDOWN_EXCLAMATION 

#define     MSG_APP_SLIDE                               MSG_KEYDOWN_SLIDE
#define     MSG_APP_TV                                  MSG_KEYDOWN_TV
#define     MSG_APP_CALL1                               MSG_KEYDOWN_CALL1
#define     MSG_APP_CALL2                               MSG_KEYDOWN_CALL2
#define     MSG_APP_CALL3                               MSG_KEYDOWN_CALL3
#define     MSG_APP_CALL4                               MSG_KEYDOWN_CALL4
#define     MSG_APP_MP3                                 MSG_KEYDOWN_MP3
#define     MSG_APP_NOTES                               MSG_KEYDOWN_NOTES
#define     MSG_APP_SMS                                 MSG_KEYDOWN_SMS
#define     MSG_APP_CALENDER                            MSG_KEYDOWN_CALENDER
#define     MSG_APP_IE                                  MSG_KEYDOWN_IE
#define     MSG_APP_HANG                                MSG_KEYDOWN_HANG
#define     MSG_APP_VOL_UP                              MSG_KEYDOWN_VOL_UP
#define     MSG_APP_VOL_DOWN                            MSG_KEYDOWN_VOL_DOWN
#define     MSG_APP_MO                                  MSG_KEYDOWN_MO
#define     MSG_APP_POWER                               MSG_KEYDOWN_POWER

#ifdef GUIF_SCROLLKEY
#define     MSG_APP_LSKOK                               MSG_TP_LSKOK
#define     MSG_APP_RSKOK                               MSG_TP_RSKOK
#endif

/*! \brief define the struct of register ps/ref message */
typedef struct  RegAppSig_tag
{
    uint16                      start_sig;          /*!< the first signal of application to handle */
    uint16                      end_sig;            /*!< the last signal of application to handle */
    PWND                        app_ptr;            /*!< the pointer of application */
}REGISTER_APP_SIG_T;



/*! \brief Define the enum of message id */
typedef enum
{
    MSG_TYPE_NONE = 0x00,                           /*!< no message */
    //MSG_TYPE_STACK,                                 /*!< the protocol message */
    //MSG_TYPE_TIMER,                                 /*!< the timer message */
    //MSG_TYPE_FOCUS,                                 /*!< the focus message */
    MSG_TYPE_BROADCAST,                             /*!< the broadcast message */
    MSG_TYPE_WINDOW,                                 /*!< the window message */
#ifdef WRE_SUPPORT
    MSG_TYPE_WRE
#endif
}MMI_MESSAGE_TYPE_E;



/*! \brief Define the struct of message */
typedef struct Message_tag
{
    MMI_HANDLE_T                handle;                /*!< the pointer of window which receive the msg */
    MMI_MESSAGE_TYPE_E            msg_type;           /*!< the type of message */
    MMI_MESSAGE_ID_E            msg_id;             /*!< the identification of message */
    DPARAM                        param_ptr;          /*!< the param  pointer of message */
}MMI_MESSAGE_T;

typedef MMI_MESSAGE_T*  MMI_MESSAGE_PTR_T;

//delete msg call back
typedef BOOLEAN (*MMK_DEL_MSG_PFUNC)(void *in_ptr,MMI_MESSAGE_T  *msg_ptr);

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/*! \brief init the message queue
 *  \param[in]        msgQueue_len                the length of message queue
 *  \author  Louis wei
 */
/******************************************************************************/
void MMK_InitMSGQueue(uint8 msgQueue_len);
                 
                 

/******************************************************************************/
/*! \brief get the message queue
 *  \param[out]        *mmi_msg_ptr        the pointer of message
 *  \author  Louis wei
 *    \return  BOOLEAN
 */
/******************************************************************************/
BOOLEAN MMK_GetMSGQueue(MMI_MESSAGE_PTR_T  *mmi_msg_ptr);




/******************************************************************************/
/*! \brief dispatch the message which get from message queue
 *  \param[in]        mmi_msg_ptr        the pointer of message which need dispatch
 *  \author  Louis wei
 */
/******************************************************************************/
void MMK_DispatchMSGQueue(MMI_MESSAGE_PTR_T mmi_msg_ptr);



/******************************************************************************/
/*! \brief destroy the space of message 
 *  \param[in]        mmi_msg_ptr        the pointer of message which need destroy
 *  \author  Louis wei
 */
/******************************************************************************/
void MMK_FreeMSG(MMI_MESSAGE_PTR_T  mmi_msg_ptr);




/******************************************************************************/
/*! \brief dispatch the external signal 
 *  \param[in]        signal_ptr        the pointer of signal which need dispatch
 *  \author  Louis wei
 */
/******************************************************************************/
void MMK_DispatchExtSig(MmiSignalS** signal_ptr);

/******************************************************************************/
/*! \brief post the broadcast  message to the window
 *  \param[in]        msg_id            message id
 *  \param[in]        param_ptr        message data pointer
 *  \param[in]        size_of_param    message data size
 *  \author  Louis wei
 *    \return  BOOLEAN
 */
/******************************************************************************/
BOOLEAN MMK_PostBCMessage(
                          MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                          DPARAM              param_ptr,      //the param of the message
                          uint32              size_of_param   //the size of the param
                          );
/*****************************************************************************/
/*! \brief  dispatch the window message to focus window
 *  \param[in]        msg_id            message id
 *  \param[in]        param_ptr        message data pointer
 *  \author  Louis wei
 *    \return  BOOLEAN
 */
/*****************************************************************************/
BOOLEAN  MMK_DispMsgToFocusWin(
                                MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                                DPARAM              param_ptr   //the param of the message
                                );

/*****************************************************************************/
//     Description : dispatch the window message to focus window
//    Global resource dependence : ;
//  Author:Louis wei
//    Note:
/*****************************************************************************/
BOOLEAN  MMK_DispMsgToWin(
                          MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                          DPARAM              param_ptr   //the param of the message
                          );

/******************************************************************************/
/*! \brief delete the message in the message quene
 *  \param[in]        msg_id            message id
 *  \param[in]        param_ptr        window or control pointer
 *  \author  Great.Tian
 *    \return  void
 */
/******************************************************************************/
PUBLIC void MMK_DeleteMSGByHwndAndMsg(
                                      MMI_HANDLE_T handle,
                                      MMI_MESSAGE_ID_E msg_id //[IN] message id
                                      );

/*****************************************************************************/
//     Description : 删除队列中对应的消息
//    Global resource dependence : 
//  Author: Jassmine
//    Note:
/*****************************************************************************/
PUBLIC void MMK_DeleteMsgByCallback(
                                    void                *in_param_ptr,      //in:
                                    MMK_DEL_MSG_PFUNC   is_delete_callback  //in:
                                    );

/*****************************************************************************/
//  Description : dispatch the tp short long msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchMSGTpShort(
                                   uint16   x,
                                   uint16   y
                                   );

/*****************************************************************************/
//  Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//  Global resource dependence : 
//  Author:Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchMSGTpLong(
                                  uint16 x,
                                  uint16 y
                                  );

/*****************************************************************************/
// Description : 
//  Global resource dependence : 
//  Author:Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMK_ReInitDblClkState(
								  BOOLEAN is_reset
								  );

/*****************************************************************************/
//     Description : send the synchronous message to the pointed control or window
//    Global resource dependence : 
//  Author: Jassmine
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SendMsg(
                           MMI_HANDLE_T     handle,
                           MMI_MESSAGE_ID_E msg_id,
                           DPARAM           param_ptr
                           );

/*****************************************************************************/
//     Description : send the asynchronous message to the pointed control or window
//    Global resource dependence : 
//  Author: Jassmine
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_PostMsg(
                           MMI_HANDLE_T     handle,         // the handle
                           MMI_MESSAGE_ID_E msg_id,         //the identify of message
                           DPARAM           param_ptr,      //the param of the message
                           uint32           size_of_param   //the size of the param
                           );

/*****************************************************************************/
//     Description : dispatch the window message 
//    Global resource dependence : ;
//  Author:Louis wei
//  Modify:wei.zhou, adjust local api to public.
//    Note:
/*****************************************************************************/
void MMK_DispatchWinMSG(
                          MMI_MESSAGE_PTR_T mmi_msg_ptr // the pointer of message which need dispatch
                          );
/*****************************************************************************/
//     Description : dispatch the broadcast message to the opend window
//    Global resource dependence : 
//  Author:Louis wei
//  Modify:wei.zhou, adjust local api to public.
//    Note:
/*****************************************************************************/
void MMK_DispatchBCMSG(
                          MMI_MESSAGE_PTR_T mmi_msg_ptr // the pointer of message which need dispatch
                          );


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_MSG_H_  */
/*! @} */ 
