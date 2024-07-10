/******************************************************************************
 ** File Name:      detect_errors.h                                           *
 ** Author:         jiayong.yang                                              *
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
 ** 10/11/2009     jiayong.yang         Create.                               *
 **                                                                           *
 ******************************************************************************/

#ifndef _DETECT_ERRORS_H_
#define _DETECT_ERRORS_H_

/* C header file */

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

void    record_samples (uint32 base_address,uint32 write_address,uint32 data_len,char *data);

uint8   check_complete_sector (uint32 base_address,uint32 write_address,uint32 sector_size);

uint8   check_one_patition (uint32 base_address,uint32 write_address);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //_DETECT_ERRORS_H_
