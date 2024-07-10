/*****************************************************************************
** File Name:      mmifmm_createwin.h                                        *
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

#ifndef _MMIFMM_CREATEWIN_H_
#define _MMIFMM_CREATEWIN_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct mmifmm_createfolder_win_param_tag
{
    wchar       *full_path_ptr;  //��ǰĿ¼ȫ·��
    uint16      new_folder_max_len; //�½�Ŀ¼������ֳ���,ucs2 len
    wchar       *new_folder_ptr;
    uint16      *new_folder_len_ptr;
    uint32      win_id;  //֪ͨ����
    uint32      msg_id;  //֪ͨ��Ϣ
} MMIFMM_CREATEFOLDER_WIN_PARAM_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : create folder win 
// Global resource dependence : 
// Author: jian.ma
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_CreateFolderWin(MMIFMM_CREATEFOLDER_WIN_PARAM_T * folder_win_param);



#endif