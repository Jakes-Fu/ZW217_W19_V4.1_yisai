/*****************************************************************************
** File Name:      mmipb_mem.h                                               *
** Author:                                                                   *
** Date:           11/18/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to memmory mangement                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma          Create
******************************************************************************/
#define _MMIPB_MEM_C_  


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
//#ifdef WIN32

#include "mmi_app_pb_trc.h"
#include "mmk_app.h"
#include "mmipb_mem.h"
//#include "mmipb_adapt.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMI_MEM_ALLOC_UINT_SIZE  100
#define MMI_MEM_ALLOC_MAX_NUM  (1024)
#define MMI_MEM_ALLOC_MAX_NODE (10)
#define MMI_MEM_UNALLOC_VALID  (0xffff)

//每个内存单元信息
typedef struct
{
    uint8  mem_ptr[MMI_MEM_ALLOC_UINT_SIZE];
}MMIMEM_UINT_INFO_T;

typedef struct MMIMEM_NODE_INFO_T_tag
{
    MMIMEM_UINT_INFO_T  *mem_pool_arr;  //mem pool buffer ptr
    uint16 is_alloc[MMI_MEM_ALLOC_MAX_NUM]; //record all state
    uint32 unalloc_num;                 //left unalloc uint num
    uint32 total_num;                   //total uint number
    BOOLEAN is_find;                    //find flag
    struct MMIMEM_NODE_INFO_T_tag *next_pool_node; 
    struct MMIMEM_NODE_INFO_T_tag *pre_pool_node;
}MMIMEM_NODE_INFO_T;

//内存分配情况总信息
typedef struct
{
    uint32 node_num;   // total node number
    MMIMEM_NODE_INFO_T *next_pool_node;
}MMIMEM_HEAD_INFO_T;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMIMEM_HEAD_INFO_T *s_mem_pool_head_ptr = PNULL; //mem head ptr

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get mem pool ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIMEM_HEAD_INFO_T *GetMemPoolHead(void);

/*****************************************************************************/
//  Description : get mem pool ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIMEM_NODE_INFO_T *CreadMemPoolNode(void);

/*****************************************************************************/
//  Description : get valid poor ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIMEM_NODE_INFO_T * GetValidNodePtr(
                                                  MMIMEM_HEAD_INFO_T **head_info, //in/out
                                                  uint32 alloc_num, //in
                                                  BOOLEAN *is_no_memory //in/out
                                                  );

/*****************************************************************************/
//  Description : get alloc num
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint32 GetAllcoNum(uint32 size);


/*****************************************************************************/
//  Description : get valid poor ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void * GetValidPoolPtr(
                            MMIMEM_NODE_INFO_T * mem_node_ptr, //in
                            uint32 need_alloc_num    //in
                            );

/*****************************************************************************/
//  Description : search and reset pool state
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchAndResetPoolState(
                                             MMIMEM_NODE_INFO_T * mem_node, //in
                                             void * mem_ptr //in
                                             );
/*****************************************************************************/
//  Description : free pool node
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FreePoolNode(MMIMEM_NODE_INFO_T *  mem_node);

/*****************************************************************************/
//  Description : find right free ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FreePtr(void* mem_ptr);

/*****************************************************************************/
//  Description : get mem pool ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIMEM_HEAD_INFO_T *GetMemPoolHead(void)
{
    MMIMEM_HEAD_INFO_T * head_ptr = PNULL;

    head_ptr = SCI_ALLOCA(sizeof(MMIMEM_HEAD_INFO_T));
    if (PNULL != head_ptr)
    {
        SCI_MEMSET(head_ptr,0x00,sizeof(MMIMEM_HEAD_INFO_T));
        head_ptr->node_num = 0;
    }

    return head_ptr;
}
/*****************************************************************************/
//  Description : get mem pool ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIMEM_NODE_INFO_T *CreadMemPoolNode(void)
{
    MMIMEM_NODE_INFO_T * mem_node_ptr = PNULL;
    int32 i = 0;
    
    mem_node_ptr = SCI_ALLOCA(sizeof(MMIMEM_NODE_INFO_T));
    if (PNULL == mem_node_ptr)
    {
        return mem_node_ptr;
    }
    SCI_MEMSET(mem_node_ptr,0x00,sizeof(MMIMEM_NODE_INFO_T));
    
    mem_node_ptr->mem_pool_arr = SCI_ALLOCA(sizeof(MMIMEM_UINT_INFO_T)*MMI_MEM_ALLOC_MAX_NUM);
    if ( PNULL == mem_node_ptr->mem_pool_arr)
    {
        SCI_FREE(mem_node_ptr);
        return PNULL;
    }
    mem_node_ptr->total_num = MMI_MEM_ALLOC_MAX_NUM;
    mem_node_ptr->unalloc_num = MMI_MEM_ALLOC_MAX_NUM;
    mem_node_ptr->is_find = FALSE;
    mem_node_ptr->next_pool_node = PNULL;
    mem_node_ptr->pre_pool_node = PNULL;
    SCI_MEMSET(mem_node_ptr->mem_pool_arr,0x00,(sizeof(MMIMEM_UINT_INFO_T)*MMI_MEM_ALLOC_MAX_NUM));
    SCI_MEMSET(mem_node_ptr->is_alloc,0x00,(sizeof(uint16)*MMI_MEM_ALLOC_MAX_NUM));
    
    for (i = 0 ; i < MMI_MEM_ALLOC_MAX_NUM ; i++)
    {
        mem_node_ptr->is_alloc[i] = MMI_MEM_UNALLOC_VALID;
    }

    return mem_node_ptr;
}


/*****************************************************************************/
//  Description : get valid poor ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIMEM_NODE_INFO_T * GetValidNodePtr(
                                                  MMIMEM_HEAD_INFO_T **head_info, //in/out
                                                  uint32 alloc_num, //in
                                                  BOOLEAN *is_no_memory //in/out
                                                  )
{
    MMIMEM_NODE_INFO_T * mem_node_ptr = PNULL;
    MMIMEM_HEAD_INFO_T * head_info_ptr = PNULL; 
    MMIMEM_NODE_INFO_T * pre_mem_node_ptr = PNULL;
    
    if (PNULL == *head_info)
    {
        //get mem head ptr
        *head_info = GetMemPoolHead();
        if (PNULL == *head_info)
        {
            SCI_TRACE_LOW("[MMIMEM]:MMIMEM_Malloc PNULL == mem_ptr 1");
            *is_no_memory =TRUE;
            return mem_node_ptr;
        }
        mem_node_ptr = CreadMemPoolNode();
        if (PNULL == mem_node_ptr)
        {
            *is_no_memory =TRUE;
            SCI_TRACE_LOW("[MMIMEM]:MMIMEM_Malloc PNULL == mem_ptr 2");
            return mem_node_ptr;
        }
        head_info_ptr = *head_info;
        head_info_ptr->next_pool_node = mem_node_ptr;
        head_info_ptr->node_num ++;
    }
    else
    {
        head_info_ptr = *head_info;
        mem_node_ptr = head_info_ptr->next_pool_node;
    }


    while (mem_node_ptr)
    {
        if (
            alloc_num <= mem_node_ptr->total_num  
            && alloc_num <= mem_node_ptr->unalloc_num 
            && !mem_node_ptr->is_find  //whether this node have alloc 
            )
        {
            *is_no_memory = FALSE;
            return mem_node_ptr;
        }
        //get next pool node
        pre_mem_node_ptr = mem_node_ptr;
        mem_node_ptr = mem_node_ptr->next_pool_node;
    }
    if (/*PNULL == mem_node_ptr &&*/ MMI_MEM_ALLOC_MAX_NODE > head_info_ptr->node_num)
    {
        //all new node
        mem_node_ptr = CreadMemPoolNode();
        if (PNULL == mem_node_ptr)
        {
            *is_no_memory =TRUE;
            SCI_TRACE_LOW("[MMIMEM]:MMIMEM_Malloc PNULL == mem_ptr 3");
            return PNULL;
        }
        mem_node_ptr->pre_pool_node = pre_mem_node_ptr;
        if(pre_mem_node_ptr != PNULL)
        {
            pre_mem_node_ptr->next_pool_node = mem_node_ptr;
        }
        head_info_ptr->node_num ++;
        if (PNULL == head_info_ptr->next_pool_node)
        {
            head_info_ptr->next_pool_node = mem_node_ptr;
        }
        *head_info = head_info_ptr;
        *is_no_memory = FALSE;
    }
    return mem_node_ptr;
}

/*****************************************************************************/
//  Description : get alloc num
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint32 GetAllcoNum(uint32 size)
{
    uint32 alloc_num = size/MMI_MEM_ALLOC_UINT_SIZE;
    uint32 mode_num = size%MMI_MEM_ALLOC_UINT_SIZE;

    alloc_num = mode_num > 0 ? (alloc_num+1):alloc_num;

    return  alloc_num;
}

/*****************************************************************************/
//  Description : get valid poor ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void * GetValidPoolPtr(
                            MMIMEM_NODE_INFO_T * mem_node_ptr,
                            uint32 need_alloc_num
                            )
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 k = 0;
    void * mem_pool_ptr = PNULL;


    if (PNULL == mem_node_ptr)
    {
        return PNULL;
    }
    
    if (need_alloc_num > mem_node_ptr->total_num)
    {
        return PNULL;
    }
    for(i = 0; i < mem_node_ptr->total_num; i++ )
    {
        if( MMI_MEM_UNALLOC_VALID == mem_node_ptr->is_alloc[i])
        {
            j++;
        }
        else
        {
            j = 0;
        }
        if (j == need_alloc_num)
        { 
            i++;
            break;
        }
    }
    if (i <= mem_node_ptr->total_num)
    {
        //get right pool 
        k = i - need_alloc_num;
        
        //return the find ptr
        mem_pool_ptr = &mem_node_ptr->mem_pool_arr[k].mem_ptr;
        
        mem_node_ptr->unalloc_num = mem_node_ptr->unalloc_num - need_alloc_num;

        for(; k < i; k++)
        {
            mem_node_ptr->is_alloc[k] = i - need_alloc_num;
        }

        mem_node_ptr->is_find = FALSE;
    }
    else
    {
        //set this node find flag
        mem_node_ptr->is_find = TRUE;
    }
    return mem_pool_ptr;
}


/*****************************************************************************/
//  Description : search and reset pool state
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchAndResetPoolState(
                                     MMIMEM_NODE_INFO_T * mem_node, 
                                     void * mem_ptr
                                     )
{
    MMIMEM_NODE_INFO_T * mem_node_ptr = PNULL;
    BOOLEAN result = FALSE;
    uint32 i = 0;
    uint32 j = 0;
    uint32 alloc_num = 0;
    if (PNULL == mem_node || PNULL == mem_ptr)
    {
        return result;
    }
    mem_node_ptr = mem_node;
    
    for (i = 0 ; i < mem_node_ptr->total_num; i++)
    {
        if (mem_ptr == &mem_node_ptr->mem_pool_arr[i].mem_ptr)
        {
            //get alloc mark 
            alloc_num = mem_node_ptr->is_alloc[i];
        
            //reset first find mem pool state
            mem_node_ptr->is_alloc[i] = MMI_MEM_UNALLOC_VALID;
            mem_node_ptr->unalloc_num ++;
            for(j = i + 1 ; j < mem_node_ptr->total_num; j++)
            {
                if (alloc_num != mem_node_ptr->is_alloc[j])
                {
                    break;
                }
                //reset mem pool state
                mem_node_ptr->is_alloc[j] = MMI_MEM_UNALLOC_VALID;
                mem_node_ptr->unalloc_num ++;
            }
            result = TRUE;
            break;
        }
    }
    return result;
}
/*****************************************************************************/
//  Description : free pool node
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FreePoolNode(MMIMEM_NODE_INFO_T *  mem_node)
{
    MMIMEM_NODE_INFO_T *  cur_mem_node_ptr = PNULL;
    MMIMEM_NODE_INFO_T *  pre_mem_node_ptr = PNULL;
    MMIMEM_NODE_INFO_T *  next_mem_node_ptr = PNULL;
    
    if ( PNULL == mem_node)
    {
        return;
    }
    cur_mem_node_ptr = mem_node;
    pre_mem_node_ptr = cur_mem_node_ptr->pre_pool_node;
    next_mem_node_ptr = cur_mem_node_ptr->next_pool_node;
    
    //free no use mem node pool
    if (cur_mem_node_ptr->total_num == cur_mem_node_ptr->unalloc_num)
    {
        if ( cur_mem_node_ptr == s_mem_pool_head_ptr->next_pool_node)
        {
            s_mem_pool_head_ptr->next_pool_node = next_mem_node_ptr;
            if (PNULL != next_mem_node_ptr)
            {
                next_mem_node_ptr->pre_pool_node = PNULL;
            }
        }
        else
        {
            pre_mem_node_ptr->next_pool_node = next_mem_node_ptr;
            if (PNULL != next_mem_node_ptr)
            {
                next_mem_node_ptr->pre_pool_node = pre_mem_node_ptr;
            }
        }

        s_mem_pool_head_ptr->node_num --;

        SCI_FREE(cur_mem_node_ptr->mem_pool_arr);
        SCI_FREE(cur_mem_node_ptr);
    }
    //free nu use mem head node, if have any mem node.
    if (0 == s_mem_pool_head_ptr->node_num)
    {
        SCI_FREE(s_mem_pool_head_ptr);
        s_mem_pool_head_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : find right free ptr
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FreePtr(void* mem_ptr)
{
    BOOLEAN result = FALSE;
    MMIMEM_NODE_INFO_T * mem_node_ptr = PNULL;
    
    if (PNULL != s_mem_pool_head_ptr)
    {
        mem_node_ptr = s_mem_pool_head_ptr->next_pool_node;
    }
    while (PNULL != mem_node_ptr)
    {
       //search right ptr node and reset alloc pool state
       result = SearchAndResetPoolState(mem_node_ptr,mem_ptr);

       if (result)
       {
            //free pool node
            FreePoolNode(mem_node_ptr);
            break;
       }
       mem_node_ptr = mem_node_ptr->next_pool_node;
    }
    return result;
}
/*****************************************************************************/
//  Description : malloc memory
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void* MMIMEM_Malloc(uint32 size)
{
    uint32 need_alloc_num = GetAllcoNum(size);
    void* mem_ptr = PNULL;
    MMIMEM_NODE_INFO_T * mem_node_ptr = PNULL;
    BOOLEAN is_no_memory = FALSE;

    if (need_alloc_num > MMI_MEM_ALLOC_MAX_NUM)
    {
        //SCI_TRACE_LOW:"[MMIMEM]:MMIMEM_Malloc PNULL == mem_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MEM_467_112_2_18_2_43_34_231,(uint8*)"");
        mem_ptr = SCI_ALLOCA(size);
        return mem_ptr;
    }
    
    do
    {
        //get node buffer

        mem_node_ptr = GetValidNodePtr(&s_mem_pool_head_ptr,need_alloc_num,&is_no_memory);

        if(is_no_memory ||(PNULL == mem_node_ptr && MMI_MEM_ALLOC_MAX_NODE <= s_mem_pool_head_ptr->node_num))
        {
            break;
        }
        //get mem buffer 
        mem_ptr = GetValidPoolPtr(mem_node_ptr,need_alloc_num);


    } while( PNULL == mem_ptr);
    
    //reset mem node use flag
    mem_node_ptr = s_mem_pool_head_ptr->next_pool_node;
    
    while ( PNULL != mem_node_ptr )
    {
        if (mem_node_ptr->is_find)
        {
            mem_node_ptr->is_find = FALSE;
        }
        mem_node_ptr = mem_node_ptr->next_pool_node;
    }
    
    if( PNULL == mem_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMEM]:MMIMEM_Malloc PNULL == mem_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MEM_502_112_2_18_2_43_34_232,(uint8*)"");
        mem_ptr = SCI_ALLOCA(size);
    }

    //SCI_TRACE_LOW:"[MMIMEM]:MMIMEM_Malloc mem_ptr =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MEM_506_112_2_18_2_43_34_233,(uint8*)"d",mem_ptr);

    return mem_ptr;

}

/*****************************************************************************/
//  Description : free memory
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void MMIMEM_MFree(void * mem_ptr)
{
    if(!FreePtr(mem_ptr))
    {
        SCI_FREE(mem_ptr);
    }
    mem_ptr = PNULL;
}


/*****************************************************************************/
//  Description : malloc memory
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void * MMIPB_MAlloc(
                    uint32              size,
                    const char          *file,
                    uint32              line
                    )
{
    SCI_ASSERT(PNULL!=file); /*assert verified*/

    //SCI_TRACE_LOW:"MMIPB_Malloc: size = %d,file = %s, line=%ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MEM_538_112_2_18_2_43_34_234,(uint8*)"dsd",size,file,line);

    return MMIMEM_Malloc(size);
}

/*****************************************************************************/
//  Description : free memory
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_MFree(
                         void             *mem_ptr, 
                         const char       *file,
                         uint32           line
                         )
{
    SCI_ASSERT(PNULL!=file); /*assert verified*/

    //SCI_TRACE_LOW:"MMIMEM_MFree: mem_ptr = 0x%x,file = %s, line=%ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MEM_555_112_2_18_2_43_34_235,(uint8*)"dsd",mem_ptr,file,line);

    MMIMEM_MFree(mem_ptr);
}

#ifdef WIN32
PUBLIC void MMIMEM_test(void)
{
    int32 i = 0;
    int32 j = 0;
    uint32 test[300] = {0};
    uint32 size = 600;//sizeof(MMIPB_CONTACT_T);
    for (j = 0 ; j<5 ; j++)
    {
        for(i = 0; i<200;i++)
        {
            test[i] = MMIPB_MALLOC(size*10);
        }

        for(i = 0; i <200;i++)
        {
            MMIPB_MFREE(test[i]);
        }
    }
    
}

#endif


