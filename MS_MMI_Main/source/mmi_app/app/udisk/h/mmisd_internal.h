/*****************************************************************************
** File Name:      mmiudisk_internal.h                                       *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng       Create
******************************************************************************/

#ifndef _MMIUDISK_INTERNAL_H_
#define _MMIUDISK_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"
#ifdef MSDC_CARD_SUPPORT
#include "scm_api.h"
#endif
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


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : u盘退出以后，重新初始化有关文件系统MMI方面的操作
//  Global resource dependence : 
//  Author: wancan.you
//  Note: @wancan.you udisk
/*****************************************************************************/
PUBLIC void MMIUDISK_InitFFS_MMI(void);

/*****************************************************************************/
//  Description : open usb select window
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_UsbExpiredInCallWindow(void);

/*****************************************************************************/
//  Description : 启动U盘功能
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_StartUdisk(void);

/*****************************************************************************/
//  Description : restore file system
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIUDISK_RestoreFS(void);

/*****************************************************************************/
//  Description : 格式化U盘
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_FormatUdisk(void);

/*****************************************************************************/
//  Description : set used memory and unused memory detail
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_SetUdiskMemoryDetail(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : start Virtual uart
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_StartVirtualUart(void);

/*****************************************************************************/
//  Description : start pc camera service
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_StartUPCC(void);

#ifdef MSDC_CARD_SUPPORT
/*****************************************************************************/
//  Description : SD 卡热插拔初始化
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMISD_SDPlugInit(
						BOOLEAN   is_sd_in,
						uint32    scm_slot
						);

PUBLIC void MMISD_CreateDefaultFold(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : get sdcard type by scm slot type
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMISD_GetSDCardType(SCM_SLOT_NAME_E slot_name);


/*****************************************************************************/
//  Description : init sdcard
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISD_InitSDCard(SCM_SLOT_NAME_E scm_name);

#endif
///*****************************************************************************/
////  Description : open waiting window    
////  Global resource dependence : 
////  Author:
///*****************************************************************************/
//PUBLIC void MMIUDISK_OpenWaitWin(
//								 uint32          usb_srv_type,
//								 MMI_TEXT_ID_T   alert_text_id
// 								 );

/*****************************************************************************/
//  Description :get udisk reday status 
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIUDISK_UPMIsReady(void);

/*****************************************************************************/
//  Description :get ps is ready to operate usb
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
//  PUBLIC BOOLEAN MMIUDISK_IsPSReady(void);



/*****************************************************************************/
//  Description : read pc camera settings
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
// PUBLIC void MMIUDISK_ReadPCCSettings(void);


/*****************************************************************************/
//  Description : start usb log
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_StartUsbLog(void);

/*****************************************************************************/
//  Description : current operation can be abort 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_CurrentAppCanbeAbort(void);

/*****************************************************************************/
//  Description : set ucom mode
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_SetUcomMode(uint32 ucom_type);

/*****************************************************************************/
//  Description : Start udisk in power off
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIUDISK_StartUdiskInPowerOff(void);
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
// PUBLIC void MMIUDISK_OpenWaitingWin(void);

/*****************************************************************************/
//  Description : open udisk room infor window
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUdisk_OpenMemoryInfoWin(void);

/*****************************************************************************/
//  Description :MMI open  pc camera 
//  Global resource dependence : 
//  Author:shangke
//  Note:2006 12 31
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_OpenPCCamera(void);

/*****************************************************************************/
//  Description : to handle alert window closed message
//  Global resource dependence : 
//  Author:vincent.shang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIUDISK_HandleUdiskAlertClosedMsg(
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param
    );

/*****************************************************************************/
//  Description : set used memory and unused memory detail
//  Global resource dependence : 
//  Author:yongwei.he
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_SetUdiskMemoryDetail(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : setting sd/udisk memory detail info
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIUDISK_SetMemoryDetail(uint32 device);
/*****************************************************************************/
//  Description : get file size string
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIUDISK_GetFileSizeString(
                             uint32  low_size,
                             uint32  hight_size,
                             wchar*  str_ptr,
                             uint16  wstr_len
                             );

#ifdef MCARE_V31_SUPPORT
PUBLIC void MCAREV31_StartUdisk(void);
#endif
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
