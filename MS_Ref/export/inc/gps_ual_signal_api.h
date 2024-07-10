/******************************************************************************
 ** File Name:      gpa_ual_signal_api.h                                      *
 ** Author:        cuifan.deng                                               *
 ** Date:           01/06/2017                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file defines constants and typedef used in 	  *												    
 **                 phone module. 											  *											 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 01/06/2017         cuifan.deng           Create.                                   *
 ******************************************************************************/
//#ifdef _MNGPS_SUPPORT_
#if !defined (_GPS_UAL_SIGNAL_API_H)
#define       _GPS_UAL_SIGNAL_API_H


//#include "mn_main.h"
#include "gps_signal_def.h"

/*****************************************************************************/
// 	Description : Mn layer sent DOWNLOAD request
//	Global resource dependence :
//   Auhor:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendDownloadReq(
	unsigned int dual_sys
	);


/*****************************************************************************/
// 	Description : Mn layer sent INIT request
//	Global resource dependence :
//   Auhor:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendInitReq(
	unsigned int dual_sys
	);

/*****************************************************************************/
// 	Description : Mn layer sent START gps request
//	Global resource dependence :
//   Auhor:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendStartReq(
	unsigned int dual_sys,
	uint8 type,
	uint32 start_mode
	);


/*****************************************************************************/
// 	Description : Mn layer sent STOP gps request
//	Global resource dependence :
//   Author:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendStopReq(
	unsigned int dual_sys
	);

/*****************************************************************************/
// 	Description : Mn layer sent RESET gps request
//	Global resource dependence :
//   Author:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendResetReq(
	unsigned int dual_sys,
       LIBGPS_RESET_MODE_E     reset_mode
	);

/*****************************************************************************/
// 	Description : Mn layer sent READ STATUS request
//	Global resource dependence :
//   Author:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendReadStatusReq(
	unsigned int dual_sys
	);

/*****************************************************************************/
// 	Description : Mn layer sent READ INFO request
//	Global resource dependence :
//   Author:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendReadInfoReq(
	unsigned int dual_sys
	);

/*****************************************************************************/
// 	Description : Mn layer sent OUTPUT request
//	Global resource dependence :
//   Author:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendOutputReq(
	unsigned int dual_sys,
	uint16  type,
	uint32 time_interval
	);

/*****************************************************************************/
// 	Description : Mn layer sent POWEROFF request
//	Global resource dependence :
//   Auhor:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendPowerOffReq(
	unsigned int dual_sys
	);

/*****************************************************************************/
// 	Description : Mn layer sent PDP INFO  request
//	Global resource dependence :
//   Auhor:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendPdpInfoReq(
	unsigned int dual_sys,
	uint32 netid
	);


/*****************************************************************************/
// 	Description : Mn layer sent assert gps request
//	Global resource dependence :
//   Auhor:	cuifang.deng
//	Note: 
/*****************************************************************************/
void libGPS_SendAssertReq(
	unsigned int dual_sys,
	uint8 type
	);

/*************************************************************************
//  Description : get the gnss work mode
//  Global resource dependence :
//  Auhor: cheng.liu3
//  Note: gnss work mode
**************************************************************************/
int libGPS_GetCpMode(void);

/*****************************************************************************/
// 	Description : ual layer sent GpsRun request
//	Global resource dependence :
//  Auhor:	cheng.liu3
//	Note: 
/*****************************************************************************/
void libGPS_SendGpsRunReq(unsigned int dual_sys);

/*****************************************************************************/
// 	Description : ual layer sent ap node request
//	Global resource dependence :
//  Auhor:	zehai.chang
//	Note:
/*****************************************************************************/
void libGPS_SendApNodeReq(unsigned int dual_sys, LIBGPS_APNODE_INFO_T *p_apnode_info);

#endif /* _MNPHONE_MODULE_API_H */
//#endif
