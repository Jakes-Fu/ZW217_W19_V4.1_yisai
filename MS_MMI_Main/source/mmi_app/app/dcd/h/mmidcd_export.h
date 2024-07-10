
/*****************************************************************************
** File Name:      mmidcd_export.h                                           *
** Author:                                                                   *
** Date:           10/21/2008                                                *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/7/2005       bin.ji       Create
******************************************************************************/

#ifndef _MMIDCD_EXPORT_H_
#define _MMIDCD_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmidcd_xml_parse.h"
#include "mmidcd_setting.h"
#include "mmk_type.h"
#include "mn_type.h"
#include "sig_code.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmifmm_export.h"
#include "guilcd.h"
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
#define DCD_MSG_GROUP (SIGNAL_GROUP_MAX + 2)
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

typedef enum _MSG_DCD
{
    MSG_DCD_BEGIN = (( DCD_MSG_GROUP << 8 ) | 1 ),

    /*add by wz*/
    DCD_SIG_INIT_REQ,
    DCD_SIG_INIT_CNF,
    DCD_SIG_PDP_ACTIVED_CNF,
    DCD_SIG_PDP_DEACTIVED_CNF,
    DCD_SIG_DL_CANCLE_REQ,
    DCD_SIG_XML_DL_REQ,
    DCD_SIG_XML_DL_CNF,    
    DCD_SIG_MIME_DL_REQ,
    DCD_SIG_MIME_DL_CNF,
    DCD_SIG_DL_OVER_IND,    
    DCD_SIG_DEFALUT_RETRY_TIMER_IND,
    DCD_SIG_ERROR_IND,
    
    /*end by wz*/

    MSG_DCD_END
} MSG_DCD_E;


/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIDCD_DcdIsOpen            MMIAPIDCD_DcdIsOpen
#define MMIDCD_MAIN_GetNetHomePage  MMIAPIDCD_MAIN_GetNetHomePage
#define MMIDCD_GetXmlDoc_Ptr        MMIAPIDCD_GetXmlDoc_Ptr
#define MMIDCD_GetFormalFeedNum     MMIAPIDCD_GetFormalFeedNum
#define MMIDCDFILE_GetFeedUseableEntryCount MMIAPIDCDFILE_GetFeedUseableEntryCount
#define MMIDCD_GetUseableItemFromIndex      MMIAPIDCD_GetUseableItemFromIndex
#define MMIDCD_GetFeedContentInfo   MMIAPIDCD_GetFeedContentInfo
#define MMIDCD_GetEntryContentInfo  MMIAPIDCD_GetEntryContentInfo
#define MMIDCD_GetEntryTitle        MMIAPIDCD_GetEntryTitle
#define MMIDCD_GetEntrySummary      MMIAPIDCD_GetEntrySummary
#define MMIDCD_GetItemAlreadyRead   MMIAPIDCD_GetItemAlreadyRead
#define MMIDCD_GeSettingInfo        MMIAPIDCD_GeSettingInfo
#define MMIDCD_WriteSettingToNv     MMIAPIDCD_WriteSettingToNv

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : the process message function of the main application
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 2003-12-09
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIDCD_HandleSysMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 是否可显示
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 2003-12-09
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_DisplayIsValid(void);

/*****************************************************************************/
//  Description : 是否可IDLE下显示
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_IsIdleDisplay(void);

/*****************************************************************************/
//  Description : init dcd app
//  Global resource dependence : none
//  Author:violent.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_AppInit(void);

/*****************************************************************************/
//  Description : whether dcd is open
//  Global resource dependence : none
//  Author:byte.guo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_DcdIsOpen(void);

/*****************************************************************************/
//  Description :get current mcc
//  Global resource dependence :
//  Author: zhuo.wang
//  Note: app interface
/*****************************************************************************/
//PUBLIC void MMIAPIDCD_SetCurMCC(uint32 mcc, uint32 mnc);

/*****************************************************************************/
//  Description : open dcd main window
//  Global resource dependence : none
//  Author:byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_OpenMainWin(void);

/*****************************************************************************/
//  Description : restore factory setting
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_RestoreFactorySetting(void);

/*****************************************************************************/
//  Description :  dcd send sig to mmi
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void DCD_Sendsig(uint16 signal_code,uint16 para);

/*****************************************************************************/
//  Description : dcd wap push msg is arrived, svr update!!
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_WappushUpdate(void);

/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_CreateIdleCtrl(MMI_WIN_ID_T win_id );

/*****************************************************************************/
//  Description : 获取IDLE下显示区域
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIAPIDCD_GetFocusCtrlRect(void);

/*****************************************************************************/
//  Description : 设置idle上的dcd tab控件的状态
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/9/6
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetIdleTabColorAndActive(BOOLEAN active_flag);

/*****************************************************************************/
//  Description :  open dcd window
//  Global resource dependence : 
//  Author:
//  Note: 供外部调用使用
/*****************************************************************************/
PUBLIC void MMIAPIDCD_ExternalOpenWin(BOOLEAN openMainmenu);

/*****************************************************************************/
//  Description : handle idle win message
//  Global resource dependence : 
//  Author: bin.ji
//  Date: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_HandleIdleWinMessage(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
//  Description : 设置更新LOG开关
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetUpdateLogOnOff(BOOLEAN is_on);

/*****************************************************************************/
//  Description : 获取更新LOG开关状态
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_GetUpdateLogOnOff(void);

/*****************************************************************************/
//  Description : 设置DCD开关状态
//  Global resource dependence : 
//  Author: bin.ji
//  Date: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetDCDEnable(BOOLEAN enable);

/*****************************************************************************/
//  Description : handle dcd flymode
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_HandFlyModeSetChange(void);

/*****************************************************************************/
//  Description : dcd is download data?
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_IsDownloadingData(void);

/*****************************************************************************/
//  Description : when stop u disk, need update idle win
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPIDCD_StopUdiskUpdateIdleWin( void );

/*****************************************************************************/
//  Description : open link list window
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_OpenLinkListWin(void);

/*****************************************************************************/
//  Description : get dcd current page index
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16  MMIAPIDCD_GetDcdCurrentPageIndex();

/*****************************************************************************/
//  Description : set dcd current page index
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetDcdCurrentPageIndex(uint16 current_page_index,uint16 current_real_index_in_page);

/*****************************************************************************/
//  Description : Set mms net setting index  
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_Setting_SetNetSettingIndex(
                                                 MN_DUAL_SYS_E dual_sys,
                                                 uint8         index
                                                 );

/*****************************************************************************/
//  Description : Get dcd net setting homepage
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/9/8
/*****************************************************************************/
PUBLIC char* MMIAPIDCD_MAIN_GetNetHomePage(void);

#ifdef MMI_ENABLE_DCD
/*****************************************************************************/
// 	Description : 获取XmlDoc
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC xmlDocPtr MMIAPIDCD_GetXmlDoc_Ptr(void);

/*****************************************************************************/
// 	Description : 获取外面使用的feed总数
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 区别于上面一个函数的地方在于这里固定了doc
/*****************************************************************************/                         
PUBLIC uint16 MMIAPIDCD_GetFormalFeedNum(void);

/*****************************************************************************/
// 	Description :获取某一频道的可用的频道项数....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/10/17
/*****************************************************************************/
PUBLIC uint16 MMIAPIDCDFILE_GetFeedUseableEntryCount(                                 
                                                  uint16 feed_index,   //频道的index
                                                  BOOLEAN is_show_read  // 是否显示已读
                                                  );

/*****************************************************************************/
// 	Description : 根据索引获取Item
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_GetUseableItemFromIndex(                                 
                                              uint16 page_index,
                                              uint16 item_index,
                                              uint16 * out_item_index_ptr //out
                                              );

/*****************************************************************************/
// 	Description : 获取指定Feed中指定Content属性值
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC char* MMIAPIDCD_GetFeedContentInfo(
                                       uint16 FeedIndex,
                                       uint16 iContentIndex,
                                       char *name_suffix
                                       );

/*****************************************************************************/
// 	Description : 获取指定Feed中指定Item指定Content属性值
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/				 
PUBLIC char * MMIAPIDCD_GetEntryContentInfo(
                                         uint16 FeedIndex,
                                         uint16 EntryIndex,
                                         uint16 iContentIndex,
                                         char *name_suffix 
                                         );

/*****************************************************************************/
// 	Description : 获取指定频道指定新闻的title, 返回的是str的长度
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/9/1 重写了这个函数...
/*****************************************************************************/
PUBLIC uint16 MMIAPIDCD_GetEntryTitle(
                                    wchar *wstr_title_ptr, //out 返回wchar型的title
                                    const uint16  dst_max_count,  
                                    const uint16  FeedIndex,
                                    const uint16  EntryIndex
                                    );

/*****************************************************************************/
// 	Description : 获取指定频道指定新闻的summary
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 
/*****************************************************************************/                   
PUBLIC uint16 MMIAPIDCD_GetEntrySummary(
                                     wchar * wstr_summary_ptr, // out 返回wchar型的title
                                     const uint16  dst_max_count, 
                                     uint16 FeedIndex,
                                     uint16 EntryIndex
                                     );

/*****************************************************************************/
// Description : 得到指定位置的ITEM 是否已读
// Global resource dependence : None
// Author: jun.hu , 2009/8/27
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_GetItemAlreadyRead(uint16 pageIndex, uint16 itemIndex);
#endif

/****************************************************************************/
//  Description: get dcd setting info
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
PUBLIC MMIDCD_NV_SETTING_T * MMIAPIDCD_GeSettingInfo(void);

/*****************************************************************************/
// 	Description : write setting info to nv
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_WriteSettingToNv(void);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /*#define _MMIDCD_EXPORT_H_ */



