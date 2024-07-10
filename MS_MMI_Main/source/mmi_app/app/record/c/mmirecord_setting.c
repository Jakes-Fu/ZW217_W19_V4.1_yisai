/*****************************************************************************
** File Name:      mmirecord_setting.c                                       *
** Author:         bin.ji                                                    *
** Date:           3/1/2005                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 7/10/2012       apple.zhang           Create                              *
**                                                                           *
******************************************************************************/
#define _MMIRECORD_SETTING_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmirecord_setting.h"
#include "mmirecord_nv.h"
#include "mn_type.h"
#include "mmi_nv.h"
#include "mmifmm_export.h"
#include "mmirecord_appcontrol.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get setting info
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL MMIRECORD_SETTING_INFO_T GetRecordSettingInfo(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : seting info ini
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SettingInit(void)
{
    GetRecordSettingInfo();
}

/*****************************************************************************/
// 	Description : get setting info
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL MMIRECORD_SETTING_INFO_T GetRecordSettingInfo(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
	const wchar*        file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    MMIRECORD_SETTING_INFO_T record_setting_info = {0};

    MMINV_READ(MMINV_RECORD_SETTING, &record_setting_info, return_value);

	file_dev_ptr = MMIAPIFMM_GetDevicePath((record_setting_info.dev));
	file_dev_len = MMIAPIFMM_GetDevicePathLen((record_setting_info.dev));

    if(MN_RETURN_SUCCESS != return_value || !MMIAPIFMM_GetDeviceStatus(file_dev_ptr, file_dev_len))
    {
        record_setting_info.dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);

        /*if(MMIRECORDSRV_QueryEncoderCapability(MMISRVAUD_RECORD_FMT_MP3))
        {
            record_setting_info.file_formattype = MMIRECORD_WORK_FORMAT_MP3;
        }
        else
        {
            record_setting_info.file_formattype = MMIRECORD_WORK_FORMAT_WAV;
        }*/
        record_setting_info.file_formattype = MMIRECORD_WORK_FORMAT_WAV;
        MMINV_WRITE(MMINV_RECORD_SETTING, &record_setting_info);
    }
    SCI_TRACE_LOW("[RECORD] GetRecordSettingInfo dev = %d", record_setting_info.dev);
    return record_setting_info;
}


/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIRECORD_Setting_GetRecordStorage(void)
{
    MMIRECORD_SETTING_INFO_T record_setting_info = GetRecordSettingInfo();
    return record_setting_info.dev;
}

/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:andy.he
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_WORK_FORMAT_E MMIRECORD_Setting_GetRecordFileFormatType(void)
{
    MMIRECORD_SETTING_INFO_T record_setting_info = GetRecordSettingInfo();
    return record_setting_info.file_formattype;
}

/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_Setting_SetRecordStorage(MMIFILE_DEVICE_E storage)
{
    MMIRECORD_SETTING_INFO_T record_setting_info = GetRecordSettingInfo();
    record_setting_info.dev = storage;
    MMINV_WRITE(MMINV_RECORD_SETTING, &record_setting_info);
}

/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author: andy.he
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_Setting_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_E filetype)
{
    MMIRECORD_SETTING_INFO_T record_setting_info = GetRecordSettingInfo();

    record_setting_info.file_formattype = filetype;
    MMINV_WRITE(MMINV_RECORD_SETTING, &record_setting_info);
}

/*****************************************************************************/
//  Description : MMIRECORD_GetRecordFileId
//  Global resource dependence :
//  Author:robert.wang
//  Note:
//  Param
/*****************************************************************************/
PUBLIC uint32 MMIRECORD_Setting_GetRecordFileId(void)
{
    MMIRECORD_SETTING_INFO_T record_setting_info = GetRecordSettingInfo();
    return record_setting_info.record_file_id;
}


/*****************************************************************************/
//  Description : MMIRECORD_SetRecordFileId
//  Global resource dependence :
//  Author:robert.wang
//  Note:
//  Param
/*****************************************************************************/
PUBLIC void MMIRECORD_Setting_SetRecordFileId(uint32 record_file_id)
{
    MMIRECORD_SETTING_INFO_T record_setting_info = GetRecordSettingInfo();

    record_setting_info.record_file_id = record_file_id;
    MMINV_WRITE(MMINV_RECORD_SETTING, &record_setting_info);
}

