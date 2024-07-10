/******************************************************************************
 ** File Name:      DigitalCamera_win32.c                                           *
 ** Author:        	figo.feng                                             	  *
 ** Date:           12/30/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    implementation of digital camera operation interface      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/30/2004     Great.Tian     	Create.                                  *
                
******************************************************************************/

#define _FFS_WIN32_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#if 0
#include "mmifmm_export.h"
//#include	"ffs.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


 /**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

FFS_ERROR_E FFS_Delete(
    FILE_DEV_E_T file_dev_type,
    const wchar *name)
{
    return (FFS_ERROR_NONE);
}

FFS_ERROR_E FFS_Rename(
    FILE_DEV_E_T file_dev_type,
    const wchar *sour,
    const wchar *dest)
{
    return (FFS_ERROR_NONE);
}

FFS_ERROR_E  FFS_CD(
    FILE_DEV_E_T file_dev_type,
    const wchar *path)
{
    return (FFS_ERROR_NONE);
}

FFS_ERROR_E FFS_CreateDir(
    FILE_DEV_E_T file_dev_type,
    const wchar *path)
{
    return (FFS_ERROR_NONE);
}

void FFS_Close(
    FILE_DEV_E_T file_dev_type,
    HFS  handle)
{
    return ;
}

FFS_ERROR_E FFS_FindNext(
    FILE_DEV_E_T file_dev_type,
    HFS find_handle,      // the handle that return by FFS_FindFirst.
    const wchar *find,      // same as FFS_FindFirst.
    FFS_FIND_DATA_T * fd)
{
    return (FFS_ERROR_INVALID_PARAM); // 可能不应该返回这个值
}

HFS FFS_FindFirst(
    FILE_DEV_E_T file_dev_type,
    const wchar *find,
    FFS_FIND_DATA_T *fd)
{
    return (1); // 可能不应该返回这个值
}

uint32 FFS_GetLength(
    FILE_DEV_E_T file_dev_type,
    HFS handle)
{
    return (0x0FFF);
}

HFS FFS_Create(
    FILE_DEV_E_T file_dev_type,
    const wchar *name,
    uint16  mode)
{
    return (0x0FFF);
}

uint32 FFS_GetFreeSpace(
    FILE_DEV_E_T file_dev_type)
{
    return (1024 * 1024 * 16);
}

FFS_ERROR_E FFS_Read(
    FILE_DEV_E_T file_dev_type,
    HFS                handle, // the handle return by FFS_Create.
    uint8               *buf,
    uint32              len,
    uint32              *transmitted,
    FFS_OVERLAPPED_T    *overlapped_ptr)
{
    return (FFS_ERROR_NONE);
}

BOOLEAN FFS_CheckExist(
    FILE_DEV_E_T file_dev_type,
    const wchar *file)
{
    return (TRUE);
}

FFS_ERROR_E FFS_Write(
    FILE_DEV_E_T file_dev_type,
    HFS                handle,
    uint8               *buf,    // the handle return by FFS_Create.
    uint32              len,
    uint32              *transmitted,
    FFS_OVERLAPPED_T    *overlapped_ptr)
{
    return (FFS_ERROR_NONE);
}

SCI_FILE* FFS_fopen ( FILE_DEV_E_T file_dev_type, const wchar *filename, const char *mode )
{
    return 0;
}

SIZE_T FFS_fwrite ( FILE_DEV_E_T file_dev_type, const void *buffer, SIZE_T size, SIZE_T count, SCI_FILE * stream )
{
    return 0;
}

int FFS_fclose ( FILE_DEV_E_T file_dev_type, SCI_FILE *stream )
{
    return 0;
}

SIZE_T FFS_fread ( FILE_DEV_E_T file_dev_type, void *buffer, SIZE_T size, SIZE_T count, SCI_FILE * stream )
{
    return 0;
}

int FFS_fseek ( FILE_DEV_E_T file_dev_type, SCI_FILE *stream, int32 offset, int origin )
{
    return 0;
}

FFS_ERROR_E FFS_AYS_Delete(
    FILE_DEV_E_T file_dev_type,
    const wchar *name,
    FFS_OVERLAPPED_T    *overlapped_ptr)
{
    return FFS_ERROR_NONE;
}

#endif