/*****************************************************************
 **  File Name: 	brw_input.c				     			 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:implemention of history class                   *
 **  feature list:                                               *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  07/04/2009		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
#include "mmi_app_browser_trc.h"
#include "brw_utility.h"
#include "brw_input.h"

#define BRW_INPUT_HISTORY_PATH	"Browser\\Input"

LOCAL BRW_INPUT_HIS_T *s_brw_input_his_ptr = PNULL;//输入历史记录

/*
 *	desc: free input history list
 */
 /*
 *	desc:init input history
 */
PUBLIC BOOLEAN BRW_InputHisInit(void)
{
    uint32  read_len=0;
    int32 i =0;
    
    s_brw_input_his_ptr = (BRW_INPUT_HIS_T*)BRWMem_Alloc(sizeof(BRW_INPUT_HIS_T));
    
    if (PNULL == s_brw_input_his_ptr)
    {
        BRWDebug_PrintWarning("BRW_InputHisInit:malloc s_brw_input_his_ptr failed");

        return FALSE;
    }
    
    BRW_Memset(s_brw_input_his_ptr, 0, sizeof(BRW_INPUT_HIS_T));
    BRWUtil_ReadFileSyn(
            (const uint8*)BRW_INPUT_HISTORY_FILE_NAME,
            (const uint8*)BRW_INPUT_HISTORY_PATH,
            (uint8*)s_brw_input_his_ptr,
            sizeof(BRW_INPUT_HIS_T),
            &read_len
            );
    
    for (i = 0; i < BRW_MAX_INPUT_HIS_ITEM_NUM; i++)
    {
        if (!s_brw_input_his_ptr->input_his_item[i].is_used)//文件容错处理
        {
            break;
        }
    }

    if (i != s_brw_input_his_ptr->his_num
        || read_len != sizeof(BRW_INPUT_HIS_T))
    {
        BRW_Memset(s_brw_input_his_ptr, 0, sizeof(BRW_INPUT_HIS_T));

        //BRWDebug_Printf:"BRW_InputHisInit: data error i =%d,his_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_INPUT_62_112_2_18_2_7_21_737,(uint8*)"dd", i, s_brw_input_his_ptr->his_num);
    }

    //BRWDebug_Printf:"BRW_InputHisInit:read_len =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_INPUT_65_112_2_18_2_7_21_738,(uint8*)"d", read_len);

    return TRUE;
}

/*
 *	desc:exist input history,store data to persistent storage
 */
PUBLIC void BRW_InputHisExit(void)
{
    if (PNULL != s_brw_input_his_ptr)
    {
/*
        BRWUtil_WriteFileSyn(
                (const uint8*)BRW_INPUT_HISTORY_FILE_NAME,
                (const uint8*)BRW_INPUT_HISTORY_PATH,
                (uint8*)s_brw_input_his_ptr,
                sizeof(BRW_INPUT_HIS_T)
                );
*/
        BRWMem_Free(s_brw_input_his_ptr);

        s_brw_input_his_ptr = PNULL;
    }    
}

/*
 *	desc:Delete input history
 */
PUBLIC void BrwDeleteInputHis(void)
{
    if (PNULL != s_brw_input_his_ptr)
    {
        BRW_Memset(s_brw_input_his_ptr, 0, sizeof(BRW_INPUT_HIS_T));

        BRWUtil_WriteFileSyn(
                (const uint8*)BRW_INPUT_HISTORY_FILE_NAME,
                (const uint8*)BRW_INPUT_HISTORY_PATH,
                (uint8*)s_brw_input_his_ptr,
                sizeof(BRW_INPUT_HIS_T)
                );
    }    
}

/*
@desc:delete the input history file
@param: 
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN BRW_ClearInputHis(void)
{
    wchar full_path[BRW_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len=0;//length in wchar
    wchar   *device_name_ptr=PNULL;
    uint16 device_name_len=0;
    BOOLEAN result=TRUE;

    device_name_ptr = BRWPL_GetBrwSysDevName(&device_name_len);
    if(PNULL == device_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRWUtil_ReadFileSyn:device null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_INPUT_114_112_2_18_2_7_21_739,(uint8*)"");
        return FALSE;
    }
    full_path_len = device_name_len + BRW_Strlen((const uint8*)FILE_DEVICE_SLASH)
                + BRW_Strlen((const uint8*)BRW_INPUT_HISTORY_PATH) + BRW_Strlen((const uint8*)BRW_FILE_SLASH)
                + BRW_Strlen((const uint8*)BRW_INPUT_HISTORY_FILE_NAME);
                
    if(full_path_len > BRW_FULL_PATH_MAX_LEN)
    {
        BRWDebug_PrintWarning("BRWUtil_ReadFileSyn:full_path_len too long");
        return FALSE;		
    }
    BRW_WstrCat(full_path,device_name_ptr);
    BRWPL_WstrStrnCatWithAsciiStr(full_path, FILE_DEVICE_SLASH, BRW_FULL_PATH_MAX_LEN + 1);
    BRWPL_WstrStrnCatWithAsciiStr(full_path, BRW_INPUT_HISTORY_PATH, BRW_FULL_PATH_MAX_LEN + 1);
    BRWPL_WstrStrnCatWithAsciiStr(full_path, BRW_FILE_SLASH, BRW_FULL_PATH_MAX_LEN + 1);
    BRWPL_WstrStrnCatWithAsciiStr(full_path, BRW_INPUT_HISTORY_FILE_NAME, BRW_FULL_PATH_MAX_LEN + 1);

    result = BRWPL_DelFileSyn(full_path, full_path_len);
    
    //BRWDebug_Printf:"[BRW]BRW_ClearInputHis result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_INPUT_134_112_2_18_2_7_21_740,(uint8*)"d", result);

    return TRUE;
}

/*
 *	desc:add input history item
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwInputHisAddItem(const uint8* url_ptr, uint16 url_len)
{
    if (PNULL != s_brw_input_his_ptr && PNULL != url_ptr)
    {
        int32 i = 0;
        BOOLEAN is_input_exist = FALSE;
        uint16 overwrite_index = 0;

        url_len = MIN(BRW_MAX_URL_LEN, url_len);

        for (i = 0; i < s_brw_input_his_ptr->his_num; i++)//find same record
        {
            if (s_brw_input_his_ptr->input_his_item[i].is_used)
            {
                uint16 his_url_len = 0;

                his_url_len = strlen((char *)s_brw_input_his_ptr->input_his_item[i].input_his_info.url_ptr);
            
                if ((his_url_len == url_len)
                     && (0 == memcmp(s_brw_input_his_ptr->input_his_item[i].input_his_info.url_ptr, url_ptr, url_len)))//same record
                {
                    is_input_exist = TRUE;

                    break;
                }
            }
        }

        if (is_input_exist)
        {
            if (i > 0)
            {
                overwrite_index = i;
            }
            else
            {
                overwrite_index = 0;
            }
        }
        else
        {        
            overwrite_index = MIN(s_brw_input_his_ptr->his_num, BRW_MAX_INPUT_HIS_ITEM_NUM - 1);

            if (s_brw_input_his_ptr->his_num < BRW_MAX_INPUT_HIS_ITEM_NUM)
            {
                s_brw_input_his_ptr->his_num++;
            }
            else
            {
                //BRWDebug_Printf:"BrwInputHisAddItem:s_brw_input_his_ptr->his_num =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_INPUT_192_112_2_18_2_7_21_741,(uint8*)"d", s_brw_input_his_ptr->his_num);
            }
        }

        if (overwrite_index > 0 && overwrite_index < BRW_MAX_INPUT_HIS_ITEM_NUM)
        {
            for (i = overwrite_index; i > 0; i--)
            {
                SCI_MEMSET(&s_brw_input_his_ptr->input_his_item[i], 0x00, sizeof(BRW_INPUT_HIS_ITEM_T));

                SCI_MEMCPY(&s_brw_input_his_ptr->input_his_item[i], &s_brw_input_his_ptr->input_his_item[i - 1], sizeof(BRW_INPUT_HIS_ITEM_T));
            }
        }
        
        SCI_MEMSET(&s_brw_input_his_ptr->input_his_item[0], 0x00, sizeof(BRW_INPUT_HIS_ITEM_T));

        SCI_MEMCPY(&s_brw_input_his_ptr->input_his_item[0].input_his_info.url_ptr, url_ptr, url_len);

        s_brw_input_his_ptr->input_his_item[0].is_used = TRUE;

        BRWUtil_WriteFileSyn(
                (const uint8*)BRW_INPUT_HISTORY_FILE_NAME,
                (const uint8*)BRW_INPUT_HISTORY_PATH,
                (uint8*)s_brw_input_his_ptr,
                sizeof(BRW_INPUT_HIS_T)
                );
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/*
 *	desc:Get match input history num
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC uint16 BRW_GetMatchInputHisNum(const uint8 *url, uint16 url_len)
{
    int32 i = 0;
    uint16 match_num = 0;

    if (PNULL == url || 0 == url_len)
    {
        match_num = s_brw_input_his_ptr->his_num;
    }
    else
    {
        for (i = 0; i < s_brw_input_his_ptr->his_num; i++)
        {
            if (s_brw_input_his_ptr->input_his_item[i].is_used)
            {
                uint16 his_url_len = strlen((char *)s_brw_input_his_ptr->input_his_item[i].input_his_info.url_ptr);

                if (url_len <= his_url_len
                    && 0 == memcmp(s_brw_input_his_ptr->input_his_item[i].input_his_info.url_ptr, url, url_len))
                {
                    match_num++;
                }
            }
        }
    }

    return match_num;
}

/*
 *	desc:Get match input history num by index
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC uint8* BRW_GetMatchInputHisUrlByIndex(const uint8 *url, uint16 url_len, uint16 index)
{
    int32 i = 0;
    uint16 match_num = 0;

    if (index >= s_brw_input_his_ptr->his_num)
    {
        //BRWDebug_Printf:"BRW_GetMatchInputHisUrl:index =%d, his_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_INPUT_264_112_2_18_2_7_21_742,(uint8*)"dd", index, s_brw_input_his_ptr->his_num);

        return PNULL;
    }

    if (PNULL == url || 0 == url_len)
    {
        return s_brw_input_his_ptr->input_his_item[index].input_his_info.url_ptr;
    }
    else
    {
        for (i = 0; i < s_brw_input_his_ptr->his_num; i++)
        {
            if (s_brw_input_his_ptr->input_his_item[i].is_used)
            {
                uint16 his_url_len = strlen((char *)s_brw_input_his_ptr->input_his_item[i].input_his_info.url_ptr);

                if (url_len <= his_url_len
                    && 0 == memcmp(s_brw_input_his_ptr->input_his_item[i].input_his_info.url_ptr, url, url_len))
                {
                    if (match_num == index)
                    {
                        return s_brw_input_his_ptr->input_his_item[i].input_his_info.url_ptr;
                    }

                    match_num++;
                }

            }
        }
    }

    return PNULL;
}
#endif
