/*****************************************************************
 **  File Name: 	brw_utility.c    							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:implemention of browser common function         *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  30/12/2008		yuzhi.chen      Create. 					 *
*****************************************************************/
#include "mmi_app_browser_trc.h"
#include "brw_utility.h"
#include "mmifmm_export.h"
#include "brw_control_api.h"

#define isHexDigit(c)  (((c)>='0'&&(c)<='9')||((c)>='a'&&(c)<='f')||((c)>='A'&&(c)<='F'))
//#define GZIP_DECODE_TEMP_SIZE 0 //(256 * 1024)

#define MMIBROWSER_ICO_STRING                  "/favicon.ico"

#if (defined BROWSER_GZIP_SUPPORT) && (defined GZIP_DEC_SUPPORT_GZIP_SW)
typedef struct 
{   
    //uint8   *data_ptr;
    uint32  data_len;
    uint32  readed;
}GZIP_READ_DATA_T;

typedef struct 
{   
    GZIP_READ_DATA_T   read_data;
    SFS_HANDLE    src_ffs_handler;
    SFS_HANDLE    dest_ffs_handler;
    //uint8   *decode_temp_ptr;
}GZIP_DECODE_INFO_T;

LOCAL GZIP_DECODE_INFO_T    s_gzip_decode_info = {0};

LOCAL int BRWUtil_DecodeGzipRead(char *buf,uint32 size);//return -1 when read fail!
LOCAL void BRWUtil_DecodeGzipWrite(void *buf,uint32 cnt);
#endif


LOCAL uint8 ctoi(uint8 c);
/************************************************************************/
/*           队列操作函数                                               */
/************************************************************************/
/*
desc:init a BRW_LIST_T struct
param:list_ptr  pointer of the list
*/
PUBLIC void BRWList_Construct(BRW_LIST_T  *list_ptr)
{
    if(PNULL == list_ptr)
    {
        //SCI_TRACE_LOW:"BRWList_Construct PNULL == list_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_53_112_2_18_2_7_30_836,(uint8*)"");
        return;
    }
    BRW_Memset(list_ptr,0,sizeof(BRW_LIST_T));
}
/*
desc:free mem of BRW_LIST_T struct
param:list_ptr pointer of list_ptr struct
      func     callback function for free member object of BRW_LIST_T
*/
PUBLIC void BRWList_Destruct(BRW_LIST_T   *list_ptr, BRW_LIST_FREE_F func)
{
    BRW_LIST_T *current_ptr = PNULL;
    BRW_LIST_T *next_ptr = list_ptr;
    
    if (PNULL == list_ptr)
    {
        BRWDebug_PrintWarning("BRWList_Destruct:list_ptr PNULL");
        return;
    }
    
    next_ptr = list_ptr->next;
    current_ptr = next_ptr;
    while (current_ptr != PNULL) 
    {
        next_ptr = current_ptr->next;
        if(func) 
        {
            func(current_ptr->object);
        }
        BRWMem_Free(current_ptr);
        current_ptr = next_ptr;
    }
    //reset head of the list
    BRWList_Construct(list_ptr);
}
/*
desc:alloc a BRW_LIST_T
return:the pointer of BRW_LIST_T
*/
PUBLIC BRW_LIST_T* BRWList_New(void)
{
    BRW_LIST_T  *new_ptr = PNULL;
    
    new_ptr = (BRW_LIST_T*) BRWMem_Alloc(sizeof(BRW_LIST_T));
    
    if(new_ptr != PNULL)
    {
        BRW_Memset(new_ptr, 0, sizeof(BRW_LIST_T));
    }
    else
    {
        BRWDebug_PrintWarning("BRWList_New:malloc failed");
    }
    return new_ptr;
}
/*
desc:free BRW_LIST_T pointer
param:list_ptr pointer of BRW_LIST_T struct
      func  callback function for free member object of BRW_LIST_T struct
*/
PUBLIC void BRWList_Delete(BRW_LIST_T * list_ptr, BRW_LIST_FREE_F func)
{
    if (list_ptr != PNULL) 
    {
        BRWList_Destruct(list_ptr, func);
        BRWMem_Free(list_ptr);
    }    
    else
    {
        BRWDebug_PrintWarning("BRWList_Delete:list_ptr PNULL");
    }
}
/*
desc:remove object form list
parm:list_ptr    pointer of BRW_LIST_T struct
     position    position of object want to remove
return:the appointed object
caution:the mem of object havn't been freed
*/
PUBLIC void * BRWList_RemoveObjectAt(BRW_LIST_T* list_ptr, uint32 position)
{
    void    *object_ptr = PNULL;
    BRW_LIST_T  *temp_list_ptr = PNULL;

    if (list_ptr != PNULL) 
    {
        BRW_LIST_T *prev_node_ptr = list_ptr;
        temp_list_ptr = list_ptr->next;
        while (temp_list_ptr != PNULL) 
        {
            if (0 == position) 
            {
                prev_node_ptr->next = temp_list_ptr->next;
                object_ptr =  temp_list_ptr->object;
                BRWMem_Free(temp_list_ptr);
                temp_list_ptr = PNULL;
                return object_ptr;
            }
            prev_node_ptr = temp_list_ptr;
            position--;
            temp_list_ptr=temp_list_ptr->next;
        }
    }    
    else
    {
        BRWDebug_PrintWarning("BRWList_RemoveObjectAt:list_ptr PNULL");
    }

    return PNULL;  /* Reached the end of the list */
}
/*
desc:insert a object at the head of list
param: list_ptr    pointer of the list
       new_object  new object want to insert
return:TRUE if SUCCESS,else FALSE
*/
PUBLIC BOOLEAN BRWList_InsertObjectAtHead(BRW_LIST_T * list_ptr, void * new_object)
{
    BRW_LIST_T *new_node_ptr = PNULL;
    
    if (list_ptr != PNULL) 
    {
        new_node_ptr = BRWList_New();
        if (PNULL != new_node_ptr)
        {
            new_node_ptr->object = new_object;
            new_node_ptr->next = list_ptr->next;
            list_ptr->next = new_node_ptr;
            return TRUE;
        }
        else
        {
            BRWDebug_PrintWarning("BRWList_InsertObjectAtHead,malloc failed");
        }
    }
    else
    {
        BRWDebug_PrintWarning("BRWList_InsertObjectAtHead:list_ptr PNULL");
    }
    return FALSE;
}
/*
desc:remove an object from list
param:list_prt  the pointer of the BRW_LIST_T
      new_obj   new object pointer want to remove
return:TRUE if SUCCESS,else FALSE
caution:havn't free object_ptr yet
*/
PUBLIC BOOLEAN BRWList_RemoveObject(BRW_LIST_T* list_ptr,void* object_ptr)
{
    BRW_LIST_T  *temp_list_ptr = PNULL;
    BRW_LIST_T  *prev_node_ptr = PNULL;
    
    if ((PNULL == list_ptr) || (PNULL == object_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BRWList_RemoveObject,list_ptr=%x,object_ptr=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_203_112_2_18_2_7_30_837,(uint8*)"dd",list_ptr,object_ptr);
        return FALSE;
    }
    
    temp_list_ptr = list_ptr->next;
    prev_node_ptr = list_ptr;
    while (temp_list_ptr != PNULL)
    {
        if(temp_list_ptr->object == object_ptr)
        {
            prev_node_ptr->next = temp_list_ptr->next;
            BRWMem_Free(temp_list_ptr);
            temp_list_ptr = PNULL;
            return TRUE;
        }
        prev_node_ptr = temp_list_ptr;
        temp_list_ptr = temp_list_ptr->next;
    }
    
    BRWDebug_PrintWarning("BRWList_RemoveObject, not find object and return FALSE");
    return FALSE;
}
/*
desc: append object to list
param:list_prt  the pointer of the BRW_LIST_T
      new_obj   new object pointer want to append
return:TRUE if SUCCESS,else FALSE
*/
PUBLIC BOOLEAN BRWList_AppendObject(BRW_LIST_T * list_ptr, void * new_obj)
{
    if (list_ptr != PNULL) 
    {
        while (list_ptr->next != PNULL)
        {
            list_ptr = list_ptr->next;
        }            
        return BRWList_InsertObjectAtHead(list_ptr, new_obj);
    }
    return FALSE;
}
/*
desc:get count of the list
param:list_ptr  pointer of the list
return:count of the list
*/
PUBLIC uint32 BRWList_GetListCount(BRW_LIST_T * list_ptr)
{
    uint32 count = 0;
    if(list_ptr != PNULL)
    {
        BRW_LIST_T  *temp_list_ptr = list_ptr->next;
        while(temp_list_ptr != PNULL)
        {
            count++;
            temp_list_ptr = temp_list_ptr->next;
        }
    }
    return count;
}
/*
desc:get object of appointed position
param:list_ptr  pointer of the list
      position  the appointed position
return:the object pointer
*/
PUBLIC void* BRWList_GetObjectByPos(BRW_LIST_T * list_ptr, uint32 position)
{
    if (list_ptr != PNULL) 
    {
        BRW_LIST_T  *temp_list_ptr = list_ptr->next;
        while (temp_list_ptr != PNULL) 
        {
            if (0 == position)
            {
                return temp_list_ptr->object;
            }
            position--;
            temp_list_ptr=temp_list_ptr->next;
        }
    }
    return PNULL;
}
/*
desc:duplicate a list
param:dst_list_ptr  destination list pointer
      src_list_ptr  source list pointer
      dup           callback function for duplicate object
return:TRUE if SUCCESS,else FALSE
*/
PUBLIC BOOLEAN BRWList_Duplicate(BRW_LIST_T* dst_list_ptr,BRW_LIST_T *src_list_ptr, BRW_LIST_DUP_F dup)
{
    void *object_ptr = PNULL;
    BRW_LIST_T  *temp_list_ptr = PNULL;
    
    if ((PNULL == dst_list_ptr) || (PNULL == src_list_ptr))
    {
        BRWDebug_PrintWarning("BRWList_Duplicate:invalid param,dst_list_ptr=%x",dst_list_ptr);
        return FALSE;
    }
    temp_list_ptr = src_list_ptr->next;
    while (temp_list_ptr != PNULL) 
    {
        if( temp_list_ptr->object != PNULL) 
        {
            object_ptr = dup(temp_list_ptr->object);
            if( object_ptr != PNULL) 
            {
                BRWList_AppendObject(dst_list_ptr, object_ptr);
            } 
            else 
            {
                return FALSE;
            }
        }
        temp_list_ptr=temp_list_ptr->next;
    }
    return TRUE;
}
/*
desc:search object from list,by cmp function
param:list_prt      pointer of list
      cmp           callback function for object compare
      object_prt    reference object
return:the first object found
*/
PUBLIC void * BRWList_SearchObject(BRW_LIST_T* list_ptr, BRW_LIST_CMP_F cmp, void* object_ptr)
{
    BRW_LIST_T  * temp_list_ptr=PNULL;
    
    if((PNULL == list_ptr) || (PNULL == object_ptr) || (PNULL == cmp))
    {
        //BRWDebug_Printf:"[BRW]BRWList_SearchObject list_ptr=%x,object_prt=%x,cmp=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_329_112_2_18_2_7_30_838,(uint8*)"ddd",list_ptr,object_ptr,cmp);
        return PNULL;
    }
    temp_list_ptr= list_ptr->next;
    while (temp_list_ptr != PNULL)
    {
        if(cmp(temp_list_ptr->object, object_ptr))
        {
            return temp_list_ptr->object;
        }
        temp_list_ptr = temp_list_ptr->next;
    }
    return PNULL;
}
/*
 *	desc:move object in list to head position
 *  param: list_ptr  list pointer
 *         obj_ptr   object pointer
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BRWList_MoveObjToHead(BRW_LIST_T* list_ptr,void* obj_ptr)
{
    BRW_LIST_T  *temp_list_ptr=PNULL;
    BRW_LIST_T  *prev_list_ptr=PNULL;
    if ((PNULL == list_ptr) || (PNULL == obj_ptr))
    {
        //BRWDebug_Printf:"[BRW]BRWList_SearchObject list_ptr=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_354_112_2_18_2_7_30_839,(uint8*)"d",list_ptr);
        return FALSE;
    }
    
    temp_list_ptr = list_ptr->next;
    prev_list_ptr = list_ptr;
    while(temp_list_ptr != PNULL)
    {
        if(list_ptr->object == obj_ptr)
        {
            prev_list_ptr->next = temp_list_ptr->next;
            temp_list_ptr->next = list_ptr->next;
            list_ptr->next = temp_list_ptr;
            return TRUE;
        }
        prev_list_ptr = temp_list_ptr;
        temp_list_ptr = temp_list_ptr->next;
    }
    return FALSE;
}
/*****************************************************************************/
//  Description : char to integer
//  Global resource dependence : 
//  Author:sally.he
//  Note: 
/*****************************************************************************/
LOCAL uint8 ctoi(uint8 c)
{
    if ('0' <= c && c <= '9')
    {
        return c - '0';
    }
    else if ('A' <= c && c <= 'F')
    {
        return c - 'A' + 0x0A;
    }
    else if ('a' <= c && c <= 'f')
    {
        return c - 'a' + 0x0A;
    }
    else
    {
        return 0xFF;
    }
}
/*
 *	desc:从url字串总提取资源名字
 *  return:如果提取到资源名字，将其返回，否则返回PNULL
 *  note:在函数总会分配内存，以存储资源名字
 */
PUBLIC uint8*   BRWUti_GetResNameFrUrl(uint8*   url_ptr)
{
    uint32  url_len = 0;
    uint8   *rtn_url_ptr = PNULL;
    
    if (url_ptr != PNULL)
    {
        int32  i=0;
        int32  j=0;
        uint8   *temp=PNULL;
        uint8   *query_str_ptr=PNULL;
        uint8   *fragment_ptr=PNULL;
        uint8   *tail_ptr=PNULL;
        uint32  len=0;
        url_len = BRW_Strlen(url_ptr); 
        for (i=url_len-1; i>=0; i--)
        {
            temp = url_ptr+i;
            if (*temp=='/')
            {
                if((i>0) && (*(temp-1)=='/'))
                {
                    return PNULL;
                }
                break;
            }
        }
        if ((i<0) || (i==(int32)(url_len-1)))
        {
            return PNULL;
        }
        i++;//'/'
        temp=url_ptr+i;
        query_str_ptr=BRW_Strchr(temp,'?');
        fragment_ptr=BRW_Strchr(temp,'#');
        if(query_str_ptr!=PNULL)
        {
            if(fragment_ptr!=PNULL)
            {
                tail_ptr = (query_str_ptr>fragment_ptr)?fragment_ptr:query_str_ptr;
            }
            else
            {
                tail_ptr = query_str_ptr;
            }
        }
        else
        {
            tail_ptr = fragment_ptr;
        }
        
        if(tail_ptr != PNULL)
        {
            len=url_len-(uint32)i-BRW_Strlen(tail_ptr);
        }
        else
        {
            len=url_len-(uint32)i;
        }
        if(0 == len)
        {
            return PNULL;
        }
        rtn_url_ptr = BRWMem_Alloc(len+1);
        if(PNULL == rtn_url_ptr)
        {
            return PNULL;
        }

        //transfer "%XX" to char
        BRW_Memset(rtn_url_ptr, 0x00, (len + 1));
        j = 0;
        for (i = 0; i < (int)len; i++, j++)
        {   //'%' is equal to 0x25
            if (0x25 == *(temp + i) && (i + 2 < (int)len)
                && isHexDigit(*(temp + i + 1)) && isHexDigit(*(temp + i + 2)))
            {
                uint8 temp_conv_ascii = ctoi(*(temp + i + 1)) * 16 + ctoi(*(temp + i + 2));

                if (temp_conv_ascii < 0x80) //ASCII
                {
                    *(rtn_url_ptr + j) = (uint8)temp_conv_ascii;
                    i += 2;
                }
                else
                {
                    *(rtn_url_ptr + j) = *(temp + i);
                }
            }
            else
            {
                *(rtn_url_ptr + j) = *(temp + i);
            }
        }
        
        return rtn_url_ptr;
    }
    return PNULL;    
}

PUBLIC char* BRWUtil_GetUrlScheme(const char *url_ptr)
{
    const char *ch = PNULL;
    char *scheme_ptr = PNULL;
    BOOLEAN is_found = FALSE;

    if (PNULL == url_ptr)
    {
        return PNULL;
    }
    
    ch = url_ptr;
    while(*ch != 0)
    {
        if(*ch==':' && *(ch+1)=='/' && *(ch+2)=='/')
        {
            is_found = TRUE;
            break;
        }
        else
        {
            ch++;
        }
    }
    if(is_found)
    {
        scheme_ptr = BRWMem_Alloc(ch-url_ptr+1);
        if(PNULL != scheme_ptr)
        {
            BRW_Memset(scheme_ptr,0,ch-url_ptr+1);
            BRW_Memcpy(scheme_ptr,url_ptr,ch-url_ptr);
        }
    }
    return scheme_ptr;
}

/*****************************************************************************/
//  Description : get the ico url
//  Global resource dependence : 
//  Author: fen.xie
//  Note:need to FREE the return value
/*****************************************************************************/
PUBLIC char* BRWUtil_GetIcoUrl(const char *url_ptr)
{
    const char *ch = PNULL;
    char *ico_url_ptr = PNULL;
    uint16  ico_url_len = 0;

    if (PNULL == url_ptr)
    {
        return PNULL;
    }
    
    ch = url_ptr;
    while(*ch != 0)
    {
        if (*ch=='/')
        {
            break;
        }

        if(*ch==':' && *(ch+1)=='/' && *(ch+2)=='/')
        {
            ch+=3;
        }
        else
        {
            ch++;
        }
    }

    ico_url_len = (uint16)(ch - url_ptr) + strlen(MMIBROWSER_ICO_STRING);
    ico_url_ptr = BRWMem_Alloc(ico_url_len + 1);
    if (ico_url_ptr != PNULL)
    {
        BRW_Memset(ico_url_ptr, 0x00, (ico_url_len + 1));
        BRW_Memcpy(ico_url_ptr, url_ptr, (ch - url_ptr));
        strncat(ico_url_ptr, MMIBROWSER_ICO_STRING, SCI_STRLEN(MMIBROWSER_ICO_STRING));
    }

    return ico_url_ptr;
} 

PUBLIC void BrwUrlRemoveFragment(
    const uint8 *src_url_ptr, 
    uint8   *dest_url_ptr,//[in/out]
    uint16  dest_url_len
    )
{
    uint8 *start = PNULL;
    uint16  url_len = 0;

    if ((PNULL == src_url_ptr) || (PNULL == dest_url_ptr))
    {
        return;
    }
    
    start = BRW_Strchr(src_url_ptr, '#');
    
    if (PNULL != start)//with fragment
    {
        url_len = start - src_url_ptr;
    }
    else
    {
        url_len = BRW_Strlen(src_url_ptr);
    }
    url_len = MIN(url_len, dest_url_len);
    BRW_Strncpy(dest_url_ptr, src_url_ptr, url_len);
}

LOCAL const uint8 *MovetoFragmentStart(const uint8 *url)
{
    const uint8 *start = PNULL;

    if (PNULL == url)
    {
        return PNULL;
    }
    
    start = BRW_Strchr(url, '#');
    
    if (PNULL == start) 
    {
        start = url+BRW_Strlen(url);
    }
    return start;
}
PUBLIC BOOLEAN BrwIsUrlIsEquExceptFragment(const uint8 *url1_ptr, const uint8 *url2_ptr)
{
    uint32  len1 = 0;
    uint32  len2 = 0;
    
    if((PNULL == url1_ptr) || (PNULL == url2_ptr))
    {
        return FALSE;
    }
    
    if(0 == BRW_Stricmp(url1_ptr,url2_ptr))
    {
        if(PNULL == BRW_Strchr(url1_ptr, '#')) //Jump in the same web page 
        {
            return FALSE;          
        }
        else
        {
            return TRUE;
        }        
    }
    len1 = MovetoFragmentStart(url1_ptr) - url1_ptr;
    if( *(url1_ptr + len1 - 1)=='/' )
    {
        len1--;
    }
    len2 = MovetoFragmentStart(url2_ptr) - url2_ptr;
    if( *(url2_ptr + len2 - 1)=='/' )
    {
        len2--;
    }

    if(len1 != len2)
    {
        return FALSE;
    }
    return (0 == BRW_Strnicmp(url1_ptr, url2_ptr, len1));
}

PUBLIC BOOLEAN BRWUtil_GetBrwRootPath(uint16 *path_ptr, uint32 path_max_len)
{
    wchar   *device_name_ptr=PNULL;
    uint16 device_name_len=0;
	
    if ((PNULL == path_ptr) || (path_max_len > BRW_FULL_PATH_MAX_LEN))
    {
        return FALSE;
    }
    BRW_Memset(path_ptr, 0, sizeof(uint16)*path_max_len);

    device_name_ptr = BRWPL_GetBrwSysDevName(&device_name_len);
    if(PNULL == device_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRWUtil_GetDefaultPath:device null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_680_112_2_18_2_7_31_840,(uint8*)"");
        return FALSE;
    }
	
    BRW_WstrCat(path_ptr,device_name_ptr);
    BRWPL_WstrStrnCatWithAsciiStr(path_ptr,FILE_DEVICE_SLASH,path_max_len);
    BRWPL_WstrStrnCatWithAsciiStr(path_ptr,BRW_ROOT_DIR,path_max_len);
    return TRUE;
}
/*
 *	根据字符串获得对应的charset类型
 */
PUBLIC BRW_CHARSET_E BRWUtil_GetCharset(const uint8* charset_ptr)
{
    if(BRW_Stricmp(charset_ptr,(const uint8*)"UTF-8")==0)
    {
        return BRW_CHARSET_UTF8;
    }
    else if(BRW_Stricmp(charset_ptr,(const uint8*)"GB2312")==0)
    {
        return BRW_CHARSET_GB2312;
    }
    return BRW_CHARSET_UNKNOWN;
}
/*
 *	desc:读取browser系统目录下的文件
 *  param:file_name_ptr  asc格式文件名,'\0'结束
 *        buf_ptr   buffer to store data
 *        buf_max_len  max length of buffer
 *        read_len_ptr[output]  real length readed 
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWUtil_ReadFileSyn(
                                const uint8* file_name_ptr,
                                const uint8 *dir_name_ptr,
                                uint8* buf_ptr,
                                uint32 buf_max_len,
                                uint32* read_len_ptr
                                )
{
    wchar full_path[BRW_FULL_PATH_MAX_LEN]={0};
    uint16 full_path_len=0;//length in wchar
    wchar   *device_name_ptr=PNULL;
    uint16 device_name_len=0;
    BOOLEAN result=TRUE;
    uint32 file_name_len = 0;

    file_name_len = (uint16)BRW_Strlen(file_name_ptr);
    if ((0 == file_name_len) || (PNULL == buf_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BRWUtil_ReadFileSyn:file_name_ptr=0x%x,buf_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_728_112_2_18_2_7_31_841,(uint8*)"dd",file_name_ptr,buf_ptr);
        return FALSE;
    }
    device_name_ptr = BRWPL_GetBrwSysDevName(&device_name_len);
    if(PNULL == device_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRWUtil_ReadFileSyn:device null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_734_112_2_18_2_7_31_842,(uint8*)"");
        return FALSE;
    }
    full_path_len = BRW_Strlen(file_name_ptr)+device_name_len+BRW_Strlen((const uint8*)FILE_DEVICE_SLASH);
    if(PNULL != dir_name_ptr)
    {
        full_path_len += BRW_Strlen(dir_name_ptr)+BRW_Strlen((const uint8*)BRW_FILE_SLASH);
    }
    if(full_path_len>=BRW_FULL_PATH_MAX_LEN)
    {
        BRWDebug_PrintWarning("BRWUtil_ReadFileSyn:full_path_len too long");
        return FALSE;		
    }
    BRW_WstrCat(full_path,device_name_ptr);
    BRWPL_WstrStrnCatWithAsciiStr(full_path,FILE_DEVICE_SLASH,BRW_FULL_PATH_MAX_LEN);
    if(PNULL != dir_name_ptr)
    {
        BRWPL_WstrStrnCatWithAsciiStr(full_path,(const char*)dir_name_ptr,BRW_FULL_PATH_MAX_LEN);
        BRWPL_WstrStrnCatWithAsciiStr(full_path,BRW_FILE_SLASH,BRW_FULL_PATH_MAX_LEN);
    }
    BRWPL_WstrStrnCatWithAsciiStr(full_path,(const char*)file_name_ptr,BRW_FULL_PATH_MAX_LEN);

    result = BRWPL_ReadFileSyn(full_path,full_path_len,buf_ptr,buf_max_len,read_len_ptr);
    return result;
}
/*
 *	desc:写browser系统目录下的文件
 *  param:file_name_ptr  asc格式文件名,'\0'结束
 *        full_path_len  length of path
 *        buf_ptr   address of data to write
 *        to_write_len  the length of data want to write
 *  return:TRUE if exist else FALSE
 */

PUBLIC BOOLEAN BRWUtil_WriteFileSyn(
									const uint8* file_name_ptr,
									const uint8 *dir_name_ptr,
									const uint8* buf_ptr,
									uint32 to_write_len
									)
{
    wchar full_path[BRW_FULL_PATH_MAX_LEN]={0};
    uint32 full_path_len=0;//length in wchar
    wchar   *device_name_ptr=PNULL;
    uint16 device_name_len=0;
    uint32 file_name_len = 0;

    file_name_len=(uint16)BRW_Strlen(file_name_ptr);
    if ((0 == file_name_len) || (PNULL == buf_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BRWUtil_ReadFileSyn:file_name_ptr=0x%x,buf_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_784_112_2_18_2_7_31_843,(uint8*)"dd",file_name_ptr,buf_ptr);
        return FALSE;
    }
    device_name_ptr=BRWPL_GetBrwSysDevName(&device_name_len);
    if(PNULL==device_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRWUtil_ReadFileSyn:device null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_790_112_2_18_2_7_31_844,(uint8*)"");
        return FALSE;
    }
    full_path_len = BRW_Strlen(file_name_ptr)+device_name_len+BRW_Strlen((const uint8*)FILE_DEVICE_SLASH);
    if(PNULL!=dir_name_ptr)
    {
        full_path_len += BRW_Strlen(dir_name_ptr)+BRW_Strlen((const uint8*)BRW_FILE_SLASH);
    }
    if(full_path_len>=BRW_FULL_PATH_MAX_LEN)
    {
        BRWDebug_PrintWarning("BRWUtil_ReadFileSyn:full_path_len too long");
        return FALSE;		
    }
    BRW_WstrCat(full_path,device_name_ptr);
    BRWPL_WstrStrnCatWithAsciiStr(full_path,FILE_DEVICE_SLASH,BRW_FULL_PATH_MAX_LEN);
    if(PNULL !=dir_name_ptr)
    {
        BRWPL_WstrStrnCatWithAsciiStr(full_path,(const char*)dir_name_ptr,BRW_FULL_PATH_MAX_LEN);
        BRWPL_WstrStrnCatWithAsciiStr(full_path,BRW_FILE_SLASH,BRW_FULL_PATH_MAX_LEN);
    }
    BRWPL_WstrStrnCatWithAsciiStr(full_path,(const char*)file_name_ptr,BRW_FULL_PATH_MAX_LEN);

    return (SFS_ERROR_NONE == BRWPL_WriteFileSyn(full_path,full_path_len,buf_ptr,to_write_len));
}
/*
 *	desc:获得browser目录下某个文件的长度
 *  param:file_name_ptr  asc格式文件名,'\0'结束 
 *        size_ptr[output]   size of file
 *  return:TRUE if SUCCESS else FALSE
 */
PUBLIC BOOLEAN BRWUtil_GetFileSize(const uint8 *file_name_ptr,uint32* size_ptr)
{
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN]={0};
    wchar unicode_dir_name[BRW_ROOT_DIR_LEN+1]={0};
    uint16 full_path_len=0;
    wchar   *unicode_file_name_ptr=PNULL;
    uint16 unicode_file_name_len=0;//字节个数
    wchar   *device_name_ptr=PNULL;
    uint16 device_name_len=0;
    uint16 file_name_len=0;
    BOOLEAN result=TRUE;
    
    file_name_len=(uint16)BRW_Strlen(file_name_ptr);
    if ((0 == file_name_len) || (PNULL == size_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BRWUtil_GetFileSize:file_name_ptr=0x%x,size_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_835_112_2_18_2_7_31_845,(uint8*)"dd",file_name_ptr,size_ptr);
        return FALSE;
    }
    *size_ptr=0;
    device_name_ptr=BRWPL_GetBrwSysDevName(&device_name_len);
    if(PNULL==device_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRWUtil_GetFileSize:device null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_842_112_2_18_2_7_31_846,(uint8*)"");
        return FALSE;
    }
    unicode_file_name_ptr=BRWMem_Alloc(sizeof(wchar)*(file_name_len+1));
    if(PNULL == unicode_file_name_ptr)
    {
        BRWDebug_PrintWarning("BRWUtil_GetFileSize:malloc unicode_file_name_ptr failed");
        return FALSE;
    }
    BRW_Memset(unicode_file_name_ptr,0,sizeof(wchar)*(file_name_len+1));
    unicode_file_name_len=file_name_len;
    BRWPL_StrToWstr((const char*)file_name_ptr,unicode_file_name_ptr);
    BRWPL_StrToWstr(BRW_ROOT_DIR,unicode_dir_name);
    
    result = BRWPL_CombinePath(device_name_ptr,device_name_len,
                            unicode_dir_name,BRW_ROOT_DIR_LEN,
                            unicode_file_name_ptr,unicode_file_name_len,
                            full_path,&full_path_len);    
    BRWMem_Free(unicode_file_name_ptr);
    if(result)
    {
        result = BRWPL_GetFileSize(full_path,full_path_len,size_ptr);
    }
    return result;
}
/*
 *	desc:删除browser目录下的某个文件(同步)
 *  param:file_name_ptr  asc格式文件名,'\0'结束
 *  return:TRUE if success else FALSE
 */
PUBLIC BOOLEAN BRWUtil_DelFileSyn(const uint8 *file_name_ptr)
{
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN]={0};
    wchar unicode_dir_name[BRW_ROOT_DIR_LEN+1]={0};
    uint16 full_path_len=0;
    wchar* unicode_file_name_ptr=PNULL;
    uint16 unicode_file_name_len=0;//字节个数
    wchar* device_name_ptr=PNULL;
    uint16 device_name_len=0;
    uint16 file_name_len=0;
    BOOLEAN result=TRUE;

    file_name_len=(uint16)BRW_Strlen(file_name_ptr);
    if(0 == file_name_len)
    {
        //BRWDebug_Printf:"[BRW]W BRWUtil_DelFileSyn:file_name_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_886_112_2_18_2_7_31_847,(uint8*)"d",file_name_ptr);
        return FALSE;
    }
    device_name_ptr=BRWPL_GetBrwSysDevName(&device_name_len);
    if(PNULL==device_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRWUtil_DelFileSyn:device null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_892_112_2_18_2_7_31_848,(uint8*)"");
        return FALSE;
    }
    unicode_file_name_ptr=BRWMem_Alloc(sizeof(wchar)*(file_name_len+1));
    if(PNULL == unicode_file_name_ptr)
    {
        BRWDebug_PrintWarning("BRWUtil_DelFileSyn:malloc unicode_file_name_ptr failed");
        return FALSE;
    }
    BRW_Memset(unicode_file_name_ptr,0,sizeof(wchar)*(file_name_len+1));
    unicode_file_name_len=file_name_len;
    BRWPL_StrToWstr((const char*)file_name_ptr,unicode_file_name_ptr);
    BRWPL_StrToWstr(BRW_ROOT_DIR,unicode_dir_name);
    
    result = BRWPL_CombinePath(device_name_ptr,device_name_len,
                            unicode_dir_name,BRW_ROOT_DIR_LEN,
                            unicode_file_name_ptr,unicode_file_name_len,
                            full_path,&full_path_len);    
    BRWMem_Free(unicode_file_name_ptr);
    if(result)
    {
        result=BRWPL_DelFileSyn(full_path,full_path_len);
    }
    return result;
}
/*
 *	desc:创建browser系统目录(如果已经存在,则返回TRUE)
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BRWUtil_CreateDir(void)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN]={0};
    wchar   unicode_dir_name[BRW_ROOT_DIR_LEN+1]={0};
    uint16  full_path_len=0;  
    wchar   *device_name_ptr=PNULL;
    uint16  device_name_len=0;
    BOOLEAN result=FALSE;

    device_name_ptr=BRWPL_GetBrwSysDevName(&device_name_len);
    if(PNULL==device_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRWUtil_CreateDir:device null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_932_112_2_18_2_7_31_849,(uint8*)"");
        return FALSE;
    }
    BRWPL_StrToWstr(BRW_ROOT_DIR,unicode_dir_name);
    result = BRWPL_CombinePath(device_name_ptr,device_name_len,
                            unicode_dir_name,BRW_ROOT_DIR_LEN,
                            PNULL,PNULL,full_path,&full_path_len); 
    if(result)
    {
       if(!BRWPL_IsFolderExist(full_path,full_path_len))
       {
           result=BRWPL_CreateDir(full_path,full_path_len);
       }
       else
       {
           result=TRUE;
       }
    }
    return result;
}

/*
desc:decode the gzip buffer
param:
return:BRW_ERROR_E:the decode result
caution:*dest_buf_ptr need to free outside
*/
PUBLIC BRW_ERROR_E BRWUtil_DecodeGzip(
                uint8 *src_buf_ptr, //[IN]
                uint32    src_len,//[IN]
                uint8 **dest_buf_ptr,//[IN/OUT]*dest_buf_ptr need to free outside
                uint32    *dest_len_ptr,//[IN/OUT]
                uint32  max_size
                )
{
#if (defined BROWSER_GZIP_SUPPORT) && (defined GZIP_DEC_SUPPORT_GZIP_SW)
    IMGGZIP_DECODE_INPUT_PARAM_T	decode_input = {0};
    IMGGZIP_DECODE_OUTPUT_T			decode_output = {0};
    IMGGZIP_DECODE_SRC_PARAM_T		src_param = {0};
    IMGGZIP_DECODE_INFO_PARAM_T		dec_info = {0};
    uint32  i = 0;
    BRW_ERROR_E brw_result = BRW_ERR_NO_ERROR;
    IMGGZIP_DECODE_RESULT_E decode_result = IMGGZIP_DECODE_SUCCESS;
        
    if ((PNULL == src_buf_ptr) ||(0 == src_len) ||(PNULL == dest_buf_ptr) ||(PNULL == dest_len_ptr))
    {
        return BRW_ERR_UNKNOWN;
    }

    do
    {
        //BRWDebug_Printf:"[BRW]BRWUtil_DecodeGzip src_len =0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_978_112_2_18_2_7_32_850,(uint8*)"d", src_len);

        src_param.src_buf_ptr = src_buf_ptr;

        //MS00217600:避免http buf的尾部带有额外字符
        for (i = src_len -1; i > 0; i--)
        {
            if ((0x0D != src_buf_ptr[i]) && (0x0A != src_buf_ptr[i]))
            {
                break;
            }
            src_len--;
        }
        
        src_param.src_size = src_len;
        IMGGZIP_Get_Info(&src_param, &dec_info);

        //BRWDebug_Printf:"[BRW]BRWUtil_DecodeGzip src_len =0x%x, dec_info.src_img_size=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_995_112_2_18_2_7_32_851,(uint8*)"dd", src_len, dec_info.src_img_size);

        if (0 == dec_info.src_img_size)
        {
            brw_result = BRW_ERR_PAGE_UNSUPPORT;
            break;
        }
        
        //protect
        if (dec_info.src_img_size > max_size)
        {
            brw_result = BRW_ERR_PAGE_TOO_BIG;
            break;
        }
        else
        {
             *dest_len_ptr = dec_info.src_img_size;
        }

        *dest_buf_ptr = BRWMem_Alloc(*dest_len_ptr);
        if (PNULL == *dest_buf_ptr )
        {
            BRWDebug_PrintWarning("BRWUtil_DecodeGzip ALLOC FAIL");
            brw_result = BRW_ERR_NO_MEM;
            break;
        }
        BRW_Memset(*dest_buf_ptr , 0x00, *dest_len_ptr);

        if(0 != gzip_initialize(0, 0))
        {	
            BRWDebug_PrintWarning("BRWUtil_DecodeGzip:gzip_initialize fail");
            brw_result = BRW_ERR_NO_MEM;
            break;
        }

        decode_input.dst_buf_ptr = *dest_buf_ptr;
        decode_input.dst_buf_size = *dest_len_ptr;
        decode_input.src_buf_ptr = src_buf_ptr;
        decode_input.src_size = src_len;
        decode_result = IMGGZIP_Decode(&decode_input,&decode_output);
        
        BRWDebug_Printf("[BRW]BRWUtil_DecodeGzip:decode_result = %d", decode_result);
        if (IMGGZIP_DECODE_SUCCESS != decode_result)
        {
            BRWDebug_PrintWarning("BRWUtil_DecodeGzip:IMGGZIP_Decode FAIL");
            brw_result = BRW_ERR_UNKNOWN;
            break;
        }
    }while(0);

    if (BRW_ERR_NO_ERROR != brw_result)//error, free the dest_buf_ptr
    {
        BRWMem_Free(*dest_buf_ptr);
        *dest_buf_ptr = PNULL;
    }
    
    gzip_release();

    return brw_result;
#else
    return BRW_ERR_PAGE_UNSUPPORT;
#endif
}

/*
desc:decode the gzip file
param:
return:BRW_ERROR_E:the decode result
caution:
*/
PUBLIC BRW_ERROR_E BRWUtil_DecodeGzipByFile(
                uint16 *src_file_name_ptr,
                uint16 *dest_file_name_ptr
                )
{
#if (defined BROWSER_GZIP_SUPPORT) && (defined GZIP_DEC_SUPPORT_GZIP_SW)
    IMGGZIP_DECODE_INPUT_PARAM_T    decode_input = {0};
    IMGGZIP_DECODE_OUTPUT_T decode_output = {0};
    IMGGZIP_DECODE_RESULT_E decode_result = IMGGZIP_DECODE_SUCCESS;
    SFS_HANDLE    src_ffs_handler = 0;
    BRW_ERROR_E brw_result = BRW_ERR_NO_ERROR;
        
    do
    {
        SCI_MEMSET(&s_gzip_decode_info, 0x00, sizeof(s_gzip_decode_info));

        if ((PNULL ==dest_file_name_ptr) ||(0 == src_file_name_ptr))
        {
            //BRWDebug_Printf:"[BRW]W BRWUtil_DecodeGzipByFile:dest_file_name_ptr=0x%x;src_file_name_ptr=0x%x;"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_1083_112_2_18_2_7_32_852,(uint8*)"dd", dest_file_name_ptr, src_file_name_ptr);
            brw_result= BRW_ERR_UNKNOWN;
            break;
        }

        src_ffs_handler = SFS_CreateFile(src_file_name_ptr, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read
        if(0 == src_ffs_handler)
        {
            brw_result= BRW_ERR_UNKNOWN;
            break;
        }
        if (SFS_NO_ERROR != SFS_GetFileSize(src_ffs_handler, &decode_input.src_size)
            || (0 == decode_input.src_size))
        {
            brw_result= BRW_ERR_UNKNOWN;
            break;
        }
        //BRWDebug_Printf:"BRWUtil_DecodeGzipByFile:decode_input.src_size = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_1100_112_2_18_2_7_32_853,(uint8*)"d", decode_input.src_size);

        if (0 != gzip_initialize(0, 0))
        {
            BRWDebug_PrintWarning("BRWUtil_DecodeGzipByFile ALLOC FAIL2");
            brw_result = BRW_ERR_NO_MEM;
            break;
        }

        s_gzip_decode_info.dest_ffs_handler = SFS_CreateFile(dest_file_name_ptr, (uint32)(SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS), NULL, NULL);
        if(0 == s_gzip_decode_info.dest_ffs_handler )
        {
            brw_result= BRW_ERR_UNKNOWN;
            break;
        }
        
        //begin to decode
        s_gzip_decode_info.src_ffs_handler = src_ffs_handler;
        src_ffs_handler = 0;
        s_gzip_decode_info.read_data.data_len = decode_input.src_size;
        s_gzip_decode_info.read_data.readed = 0;

        decode_input.read_fun = BRWUtil_DecodeGzipRead;
        decode_input.write_fun = BRWUtil_DecodeGzipWrite;

        decode_result = IMGGZIP_Decode(&decode_input,&decode_output);
        //BRWDebug_Printf:"BRWUtil_DecodeGzipByFile:decode_result =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_1130_112_2_18_2_7_32_854,(uint8*)"d", decode_result);
        if(IMGGZIP_DECODE_SUCCESS == decode_result)
        {
            brw_result = BRW_ERR_NO_ERROR;
        }
        else
        {
            brw_result= BRW_ERR_UNKNOWN;
        }

    }while(0);

    //begin:reset the param after decode
    if(0 != src_ffs_handler  )
    {
        SFS_CloseFile(src_ffs_handler);
        src_ffs_handler  = 0;
    }

    gzip_release();
    
    if(0 != s_gzip_decode_info.dest_ffs_handler )
    {
        SFS_CloseFile( s_gzip_decode_info.dest_ffs_handler );
        s_gzip_decode_info.dest_ffs_handler  = 0;
    }
    if(0 != s_gzip_decode_info.src_ffs_handler  )
    {
        SFS_CloseFile(s_gzip_decode_info.src_ffs_handler);
        s_gzip_decode_info.src_ffs_handler  = 0;
    }
    //end:reset the param after decode
    
    return brw_result;
#else
    return BRW_ERR_PAGE_UNSUPPORT;
#endif
}


#if (defined BROWSER_GZIP_SUPPORT) && (defined GZIP_DEC_SUPPORT_GZIP_SW)
LOCAL int BRWUtil_DecodeGzipRead(char *buf,uint32 size)//return -1 when read fail!
{
    uint32 bytes_readed = 0;
    uint32  to_read_size = 0;
    SFS_ERROR_E	result = SFS_ERROR_NONE;

    //BRWDebug_Printf:"[BRW]BRWUtil_DecodeGzipRead:buf =0x%x, size =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_1178_112_2_18_2_7_32_855,(uint8*)"dd", buf, size);
    if ((PNULL == buf) || (0 == size)) 
    {
        return -1;
    }

    //BRWDebug_Printf:"[BRW]BRWUtil_DecodeGzipRead:src_ffs_handler =0x%x, data_len =%d, readed =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_1185_112_2_18_2_7_32_856,(uint8*)"ddd", s_gzip_decode_info.src_ffs_handler , s_gzip_decode_info.read_data.data_len, s_gzip_decode_info.read_data.readed);
    
    if (s_gzip_decode_info.read_data.data_len > s_gzip_decode_info.read_data.readed)
    {
        to_read_size = s_gzip_decode_info.read_data.data_len - s_gzip_decode_info.read_data.readed;
        
        to_read_size = MIN(to_read_size, size);
        result = SFS_ReadFile(s_gzip_decode_info.src_ffs_handler, buf, to_read_size,&bytes_readed, PNULL);
        //BRWDebug_Printf:"[BRW]BRWUtil_DecodeGzipRead:result =%d, bytes_readed =%d, to_read_size =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_1193_112_2_18_2_7_32_857,(uint8*)"ddd", result, bytes_readed, to_read_size);
        
        s_gzip_decode_info.read_data.readed += bytes_readed;
        if (SFS_ERROR_NONE == result)
        {
            return 0;        
        }
        else
        {
            return -1;        
        }
    } 
    else 
    {
        return 0;
    }
}
LOCAL void BRWUtil_DecodeGzipWrite(void *buf,uint32 cnt)
{
    uint32 bytes_written = 0;
    SFS_ERROR_E	result = SFS_ERROR_NONE;

    if(0 != s_gzip_decode_info.dest_ffs_handler )
    {
        result = SFS_WriteFile(s_gzip_decode_info.dest_ffs_handler, buf, cnt, &bytes_written, PNULL);
        //BRWDebug_Printf:"[BRW]BRWUtil_DecodeGzipWrite:result =%d, cnt =%d, bytes_written =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_UTILITY_1218_112_2_18_2_7_32_858,(uint8*)"ddd", result, cnt, bytes_written);
    }
    else
    {
        BRWDebug_PrintWarning("[BRW]BRWUtil_DecodeGzipWrite:dest_ffs_handler NULL");
    }
}

#endif
