/*****************************************************************************
** File Name:               mmiap_musicarray.c                                *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#define _MMIAP_MUSICARRAY_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmivirtualarray.h"
#include "mmiap_musicarray.h"
#include "mmisrvfmm_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MUSICARRAY_SAVEFILE_IDENT        "MUSICARRAY SAVEFILE 01.00.00"  //�洢�ļ���ʽ�汾��
#define MUSICARRAY_SAVEFILE_IDENT_LEN    27
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create handle
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void * MMIAPLAYERARRAY_CreateArray(uint32 data_size,
        const char *file,
        uint32   line
                                         )
{
    void * music_array = NULL;

    music_array = MMIVIRTUALARRAY_Create(data_size,file,line);

    SCI_TRACE_LOW("MMIAPLAYERARRAY_CreateArray, music_array = %x",music_array);

    return music_array;
}

/*****************************************************************************/
//  Description : destory handle
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPLAYERARRAY_DestroyArray(void ** music_array)
{
    if(NULL == music_array)
    {
        return;
    }

    MMIVIRTUALARRAY_Destroy(music_array);
}

/*****************************************************************************/
//  Description : Get the total number of music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPLAYERARRAY_GetTotalNum(void * music_array)
{
    if(NULL == music_array)
    {
        return 0;
    }

    return MMIVIRTUALARRAY_GetArraySize(music_array);
}

/*****************************************************************************/
//  Description : Get music mark detail info.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Read(
    void * music_array,
    uint32 index,                                       //[IN]
    void *data_info_ptr      //[OUT]
)
{
    BOOLEAN     result = FALSE;

    if(NULL == music_array
            || NULL == data_info_ptr)
    {
        return FALSE;
    }

    result = MMIVIRTUALARRAY_Read(music_array, index, data_info_ptr);

    SCI_TRACE_LOW("MMIAPLAYERARRAY_Read result = %d",result);
    return result;
}


/*****************************************************************************/
//  Description : load all music mark brief info.
//  Global resource dependence : s_music_mark_info_ptr
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_LoadFromFile(void * music_array,
        uint16     *full_path_ptr,
        uint16     full_path_len)
{
    BOOLEAN         result = FALSE;
    MMIFILE_HANDLE      sfs_handle = SFS_NO_ERROR;
    uint32          read_size = 0;
    uint8           file_ident[MUSICARRAY_SAVEFILE_IDENT_LEN+2] = {0};
    SFS_ERROR_E     file_ret = SFS_ERROR_NONE;

    if(NULL == music_array
            || NULL == full_path_ptr
            || 0 == full_path_len)
    {
        return FALSE;
    }

    sfs_handle = MMIAPIFMM_CreateFile(full_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if(NULL != sfs_handle)
    {
        //���ļ���ʶ, ������filearray�����ļ��ı�ʶ, �ż���
        file_ret = MMIAPIFMM_ReadFile(sfs_handle, file_ident, MUSICARRAY_SAVEFILE_IDENT_LEN, &read_size, NULL);
        MMIAPIFMM_CloseFile(sfs_handle);
        if(SFS_ERROR_NONE == file_ret)
        {
            if(0 == memcmp(file_ident, MUSICARRAY_SAVEFILE_IDENT, MUSICARRAY_SAVEFILE_IDENT_LEN))
            {
                result = MMIVIRTUALARRAY_LoadArrayFromFile(music_array, full_path_ptr, full_path_len, MUSICARRAY_SAVEFILE_IDENT_LEN);
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Add a new music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Add(void * music_array,
                                   void * data_info_ptr)
{
    BOOLEAN     result = FALSE;

    if(NULL == music_array
            || NULL == data_info_ptr)
    {
        return FALSE;
    }

    result = MMIVIRTUALARRAY_Add(music_array, data_info_ptr);

    return result;
}

/*****************************************************************************/
//  Description : Delete music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Delete(void * music_array,
                                      uint16 data_index)
{
    BOOLEAN     result = FALSE;

    if(NULL == music_array)
    {
        return FALSE;
    }

    result = MMIVIRTUALARRAY_Delete(music_array, data_index);

    return result;
}

/*****************************************************************************/
//  Description : Update music mark info.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Modify(void * music_array,
                                      uint16  data_index,         //[IN]mark index
                                      void * data_info_ptr )
{
    BOOLEAN     result = FALSE;

    if(NULL == music_array
            || NULL == data_info_ptr)
    {
        return FALSE;
    }

    result = MMIVIRTUALARRAY_Modify(music_array, data_index, data_info_ptr);

    return result;
}

/*****************************************************************************/
//  Description : ��filearray���鱣�浽ָ���ļ��У��ػ���Ҳ���ᶪʧ
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIAPLAYERARRAY_SaveArrayToFile(
    void*  music_array,     //in, filearrayֵ
    uint32  data_size,
    uint16     *full_path_ptr,     //Ҫ���浽���ļ���, �ļ�һ��Ҫ����
    uint16     full_path_len
)
{
    MMIFILE_ERROR_E result = SFS_ERROR_NONE;
    MMIFILE_HANDLE  sfs_handle = SFS_NO_ERROR;
    uint32      file_array_size = 0;
    uint32      file_array_total_size = 0;
    uint32      write_size = 0;

    if(NULL == music_array
            || NULL == full_path_ptr
            || 0 == full_path_len)
    {
        return SFS_ERROR_INVALID_PARAM;
    }

    //�����ļ�
    sfs_handle = MMIAPIFMM_CreateFile(full_path_ptr, SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if(NULL != sfs_handle)
    {
        file_array_size = MMIAPIFILEARRAY_GetArraySize(music_array);
        file_array_total_size = file_array_size * data_size + MUSICARRAY_SAVEFILE_IDENT_LEN;

        if((file_array_total_size >> 10) >=  MMI_GetFreeSpace(MMIAPIFMM_GetDeviceTypeByPath(full_path_ptr, MMIFILE_DEVICE_LEN)))
        {
            MMIAPIFMM_CloseFile(sfs_handle);
            result = SFS_ERROR_NO_SPACE;   //changed by sandy for cr246623 from SFS_ERROR_NO_ENOUGH_RESOURCE to SFS_ERROR_NO_SPACE,Ӧ����ʾ�ռ䲻�㣻
        }
        else
        {
            //clean this file
            MMIAPIFMM_SetFileSize(sfs_handle, 0);

            //д�ļ���ʶ
            result = MMIAPIFMM_WriteFile(sfs_handle,
                                         MUSICARRAY_SAVEFILE_IDENT,
                                         MUSICARRAY_SAVEFILE_IDENT_LEN,
                                         &write_size, NULL);

            MMIAPIFMM_CloseFile(sfs_handle);
            if(SFS_ERROR_NONE == result)
            {
                result = MMIVIRTUALARRAY_SaveArrayToFileEx(music_array, full_path_ptr, full_path_len);
            }
        }

    }
    else
    {
        result =  SFS_ERROR_INVALID_PARAM;
    }
    return result;
}

/*****************************************************************************/
//  Description : ��filearray�����������
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Sort(  //����True�ɹ���Falseʧ��
    void *       music_array, //in, filearrayֵ
    APLAYER_LIST_SORT_TYPE_E sort_type,   //in, ��������
    MMIVIRTUALARRAY_COMPARE  compare_func
)
{
    BOOLEAN     result = FALSE;

    if(NULL == music_array
            || APLAYER_LSIT_SORT_NULL == sort_type
            || PNULL == compare_func)
    {
        return FALSE;
    }

#ifdef MMI_FMM_SORT_API_EXT_SUPPORT
    result = MMIVIRTUALARRAY_Sort(music_array, compare_func);
#endif
    return result;

}


/*****************************************************************************/
//  Description : set buffer size for array
//  Global resource dependence :
//  Author: ying.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_SetBufferSize(
    void*    music_array,
    uint32          size           //counted in KB
)
{
    return MMIVIRTUALARRAY_SetBufferSize(music_array, size);
}
