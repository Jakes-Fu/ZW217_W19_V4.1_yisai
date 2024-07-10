/******************************************************************************
 ** File Name:      wav_adp.h                                                 *
 ** Author:         Chrry.Liu                                              *
 ** Date:           5/02/2008                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                            *
*******************************************************************************
*******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 5/02/2008     Chrry.Liu         Create.                                   *
*******************************************************************************/
#ifndef _WAV_ADP_H_
#define _WAV_ADP_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro definitions                                     *
 **---------------------------------------------------------------------------*/
#define WAV_EXT_OPE_COUNT 2
#define WAV_EXT_OPE_SHAREDMODE_COUNT 2

/**---------------------------------------------------------------------------*
 **                         Structures                                     *
 **---------------------------------------------------------------------------*/
//wav stream buffer type definition~~~~~~~~~~~~
typedef enum WAV_Subtype_enum
{
    WAV_IMAADPCM,  
    WAV_PCM,
    WAV_PCM_BIGENDIAN
}WAV_SUB_TYPE_E;

typedef struct 
{
	WAV_SUB_TYPE_E eSubtype;
    uint32 uiSampleRate;  
	uint32 uiBitPerSample;  
    uint16 uiChannelNum; 
    uint32 uiDataSize; /*Equal sizeof(WAV_STREAM_BUFFER_T)*/
	uint32 uiBlockAlign;
	void* pvOtherInfo;  /*!<Extend stream data format information. */
}WAV_STREAM_FORMATINFO_T;

typedef struct  
{
	uint32 uiDataLength; /*The length of wav stream header data */
	uint8* puiDataBuf; /*Start address of pointer to wav stream header data */
}WAV_STREAM_HEADER_DATA_T; 

union Wav_stream_data_union
{
    WAV_STREAM_FORMATINFO_T  tWavFormatInfo;
 	WAV_STREAM_HEADER_DATA_T tWavHeaderData;

};

typedef struct 
{
    BOOLEAN bType;//SCI_TRUE:   mmi supplies specific format infomation
                  //SCI_FALSE: mmi supplies the source data of music header
	BOOLEAN pad[3];
				  
	union Wav_stream_data_union unWavStreamData; 
}WAV_STREAM_BUFFER_T;

/****************************************************************************
**NOTES: 
**     For WAV stream buffer playback, APP passed wav stream format info by the
**     following two ways, which was distinguished by flag(bType) :
**     1) passed specified stream format info directly
**     2) passed source data contained stream format info, need wav codec parser.
**
**     The parameter in AUDIO_CreateStreamBufferHandle()
**     const uint8* pucData,   // the music data pointer
**                         //Here is specified that is start address of WAV_STREAM_BUFFER_T 
**     uint32 uiDataLength,    // the music data length
**                         //Here is specified that is sizeof(WAV_STREAM_BUFFER_T)                                  
*****************************************************************************/

//wav extensive format definition~~~~~~~~~~~~
typedef struct 
{
	WAV_SUB_TYPE_E  eSubType;
	uint32 uiFmtChunkOffset;
    uint32 uiFmtChunkSize;
	uint32 uiDataChunkOffset;
    uint32 uiDataChunkSize;
	uint32 uiAudioDataOffset;
	uint64 uiAudioDataSize;	   
    uint32 uiBlockAlign;
    uint32 uiBitPerSample;
}WAV_EXT_FORMATINFO_T;

typedef struct 
{  
	HAUDIO	hAudioHandle; 
    uint32	uiPlaytimes; 
}WAV_EXT_OPE_PARAM_T;

typedef struct 
{  
	HAUDIO	hAudioHandle; 
    BOOLEAN	bIsSharedMode; 
}WAV_EXT_OPE_SHAREDMODE_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This Function is used to register wav codec plugger. 
//  Author:         Benjamin.Wang
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC WAV_RegCodecPlugger(
    HCOLLISION hCollision, 
    AUDIO_CODEC_EXT_CFGINFO_T* tCodecExtCfgInfo
);

              
#ifdef __cplusplus
    }
#endif

#endif /* _PCM_ADP_H_ */

/*~~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~*/

