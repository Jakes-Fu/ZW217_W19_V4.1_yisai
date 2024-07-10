/******************************************************************************
 ** File Name:      wma_adp.h                                                 *
 ** Author:         Shujing.Dong                                              *
 ** DATE:           03/15/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    It is the adapter layer of WMA for Audio Service.         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/15/2007     Shujing.Dong     Create.                                   *
 ******************************************************************************/

#ifndef _WMA_ADP_H
#define _WMA_ADP_H

#include "audio_api.h"
#include "asm.h"
#include "sfs.h"
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//change the positon to define this macro to the project file for distinguishing 
//platform environment form simulation.
#ifndef _WMA_SIMULATION_
#ifdef WMA_DEBUG 

//if need to know the info of decoding progress, use this macro.
//#define WMA_DEVELOP_DEBUG 

//if need to know the time info of decoding, use this macro.
//#define WMA_TIME_DEBUG  

//if need to debug puase bak info, use this macro.
//#define WMA_PAUSE_BAK_DEBUG  

//#define WMA_EXTERNAL_STATE_DEBUG
#endif
#endif

#ifndef _WMA_SIMULATION_

#ifdef WMA_DEBUG
/*! \def WMA_PRINT(format)	
 *  print info in the debug status
 */
	#ifndef WMA_PRINT
	#define WMA_PRINT( format )		SCI_TRACE_LOW format
	#endif
    #ifndef WMA_TRACE
    #define WMA_TRACE                       SCI_TRACE_LOW
    #endif
  //  #define WMA_FILE_DEBUG
#else
/*! \def WMA_PRINT(format)	
 *  not print info in the not debug status
 */
	#ifndef WMA_PRINT
	#define WMA_PRINT( format )
	#endif	
#ifndef WMA_TRACE
#define WMA_TRACE                       
#endif
#endif

#ifndef WMA_PASSERT
#define WMA_PASSERT SCI_PASSERT 
#endif 

#ifndef WMA_ASSERT
#define WMA_ASSERT SCI_ASSERT 
#endif	

#ifdef WMA_DEVELOP_DEBUG
	#ifndef WMA_DEVELOP_PRINT
	#define WMA_DEVELOP_PRINT( format )		SCI_TRACE_LOW format
	#endif
#else
	#ifndef WMA_DEVELOP_PRINT
	#define WMA_DEVELOP_PRINT( format )
	#endif	
#endif

#ifdef WMA_PAUSE_BAK_DEBUG
	#ifndef WMA_PAUSE_BAK_PRINT
	#define WMA_PAUSE_BAK_PRINT( format )  SCI_TRACE_LOW format
	#endif	
#else
	#ifndef WMA_PAUSE_BAK_PRINT
	#define WMA_PAUSE_BAK_PRINT( format )
	#endif	
#endif

#ifdef WMA_EXTERNAL_STATE_DEBUG
	#ifndef WMA_EXTERNAL_STATE_PRINT
	#define WMA_EXTERNAL_STATE_PRINT( format )  SCI_TRACE_LOW format
	#endif	
#else
	#ifndef WMA_EXTERNAL_STATE_PRINT
	#define WMA_EXTERNAL_STATE_PRINT( format )
	#endif	
#endif

#ifndef WMA_FREE
#define WMA_FREE(_MEM_PTR) \
        {\
        SCI_DisableIRQ();   \
        SCI_FREE(_MEM_PTR);\
        SCI_RestoreIRQ();\
        }
#endif 
#define WMA_ALLOC(_SIZE)    AUDIO_ALLOC(_SIZE)

#else
/*! \def WMA_PRINT(format)	
 *  not print info in the not debug status
 */
#ifndef WMA_PRINT
#define WMA_PRINT( format )
#endif	

#ifndef WMA_DEVELOP_PRINT
#define WMA_DEVELOP_PRINT( format )
#endif

#ifndef WMA_PAUSE_BAK_PRINT
#define WMA_PAUSE_BAK_PRINT( format )
#endif

#ifndef WMA_EXTERNAL_STATE_PRINT
#define WMA_EXTERNAL_STATE_PRINT( format )
#endif		

#ifndef WMA_PASSERT
#define WMA_PASSERT(_EXP,PRINT_STR) assert(_EXP)
#endif 

#ifndef WMA_ASSERT
#define WMA_ASSERT(_EXP)  assert(_EXP) 
#endif	

#ifndef WMA_ALLOC
#define WMA_ALLOC(_SIZE)    malloc(_SIZE)
#endif

#ifndef WMA_FREE
#define WMA_FREE(_MEM_PTR) \
        {\
        free(_MEM_PTR);     (_MEM_PTR) = PNULL;\
        }
#endif
#endif


/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

 /*! \typedef AUDIO_WMA_WRITE_PCM_DATA_CALLBACK_PFUNC
 *  \brief WMA SEND PCM DATA OUT. 
 */
typedef void  (* AUDIO_WMA_WRITE_PCM_DATA_CALLBACK_PFUNC) 
( 
    uint32 uiTrackNum, 
    int16* pusLeftData, 
    int16* pusRightData, 
    uint32 uiCount
);  

// audio/video stream information structure definition
typedef struct 
{
    uint32	   bHasValidAudio;			// if there is a valid audio stream , necessary

    uint16     u16WMAVersion;			// WMA version number, 1 or 2. , unnecessary
    uint16     u16Channels;			    // number of channels, 1 or 2., necessary

    uint16     u16BitsPerSample;		// bits per sample, always 16    , necessary
    uint16	    wFormatTag;				// compression type	, necessary

    uint16     u16EncodeOpt;                 // necessary
    uint16     pad;


    uint32     u32BitRate;				// bitrate, in bits per second, necessary
    uint32     u32SamplesPerSec;		// sampling rate , necessary
    uint32     u32Duration;			    // duration in miliseconds, unnecessary
    uint32	    bHasDRM;				    // if there is a DRM, unnecessary
    uint32     u32BlockAlign;

    uint32	    u32AudioSize;              //unnecessary
    uint32     u32PacketSize;          //unnecessary
} DEMUX_AUDIO_INFO_T, WMA_DEMUX_AUDIO_INFO_T;

//info of wma data buffer,  which is from demux and sent to decode.
 typedef struct WMA_DATA_BUFFER_INFO
{    
    uint8 * uc_wma_data_ptr;//get address of wma data 
    uint32 ui_wma_data_len;//get length of wma_data 
}WMA_DATA_BUFFER_INFO_T;

//info provided by mmi/app when stream buffer play .
typedef enum
{
    WMA_STREAMBUFFER_HEADER_TYPE_FORMAT,
    WMA_STREAMBUFFER_HEADER_TYPE_DATA    
}WMA_STREAMBUFFER_HEADER_TYPE_E;

typedef struct
{
    WMA_DEMUX_AUDIO_INFO_T demux_audio_info_t;//get from demux and used in decoding.
    AUDIO_FORMATINFO_T           audio_format_info_t;//used for displaying by mmi/app.        
}WMA_STREAMBUFFER_HEADER_FORMAT_T;

typedef struct
{
    uint8 * uc_src_data_ptr;
    uint32  ui_src_data_len;     
}WMA_STREAMBUFFER_HEADER_DATA_T;

typedef struct WMA_Codec_Seek_Info
{
    BOOLEAN b_seek_occur;//true:seek occur and not take effect, false:not seek or seek has taken effect.
    uint8 pad0;
    uint16 pad2;
    uint32 ui_seek_offset; //seek dest offset, unit:byte.
}WMA_CODEC_SEEK_INFO_T;

typedef union
{

    WMA_STREAMBUFFER_HEADER_FORMAT_T header_format_t;
    WMA_STREAMBUFFER_HEADER_DATA_T      header_data_t;
}WMA_STREAMBUFFER_HEADER_CONTENT_U;

typedef struct
{
    WMA_STREAMBUFFER_HEADER_TYPE_E header_type;
    WMA_STREAMBUFFER_HEADER_CONTENT_U header_content_u;
}WMA_STREAMBUFFER_HEADER_INFO_T;
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC WMA_RegCodecPlugger(
//!    HCOLLISION h_collision,
//!    AUDIO_CODEC_EXT_CFGINFO_T *ptCodecExtCfgInfo
//!    )
//! \param hCollision  the collision handle which will be used.
//! \param ptCodecExtCfgInfo aac extend config info
//! \return SUCCESS - return the codec handle created
//!            FAILURE  - return SCI_NULL
//! \brief  Description:  This function is used to create an codec handle.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOCODEC WMA_RegCodecPlugger( 
    HCOLLISION hCollision, 
    AUDIO_CODEC_EXT_CFGINFO_T *ptCodecExtCfgInfo
    );
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _WMA_ADP_H

// End of wma_adp.h


