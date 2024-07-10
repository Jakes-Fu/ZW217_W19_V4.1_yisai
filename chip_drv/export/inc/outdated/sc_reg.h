/******************************************************************************
 ** File Name:      sc_reg.h                                                  *
 ** Author:         Jimmy.Jia                                                 *
 ** DATE:           07/03/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                 Register header file for all spreadtrum chip                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07-03-2003     Jimmy.Jia        Create.                                   *
 ******************************************************************************/
#ifndef _SC_REG_H_
#define _SC_REG_H_
#ifdef   __cplusplus
extern   "C"
{
#endif


/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "../../chip_plf/export/inc/chip_drv_reg.h"

#ifndef WIN32
#include "../../chip_plf/export/inc/dma_drv_internal.h"
#endif

#ifdef   __cplusplus
}
#endif
#endif  //_SC_REG_H_

