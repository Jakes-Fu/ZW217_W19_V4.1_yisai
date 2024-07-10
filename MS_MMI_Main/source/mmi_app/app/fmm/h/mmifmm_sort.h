/*****************************************************************************
** File Name:      mmifmm_sort.h                                             *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#ifndef _MMIFMM_SORT_H_
#define _MMIFMM_SORT_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 预排序，将文件夹排前，文件排后
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_FilePreSort (MMIFMM_DATA_INFO_T* data_ptr);

/*****************************************************************************/
//  Description : 比较名称
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileName (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
) ;

/*****************************************************************************/
//  Description : 比较文件大小
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileSize (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
);

/*****************************************************************************/
//  Description : 比较文件时间
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileTime (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
);

/*****************************************************************************/
//  Description : 比较类型
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileType (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
);


#endif