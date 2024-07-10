/*****************************************************************************
** File Name:      mmiatv_simu.c                                             *
** Author:                                                                   *
** Date:           2010-05	                                                 *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to dummy atv ref function for simulator *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#include "guilcd.h"
#include "os_api.h"
#include "mmiatv_main.h"
#include "mmiatv_data.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIATV_SEARCH_NUM_MAX       14

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/******************************************************************************/
// Description:analog tv start auto search channel dummy callback
// Author: aoke.hu
// Note:       
/******************************************************************************/ 
PUBLIC void MMIATV_WIN32SearchMsg(
                                  DRECORDER_PREVIEW_PARAM_T const *p_preview_param)
{
    DRECORDER_REPORT_DATA_U param = {0};

    if(PNULL == p_preview_param || PNULL == p_preview_param->report_callback)
    {
        return;
    }

    param.atv_report.report_typ = DRECORDER_REPORT_ATV_SCAN_CHN;

    param.atv_report.chn_id = 1;
    param.atv_report.chn_is_valid = 1;
    p_preview_param->report_callback(&param);

    param.atv_report.chn_id = 2;
    param.atv_report.chn_is_valid = 1;
    p_preview_param->report_callback(&param);

    param.atv_report.report_typ = DRECORDER_REPORT_ATV_SCAN_CHN_END;
    p_preview_param->report_callback(&param);
}

#if 0
/******************************************************************************/
// Description:analog tv start auto search channel
// Author:     Tim.zhu
// Input:      fun: call back function
// Output:     none
// Return:     SCI_TRUE    open success
//             SCI_FALSE   open fail
// Note:       
/******************************************************************************/ 
PUBLIC DRECORDER_RETURN_E DRECORDER_ATV_StartScanChn(
                            DRECORDER_PREVIEW_PARAM_T const *p_preview_param)
{
    DRECORDER_REPORT_DATA_U param = {0};

    param.atv_report.report_typ = DRECORDER_REPORT_ATV_SCAN_CHN;

    param.atv_report.chn_id = 1;
    param.atv_report.chn_is_valid = 1;
    p_preview_param->report_callback(&param);

    param.atv_report.chn_id = 2;
    param.atv_report.chn_is_valid = 1;
    p_preview_param->report_callback(&param);

    param.atv_report.report_typ = DRECORDER_REPORT_ATV_SCAN_CHN_END;
    p_preview_param->report_callback(&param);
    
    return SCI_SUCCESS;
}

/******************************************************************************/
// Description:analog tv stop auto search channel
// Author:     Tim.zhu
// Input:      fun: call back function
// Output:     none
// Return:     SCI_TRUE    open success
//             SCI_FALSE   open fail
// Note:       
/******************************************************************************/ 
PUBLIC DRECORDER_RETURN_E DRECORDER_ATV_StopScanChn(void)
{
    DRECORDER_REPORT_DATA_U param = {0};
    param.atv_report.report_typ = DRECORDER_REPORT_ATV_SCAN_CHN_STOP;
    MMIATV_SetPlayStatus(MMIATV_STATUS_STOPPED);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description:Select source device for drecorder.Optional API.
// Author:     
// Input:      none
// Output:     none
// Return:     
// Note:       This function should be called flowing DRECORDER_Open(). 
//             If the API hasn't be called, source type will be
//             the default( = DRECORDER_SRC_TYP_DCAMERA).
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_SelectSrcDevice(
                                           DRECORDER_SOURCE_DEVICE_E  src_dev)
{
    return DRECORDER_SUCCESS;
}

/******************************************************************************/
// Description: Get the recorded information when recording.
// Author:     
// Input:      p_record_info, address to receive the recorded info.
// Output:     none
// Return:     
// Note:       none
/******************************************************************************/
PUBLIC DRECORDER_RETURN_E DRECORDER_GetRecordInfo(
                            DRECORDER_RECORD_INFO_T *p_record_info
                            )
{
    static uint32 record_time = 0;
    record_time = record_time + 1000;
    p_record_info->media_time = record_time;

    return DRECORDER_SUCCESS;
}
#endif

#endif
