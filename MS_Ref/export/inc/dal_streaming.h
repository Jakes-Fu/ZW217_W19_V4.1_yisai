/******************************************************************************
 ** File Name:      AP_VT_API.h                                              
 ** Author:         shunyi.wang                                              
 ** DATE:           8/24/2006                                                
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:    This file defines the basic macro, struct and enums 
 **
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 **  -----------------------------------------------------------------------  
 ** DATE           NAME             DESCRIPTION                               
 ** 8/24/2006     shunyi.wang     Create.                                   
 ******************************************************************************/


#ifndef _DAL_STREAMING_H
#define _DAL_STREAMING_H


#include "sci_types.h"
typedef struct{
    uint16	   	left;        	   	/*!<left up point coordinate x */
    uint16	   	top;           	   	/*!<left up point coordinate y */
    uint16	   	right;         	   	/*!<right down point coordinate x */
    uint16	   	bottom;       	   	/*!<right down point coordinate y */
}AP_ST_RECT_T;


typedef struct AP_ST_OSTDATA_TAG
{
    AP_ST_RECT_T OSD_Rect;
    uint16* Image; 
    uint16 BackColor;
    uint16 u16pad;
}AP_ST_OSTDATA_T;

typedef enum{
    STREAMING_AP_PARTLCD,
    STREAMING_AP_FULLLCD
}STREAMING_AP_DISMODE_E;



/*****************************************************************************/
//  Description:     
//  Author:     Jack.Chen
//  Note:   Nothing
/*****************************************************************************/ 
PUBLIC uint32 Streaming_DeviceInit(void);

/*****************************************************************************/
//  Description:   
//  Author:     Jack.Chen
//  Note:   Nothing
/*****************************************************************************/ 
PUBLIC uint32 Streaming_DeviceDeinit(void);

/*****************************************************************************/
//  Description:  
//  Author:     Jack.Chen
//  Note:   Nothing
/*****************************************************************************/ 
PUBLIC uint32 Streaming_SetDisplayMode(STREAMING_AP_DISMODE_E mode);


/*****************************************************************************/
//  Description:  
//  Author:     Jack.Chen
//  Note:   Nothing
/*****************************************************************************/ 
uint32 Streaming_VideoDecoderInit
    ( unsigned int uCodecObjectTypeIndication
        , unsigned char* pConfig
        , int iLen
        , unsigned char* pConfigEnhance
        , int iEnhLen
        , int iNALHeaderLengthSize
        , int* piWidth
        , int* piHeight
        , int* piPitch
        , unsigned int* puUserData );


/*****************************************************************************/
//  Description:   
//  Author:     Jack.Chen
//  Note:   Nothing
/*****************************************************************************/ 
PUBLIC uint32 Streaming_VideoDecoderDecode(BOOLEAN enable);

/*****************************************************************************/
//  Description:  
//  Author:     Jack.Chen
//  Note:   Nothing
/*****************************************************************************/ 
PUBLIC uint32 Streaming_VideoDecoderDeinit(void);


/*****************************************************************************/
//  Description: 
//  Author:     Jack.Chen
//  Note:   Nothing
/*****************************************************************************/ 
PUBLIC uint32 Streaming_AudioDecoderPutData(uint8* pinbuf, uint32  bufsize);



/*****************************************************************************/
//  Description: 
//  Author:     Jack.Chen
//  Note:   Nothing
/*****************************************************************************/ 
PUBLIC unsigned int Streaming_AudioDecoderInit( unsigned int uOTI
	, unsigned char* pConfig
	, int iConfigLen
	, unsigned char* pFrame
	, int iFrameLen
	, unsigned int* puSamplingRate
	, unsigned int* puNumOfChannels
	, unsigned int* puBitsPerSample
	, unsigned int* puNumOfSamplesPerChannel
	, unsigned int* puUserData );


PUBLIC uint32 Streaming_AudioDecoderDeinit(void);
PUBLIC uint32 Streaming_Setosd( AP_ST_RECT_T OSD_Rect, uint16* Image, uint16 BackColor);
PUBLIC uint32 Streaming_DeviceInit(void );
PUBLIC uint32 Streaming_VideoDecode(unsigned char* pData, int iLen, unsigned int uDTS, int iEnhancement, unsigned int* puDecodeResult, unsigned int uUserData );


#endif


