/*****************************************************************************
** File Name:      mmifmm_autotest.c                                         *
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

#define _MMIFMM_AUTOTEST_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_autotest.h"
#include "sfs.h"
#include "mmifmm_mainwin.h"
#include "mmi_autotest.h"
#include "mmifmm_comfunc.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_SYNCTOOL_SUPPORT
/*****************************************************************************/
//  Description : create folders in same path
//  Global resource dependence : s_fmm_current_path_ptr
//  Parameter: folders number
//  Author: shuyan.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMCreateFolderHori (uint16 hori_num)
{
    MMI_RESULT_E               result = MMI_RESULT_TRUE;
    uint16                          full_path_len = 0;
    uint16                         string_len = 0;
    uint16                          path_len = 0;
    SFS_ERROR_E              file_new = SFS_NO_ERROR;
    uint16                           inum;
    wchar full_len[MMIFMM_FULL_FILENAME_LEN+1]	= {0};
    MMIFMM_PATH_INFO_T * path = PNULL;
    wchar wch_name[10] = {0};
    wch_name[0] = '\\';
    wch_name[1] = 'h';

    //SCI_TRACE_LOW:"[ATEST][Atest_FMMCreateFolderHori:]%d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_12960_112_2_18_2_21_2_551, (uint8*) "d", hori_num);

    //check input folder number
    if (hori_num > MMIFMM_FILE_NUM)
    {
        //SCI_TRACE_LOW:"[atest]Atest_FMMCreateFolderHori: entry number error! "
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_12964_112_2_18_2_21_2_552, (uint8*) "");
        result = MMI_RESULT_FALSE;
        return (result);
    }

    SCI_MEMSET (full_len, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
    //get folder path
    path = MMIFMM_GetCurrentPath();
    path_len = MMIAPICOM_Wstrlen (path->pathname);
    MMIAPICOM_Wstrncpy (full_len, path->pathname, path_len);

    for (inum = 0; inum < hori_num; inum++)
    {
        if (hori_num < 10)
        {
            wch_name[2] = 0x0030 + inum % 10;
        }
        else if (hori_num < 100 && hori_num > 10)
        {
            wch_name[2] = 0x0030 + inum / 10;
            wch_name[3] = 0x0030 + inum % 10;
        }
        else
        {
            wch_name[2] = 0x0030 + inum / 100;
            wch_name[3] = 0x0030 + (inum % 100) / 10;
            wch_name[3] = 0x0030 + inum % 10;
        }

        string_len = MMIAPICOM_Wstrlen (wch_name);
        MMIAPICOM_Wstrncpy (&full_len[path_len], wch_name, string_len);
        full_path_len = MMIAPICOM_Wstrlen (full_len);

        //check the folder is exist
        if (!MMIAPIFMM_IsFolderExist (full_len, full_path_len))
        {
            //create folder
            file_new = SFS_CreateDirectory (full_len);
            //SCI_TRACE_LOW:"[Atest]Atest_FMMCreateFolderHori: new:%d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13000_112_2_18_2_21_2_553, (uint8*) "d", file_new);
            result = MMI_RESULT_TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[atest]Atest_FMMCreateFolderHori:  new:%d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13005_112_2_18_2_21_2_554, (uint8*) "d", file_new);
            result = MMI_RESULT_FALSE;
        }

    }

    return (result);
}


/*****************************************************************************/
//  Description : create folders across path
//  Global resource dependence : s_fmm_current_path_ptr
//  Parameter: folders number
//  Author: shuyan.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMCreateFolderVerti (uint16 Veri_num)
{
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    uint16                          string_len = 0;
    uint16                          depth_len = 0;
    uint16                          path_len = 0;
    uint16                          full_path_len = 0;
    SFS_ERROR_E               file_new = SFS_NO_ERROR;
    uint16                           inum;
    wchar full_len[MMIFMM_FULL_FILENAME_LEN+1]	= {0};
    MMIFMM_PATH_INFO_T* path = PNULL;

    wchar wch_name[10] = {0};
    wch_name[0] = '\\';
    wch_name[1] = 'V';

    //check input number
    if (Veri_num > MMIFMM_PATH_DEPTH)
    {
        //SCI_TRACE_LOW:"[Atest]Atest_FMMCreateFolderVerti: entry error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13039_112_2_18_2_21_2_555, (uint8*) "");
        result = MMI_RESULT_FALSE;
        return (result);

    }

    SCI_MEMSET (full_len, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
    //get folder path and depth
    path = MMIFMM_GetCurrentPath();
    depth_len = path->path_depth;
    path_len = MMIAPICOM_Wstrlen (path->pathname);
    MMIAPICOM_Wstrncpy (full_len, path->pathname, path_len);
    full_path_len = path_len;

    for (inum = 0; inum < Veri_num && depth_len <= MMIFMM_PATH_DEPTH; inum++)
    {
        wch_name[2] = 0x0030 + inum / 10;
        wch_name[3] = 0x0030 + inum % 10;
        string_len = MMIAPICOM_Wstrlen (wch_name);
        MMIAPICOM_Wstrncpy (&full_len[full_path_len], wch_name, string_len);

        //check the folder is exist
        if (!MMIAPIFMM_IsFolderExist (full_len, full_path_len))
        {
            //create the folder
            file_new = SFS_CreateDirectory (full_len);
            //SCI_TRACE_LOW:"[atest]Atest_FMMCreateFolderVerti:new:%d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13062_112_2_18_2_21_2_556, (uint8*) "d", file_new);
            result = MMI_RESULT_TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[atest]Atest_FMMCreateFolderVerti:new:%d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_13067_112_2_18_2_21_2_557, (uint8*) "d", file_new);
            result = MMI_RESULT_FALSE;
        }

        //path length and path depth add
        full_path_len = full_path_len + string_len;
        depth_len = depth_len + 1;

    }

    return (result);
}

#endif //#ifdef MMI_SYNCTOOL_SUPPORT

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Parameter:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMTrace (
    const MMIFMM_DATA_INFO_T *list_data_ptr,
    const MMIFMM_PATH_INFO_T *current_path_ptr,
    MMI_CTRL_ID_T ctrl_id
)
{
    uint16      index = 0;
    wchar       full_file_name[MMIFMM_PATHNAME_LEN +1];
    index = GUILIST_GetCurItemIndex (ctrl_id);
    MMIFMM_CombineFullFileName (list_data_ptr,
                                current_path_ptr,
                                index,
                                full_file_name,
                                MMIFMM_FULL_FILENAME_LEN);
    MMI_Wchar_AutoTest_Trace (MMI_FMM_TEST_WCHAR,
                              MMIAPICOM_Wstrlen (MMI_FMM_TEST_WCHAR),
                              full_file_name,
                              MMIAPICOM_Wstrlen (full_file_name)
                             );
    return TRUE;
}
