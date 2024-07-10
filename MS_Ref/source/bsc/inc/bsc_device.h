/******************************************************************************
 ** File Name:      bsc_device.h                                                    *
 ** Author:                                                     *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE          	NAME             	DESCRIPTION                               *
 **                                   *                                *
 ******************************************************************************/
#ifndef BSC_DEVICE_H
#define BSC_DEVICE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "bsd.h"
#include "bsc.h"
#include "tb_flash.h" //added to use nor flash apis


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define CUSTOM_DATA_ADDR	(0x0 + 88)	
#define CUSTOM_DATA_SIZE	644

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC BSC_ERROR_E BSCD_Open(void);
PUBLIC void BSCD_Close(void);
PUBLIC BSC_ERROR_E BSCD_Read(uint16 ItemID, uint32 Size, const void *pDat);
PUBLIC BSC_ERROR_E BSCD_Write(uint16 ItemID, uint32 Size, const void *pDat);
PUBLIC BOOLEAN BSCD_Sector_Is_Read();
PUBLIC uint32 BSCD_GetHashLength(void);	//Added this api to get Hash Length

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */ 

#endif /* BSC_DEVICE_H */
