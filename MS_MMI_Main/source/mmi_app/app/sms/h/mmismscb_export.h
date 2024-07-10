/******************************************************************************
**
**
**
**  小区广播模块
**
**
**
**
**
**
*******************************************************************************/
#if defined(MMI_SMSCB_SUPPORT)

#ifndef  _MMISMSCB_EXPORT_H_    
#define  _MMISMSCB_EXPORT_H_   
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mn_type.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/******************************************************************************************************************/
//
//
//                        小区广播模块对外接口函数声明
//
//
/******************************************************************************************************************/

/***************************************************************************/
//     Description : 初始化小区广播接口函数(在手机初始化时调用)
//    Global resource dependence : none
//  Author:肖凡
//    Note:
/***************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIAPISMSCB_Init(MN_DUAL_SYS_E dual_sys);

//window related
/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenMainMenu(void);

/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_RestoreFactorySetting(void);

/*****************************************************************************/
//  Description : open SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenSettingWin(void);

/*****************************************************************************/
//     Description : MMIAPISMSCB_OpenCBList
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_OpenCBList(void);

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set SMS CB Status
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SwitchSmsCBStatus(BOOLEAN cb_enabled);
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_HandleCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleCBMsgInd(DPARAM param);       //收到小区广播信号处理

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_HandleMarcoCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleMarcoCBMsgInd(DPARAM param);       //收到小区广播信号处理

#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description : Get Unread SMS CB Number
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetUnreadSMSCBNum(void);

/*****************************************************************************/
//  Description : Get Unread SMS CB Channel
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBChannel(wchar *name_ptr, uint16 max_len);

/*****************************************************************************/
//  Description : Get Unread SMS CB Time
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBTime(wchar *time_ptr, uint16 max_len);
#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetScrollItemNewSMSCB(void);
#endif


#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_StopETWSVibrateTimer(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_ConvertCBTimeToStr
//  功能描述:        将小区广播时间转为UCS2或标准字符串格式
//  全局变量使用:    无
//  返回值:          转换后的字符串长度
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC uint16 MMISMSCB_ConvertCBTimeToStr(                            //RETURN:
								   SCI_DATE_T    date,        //IN: 要进行转换的日期结构
								   SCI_TIME_T    time,       //IN: 要进行转换的时间结构
								   wchar            *str_ptr    //OUT: 输出字符串的指针
								   );
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif//_MMISMSCB_EXPORT_H_
#endif//#if defined(MMI_SMSCB_SUPPORT)

