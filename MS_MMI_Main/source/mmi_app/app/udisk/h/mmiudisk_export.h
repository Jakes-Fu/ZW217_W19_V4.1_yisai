/*****************************************************************************
** File Name:      mmiudisk.h                                                   *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng       Create
******************************************************************************/

#ifndef _MMIUDISK_EXPORT_H_
#define _MMIUDISK_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"
//#include "ffs.h"

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
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

typedef enum
{
    DEVICE_STATUS_OK,   //设备准备就绪
    DEVICE_STATUS_FORMAT,//格式不支持，需要格式化
    DEVICE_STATUS_ERROR,  //设备出错或者不存在
    DEVICE_STATUS_MAX
}DEVICE_STATUS_E;

typedef enum
{
	MMI_USB_SERVICE_UDISK = 0,//udisk
	MMI_USB_SERVICE_UCOM, //pc data communicate
	MMI_USB_SERVICE_UPCC, //pc camera
	MMI_USB_SERVICE_LOG,  //log
	MMI_USB_SERVICE_NULL
}MMI_USBSRV_TYPE;
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIUDISK_HandleATCExtCmd(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPIUDISK_IsOpened(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:vincent.shang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_IsFormatWaitWinOpened(void);


/*****************************************************************************/
// 	Description : deal with signal of UDISK_PLUGIN
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIUdisk_HandleUsbCablePlugIn(void);

/*****************************************************************************/
// 	Description : set  ps is ready to operate usb
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
//PUBLIC void MMIAPIUdisk_SetPSIsReady(
//                                       BOOLEAN is_ready
//                                        );

/*****************************************************************************/
// 	Description : deal with signal of UDISK_PLUGOUT
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIUdisk_HandleUsbCablePlugOut(void);

/*****************************************************************************/
// 	Description : set udisk ready status
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
//PUBLIC void MMIAPIUdisk_SetUPMIsReady(
//    BOOLEAN is_ready
//    );
// 
/*****************************************************************************/
// 	Description : udisk run status
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_SetRunStatus(BOOLEAN is_run);

/*****************************************************************************/
// 	Description : open udisk room infor window
//	Global resource dependence :
//  Author:jassmine.meng
//	Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPIUdisk_OpenMemoryInfoWin(void);

/*****************************************************************************/
// 	Description :get vuart run status 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUdisk_VUARTIsRun(void);


/*****************************************************************************/
//  Description :usb service started
//  Global resource dependence : 
//  Author:jassmine.meng
// te: 
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_USBServiceStartInd(MMI_USBSRV_TYPE usb_srv_type);

/*****************************************************************************/
//  Description :usb service stopped
//  Global resource dependence : 
//  Author:jassmine.meng
// te: 
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_USBServiceStopInd(MMI_USBSRV_TYPE usb_srv_type);

/*****************************************************************************/
// 	Description : 异步格式化文件系统的消息处理函数
//	Global resource dependence : 
//  Author:	Jassmine.Meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_HandleFormatCnf(
    BOOLEAN    result
    );

/*****************************************************************************/
// 	Description :get usb delay handle state
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUdisk_IsUSBDelay(void);

/*****************************************************************************/
// 	Description : open usb select window
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUdisk_OpenUsbOperWindow(void);

/*****************************************************************************/
// 	Description : set usb delay handle state
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_SetUSBDelay(BOOLEAN is_ready);

/*****************************************************************************/
// 	Description : UDISK Is Processing
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPIUdisk_IsProcessing(void);
/*****************************************************************************/
// 	Description : if  pc camera is opened
//	Global resource dependence : 
//  Author:shangke
//	Note:2006 12 29
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUdisk_IsOpenPcCamera(void);
/*****************************************************************************/
// 	Description :MMI open  pc camera 
//	Global resource dependence : 
//  Author:shangke
//	Note:2006 12 31
/*****************************************************************************/
// PUBLIC BOOLEAN MMIUDISK_OpenPCCamera(void);
/*****************************************************************************/
// 	Description : MMI close pc camera 
//	Global resource dependence : 
//  Author:shangke
//	Note:2006 12 31
/*****************************************************************************/
// PUBLIC void MMIAPIUDISK_ClosePCCamera(void);

/*****************************************************************************/
// 	Description :is in PC Camera 
//	Global resource dependence : 
//  Author:ryan.xu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_IsInPCCamera(void);

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
//                                 );
/*****************************************************************************/
// 	Description : close pc camera menu
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
// PUBLIC void MMIUDISK_ClosePCCameraMenu(void); 

/*****************************************************************************/
// 	Description : open udisk managment window
//	Global resource dependence:
//  Author:haiyang.hu
//	Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIUDISK_OpenUsbManagmentWindow(void);

/*****************************************************************************/
// 	Description :get udisk run status 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_UdiskIsRun(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_IsStopWainWinOpened(void);

/*****************************************************************************/
// 	Description : MMI close pc camera 
//	Global resource dependence : 
//  Author:shangke
//	Note:2006 12 31
/*****************************************************************************/
PUBLIC void MMIAPIUDISK_ClosePCCamera(void);
/*****************************************************************************/
//  Description:   init hidden partion on nand flash.
//  Author     :   Arthur.Peng 2006/12
//  Param
//           void
//  Return:
//         TRUE : INIT COMPLETE
//         FALSE : INIT FAILED.
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPIUDISK_SysDeviceinit(void);
/*****************************************************************************/
//  Description : get device  status
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC DEVICE_STATUS_E MMIAPIUDISK_GetDeviceStatus( 
                                                   const wchar *device_ptr,  
                                                   uint16 device_len  );
/*****************************************************************************/
//  Description : if there is a device for using
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_IsDeviceReady(void);

/*****************************************************************************/
//  Description : open Memory detail win
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUDISK_OpenMemDetailWin(uint32 device);


/*****************************************************************************/
//  Description : open Memory format win
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUDISK_OpenFormatWin(uint32 device);

/*****************************************************************************/
//  Description : 获得当前服务类型的显示的字符串ID
//  Global resource dependence : 
//  Author: 
//  Note:返回该该类型的显示的字符串ID
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIUDISK_GetCurServiceNameTextID(void);

/*****************************************************************************/
//  Description :is in PC Camera 
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_GetIsPowerOn(void);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_TipWin_Judge(void);

#ifdef   __cplusplus
    }
#endif
    
#endif
