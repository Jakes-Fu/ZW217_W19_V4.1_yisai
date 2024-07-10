/****************************************************************************
** File Name:      cafmodule.c                                             *
** Author:         James.Zhang                                             *
** Date:           06/16/2009                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create                                 *
**                                                                         * 
****************************************************************************/
#define MMI_APPLET_TABLE_C

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_applet_table.h"

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get static module info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E StaticModuleEntry( const void* ph, IMODULE_T** ppMod );

/*****************************************************************************/
// 	Description : get static module info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E StaticModuleCreateInstance( IMODULE_T* pMod, CAF_GUID_T guid, void** ppObj );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get static module info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E StaticModuleEntry( const void* ph, IMODULE_T** ppMod )
{
    return CAFMODULE_New( sizeof(CAF_MODULE_T), ph, ppMod, StaticModuleCreateInstance, PNULL );
}

/*****************************************************************************/
// 	Description : get idle module info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: ÐèÒª±»mmk_modinfo.c extern
/*****************************************************************************/
void MMI_GetStaticModuleInfo( CAF_STATIC_MODULE_T* static_module_ptr )
{
    SCI_ASSERT( PNULL != static_module_ptr ); /*assert verified*/

    static_module_ptr->entry = StaticModuleEntry;
    static_module_ptr->applet_info_pptr = MMI_GetAppletInfo( &static_module_ptr->applet_info_num );
}

/*****************************************************************************/
// 	Description : get static module info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E StaticModuleCreateInstance( IMODULE_T* pMod, CAF_GUID_T guid, void** ppObj )
{
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    uint32 i = 0;
    uint32 j = 0;
    CAF_STATIC_MODULE_T static_module = {0};

    *ppObj = NULL;

    MMI_GetStaticModuleInfo( &static_module );

    for ( i = 0; i < static_module.applet_info_num; i++ )
    {
        if ( PNULL != static_module.applet_info_pptr[i] )
        {
            for ( j = 0; j < static_module.applet_info_pptr[i]->num_of_applet; j++ )
            {
                if( guid == static_module.applet_info_pptr[i]->applet_ptr[j].guid )
                {
                    result = CAFAPPLET_New( static_module.applet_info_pptr[i]->applet_ptr[j].size,
                        guid, pMod, (IAPPLET_T**)ppObj, static_module.applet_info_pptr[i]->applet_ptr[j].func, PNULL, PNULL );
                    
                    return result;
                }
            }
        }
    }
    
    return result;
}
