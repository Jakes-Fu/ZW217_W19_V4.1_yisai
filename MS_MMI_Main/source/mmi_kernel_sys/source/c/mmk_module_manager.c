/****************************************************************************
** File Name:      mmk_module_manager.c                                    *
** Author:         gang.tong                                               *
** Date:           06/03/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application module manager*
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2008        gang.tong        Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/
#include "mmi_kernel_sys_trc.h"
#include "cafshell.h"
#include "cafmodule.h"
#include "mmi_mem.h"
#ifdef DYNAMIC_MODULE_SUPPORT
#include "sfs.h"
#endif
#include "mmi_link.h"
#include "cafhelp.h"
#include "mmk_modinfo.h"
#include "cafmodule_gen.h"
#include "mmk_app.h"
#include "mmk_module_manager.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMK_MODULE_HEAP_SIZE                (1024*50)
/* by wei.zhou,  the app bin size exceed 30k */
/*
#define MMK_DYN_MODULE_CODE_SIZE_MAX        (1024*30)
*/
#define MMK_DYN_MODULE_CODE_SIZE_MAX        (1024*200)
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

typedef enum
{
    MMK_MODULE_ON_FLASH = 0,
    MMK_MODULE_ON_FILE,
    MMK_MODULE_ON_IMG,
    MMK_MODULE_LOCATION_MAX 
}MMK_MODULE_LOCATION_E;

typedef struct caf_module_load_struct
{	
    uint32					com_id;
    MMK_MODULE_LOCATION_E	module_location_type;
#ifdef DYNAMIC_MODULE_SUPPORT
    uint32					code_size;
    uint8*					load_buf_addr;
    uint32					load_buf_size;
#endif
    MODULEENTRY	            EntryFunc;
    IMODULE_T* 				pMod;    
}MMK_MODULE_LOAD_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef DYNAMIC_MODULE_SUPPORT
/* module memory for dynamic load */
LOCAL uint8* s_module_heap_ptr = PNULL;
/* module memory for dynamic load */
LOCAL uint32 s_heap_size = PNULL;
/* module memory manager id */
LOCAL int16 s_heap_manager_id = 0;
/* apply for memory */
LOCAL MMKMNG_APPLYFORMEMORY s_apply_mem_func = PNULL;
/* release memory */
LOCAL MMKMNG_RELEASEMEMORY s_release_mem_func = PNULL;
#endif
/* loaded module manage link head */
LOCAL MMI_LINK_NODE_T * s_loaded_module_head = PNULL;

/* static class */
extern const CAF_STATIC_CLASS_INFO_T g_ctrl_static_class_info;
#ifdef DYNAMIC_MODULE_SUPPORT
extern const CAF_STATIC_CLASS_INFO_T g_guidc_static_class_info;
extern const CAF_STATIC_CLASS_INFO_T g_sfs_static_class_info;
extern const CAF_STATIC_CLASS_INFO_T g_netmgr_static_class_info;
extern const CAF_STATIC_CLASS_INFO_T g_tapi_static_class_info;
extern const CAF_STATIC_CLASS_INFO_T g_sound_static_class_info;
extern const CAF_STATIC_CLASS_INFO_T g_thread_static_class_info;
extern const CAF_STATIC_CLASS_INFO_T g_image_static_class_info;
extern const CAF_STATIC_CLASS_INFO_T g_heap_static_class_info;
extern const CAF_STATIC_CLASS_INFO_T g_media_static_class_info;
#endif

LOCAL const CAF_STATIC_CLASS_INFO_T* const s_static_class_info[] = 
{
    &g_ctrl_static_class_info,
#ifdef DYNAMIC_MODULE_SUPPORT  
    &g_guidc_static_class_info,
    &g_sfs_static_class_info,
&g_media_static_class_info,
    &g_sound_static_class_info,
    &g_thread_static_class_info,
    &g_image_static_class_info,
    &g_netmgr_static_class_info,
    &g_tapi_static_class_info,
    &g_heap_static_class_info
#endif
};

#ifdef DYNAMIC_MODULE_SUPPORT
/* help class */
extern const CAF_HELP_T g_caf_help_vtbl;



extern BOOLEAN MMIRES_LoadComResource(uint32 com_id);
// extern PUBLIC uint16 GUI_UCS2GB(						// return the length of gb2312_ptr
//     					uint8  *gb_ptr,			// store the gb2312 string code
// 					    const uint8  *ucs_ptr,  // the ucs2 stringcode that need to be converted.
// 					    uint16   length         //the unicode length
//     					) ;

extern int16 CAFRES_MemConfig( void );
extern void CAFRES_MemDestroy( void );
#ifdef WIN32
extern uint32 GUI_WstrToGB(						// return the length of gb2312_ptr
                           uint8  *gb_ptr,			// store the gb2312 string code
                           const wchar  *wstr_ptr,  // the ucs2 stringcode that need to be converted.
                           uint32   wstr_len         //the unicode length
                           );
extern BOOLEAN MMIAPICOM_GetFileName(const uint16 *src_name_ptr, uint16 src_name_len, uint16 *des_path_ptr, uint16 *des_path_len);
#endif
#endif

extern void MMIRES_EmptyComRes(uint32 com_id);
/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : init module memory
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMKMNG_MemInit(MMKMNG_APPLYFORMEMORY apply_func, MMKMNG_RELEASEMEMORY release_func)
{ 
    /* set memory callback function */
    SCI_ASSERT(PNULL != apply_func);/*assert verified*/
    SCI_ASSERT(PNULL != release_func);/*assert verified*/
    s_apply_mem_func = apply_func;
    s_release_mem_func = release_func;    
}

/*****************************************************************************/
// 	Description : config module memory
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC int16 MMKMNG_MemConfig(void)
{
    /* config memory */
    if(0 == s_heap_manager_id)
    {
        SCI_ASSERT(PNULL != s_apply_mem_func);/*assert verified*/
        if(s_apply_mem_func(&s_module_heap_ptr, &s_heap_size))
        {
            /* config module memory manager */
            s_heap_manager_id = MMIAPIMEM_Config((uint8*)s_module_heap_ptr, s_heap_size, NULL, 0);
            SCI_ASSERT(s_heap_manager_id > 0);/*assert verified*/
        }
        else
        {
            SCI_ASSERT(0);/*assert verified*/
        }
    }
    else
    {
        /* the memory can't be configured twice continuously */
        //SCI_ASSERT(0);
    }
    return s_heap_manager_id;
}

/*****************************************************************************/
// 	Description : malloc module memory
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC uint8 * MMKMNG_MemMalloc(uint32 size)
{ 
    uint8* ret_ptr = PNULL;

    SCI_ASSERT(s_heap_manager_id > 0);/*assert verified*/
    
    /* malloc from module memory heap */
    ret_ptr = MMIAPIMEM_Malloc(s_heap_manager_id, size);     

    SCI_ASSERT(PNULL != ret_ptr);   /*assert verified*/
    
    return ret_ptr;
}

/*****************************************************************************/
// 	Description : free module memory
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMKMNG_MemFree(uint8* pointer)
{     
    SCI_ASSERT(s_heap_manager_id > 0);    /*assert verified*/    
    /* free module memory */
    MMIAPIMEM_Free(pointer);            
}

/*****************************************************************************/
// 	Description : destroy module memory manage
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMKMNG_MemDestroy(void)
{ 
    if(0 != s_heap_manager_id)
    {
        MMIAPIMEM_Destroy(s_heap_manager_id);
        s_heap_manager_id = 0;
        /* release mmk module memory */
        SCI_ASSERT(PNULL != s_release_mem_func);/*assert verified*/
        s_release_mem_func(s_module_heap_ptr);        
    }
    else
    {
        //SCI_TRACE_LOW:"[MOD MNG]: warring! the mmk manager memory has be destroy"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODULE_MANAGER_215_112_2_18_3_23_17_22,(uint8*)"");
    }
}
#endif

/*****************************************************************************/
// 	Description : add loaded module info node to link
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void AddComModuleNode(MMK_MODULE_LOAD_T const * mod_node_ptr)
{
    if(PNULL == s_loaded_module_head)
    {
        s_loaded_module_head = (MMI_LINK_NODE_T*)MMILINK_CreateHead((uint32)mod_node_ptr);
    }
    else
    {
        s_loaded_module_head = (MMI_LINK_NODE_T*)MMILINK_AddNodeBeforeBaseNode(s_loaded_module_head, MMILINK_CreateNode((uint32)mod_node_ptr));
    }
}


/*****************************************************************************/
// 	Description : free loaded module node 
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
// LOCAL CAF_RESULT_E FreeComModuleNode(MMK_MODULE_LOAD_T * mod_node_ptr)
// {
//     SCI_ASSERT(PNULL != mod_node_ptr);    
// 
// 
//     if(1 == ((CAF_MODULE_T*)mod_node_ptr->pMod)->ref_num)
//     {
// #ifdef WIN32
//         //free dll
//         FreeLibrary( (HMODULE)mod_node_ptr->load_buf_addr );
// #else
//         /* free code buffer */
//         MMKMNG_MemFree( mod_node_ptr->load_buf_addr );
// #endif
//         mod_node_ptr->load_buf_addr = PNULL;       
//         /* free resource */
//         MMIRES_EmptyComRes(mod_node_ptr->com_id);
//     }
//     else
//     {
//         return CAF_RESULT_FAILED;
//     }
// 
//     return CAF_RESULT_SUCCEEDED;
// }

/*****************************************************************************/
// 	Description : condition for find
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchNodeCallback(MMI_LINK_NODE_T const* node_ptr, uint32 com_id, uint32 type)
{
    MMK_MODULE_LOAD_T * mod_info_ptr = PNULL;

    mod_info_ptr = (MMK_MODULE_LOAD_T*)(node_ptr->data);    

    return (BOOLEAN)(mod_info_ptr->com_id == com_id);    
}

/*****************************************************************************/
// 	Description : add dynamic resource node to link
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL MMK_MODULE_LOAD_T* SearchLoadedModule(uint32 com_id)
{
    MMI_LINK_NODE_T* found_node_ptr = PNULL;    
    if(PNULL != s_loaded_module_head)
    {        
        found_node_ptr = MMILINK_SearchNode(s_loaded_module_head, TRUE, SearchNodeCallback, com_id, 0);
    }

    if(PNULL == found_node_ptr)
    {
        return PNULL;
    }
    
    return (MMK_MODULE_LOAD_T*)(found_node_ptr->data);
}

#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : relocal code
//	Global resource dependence : none
//  Author: andrew.zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ReLocalCode(uint8 *code_ptr )
{
	uint *p1_ptr = NULL;
	uint8 *p2_ptr = NULL;
	uint  ro_size = 0;
	uint  count = 0;
	uint  offset1 = 0;
	uint  offset2 = 0;

	p1_ptr = (uint *)code_ptr;

	//SCI_TRACE_LOW:"andrewzhang:code_ptr:%d\r\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODULE_MANAGER_316_112_2_18_3_23_17_23,(uint8*)"d", (uint)p1_ptr);
	
	count = *p1_ptr++;
	ro_size = *(p1_ptr + count);
	p2_ptr = (uint8 *)(p1_ptr + count + 1);

	//SCI_TRACE_LOW:"andrewzhang:count:%d\r\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODULE_MANAGER_322_112_2_18_3_23_17_24,(uint8*)"d", count);

	while(count > 0)
	{
		offset1 = *p1_ptr;

		//全局变量原始地址
		offset2 = *((uint *)(p2_ptr + offset1));

		//SCI_TRACE_LOW:"andrewzhang:offset2:%d\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODULE_MANAGER_331_112_2_18_3_23_17_25,(uint8*)"d", offset2);
		//SCI_TRACE_LOW:"andrewzhang:ro_size:%d\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODULE_MANAGER_332_112_2_18_3_23_17_26,(uint8*)"d", ro_size);

		//全局变量重定位后的地址
		offset2 += (uint)p2_ptr;
		offset2 += ro_size;
		//设置重定位后的值
		*((uint *)(p2_ptr + offset1)) = offset2;

		//SCI_TRACE_LOW:"andrewzhang:address:%d\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODULE_MANAGER_340_112_2_18_3_23_17_27,(uint8*)"d", offset2);

		p1_ptr++;
		count--;
	}

	return TRUE;
}
#endif

/*****************************************************************************/
// 	Description : load com module from flash or file system to memory, and then 
//              create instance of module.
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E MMKMNG_LoadComModule( 
                                         uint32 com_id,
                                         IMODULE_T** mod_pptr
                                         )
{   
    MMK_MODULE_LOAD_T * loaded_mod_ptr = PNULL;
    MMI_MODINFO_NODE_T * mod_info_ptr = PNULL;
    CAF_RESULT_E        result = CAF_RESULT_SUCCEEDED;

    mod_info_ptr = MMK_SearchModInfo(com_id, MMI_QUERY_BY_MODID);

    if( PNULL == mod_info_ptr )
    {
        /* no found */
        result = CAF_RESULT_FAILED;
        return result;
    }
    
    SCI_ASSERT( PNULL != mod_pptr );/*assert verified*/
    /* got module info */
    //loaded_mod_ptr = (MMK_MODULE_LOAD_T*)MMKMNG_MemMalloc(sizeof(MMK_MODULE_LOAD_T));
    loaded_mod_ptr = (MMK_MODULE_LOAD_T*)SCI_ALLOC_APP(sizeof(MMK_MODULE_LOAD_T));
    
    if( PNULL != loaded_mod_ptr )
    {
        switch(mod_info_ptr->mod_type)
        {
        case CAF_MOD_STATIC:
            /* LOCAL module info */
            loaded_mod_ptr->com_id = com_id;
            loaded_mod_ptr->module_location_type = MMK_MODULE_ON_FLASH;
            loaded_mod_ptr->EntryFunc = mod_info_ptr->module_info.entry;     
#ifdef DYNAMIC_MODULE_SUPPORT            
            loaded_mod_ptr->code_size = 0;
            loaded_mod_ptr->load_buf_addr = PNULL;
            loaded_mod_ptr->load_buf_size = 0;
#endif
            loaded_mod_ptr->EntryFunc(
#ifdef DYNAMIC_MODULE_SUPPORT
                &g_caf_help_vtbl,
#else
                PNULL,
#endif
                &loaded_mod_ptr->pMod);            
            break;
#ifdef DYNAMIC_MODULE_SUPPORT
        case CAF_MOD_DYNAMIC:
            /* dynamic module info */
            loaded_mod_ptr->com_id = com_id;
            loaded_mod_ptr->module_location_type = MMK_MODULE_ON_FILE;
            
            MMKMNG_MemConfig();
            CAFRES_MemConfig();
#ifdef WIN32
            /* .dll for windows simulator */    
            {
                wchar       file_name[CAF_FILENAME_MAX_LEN + 1]    = {0};
                uint16      file_name_len = 0;
                uint8       name[CAF_FILENAME_MAX_LEN + 1] = {0};
                /* load dynamic library */
                HMODULE dll_handle = 0;
                
                MMIAPICOM_GetFileName( mod_info_ptr->module_info.dll_file_name, 
                    MMIAPICOM_Wstrlen( mod_info_ptr->module_info.dll_file_name ),
                    file_name, &file_name_len
                    );
                // _ModGetDevDllName(mod_info_ptr->module_info.dll_file_name, file_name, &file_name_len);

                GUI_WstrToGB( name, file_name, file_name_len );
                dll_handle = LoadLibrary((char*)name);

                /* get caf_moduleload function entry addr */
                loaded_mod_ptr->EntryFunc = (MODULEENTRY)GetProcAddress( dll_handle,"CAFMODULE_Load"); 

                loaded_mod_ptr->code_size       = 0;
                loaded_mod_ptr->load_buf_addr   = (uint8*)dll_handle;
                loaded_mod_ptr->load_buf_size   = 0;
            }
#else         
            /* .bin for ARM environment */
            {
                uint32 read_size = 0;
                uint8 align_offset = 0;
                /* open dynamic library .bin file  */
                SFS_HANDLE f_handle = SFS_CreateFile(mod_info_ptr->module_info.dll_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);/*lint !e655*/
                if(0 != f_handle)
                {
                    /* get file size */
                    SFS_GetFileSize(f_handle, &(loaded_mod_ptr->code_size));
                    /* check file size, if the size is equal to zero or belong the limit size, return error */
                    /* replace the macro MMK_DYN_MODULE_CODE_SIZE_MAX, use var s_heep_size , by wei.zhou 2009.06.22 */
                    if(loaded_mod_ptr->code_size == 0 || loaded_mod_ptr->code_size > s_heap_size)
                    {
                        //SCI_TRACE_LOW:"[MOD MNG]: error! module code size is 0x%x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODULE_MANAGER_448_112_2_18_3_23_18_28,(uint8*)"d", loaded_mod_ptr->load_buf_size);
                        return CAF_RESULT_MODU_SIZE_ERROR;
                    }
                    /* malloc a memory for store the code of dynamic library */
                    loaded_mod_ptr->load_buf_size = loaded_mod_ptr->code_size +8;
                    loaded_mod_ptr->load_buf_addr = (uint8*)MMKMNG_MemMalloc(loaded_mod_ptr->load_buf_size);
                    if(PNULL == loaded_mod_ptr->load_buf_addr)
                    {
                        //SCI_TRACE_LOW:"[MOD MNG]: error! module code no memory malloc"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODULE_MANAGER_456_112_2_18_3_23_18_29,(uint8*)"");
                        return CAF_RESULT_NO_MEMORY;
                    }
                    /* the start of code is entry function, so its addr must be align by 4 */
                    align_offset = 8 - ((uint32)(loaded_mod_ptr->load_buf_addr) % 4);
                    /* read code from .bin file to memory */
                    SFS_ReadFile(f_handle, loaded_mod_ptr->load_buf_addr+align_offset, loaded_mod_ptr->code_size, &read_size, PNULL);
                    SCI_ASSERT(read_size == loaded_mod_ptr->code_size);/*assert verified*/
                    /* close file */
                    SFS_CloseFile(f_handle);
                }

				//added by andrew.zhang at 2009/09/03
				ReLocalCode((uint8 *)(loaded_mod_ptr->load_buf_addr+align_offset));
				
				//modified by andrew.zhang at 2009/09/03				
                /* set entry */
                //loaded_mod_ptr->EntryFunc = (MODULEENTRY)(loaded_mod_ptr->load_buf_addr+align_offset+1);
                /* set entry - 4 bytes as help pointer */
                //*(uint32*)((uint8*)(loaded_mod_ptr->EntryFunc) - 4) = (uint32*)&g_caf_help_vtbl;
 				{
					uint *	p1_ptr = NULL;
					uint8 *	p2_ptr = NULL;

					p1_ptr = (uint *)(loaded_mod_ptr->load_buf_addr+align_offset);
					p2_ptr = loaded_mod_ptr->load_buf_addr+align_offset + ((*p1_ptr) + 2) * 4;

					/* set entry */
					loaded_mod_ptr->EntryFunc = (MODULEENTRY)(p2_ptr+1);/*lint !e611*/
					/* set entry - 4 bytes as help pointer */
					*(uint32*)((uint8*)(loaded_mod_ptr->EntryFunc) - 4) = (uint32)&g_caf_help_vtbl;/*lint !e611*/
				}			
			}           
#endif      
            /* invoke entry function */
            if (PNULL != loaded_mod_ptr->EntryFunc)
            {
                if(MMIRES_LoadComResource(loaded_mod_ptr->com_id))
                {
                    loaded_mod_ptr->EntryFunc( &g_caf_help_vtbl, &loaded_mod_ptr->pMod );
                }
                else
                {
                    /* load resource failed */
                    MMKMNG_MemFree(loaded_mod_ptr->load_buf_addr);
                    result = CAF_RESULT_FAILED;    
                }
            }
            else
            {
                result = CAF_RESULT_FAILED;
            }
            break;
#endif
        default:
            // 这两个是保留字段，但为了消除lint告警，这里增加，不会影响程序运行
            loaded_mod_ptr->module_location_type = MMK_MODULE_ON_IMG; 
            loaded_mod_ptr->module_location_type = MMK_MODULE_LOCATION_MAX;
            SCI_ASSERT( FALSE );/*assert verified*/
            break;
        }
    }
    else
    {
        /* no memory */
        return CAF_RESULT_NO_MEMORY;
    }
#ifdef DYNAMIC_MODULE_SUPPORT
    if(CAF_RESULT_SUCCEEDED == result)
#endif
    {
        AddComModuleNode( loaded_mod_ptr );
        *mod_pptr = loaded_mod_ptr->pMod;
    }
    return result;
}

/*****************************************************************************/
// 	Description :  create instance of interface .
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CreateStaticClassInstance(CAF_GUID_T guid, void ** object_pptr)
{    
    uint32 i = 0;
    uint32 j = 0;
    uint32 size_of_static_list = ARR_SIZE(s_static_class_info);
    
    //find LOCAL class first
    for ( i = 0; i < size_of_static_list; i++ )
    {
        for ( j = 0; j < s_static_class_info[i]->num_of_class; j++ )
        {
            if( guid == s_static_class_info[i]->class_list_ptr[j].guid )
            {
                s_static_class_info[i]->class_list_ptr[j].ClassNew( guid, object_pptr);
                
                return CAF_RESULT_SUCCEEDED;
            }
        }
    }
    return CAF_RESULT_NOT_SUPPORT;
}

/*****************************************************************************/
// 	Description :  create instance of interface .
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E MMKMNG_CreateInstance( 
                                          CAF_GUID_T guid,
                                          void ** object_pptr
                                          )
{
    CAF_RESULT_E        result         = CAF_RESULT_FAILED;
    IMODULE_T*          imodule_ptr    = {0};
    MMK_MODULE_LOAD_T*  loaded_mod_ptr = PNULL;
    MMI_MODINFO_NODE_T* mod_info_ptr   = PNULL;

    SCI_ASSERT( PNULL != object_pptr );    /*assert verified*/
        
    if( CAF_RESULT_SUCCEEDED != ( result = CreateStaticClassInstance( guid, object_pptr ) ) )
    {
        mod_info_ptr = MMK_SearchModInfo( guid, MMI_QUERY_BY_GUID );

        SCI_ASSERT( PNULL != mod_info_ptr );/*assert verified*/

        //加载前, 校验版本是否有效
        if( ( mod_info_ptr->module_info.flag & CAF_MOD_FLAG_INVALID_VER ) == CAF_MOD_FLAG_INVALID_VER )
        {           
            result = CAF_RESULT_INVALID_VERSION;
        }
        else
        {
            loaded_mod_ptr = SearchLoadedModule( mod_info_ptr->mod_id );
            
            if( PNULL == loaded_mod_ptr )
            {
                /* this module dont loaded in memory, so load it */
                result = MMKMNG_LoadComModule( mod_info_ptr->mod_id, &imodule_ptr );
            }
            else
            {
                /* platform has loaded this mode */
                imodule_ptr = loaded_mod_ptr->pMod;
                result = CAF_RESULT_SUCCEEDED;
            }
            
            if( CAF_RESULT_SUCCEEDED == result )
            {
                result = IMODULE_CreateInstance( imodule_ptr, guid, object_pptr );
            }
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description :  create static class instance
//	Global resource dependence : none
//  Author: andrew.zhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E MMKMNG_CreateStaticClassInstance( 
                                          CAF_GUID_T guid,
                                          void ** object_pptr
                                          )
{
    return CreateStaticClassInstance(guid, object_pptr);
}

/*****************************************************************************/
// 	Description : condition for find
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchModuleCallback(MMI_LINK_NODE_T const* node_ptr, uint32 condition, uint32 type)
{
    MMK_MODULE_LOAD_T * mod_node_ptr = PNULL;
    mod_node_ptr = (MMK_MODULE_LOAD_T*)(node_ptr->data);

    /* whether the handle flag contain the handle bit */    
    if(0 == IMODULE_Release( mod_node_ptr->pMod ))
    {
        /* destroy the module node from link */
        s_loaded_module_head = (MMI_LINK_NODE_T*)MMILINK_DestroyNode((MMI_LINK_NODE_T*)node_ptr, (MMI_LINK_NODE_T*)s_loaded_module_head, PNULL);        
        /* unload resource */
        MMIRES_EmptyComRes(mod_node_ptr->com_id);
#ifdef DYNAMIC_MODULE_SUPPORT
        /* free the module load buffer */
        if( PNULL != mod_node_ptr->load_buf_addr )
        {
#ifdef WIN32
            //free dll
            FreeLibrary( (HMODULE)mod_node_ptr->load_buf_addr );
#else
            /* free code buffer */
            MMKMNG_MemFree( mod_node_ptr->load_buf_addr );
#endif
            mod_node_ptr->load_buf_addr = PNULL;    
        }
#endif
        /* free the memory of the module */
        SCI_FREE(mod_node_ptr); 
    }
    else
    {
        IMODULE_AddRef( mod_node_ptr->pMod );  
    }
    return FALSE;
}

#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : condition for find
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchDynModuleCallback(MMI_LINK_NODE_T const* node_ptr, uint32 condition, uint32 type)
{
    MMK_MODULE_LOAD_T * mod_node_ptr = PNULL;
    mod_node_ptr = (MMK_MODULE_LOAD_T*)(node_ptr->data);

    /* search dynamic module */    
    if(MMK_MODULE_ON_FILE == mod_node_ptr->module_location_type)
    {
        return TRUE;
    }    
    return FALSE;
}

/*****************************************************************************/
// 	Description : dynamic module manager memory used info check
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMKMNG_MemoryUseInfoCheck(void)
{
    MMI_LINK_NODE_T* found_node_ptr = PNULL;        
    /* search the dynamic module, if no dynamic module exist, destroy memory. */
    found_node_ptr = MMILINK_SearchNode(s_loaded_module_head, TRUE, SearchDynModuleCallback, 0, 0);
    if(PNULL == found_node_ptr)
    {
        MMKMNG_MemDestroy();
        CAFRES_MemDestroy();
    }
}
#endif

/*****************************************************************************/
// 	Description : free loaded module node from link
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E	MMKMNG_ClearFreeModule(void)
{
    MMI_LINK_NODE_T* found_node_ptr = PNULL;        
    if(PNULL != s_loaded_module_head)
    {
        /* search the not used module */        
        found_node_ptr = MMILINK_SearchNode(s_loaded_module_head, TRUE, SearchModuleCallback, 0, 0);
        SCI_ASSERT(PNULL == found_node_ptr);        /*assert verified*/
#ifdef DYNAMIC_MODULE_SUPPORT
        /* dynamic module manager memory used info check */
        MMKMNG_MemoryUseInfoCheck();
#endif
        
    }
    return CAF_RESULT_SUCCEEDED;
}

/*****************************************************************************/
// 	Description : search if the module is loaded.
//	Global resource dependence : none
//    Author: peng.chen
//	Note: CAF_RESULT_SUCCEEDED is means the module has been loaded
/*****************************************************************************/
PUBLIC CAF_RESULT_E MMKMNG_SearchLoadedModule(uint32 com_id)
{
	if (PNULL != SearchLoadedModule(com_id))
	{
		return CAF_RESULT_SUCCEEDED;
	}

	return CAF_RESULT_FAILED;
}

/*****************************************************************************/
// 	Description :  whether is static class
//	Global resource dependence : none
//  Author: Andrew.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMKMNG_IsStaticClass(CAF_GUID_T guid)
{    
    uint32 i = 0;
    uint32 j = 0;
    uint32 size_of_static_list = ARR_SIZE(s_static_class_info);
    
    //find LOCAL class first
    for ( i = 0; i < size_of_static_list; i++ )
    {
        for ( j = 0; j < s_static_class_info[i]->num_of_class; j++ )
        {
            if( guid == s_static_class_info[i]->class_list_ptr[j].guid )
            {                
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

