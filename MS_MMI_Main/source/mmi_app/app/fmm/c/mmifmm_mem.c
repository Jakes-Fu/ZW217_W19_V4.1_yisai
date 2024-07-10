/*****************************************************************************
** File Name:      mmifmm_mem.c                                              *
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

#define _MMIFMM_MEM_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_mem.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMI_FMM_MAX_ALLOC_NUM  100
#define MMI_FMM_MAX_BUFFER_NUM  (MMIFMM_FILE_NUM/MMI_FMM_MAX_ALLOC_NUM)

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//for file buffer malloc and free pointer
LOCAL void  * s_pointer_buffer[MMI_FMM_MAX_BUFFER_NUM] = {PNULL};
LOCAL BOOLEAN s_buffer[MMI_FMM_MAX_ALLOC_NUM] = {FALSE};
LOCAL int32 s_buffer_index = 0;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : malloc file info buffer
//  Global resource dependence :
//  Author: jian ma
//  Note:
/*****************************************************************************/
PUBLIC void * MMIFMM_GetFileInfoBuffer (void)
{
    int32   i = 0;
    int32   j = s_buffer_index;

    while (1) /*lint !e716*/
    {
        if (PNULL == s_pointer_buffer[j])
        {
            s_pointer_buffer[j] = (MMIFMM_FILE_INFO_T *) SCI_ALLOCA (sizeof (MMIFMM_FILE_INFO_T) * MMI_FMM_MAX_ALLOC_NUM);

            if (PNULL == s_pointer_buffer[j])
            {
                SCI_TRACE_LOW ("FMM: can't malloc memory !");

                if (s_buffer_index > 0)
                {
                    s_buffer_index--;
                }

                return PNULL;
            }

            SCI_MEMSET (s_pointer_buffer[j], 0, (sizeof (MMIFMM_FILE_INFO_T) *MMI_FMM_MAX_ALLOC_NUM));
        }

        for (i = 0; i < MMI_FMM_MAX_ALLOC_NUM; i++)
        {
            if (!s_buffer[i])
            {
                s_buffer[i] = TRUE;
                break;
            }
        }

        if (i == MMI_FMM_MAX_ALLOC_NUM)
        {
            j++;

            if (j < MMI_FMM_MAX_BUFFER_NUM)
            {
                SCI_MEMSET (s_buffer, 0x00, (MMI_FMM_MAX_ALLOC_NUM*sizeof (BOOLEAN)));
                s_buffer_index = j;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    if (j < MMI_FMM_MAX_BUFFER_NUM)
    {
        return ( (MMIFMM_FILE_INFO_T *) s_pointer_buffer[j] + i);
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
//  Description : free file info buffer
//  Global resource dependence :
//  Author: jian ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FreeFileInfoBuffer (BOOLEAN is_free_all)
{
    int32 i = 0;

    if (is_free_all)
    {
        for (i = 0 ; i < MMI_FMM_MAX_BUFFER_NUM; i++)
        {
            if (PNULL != s_pointer_buffer[i])
            {
                SCI_FREE (s_pointer_buffer[i]);
            }
        }
    }
    else
    {
        for (i = 0 ; i < MMI_FMM_MAX_BUFFER_NUM; i++)
        {
            if (PNULL != s_pointer_buffer[i])
            {
                SCI_MEMSET (s_pointer_buffer[i], 0x00, (sizeof (MMIFMM_FILE_INFO_T) *MMI_FMM_MAX_ALLOC_NUM));
            }
        }
    }

    s_buffer_index = 0;
    SCI_MEMSET (s_buffer, 0x00, (MMI_FMM_MAX_ALLOC_NUM*sizeof (BOOLEAN)));

}
/*****************************************************************************/
//  Description : free file info
//  Global resource dependence :
//  Author: jian ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FreeFileInfo (MMIFMM_DATA_INFO_T *list_data_ptr, BOOLEAN is_free_all)
{
    //modified by liyan.zhu for c++ test
    //uint16 i = 0;
    if (PNULL != list_data_ptr)
    {
        uint16 i = 0;

        for (i = 0 ; i < list_data_ptr->file_num + list_data_ptr->folder_num; i++)
        {
            if (PNULL != list_data_ptr->data[i])
            {
                if (PNULL != list_data_ptr->data[i]->filename_ptr)
                {
                    SCI_FREE (list_data_ptr->data[i]->filename_ptr);
                }
            }
        }
    }

    MMIFMM_FreeFileInfoBuffer (is_free_all);
}