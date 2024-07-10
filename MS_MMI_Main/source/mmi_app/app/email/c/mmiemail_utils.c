/****************************************************************************
** File Name:      mmiemail_utils.c                                        *
** Author:                                                                 *
** Date:           05/13/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011                           create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_email_trc.h"
#include "mmiemail_utils.h"
#include "we_def.h"
#include "mmiemail_def.h"
#include "mmicom_time.h"
#include "we_scl.h"
#include "cafhelp.h"
#include "gui_ucs2b_converter.h"
#include "mmiset_export.h"

/**---------------------------------------------------------------------------*
**                         Macro Declaration    							  *
**---------------------------------------------------------------------------*/
#define DW_CC_PARAM_INT        0xcdcdcdcd
#define DW_CC_PARAM_BYTE       0xcd

#define BYTE_AFTER_FREE        0xEE

#define EME_EMAIL_DATE_YMD_LEN 10   // 2007/12/31
#define EME_EMAIL_DATE_MD_LEN  5    // 12/31
#define EME_EMAIL_TIME_LEN     8    // 24:59:59

#define EME_EMAIL_ADDR_SPACE     0x0020
#define EME_EMAIL_ADDR_QUOTA     0x0022
#define EME_EMAIL_ADDR_LEFT_B    0x003C
#define EME_EMAIL_ADDR_RIGHT_B   0x003E
#define EMAIL_TRACE_STRING_LEN   256

/**---------------------------------------------------------------------------*
**                         TYPE AND CONSTANT    							  *
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL char  s_DBGbuf[EMAIL_TRACE_STRING_LEN] = {0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : charset usc2 to utf8 len
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
LOCAL WE_INT32 CharsetUcs2ToUtf8Len
(
    const WE_CHAR *src_ptr, 
    WE_LONG *src_Len_ptr, 
    WE_LONG  *dst_Len_ptr,
    WE_INT32  big_endian
);


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description: malloc memory
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void* MMIEMAIL_Malloc(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, unsigned long size,const char * file_name_ptr, int line)
{
    struct email_mem_link_tag * temp_ptr = PNULL;
    int result = EMAIL_SUCCESS;
    unsigned char* dwp_ptr = PNULL;//extra memory
    int32 len = 0;
    
    //check input parameters
    if(PNULL == file_name_ptr || 0 == size)
    {
        EMA_UTILS_LOG_INFO(("EMA_UTILS_MALLOC, invalid param."));
        return PNULL;
    }
    
    do 
    {
        if(PNULL == mem_mgr_ptr)
        {
            return (void*)SCI_ALLOC_APP(size);
        }
        
        if(PNULL == mem_mgr_ptr->tail_ptr)
        {
            mem_mgr_ptr->tail_ptr = (struct email_mem_link_tag*)&mem_mgr_ptr->header; 
        }
        /************************************************************************/
        /*Initialize memory list node , the node consists of (node + data content + extra memory)*/
        /************************************************************************/
        temp_ptr = (struct email_mem_link_tag *)SCI_ALLOCAZ(size + sizeof(struct email_mem_link_tag) + sizeof(unsigned long) * 2);
        EMA_UTILS_BREAKNULL(temp_ptr, result, "failed to malloc");
        SCI_MEMSET(temp_ptr, 0, size + sizeof(struct email_mem_link_tag) + sizeof(unsigned long) * 2);
        /************************************************************************/
        /*  initialize node                                                     */                
        /************************************************************************/
        temp_ptr->cc1 = temp_ptr->cc2 = DW_CC_PARAM_INT;
        temp_ptr->id = 0;
        temp_ptr->length = size;
        temp_ptr->line = line;
        //temp_ptr->file_name_ptr = strdup(file_name_ptr);
        
        temp_ptr->mem_mgr_ptr = mem_mgr_ptr;
        
        mem_mgr_ptr->tail_ptr->next_ptr = temp_ptr;
        temp_ptr->next_ptr = PNULL;
        temp_ptr->last_ptr = mem_mgr_ptr->tail_ptr;
        mem_mgr_ptr->tail_ptr = temp_ptr;
        /************************************************************************/
        /* initialize 2 extra memory block                                      */                              
        /************************************************************************/
        dwp_ptr = (unsigned char*)((unsigned long)temp_ptr + sizeof(struct email_mem_link_tag) + size);
        
        /************************************************************************/
        /*  avoid init address value using integer cos it will lead to memory alignment problem*/
        /************************************************************************/
        for(len = 0; len< (int)(sizeof(unsigned long) * 2); len++)
        {
            *(dwp_ptr + len) = DW_CC_PARAM_BYTE;
        }
        
        /************************************************************************/
        /* update the current memory we used                                    */                            
        /************************************************************************/
        mem_mgr_ptr->status.lib_mem_used += size;
        if(mem_mgr_ptr->status.lib_mem_max < mem_mgr_ptr->status.lib_mem_used) 
        {
            mem_mgr_ptr->status.lib_mem_max = mem_mgr_ptr->status.lib_mem_used;
        } 
        mem_mgr_ptr->status.lib_mem_obj ++;
        
    } while(0);
    
    //exception handling
    if(EMAIL_SUCCESS != result)
    {
        free(temp_ptr);
        return PNULL;
    }
    
    
    EMA_UTILS_LOG_INFO(("**************************************************************************************"));
    EMA_UTILS_LOG_INFO(("Malloced memory, Address: %x, FILE", (unsigned long)temp_ptr + sizeof(struct email_mem_link_tag)));
    EMA_UTILS_LOG_INFO(("Truely Address: %x", temp_ptr));
    EMA_UTILS_LOG_INFO(("%s",file_name_ptr));
    EMA_UTILS_LOG_INFO(("%d",line));
    EMA_UTILS_LOG_INFO(("**************************************************************************************"));
    
    return (void*)((unsigned long)temp_ptr + sizeof(struct email_mem_link_tag));
}

/*****************************************************************************/
// 	Description: out put memory that used
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_OutputMemUsed(EMAIL_MEM_HANDLE_T* mem_mgr_ptr)
{
    struct email_mem_link_tag * link_ptr = PNULL;
    
    if(mem_mgr_ptr->status.lib_mem_obj > 0)
    {
        EMA_UTILS_LOG_INFO(("memory leak, there are %d blocks and %u bytes not released", 
            mem_mgr_ptr->status.lib_mem_obj, mem_mgr_ptr->status.lib_mem_used));
        
        /************************************************************************/
        /* check the list for the memory unreleased                             */                                        
        /************************************************************************/
        for(link_ptr = mem_mgr_ptr->header.next_ptr; PNULL != link_ptr; link_ptr = link_ptr->next_ptr)
        {
            if(PNULL == link_ptr->file_name_ptr)
            {
                EMA_UTILS_LOG_INFO(("**********************************************************************************************************"));
                EMA_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
                EMA_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
                EMA_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
                EMA_UTILS_LOG_INFO(("unknown memory leak, %u bytes, the %d times allocate", link_ptr->length, link_ptr->id));
                EMA_UTILS_LOG_INFO(("Memory leak, Address: %x", (unsigned long)link_ptr + sizeof(struct email_mem_link_tag)));
                EMA_UTILS_LOG_INFO(("Truely Address: %x", link_ptr));    
                EMA_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
                EMA_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
                EMA_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
                EMA_UTILS_LOG_INFO(("**********************************************************************************************************"));
                
            }
            else
            {
                EMA_UTILS_LOG_INFO(("**********************************************************************************************************"));
                EMA_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
                EMA_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
                EMA_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));            
                EMA_UTILS_LOG_INFO(("memory leak, %s(%d): %u bytes, the %d times allocate", 
                    link_ptr->file_name_ptr,
                    link_ptr->line,
                    link_ptr->length,
                    link_ptr->id));
                EMA_UTILS_LOG_INFO(("Memory leak, Address: %x", (unsigned long)link_ptr + sizeof(struct email_mem_link_tag)));
                EMA_UTILS_LOG_INFO(("Truely Address: %x", link_ptr));               
                EMA_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
                EMA_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
                EMA_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
                EMA_UTILS_LOG_INFO(("**********************************************************************************************************"));
            }
        }
    }
    else
    {
        EMA_UTILS_LOG_INFO(("maximum %d(%d k)bytes have been used ", 
            mem_mgr_ptr->status.lib_mem_max,
            mem_mgr_ptr->status.lib_mem_max/1024));
    }
}

/*****************************************************************************/
// 	Description: realloc memory
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void* MMIEMAIL_Realloc(EMAIL_MEM_HANDLE_T* mem_mgr_ptr,void *src_ptr, unsigned long size,const char * file_name_ptr,int line)
{
    struct email_mem_link_tag * temp_ptr = PNULL;
    struct email_mem_link_tag * temp2_ptr = PNULL;
    unsigned long src_size = 0;
    int result = EMAIL_SUCCESS;
    unsigned char* dwp_ptr = PNULL;//extra memory
    void *new_temp_ptr = PNULL;
    int32 len = 0;
    
    
    //check input parameters
    if(PNULL == file_name_ptr)
    {
        EMA_UTILS_LOG_INFO(("EMA_UTILS_MALLOC, invalid param."));
        return PNULL;
    }
    
    do 
    {
        if(PNULL == mem_mgr_ptr)
        {
            return (void*)realloc(src_ptr,size);
        }
        
        if (PNULL == src_ptr)
        {
            return (void *)EMA_UTILS_MALLOC(mem_mgr_ptr,size);
        }
        
        /*compute the size of  src_ptr*/
        temp2_ptr = (struct email_mem_link_tag *)((unsigned long)src_ptr -  sizeof(struct email_mem_link_tag));
        src_size = temp2_ptr->length;
        
        /*error handling : reallocate buffer smaller the the source buff  */
        if (size < src_size)
        {
            result = EMAIL_ERROR;
            break;
        }
        
        
        if(PNULL == mem_mgr_ptr->tail_ptr)
        {
            mem_mgr_ptr->tail_ptr = (struct email_mem_link_tag*)&mem_mgr_ptr->header; 
        }
        /************************************************************************/
        /*Initialize memory list node , the node consists of (node + data content + extra memory)*/
        /************************************************************************/
        temp_ptr = (struct email_mem_link_tag *)SCI_ALLOCAZ(size + sizeof(struct email_mem_link_tag) + sizeof(unsigned long) * 2);
        EMA_UTILS_BREAKNULL(temp_ptr, result, "failed to malloc");
        SCI_MEMSET(temp_ptr, 0, size + sizeof(struct email_mem_link_tag) + sizeof(unsigned long) * 2);
        /************************************************************************/
        /*  initialize node                                                     */                
        /************************************************************************/
        temp_ptr->cc1 = temp_ptr->cc2 = DW_CC_PARAM_INT;
        temp_ptr->id = 0;
        temp_ptr->length = size;
        temp_ptr->line = line;
        //temp_ptr->file_name_ptr = strdup(file_name_ptr);
        
        /* for other mod free and no mem-header free */
        temp_ptr->mem_mgr_ptr = mem_mgr_ptr;
        
        mem_mgr_ptr->tail_ptr->next_ptr = temp_ptr;
        temp_ptr->next_ptr = PNULL;
        temp_ptr->last_ptr = mem_mgr_ptr->tail_ptr;
        mem_mgr_ptr->tail_ptr = temp_ptr;
        /************************************************************************/
        /* initialize 2 extra memory block                                      */                              
        /************************************************************************/
        dwp_ptr = (unsigned char*)((unsigned long)temp_ptr + sizeof(struct email_mem_link_tag) + size);
        
        /************************************************************************/
        /*  avoid init address value using integer cos it will lead to memory alignment problem*/
        /************************************************************************/
        for(len = 0; len< (int)(sizeof(unsigned long) * 2); len++)
        {
            *(dwp_ptr + len) = DW_CC_PARAM_BYTE;
        }
        
        /************************************************************************/
        /* update the current memory we used                                    */                            
        /************************************************************************/
        mem_mgr_ptr->status.lib_mem_used += size;
        if(mem_mgr_ptr->status.lib_mem_max < mem_mgr_ptr->status.lib_mem_used) 
        {
            mem_mgr_ptr->status.lib_mem_max = mem_mgr_ptr->status.lib_mem_used;
        } 
        mem_mgr_ptr->status.lib_mem_obj ++;
        
        /*new allocate memory*/
        new_temp_ptr =(void *)((unsigned long)temp_ptr + sizeof(struct email_mem_link_tag));
        
        /*copy source data to new buff*/
        SCI_MEMCPY((void *)new_temp_ptr,(const void *)src_ptr,src_size);
        
        /*clear source data buffer*/
        EMA_UTILS_FREE(mem_mgr_ptr,src_ptr);
        temp2_ptr = PNULL;
        
    } while(0);
    
    
    //exception handling
    if(EMAIL_SUCCESS != result)
    {
        free(temp_ptr);
        return PNULL;
    }
    
    EMA_UTILS_LOG_INFO(("**************************************************************************************"));
    EMA_UTILS_LOG_INFO(("Malloced memory, Address: %x, FILE", (unsigned long)temp_ptr + sizeof(struct email_mem_link_tag)));
    EMA_UTILS_LOG_INFO(("Truely Address: %x", temp_ptr));
    EMA_UTILS_LOG_INFO(("%s",file_name_ptr));
    EMA_UTILS_LOG_INFO(("%d",line));
    EMA_UTILS_LOG_INFO(("**************************************************************************************"));
    
    return (void*)new_temp_ptr;
}

/*****************************************************************************/
// 	Description: free memory
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_Free(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, void * mem_free_ptr,const char * file_name_ptr,int line)
{
    struct email_mem_link_tag * temp_ptr = PNULL;
    unsigned char* dwp_ptr = PNULL;
    int32  i = 0;
    int result = EMAIL_SUCCESS;
    
    if(PNULL == mem_free_ptr)
    {
        return EMAIL_ERROR;
    }
    
    do 
    {
        if(PNULL == mem_mgr_ptr)
        {
            EMA_UTILS_FREEIF(PNULL,mem_free_ptr);
            return EMAIL_SUCCESS;
        }
        
        temp_ptr = (struct email_mem_link_tag*)((unsigned long)mem_free_ptr - sizeof(struct email_mem_link_tag));//point to the node
        EMA_UTILS_BREAKNULL(temp_ptr, result, "failed to malloc");
        
        /* test only */
        mem_mgr_ptr = (EMAIL_MEM_HANDLE_T*)EMA_UTILS_MEMMGR_FOR_FREE;
        /* test only */
        if ((unsigned long)mem_mgr_ptr == EMA_UTILS_MEMMGR_FOR_FREE)
        {
            mem_mgr_ptr = (void*)temp_ptr->mem_mgr_ptr;
        }
        //check the node
        //check the length of actual data content
        if(DW_CC_PARAM_INT != temp_ptr->cc1 || DW_CC_PARAM_INT != temp_ptr->cc2)
        {
            result = EMAIL_ERROR;
            temp_ptr->cc1 = temp_ptr->cc2 = 0xcccccccc;//memory not initialized
        }
        if (EMAIL_SUCCESS != result)
        {
            EMA_UTILS_LOG_INFO(("**********************************************************************************************************"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("Free Memory Error! Address: %x",mem_free_ptr));
            EMA_UTILS_LOG_INFO(("Truely Address: %x", temp_ptr));
            EMA_UTILS_LOG_INFO(("%s",file_name_ptr));
            EMA_UTILS_LOG_INFO(("Line: %d",line));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
            EMA_UTILS_LOG_INFO(("**********************************************************************************************************"));
            break;
            //OEMP_UTILS_BREAKIF(result, "Free Memory Error!Address: %x, FILE: %s, LINE: %d",mem_free_ptr,file_name_ptr,line);
        }
        /************************************************************************/
        /*check the last_ptr two checksum                                           */                           
        /************************************************************************/
        dwp_ptr = (unsigned char*)((unsigned long)mem_free_ptr + temp_ptr->length);
        for (i = 0; i< (int)(sizeof(unsigned long)) * 2; i++)
        {
            if (*dwp_ptr != DW_CC_PARAM_BYTE)
            {
                result = EMAIL_ERROR;
                break;
            }
            
            dwp_ptr++;
        }
        
        if (EMAIL_SUCCESS != result)
        {
            EMA_UTILS_LOG_INFO(("**********************************************************************************************************"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));     
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));                 
            EMA_UTILS_LOG_INFO(("Free Memory Error! Address: %x",mem_free_ptr));
            EMA_UTILS_LOG_INFO(("Truely Address: %x", temp_ptr));
            EMA_UTILS_LOG_INFO(("%s",file_name_ptr));
            EMA_UTILS_LOG_INFO(("Line: %d",line));
            EMA_UTILS_LOG_INFO(("The Malloc Info:",mem_free_ptr));
            EMA_UTILS_LOG_INFO(("%s",temp_ptr->file_name_ptr));
            EMA_UTILS_LOG_INFO(("Line: %d",temp_ptr->line));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));     
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
            EMA_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO")); 
            EMA_UTILS_LOG_INFO(("**********************************************************************************************************"));
            break;
        }
        /************************************************************************/
        /* in case the node to delete is the last node                          */                                           
        /************************************************************************/
        if(temp_ptr == mem_mgr_ptr->tail_ptr)
        {
            mem_mgr_ptr->tail_ptr = temp_ptr->last_ptr;
            mem_mgr_ptr->tail_ptr->next_ptr = PNULL;
        }
        /************************************************************************/
        /*  in case the node to delete is in the middle of reverse list         */                                                           
        /************************************************************************/
        else
        {
            temp_ptr->last_ptr->next_ptr = temp_ptr->next_ptr;
            temp_ptr->next_ptr->last_ptr = temp_ptr->last_ptr;
        }
        mem_mgr_ptr->status.lib_mem_obj--;
        mem_mgr_ptr->status.lib_mem_used -= temp_ptr->length;
        EMA_UTILS_FREEIF(PNULL,temp_ptr->file_name_ptr);
        EMA_UTILS_MEMSET(mem_free_ptr, BYTE_AFTER_FREE, temp_ptr->length);      
        
        EMA_UTILS_FREE(PNULL,temp_ptr);   
   } while(0);
   
   if(EMAIL_SUCCESS != result)
   {
   }
   return result;
}

/*****************************************************************************/
// 	Description: change the w string to string format string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC char *MMIEMAIL_WstrToStr(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, wchar *src_ptr)
{
    WE_UINT32   str_len = 0;
    WE_CHAR     *dst_ptr = PNULL;
    
    if (PNULL == src_ptr)
    {
        return PNULL;
    }
    
    str_len = EMA_UTILS_WSTRLEN(src_ptr);
    
    dst_ptr = (char*)EMA_UTILS_MALLOC(mem_mgr_ptr, (str_len + 1) * sizeof(char));
    if (PNULL == dst_ptr)
    {
        return PNULL;
    }
    EMA_UTILS_MEMSET(dst_ptr, 0, (str_len + 1) * sizeof(char));
    
    if (!MMIAPICOM_WstrToStr(src_ptr, (uint8*)dst_ptr))
    {
        EMA_UTILS_FREE(mem_mgr_ptr, dst_ptr);
        return PNULL;
    }
    
    return dst_ptr;
}

/*****************************************************************************/
// 	Description: change the w string to string format string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar *MMIEMAIL_StrToWstr(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, char *pcFrom)
{
    WE_UINT32   str_len = 0;
    wchar     *dst_ptr = PNULL;
    
    if (PNULL == pcFrom)
    {
        return PNULL;
    }
    
    str_len = EMA_UTILS_STRLEN(pcFrom);
    
    dst_ptr = (wchar*)EMA_UTILS_MALLOC(mem_mgr_ptr, (str_len + 1) * sizeof(wchar));
    if (PNULL == dst_ptr)
    {
        return PNULL;
    }
    EMA_UTILS_MEMSET(dst_ptr, 0, (str_len + 1) * sizeof(wchar));
    
    if (!MMIAPICOM_StrToWstr((const uint8*)pcFrom, dst_ptr))
    {
        EMA_UTILS_FREE(mem_mgr_ptr, dst_ptr);
        return PNULL;
    }
    
    return dst_ptr;
}

/*****************************************************************************/
// 	Description: change the uft8 to w string format string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar *MMIEMAIL_Utf8ToWstr(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, WE_CHAR *pcFrom)
{
    WE_UINT32      str_len = 0;
    wchar      *dst_ptr = PNULL;
    EMAIL_MEM_HANDLE_T *mem_ptr = PNULL;
    
    if (PNULL == pcFrom)
    {
        return PNULL;
    }
    /*Get memory handle*/
    mem_ptr = mem_mgr_ptr;
    
    str_len = EMA_UTILS_STRLEN(pcFrom);
    
    /*Allocate memory for the WSTR*/
    dst_ptr = (wchar*)EMA_UTILS_MALLOC(mem_ptr, (str_len + 1) * sizeof(wchar));
    if (PNULL == dst_ptr)
    {
        return PNULL;
    }
    EMA_UTILS_MEMSET(dst_ptr, 0, (str_len + 1) * sizeof(wchar));
    
    if (EMA_UTILS_STRLEN(pcFrom) > 0)
    {
        if (!GUI_UTF8ToWstr(dst_ptr, (str_len + 1), (const uint8*)pcFrom, str_len))
        {
            EMA_UTILS_FREE(mem_ptr, dst_ptr);
            return PNULL;
        }
    }
    
    return dst_ptr;
}

/*****************************************************************************/
// 	Description: change the w string to uft8 format string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC char *MMIEMAIL_WstrToUtf8(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, wchar *src_ptr)
{
    uint32      str_len = 0;
    uint32      str_bytes = 0;
    char        *dst_ptr = PNULL;
    uint32      utf8_len = 0x7fffffff;
#ifdef CHIP_ENDIAN_LITTLE
    BOOLEAN     isBiged = FALSE;
#else
    BOOLEAN     isBiged = TRUE;
#endif
    
    if (PNULL == src_ptr)
    {
        return PNULL;
    }
    
    str_len = EMA_UTILS_WSTRLEN(src_ptr);
    str_bytes = str_len * sizeof(wchar);
    //utf8_len = str_len * MAX_UTF8_CHAR_LEN + 1;
#ifdef WIN32
    CharsetUcs2ToUtf8Len((char*)src_ptr, (long*)&str_bytes, (long*)&utf8_len, FALSE);
#else
    CharsetUcs2ToUtf8Len((char*)src_ptr, (long*)&str_bytes, (long*)&utf8_len, isBiged);
#endif
    if (0x7fffffff == utf8_len)
    {
        return PNULL;
    }
    utf8_len = utf8_len + 1;  /*Null-terminate*/
    
    dst_ptr = (char*)EMA_UTILS_MALLOC(mem_mgr_ptr, utf8_len * sizeof(char));
    if (PNULL == dst_ptr)
    {
        return PNULL;
    }
    EMA_UTILS_MEMSET(dst_ptr, 0, utf8_len * sizeof(char));
    
    if (EMA_UTILS_WSTRLEN(src_ptr))
    {
        if (!GUI_WstrToUTF8((uint8*)dst_ptr, (utf8_len-1) * sizeof(char), src_ptr, str_len))
        {
            EMA_UTILS_FREE(mem_mgr_ptr, dst_ptr);
            return PNULL;
        }
    }
    
    return dst_ptr;
}

/*****************************************************************************/
// Description : charset usc2 to utf8 len
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
LOCAL WE_INT32 CharsetUcs2ToUtf8Len
(
 const WE_CHAR *src_ptr, 
 WE_LONG *src_Len_ptr, 
 WE_LONG  *dst_Len_ptr,
 WE_INT32  big_endian
 )
{
    WE_UINT32   trans_to_ucs = 0;
    WE_UINT32   sin_dex = 0;
    WE_UINT32   din_dex = 0;
    WE_UINT32   sou_num_byte = 0; 
    WE_UINT32   des_num_byte = 0;
    WE_UINT8    uft8_mask = 0;
    WE_UINT32   shift_first = 0;
    WE_UINT32   shift_second = 0;
    
    
    if(PNULL == src_ptr || PNULL == src_Len_ptr || PNULL == dst_Len_ptr)
    {
        return FALSE;
    }
    if (big_endian)
    {
        shift_first = 0; shift_second = 1; 
    }
    else
    {
        shift_first = 1; shift_second = 0; 
    }
    
    while(1)//lint !e716
    {
        /* First, translate to UCS4 */
        if ((WE_LONG)(sin_dex + 2 )> *src_Len_ptr)
            break;
        trans_to_ucs = (WE_UINT32)(((WE_UCHAR)src_ptr[sin_dex + shift_first] << 8) |(WE_UCHAR)src_ptr[sin_dex + shift_second]);
        sou_num_byte = 2;
        
        /* Determine how many UTF8-bytes are needed for this character,
        * and which bits should be set in the first byte. */
        if (trans_to_ucs < 0x80)
        {
            des_num_byte = 1;
            uft8_mask = 0;
        }
        else if (trans_to_ucs < 0x800)
        {
            des_num_byte = 2;
            uft8_mask = 0xc0;
        }
        else 
        {
            des_num_byte = 3;
            uft8_mask = 0xe0;
        }
        
        /* Is there room for this in the destination vector? */
        if ((WE_LONG)(din_dex + des_num_byte) > *dst_Len_ptr)
            break;
        
        sin_dex += (WE_UINT32)sou_num_byte;
        din_dex += (WE_UINT32)des_num_byte;
    }
    *src_Len_ptr = (WE_LONG)sin_dex;
    *dst_Len_ptr = (WE_LONG)din_dex;
    
    return TRUE;    
}

/*****************************************************************************/
// Description : charset usc2 to utf8 len
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC char* MMIEMAIL_StrStrNC(char *to_search_ptr, const char *search_for_ptr)
{
    uint32 pos = 0;
    uint32 i = 0;
    uint32 j = 0;
    
    if (PNULL == to_search_ptr || PNULL ==search_for_ptr)
    {
        return PNULL;
    }
    for (pos = 0; pos < EMA_UTILS_STRLEN(to_search_ptr); pos++)
    {
        /* set the begin pos for find. */
        j = pos; 
        
        if (EMA_UTILS_STRLEN(to_search_ptr)- pos < EMA_UTILS_STRLEN(search_for_ptr))
        {
            return PNULL;
        }
        
        for (i = 0; i < EMA_UTILS_STRLEN(search_for_ptr); i++)
        {
            if (to_search_ptr[j] == search_for_ptr[i])
            {
                j++;         
            }       
            
            else
            {
                if ( to_search_ptr[j] < 'A' || to_search_ptr[j] > 'z' || (to_search_ptr[j] > 'Z' && to_search_ptr[j] < 'a') || /* isn't alpha */
                    (to_search_ptr[j] >= 'A' && to_search_ptr[j] <= 'Z' && to_search_ptr[j] + ' ' != search_for_ptr[i]) || /* is upper alpha */
                    (to_search_ptr[j] >= 'a' && to_search_ptr[j] <= 'z' && to_search_ptr[j] != search_for_ptr[i] + ' ') ) /* is lower alpha */
                {
                    break;
                }
                
                else         
                {
                    j++;
                }
            }         
        }
        
        if ( search_for_ptr[i] == '\0' )
        {
            /* here find the pos in the to_search_ptr . */
            return &to_search_ptr[pos];
        }      
    }
    
    return PNULL;
}


/*****************************************************************************/
// 	Description: string copy
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC char* MMIEMAIL_StrDup(void* mem_mgr_ptr, char* src_ptr)
{
    char * temp_ptr = PNULL;
    
    if(PNULL == src_ptr)
    {
        return PNULL;
    }
    
    if(PNULL !=  (temp_ptr = (char*)EMA_UTILS_MALLOC(mem_mgr_ptr, sizeof(char) * (EMA_UTILS_STRLEN(src_ptr) + 1))))
    {
        WE_UINT32 uiLen = (EMA_UTILS_STRLEN(src_ptr) + 1) * sizeof(char);
        EMA_UTILS_MEMSET(temp_ptr , 0x00, uiLen);
        uiLen = EMA_UTILS_STRLEN(src_ptr) *sizeof(char);
        EMA_UTILS_MEMCPY(temp_ptr , src_ptr, uiLen);
    }
    return temp_ptr;
}

/*****************************************************************************/
// 	Description: w string copy
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIEMAIL_WstrDup(void* mem_mgr_ptr, wchar* src_ptr)
{
    wchar * temp_ptr = PNULL;
    
    if(PNULL == src_ptr)
    {
        return   PNULL;
    } 
    
    if(PNULL !=  (temp_ptr = (wchar*)EMA_UTILS_MALLOC(mem_mgr_ptr, sizeof(wchar) * (EMA_UTILS_WSTRLEN(src_ptr) + 1))))
    {
        WE_UINT32 uiLen = (EMA_UTILS_WSTRLEN(src_ptr) + 1)* sizeof(wchar);
        EMA_UTILS_MEMSET(temp_ptr , 0x00, uiLen);
        uiLen = EMA_UTILS_WSTRLEN(src_ptr)* sizeof(wchar);
        EMA_UTILS_MEMCPY(temp_ptr , src_ptr, uiLen);
    }       
    return temp_ptr;       
}

/*****************************************************************************/
// 	Description: get file suffix
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL WE_CHAR  *GetFileSuffix(WE_CHAR  *file_name_ptr)
{
    char  *temp_ptr = PNULL;
    WE_BOOL  is_found = FALSE;
    
    if (PNULL == file_name_ptr)
    {
        return PNULL;
    }
    
    temp_ptr = (file_name_ptr) + EMA_UTILS_STRLEN(file_name_ptr) - 1;
    while (temp_ptr > file_name_ptr)
    {
        if (*temp_ptr == '.')
        {
            if (temp_ptr == (file_name_ptr) + EMA_UTILS_STRLEN(file_name_ptr) - 1)
            {
                is_found = FALSE;
                break;
            }         
            is_found = TRUE;
            temp_ptr++;
            break;
        }
        temp_ptr--;
    }
    
    if (is_found)
    {
        return temp_ptr;
    }
    else
    {
        return PNULL;
    }
}


/*****************************************************************************/
// 	Description: get email address
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar *MMIEMAIL_GetEmailAddress(wchar *full_addr_ptr)
{
    wchar      *addr_ptr = PNULL;
    WE_UINT32   len = 0;
    wchar      *temp_ptr = PNULL;
    wchar      *rq_ptr = PNULL;
    wchar      *lb_ptr = PNULL;
    WE_BOOL     is_ok = FALSE;
    
    if (PNULL == full_addr_ptr)
    {
        return PNULL;
    }
    
    len = EMA_UTILS_WSTRLEN(full_addr_ptr);
    
    temp_ptr = full_addr_ptr;
    while (temp_ptr < full_addr_ptr + len)
    {
        if (*temp_ptr == EME_EMAIL_ADDR_SPACE)
        {
            temp_ptr++;
        }
        else
        {
            is_ok = TRUE;
            break;
        }
    }
    if (!is_ok)
    {
        addr_ptr = full_addr_ptr;
        return addr_ptr;
    }
    
    if (*temp_ptr == EME_EMAIL_ADDR_LEFT_B && *(temp_ptr + EMA_UTILS_WSTRLEN(temp_ptr) - 1) == EME_EMAIL_ADDR_RIGHT_B)
    {
        return full_addr_ptr + 1;
    }
    else if (*temp_ptr != EME_EMAIL_ADDR_QUOTA ||
        *(temp_ptr + EMA_UTILS_WSTRLEN(temp_ptr) - 1) != EME_EMAIL_ADDR_RIGHT_B)
    {
        addr_ptr = full_addr_ptr;
        return addr_ptr;
    }
    
    while (temp_ptr < full_addr_ptr + len)
    {
        temp_ptr++;
        if (*temp_ptr == EME_EMAIL_ADDR_QUOTA)
        {
            if (PNULL != rq_ptr)
            {
                is_ok = FALSE;
                break;
            }
            rq_ptr = temp_ptr;
        }
        if (*temp_ptr == EME_EMAIL_ADDR_LEFT_B)
        {
            if (PNULL != lb_ptr)
            {
                is_ok = FALSE;
                break;
            }
            lb_ptr = temp_ptr;
        }     
    }
    if (PNULL== lb_ptr || PNULL == rq_ptr)
    {
        is_ok = FALSE;
    }
    else if (lb_ptr < rq_ptr)
    {
        is_ok = FALSE;
    }
    
    if (!is_ok)
    {
        addr_ptr = full_addr_ptr;
        return addr_ptr;
    }

    if(PNULL != lb_ptr)
    {
      addr_ptr = lb_ptr + 1;//lint !e613
    }
    
    return addr_ptr;
}


/*****************************************************************************/
// 	Description: get date time
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIEMAIL_GetDateTime(WE_VOID *mem_mgr_ptr, uint32 date, EMAIL_DATE_TYPE_E date_type) 
{
    wchar *date_w_ptr = PNULL;
    char *date_ptr = PNULL;
    wchar *temp_ptr = PNULL;
    MMI_TM_T date_time = {0};
    //int size = 0;
    int size = 0;
    MMISET_DATE_DISPLAY_TYPE_E set_date_type = 0;
    MMISET_TIME_DISPLAY_TYPE_E set_time_type = 0;
    
    do 
    {
        date_time = MMIAPICOM_Second2Tm(date);
        
        /* count the length of the text */
        if (EMA_DATE_YMD == date_type)
        {
            //size = (EME_EMAIL_DATE_LEN + 1)*sizeof(wchar);
            size = (EME_EMAIL_DATE_YMD_LEN + 1)*sizeof(char);
        }
        else if (EMA_DATE_MD == date_type)
        {
            //size = (EME_EMAIL_DATE_LEN + 1)*sizeof(wchar);
            size = (EME_EMAIL_DATE_MD_LEN + 1)*sizeof(char);
        }
        else if (EMA_TIME == date_type)
        {
            //size = (EME_EMAIL_TIME_LEN + 1)*sizeof(wchar);
            size = (EME_EMAIL_TIME_LEN + 1)*sizeof(char);
        }
        else
        {
            //size = (EME_EMAIL_DATE_LEN + EME_EMAIL_TIME_LEN + 2) * sizeof(wchar);
            size = (EME_EMAIL_DATE_YMD_LEN + EME_EMAIL_TIME_LEN + 2) * sizeof(char);
        }
        
        /* malloc the memory */
        //       date_w_ptr = (wchar*)EMA_UTILS_MALLOC(mem_mgr_ptr, size);
        //       if (PNULL==date_w_ptr)
        //       {
        //          break;
        //       }
        //       EMA_UTILS_MEMSET(date_w_ptr, 0 , size);
        
        date_ptr = (char *)EMA_UTILS_MALLOC(mem_mgr_ptr, size);
        if (PNULL == date_ptr)
        {
            break;
        }
        EMA_UTILS_MEMSET(date_ptr, 0 , size);
        
        /* get date Y/M/D */
        if (EMA_DATE_YMD == date_type)
        {
            set_date_type = MMIAPISET_GetDateDisplayType();
            if (MMISET_DATE_YMD == set_date_type)
            {
                EMA_UTILS_SPRINTF(date_ptr, "%02d/%02d/%02d", date_time.tm_year, date_time.tm_mon, date_time.tm_mday);
            }
            else if(MMISET_DATE_MDY == set_date_type)
            {   
                EMA_UTILS_SPRINTF(date_ptr, "%02d/%02d/%02d", date_time.tm_mon, date_time.tm_mday, date_time.tm_year);
            }
            else if(MMISET_DATE_DMY == set_date_type)
            {
                EMA_UTILS_SPRINTF(date_ptr, "%02d/%02d/%02d",  date_time.tm_mday, date_time.tm_mon, date_time.tm_year);
            }
            /*         EMA_UTILS_WSPRINTF(date_w_ptr, size, L"%d-%d-%d", date_time.tm_year, date_time.tm_mon, date_time.tm_mday);*/
            date_w_ptr = EMA_UTILS_STRTOWSTR(PNULL, date_ptr);
        }
        /* get date M/D */
        else if (EMA_DATE_MD == date_type)
        {
            set_date_type = MMIAPISET_GetDateDisplayType();
            if (MMISET_DATE_YMD == set_date_type || MMISET_DATE_MDY == set_date_type)
            {
                 EMA_UTILS_SPRINTF(date_ptr, "%02d/%02d", date_time.tm_mon, date_time.tm_mday);
            }
            else
            {
                 EMA_UTILS_SPRINTF(date_ptr, "%02d/%02d", date_time.tm_mday, date_time.tm_mon);
            }
            /*         EMA_UTILS_WSPRINTF(date_w_ptr, size, L"%d-%d-%d", date_time.tm_year, date_time.tm_mon, date_time.tm_mday);*/
            date_w_ptr = EMA_UTILS_STRTOWSTR(PNULL, date_ptr);
        }
        /* get time */
        else if (EMA_TIME == date_type)
        {
            set_time_type = MMIAPISET_GetTimeDisplayType();
            if (MMISET_TIME_24HOURS == set_time_type)
            {
                EMA_UTILS_SPRINTF(date_ptr, "%02d:%02d", date_time.tm_hour, date_time.tm_min);
            }
            else if(MMISET_TIME_12HOURS == set_time_type)
            {
                if ((MMISET_TIME_PM_HOUR > date_time.tm_hour))//AM
                {
                    if ((MMISET_TIME_1_HOUR> date_time.tm_hour))//00:00-1:00
                    {
                        EMA_UTILS_SPRINTF((char*)date_ptr,"%02d:%02d%s",(date_time.tm_hour + MMISET_TIME_PM_HOUR),date_time.tm_min,"AM");
                    }
                    else
                    {
                        EMA_UTILS_SPRINTF((char*)date_ptr,"%02d:%02d%s",date_time.tm_hour,date_time.tm_min,"AM");
                    }
                }
                else//PM
                {
                    if ((MMISET_TIME_PM_HOUR <= date_time.tm_hour) && (MMISET_TIME_13_HOUR > date_time.tm_hour))//12:00-13:00
                    {
                        EMA_UTILS_SPRINTF((char*)date_ptr, "%02d:%02d%s", date_time.tm_hour, date_time.tm_min, "PM");
                    }
                    else
                    {
                        EMA_UTILS_SPRINTF((char*)date_ptr, "%02d:%02d%s", (date_time.tm_hour - MMISET_TIME_PM_HOUR), date_time.tm_min, "PM");
                    }
                }
            }
            //EMA_UTILS_WSPRINTF(date_w_ptr, size, L"%2d:%2d", date_time.tm_hour, date_time.tm_min);
            date_w_ptr = EMA_UTILS_STRTOWSTR(PNULL, date_ptr);
            /* 12: 4:30 -> 12:04:30 ,added by jiangdingqing */
            temp_ptr = date_w_ptr;
            while (PNULL != temp_ptr && 0x0000 != *temp_ptr)
            {
                if (0x0020 == *temp_ptr)
                {
                    *temp_ptr = 0x0030;
                }
                temp_ptr++;
            }
        }
        /* YYYY-MM-DD HH:mm:ss */
        else
        {
            //EMA_UTILS_WSPRINTF(date_w_ptr, size, L"%d-%d-%d %2d:%2d", date_time.tm_year, date_time.tm_mon, date_time.tm_mday, date_time.tm_hour, date_time.tm_min);
            EMA_UTILS_SPRINTF(date_ptr, "%02d-%02d-%02d %02d:%02d", date_time.tm_year, date_time.tm_mon, date_time.tm_mday, date_time.tm_hour, date_time.tm_min);
            date_w_ptr = EMA_UTILS_STRTOWSTR(PNULL, date_ptr);
        }      
        
    } while(0);
    
    EMA_UTILS_FREEIF(PNULL, date_ptr);
    
    return date_w_ptr;
}

/*****************************************************************************/
// 	Description: judge is today
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIEMAIL_IsToday(uint32 date)
{
   uint32 curr = 0;
   MMI_TM_T curr_time = {0};
   MMI_TM_T date_time = {0};

   curr = MMIAPICOM_GetCurTime();
   curr_time = MMIAPICOM_Second2Tm(curr);
   date_time =  MMIAPICOM_Second2Tm(date);

   if ((date_time.tm_year==curr_time.tm_year)
       &&(date_time.tm_mon==curr_time.tm_mon)
       &&(date_time.tm_mday==curr_time.tm_mday))
   {
       return EMA_TIME;
   }
   else if (date_time.tm_year == curr_time.tm_year)
   {
       return EMA_DATE_MD;
   }
   else
   {
       return EMA_DATE_YMD;
   }

   //return ((date_time.tm_year==curr_time.tm_year)&&(date_time.tm_mon==curr_time.tm_mon)&&(date_time.tm_mday==curr_time.tm_mday)) ? TRUE : FALSE;
}

/*****************************************************************************/
// 	Description: set size text
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMIEMAIL_SetSize2Text(void *mem_mgr_ptr,uint32 size, wchar ** out_text_pptr)
{
    uint32 temp_total	= 0;
    uint32 total_size	= 0;
    uint32 temp_total2	= 0;
    char   *kb_ptr      = PNULL;
    char   *size_ptr    = PNULL;
    
    size_ptr = EMA_UTILS_MALLOC(mem_mgr_ptr,50);
    if (PNULL == size_ptr)
    {
        return EMAIL_ERROR;
    }
    EMA_UTILS_MEMSET(size_ptr,0x00,50);
    
    if(size >= 1024 * 1024*1000)
    {
        temp_total = (size >>20) & 0x3FF;
        temp_total = temp_total *10/1024;
        total_size = size >>30;
        kb_ptr = "G";
        EMA_UTILS_SPRINTF(size_ptr, "%d.%d%s", total_size,temp_total,kb_ptr);
    }
    else if(size >= 1024 * 1000)
    {
        
        temp_total = ((size>>10)&0x3FF)*10/1024;
        temp_total2= (((size>>10)&0x3FF)*10%1024)*10/1024;
        total_size = size >>20;
        kb_ptr = "M";
        EMA_UTILS_SPRINTF(size_ptr, "%d.%d%d%s", total_size,temp_total,temp_total2,kb_ptr);
    }
    else if (size >= 102)
    {         
        temp_total = size & 0x3FF;
        temp_total = temp_total *10/1024;
        total_size = size >>10;
        kb_ptr = "KB";
        EMA_UTILS_SPRINTF(size_ptr, "%d.%d%s", total_size,temp_total,kb_ptr);
        
    }
    else
    {
        kb_ptr = "B";
        EMA_UTILS_SPRINTF(size_ptr, "%d%s", size,kb_ptr);
    }
    
    if (EMA_UTILS_STRLEN(size_ptr))
    {
        * out_text_pptr = EMA_UTILS_STRTOWSTR(PNULL, size_ptr);
        EMA_UTILS_FREE(PNULL, size_ptr);
        return EMAIL_SUCCESS;
    }
    else
    {
        EMA_UTILS_FREE(PNULL, size_ptr);
        return EMAIL_ERROR;
    }
    
}

/*****************************************************************************/
// 	Description: find the char in w string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar *MMIEMAIL_Wstrchr(wchar *string_ptr, wchar flag)
{
    uint32 index = 0;
    uint32 wstr_len = MMIAPICOM_Wstrlen(string_ptr);
    wchar *tmp_ptr = string_ptr;

    for (index = 0; index < wstr_len; index++)
    {
        if (MMIAPICOM_Wstrncmp(tmp_ptr, (wchar*)&flag, 1) == 0)
        {
            return string_ptr + index;
        }
        tmp_ptr++;
    }

    return PNULL;
}

/*****************************************************************************/
// 	Description: trace
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_DbgPrintf(const char *format_ptr,...)
{
    char *prefix_ptr = "[MMIEMAIL]: ";
    va_list args = {0};
    uint32 len = 0;
    
    va_start(args, format_ptr);   //lint !e628
    
    EMA_UTILS_MEMSET(s_DBGbuf, 0x00, EMAIL_TRACE_STRING_LEN);

    len = EMA_UTILS_STRLEN(prefix_ptr);
    EMA_UTILS_MEMCPY(s_DBGbuf, prefix_ptr, len);
    
    _vsnprintf(s_DBGbuf+EMA_UTILS_STRLEN(prefix_ptr), EMAIL_TRACE_STRING_LEN - EMA_UTILS_STRLEN(prefix_ptr), format_ptr, args);
    
    //SCI_TRACE_LOW:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEMAIL_UTILS_1215_112_2_18_2_16_57_4,(uint8*)"s", s_DBGbuf);
    
    va_end(args);
    
    return;
}
