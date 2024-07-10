/******************************************************************************
 ** File Name:    chip_drv_config.h                                            *
 ** Author:       steve.zhan                                                 *
 ** DATE:         11/13/2005                                                  *
 ** Copyright:    2010  Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.zhan      Create.                                     *
 ******************************************************************************/
#ifndef _CHIP_DRV_CONFIG_H_
#define _CHIP_DRV_CONFIG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

//CHIP CONFIG
#if defined(PLATFORM_SC6800H)

//Special Chip platfrom io
#include "./sc6800h/sc6800h_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "./sc6800h/sc6800h_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "./sc6800h/sc6800h_reg_base.h"
//All chip module reg.
#include "./sc6800h/sc6800h_module_ver.h"

#define CHIP_PLATFORM_DEFINED

#endif



#if defined(PLATFORM_SC6600L)
//Special Chip platfrom io
#include "./sc6600l/sc6600l_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "./sc6600l/sc6600l_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "./sc6600l/sc6600l_reg_base.h"
//All chip module reg.
#include "./sc6600l/sc6600l_module_ver.h"

#define CHIP_PLATFORM_DEFINED
#endif


#if defined(PLATFORM_SC8800G)
//Special Chip platfrom io
#include "./sc8800g/sc8800g_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "./sc8800g/sc8800g_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "./sc8800g/sc8800g_reg_base.h"
//All chip module reg.
#include "./sc8800g/sc8800g_module_ver.h"

#define CHIP_PLATFORM_DEFINED
#endif


#if defined(PLATFORM_SC6530)

#if defined(CHIP_VER_6531)
//Special Chip platfrom io
#include "./sc6531/sc6531_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "./sc6531/sc6531_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "./sc6531/sc6531_reg_base.h"
//All chip module reg.
#include "./sc6531/sc6531_module_ver.h"

#define CHIP_PLATFORM_DEFINED

#else //CHIP_VER_6530

//Special Chip platfrom io
#include "./sc6530/sc6530_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "./sc6530/sc6530_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "./sc6530/sc6530_reg_base.h"
//All chip module reg.
#include "./sc6530/sc6530_module_ver.h"

#define CHIP_PLATFORM_DEFINED

#endif //CHIP_VER_6531
#endif

#if defined(PLATFORM_SC6531EFM)
//Special Chip platfrom io
#include "./sc6531efm/sc6531efm_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "./sc6531efm/sc6531efm_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "./sc6531efm/sc6531efm_reg_base.h"
//All chip module reg.
#include "./sc6531efm/sc6531efm_module_ver.h"

#define CHIP_PLATFORM_DEFINED

#endif

#if defined(PLATFORM_UWS6121E)
#if defined(CHIP_VER_UWS6121E)
#include "./uws6121e/global_macros.h"
//Special Chip platfrom io
#include "./uws6121e/uws6121e_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "./uws6121e/uws6121e_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "./uws6121e/uws6121e_reg_base.h"
#include "./uws6121e/globals.h"
//All chip module reg.
#include "./uws6121e/uws6121e_module_ver.h"
#endif

#define CHIP_PLATFORM_DEFINED

#endif

//End CHIP CONFIG



#if !defined(CHIP_PLATFORM_DEFINED)
//#error "chip_drv_config.h CONFIG ERROR"
#endif


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
