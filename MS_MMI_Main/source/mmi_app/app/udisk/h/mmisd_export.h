/****************************************************************************
** File Name:      mmisd_export.h                                                *
** Author:                                                                 *
** Date:           07/2006                                                 *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                  message                                                *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2006       Bruce.Chi          Create                                 *
**                                                                         *
****************************************************************************/
#ifndef _MMI_SD_H_    
#define _MMI_SD_H_   

/**-------------------------------------------------------------------------*
**                         Compile Flag                                   *
**------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
    
/**-------------------------------------------------------------------------*
**                         Include Files                                   *
    **------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmifmm_export.h"
#include "sfs.h"
//#include "sd.h"
#ifdef MSDC_CARD_SUPPORT
#include "scm_api.h"
#endif
/*-------------------------------------------------------------------------*/
/*                         MACROS                                          */
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
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
    );

/*****************************************************************************/
// 	Description : 打开处理等待窗口
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISD_HandleSDPlug(
								  void             *param_ptr, 
								  MMI_MESSAGE_ID_E plug_msg_id);

/*****************************************************************************/
//  Description : 停止当前的操作
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISD_StopCurOperation(
                            MMI_MESSAGE_ID_E msg_id
                            );

/*****************************************************************************/
// 	Description : open SD room infor window
//	Global resource dependence :
//  Author:jassmine.meng
//	Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISD_OpenMemoryInfoWin(void);

/*****************************************************************************/
//	Description : init sd card     
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPISD_Init(void);


/*****************************************************************************/
// 	Description : stop current operation when sd plug
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISD_StopOperaSDPlug(void);

/*****************************************************************************/
// 	Description : Is need handle SD plug msg
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISD_CheckAllDelayedDevice(void);


/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:vincent.shang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_IsFormatWaitWinOpened(void);

/*****************************************************************************/
// 	Description : open SD managment window
//	Global resource dependence:
//  Author:haiyang.hu
//	Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPISD_OpenSDManagmentWindow(void);

/*****************************************************************************/
// 	Description : is SD card plug in
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_IsSDPlugIn(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description:   open cardlog
//  Author     :  bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPISD_OpenCardLog(void);

/*****************************************************************************/
//  Description:   close cardlog
//  Author     :   bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPISD_CloseCardLog(void);

/*****************************************************************************/
//  Description:   is cardlog on
//  Author     :  bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPISD_IsCardLogOn(void);

/*****************************************************************************/
//  Description:   is usb log on
//  Author     :  bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPISD_IsUsbLogOn(void);

/*****************************************************************************/
// 	Description : is SD card plug in
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
// PUBLIC BOOLEAN MMISD_IsSDPlugIn(void);

/*****************************************************************************/
// 	Description : stop SD card using
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_StopSDCard(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
// 	Description : is SD card plug in
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISD_SetSDPlugIn(MMIFILE_DEVICE_E device_type, BOOLEAN is_sd_plug_in);

/*****************************************************************************/
//  Description:   init for cardlog
//  Author     :   bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPISD_InitForFAT(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
// 	Description : 处理等待窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISD_HandleSDPlugWaiting  (
                                        MMI_WIN_ID_T       win_id, 	//窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id,     //内部消息ID
                                        DPARAM             param	    //相应消息的参数
                                        );

/*****************************************************************************/
//  Description : before sd card format, we have something to do.
//  Global resource dependence : 
//  Author:haiyanghu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISD_HandleBeforeSDCardFormat(void);


/*****************************************************************************/
//  Description : check if there is a sd card whose left space is bigger than the min space
//  Global resource dependence : 
//  Author:charlie.wang
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPISD_CheckSDSpace(uint32 min_space, MMIFILE_DEVICE_E *fmm_device);

/*****************************************************************************/
//  Description : check if the fmm_device(SD Card) needs format
//  Global resource dependence : 
//  Author:charlie.wang
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPISD_IsSDNeedFormat(MMIFILE_DEVICE_E fmm_device);

/*****************************************************************************/
//  Description : check if there is one sd card existed
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMISD_IsSDExistEx(void);

/*****************************************************************************/
//  Description : return existed SD card type
//  Global resource dependence : 
//  Author:
//  Note:返回第一张存在的SD卡，若不存在则返回UDISK
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMISD_GetExistSDCardType(void);
#ifdef MSDC_CARD_SUPPORT
PUBLIC SCM_SLOT_NAME_E MMISD_GetSLOTName(MMIFILE_DEVICE_E device_type);
#endif

/**-------------------------------------------------------------------------*
**                         Compile Flag                                   *
**------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
    
#endif
