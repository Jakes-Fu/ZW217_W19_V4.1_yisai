/******************************************************************************
 ** File Name:      char_dev.h                                                *
 ** Author:                                                                   *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This driver is for development board, Integrator/AP.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ******************************************************************************/

#ifndef _CHAR_DEV_H_
#define _CHAR_DEV_H_

#include "sci_types.h"
#include "sio.h"

#define CHAR_DEV_UART       0
#define CHAR_DEV_UCOM       1
#define CHAR_DEV_BT         2
#define CHAR_DEV_CARD       3
#define CHAR_DEV_CDC        4
#define CHAR_DEV_SPI        5



typedef  sio_operation_st    CHAR_OPER_T;

/*
*/
PUBLIC BOOLEAN CharDev_Install_Operation(CHAR_OPER_T *dest_op,uint32 phy_port);

/*
*/
PUBLIC void	   CharDev_Register_Operation(uint8 dev_id,const CHAR_OPER_T* char_dev_op_ptr);

/*
*/
PUBLIC void	CharDev_Uninitialize(void);

/*
*/
PUBLIC void	CharDev_Initialize(void);

#endif /*End char_dev.h*/


