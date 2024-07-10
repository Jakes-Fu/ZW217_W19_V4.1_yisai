/******************************************************************************
** File Name:      vt_adp.h                                              
** Author:         shujing.dong                                              
** DATE:           12/14/2009                                                
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces of vt codec.
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 12/14/2009     Shujing.Dong    Create.                                               *
******************************************************************************/
#ifndef _VT_ADP_H
#define _VT_ADP_H
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "apm_codec.h"
#include "asm.h"
#include "audio_config.h"
/**---------------------------------------------------------------------------**
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------**/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/
#define VT_EXT_OPER_START_PARA_COUNT 5	
#define VT_EXT_OPER_STOP_PARA_COUNT 1
#define VT_EXT_OPER_START_RECORD_PARA_COUNT 5
#define VT_EXT_OPER_STOP_RECORD_PARA_COUNT 1	
#define VT_EXT_OPER_WRITE_VOICEDATA_PARA_COUNT 1
#define VT_EXT_OPER_READ_VOICEDATA_PARA_COUNT 1
#define VT_EXT_OPER_READ_RECORDDATA_PARA_COUNT 1	
#define VT_EXT_OPER_APP_SEND_VOICEDATA_PARA_COUNT 1
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef struct
{
    uint16 * us_source_buf_ptr;
    uint16    us_data_length;
}VT_EXT_OPER_DATA_T;   

typedef struct
{
    uint8 * uc_source_buf_ptr;
    uint32    ui_data_length;
}VT_EXT_OPER_DATA_BYTE_T; 

typedef struct 
{  
    HAUDIO    hAudioHandle; 
    PLAY_CALLBACK play_callback;
    RECORD_CALLBACK record_callback;
    uint16 para0;
    uint16 para1;    
}VT_EXT_OPER_START_PARA_T;

typedef struct 
{  
    HAUDIO    hAudioHandle;   
}VT_EXT_OPER_STOP_PARA_T;

typedef struct
{
    HAUDIO  hAudioHandle;    	
    AUD_REC_FORMAT_E recorder_format;
    RECORD_CALLBACK callback_fun;
    BOOLEAN is_from_mic;
    BOOLEAN is_from_net;  
}VT_EXT_OPER_START_RECORD_T;

typedef struct 
{  
    HAUDIO    hAudioHandle;   
}VT_EXT_OPER_STOP_RECORD_T;

typedef struct 
{  
    VT_EXT_OPER_DATA_T write_voicedata_t;   
}VT_EXT_OPER_WRITE_VOICEDATA_PARA_T;

typedef union
{
	VT_EXT_OPER_DATA_T read_voicedata_2byets_t;  
	VT_EXT_OPER_DATA_BYTE_T read_voicedata_byte_t;  
}VT_EXT_OPER_DATA_UNION;

typedef struct 
{  
	uint32 uiDataType;//0:half-word, 1:byte
	VT_EXT_OPER_DATA_UNION read_voicedata_t;
}VT_EXT_OPER_READ_VOICEDATA_PARA_T;

typedef struct 
{  
    VT_EXT_OPER_DATA_T read_recorddata_t;   
}VT_EXT_OPER_READ_RECORDDATA_PARA_T;

//For current audio I/F, the downlink AMR frame data are saved in buffer by 
//using half_word(16bits) format. And the buffer length should be 32 half_words(64bytes), 
//the unit of length also is halv_word(16bits). Below is the table for the data format.
//byte0	Byte1	Byte2	Byte3	бн	бн	Byte30	Byte31	бн	бн	Byte60	Byte61	Byte62	Byte63
//0x00	amr_B0	0x00	amr_B1	бн	бн	0x00	amr_B15	бн	бн	0x00	amr_B30	0x00	Err_type
//The Err_type is only used in downlink path. 
//If  Err_type is equal to zero, it means this amr frame data is ok;  
//If  Err_type is equal to 1, it means that crc_error or other error has been check out 
//             in this amr frame data, the data may be destroyed through wireless path; 
//If  Err_type is equal to 2, it means this amr frame data is one stuffing frame, 
//             no amr downlink data is cached. 
typedef struct 
{  
	uint8 uc_crc_flag;//0:good, 1:bad, 2:none.
    VT_EXT_OPER_DATA_BYTE_T write_voicedata_t;   
}VT_EXT_OPER_APP_SEND_VOICEDATA_PARA_T;
/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/
	
	
/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/
//symbol of this codec:
//ANSI2UINT16("VT_CODEC"); 

//symbol of extend operations:
//ANSI2UINT16("VT_EXT_START");
//ANSI2UINT16("VT_EXT_STOP");
//ANSI2UINT16("VT_EXT_STARTRECORD");
//ANSI2UINT16("VT_EXT_STOPRECORD");
//ANSI2UINT16("VT_EXT_WRITEVOICEDATA");
//ANSI2UINT16("VT_EXT_READVOICEDATA");
//ANSI2UINT16("VT_EXT_READRECORDDATA");
//ANSI2UINT16("VT_EXT_APPSENDDLKDATA");


//There are two ways to work.
//1.app offer downlink data actively:
//1)the play_callback in VT_EXT_OPER_START_PARA_T should be set null when call the 
//  extend operation:VT_EXT_START.
//2)offer the downlink data timly by extend operation:VT_EXT_APPSENDDLKDATA
//2.app offer downlink data passively: offer the downlink data only when 
// dsp interrupt(layer1/ref/dv)
// app layer of audio service should offer the play_callback in the extend operation
//   VT_EXT_OPER_START_PARA_T


//app can get uplink amr data in bytes or in halfword way by set different value 
// when execute extend operation: VT_EXT_OPER_READ_VOICEDATA_PARA_T
//the data is put in bigendia way in halfword way:
//
/**---------------------------------------------------------------------------**
 **                          Function Declare                                 **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//  Description:    This Function is used to initialize the codec. 
//  Author:         Shujing.Dong
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC VT_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo
    );
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _VOICE_ADP_H

//end of file
