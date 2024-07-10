/*****************************************************************************
** File Name:      mmidc_srv.h                                               *
** Author:                                                                   *
** Date:           06/21/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera work mode                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2012       chunyou             Create
******************************************************************************/

#ifndef _MMIDC_SRV_H_
#define _MMIDC_SRV_H_ 

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
    MMIDC_PHOTO_SIZE_E max_photo_size;
    DCAMERA_DISP_MODE_E disp_mode;
}MMIDCSERV_DC_INFO_T;

typedef struct
{
    VIDEO_SIZE_E max_video_size;
    DRECORDER_DISP_MODE_E disp_mode;
}MMIDCSRV_DV_INFO_T;

typedef struct
{
    MMIDCSRV_WORK_PARAM_T work_param;
    MMIDCSRV_DV_INFO_T dv_info;
    MMIDCSERV_DC_INFO_T dc_info;
}MMIDCSRV_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : init mmidcsrv info
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDCSRV_InitInfo(void);

/*****************************************************************************/
//  Description : get mmidcsrv work mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDCSRV_MODE_E MMIDCSRV_GetWorkMode(void);

/*****************************************************************************/
//  Description : get mmidcsrv dv mem cfg
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDCSRV_MEM_CFG_E MMIDCSRV_GetDVMemCfg(void);

/*****************************************************************************/
//  Description : get mmidcsrv dc mem cfg
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDCSRV_MEM_CFG_E MMIDCSRV_GetDCMemCfg(void);

/*****************************************************************************/
//  Description : get mmidcsrv max video size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDCSRV_GetMaxVideoSizee(void);

/*****************************************************************************/
//  Description : check video size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDCSRV_CheckVideoSize(void);

/*****************************************************************************/
//  Description : check photo size
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDCSRV_CheckPhotoSize(void);

/*****************************************************************************/
//  Description : get photo size
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDCSRV_GetMaxPhotoSize(void);

/*****************************************************************************/
//  Description : check mmidcsrv param
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCSRV_CheckParam(void);

/*****************************************************************************/
//  Description : get mmidcsrv dv display mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DRECORDER_DISP_MODE_E MMIDCSRV_GetDVDisplayMode(void);

/*****************************************************************************/
//  Description : get mmidcsrv dc display mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_DISP_MODE_E MMIDCSRV_GetDCDisplayMode(void);

/*****************************************************************************/
//Description : set dc work parameter
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC MMIDCSRV_WORK_PARAM_T MMIDCSRV_GetDCWorkParam(MMIDCSRV_MEM_CFG_E mem_cfg);

/*****************************************************************************/
//Description : set dc work parameter
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDCSRV_SetWorkParam(MMIDCSRV_WORK_PARAM_T *work_param_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIDC_SRV_H_


