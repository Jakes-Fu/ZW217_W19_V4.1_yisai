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
 **                         nv status enum								                               *
 **---------------------------------------------------------------------------*/
typedef enum NV_STATUS_TYPE_tag
{
#ifdef FLASH_TYPE_SPI
    NV_BOTH_INVALID = 0x1002,   //both master sector and slave sector include nothing "BI", only mark slave
    NV_MOK_SINVALID = 0x1812,   //master is valide , slave are invalid "MO", mark master and slave
    NV_MOK_SOK       = 0x7E7F,   //master and slave are valid "BO", only mark master
    NV_MINVALID_SOK = 0x3A3E,   //master is invalid, slave is valid "SO" , only mark slave   
    NV_SOK_PRD_INFO = 0x5C56,   //slave sector contain product info, only mark slave
#else
    NV_BOTH_INVALID = 0x4249,    //both master sector and slave sector include nothing "BI"
    NV_MINVALID_SOK = 0x534f,    //master is invalid, slave is valid "SO"
    NV_MOK_SINVALID = 0x4d4f,    //master is valide , slave are invalid "MO"
    NV_MOK_SOK          = 0x424F,    //master and slave are valid "BO"
    NV_SOK_PRD_INFO   = 0x7370,    //slave sector contain product info
    /*以下是为了兼容老版本*/
    NV_BOTH_INVALID_OLD = 0xFFFF,    //both master sector and slave sector include nothing
    NV_MINVALID_SOK_OLD = 0xFFFE,    //master is invalid, slave is valid
    NV_MOK_SINVALID_OLD = 0xFFFC,    //master is valide , slave are invalid
    NV_MOK_SOK_OLD     = 0xFFF0    //master and slave are valid
#endif
}NV_STATUS_TYPE_E;


/**---------------------------------------------------------------------------*
 **                         MACRO Defination								                               *
 **---------------------------------------------------------------------------*/

#define WRITE_MASTER_NV_STATUS(status) HAL_WriteWord(master_address+sector_size-2,status)
#define WRITE_SLAVE_NV_STATUS1(status) HAL_WriteWord(slave_address+sector_size-2,status)
#define WRITE_SLAVE_NV_STATUS2(status) HAL_WriteWord(slave_address+sector_size-4,status)

/**---------------------------------------------------------------------------*
 **                         Function Defination								                               *
 **---------------------------------------------------------------------------*/
extern void update_master_fixnv_by_slave(uint32 sector_size);
extern uint32 FLASH_GetProductInfoAddr(void);
extern void* NVITEM_MallocCaliBuf(void);
/**
  * update_fixnv - update the fixnv by new fixnv buffer
  * @fix_nv_address:        specify the new fix nv buffer
  * @fix_nv_size:     fix nv buffer length
  *
  * Returns value:
  *		SCI_TRUE: update successfully.
  *		SCI_FALSE: fail to update.	
  */
 uint32	compute_valid_data_length(uint8 *buffer,uint32 length);
 
 void verify_fixNV(void);
 BOOLEAN 	update_fixnv(char * fix_nv_address,uint32 fix_nv_area_size);
 uint32 calculate_fixNV_length( uint32  start_addr, uint32 end_addr);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //_UPDATE_FIXNV__H_
