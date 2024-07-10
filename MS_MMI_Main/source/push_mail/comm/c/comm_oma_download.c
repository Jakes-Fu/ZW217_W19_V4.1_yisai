/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_oma_download.c
 *	Author		: sunguochen    
 *	Description	: 实现OMA Download
                             因为用于音乐随身听，去掉部分tg以节省时间
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: Create 2010-03-31
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/


#include "mmi_push_mail.h"
#include "std_header.h"

#include "comm_oma_download.h"
#include "comm_interface.h"
#include "comm_platform.h"
#include "comm_memory.h"



static char oma_dd_tag[TAG_DD_TAG_NUM + 1][OMA_DOWNLOAD_TAG_LEN]={
                           "type",
				"name",                            
				"size",
				"objectURI",
				"installNotifyURI",
#ifdef OMA_DOWNLOAD_FULL_SUPPORT
				"DDVersion",
				"description",
				"nextURL",
				"vendor",
				"infoURL",
				"iconURI",
				"installParam",
#endif				
				0};



/*==========================================================
 * Function 	: oma_extract_one_element
 * Author		: sunguochen      
 * Description : 解析单个tag并保存其内容
 * Parameter	: type          parameter       Description
                      const char*                        tag_buffer,       --dd tag name
                      int                                    len_tag,            --dd tag name len
                      const char*                        ptr_dd_element, --dd tag content 
                      int                                    len_element,     --dd tag content len
                      oma_download_desc_t*  oma_dd_data    --buffer to save dd tag content  
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
static void comm_OmaExtractOneTag(const char*                        tag_buffer, 
                                                                    int                                    len_tag, 
                                                                    char*                        ptr_dd_element, 
                                                                    int                                    len_element,
                                                                    oma_download_desc_t*      oma_dd_data)
{

    int i=0;
    char* dd_element=NULL;
    char* dd_element_tmp = NULL;
    int     dd_element_len = 0;

    if(0==len_tag||0==len_element||NULL==tag_buffer||NULL==ptr_dd_element)
    {
        return;
    }

    dd_element=(char*)comm_alloc(len_element+2);

    if(NULL == dd_element)
    {
        return;
    }

    memset(dd_element, 0x0, (len_element+2));

    dd_element_tmp = ptr_dd_element;
    dd_element_len = len_element;

    //delete unwanted space
    while(0x20 == *dd_element_tmp)
    {
        //delete space
        dd_element_tmp ++;

        if(dd_element_len)
        {
            dd_element_len--;
        }
    }
    
    memcpy(dd_element, dd_element_tmp, dd_element_len);

    if(dd_element_len)
    {
        dd_element_len--;
    }

    while(0x20 == dd_element[dd_element_len])
    {
        dd_element[dd_element_len] = 0x00;

        if(dd_element_len)
        {
            dd_element_len--;
        }
        else
        {
            break;
        }
        
    }

    while(0 != oma_dd_tag[i][0])
    {

        if(!strncmp(oma_dd_tag[i], tag_buffer, len_tag))
        {
            if(TAG_TYPE == i)//tag  is type
            {

                if(oma_dd_data->dd_element[TAG_TYPE]==NULL)
                {
                    oma_dd_data->dd_element[TAG_TYPE] = dd_element;
                    break;
                }
                else
                {
                    /*do nothing*/
                    break;
                }

            }
            else if(TAG_NAME == i)//file name
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            }            
            else if(TAG_SIZE == i)//file size
            {
                oma_dd_data->dd_element[i] = dd_element;
                oma_dd_data->file_size = strtoul(dd_element, 0, OMA_MAX_FILE_SIZE);
                break;
            }
            else if(TAG_OBJECT_URI == i)//object url
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            } 
            else if(TAG_INSTALL_NOTIFY_URI == i)//report url
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            } 
#ifdef OMA_DOWNLOAD_FULL_SUPPORT            
            else if(TAG_NEXT_URL == i)//next object url now not used
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            } 
            else if(TAG_DD_VERSION == i)//dd version
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            } 
            else if(TAG_DESCRIPTION == i)//file description now not used
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            }
            else if(TAG_VENDOR == i)//file organisation now not used
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            }
            else if(TAG_INFO_URL == i)//file further describing url now not used
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            }
            else if(TAG_ICON_URI == i)//file icon url now not used
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            }
            else if(TAG_INSTALL_PARAM == i)//file install parameter now not used
            {
                oma_dd_data->dd_element[i] = dd_element;
                break;                
            }
#endif            
            else//do not support this dd tag
            {
                break;
            }

        }

        //search next one
        i++;

    }

}




/*==========================================================
 * Function 	: oma_extract_all_dd_element
 * Author		: sunguochen      
 * Description : 解析dd 文件
 * Parameter	: type          parameter       Description
                      const char*                        dd_data         - dd  buffer
                      oma_download_desc_t*  oma_dd_data  -存放解析后的dd 信息
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
static comm_boolean comm_OmaExtractAllTag(const char* dd_data, oma_download_desc_t* oma_dd_data)
{

    char* ptr = NULL;

    if(dd_data == NULL)
    {
        return FALSE;
    }
    
    ptr = strchr(dd_data, '<');

    while((NULL != ptr) && (0 != *ptr))
    {
        if(*ptr==0x3c&&*(ptr+1)!='/')	//meet tag  start flag .  0x3c =' < '
        {
            //int i;
            int len_tag, len_element;
            const char* ptr_element = NULL;
            const char* ptr_tag = ptr;

            /*get */
            ptr_tag++;			
            ptr++;
            for(len_tag=0;*ptr!=0x3e;ptr++)//0x3e='>'
            {
                len_tag++;
            }
            ptr++;

            ptr_element=ptr;

            for(len_element=0;*ptr!=0x3c;ptr++)//meet tag end flag
            {
                len_element++;
            }
            ptr++;

            comm_OmaExtractOneTag(ptr_tag, len_tag, ptr_element, len_element, oma_dd_data);

        }
        else
        {
            ptr = strchr(ptr, '<');
        }

    }

    return TRUE;

}



/*==========================================================
 * Function 	: comm_start_oma_download
 * Author		: sunguochen      
 * Description : 开始oma download
                      目前不支持多个 oma download并发,
                      如果发现之前有download，根据状态处理
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
comm_boolean comm_start_oma_download(const char* dd_data, oma_download_desc_t* oma_dd_data)
{

    comm_uint32 free_space = 0;
    
    if(!oma_dd_data)
    {
        //memory is error
        //COMM_TRACE:"comm_start_oma_download mem err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_OMA_DOWNLOAD_294_112_2_18_3_26_9_12,(uint8*)"");
        return FALSE;
    }
    
    if(!dd_data || !*dd_data)
    {
        //dd error
        oma_dd_data->download_status = OMA_STATUS_INVALID_DD;
        goto Oma_download_error;
    }

    if(oma_dd_data->id_downloading)
    {
        //COMM_TRACE:"comm_start_oma_download is downloading, can not down"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_OMA_DOWNLOAD_307_112_2_18_3_26_9_13,(uint8*)"");
        return FALSE;
    }

    //parse dd and get key infomations
    comm_OmaExtractAllTag(dd_data, oma_dd_data);    

    //dd error can not download
    if((NULL == oma_dd_data->dd_element[TAG_OBJECT_URI])
      ||(NULL == oma_dd_data->dd_element[TAG_SIZE])
      ||NULL==oma_dd_data->dd_element[TAG_TYPE])
      
    {
        oma_dd_data->download_status = OMA_STATUS_INVALID_DD;
        goto Oma_download_error;

    }

   //file size is zero dd error
    if(!oma_dd_data->file_size)
    {    	
        oma_dd_data->download_status = OMA_STATUS_INVALID_DD;
        goto Oma_download_error;

    }

    //free space is enough?
    free_space = comm_DownloadFreeSpace();

    if(oma_dd_data->file_size > free_space)
    {
        oma_dd_data->download_status = OMA_STATUS_LACK_MEM;
        goto Oma_download_error;        
    }

// TODO: 开始http下载 /*-sunguochen 2010.3.31 -*/
    


    return TRUE;


Oma_download_error:
    
    //COMM_TRACE:"comm_start_oma_download error !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_OMA_DOWNLOAD_351_112_2_18_3_26_9_14,(uint8*)"");
    
    comm_PostFailResult(oma_dd_data);
    
    return FALSE;

}


/*==========================================================
 * Function 	: Music_PostSuccessResult
 * Author		: sunguochen      
 * Description : 发送状态报告
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_PostSuccessResult (oma_download_desc_t* oma_dd_data)
{
    int                   str_len =0;
    char*               ret;

    oma_dd_data->download_result = TRUE;

    //No report url ,not need to report
    if(!oma_dd_data->dd_element[TAG_INSTALL_NOTIFY_URI]) 
    {        
        return;
    }

    ret = (char *) comm_alloc(OMA_NOTIFY_REPORT_MAX_LEN);
    if(NULL == ret)
    {
        return;
    }
    
    memset(ret, 0, OMA_NOTIFY_REPORT_MAX_LEN);
    

    strcpy(ret, OMA_DL_SUCCESS);


    //add command end flag -CRLF
    str_len = strlen(ret);

    ret[str_len++] = 0x0d;
    ret[str_len++] = 0x0a;    
    

    //post result
    //display download result
    //COMM_TRACE:"comm_PostSuccessResult %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_OMA_DOWNLOAD_402_112_2_18_3_26_9_15,(uint8*)"s", ret);

    comm_free (ret); //lint !e774

}



/*==========================================================
 * Function 	: Music_PostFailResult
 * Author		: sunguochen      
 * Description : 发送状态报告
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_PostFailResult (oma_download_desc_t* oma_dd_data)
{

    int                   str_len =0;
    char*               ret;
    char*               result_str_ptr = NULL;

    oma_dd_data->download_result = FALSE;

    //No report url ,not need to report
    if(!oma_dd_data || !oma_dd_data->dd_element[TAG_INSTALL_NOTIFY_URI]) 
    {        
        return;
    }

    ret = (char *) comm_alloc(OMA_NOTIFY_REPORT_MAX_LEN);
    if(NULL == ret)
    {
        return;
    }
    
    memset(ret, 0, OMA_NOTIFY_REPORT_MAX_LEN);

    //error result    
    switch(oma_dd_data->download_status)
    {
        case OMA_STATUS_SUCCESS:
            result_str_ptr = OMA_DL_SUCCESS;
            break;

        case OMA_STATUS_LACK_MEM:
            result_str_ptr = OMA_DL_NO_MEMORY;
            break;

        case OMA_STATUS_CANCEL:
            result_str_ptr = OMA_DL_USER_CANCEL;
            break;
            
        case OMA_STATUS_LOSS_SERVICE:
            result_str_ptr = OMA_DL_LOSS_SERVICE;
            break;

        case OMA_STATUS_MISMATCH:
            result_str_ptr = OMA_DL_ATTRIBUTE_ERR;
            break;

        case OMA_STATUS_INVALID_DD:
            result_str_ptr = OMA_DL_DD_INVALID;
            break;

        case OMA_STATUS_INVALID_VERSION:
            result_str_ptr = OMA_DL_VERSION_INVALID;
            break;

        case OMA_STATUS_ABORT:
            result_str_ptr = OMA_DL_DEVICE_ABORT;
            break;  

        case OMA_STATUS_NON_ACCEPTABLE:
            result_str_ptr = OMA_DL_ERR_CONTENT;
            break;  

        case OMA_STATUS_LOADER_ERR:
            result_str_ptr = OMA_DL_LOADER_ERR;
            break;  

        default:
            break;

    }

    if(!result_str_ptr)
    {
        comm_free (ret); //lint !e774        
        return;
    }
    
    strcpy(ret, result_str_ptr);

    //add command end flag -CRLF
    str_len = strlen(ret);

    ret[str_len++] = 0x0d;
    ret[str_len++] = 0x0a;    
    
    //post result
    //display download result
    //COMM_TRACE:"comm_PostFailResult %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_OMA_DOWNLOAD_505_112_2_18_3_26_9_16,(uint8*)"s", ret);

    comm_free (ret); //lint !e774

}





