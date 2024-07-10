/****************************************************************************
** File Name:      mmk_module_manager.h                                     *
** Author:         gang.tong                                               *
** Date:           06/05/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application module manager*
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2008        gang.tong        Create
** 
****************************************************************************/

#ifndef  _MMK_MODULE_NANAGER_H_
#define  _MMK_MODULE_NANAGER_H_

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "cafmodule.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
/* apply for memory */
typedef BOOLEAN (*MMKMNG_APPLYFORMEMORY)(uint8** memory_pptr, uint32 * size_ptr);
/* release memory */
typedef void (*MMKMNG_RELEASEMEMORY)(uint8* memory_ptr);

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init module memory
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMKMNG_MemInit(MMKMNG_APPLYFORMEMORY apply_func, MMKMNG_RELEASEMEMORY release_func);
/*****************************************************************************/
// 	Description : malloc module memory
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC uint8 * MMKMNG_MemMalloc(uint32 size);
/*****************************************************************************/
// 	Description : free module memory
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMKMNG_MemFree(uint8* pointer);
/*****************************************************************************/
// 	Description : destroy module memory manage
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMKMNG_MemDestroy(void);
/*****************************************************************************/
// 	Description : load com module from flash or file system to memory, and then 
//              create instance of module.
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E MMKMNG_LoadComModule(uint32 com_id, IMODULE_T** mod_pptr);
/*****************************************************************************/
// 	Description :  create instance of interface .
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E MMKMNG_CreateInstance( CAF_GUID_T guid, void ** object_pptr );


/*****************************************************************************/
// 	Description :  create static class instance
//	Global resource dependence : none
//  Author: andrew.zhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E MMKMNG_CreateStaticClassInstance( 
                                          CAF_GUID_T guid,
                                          void ** object_pptr
                                          );


/*****************************************************************************/
// 	Description : free loaded module node from link
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E	MMKMNG_ClearFreeModule(void);
/*****************************************************************************/
// 	Description : config module memory
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC int16 MMKMNG_MemConfig(void);
/*****************************************************************************/
// 	Description : dynamic module manager memory used info check
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMKMNG_MemoryUseInfoCheck(void);

/*****************************************************************************/
// 	Description : search if the module is loaded.
//	Global resource dependence : none
//    Author: peng.chen
//	Note: CAF_RESULT_SUCCEEDED is means the module has been loaded
/*****************************************************************************/
PUBLIC CAF_RESULT_E MMKMNG_SearchLoadedModule(uint32 com_id);

/*****************************************************************************/
// 	Description :  whether is static class
//	Global resource dependence : none
//  Author: Andrew.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMKMNG_IsStaticClass(CAF_GUID_T guid);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif // _MMK_MODULEINFO_H_
