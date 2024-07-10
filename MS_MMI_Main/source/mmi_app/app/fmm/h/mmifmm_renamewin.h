/*****************************************************************************
** File Name:      mmifmm_renamewin.h                                        *
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

#ifndef _MMIFMM_RENAMEWIN_H_
#define _MMIFMM_RENAMEWIN_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmifmm_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    wchar       *full_path_ptr; //ȫ·��,����֮ǰ�����֣�����֮���ٱ��浽���
    MMIFMM_FILE_TYPE_E     type;      //�ļ�����
    uint32      win_id;         //֪ͨ����
    uint32      msg_id;         //֪ͨ��Ϣ
    uint32      max_file_lan;   //����Ŀ¼����󳤶�
} MMIFMM_RENAME_WIN_PARAM_T;

typedef struct
{
    wchar                       file_name_ptr[MMIFILE_FULL_PATH_MAX_LEN+1];
    uint32                      file_name_len;
} MMIFMM_RENAME_RETURN_T;
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : enter rename win
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterReNameWin (
    MMIFMM_RENAME_WIN_PARAM_T*  rename_param// ����
);

#endif