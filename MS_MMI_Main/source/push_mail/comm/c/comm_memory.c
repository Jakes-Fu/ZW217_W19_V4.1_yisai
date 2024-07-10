/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_memory.c
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#include "mmi_push_mail.h"
#include "std_header.h"
#include "comm_platform.h"
#include "comm_debug.h"

comm_uint32 memory_used_num = 0;


#ifdef MAIL_MEMORY_DEBUG_MODE

#define SAVE_FILE_NAME_LEN 20
#define MAIL_MAX_ALLOC_NUM 2000

typedef struct MAIL_MEM_LIST_
{
    comm_uint32 *addr;
    comm_uint32 size;
    char file_name[SAVE_FILE_NAME_LEN + 1];
    int line_num;

}MAIL_MEM_LIST;


comm_uint32 memory_used_size = 0;
MAIL_MEM_LIST g_mail_mem_table[MAIL_MAX_ALLOC_NUM];//tail link  that is last malloced mem link,



/*==========================================================
 * Function 	: mail_mem_link_init
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/20/2010 
 ==========================================================*/
void mail_mem_link_init(void)
{

    memory_used_num = 0;
    memory_used_size = 0;

    memset(g_mail_mem_table, 0, MAIL_MAX_ALLOC_NUM*sizeof(MAIL_MEM_LIST));	
    
    return;
}



/*==========================================================
 * Function 	: mail_mem_link_alloc
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_mem_link_alloc(comm_uint32* addr, comm_uint32 size, char* file_name, int line_num)
{
    int i = 0;

    char* file_name_ptr = NULL;

    file_name_ptr = strrchr(file_name, '\\');
    if(file_name_ptr)
    {
        //存在路径信息
        file_name_ptr++; 
    }
    else
    {
        //不存在路径信息
        file_name_ptr = file_name;
    }

    for(i=0; i < MAIL_MAX_ALLOC_NUM; i++)
    {
        if(0 == g_mail_mem_table[i].addr)
        {
            g_mail_mem_table[i].size = size;
            g_mail_mem_table[i].addr = addr;

            memcpy(g_mail_mem_table[i].file_name, file_name_ptr, SAVE_FILE_NAME_LEN);
            g_mail_mem_table[i].line_num = line_num;

            memory_used_size += size;
            break;
        }
    }
    
    //COMM_TRACE("Mail:mail_mem_link_alloc-size: %d file: %s line:%d", size, file_name_ptr, line_num);
	
    return;

}



/*==========================================================
 * Function 	: mail_mem_link_free
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_mem_link_free(comm_uint32* addr, char* file_name, int line_num)
{
    int i = 0;

    char* file_name_ptr = NULL;

    file_name_ptr = strrchr(file_name, '\\');
    if(file_name_ptr)
    {
        //存在路径信息
        file_name_ptr++; 
    }
    else
    {
        //不存在路径信息
        file_name_ptr = file_name;
    }

    //COMM_TRACE("Mail:mail_mem_link_free  file: %s line:%d",  file_name_ptr, line_num);
   
    for(i=0; i < MAIL_MAX_ALLOC_NUM; i++)
    {
        if(addr == g_mail_mem_table[i].addr)
        {
            memory_used_size -= g_mail_mem_table[i].size;
            
            g_mail_mem_table[i].size = 0;
            g_mail_mem_table[i].addr = 0;
            memset(g_mail_mem_table[i].file_name, 0, SAVE_FILE_NAME_LEN);
            g_mail_mem_table[i].line_num = 0;

            break;
        }
    }

    if(i >= MAIL_MAX_ALLOC_NUM)
    {
        //free not alloc memory or memory alloced by other applications
        //COMM_TRACE:"Mail:mail_mem_link_free: %x file: %s line:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_MEMORY_154_112_2_18_3_26_8_8,(uint8*)"dsd", addr, file_name, line_num);
        //COMM_ASSERT(0);
    }
	    
    return;
}

#define MAIL_ALLOC_MEMORY_SIZE  (3*1024*1024)

/*==========================================================
 * Function 	: comm_alloc
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void* debug_comm_alloc(comm_uint32 size, char* file_name, int lin_num)
{
    void *p = NULL;

    if(size > MAIL_ALLOC_MEMORY_SIZE)
    {
        char* file_name_ptr = NULL;
    
        file_name_ptr = strrchr(file_name, '\\');
        if(file_name_ptr)
        {
            //存在路径信息
            file_name_ptr++; 
        }
        else
        {
            //不存在路径信息
            file_name_ptr = file_name;
        }        
        //COMM_TRACE:"Mail:debug_comm_alloc size :%d- %s:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_MEMORY_190_112_2_18_3_26_8_9,(uint8*)"dsd", size, file_name_ptr, lin_num);
        return NULL;
    }

    p = SCI_ALLOC(size);

    if(p)
    {
        memory_used_num ++;
        memset(p, 0, size);
        mail_mem_link_alloc(p, size, file_name, lin_num);
    }
    else
    {
        //malloc memory fail
        //COMM_TRACE:"Mail:debug_comm_alloc NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_MEMORY_205_112_2_18_3_26_8_10,(uint8*)"");
        return NULL;
    }

    return p;

}


/*==========================================================
 * Function 	: comm_free
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_free_debug(void* ptr, char* file_name, int lin_num)
{
    if(ptr)
    {
        if(memory_used_num)
        {
            memory_used_num --;
        }

        mail_mem_link_free(ptr, file_name, lin_num);

        SCI_FREE(ptr);

    }
}


#else

/*==========================================================
 * Function 	: comm_alloc
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void* comm_alloc(comm_uint32 size)
{
    void *p = NULL;

    p = SCI_ALLOC(size);

    if(p)
    {
        memory_used_num ++;
        memset(p, 0, size);
    }
    else
    {
        //malloc memory fail
        //COMM_TRACE:"Mail:comm_alloc NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_MEMORY_263_112_2_18_3_26_8_11,(uint8*)"");
        return NULL;
    }
    return p;

}



/*==========================================================
 * Function 	: comm_free_debug
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_free_debug(void* ptr)
{
    if(ptr)
    {
        if(memory_used_num)
        {
            memory_used_num --;
        }

        SCI_FREE(ptr);
    }
}


#endif



/*==========================================================
 * Function 	: comm_memory_unfree
 * Author		: sunguochen      
 * Description : 测试函数
                      退出时调用,可以打印出所有未释放内存信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_memory_unfree(void)
{
#ifdef MAIL_MEMORY_DEBUG_MODE    
    int             i;
    comm_uint32     total_unfree_mem_num = 0;

    total_unfree_mem_num = memory_used_num;
#endif

#ifdef MAIL_DEBUG_SAVE_FILE

    mail_debug_finial();

#endif

    if(memory_used_num)
    {
        COMM_TRACE((char *)"MAIL:some memory is not freed num = %d \n", memory_used_num);

#ifdef MAIL_MEMORY_DEBUG_MODE
        for(i=0; i < MAIL_MAX_ALLOC_NUM; i++)
        {
            if(g_mail_mem_table[i].addr)
            {                
                COMM_TRACE((char *)"MAIL:addr:%x  size: %d  file: %s line: %d \n", 
                                        g_mail_mem_table[i].addr, 
                                        g_mail_mem_table[i].size,
                                        g_mail_mem_table[i].file_name,
                                        g_mail_mem_table[i].line_num);

                total_unfree_mem_num --;
            }

            if(total_unfree_mem_num == 0)
            {
                //print finished
                break;
            }
        }  
#endif    
    }

}

