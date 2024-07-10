/****************************************************************************

 *        Copyright 2005 World Peace Group(China). All Rights Reserved.
 *										
 *  This software is copyrighted by and is the property of World Peace Group(China).
 *  All rights are reserved by World Peace Group(China)
 *  This software may only be used in accordance with the	
 *  corresponding license agreement. Any unauthorized use, duplication,		
 *  distribution, or disclosure of this software is expressly forbidden.
 *																
 *  This Copyright notice MUST not be removed or modified without prior		
 *  written consent of World Peace Group(China)							
 *																			
 *  World Peace Group reserves the right to modify this			
 *  software without notice.												
 *																		
 *  	Author: 				Snow Hsu(TME of WPG)														
 *	Create Date:			17:42 pm, Dec 16th,2005
 *	Last Modified Date:		Dec 16th,2005
 *
 *
 *
 *	The CL6012X Driver totally contain 2 files:CL6012X.c and CL6012X.h
 *	CL6012X.c mainly inlcude the function definition 
 *	
 *	CL6012X.h is the macro definition the programmer may use and the API's declaration
 *	
 ***************************************************************************/
#ifndef _CL6012X_H
#define	_CL6012X_H

#define FMDRV_DEBUG
#ifdef FMDRV_DEBUG
#define FMDRV_PRINT	SCI_TRACE_LOW
#else
#define FMDRV_PRINT	
#endif
/*-----------------------------------------
	China,US,Europe	87.5Mhz~108Mhz(8750~10800)
	Japan			76Mhz~91Mhz(7600~9100)
---------------------------------------------*/
#define	CL6012X_FREQ_LOW_LIMIT				8750
#define	CL6012X_FREQ_HIGH_LIMIT				10800

/*	Return Error Code	*/
#define	CL6012X_RET_OK 						0x00
#define	CL6012X_RET_NULLPOINTER 				0x01
#define	CL6012X_RET_TIMEOUT					0x02
#define	CL6012X_RET_REACH_BANDLIMIT			0x04
#define	CL6012X_RET_INVALID_PARAMETER		0x08

/*The following define the cl6018 volume level*/
#define CL6012X_VOLUME_LEVEL0				0x0<<2
#define CL6012X_VOLUME_LEVEL1				0x1<<2
#define CL6012X_VOLUME_LEVEL2				0x2<<2
#define CL6012X_VOLUME_LEVEL3				0x4<<2
#define CL6012X_VOLUME_LEVEL4				0x6<<2
#define CL6012X_VOLUME_LEVEL5				0x8<<2
#define CL6012X_VOLUME_LEVEL6				0xa<<2
#define CL6012X_VOLUME_LEVEL7				0xc<<2
#define CL6012X_VOLUME_LEVEL8				0xe<<2
#define CL6012X_VOLUME_LEVEL9				0xf<<2


/*The following define the band*/
#define	CL6012X_CHINA_BAND					0x0<<6
#define	CL6012X_JAPAN_BAND					0x2<<6

/*	The following define the search rssi strength level	*/
#define	CL6012X_SSL_LEVEL_LOW				0x80
#define	CL6012X_SSL_LEVEL_MID				0xa0
#define	CL6012X_SSL_LEVEL_HIGH				0xb0

/*The following are cl6018 reg list*/
#define CL6012X_STATUS_RSSI					0x00
#define CL6012X_READ_CHAN					0x01
#define CL6012X_QOS							0X02
#define CL6012X_FUNCTION						0x03
#define CL6012X_CHANNEL_VOLUME				0x04
#define CL6012X_SYSCONFIG					0x05
#define CL6012X_SEEK							0x06
#define CL6012X_MUTE							0x07
#define CL6012X_POWERDOWN					0x08
#define CL6012X_REG_MAX						0x09

PUBLIC extern FM_OPERATION_T g_CL6012X_ops;
#endif

