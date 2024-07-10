/*****************************************************************************
** File Name:      mmirecord_setting.h                                           *
** Author:                                                                   *
** Date:           2012/6/18                                                 *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/6/18      apple.zhang       Create
******************************************************************************/

#ifndef _MMIRECORD_SETTING_H_
#define _MMIRECORD_SETTING_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmirecord_export.h"
#include "mmi_filemgr.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

    /*---------------------------------------------------------------------------*/
    /*                          MACRO DEFINITION                                 */
    /*---------------------------------------------------------------------------*/


    /**--------------------------------------------------------------------------*
    **                         TYPE AND CONSTANT                                *
    **--------------------------------------------------------------------------*/
    typedef struct
    {
        MMIFILE_DEVICE_E    dev;
        uint32              record_file_id;
        MMIRECORD_WORK_FORMAT_E  file_formattype;//andy.he_record
    } MMIRECORD_SETTING_INFO_T;

    /*****************************************************************************/
// 	Description : seting info ini
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_SettingInit(void);

    /*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMIFILE_DEVICE_E MMIRECORD_Setting_GetRecordStorage(void);

    /*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:andy.he
//  Note:
    /*****************************************************************************/
    PUBLIC MMIRECORD_WORK_FORMAT_E MMIRECORD_Setting_GetRecordFileFormatType(void);

    /*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_Setting_SetRecordStorage(MMIFILE_DEVICE_E storage);

    /*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author: andy.he
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_Setting_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_E filetype);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordFileId
//  Global resource dependence :
//  Author:robert.wang
//  Note:
//  Param
    /*****************************************************************************/
    PUBLIC uint32 MMIRECORD_Setting_GetRecordFileId(void);

    /*****************************************************************************/
//  Description : MMIRECORD_SetRecordFileId
//  Global resource dependence :
//  Author:robert.wang
//  Note:
//  Param
    /*****************************************************************************/
    PUBLIC void MMIRECORD_Setting_SetRecordFileId(uint32 record_file_id);
    /**--------------------------------------------------------------------------*
    **                         Compiler Flag                                    *
    **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
