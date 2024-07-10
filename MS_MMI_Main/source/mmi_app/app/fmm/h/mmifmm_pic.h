/*****************************************************************************
** File Name:      mmifmm_pic.h                                              *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/

#ifndef _MMIFMM_PIC_H_
#define _MMIFMM_PIC_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmifmm_export.h"

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


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct
{
    uint16      total_num;                    //总数
    uint16      current_pos;                  //当前位置
    uint16      list_index[MMIFMM_FILE_NUM];  //当前图像数据的索引
} MMIFMM_PICTURE_INDEX_DATA_T;

typedef struct
{
    MMI_CTRL_ID_T               list_ctrl_id;
    MMIFMM_PICTURE_INDEX_DATA_T *pic_data_ptr;
} MMIFMM_PIC_ADD_DATA_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : preview image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_PicPreview(
                              MMI_CTRL_ID_T                 list_ctrl_id,
                              MMIFMM_PICTURE_INDEX_DATA_T   *pic_data_ptr
                              );

/*****************************************************************************/
//  Description : get file name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIFMM_GetPicName(
                                uint16  item_index,     //in:
                                uint16  *name_len_ptr   //in/out:
                                );

/*****************************************************************************/
//  Description : return true or false to sign whther the pic is supported with Pic index data
//  Global resource dependence : 
//  Author: Liyan.zhu
//  Note:CR NEWMS00210875,CR NEWMS00217600
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsPicSupportFromPath(wchar   *full_path_name,uint16 len);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
