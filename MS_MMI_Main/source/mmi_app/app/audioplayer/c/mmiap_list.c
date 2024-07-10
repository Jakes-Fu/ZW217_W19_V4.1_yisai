/*****************************************************************************
** File Name:               mmiap_list.c                                     *
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

#define _MMIAP_LIST_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmiap_musicarray.h"
#include "mmiap_list.h"
#include "mmisrvfmm_export.h"
#include "mmi_filemgr.h"
#include "mmk_app.h"
#include "mmi_string.h"
#include "mmisrvim.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SBC_CHAR_START                      (0xFF01)            //全角符号:"!"
#define SBC_CHAR_END                        (0xFF5E)            //全角符号:"~"
#define SBC_TO_DBC_STEP                     (0xFEE0)            //全角半角转换间隔
#define SBC_SPACE                           (0x3000)            //全角符号:空格
#define DBC_SPACE                           (0x20)              //半角空格

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

/*****************************************************************************/
//  Description : compare two item
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CompareItemData(APLAYER_LIST_ITEM_DATA_T item_data1,
                              APLAYER_LIST_ITEM_DATA_T item_data2,
                              BOOLEAN is_only_for_name);

/*****************************************************************************/
//  Description : 名字升序排序
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareNameAsce(
    const void       *data_1_ptr,
    const void       *data_2_ptr
);


/*****************************************************************************/
//  Description : 文件类型升序排序
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareTypeAsce(
    const void       *data_1_ptr,
    const void       *data_2_ptr
);

/*****************************************************************************/
//  Description : asecend as file time
//  Global resource dependence :
//  Author: ying.xu
//  Note:       20100902
/*****************************************************************************/
LOCAL int CompareMTimeAsec(
    const void       *data_1_ptr,
    const void       *data_2_ptr
);

/*****************************************************************************/
//  Description : asecend as file size
//  Global resource dependence :
//  Author: ying.xu
//  Note:       20100902
/*****************************************************************************/
LOCAL int CompareSizeAsec(
    const void       *data_1_ptr,
    const void       *data_2_ptr
);

/*****************************************************************************
    Description : Convert the characer from SBC to DBC.
    Global resource dependence : none
       Author:
    Note:
*****************************************************************************/
LOCAL BOOLEAN ConvertSBC2DBC (
    uint16 *ch    //IN/OUT:全角符号转换成半角
);

/*****************************************************************************
    Description : Convert the character from upper to lower
    Global resource dependence : none
       Author:
    Note:
*****************************************************************************/
LOCAL BOOLEAN ConvertToLower (uint16 *ch);

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
);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : add an item to the list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_AddItem(APLAYER_LIST_HANDLE list_handle,
                                       APLAYER_LIST_ITEM_DATA_T item_data)
{
    BOOLEAN     result  = FALSE;

    if (PNULL != list_handle)
    {
        result = MMIAPLAYERARRAY_Add(list_handle,&item_data);
    }

    SCI_TRACE_LOW("[APlayer] MMIAPlayer_LIST_AddItem result = %d", result);
    return result;

}

/*****************************************************************************/
//  Description : delete an item to the list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_LIST_DeleteItem(APLAYER_LIST_HANDLE list_handle,
        uint16  index,
        BOOLEAN is_delete_file) //delete file from disk
{
    APLAYER_LIST_ITEM_DATA_T item_data = {0};
    APLAYER_LIST_ERROR_E  result  = APLAYER_LIST_INVALID_PARAM;

    if (PNULL != list_handle)
    {
        if (is_delete_file)
        {
            if(!MMIAPLAYERARRAY_Read(list_handle,index,&item_data))
            {
                result = APLAYER_LIST_DATA_ERROR;
                is_delete_file = FALSE;
            }
        }


        if (!MMIAPLAYERARRAY_Delete(list_handle,index))
        {
            result = APLAYER_LIST_ERROR;
        }
        else if(is_delete_file)
        {
            if(SFS_NO_ERROR ==  MMIAPIFMM_DeleteFile(item_data.filename,PNULL))
            {
                result = APLAYER_LIST_SUCCESS;
            }
            else
            {
                result = APLAYER_LIST_DELETE_FILE_ERROR;
            }
        }
        else
        {
            result = APLAYER_LIST_SUCCESS;
        }
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_LIST_DeleteItem result = %d, index = %d, is_delete_file = %d", result, index, is_delete_file);
    return result;
}

/*****************************************************************************/
//  Description : get list total item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPlayer_LIST_GetTotalNum(APLAYER_LIST_HANDLE list_handle)
{
    uint32      total_num  = 0;

    if (PNULL != list_handle)
    {
        total_num = MMIAPLAYERARRAY_GetTotalNum(list_handle);
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_LIST_GetTotalNum total_num = %d",total_num);
    return total_num;
}

/*****************************************************************************/
//  Description : load list from file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_LoadFromFile(APLAYER_LIST_HANDLE list_handle,
        wchar   *full_path_ptr,
        uint16  full_path_len)
{
    BOOLEAN     result     = FALSE;

    if (PNULL != list_handle
            && PNULL != full_path_ptr)
    {

        result = MMIAPLAYERARRAY_LoadFromFile(list_handle,full_path_ptr,full_path_len);
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_LIST_LoadFromFile result = %d, full_path_len = %d", result, full_path_len);
    return result;
}

/*****************************************************************************/
//  Description : modify item data
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_ModifyItem(APLAYER_LIST_HANDLE list_handle,
        uint16 index,
        APLAYER_LIST_ITEM_DATA_T item_data)
{

    BOOLEAN     result     = FALSE;

    if (PNULL != list_handle)
    {
        result = MMIAPLAYERARRAY_Modify(list_handle,index,&item_data);
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_LIST_ModifyItem index = %d, result = %d", index, result);
    return result;
}

/*****************************************************************************/
//  Description : get item data
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_GetItemData(APLAYER_LIST_HANDLE list_handle,
        uint16 index,
        APLAYER_LIST_ITEM_DATA_T *item_data_ptr)
{
    BOOLEAN     result     = FALSE;

    if (PNULL != list_handle
            && PNULL != item_data_ptr)
    {
        result = MMIAPLAYERARRAY_Read(list_handle,index,item_data_ptr);
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_LIST_GetItemData index = %d, result = %d", index, result);
    return result;
}

/*****************************************************************************/
//  Description : save list to file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_SaveFile(APLAYER_LIST_HANDLE list_handle,
                                        wchar  *full_path_ptr,
                                        uint16  full_path_len)
{
    BOOLEAN     result     = FALSE;

    if (PNULL != list_handle
            && PNULL != full_path_ptr)
    {
        result =  MMIAPLAYERARRAY_SaveArrayToFile(list_handle,sizeof(APLAYER_LIST_ITEM_DATA_T),full_path_ptr,full_path_len);
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_LIST_SaveFile result = %d, full_path_len = %d", result, full_path_len);
    return result;
}

/*****************************************************************************
    Description : Convert the characer from SBC to DBC.
    Global resource dependence : none
       Author:
    Note:
*****************************************************************************/
LOCAL BOOLEAN ConvertSBC2DBC (
    uint16 *ch    //IN/OUT:全角符号转换成半角
)
{
    BOOLEAN result = FALSE;

    if ( (*ch >= SBC_CHAR_START) && (*ch <= SBC_CHAR_END))
    {
        *ch -= SBC_TO_DBC_STEP;

        result = TRUE;
    }
    else if (*ch == SBC_SPACE)
    {
        *ch = DBC_SPACE;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************
    Description : Convert the character from upper to lower
    Global resource dependence : none
       Author:
    Note:
*****************************************************************************/
LOCAL BOOLEAN ConvertToLower (uint16 *ch)
{
    if ('A' <= *ch && *ch <= 'Z')
    {
        //upper case change to low case
        *ch = *ch + ('a' - 'A');
        return TRUE;
    }

    return FALSE;
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
    if (is_ucs2_ch1 && ConvertSBC2DBC (&diff_char1))
    {
        flag |= BIT_0;
        is_ucs2_ch1 = FALSE;
    }

    if (is_ucs2_ch2 && ConvertSBC2DBC (&diff_char2))
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
        flag |= ConvertToLower (&diff_char1) ? BIT_0 : 0;
        flag |= ConvertToLower (&diff_char2) ? BIT_1 : 0;

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

/*****************************************************************************/
//  Description : 名字升序排序
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareNameAsce(
    const void       *data_1_ptr,
    const void       *data_2_ptr
)
{
    int     result = 0;
    APLAYER_LIST_ITEM_DATA_T    *file_data_1 = NULL;
    APLAYER_LIST_ITEM_DATA_T    *file_data_2 = NULL;
    wchar file_name1[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 file_name1_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar file_name2[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 file_name2_len = MMIFILE_FULL_PATH_MAX_LEN;

    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//容错
    {
        return -1;
    }

    file_data_1 = (APLAYER_LIST_ITEM_DATA_T*)data_1_ptr;
    file_data_2 = (APLAYER_LIST_ITEM_DATA_T*)data_2_ptr;

    MMIAPIFMM_SplitFullPath(file_data_1->filename,file_data_1->name_len,PNULL,PNULL,PNULL,PNULL,file_name1,&file_name1_len);
    MMIAPIFMM_SplitFullPath(file_data_2->filename,file_data_2->name_len,PNULL,PNULL,PNULL,PNULL,file_name2,&file_name2_len);
    result = CompareName(file_name1, file_name1_len,
                         file_name2, file_name2_len);

    if(0 < result)
    {
        return 1;
    }
    else if(0 == result)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/*****************************************************************************/
//  Description : 文件类型升序排序
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareTypeAsce(
    const void       *data_1_ptr,
    const void       *data_2_ptr
)
{
    APLAYER_LIST_ITEM_DATA_T    *file_data_1 = NULL;
    APLAYER_LIST_ITEM_DATA_T    *file_data_2 = NULL;
    int32               result  = 0;
    uint16              data1_suffix_offset = 0;
    uint16              data2_suffix_offset = 0;


    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//容错
    {

        return 0;
    }

    file_data_1 = (APLAYER_LIST_ITEM_DATA_T*)data_1_ptr;
    file_data_2 = (APLAYER_LIST_ITEM_DATA_T*)data_2_ptr;


    if(0 == file_data_1->name_len)
    {
        return (int)file_data_1->name_len - (int)file_data_2->name_len;
    }
    for (data1_suffix_offset = (uint16)(file_data_1->name_len -1); data1_suffix_offset > 0; data1_suffix_offset--)
    {
        if (MMIFILE_DOT == file_data_1->filename[data1_suffix_offset])
        {
            break;
        }
    }
    for (data2_suffix_offset = (uint16)(file_data_2->name_len -1);  data2_suffix_offset > 0; data2_suffix_offset--)
    {
        if (MMIFILE_DOT == file_data_2->filename[data2_suffix_offset])
        {
            break;
        }
    }

    if (0 == data1_suffix_offset  &&  data2_suffix_offset > 0)
    {
        // 第一个文件没有后缀
        result  = -1;
    }
    else if (0 == data2_suffix_offset  &&  data1_suffix_offset > 0)
    {
        // 第二个文夹没有后缀
        return 1;
    }
    else if (0 == data1_suffix_offset   && 0 == data2_suffix_offset)
    {
        // 都没有后缀，比较文件名
        result = MMIAPICOM_CompareTwoWstrExt( file_data_1->filename, file_data_1->name_len,
                                              file_data_2->filename, file_data_2->name_len, FALSE);
    }
    else
    {
        // 都有后缀，比较后缀
        result = MMIAPICOM_CompareTwoWstrExt( &file_data_1->filename[data1_suffix_offset+1], (uint16)(file_data_1->name_len - data1_suffix_offset-1),
                                              &file_data_2->filename[data2_suffix_offset+1], (uint16)(file_data_2->name_len- data2_suffix_offset-1),FALSE);

        if(0 == result)
        {
            result = MMIAPICOM_CompareTwoWstrExt( file_data_1->filename, file_data_1->name_len,
                                                  file_data_2->filename, file_data_2->name_len, FALSE);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : asecend as file time
//  Global resource dependence :
//  Author: ying.xu
//  Note:       20100902
/*****************************************************************************/
LOCAL int CompareMTimeAsec(
    const void       *data_1_ptr,
    const void       *data_2_ptr
)
{
    APLAYER_LIST_ITEM_DATA_T    *file_data_1 = NULL;
    APLAYER_LIST_ITEM_DATA_T    *file_data_2 = NULL;


    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//容错
    {
        return -1;
    }

    file_data_1 = (APLAYER_LIST_ITEM_DATA_T*)data_1_ptr;
    file_data_2 = (APLAYER_LIST_ITEM_DATA_T*)data_2_ptr;


    if(file_data_1->modify_date.year != file_data_2->modify_date.year)
    {
        return file_data_1->modify_date.year - file_data_2->modify_date.year ;
    }
    else if( file_data_1->modify_date.mon !=  file_data_2->modify_date.mon)
    {
        return file_data_1->modify_date.mon -  file_data_2->modify_date.mon ;
    }
    else if( file_data_1->modify_date.mday !=  file_data_2->modify_date.mday)
    {
        return file_data_1->modify_date.mday -  file_data_2->modify_date.mday ;
    }
    else if( file_data_1->modify_time.hour !=  file_data_2->modify_time.hour)
    {
        return file_data_1->modify_time.hour -  file_data_2->modify_time.hour ;
    }
    else if( file_data_1->modify_time.min !=  file_data_2->modify_time.min)
    {
        return file_data_1->modify_time.min -  file_data_2->modify_time.min ;
    }
    else if( file_data_1->modify_time.sec !=  file_data_2->modify_time.sec)
    {
        return file_data_1->modify_time.sec -  file_data_2->modify_time.sec ;
    }
    else
    {
        return -1;
    }
}


/*****************************************************************************/
//  Description : asecend as file size
//  Global resource dependence :
//  Author: ying.xu
//  Note:       20100902
/*****************************************************************************/
LOCAL int CompareSizeAsec(
    const void       *data_1_ptr,
    const void       *data_2_ptr
)
{
    APLAYER_LIST_ITEM_DATA_T    *file_data_1 = NULL;
    APLAYER_LIST_ITEM_DATA_T    *file_data_2 = NULL;


    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//容错
    {
        return 0;
    }

    file_data_1 = (APLAYER_LIST_ITEM_DATA_T*)data_1_ptr;
    file_data_2 = (APLAYER_LIST_ITEM_DATA_T*)data_2_ptr;

    if(file_data_1->size != file_data_2->size)
    {
        return file_data_1->size - file_data_2->size ;
    }
    else
    {
        return file_data_1->size - file_data_2->size ;
    }
}
/*****************************************************************************/
//  Description : sort list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_Sort(APLAYER_LIST_HANDLE list_handle,
                                    APLAYER_LIST_SORT_TYPE_E sort_type)
{

    BOOLEAN     result     = FALSE;
    MMIVIRTUALARRAY_COMPARE compare_func = PNULL;

    if (PNULL != list_handle)
    {
        switch(sort_type)
        {
        case APLAYER_LSIT_SORT_NAME:
            compare_func = CompareNameAsce;
            break;

        case APLAYER_LSIT_SORT_TIME:
            compare_func = CompareMTimeAsec;
            break;

        case APLAYER_LSIT_SORT_SIZE:
            compare_func = CompareSizeAsec;
            break;

        case APLAYER_LSIT_SORT_TYPE:
            compare_func = CompareTypeAsce;
            break;

        default:
            break;
        }

        result =  MMIAPLAYERARRAY_Sort(list_handle,sort_type,compare_func);
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_LIST_Sort result = %d, sort_type = %d", result, sort_type);
    return result;
}

/*****************************************************************************/
//  Description : create list handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_HANDLE MMIAPlayer_LIST_Create(void)
{
    FILEARRAY list_handle = PNULL;

    list_handle = MMIAPLAYERARRAY_CreateArray(sizeof(APLAYER_LIST_ITEM_DATA_T),_D_FILE_NAME,_D_FILE_LINE);

    SCI_TRACE_LOW("[APlayer] MMIAPlayer_LIST_Create list_handle = %d", list_handle);
    return list_handle;

}

/*****************************************************************************/
//  Description : destroy list handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_LIST_Destroy(APLAYER_LIST_HANDLE list_handle)
{

    SCI_TRACE_LOW("[APlayer] MMIAPlayer_LIST_Destroy list_handle = %d", list_handle);

    if (PNULL != list_handle)
    {
        MMIAPLAYERARRAY_DestroyArray(&list_handle);
    }
}

/*****************************************************************************/
//  Description : compare two item
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CompareItemData(APLAYER_LIST_ITEM_DATA_T item_data1,
                              APLAYER_LIST_ITEM_DATA_T item_data2,
                              BOOLEAN is_only_for_name)
{
    BOOLEAN result = FALSE;


    if(0 == CompareNameAsce(&item_data1,&item_data2))
    {
        result = TRUE;
    }

    if (result
            && !is_only_for_name)
    {
        result = FALSE;
        if(0 == CompareSizeAsec(&item_data1,&item_data2))
        {
            result = TRUE;
        }

        if (result
                && (0 == CompareMTimeAsec(&item_data1,&item_data2))
            )    
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }

    SCI_TRACE_LOW("[APlayer] CompareItemData is_only_for_name = %d, result = %d", is_only_for_name,result);
    return result;

}
/*****************************************************************************/
//  Description : get item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_LIST_GetItemIndex(APLAYER_LIST_HANDLE list_handle,
        APLAYER_LIST_ITEM_DATA_T item_data,
        uint16  * file_index_ptr,
        BOOLEAN is_only_for_name)
{

    uint32      total_num  = 0;
    uint32      i = 0;
    APLAYER_LIST_ITEM_DATA_T temp_item_data = {0};
    APLAYER_LIST_ERROR_E     result = APLAYER_LIST_FILE_NOT_IN_LIST;

    if (PNULL != list_handle
            && PNULL != file_index_ptr)
    {
        total_num = MMIAPLAYERARRAY_GetTotalNum(list_handle);
        for (i = 0; i < total_num; i++)
        {
            if (MMIAPLAYERARRAY_Read(list_handle,i,&temp_item_data))
            {
                if (CompareItemData(item_data, temp_item_data,is_only_for_name))
                {
                    *file_index_ptr = (uint16)i;
                    if (!MMIAPIFMM_IsFileExist(item_data.filename,item_data.name_len))
                    {
                        result = APLAYER_LIST_FILE_NO_EXIST;
                    }
                    result = APLAYER_LIST_SUCCESS;
                    break;
                }
            }
        }
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_LIST_GetItemIndex i = %d, total_num = %d, result = %d",i,total_num,result);
    return result;

}

/*****************************************************************************/
//  Description : set buffer size
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_SetBufferSize(APLAYER_LIST_HANDLE list_handle,
        uint32 buffer_size)
{
    BOOLEAN     result = FALSE;

    if (PNULL != list_handle
            && 0 != buffer_size)
    {
        result = MMIAPLAYERARRAY_SetBufferSize(list_handle,buffer_size);
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_LIST_SetBufferSize buffer_size = %d, result = %d", buffer_size, result);
    return result;
}

