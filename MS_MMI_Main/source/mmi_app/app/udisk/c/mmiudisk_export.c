/*****************************************************************************
** File Name:      mmiudisk_export.c                                         *
** Author:         jian.ma                                                   *
** Date:           11/2007                                                   *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2007       jian.ma            Create			                         *
******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
    **--------------------------------------------------------------------------*/
//#include "std_header.h"
//#include "mmiudisk_export.h"
//#include "mmisd_export.h"   

    
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
//PUBLIC void MMIAPIUDISK_HandleATCExtCmd(void)
//{}
//
//
//
///*****************************************************************************/
//// 	Description :
////	Global resource dependence :
////  Author:vincent.shang
////	Note:
///*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUDISK_IsFormatWaitWinOpened(void)
//{
//    return FALSE;
//}
//
///*****************************************************************************/
//// 	Description : deal with signal of UDISK_PLUGIN
////	Global resource dependence : 
////  Author: Jassmine
////	Note:
///*****************************************************************************/
//PUBLIC MMI_RESULT_E MMIAPIUdisk_HandlePlugIn(void)
//{
//    return MMI_RESULT_FALSE;
//}
//
///*****************************************************************************/
//// 	Description : set  ps is ready to operate usb
////	Global resource dependence : 
////  Author:wancan.you
////	Note: 
///*****************************************************************************/
//PUBLIC void MMIAPIUdisk_SetPSIsReady(
//    BOOLEAN is_ready
//    )
//{}
///*****************************************************************************/
//// 	Description : deal with signal of UDISK_PLUGOUT
////	Global resource dependence : 
////  Author: Jassmine
////	Note:
///*****************************************************************************/
//PUBLIC MMI_RESULT_E MMIAPIUdisk_HandlePlugOut(void)
//{
//    return MMI_RESULT_FALSE;
//}
///*****************************************************************************/
//// 	Description : set udisk ready status
////	Global resource dependence : 
////  Author:wancan.you
////	Note: 
///*****************************************************************************/
//PUBLIC void MMIAPIUdisk_SetUPMIsReady(
//    BOOLEAN is_ready
//    )
//{}
//
///*****************************************************************************/
//// 	Description : udisk run status
////	Global resource dependence : 
////  Author:jassmine.meng
////	Note: 
///*****************************************************************************/
//PUBLIC void MMIAPIUdisk_SetRunStatus(BOOLEAN is_run)
// {}
///*****************************************************************************/
//// 	Description : open udisk room infor window
////	Global resource dependence :
////  Author:jassmine.meng
////	Note:
///*****************************************************************************/
//PUBLIC BOOLEAN MMIUdisk_OpenMemoryInfoWin(void)
//{
//    return FALSE;
//}
///*****************************************************************************/
//// 	Description :get vuart run status 
////	Global resource dependence : 
////  Author:jassmine.meng
////	Note: 
///*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUdisk_VUARTIsRun(void)
//{
//    return FALSE;
// }

/*****************************************************************************/
// 	Description : 异步格式化文件系统的消息处理函数
//	Global resource dependence : 
//  Author:	Jassmine.Meng
//	Note: 
/*****************************************************************************/
//PUBLIC void MMIAPIUdisk_HandleFormatCnf(
//    BOOLEAN    result
//    )
// {}

/*****************************************************************************/
// 	Description :get usb delay handle state
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUdisk_IsUSBDelay(void)
//{
//    return FALSE;
// }
/*****************************************************************************/
// 	Description : open usb select window
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUdisk_OpenUsbOperWindow(void)
//{
//    return FALSE;
// }
/*****************************************************************************/
// 	Description : set usb delay handle state
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
//PUBLIC void MMIAPIUdisk_SetUSBDelay(BOOLEAN is_ready)
// {}
/*****************************************************************************/
// 	Description : UDISK Is Processing
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUdisk_IsProcessing(void)
//{
//	return FALSE;
// }
/*****************************************************************************/
// 	Description : if  pc camera is opened
//	Global resource dependence : 
//  Author:shangke
//	Note:2006 12 29
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUdisk_IsOpenPcCamera(void)
//{
//    return FALSE;
// }
/*****************************************************************************/
// 	Description :MMI open  pc camera 
//	Global resource dependence : 
//  Author:shangke
//	Note:2006 12 31
/*****************************************************************************/
//PUBLIC BOOLEAN MMIUDISK_OpenPCCamera(void)
//{
//	return FALSE;
// }
/*****************************************************************************/
// 	Description : MMI close pc camera 
//	Global resource dependence : 
//  Author:shangke
//	Note:2006 12 31
/*****************************************************************************/
//void MMIAPIUDISK_ClosePCCamera(void)
// {}
/*****************************************************************************/
// 	Description :is in PC Camera 
//	Global resource dependence : 
//  Author:ryan.xu
//	Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUDISK_IsInPCCamera(void)
//{
//    return FALSE;
// }
/*****************************************************************************/
// 	Description : to handle alert window closed message
//	Global resource dependence : 
//  Author:vincent.shang
//	Note: 
/*****************************************************************************/
//PUBLIC MMI_RESULT_E MMIUDISK_HandleUdiskAlertClosedMsg(
//                                MMI_WIN_ID_T win_id, 
//                                MMI_MESSAGE_ID_E msg_id, 
//                                DPARAM param
//                                )
//{
//	return MMI_RESULT_FALSE;
// }
/*****************************************************************************/
// 	Description : close pc camera menu
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
//PUBLIC void MMIUDISK_ClosePCCameraMenu(void)
// {}


/*****************************************************************************/
// 	Description : open udisk managment window
//	Global resource dependence:
//  Author:haiyang.hu
//	Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUDISK_OpenUsbManagmentWindow(void)
//{
//    return FALSE;
// }

/*****************************************************************************/
// 	Description :get udisk run status 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUDISK_UdiskIsRun(void)
//{
//    return FALSE;
// }



#ifndef MSDC_CARD_SUPPORT
#include "mmk_type.h"

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISD_NotInterruptFfs(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E 	msg_id,
    DPARAM              param
    )
{
    return MMI_RESULT_FALSE;
}


/*****************************************************************************/
// 	Description : 打开处理等待窗口
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISD_HandleSDPlug(MMI_MESSAGE_ID_E plug_msg_id)
{}

/*****************************************************************************/
// 	Description : open SD room infor window
//	Global resource dependence :
//  Author:jassmine.meng
//	Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISD_OpenMemoryInfoWin(void)
//{
//    return FALSE;
// }
/*****************************************************************************/
//	Description : init sd card     
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN	MMIAPISD_Init(void)
//{
//    return FALSE;
//}

/*****************************************************************************/
// 	Description : stop current operation when sd plug
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISD_StopOperaSDPlug(void)
{}
/*****************************************************************************/
// 	Description : Is need handle SD plug msg
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPISD_NeedHandleSDPlug(void)
{
    return FALSE;
}
/*****************************************************************************/
// 	Description : handle SD plug msg
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISD_ReHandleSDPlug(void)
{}
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:vincent.shang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_IsFormatWaitWinOpened(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : open SD managment window
//	Global resource dependence:
//  Author:haiyang.hu
//	Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISD_OpenSDManagmentWindow(void)
//{
//    return FALSE;
//}

/*****************************************************************************/
// 	Description : is SD card plug in
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_IsSDPlugIn(void)
{
    return FALSE;
}
/*****************************************************************************/
//  Description:   init for cardlog
//  Author     :   bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_InitForFAT(void)
{
    return FALSE;
}
/*****************************************************************************/
//  Description:   close cardlog
//  Author     :   bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_CloseCardLog(void)
{
    return FALSE;
}
/*****************************************************************************/
//  Description:   open cardlog
//  Author     :  bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_OpenCardLog(void)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : stop SD card using
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_StopSDCard(void)
{
    return TRUE;
}
/*****************************************************************************/
//  Description:   is cardlog on
//  Author     :  bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_IsCardLogOn(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : 停止当前的操作
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISD_StopCurOperation(
                            MMI_MESSAGE_ID_E msg_id
                            )
{
}

/*****************************************************************************/
//  Description : is SD card plug in
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISD_SetSDPlugIn(BOOLEAN is_sd_plug_in)
{    
}

/*****************************************************************************/
//  Description : 处理等待窗口
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISD_HandleSDPlugWaiting (
                                        MMI_WIN_ID_T       win_id,  //窗口的ID
                                        MMI_MESSAGE_ID_E    msg_id,     //内部消息ID
                                        DPARAM             param        //相应消息的参数
                                        )
{
	return MMI_RESULT_FALSE;
}

/*****************************************************************************/
//  Description:   is usb log on
//  Author     :  bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_IsUsbLogOn(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : before sd card format, we have something to do.
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISD_HandleBeforeSDCardFormat(void)
{
}
#endif
    

#if defined(__cplusplus)
}
#endif

