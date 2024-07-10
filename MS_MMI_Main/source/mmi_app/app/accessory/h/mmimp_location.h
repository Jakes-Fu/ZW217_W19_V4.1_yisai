/*****************************************************************************
** File Name:		mmifindmp_infor.c                                                
** Author:			zhenhao.huang
** Date:            2007/10
** Copyright:		2007 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:		This file is used to describe mmi stopwatch common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE        	NAME             	DESCRIPTION                               *
** 2007/10/29	zhenhao.huang		Create                                   *
******************************************************************************/
#ifndef MMIFINDMP_INFOR_H_
#define MMIFINDMP_INFOR_H_

#define MMIFINDMP_CITYNAME_LENGTH	30

#ifdef HOME_LOCATION_INQUIRE_SUPPORT
#define MPNB_TOTAL_LENGTH           11						// the length of mobile phone number
#define MPNB_USE_LENGTH             7						// just extracting the first 7 digits

#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
#define  R_ACC_CITY_TYPE_QUHAO_TCARD  "E:\\location\\R_ACC_CITY_TYPE_QUHAO.pnl"
#define  R_ACC_CITYID_QUHAO_TCARD     "E:\\location\\R_ACC_CITYID_QUHAO.pnl"
#define  R_ACC_MPLOCSTR_TCARD         "E:\\location\\R_ACC_MPLOCSTR.pnl"
#define  R_ACC_QUHAOCITY_TCARD        "E:\\location\\R_ACC_QUHAOCITY.pnl"
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//  Mender: Mingxuan.Zhu
//	Note: return length of location_str, if not found return 0
//        this function is invoked by atest, which is added a outputed param named phone_location.
/*****************************************************************************/
PUBLIC uint16 MMIAPI_FindMPLocation(	uint8*   tel_num_str,		// input phone number string in ASCII
                                        int32    tel_num_length,    // string length
                                        uint16*  location_str,	    // output string, length is MMIFINDMP_CITYNAME_LENGTH
                                        char*    phone_location     // output param : get the name of the phone location
                                    );

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//	Note: return the ID of location, if not found return 0
/*****************************************************************************/
PUBLIC uint16 MMIAPI_FindMPLocationID(	uint8* tel_num_str,		// input phone number string in ASCII
										int32  tel_num_length	// string length
									);

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//	Note: return the ID of location, if not found return 0
/*****************************************************************************/
PUBLIC void MMIAPI_FindMPLocationText(	uint8* city_name,	// output in GB2313, buffer length is MMIFINDMP_CITYNAME_LENGTH
										uint16 city			// input city ID
									);

#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
/*****************************************************************************/
// 	Description : get home location data by phone string
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
//ATEST_SUPPORT
#include "..\\..\\..\\common\\h\\Mmi_ring.h"
PUBLIC void GetAreaCodeData( 
                                  uint8* file_path,  //in
                                  MMIAUD_RING_DATA_INFO_T *file_info_ptr  //in/out
                                  );
#endif
#endif//HOME_LOCATION_INQUIRE_SUPPORT
#endif// MMIFINDMP_INFOR_H_


