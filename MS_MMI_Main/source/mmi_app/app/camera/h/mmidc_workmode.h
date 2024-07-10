/*****************************************************************************
** File Name:      mmidc_workmode.h                                          *
** Author:                                                                   *
** Date:           01/16/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera work mode                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012       robert.wang         Create
******************************************************************************/

#ifndef _MMIDC_WORKMODE_H_
#define _MMIDC_WORKMODE_H_ 

#if defined(MMIDC_F_WORK_MODE)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmidc_export.h"
#include "mmidc_setting.h"

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
    MMIDC_WORK_ERROR_E err_ret;
    MMIDC_WORK_PARAM_T work_param;
    int32 file_name_len;//文件路径长度
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN + 1];//文件名称（包括路径）
    MMIDC_PHOTO_SIZE_E max_photo_size;
    VIDEO_SIZE_E max_video_size;
    BOOLEAN is_enable;
    BOOLEAN is_check_param;
}MMIDC_WORK_INFO_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get max photo size of work mode in dc module
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetMaxPhotoSizeOfWorkMode(void);

/*****************************************************************************/
//  Description : get max video size of work mode in dc module
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDC_GetMaxVideoSizeOfWorkMode(void);

/*****************************************************************************/
//  Description : process work callback
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ProcessWorkCallback(void);

/*****************************************************************************/
//  Description : release dc work mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_ReleaseWorkMode(void);

/*****************************************************************************/
//  Description : is dc work mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsWorkMode(void);

/*****************************************************************************/
//  Description : set work error no.
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetWorkError(MMIDC_WORK_ERROR_E err);

/*****************************************************************************/
//  Description : get limited video file size of work mode in dc module
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetLimitedVideoFileSizeOfWorkMode(void);

/*****************************************************************************/
//  Description : save work mode file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SaveWorkFile(void);

/*****************************************************************************/
//  Description : is check work param
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsCheckWorkParam(void);

/*****************************************************************************/
//  Description : set work param flag
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCheckWorkParamFlag(BOOLEAN ret);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //#if defined(MMIDC_F_WORK_MODE)

#endif //_MMIDC_WORKMODE_H_


