/******************************************************************************
 ** File Name:      datadump.h                                            *
 ** Author:         yaye.jiang                                                 *
 ** DATE:           03/21/2013                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 03/21/2013      yaye.jiang      Create                         *
 ******************************************************************************/
#ifndef _DATADUMP_H
#define _DATADUMP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_service.h"
#include "sci_types.h"
#include "sfs.h"
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/
#define DUMP_DATA_POSITION_MAX_COUNT 32
#define DUMP_CACH_FRAME_COUNT_MIN 2
#define DUMP_FRAME_DATA_MAX_LEN            640//unit:bytes
#define DUMP_DEBUG_DATA_FILE_BUF_SIZE (DUMP_FRAME_DATA_MAX_LEN * 100)//uint:bytes
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef enum
{
    DUMP_NO_ERROR = SCI_SUCCESS,       /*!<Operation success.*/
    DUMP_NO_ENOUGH_MEMORY,
    DUMP_CONFLICT,     /*!<Audio play conflict.*/
    DUMP_PARAM_ERROR,
    DUMP_INVALID_SAMPLERATE,
    DUMP_ERROR = SCI_ERROR /*!<General ERROR.*/
}DUMP_RESULT_E;

typedef struct
{
	SIGNAL_VARS
	uint32 uiPosIndex;
	uint16 *spSrcData;
	uint32 uiDataLen;
    uint32 uiDataIndexOut;
}DATADUMP_SIGNAL_T;

typedef enum
{
    DATA_DUMP_TO_FILE = 0,
    DATA_DUMP_TO_ARM_LOG = 1,
}DATADUMP_OUTPUT_FLAG_E;

#if 0
typedef enum
{
    DATA_DUMP_INTERNAL_ENABLE = 0,
    DATA_DUMP_FIRST_ENABLE = 1,
    DATA_DUMP_LAST_ENABLE = 2,
}DATADUMP_ENABLE_TYPE_E;
#endif
typedef struct
{
    DATADUMP_OUTPUT_FLAG_E uiDumpOutFlag; // 0:to_file; 1:to_armlog;
	uint8  uicompanynum;//0:spredtrum; l:spred_customer1; 2:spred_customer2....
	uint32 uiposflag;//positon flag: bit_0:positon 0 ; bit_1:positon 1;....... 0:disable; 1:enable;
}DATADUMP_NV_INTO_T;

#if 0
typedef struct
{
    BOOLEAN bEnable; //on/off
	BOOLEAN bPointIndexFlag;//true: all positions, false:some pointed positions
	uint32  uiposflag;//when(bPointIndexFlag == false)positon flag: bit_0:positon 0 ; bit_1:positon 1;....... 0:disable; 1:enable;
	//uint8  *pucDebugFileName[DUMP_DATA_POSITION_MAX_COUNT];//if(file) [AUDIO_SFS_DEVICE_LEN];
}DATADUMP_ENALBE_RECORD_DEBUG_DATA_T;
#endif
typedef struct
{
    uint8 *psBuffer;
    uint32 uiCurWriteCount;//uint:bytes
    uint32 uiFlag;//0: can be input; 1: is outputing.
}DATADUMP_RECORD_CACH_INFO_T;

typedef struct
{
	uint32 *puiCompressHandle;
	uint8 *pucCompressBuffer;
	uint32 uiCompressBufSize;//unit: bytes
}DATADUMP_RECORD_CACH_COMPRESS_INFO_T;


typedef struct
{
    BOOLEAN bEnableRecording;// if the pcm data of some position is recording.
    BOOLEAN bDumpToFile; // TRUE: file; FALSE: armlog.
    uint8 *psTotalBuffer;
    DATADUMP_RECORD_CACH_INFO_T  *ptCachData;
    DATADUMP_RECORD_CACH_COMPRESS_INFO_T tCachCompData; //DUMP TO ARMLOG
    uint32 uiCurIndex;//current input cach index
    uint32 uiLostFramesCount;//unit: frame.
    uint32 uiAvailableCount;//total size to save. unit:bytes.
    SFS_HANDLE hDebugFileHandle;    // debug file handle(DUMP TO FILE)
}DATADUMP_RECORD_DEBUG_DATA_INFO_T;

typedef void (*DUMP_DATA_CALLBACK)(
    uint32 uiPositionIndex,
    uint16 *psBuf,
    uint32 uiLength //its unit is bytes.
    );

typedef struct
{
	uint32 uiFrameBytes; //unit:bytes; bytes once write to buffer
	uint32 uiCachCount; //cach number
	DUMP_DATA_CALLBACK pDumpDataCallbackFunc;//encode funciton to armlog
}DATADUMP_CONFIG_DUMP_DATA_PARAM_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: This Function is used to set config data
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC DUMP_RESULT_E Datadump_SetConfigPara(
                                            uint32 uiPosindex,//bit_0:pos_0..... true:set,false:not
                                            void* puiParamStruct
                                            );

/*****************************************************************************/
//  Description: set config from NV
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC DUMP_RESULT_E Datadump_ConfigFromNv(void);

/*****************************************************************************/
//  Description:    This Function is used to enable position.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC DUMP_RESULT_E Datadump_Enable(
                                        BOOLEAN bOn,
                                        uint32  uiposflag//pos flag(bit_0: pos_0 ....) 1:enable
                                        );

/*****************************************************************************/
//  Description: output data to file/armlog
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void Datadump_Output(
                                uint32 uiPosIndex, //position index
                                uint16 *srcData,
                                uint32 uiDataLenByBytes //input bytes once(uint: bytes)
                                );

#ifdef CVSRCD_ENABLE
PUBLIC DUMP_RESULT_E Datadump_Enable_CVSRCD(
    BOOLEAN bOn,
    uint32 is_dump_to_file,
    uint32 uiposflag);

PUBLIC void Datadump_Output_CVSRCD(
    uint32 uiPosIndex,          //position index
    uint16 *srcData,
    uint32 uiDataLenByBytes);     //input bytes once(uint: bytes)
#endif

#endif

