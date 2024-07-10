/******************************************************************************
 ** File Name:      adi_drv.h                                                *
 ** Author:         lichd                                                   *
 ** DATE:           06/01/2011                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    define trace interface just for testing usage             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 15/08/2011     lichd            Create                                    *
 ******************************************************************************/

#ifndef _ADI_DRV_H_
#define _ADI_DRV_H_

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function  Declare                                 *
 **---------------------------------------------------------------------------*/
/***************************************************************************** 
 *  Description:    this function is used to init analog to digital module.   * 
 *                      it will enable adi_acc and soft reset adi_module,    *
 *				and then config the priority of each channel.             * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                        * 
 *  Note:                                                                   * 
******************************************************************************/
PUBLIC void ADI_Init(void);

/***************************************************************************** 
 *  Description:    this function performs read operation to the analog die reg . * 
 *                      it will disable all interrupt and polling the ready bit,  *
 *				and return a half-word value after read complete.             * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                        * 
 *  Note:   return register value                                               * 
******************************************************************************/
PUBLIC uint16 ADI_REG_READ(uint32 addr);

/***************************************************************************** 
 *  Description:    this function performs write operation to the analog die reg .   * 
 *                      it will write the analog die register if the fifo is not full *
 *				It will polling the fifo full status til it is not full               * 
 *  Global Resource Dependence:                                              * 
 *  Author: Tim Luo                                                        * 
 *  Note:                                                                      * 
******************************************************************************/
PUBLIC void ADI_REG_WRITE(uint32 addr, uint32 data);

/*
 NOTE: You should capsule your own xx_REG_RD/WR() when it was used in a new driver.
       It was not sugguested using these interface directly.
*/
#endif
