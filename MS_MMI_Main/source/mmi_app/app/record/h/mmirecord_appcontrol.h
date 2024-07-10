/*************************************************************************
** File Name:      mmirecord_appcontrol.h                                  *
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

#ifndef _MMIRECORD_APPCONTROL_H_
#define _MMIRECORD_APPCONTROL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "mmirecord_export.h"
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


    /**--------------------------------------------------------------------------*
     **                         TYPE AND CONSTANT                                *
     **--------------------------------------------------------------------------*/
    typedef struct
    {
        uint8                   second_low;
        uint8                   second_high;
        uint8                   minute_low;
        uint8                   minute_high;
        uint8                   hour_low;
        uint8                   hour_high;
    } MMIRECORD_TIME_T;



    /**--------------------------------------------------------------------------*
     **                         FUNCTION DEFINITION                              *
     **--------------------------------------------------------------------------*/
    /*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_Constructor(void);

    /*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_Destructor(void);

    /*****************************************************************************/
//  Description : ÓÃ»§²»±£´æÂ¼Òô£¬É¾³ýÁÙÊ±ÎÄ¼þ
//  Global resource dependence :
//      Author: bin.ji
//  Note:
    /*****************************************************************************/
    PUBLIC MMIRECORD_RESULT_E MMIRECORD_DeleteRecordFile(void);

    /*****************************************************************************/
//  Description : ¿ªÊ¼Â¼Òô£¨Í¨³£Çé¿ö£©
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
    /*****************************************************************************/
    PUBLIC MMIFILE_ERROR_E MMIRECORD_StartRecordNormal(BOOLEAN is_reRecord,
            MMIRECORD_WORK_FORMAT_E fmt,
            uint32 *record_id_ptr,
            MMIFILE_DEVICE_E record_dev);

    /*****************************************************************************/
//  Description : ConvertWorkFMTToSRVFMT
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
    /*****************************************************************************/
    PUBLIC MMISRVAUD_RECORD_FMT_E MMIRECORD_ConvertWorkFMTToSRVFMT(MMIRECORD_WORK_FORMAT_E fmt);

    /*****************************************************************************/
//  Description : MMIRECORD_GetCurrentDev
//  Global resource dependence :
//  Author:
//  Note:
    /*****************************************************************************/
    PUBLIC MMIFILE_DEVICE_E MMIRECORD_GetCurrentDev(void);

    /*****************************************************************************/
//  Description : ÔÝÍ£Â¼Òô
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_PauseRecord(void);

    /*****************************************************************************/
//  Description : ¼ÌÐøÂ¼Òô
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_ResumeRecord(void);

    /*****************************************************************************/
//  Description : Í£Ö¹²¥·ÅÂ¼Òô
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_StopPlay(void);

    /*****************************************************************************/
//  Description : ¼ÌÐø²¥·ÅÂ¼Òô
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_ResumePlay(void);

    /*****************************************************************************/
//  Description : ÔÝÍ£²¥·ÅÂ¼Òô
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_PausePlay(void);

    /*****************************************************************************/
//  Description : play current record
//  Global resource dependence :
//  Author: ryan.xu
//  Note: only called by this module
    /*****************************************************************************/
    PUBLIC MMIRECORD_RESULT_E MMIRECORD_PlayCurrent(void);


    /*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
    /*****************************************************************************/
    PUBLIC wchar* MMIRECORD_GetRecordFileName(void);

    /*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
    /*****************************************************************************/
    PUBLIC wchar* MMIRECORD_GetRecordFilePath(void);

    /*****************************************************************************/
//  Description :stop record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
    /*****************************************************************************/
    PUBLIC MMIRECORD_RESULT_E MMIRECORD_StopRecord(void);


    /*****************************************************************************/
//  Description : MMIRECORD_IsReachMax
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_IsReachMax(uint32 max_file_time, uint32 max_file_size);

    /*****************************************************************************/
//  Description : already starting recording
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_IsRecord(void);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordFileTotalTime
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC uint32 MMIRECORD_GetRecordFileTotalTime(void);

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordTime
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
    /*****************************************************************************/
    PUBLIC uint32 MMIRECORD_GetRecordSecondTime(void);

    /*****************************************************************************/
//  Description : calculate time of MMIRECORD_TIME_T
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
    /*****************************************************************************/
    PUBLIC MMIRECORD_TIME_T MMIRECORD_GetRecordTime(void) ;

    /*****************************************************************************/
//  Description : MMIRECORD_GetRecordDB
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC uint32 MMIRECORD_GetRecordDB(void);

    /*****************************************************************************/
//  Description : MMIRECORD_RecordFilePlaySeek
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_PlaySeek(uint16 percent);

    /*****************************************************************************/
//  Description : MMIRECORD_SetVolume
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_SetVolume(uint32 cur_value);

    /*****************************************************************************/
//  Description : MMIRECORD_IsSupport
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_E fmt);

    /*****************************************************************************/
//  Description : MMIRECORD_FreeVirtualHandle
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_FreeVirtualHandle(void);


    /*****************************************************************************/
//  Description : MMIRECORD_RequestVirtualHandle
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIRECORD_RequestVirtualHandle(void);

/*****************************************************************************/
//  Description : record do factory reset
//  Parameter: [In]  None
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_FactoryReset(void);

#endif
