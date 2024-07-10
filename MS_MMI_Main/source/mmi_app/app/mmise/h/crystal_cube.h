/****************************************************************************
** File Name:      crystal_cube.h                                                	                    *
** Author:           jila                                                      		            *
** Date:           2011.04.11                                             	                            *
** Description:    This file is used to describe the tyd  public applications          *
****************************************************************************
**                         Important Edit History                         	 	*
** ------------------------------------------------------------------------*
** DATE           	        NAME             DESCRIPTION                       	*
** 2011.03.15        	jila         	
** 
****************************************************************************/
#ifndef _CRYSTAL_CUBE_H_
#define _CRYSTAL_CUBE_H_ 
 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmise_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif 


/****************************************************************************************
MARCO DEFINITION
*****************************************************************************************/

/*****************************************************************************/
// 	Description : 重置速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void CCUBE_ResetVelocityItem( void );

PUBLIC uint8* DualBuff_GetWriteBuf(DUALBUFF_INFO_T* dbuff_info_ptr);


#ifdef   __cplusplus
    }
#endif

#endif //
