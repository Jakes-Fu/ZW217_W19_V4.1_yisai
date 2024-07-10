/******************************************************************************
 ** File Name:      ffs_error.h                                              *
 ** Author:                                                          *
 ** DATE:           12/16/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 **                                                                           *
 ** Description:    This file include error code define used by FFS.          *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/16/2004             Create.                                   *
 ******************************************************************************/

#ifndef FFS_ERROR_H
#define FFS_ERROR_H
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif


typedef enum FFS_ERROR_E_TAG
{

    FFS_NO_ERROR                   = 0,
    FFS_ERROR_NONE                 = 0,

    FFS_ERROR_DEVICE,

    FFS_ERROR_SYSTEM, 
    FFS_ERROR_INVALID_PARAM,
    FFS_ERROR_NO_ENOUGH_RESOURCE, 

    FFS_ERROR_IO_PENDING,   // IO operation is pending.
    FFS_ERROR_ACCESS,       // access denied

    FFS_ERROR_NOT_EXIST,    // file not exist.
    FFS_ERROR_HAS_EXIST,    // file has exist
    
/**------------------------------------------------------------------*
 **     device-lay error code                                        *
 **------------------------------------------------------------------*/
    FFS_ERROR_HARDWARE_FAILED,
    FFS_ERROR_WRITE_CONFLICT,

/**------------------------------------------------------------------*
 **     item-lay error code                                          *
 **------------------------------------------------------------------*/
    FFS_ERROR_NO_SPACE


/**------------------------------------------------------------------*
 **     IO-Task lay error code                                       *
 **------------------------------------------------------------------*/

/**------------------------------------------------------------------*
 **     interface-lay error code                                     *
 **------------------------------------------------------------------*/


}FFS_ERROR_E;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif   /* FFS_ERROR_H   */

/* End of file  */

