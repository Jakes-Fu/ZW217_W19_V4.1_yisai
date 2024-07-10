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
//  Description : Ԥ���򣬽��ļ�����ǰ���ļ��ź�
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_FilePreSort (MMIFMM_DATA_INFO_T* data_ptr);

/*****************************************************************************/
//  Description : �Ƚ�����
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileName (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
) ;

/*****************************************************************************/
//  Description : �Ƚ��ļ���С
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileSize (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
);

/*****************************************************************************/
//  Description : �Ƚ��ļ�ʱ��
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileTime (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
);

/*****************************************************************************/
//  Description : �Ƚ�����
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileType (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
);


#endif