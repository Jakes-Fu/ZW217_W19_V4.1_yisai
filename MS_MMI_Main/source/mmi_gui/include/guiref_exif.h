/*****************************************************************************
** File Name:      guiref_exif.h                                             *
** Author:                                                                   *
** Date:           02/2007                                                   *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2007       Jassmine           Create
******************************************************************************/

#ifndef _GUIREF_EXIF_H_    
#define _GUIREF_EXIF_H_   

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIREF_EXIF_REVERSED_BUF_SIZE   (8*1024)    //8k

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : split jpg exif file
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC uint8 GUIREF_SplitJpgExif(
                                 uint8      *data_buf_ptr,
                                 uint32     data_size,
                                 uint32     *thumbnail_offset_ptr,
                                 uint32     *thumbnail_size_ptr,
                                 uint32     *main_offset_ptr,
                                 uint32     *main_size_ptr
                                 );

/*****************************************************************************/
// 	Description : split jpg exif file
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUIREF_MergeJpgExif(
                                  uint8      *data_buf_ptr,     //Buffer save the exif,前面必须留8K的空间
                                  uint32     thumbnail_offset,  //size of Thumbnail image
                                  uint32     thumbnail_size,    //main offset from the start of pBuf
                                  uint32     main_offset,       //size of Main image
                                  uint32     main_size,         //size of Main image
                                  uint8      merge_type         //0: Spreadtrum image; 1: External image
                                  );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif