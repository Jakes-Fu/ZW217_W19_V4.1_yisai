/*************************************************************************
** File Name:      mmirecord_main.h                                  *
** Author:         bin.ji                                                *
** Date:           3/2/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.    *
** Description:     This file declare the function about record          *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 3/2/2005     bin.ji             Create.                              *
*************************************************************************/

#ifndef _MMIRECORD_MAIN_H_
#define _MMIRECORD_MAIN_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "mmirecord_export.h"
#include "mmicom_panel.h"
#include "mmi_filemgr.h"
#include "mmisrvrecord_export.h"
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

#define MMIRECORD_WORK_LEASTTIME  1000// 1 *1000ms   最短时间
#define MMIRECORD_WORK_LEASTSIZE   1024// 1 *1024Bit     最小大小

    /**--------------------------------------------------------------------------*
     **                         TYPE AND CONSTANT                                *
     **--------------------------------------------------------------------------*/
    typedef enum
    {
        MMIRECORD_STATE_START,          // start state
        MMIRECORD_STATE_RECORDING,      // recording
        MMIRECORD_STATE_PLAYING,        // is playing
        MMIRECORD_STATE_PLAY_PAUSED,    // play be paused
        MMIRECORD_STATE_RECORD_PAUSED,
        MMIRECORD_STATE_STOPPED,
        MMIRECORD_STATE_MAX             // record is closed
    }
    MMIRECORD_STATE_E;  //record state

    /**--------------------------------------------------------------------------*
     **                         FUNCTION DEFINITION                              *
     **--------------------------------------------------------------------------*/
    /*****************************************************************************/
//  Description : Register record menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_RegMenuGroup(void);

    /*****************************************************************************/
//  Description :MMIAPIRECORD_DecideRecordWorkFormat
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMIRECORD_WORK_FORMAT_E  MMIRECORD_DecideRecordWorkFormat(uint32  record_support_type);//recordwork;

    /*****************************************************************************/
//  Description : open record file
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_OpenRecordFileList(void);

    /*****************************************************************************/
//  Description : register RECORD module applet info
//  Global resource dependence :
//  Author:fen.xie
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_RegAppletInfo(void);

    /*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_SendFileViaBT(wchar       *record_file_path_ptr,
                                           uint16 record_file_path_len);

    /*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_SendFileViaMMS(wchar       *record_file_path_ptr,
                                            uint16  record_file_path_len);


    /*****************************************************************************/
//  Description : 调节音量窗口
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type);

    /*****************************************************************************/
//  Description : show error message as error type
//  Global resource dependence :
//  Author:robert.wang
//  Note:
//  Param [Input]:  fs_error    error  type as MMIFILE_ERROR_E
    /*****************************************************************************/
    PUBLIC void  MMIRECORD_ShowErrorMsg(MMIFILE_ERROR_E fs_error);

    /*****************************************************************************/
// 	Description : set mute
//	Global resource dependence : none
//  Author:
//	Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_SetMute(void);

    /*****************************************************************************/
//  Description : open record main win
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
    /*****************************************************************************/
    PUBLIC MMIRECORD_WORK_ERROR_E MMIRECORD_OpenMainWin(MMIRECORD_WORK_PARAM_T *work_param_ptr);
#ifdef ADULT_WATCH_SUPPORT
    /*****************************************************************************/
    //  Description : 外部模块调用录音模块插入录音，如MMS/ASP/ACENDLER/weibo编辑插入录音
    //  Global resource dependence :
    //      Author: feiyue.ji
    //  Note:
    /*****************************************************************************/
    PUBLIC void WATCHRECORD_OpenMainWin(void);
#endif
    /*****************************************************************************/
//  Description : recorder exit
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_Exit(void);


    /*****************************************************************************/
//  Description : Whether the record module is busy with working.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_IsOpened(void); //TRUE: have opened;

    /*****************************************************************************/
//  Description : stop record and stop play
//  Global resource dependence : none
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_StopRecordOrPlay(void);

    /*****************************************************************************/
//  Description : insert record file
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_InsertRecord(wchar * file_full_path_ptr,
                                          uint16  file_full_path_len);

    /*****************************************************************************/
//  Description : MMIRECORD_IsExistValidateDevice
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_IsExistValidateDevice(void);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordDeviceIndex
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC uint16 MMIRECORD_GetRecordDeviceIndex(void);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordFileFormatType
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC MMIRECORD_WORK_FORMAT_E MMIRECORD_GetRecordFileFormatType(void);

    /*****************************************************************************/
//  Description : MMIRECORD_SetRecordFileFormatType
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_E filetype);

    /*****************************************************************************/
//  Description : MMIRECORD_SetRecordStorage
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_SetRecordStorage(MMIFILE_DEVICE_E storage);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordStorage
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC MMIFILE_DEVICE_E MMIRECORD_GetRecordStorage(void);

    /*****************************************************************************/
//  Description : MMIRECORD_SetRecordId
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_SetRecordId(uint32 record_id);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordId
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC uint32 MMIRECORD_GetRecordId(void);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordWin
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC MMI_HANDLE_T MMIRECORD_GetRecordWin(void);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordState
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC MMIRECORD_STATE_E MMIRECORD_GetRecordState(void);

    /*****************************************************************************/
//  Description : MMIRECORD_SetRecordState
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_SetRecordState(MMIRECORD_STATE_E state);

    /*****************************************************************************/
//  Description : MMIRECORD_SetRecordHandle
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_SetRecordHandle(RECORD_SRV_HANDLE srv_handle);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordHandle
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC RECORD_SRV_HANDLE MMIRECORD_GetRecordHandle(void);

#endif
