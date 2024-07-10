/******************************************************************************
 ** File Name:      sfc_drvapi.h                                                 *
 ** Author:         Fei.Zhang                                                 *
 ** DATE:           01/15/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************/

#ifndef _SFC_DRVAPI_H_
#define _SFC_DRVAPI_H_

#include "sci_types.h"

#define SFC_INT_STATUS  BIT_0
#define SFC_IDLE_STATUS BIT_1


// for new flash modify
#if defined(FLASH_SIZE_128MBITX32MBIT) || defined(MULTI_FLASH_SUPPORT)
#define SFC_MAX_CS    2
#else
#define SFC_MAX_CS    1
#endif



#if defined(PLATFORM_UWS6121E)
typedef enum BIT_MODE_E_TAG
{
    BIT_MODE_1 = 0,
    BIT_MODE_2,
    BIT_MODE_4,
    BIT_MODE_MAX
}BIT_MODE_E;

typedef enum SEND_MODE_E_TAG
{
    SEND_MODE_0 = 0,    //byte0,1,2,3 order
    SEND_MODE_1,        //byte3,2,1,0 order
    SEND_MODE_MAX
}SEND_MODE_E;

PUBLIC uint32 SFCDRV_GetStatus(int cs);

/*****************************************************************************/
//  Description:  set clock config register
//  Author: Fei.zhang
//  Param:
//      value: clock configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_ClkCfg (int cs, uint32 value);

/*****************************************************************************/
//  Description:  set CS config register
//  Author: Fei.zhang
//  Param:
//      value: CS configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_CSCfg (uint32 value);

#else
#define SFC_DEBUG
#define SFC_DEBUG_BUF_LEN 48

typedef enum CMD_MODE_E_TAG
{
    CMD_MODE_WRITE = 0,
    CMD_MODE_READ,
    CMD_MODE_MAX
}CMD_MODE_E;

typedef enum BIT_MODE_E_TAG
{
    BIT_MODE_1 = 0,
    BIT_MODE_2,
    BIT_MODE_4,
    BIT_MODE_MAX
}BIT_MODE_E;

typedef enum INI_ADD_SEL_E_TAG
{
    INI_CMD_BUF_7 = 0,
    INI_CMD_BUF_6,
    INI_CMD_BUF_5,
    INI_CMD_BUF_4,
    INI_CMD_BUF_MAX
}INI_ADD_SEL_E;

typedef enum CMD_BUF_INDEX_E_TAG
{
    CMD_BUF_0 = 0,
    CMD_BUF_1,
    CMD_BUF_2,
    CMD_BUF_3,
    CMD_BUF_4,
    CMD_BUF_5,
    CMD_BUF_6,
    CMD_BUF_7,
    CMD_BUF_8,
    CMD_BUF_9,
    CMD_BUF_10,
    CMD_BUF_11,
    CMD_BUF_MAX
}CMD_BUF_INDEX_E;

typedef enum SEND_MODE_E_TAG
{
    SEND_MODE_0 = 0,    //byte0,1,2,3 order
    SEND_MODE_1,        //byte3,2,1,0 order
    SEND_MODE_MAX
}SEND_MODE_E;

typedef enum BYTE_NUM_E_TAG
{
    BYTE_NUM_1 = 0,
    BYTE_NUM_2,
    BYTE_NUM_3,
    BYTE_NUM_4,
    BYTE_NUM_MAX
}BYTE_NUM_E;

/* SPI CMD&DATA transfer flags */
#define SPI_CMD_DATA_BEGIN  0x01            /* transfer begin */
#define SPI_CMD_DATA_END    0x02            /* transfer end   */
typedef struct _sfc_cmd_des
{
    uint32     cmd;
    uint32     cmd_byte_len;
    BOOLEAN    is_valid;

    CMD_MODE_E cmd_mode;    //write=0, read=1
    BIT_MODE_E bit_mode;    //SPI/DPI/QPI
    SEND_MODE_E send_mode;  //0--byte 0123 order; 1--byte 3210 order
} SFC_CMD_DES_T, *SFC_CMD_DES_PTR;

/*****************************************************************************/
//  Description:  set cmd_cfg register
//  Author: Fei.zhang
//  Param:
//      cmdmode: read or write operation
//      bitmode: AHB read data bit mode, 1bit ,2bit and 4bit canbe used
//      iniAddSel: Read back status information initial address selection
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_SetCMDCfgReg (int cs,CMD_MODE_E cmdmode, BIT_MODE_E bitmode, INI_ADD_SEL_E iniAddSel);

PUBLIC void SFCDRV_SoftReq(void);

/*****************************************************************************/
//  Description:  read sfc status register
//  Author: Fei.zhang
//  Param:
//
//  Return:
//      sfc status value: SFC_INT_STATUS/SFC_IDLE_STATUS
//  Note:
/*****************************************************************************/
PUBLIC uint32 SFCDRV_GetStatus(void);

/*****************************************************************************/
//  Description:  clear all cmd buffer register
//  Author: Fei.zhang
//  Param:
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_CMDBufClr (int cs);

/*****************************************************************************/
//  Description:  clear all type buffer register
//  Author: Fei.zhang
//  Param:
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_TypeBufClr (int cs);

/*****************************************************************************/
//  Description:  clear sfc interrupt request signal
//  Author: Fei.zhang
//  Param:
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_IntClr (int cs);

/*****************************************************************************/
//  Description:  set cs_timing_cfg register
//  Author: Fei.zhang
//  Param:
//      value: cs timing configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_CSTimingCfg (int cs, uint32 value);

/*****************************************************************************/
//  Description:  set rd_timing_cfg register
//  Author: Fei.zhang
//  Param:
//      value: rd timing configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_RDTimingCfg (int cs, uint32 value);

/*****************************************************************************/
//  Description:  set clock config register
//  Author: Fei.zhang
//  Param:
//      value: clock configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_ClkCfg (int cs, uint32 value);

/*****************************************************************************/
//  Description:  set CS config register
//  Author: Fei.zhang
//  Param:
//      value: CS configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_CSCfg (uint32 value);

/*****************************************************************************/
//  Description:  set endian config register
//  Author: Fei.zhang
//  Param:
//      value: endian configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_EndianCfg (int cs, uint32 value);

/*****************************************************************************/
//  Description:  set cmd buffer register
//  Author: Fei.zhang
//  Param:
//      index: cmd buffer index
//      value: cmd buffer configure value
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_SetCMDBuf (int cs, CMD_BUF_INDEX_E index, uint32 value);

/*****************************************************************************/
//  Description:  set cmd buffer register
//  Author: Fei.zhang
//  Param:
//      index: cmd buffer index
//      buf: src data
//      count: bytes count
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_SetCMDBufEx(int cs, CMD_BUF_INDEX_E index, const uint8 *buf,  uint32 count);

/*****************************************************************************/
//  Description:  get cmd buffer register
//  Author: Fei.zhang
//  Param:
//      index: cmd buffer index
//  Return:
//      cmd buffer configure value
//  Note:
/*****************************************************************************/
PUBLIC uint32 SFCDRV_GetCMDBuf (int cs, CMD_BUF_INDEX_E index);

/*****************************************************************************/
//  Description:  set cmd buffer register
//  Author: Fei.zhang
//  Param:
//      index: cmd buffer index
//      bitmode: bit mode
//      bytenum: byte number
//      cmdmode: read or write
//      sendmode: send mode
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SFCDRV_SetTypeInfBuf (int cs, CMD_BUF_INDEX_E index, BIT_MODE_E bitmode, BYTE_NUM_E bytenum, CMD_MODE_E cmdmode, SEND_MODE_E sendmode);

/*****************************************************************************/
//  Description:  set cs1 start address register
//  Author: Yong.Li
//  Param:
//      start_addr: flash cs1 start address
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC uint32 SFCDRV_SetCS1StartAddr(uint32 start_addr);

/*
 * Send a multi-byte command to the device and read the response.
 * Example:
 */
PUBLIC void spiflash_read_write(int cs, SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_len, uint32* din);
/*
 * Xip support, set the read cmd for this feature.
 * Example:
 * Note: You must set address send_mode as SEND_MODE_1, byte3-2-1-0 as chip specified.
 *       ------------Attention! ----------Attention!!------------------------------//
 */
PUBLIC void spiflash_set_xip(int cs, SFC_CMD_DES_T* cmd_des_ptr, uint32 cmd_len, BIT_MODE_E bit_mode);
#endif

#endif    //_SPI_DRVAPI_H_
