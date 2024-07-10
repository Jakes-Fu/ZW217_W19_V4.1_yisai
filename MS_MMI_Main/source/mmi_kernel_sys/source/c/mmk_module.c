/****************************************************************************
** File Name:      mmk_module.c                                            *
** Author:         James.Zhang                                             *
** Date:           05/07/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        James.Zhang       Create
** 
****************************************************************************/
#define MMK_MODULE_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafhelp.h"
#include "mmk_modinfo.h"
/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
#ifdef DYNAMIC_MODULE_SUPPORT
extern const CAF_STDLIB_T g_caf_stdlib_vtbl;
extern const CAF_SHELL_T g_caf_shell_vtbl;

const CAF_HELP_T g_caf_help_vtbl = 
{
    &g_caf_stdlib_vtbl,
    &g_caf_shell_vtbl
};
#endif

// extern const CAF_STATIC_MODULE_T g_idle_static_module;
// 
// LOCAL const CAF_STATIC_MODULE_T* s_all_static_module_arry[] =
// {
//     &g_idle_static_module,
// };
// 
// const CAF_STATIC_MODULE_INFO_T g_caf_static_module_info = 
// {
//     s_all_static_module_arry,
//     ARR_SIZE( s_all_static_module_arry )
//};