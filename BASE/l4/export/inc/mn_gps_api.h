/******************************************************************************
 ** File Name:      mn_gps_api.h                                                  *
 ** Author:         cuifang deng                                            *
 ** Date:           06/06/2017                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 06/06/2017    cuifang.deng      Create.                                   *
 ******************************************************************************/
 
//#ifdef _MNGPS_SUPPORT_
#if !defined (_MN_GPS_API_H)
#define _MN_GPS_API_H
#include "mn_type.h"
#include "mn_gps_type.h"

/******************************************************************************
 **                        MNGPS MODULE API                                   *
 ******************************************************************************/

/*****************************************************************************/
//  Description : send  gps DOWNLOAD request
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_DownloadReq(
              MN_DUAL_SYS_E dual_sys
              );


/*****************************************************************************/
//  Description : send  gps INIT request
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_InitReq(
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : send  gps  START request
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_StartGpsReq(
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : send  gps  STOP request
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_StopGpsReq(
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : send  gps  RESET request
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_ResetGpsReq(
              MN_DUAL_SYS_E dual_sys,
              MNGPS_RESET_MODE_E reset_mode            
              );

/*****************************************************************************/
//  Description : send  gps  READ STATUS request
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_ReadStatusReq(
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : send  gps  READ INFO request
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_ReadInfoReq(
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : send  gps  OUTPUT request
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_OutputReq(
              MN_DUAL_SYS_E dual_sys,
              uint16  type,
              uint32 time_interval
              );

/*****************************************************************************/
//  Description : send  gps  POWER OFF request
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_PowerOffReq(
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : get  gps  INFO 
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_SendPdpInfoReq(
              MN_DUAL_SYS_E dual_sys,
              uint8 pdp_id,
              uint8 nsapi
              );

/*****************************************************************************/
//  Description : send request to assert gps
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_SendAssertReq(
              MN_DUAL_SYS_E dual_sys,
              uint8  type
              );

/*****************************************************************************/
//  Description : get  gps  INFO 
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNGPS_GetStatusInfo(
              MN_DUAL_SYS_E dual_sys,
              MNGPS_STATUS_INFO_T  *status_info
              );
#endif /* end of _MN_GPS_API_H */
//#endif

