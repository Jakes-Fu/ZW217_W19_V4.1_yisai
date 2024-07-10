/******************************************************************************
 ** File Name:       rotation_drv.h                                               *
 ** Author:                                                                   *
 ** DATE:            03/15/2006                                               *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE              NAME             DESCRIPTION                            *
 ******************************************************************************/


#ifndef _ROTATION_DRV_H_
#define _ROTATION_DRV_H_

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


/*
 * Data Type struct
 */
typedef enum
{
    ERR_ROTATION_NONE = SCI_SUCCESS,        // Success,no error
    ERR_ROTATION_PARAMETERS_ERROR,          // parameter error
    ERR_ROTATION_PARAMETERS_TIMEOUT     // time out error
}
ERR_ROTATION_E;

typedef enum
{
    ROTATION_PIXFORMAT_1BYTE,
    ROTATION_PIXFORMAT_2BYTE,
    ROTATION_PIXFORMAT_4BYTE,
    ROTATION_PIXFORMAT_MAX = ROTATION_PIXFORMAT_4BYTE
} ROTATION_PIXFORMAT_E;

typedef enum
{
    ROTATION_ANGEL_90,
    ROTATION_ANGEL_270,
#if !defined(CHIP_VER_8800H5) && !defined(PLATFORM_SC8800G)
    ROTATION_ANGEL_MAX = ROTATION_ANGEL_270
#else
    ROTATION_ANGEL_180,
    ROTATION_ANGEL_MIRROR,
    ROTATION_ANGEL_MAX = ROTATION_ANGEL_MIRROR
#endif
} ROTATION_ANGEL_E;

#if !defined(CHIP_VER_8800H5) && !defined(PLATFORM_SC8800G)
typedef enum
{
    ROTATION_UNIT_8X8,
    ROTATION_UNIT_16X16,
    ROTATION_UNIT_MAX = ROTATION_UNIT_16X16
} ROTATION_UNIT_E;
#else
typedef enum
{
    ROTATION_MOD_NORMAL,
    ROTATION_MOD_UV422,
    ROTATION_UNIT_MAX = ROTATION_MOD_UV422
} ROTATION_ROT_MOD_E;
#endif
typedef struct rotation_parameters_tag
{
    uint32 srcAddr;                          //address of source
    uint32 desAddr;                          //address of dest
    uint32 width;                            //image width before rotation
    uint32 height;                           //image height before rotation
    ROTATION_PIXFORMAT_E pixFormat;          //RGB565:2BYTES/pixel,RGB888:2BYTES/pixel
    ROTATION_ANGEL_E angel;                  //rotation angel,clockwise in direction
#if !defined(CHIP_VER_8800H5) && !defined(PLATFORM_SC8800G)
    ROTATION_UNIT_E unit;                    //rotation unit,
    /*NOTE:only pixformat is 1BYTE/pixel,
        the rotation unit can be 16X16*/
#else
    ROTATION_ROT_MOD_E mode;             //rot mode,0 normal, 1 UV422
#endif
} ROTATION_PARAMETERS_T;


/**---------------------------------------------------------------------------*
 **                         Constant Variables
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Prototypes
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:  do rotation.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  ERR_ROTATION_E  ROTATION_Open (void);

PUBLIC  ERR_ROTATION_E  ROTATION_Close (void);

PUBLIC  ERR_ROTATION_E  ROTATION_Ratation (
    ROTATION_PARAMETERS_T *rotation_param,
    int (*func) (uint32 param)
);



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _ROTATION_H

