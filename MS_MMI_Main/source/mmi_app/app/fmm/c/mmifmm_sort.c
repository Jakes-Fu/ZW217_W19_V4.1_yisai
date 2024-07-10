/*****************************************************************************
** File Name:      mmifmm_sort.c                                             *
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

#define _MMIFMM_SORT_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_sort.h"
#include "mmifmm_comfunc.h"
#include "mmiim.h"
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
/*****************************************************************************/
//  Description : 预排序，将文件夹排前，文件排后
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_FilePreSort (MMIFMM_DATA_INFO_T* data_ptr)
{
    BOOLEAN result = FALSE;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW ("MMIFMM_FilePreSort PNULL == data_ptr!");
        return result;
    }

    if (0 != data_ptr->file_num && 0 != data_ptr->folder_num)
    {//需要进行排序的情?
        uint16 begin = 0;
        uint16 end = data_ptr->folder_num + data_ptr->file_num - 1;
        MMIFMM_FILE_INFO_T *temp_data = PNULL;

        while (begin < end)
        {
            if (data_ptr->data[begin]->type == MMIFMM_FILE_TYPE_FOLDER)
            {
                begin++;
            }
            else
            {
                while (data_ptr->data[end]->type != MMIFMM_FILE_TYPE_FOLDER)
                {
                    end--;
                }

                if (begin < end)
                {
                    temp_data = (data_ptr->data[begin]) ;
                    (data_ptr->data[begin]) = (data_ptr->data[end]);
                    (data_ptr->data[end]) = temp_data;
                    end--;
                }
            }
        }//while
    }
    result = TRUE;

    return result;
}




/*****************************************************************************/
//  Description : sort
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_FileDataSort (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_SORT_TYPE_E    sort
)
{
    SCI_TRACE_LOW ("MMIFMM: MMIAPIFMM_FileDataSort, sort =%d.", sort);

    //SCI_ASSERT(PNULL != list_data_ptr);
    if (PNULL == list_data_ptr)
    {
        SCI_TRACE_LOW ("MMIFMM: MMIAPIFMM_FileDataSort, param error:PNULL == list_data_ptr");
        return 0;
    }

    switch (sort)
    {
    case MMIFMM_SORT_TYPE_NAME:

        // 排序文件夹
        if (0 < list_data_ptr->folder_num)
        {
            qsort (list_data_ptr->data, list_data_ptr->folder_num, sizeof (MMIFMM_FILE_INFO_T*), MMIAPIFMM_CompareFileName);
        }

        // 排序文件
        qsort (list_data_ptr->data + list_data_ptr->folder_num, list_data_ptr->file_num, sizeof (MMIFMM_FILE_INFO_T*), MMIAPIFMM_CompareFileName);
        list_data_ptr->sort  = MMIFMM_SORT_TYPE_NAME;
        break;

    case MMIFMM_SORT_TYPE_TIME:

        // 排序文件夹
        if (0 < list_data_ptr->folder_num)
        {
            qsort (list_data_ptr->data, list_data_ptr->folder_num, sizeof (MMIFMM_FILE_INFO_T*), MMIAPIFMM_CompareFileTime);
        }

        // 排序文件
        qsort (list_data_ptr->data + list_data_ptr->folder_num, list_data_ptr->file_num, sizeof (MMIFMM_FILE_INFO_T*), MMIAPIFMM_CompareFileTime);
        list_data_ptr->sort  = MMIFMM_SORT_TYPE_TIME;
        break;
    case MMIFMM_SORT_TYPE_SIZE:

        if (0 < list_data_ptr->folder_num)
        {
            qsort (list_data_ptr->data, list_data_ptr->folder_num, sizeof (MMIFMM_FILE_INFO_T*), MMIAPIFMM_CompareFileSize);
        }

        // 排序文件
        qsort (list_data_ptr->data + list_data_ptr->folder_num, list_data_ptr->file_num, sizeof (MMIFMM_FILE_INFO_T*), MMIAPIFMM_CompareFileSize);
        list_data_ptr->sort  = MMIFMM_SORT_TYPE_SIZE;
        break;

    case MMIFMM_SORT_TYPE_TYPE:

        // 排序文件夹
        if (0 < list_data_ptr->folder_num)
        {
            qsort (list_data_ptr->data, list_data_ptr->folder_num, sizeof (MMIFMM_FILE_INFO_T*), MMIAPIFMM_CompareFileType);
        }

        // 排序文件
        qsort (list_data_ptr->data + list_data_ptr->folder_num, list_data_ptr->file_num, sizeof (MMIFMM_FILE_INFO_T*), MMIAPIFMM_CompareFileType);
        list_data_ptr->sort  = MMIFMM_SORT_TYPE_TYPE;
        break;

    default:
        SCI_TRACE_LOW ("MMIAPIFMM_FileDataSort sort = %d", sort);
        break;
    }

    return (1);
}


/*****************************************************************************
 	Description : Compare the double string. Compare the double characters by pinyin format if
 	                    the characters are ucs2.
	Global resource dependence : none
       Author:
	Note:
*****************************************************************************/
LOCAL int CompareName (
    const wchar       *wstr1_ptr,
    uint16             wstr1_len,
    const wchar       *wstr2_ptr,
    uint16             wstr2_len
)
{
    int32   result = 0;
    int16  min_len = 0;
    BOOLEAN is_ucs2_ch1 = FALSE;
    BOOLEAN is_ucs2_ch2 = FALSE;
    uint16 diff_char1 = 0;
    uint16 diff_char2 = 0;
    uint8   flag = 0;
    //modified by liyan.zhu for c++ test
    //wchar ucs2_str1[10] = {0};
    //wchar ucs2_str2[10] = {0};

    //check string length.
    if ( (0 == wstr1_len) && (0 == wstr2_len))
    {
        return (0);
    }
    else if ( (0 == wstr1_len) || (0 == wstr2_len))
    {
        return ( (wstr1_len < wstr2_len) ? (-1) : (1));
    }

    //check string pointer.
    if (wstr1_ptr == PNULL && wstr2_ptr == PNULL)
    {
        return (0);
    }
    else if (wstr1_ptr == PNULL && wstr2_ptr != PNULL)
    {
        return (-1);
    }
    else if (wstr1_ptr != PNULL && wstr2_ptr == PNULL)
    {
        return (1);
    }

    min_len = (int16) MIN (wstr1_len, wstr2_len);

    /*lint -save -e613 -e794 */
    while (min_len && *wstr1_ptr && *wstr1_ptr == *wstr2_ptr)
    {
        wstr1_ptr++;
        wstr2_ptr++;
        min_len--;
    }

    if (0 == min_len)  //前面几个字符都相同，则比较字串长度
    {
        if (wstr1_len == wstr2_len)
        {
            return 0;
        }
        else
        {
            return ( (wstr1_len < wstr2_len) ? (-1) : (1));
        }
    }

    diff_char1 = (uint16) * wstr1_ptr;
    diff_char2 = (uint16) * wstr2_ptr;
    /*lint -restore */

    is_ucs2_ch1 = MMIAPICOM_IsUcs2Char (diff_char1);
    is_ucs2_ch2 = MMIAPICOM_IsUcs2Char (diff_char2);

    //全角符号转化成半角符号
    if (is_ucs2_ch1 && MMIFMM_ConvertSBC2DBC (&diff_char1))
    {
        flag |= BIT_0;
        is_ucs2_ch1 = FALSE;
    }

    if (is_ucs2_ch2 && MMIFMM_ConvertSBC2DBC (&diff_char2))
    {
        flag |= BIT_1;
        is_ucs2_ch2 = FALSE;
    }

    if (is_ucs2_ch1 && is_ucs2_ch2)
    {
        wchar ucs2_str1[10] = {0};
        wchar ucs2_str2[10] = {0};
        //Compare pinyin index if the double ucs2 characters.
        MMIAPIIM_GetTextLetters (diff_char1, ucs2_str1);
        MMIAPIIM_GetTextLetters (diff_char2, ucs2_str2);

        result = MMIAPICOM_Wstrcmp (ucs2_str1, ucs2_str2);
    }
    else if (!is_ucs2_ch1 && !is_ucs2_ch2)
    {
        //Both charaters is not ucs2 charater.
        flag |= MMIFMM_ConvertToLower (&diff_char1) ? BIT_0 : 0;
        flag |= MMIFMM_ConvertToLower (&diff_char2) ? BIT_1 : 0;

        result = (diff_char1 - diff_char2);
    }
    else
    {
        //Either of characters is ucs2 charater.
        result = is_ucs2_ch1 ? (1) : (-1);
    }

    if (0 == result)
    {
        if (0 == flag)
        {
            //Compare the name length if the charaters is equal.
            result = (wstr1_len > wstr2_len) ? (1) : (-1);
        }
        else
        {
            //Switch the position if the first charater is SBC.
            result = (flag & BIT_0) ? (-1) : (1);
        }
    }

    return ( result > 0 ? 1 : -1 );//coverity 10069
    //return (result > 0 ? 1 : (result < 0 ? -1 : 0));
}
//-CR230144

/*****************************************************************************/
//  Description : 比较名称
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileName (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
)
{
    int                 result  = 0;
    MMIFMM_FILE_INFO_T  *data1_ptr = PNULL, *data2_ptr = PNULL;

    //SCI_ASSERT(PNULL != i1_data && PNULL != i2_data);
    if ( (PNULL == i1_data) || (PNULL == i2_data))
    {
        SCI_TRACE_LOW ("MMIAPIFMM_CompareFileName param error!");
        return result;
    }

    data1_ptr = * (MMIFMM_FILE_INFO_T **) i1_data;
    data2_ptr = * (MMIFMM_FILE_INFO_T **) i2_data;

    //+CR230144
    result = CompareName (data1_ptr->filename_ptr, data1_ptr->filename_len,
                          data2_ptr->filename_ptr, data2_ptr->filename_len);

    //result = MMIAPICOM_CompareTwoWstrExt(data1_ptr->filename, data1_ptr->filename_len,
    //    data2_ptr->filename, data2_ptr->filename_len, FALSE);
    //-CR230144

    return (result);
}

/*****************************************************************************/
//  Description : 比较文件大小
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileSize (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
)
{
    int32               result  = 0;
    MMIFMM_FILE_INFO_T  *data1_ptr = PNULL, *data2_ptr = PNULL;

    //SCI_ASSERT(PNULL != i1_data && PNULL != i2_data);
    if ( (PNULL == i1_data) || (PNULL == i2_data))
    {
        SCI_TRACE_LOW ("MMIAPIFMM_CompareFileSize param error!");
        return result;
    }

    data1_ptr = * (MMIFMM_FILE_INFO_T **) i1_data;
    data2_ptr = * (MMIFMM_FILE_INFO_T **) i2_data;

    if (data1_ptr->file_size > data2_ptr->file_size)
    {
        result = 1;
    }
    else if (data1_ptr->file_size < data2_ptr->file_size)
    {
        result = -1;
    }
    else
    {
        result = MMIAPICOM_CompareTwoWstr (data1_ptr->filename_ptr, data1_ptr->filename_len,
                                           data2_ptr->filename_ptr, data2_ptr->filename_len);
    }

    return (result);
}

/*****************************************************************************/
//  Description : 比较文件时间
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileTime (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
)
{
    MMIFMM_FILE_INFO_T *data1_ptr = PNULL,   *data2_ptr = PNULL;
    int32          result  = 0;

    //SCI_ASSERT(PNULL != i1_data && PNULL != i2_data);
    if ( (PNULL == i1_data) || (PNULL == i2_data))
    {
        SCI_TRACE_LOW ("MMIAPIFMM_CompareFileTime param error!");
        return result;
    }

    data1_ptr = * (MMIFMM_FILE_INFO_T **) i1_data;
    data2_ptr = * (MMIFMM_FILE_INFO_T **) i2_data;

    if (data1_ptr->time > data2_ptr->time)
    {
        result = 1;
    }
    else if (data1_ptr->time < data2_ptr->time)
    {
        result = -1;
    }
    else
    {
        result = MMIAPICOM_CompareTwoWstr (data1_ptr->filename_ptr, data1_ptr->filename_len,
                                           data2_ptr->filename_ptr, data2_ptr->filename_len);
    }

    return (result);
}
/*****************************************************************************/
//  Description : CompareType
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL int32 CompareType (
    wchar * data1_suf ,
    uint16 data1_suf_len,
    wchar * data2_suf,
    uint16 data2_suf_len
)
{
    MMIFMM_FILE_TYPE_E data1_type = MMIAPIFMM_ConvertFileType (data1_suf, data1_suf_len);
    MMIFMM_FILE_TYPE_E data2_type = MMIAPIFMM_ConvertFileType (data2_suf, data2_suf_len);

    if (data2_type > data1_type)
    {
        return -1;
    }
    else if (data2_type < data1_type)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
/*****************************************************************************/
//  Description : GetSuffixOffset
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSuffixOffset (wchar * filename,
                              uint16 filename_len,
                              MMIFMM_FILE_TYPE_E  type)
{
    uint16 suffix_offset = 0;

    if (MMIFMM_FILE_TYPE_FOLDER != type)
    {
        for (suffix_offset = (uint16) (filename_len - 1); suffix_offset > 0; suffix_offset--)
        {
            if (MMIFILE_SLASH == filename[suffix_offset])
            {
                suffix_offset = 0;
                break;
            }
            else if (MMIFILE_DOT == filename[suffix_offset])
            {
                break;
            }
        }
    }

    return suffix_offset;
}

/*****************************************************************************/
//  Description : 比较文件类型
//  Global resource dependence :
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileType (
    const void     *i1_data,   // first data
    const void     *i2_data    // second data
)
{
    uint16              data1_suffix_offset = 0;
    uint16              data2_suffix_offset = 0;
    MMIFMM_FILE_INFO_T  *data1_ptr = PNULL, *data2_ptr = PNULL;
    int32 type_cmp = 0;
    int32 suffix_cmp = 0;
    int32 filename_cmp = 0;

    //SCI_ASSERT(PNULL != i1_data && PNULL != i2_data);
    if ( (PNULL == i1_data) || (PNULL == i2_data))
    {
        SCI_TRACE_LOW ("MMIAPIFMM_CompareFileType param error!");
        return type_cmp;
    }

    data1_ptr = * (MMIFMM_FILE_INFO_T **) i1_data;
    data2_ptr = * (MMIFMM_FILE_INFO_T **) i2_data;

    data1_suffix_offset = GetSuffixOffset (data1_ptr->filename_ptr, data1_ptr->filename_len, data1_ptr->type);
    data2_suffix_offset = GetSuffixOffset (data2_ptr->filename_ptr, data2_ptr->filename_len, data2_ptr->type);

    type_cmp = CompareType (
                   &data1_ptr->filename_ptr[data1_suffix_offset+1],
                   (uint16) (data1_ptr->filename_len - data1_suffix_offset - 1),
                   &data2_ptr->filename_ptr[data2_suffix_offset+1],
                   (uint16) (data2_ptr->filename_len - data2_suffix_offset - 1)
               );

    if (type_cmp != 0)
    {
        return type_cmp;
    }

    if (0 != data1_suffix_offset && 0 != data2_suffix_offset)
    {
        suffix_cmp = MMIAPICOM_CompareTwoWstrExt (
                         &data1_ptr->filename_ptr[data1_suffix_offset+1],
                         (uint16) (data1_ptr->filename_len - data1_suffix_offset - 1),
                         &data2_ptr->filename_ptr[data2_suffix_offset+1],
                         (uint16) (data2_ptr->filename_len - data2_suffix_offset - 1),
                         FALSE);

        if (suffix_cmp != 0)
        {
            return suffix_cmp;
        }
    }
    //第一个文件没有后缀，第二个文件有后缀
    else if (0 == data1_suffix_offset && data2_suffix_offset > 0)
    {
        return -1;
    }
    //第一个文件有后缀，第二个文件没有后缀
    else if (data1_suffix_offset > 0 && 0 == data2_suffix_offset)
    {
        return 1;
    }

    filename_cmp = MMIAPICOM_CompareTwoWstrExt (
                       data1_ptr->filename_ptr,
                       data1_ptr->filename_len,
                       data2_ptr->filename_ptr,
                       data2_ptr->filename_len,
                       FALSE);

    return filename_cmp;
}

