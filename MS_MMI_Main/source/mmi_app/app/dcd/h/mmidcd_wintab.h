
/***************************************************************************
** File Name:      mmidcd_wintab.h                                         *
** Author:                                                                 *
** Date:           20/09/2007                                              *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 09/2007        violent.wei      Create
****************************************************************************/
#ifndef  _MMIDCD_WINTAB_H_    
#define  _MMIDCD_WINTAB_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef MMI_ENABLE_DCD
#include "mmidcd_xml_parse.h"
#include "guilcd.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIDCD_SETTING_SELECT_DCD_OPEN,
    MMIDCD_SETTING_SELECT_CONNECT_MODE,
    /*modified by @jun.hu, 2009/7/27, begin +*/
    MMIDCD_SETTING_SELECT_SCROLL_TIME,
    MMIDCD_SETTING_SELECT_SHOW_READED,
    MMIDCD_SETTING_SELECT_MEMORY,
    MMIDCD_SETTING_SELECT_SIM_INTERET,    
    MMIDCD_SETTING_SELECT_RECOVER_FACTORY,
    /*modified by @jun.hu, 2009/7/27, end -*/
    MMIDCD_SETTING_TYPE_MAX,

} MMIDCD_SETTING_SELECT_TYPE_E;

typedef enum
{
    DCD_ITEMMENU_NODE_ROOT,     //root
    //for dcd menu
    DCD_ITEMMENU_UPDATE_START,
    DCD_ITEMMENU_UPDATE_STOP,   
    DCD_ITEMMENU_ITEM_DELETE,
    DCD_ITEMMENU_MARK,  
    DCD_ITEMMENU_SUB_MARK,
    DCD_ITEMMENU_CANCEL_MARK,
    DCD_ITEMMENU_MARK_ALL,
    DCD_ITEMMENU_CANCEL_ALL_MARK,
    DCD_ITEMMENU_ITEM_PREVIEW_LOCAL,
    DCD_ITEMMENU_CHANNEL_MANAGEMENT,
    DCD_ITEMMENU_ITEM_ADD_CHANNEL,
    DCD_ITEMMENU_ITEM_DELETE_CHANNEL,
    DCD_ITEMMENU_ITEM_PREVIEW,
    DCD_ITEMMENU_ITEM_SETTING,
    DCD_ITEMMENU_ITEM_SETTING_DCD_OPEN_SWITCH,
    DCD_ITEMMENU_ITEM_SETTING_CONNECT_MODE,
    DCD_ITEMMENU_ITEM_SETTING_SCROLL_TIME,
    DCD_ITEMMENU_ITEM_SETTING_SHOW_READED,
    DCD_ITEMMENU_ITEM_SETTING_MEMORY,
    DCD_ITEMMENU_ITEM_SETTING_RECORY_FACRORTY,
    DCD_ITEMMENU_ITEM_SETTING_HELP,
    DCD_ITEMMENU_SEND,
    DCD_ITEMMENU_SEND_BY_SMS,
    DCD_ITEMMENU_SEND_BY_MMS,
    DCD_ITEMMENU_SAVE,
    DCD_ITEMMENU_ENTER_MAINMENU,
}DCD_ITEMMENU_NODE_E;
//added by @jun.hu, 2009/7/31
typedef enum
{
     DCD_MARK_CURONE,
     DCD_MARK_ALL,
     DCD_UNMARK_CURONE,
     DCD_UNMARK_ALL,
     DCD_MARK_MAX,
}DCD_MARR_OP_E;
//dcd 动画状态....
typedef enum
{
     MMIDCD_ANIM_INIT,
     MMIDCD_ANIM_PLAY,
     MMIDCD_ANIM_STOP,
}MMIDCD_ANIM_STATE_E;

//jun.hu, 2009/8/28 把它从mmidcd_main.h中移动这里来...
typedef struct dcd_gui_struct_tag
{
    uint16 total_page_num;

    uint16 current_page_index;          /*dcd page num 绝对量*/
    uint16 current_index_in_page;       /*dcd main win 相对值 */
    uint16 current_item_num;            /*dcd main win 相对值 */
    uint16 current_real_index_in_page;	/*dcd item index in 1 page ,绝对值*/
	
    uint16 cur_idle_page_index;         
    uint16 cur_idle_realitem_index;

} DCD_GUI_STRUCT_T;

//added by @jun.hu
#define MMIDCDSET_DCD_SWITCH_MAX                   2
#define MMIDCDSET_DCD_SHOWREAD_MAX                 2
#define MMIDCDSET_DCD_SIMSWITCH_MAX                 2

#define SECOND_PER_MICROSECOND_POWER              1000
#define MMIDCD_FEED_TITLE_LEN                      64

//更新的动画控件的相应坐标...
#ifdef MAINLCD_SIZE_176X220
#define  MMIDCD_ANIMATION_WIDTH                    23
#define  MMIDCD_ANIMATION_HEIGHT                   21
#else
#define  MMIDCD_ANIMATION_WIDTH                    28
#define  MMIDCD_ANIMATION_HEIGHT                   24
#endif

#define  MMIDCD_ANIMATION_TOP                      ((MMI_TITLE_HEIGHT - MMIDCD_ANIMATION_HEIGHT)/2)
#define  MMIDCD_ANIMATION_RIGHT                    (40)
#define  MMIDCD_ANIMATION_LEFT                     (5)
#define  MMIDCD_ITEM_PICTURE_WIDTH                 (60)
#define  MMIDCD_ITEM_PICTURE_HIGHT                 (60)
/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DECLARE                          *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetDcdCurrentPageIndex();
/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_SetDcdCurrentPageIndex(uint16  current_page_index,uint16 current_real_index_in_page);

/*****************************************************************************/
//  Description :  Father windows
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_OpenMainWin(BOOLEAN openMainmenu);
/*****************************************************************************/
//  Description : appen one item text and anim
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/7
/*****************************************************************************/
PUBLIC void MMIDCD_IdleAppendOneTabPage(
                                        GUI_POINT_T      point,                                                              
                                        uint16           item_index  //in:item索引
                                        );
/*****************************************************************************/
//  Description : to set dcd list item in 1 page
//  Global resource dependence : 
//  Author: @jun.hu
//  Note:  2009/8/7
/*****************************************************************************/
PUBLIC void MMIDCD_SetTabListItem(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//  Author: jun.hu 
//  Note: 2009/8/28
/*****************************************************************************/
PUBLIC void MMIDCD_UpdateFrame(BOOLEAN is_last_update);
/*****************************************************************************/
//  Description : draw addation text in window title on right conner 
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
PUBLIC void MMIDCD_UpdateIdleWin(void );

/*****************************************************************************/
//  Description : appen one item text and anim
//  Global resource dependence : 
//  Author: 
//  Note: jun.hu 2009/12/30 改造直接传给底层图片数据
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_AppendOneTabPage(
                                       MMI_CTRL_ID_T    ctrl_id,         //in:control id
                                       GUI_POINT_T      point,                                                              
                                       uint16           item_index     //in:item索引
                                       );
/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDCD_GetFocusCtrlRect(void);

/*****************************************************************************/
//  Description : set dcd page item bg
//  Global resource dependence : none
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC void  DCD_SetPageFocusBgImage(BOOLEAN is_active);
/*****************************************************************************/
//  Description : 设置DCD动画控件......
//  Global resource dependence :
//  Author: jun.hu
//  Note:   2009/9/17 state 更新动画当前需要设定的状态:1,动画;2,静止
///*****************************************************************************/
PUBLIC void MMIDCD_SetAnimateCtrlState(MMIDCD_ANIM_STATE_E state);
/*****************************************************************************/
//  Description : DCD 最低优先级提醒~
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IndicationShow( MMIDCD_REQ_TYPE_E req_type, MMIDCD_ERROR_CODE_E  error_code, BOOLEAN is_enble_show);
/*****************************************************************************/
// 	Description : mmi dcd update start
//	Global resource dependence : none
//  Author: jun.hu
//	Note: 2009/10/10, 增加此函数是因为原来wintab.c 中直接调用MMIDCD_UpdateStart
//        有多处，但这个函数没起动画，为统一控制，写此函数专供mmi调用...
//        10/26 把此函数升级为public ,供所有发起更新的地方调用....
/*****************************************************************************/
PUBLIC void MMIDCD_UpdateStart(MMIDCD_REQ_TYPE_E req_type);

/*****************************************************************************/
//  Description : DCD  wake up indication~
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_IdleWinWakeupInd(MMI_WIN_ID_T win_id );

/*****************************************************************************/
//  Description : DCD  wake up indication~
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_Wakeup(void );
/*****************************************************************************/
//  Description : inform this function finished 
//  Global resource dependence : none
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_SetResetFinishedInd(void);

/*****************************************************************************/
// 	Description : handle idle win message
//	Global resource dependence : 
//  Author: bin.ji
//  Date: 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_HandleIdleWinMessage(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : DCD  stop udisk ,need update~
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_StopUdiskUpdateIdleWin(void );
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:violent.wei
//  Note: jun.hu 09/8/27 把它从mmidcd_main.c中移动到这里
/*****************************************************************************/
PUBLIC void MMIDCD_SetData(void);
/*****************************************************************************/
//  Description : open input homepage window of dcd
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_OpenMainWinFromIdle(BOOLEAN openMainmenu);
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIDCD_AppendChildWin(MMI_WIN_ID_T    win_id);
/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_CreateIdleCtrl(MMI_WIN_ID_T win_id );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_OpenLinkListWin(void);
/*****************************************************************************/
//  Description : 处理DCD开关的函数
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 请统一调用此函数处理DCD的开关操作
/*****************************************************************************/
PUBLIC void MMIDCD_HandleDCDSwitch(
                                   BOOLEAN to_Flag,  // 开关DCD
                                   BOOLEAN is_reset_factory // 是否是恢复出厂设置
                                   );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 此函数专供手动更新成功时，提示用户更新完成, 其他如手动更新失败，
//        zhuo 的indiciotionshow 处理, 其他模式非手动更新，不需提示
/*****************************************************************************/
PUBLIC void MMIDCD_ShowUpdateComplete(
                                      MMIDCD_REQ_TYPE_E    req_type, 
                                      MMIDCD_ERROR_CODE_E  error_code
                                      );
/*****************************************************************************/
//  Description : register DCD module applet info
//  Global resource dependence : none
// Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_RegAppletInfo(void);

/*****************************************************************************/
//  Description : 设置更新取消菜单是否显示
//  Global resource dependence : 
//  Author:koki.gao
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_SetUpdateStopFlag(BOOLEAN flag);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:koki gao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_GetUpdateStopFlag(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_ResetDCDStart(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
#endif //MMI_ENABLE_DCD
#endif

