/******************************************************************************
 ** File Name:      fdl_main.h                                                *
 ** Author:         Leo.Feng                                                  *
 ** DATE:           30/11/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces of  the FIFO for serial  *
 **                 I/O operation. The FIFO is a clic queue.Writing operation *
 *                  can overlap the data has not be reading.                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 30/11/2001     Leo.Feng         Create.                                   *
 **                                                                           *
 ******************************************************************************/


#ifndef _UPDATE_FIXNV__H_
#define _UPDATE_FIXNV__H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         nv status enum                                                             *
 **---------------------------------------------------------------------------*/
typedef enum NV_STATUS_TYPE_tag
{
#ifdef FLASH_TYPE_SPI
    NV_BOTH_INVALID = 0x1002,   //both master sector and slave sector include nothing "BI", only mark slave
    NV_MOK_SINVALID = 0x1812,   //master is valide , slave are invalid "MO", mark master and slave
    NV_MOK_SOK       = 0x7E7F,   //master and slave are valid "BO", only mark master
    NV_MINVALID_SOK = 0x3A3E,   //master is invalid, slave is valid "SO" , only mark slave   
    NV_SOK_PRD_INFO = 0x5C56,   //slave sector contain product info, only mark slave

    NV_BOTH_INVALID_OLD = 0x4249,    //both master sector and slave sector include nothing "BI"
    NV_MINVALID_SOK_OLD = 0x534f,    //master is invalid, slave is valid "SO"
    NV_MOK_SINVALID_OLD = 0x4d4f,    //master is valide , slave are invalid "MO"
    NV_MOK_SOK_OLD         = 0x424F,    //master and slave are valid "BO"
    NV_SOK_PRD_INFO_OLD   = 0x7370,    //slave sector contain product info

#else
    NV_BOTH_INVALID = 0x4249,    //both master sector and slave sector include nothing "BI"
    NV_MINVALID_SOK = 0x534f,    //master is invalid, slave is valid "SO"
    NV_MOK_SINVALID = 0x4d4f,    //master is valide , slave are invalid "MO"
    NV_MOK_SOK          = 0x424F,    //master and slave are valid "BO"
    NV_SOK_PRD_INFO   = 0x7370,    //slave sector contain product info
    NV_BOTH_INVALID_OLD = 0xFFFF,    //both master sector and slave sector include nothing
    NV_MINVALID_SOK_OLD = 0xFFFE,    //master is invalid, slave is valid
    NV_MOK_SINVALID_OLD = 0xFFFC,    //master is valide , slave are invalid
    NV_MOK_SOK_OLD     = 0xFFF0    //master and slave are valid
#endif
}NV_STATUS_TYPE_E;


/**---------------------------------------------------------------------------*
 **                         MACRO Defination                                                               *
 **---------------------------------------------------------------------------*/

#define DL_SLAVE_SECTOR_ID  (0xf7bde36c)


#define READ_WORD16(address) (*((uint16*)(address)))
#define READ_WORD32(address) (*((uint32*)(address)))

#define FLASH_WRITE_WORD16(address,status)   {FDL_FlashWriteArea(address, (char *)&status, 2);}
#define FLASH_WRITE_WORD32(address,status)   {FDL_FlashWriteArea(address, (char *)&status, 4);}

#define WRITE_MASTER_NV_STATUS(status) FLASH_WRITE_WORD16(master_address+sector_size-2,status)
#define WRITE_MASTER_NV_LENGTH(length) FLASH_WRITE_WORD16(master_address+sector_size-8,length)

#define WRITE_SLAVE_NV_STATUS1(status) FLASH_WRITE_WORD16(slave_address+sector_size-2,status)
#define WRITE_SLAVE_NV_STATUS2(status) FLASH_WRITE_WORD16(slave_address+sector_size-4,status)
#define WRITE_SLAVE_NV_LENGTH(length) FLASH_WRITE_WORD16(slave_address+sector_size-8,length)

#define WRITE_SPARE_STATUS(status) FLASH_WRITE_WORD16(slave_address+sector_size-6,status)

#define READ_NV_STATUS(status,start_address) {status = READ_WORD16(start_address + sector_size-2);}

#define READ_SLAVE_NV_STATUS1(status) {status = READ_WORD16(slave_address + sector_size-2);}
#define READ_SLAVE_NV_STATUS2(status) {status = READ_WORD16(slave_address + sector_size-4);}

#define READ_MASTER_NV_STATUS(status) {status = READ_WORD16(master_address + sector_size-2);}
#define READ_SPARE_STATUS(status,address) {status = READ_WORD16(address + sector_size-6);}


void update_sector_by_slave(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_UPDATE_FIXNV__H_
