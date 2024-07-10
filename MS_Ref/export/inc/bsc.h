/******************************************************************************
 ** File Name:      bsc.h                                                    *
 ** Author:                                                     *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE          	NAME             	DESCRIPTION                               *
 ** 								                                  *                                *
 ******************************************************************************/
#ifndef BSC_H
#define BSC_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

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
#define INPUT
#define OUTPUT
#define INOUT

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum _BSC_ERROR {
    BSCERR_NONE  = 0,   			/* Success */    
    BSCERR_SYSTEM,      			/* System error, e.g. hardware failure */
    BSCERR_INVALID_PARAM,
    BSCERR_NO_ENOUGH_RESOURCE,
    BSCERR_NOT_EXIST,
    BSCERR_ACCESS_DENY,
    BSCERR_INCOMPATIBLE,
    BSCERR_NOT_OPENED,
    BSCERR_NOT_PROTECTED
}BSC_ERROR_E;

typedef enum _BSC_ID_E
{
	BSC_MIN_ID = 0,
	BSC_BOOT_DATA,
	BSC_CUSTOM_DATA,
	BSC_MAX_ID
}BSC_ID_E;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC BSC_ERROR_E BSC_Init();
PUBLIC BSC_ERROR_E BSC_Read(INPUT uint16 ItemID, INPUT uint16 cchSize, 
	OUTPUT void *pBuf);
PUBLIC BSC_ERROR_E BSC_Write(INPUT uint16 ItemID, INPUT uint16 cchSize,  
	INPUT const void *pBuf);
PUBLIC uint32 BSC_GetHashLength(void);	//Added this api to get hAsh Length

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */ 

#endif /* BSC_H */
