/******************************************************************************
 ** File Name:      mp3_adp.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           12/03/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    MP3 Codec adapter layer.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/03/2005     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

#ifndef _MP3_ADP_H
#define _MP3_ADP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "audio_api.h"
#include "asm.h"
#include "sfs.h"
   
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
//#define MP3_ASSERT_MODE
#ifdef MP3_ASSERT_MODE
	#define MP3_ASSERT_WARNING SCI_ASSERT /*assert verified*/
#else
	#define MP3_ASSERT_WARNING(__cond)  /*assert verified*/
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//~~~~~~~~~~~~stream buffer type definition~~~~~~~~~~~~//
typedef struct 
{

    uint32 uiSampleRate;  
    uint32 uiChannelNum; 
	void* pvOtherInfo;  /*!<Extend stream data format information. */
}MP3_STREAM_FORMATINFO_T; 

typedef struct  
{
	uint32 uiDataLength; /*The length of source data which contained format info*/
	uint8* puiDataBuf; /*Start address of pointer to source data which contained format info*/
}MP3_STREAM_SOURCE_DATA_T; 

union Mp3_stream_data_union
{
    MP3_STREAM_FORMATINFO_T  tMp3FormatInfo;
 	MP3_STREAM_SOURCE_DATA_T tMp3SourceData;
};

typedef struct 
{
    BOOLEAN bType;//SCI_TRUE:   mmi supplies specific format infomation
                  //SCI_FALSE: mmi supplies the source data of mp3 header
	BOOLEAN pad[3];
				  
	union Mp3_stream_data_union unMp3StreamData; 
}MP3_STREAM_BUFFER_T;

/***************************************************************************
**NOTES: 
**     For MP3 stream buffer playback, APP passed mp3 stream format info by the
**     following two ways, which was distinguished by flag(bType) :
**     1) passed specified stream format info directly
**     2) passed source data contained stream format info, need mp3 codec parser.
**
**     The parameter in AUDIO_CreateStreamBufferHandle()
**     const uint8* pucData,   // the music data pointer
**                          //Here is specified that is start address of MP3_STREAM_BUFFER_T 
**     uint32 uiDataLength,    // the music data length
**                         //Here is specified that is sizeof(MP3_STREAM_BUFFER_T)                                  
***************************************************************************/

//~~~~~~~~~~~~mp3 extensive format definition~~~~~~~~~~~~//
/*---xing header---*/
typedef struct  
{
	uint32  totalFrameNum;
	uint32  fileLength;
	uint8   tocTable[100];
	uint32  quality; 
	
	uint32  xingHeaderLen;
       uint8  is_table_existed;
	BOOLEAN islameTagAppend;
       uint8 pad[2];
}MP3_XING_HEADER_INFO_T;

/*---id3v1 tag---*/
typedef struct
{

	uint32 tagVersion;			//xyz->x.yz
	uint32 tagSize;
	uint8  title[32];			//title	  30bytes , OEM code
	uint8  artist[32];			//artist	  30bytes , OEM code
	uint8  album[32];			//album	  30bytes , OEM code
    uint32 year ;				//year 
	uint32 track;               //track
	uint8  comment[32];			//comment  28 or 30bytes, OEM code
	const char* genrePtr;		//genre pointer , ASCII code
	void* pvOtherInfo;			// expand info,reserved for future
}MP3_ID3V1_INFO_T;

/*---id3v2 tag---*/
typedef struct
{
	uint32 tagVersion;// XYZ->X.YZ
	uint32 tagSize;
	uint32 frameNum;            //total number of frames
	
	uint8 title[32];				//frame ID "TIT2" ,OEM code
	uint8 artist[32];			//frame ID "TPE1" , OEM code
	uint8 album[32];				//frame ID "TALB" , OEM code
	uint8 comment[32];			//frame ID "COMM" , OEM code
	uint8 genre[32];				//frame ID "TCON" , ASCII code
	uint8 composer[32]; 			//frame ID "TCOM" ,OEM code
	uint8 language[32];			//frame ID "TLAN" ,OEM code
    uint32 year ;				//frame ID "TYER" 
	uint32 track;               //frame ID "TRCK" 
	void* pvOtherInfo;			// expand info,reserved for future 
}MP3_ID3V2_INFO_T;

/*---ape tag---*/
typedef struct  
{
	uint32 tagVersion;//  xyz->x.yz
	uint32 tagSize;
	uint32 itemCount;
	BOOLEAN bHeader;//is there is a header?
	BOOLEAN bFooter;//is there is a footer?
}MP3_APE_TAG_INFO_T;

/*---lame tag---*/
typedef struct  
{
	uint32   encoderVersion;   //xyz->x.yz
	uint32   tagSize;
	BOOLEAN  isSimpleTag;
	uint8    encoderString[20];//the descrioption of the decoder
	uint32   lowpassFilterHz;  //uint:Hz
	uint32   BitRate;          //uint:bps
	const char*   vbrInfo;
	uint32  musicLength;       //lAME tag + music data ;do not include id3 tags
}MP3_LAME_TAG_INFO_T;

/*---lyrics3 tag---*/
typedef struct  
{
	uint32   tagVersion;   //xyz->x.yz
	uint32   tagSize;
}MP3_LYRICS3_TAG_INFO_T;

//xing header
typedef struct
{
	BOOLEAN bIsXingheader;
	uint32  uiOffset;
	MP3_XING_HEADER_INFO_T xingHeaderInfo;
}MP3_XINGHEADER_FMT_T;

//id3v1 Tag
typedef struct
{
	BOOLEAN bIsId3v1Tag;
	uint32  uiOffset;
	MP3_ID3V1_INFO_T id3v1TagInfo;
}MP3_ID3V1TAG_FMT_T;

//id3v2 Tag
typedef struct
{
	BOOLEAN bIsId3v2Tag; 
	uint32 uiOffset;
	MP3_ID3V2_INFO_T id3v2TagInfo;
}MP3_ID3V2TAG_FMT_T;

//ape Tag
typedef struct
{
	BOOLEAN bIsApeTag;
	uint32  uiOffset;
	MP3_APE_TAG_INFO_T apeTagInfo;
}MP3_APEETAG_FMT_T;

//lame Tag
typedef struct
{
	BOOLEAN bIsLameTag;
	uint32  uiOffset;
	MP3_LAME_TAG_INFO_T lameTagInfo;
}MP3_LAMETAG_FMT_T;

//lyrics3 tag
typedef struct
{
	BOOLEAN bIsLyrics3Tag;
	uint32  uiOffset;
	MP3_LYRICS3_TAG_INFO_T lyrics3TagInfo;
}MP3_LYRICS3TAG_FMT_T;

//MP3 extensive format definition
typedef struct 
{
	uint32  uiMpegVersion;
	uint32  uiSamplesPerFrame;
	uint32  uiFirstFrameOffset;
	uint32  uiMusicDataLen;
       uint32 seek_time;
       uint32 cur_frame_id;
       
       uint8 is_seek;
	BOOLEAN bIsVbr;
	BOOLEAN pad[2];

	MP3_XINGHEADER_FMT_T xingHeader;
	MP3_ID3V1TAG_FMT_T   id3v1Tag;
	MP3_ID3V2TAG_FMT_T   id3v2Tag;
	MP3_APEETAG_FMT_T    apeTag;
	MP3_LAMETAG_FMT_T    lameTag;
	MP3_LYRICS3TAG_FMT_T lyrics3Tag;
}MP3_EXT_FORMATINFO_T;

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
//  Description:    This Function is used to register mp3 codec plugger. 
//  Author:         Benjamin.Wang
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC MP3_RegCodecPlugger(
    HCOLLISION hCollision, 
    AUDIO_CODEC_EXT_CFGINFO_T* tCodecExtCfgInfo
);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _MP3_ADP_H

// End of mp3_adp.h

