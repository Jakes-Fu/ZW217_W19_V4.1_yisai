/******************************************************************************
** File Name:      mmiwww_slim_peer_alt.c                                     *
** Author:                                                                    *
** Date:           06/15/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the slim alt peer for WWW    *
**                 Browser                                                    *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create                                    *
******************************************************************************/
#define _MMIWWW_SLIM_PEER_ALT_C_  

/**--------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/
#include "mmi_app_www_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
#include "mmiwww_slim_peer.h"
#include "block_mem.h"
#include "mmibrowser_export.h"
#include "mmi_appmsg.h"
#include "mmidisplay_data.h"
#include "mmiwww_func.h"
#include "mmiwww_export.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//#define SLIM_PEER_MEM_DEBUG

#ifdef SLIM_PEER_MEM_DEBUG
#define PEER_MEM_DP(msg) PEER_DP(msg)
#else
#define PEER_MEM_DP(msg) 
#endif

#define MMIWWW_INVALID_MEM_HANDLE 0x00
#define MMIWWW_MEM_FROM_BLOCK  0x0000FFFF
#define MMIWWW_MEM_FROM_SYS_POOL 0xFFFF0000
#define MMIWWW_POOL_NAME   "www pool"

#define MMIWWW_ALERT_MEMORY_POOR_SIZE    (MMIWWW_MEMORY_SIZE / 2)
#define MMIWWW_MEM_FLAG_LEN (sizeof(uint32))

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL void *s_www_mem_handle = PNULL;
BOOLEAN s_is_alloc = FALSE;

//由于如果将mem_handle定义在applet，每次获取mem_handle效率很低，所以定义成全局变量

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: WWW Malloc Memory
//	Global resource dependence:
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void *mmiwww_Malloc(uint32 mem_size) ;

/*****************************************************************************/
// 	Description: WWW Free Memory
//	Global resource dependence:
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void mmiwww_Free(void *ptr) ;

/**--------------------------------------------------------------------------*
 **                         Constant Variables                               *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: Create Memory Handle
//	Global resource dependence:
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_CreateMemory(void)
{
#if 0
    PEER_DP(("MMIWWW_CreateMemory"));

    if (MMIWWW_INVALID_MEM_HANDLE == s_www_mem_handle)
    {
        uint8 *block_addr = PNULL;
        uint32 block_size = 0;
        APP_MEM_CFG_T mem_cfg = {0};

        block_addr = (uint8 *)MMI_BL_MALLOC((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_WAP);
        block_size = BL_GetSize((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_WAP);

        mem_cfg.min_pool_size = 4;
        mem_cfg.pool_space_size = block_size >> 2;
        mem_cfg.heap_mem_watermark = 20 * 1024;
        mem_cfg.pool_mode = MEM_POOL_RELEASE_MODE;

        s_www_mem_handle = SCI_CreateMemPool(MMIWWW_POOL_NAME, block_addr, block_size, &mem_cfg);
        PEER_DP(("MMIWWW_CreateMemory s_www_mem_handle=0x%x", s_www_mem_handle));

        if (MMIWWW_INVALID_MEM_HANDLE == s_www_mem_handle)
        {
            PEER_DP(("MMIWWW_CreateMemory Create Mem handle Fail!"));
            MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_WAP);
        }
    }
    else
    {
        SCI_PASSERT(FALSE, ("MMIWWW_CreateMemory Memory Handle Has been Created!"));
    }
#else
    uint32 mem_size = MMIWWW_MEMORY_SIZE - MMIWWW_DISPLAY_BUFFER_SIZE;
#ifdef USE_EXT_DISPLAY_BUFFER
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
#endif /* USE_EXT_DISPLAY_BUFFER */

    //SCI_TRACE_LOW:"MMIWWW_CreateMemory mem_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_ALT_125_112_2_18_3_11_51_119,(uint8*)"d", mem_size);

    s_www_mem_handle = SCI_ALLOCA(mem_size);
    if (PNULL == s_www_mem_handle)
    {
        //SCI_TRACE_LOW:"MMIWWW_CreateMemory mem_handle No mem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_ALT_130_112_2_18_3_11_51_120,(uint8*)"");
        return FALSE;
    }

#ifdef USE_EXT_DISPLAY_BUFFER
    if (PNULL != www_instance && PNULL == www_instance->display_buf_ptr)
    {
        www_instance->display_buf_ptr = SCI_ALLOCA(MMIWWW_DISPLAY_BUFFER_SIZE);

        if (PNULL == www_instance->display_buf_ptr)
        {
            //SCI_TRACE_LOW:"MMIWWW_CreateMemory buf_ptr No mem"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_ALT_141_112_2_18_3_11_51_121,(uint8*)"");
            SCI_FREE(s_www_mem_handle);
            s_www_mem_handle = PNULL;
            return FALSE;
        }
    }
#endif /* USE_EXT_DISPLAY_BUFFER */
#endif
    return TRUE;
}

/*****************************************************************************/
// 	Description: Destroy Memory Handle
//	Global resource dependence:
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_DestroyMemory(void)
{
#if 0
    PEER_DP(("MMIWWW_DestroyMemory"));

    if (MMIWWW_INVALID_MEM_HANDLE != s_www_mem_handle)
    {
        SCI_DeleteMemPool(s_www_mem_handle, TRUE);
        s_www_mem_handle = MMIWWW_INVALID_MEM_HANDLE;
    }

    MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_WAP);
#else
    if (PNULL != s_www_mem_handle)
    {
        SCI_FREE(s_www_mem_handle);
        s_is_alloc = FALSE;
        s_www_mem_handle = PNULL;
    }

#ifdef USE_EXT_DISPLAY_BUFFER
    MMIWWW_FreeDisplayBuf();
#endif /* USE_EXT_DISPLAY_BUFFER */

#endif
}

/*****************************************************************************/
// 	Description: WWW Malloc Memory
//	Global resource dependence:
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void *mmiwww_Malloc(uint32 mem_size) 
{
#if 0
    uint8 *new_ptr = PNULL;
    uint32 real_size = mem_size + MMIWWW_MEM_FLAG_LEN;

    if (0 == mem_size)
    {
        PEER_DP(("mmiwww_Malloc error:size == 0"));
        return PNULL;
    }

    if (MMIWWW_INVALID_MEM_HANDLE != s_www_mem_handle)
    {
        new_ptr = SCI_MPALLOC(real_size, s_www_mem_handle);

        if (PNULL != new_ptr)
        {
            *((uint32*)new_ptr) = real_size;
        }
    }
    
    if (PNULL == new_ptr)
    {
        return PNULL;

//        new_ptr = SCI_ALLOCA(real_size);

//        if (PNULL != new_ptr)
//        {
//            *((uint32*)new_ptr) = MMIWWW_MEM_FROM_SYS_POOL;
//        }
    }

    //if (PNULL != new_ptr)
    {
        //SCI_MEMSET((uint8 *)(new_ptr + MMIWWW_MEM_FLAG_LEN), 0x00, mem_size);

        return (void*)(new_ptr + MMIWWW_MEM_FLAG_LEN);
    }
//    else
//    {
//        PEER_DP(("mmiwww_Malloc:malloc failed,size = %d", mem_size));
//        return PNULL;
//    }
#else
    return PNULL;
#endif
}

/*****************************************************************************/
// 	Description: WWW Free Memory
//	Global resource dependence:
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void mmiwww_Free(void *ptr) 
{
    uint32 flag = 0;
    void* real_ptr = PNULL;
    
    real_ptr = (void*)((uint32)ptr - sizeof(uint32));

    flag = *((uint32*)real_ptr);

    if (MMIWWW_INVALID_MEM_HANDLE != s_www_mem_handle)
    {
        SCI_MPFREE(real_ptr);
    }
    else
    {
        PEER_DP(("mmiwww_Free:s_www_mem_handle has been free!"));
    }
}

/*****************************************************************************/
// Description : Allocates memory
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void *slimMemoryAltMallocPeer(slim_int in_size)
{
#if 0
    uint8 *p = PNULL; 
    
    if (0 == in_size)
    {
        PEER_DP(("slimMemoryAltMallocPeer error:size == 0"));
        return PNULL;
    }

    PEER_MEM_DP(("PEER_ALT:slimMemoryAltMallocPeer, size=%d", in_size));
    p = (uint8 *)mmiwww_Malloc(in_size);

    if (PNULL == p)
    {
        return PNULL;
    }

    SCI_MEMSET(p, 0x00, in_size);

    PEER_MEM_DP(("PEER_ALT:slimMemoryAltMallocPeer, alloc ptr=0x%p", p));

    return (void *)p;
#else
    //SCI_TRACE_LOW:"slimMemoryAltMallocPeer size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_ALT_289_112_2_18_3_11_51_122,(uint8*)"d", in_size);
    if (!s_is_alloc)
        {
        s_is_alloc = TRUE;
        return s_www_mem_handle;
    }
    else
    {
        return PNULL;
    }
#endif
}

/*****************************************************************************/
// Description : Releases memory
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimMemoryAltFreePeer(void *in_p)
{
#if 0
    if (PNULL == in_p)
    {
        return;
    }

    PEER_MEM_DP(("PEER_ALT:slimMemoryAltFreePeer, free ptr=0x%p", in_p));

    mmiwww_Free(in_p);
#else
    //SCI_TRACE_LOW:"slimMemoryAltFreePeer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_ALT_318_112_2_18_3_11_51_123,(uint8*)"");
    s_is_alloc = FALSE;
#endif
}

/*****************************************************************************/
// Description : Reallocates memory
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void *slimMemoryAltReallocPeer(void *in_p, slim_int in_size)
{
#if 0
    PEER_MEM_DP(("PEER_ALT:slimMemoryAltReallocPeer, new size%d", in_size));

    if (PNULL == in_p)
    {
        if (0 == in_size)
        {
            return PNULL;
        }
        else
        {
            char *p2 = PNULL; 

            p2 = mmiwww_Malloc(in_size);

            if (PNULL != p2)
            {
                SCI_MEMSET(p2, 0x00, in_size);
            }

            return p2;
        }
    }
    else //in_p != PNULL
    {
        if (0 == in_size)
        {
            mmiwww_Free(in_p);

            return PNULL;
        }
        else
        {
            char *p2 = PNULL;
            uint32 org_size = 0;

            p2 = mmiwww_Malloc(in_size);

            if (PNULL != p2)
            {
                org_size = *(uint32 *)((uint8 *)in_p - MMIWWW_MEM_FLAG_LEN);
                SCI_MEMSET(p2, 0x00, in_size);
                SCI_MEMCPY(p2, in_p, (uint32)(org_size < (uint32)in_size ? org_size : (uint32)in_size));

                mmiwww_Free(in_p);

                return p2;
            }
            else
            {
                return PNULL;
            }
        }
    }
#else
    return PNULL;
#endif
}
#endif //#ifdef BROWSER_SUPPORT_NETFRONT
