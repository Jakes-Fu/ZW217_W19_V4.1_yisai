/*****************************************************************************
** File Name:		mmiphonesearch_export.h                                                
** Author:			zhenhao.huang
** Date:        2007/11
** Copyright:		2007 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description: This file is used to describe mmi phonesearch interface    *
******************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE        	NAME             	DESCRIPTION                             	*
** 2007/11/16	  zhenhao.huang		Create   
** 2012/02/10	  murphy.xie		  Change for interface clear up                *
******************************************************************************/
#ifndef _MMIPHONESEARCH_EXPORT_H_
#define _MMIPHONESEARCH_EXPORT_H_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MMIPHONESEARCH_LOCALNUMBER	1

/*****************************************************************************/
// 	Description : open the wnd of phone search
//	Global resource dependence : none
//  Author: zhenhao.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONESEARCH_OpenWnd(void);

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//	Note: return length of prompt_str, if not found return 0
/*****************************************************************************/
PUBLIC uint16 MMIAPIPHONESEARCH_FindLocation(	
								uint8* tel_num_str,		// input phone number string in ASCII
								int32  tel_num_length,  // string length
								uint16* location_str	// output string, length is MMIFINDMP_CITYNAME_LENGTH
								);

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//  Mender: Mingxuan.Zhu
//	Note: return length of location_str, if not found return 0
//        this funciton is invoked by atest, which is added a output param named phone_location.
/*****************************************************************************/
PUBLIC uint16 MMIAPI_FindPhoneLocation(	
                                       uint8*   tel_num_str,	// input phone number string in ASCII
                                       int32    tel_num_length, // string length
                                       uint16*  location_str,	// output string, length is MMIFINDMP_CITYNAME_LENGTH
                                       char*    phone_location  // get the name of the phone location.
                                      );
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
/*******************************************************************/
//  Description : 	MMIAPIPHONESEARCH_RegBrowserCB
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIPHONESEARCH_RegBrowserCB(void);
#endif
#endif// MMIPHONESEARCH_WINTAB_H_
