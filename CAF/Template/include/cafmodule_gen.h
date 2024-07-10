/****************************************************************************
** File Name:      cafmodule_gen.h                                         *
** Author:         James.Zhang                                             *
** Date:           04/15/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _CAF_MODULE_GEN_H_    
#define  _CAF_MODULE_GEN_H_  

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
#include "cafstdlib.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef CAF_RESULT_E (*MODULECREATEINST)( IMODULE_T* module_ptr, CAF_GUID_T guid, void ** object_pptr );
typedef void  (*MODULEFREEDATA)( IMODULE_T* module_ptr );

typedef struct                                     
{
	CAF_VTBL_T(IMODULE_T) *vtbl_ptr;

    uint32              ref_num;

    MODULEFREEDATA      FreeData;

}CAF_MODULE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
    
/*****************************************************************************/
// 	Description : module load
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
#ifdef WIN32
__declspec(dllexport)
#endif
CAF_RESULT_E CAFMODULE_Load( 
                            const void* help_ptr, 
                            IMODULE_T** mod_pptr 
                            );
/*****************************************************************************/
// 	Description : create instance
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
CAF_RESULT_E CAFMODULE_CreateInstance( 
                                      IMODULE_T* module_ptr,
                                      CAF_GUID_T guid,
                                      void ** object_pptr
                                      );

/*****************************************************************************/
// 	Description : module new
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E CAFMODULE_New( 
                                  uint32           size,
                                  const void       *help_ptr,
                                  IMODULE_T        **mod_pptr,
                                  MODULECREATEINST mod_cretae_func,
                                  MODULEFREEDATA   mod_free_func
                                  );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif


