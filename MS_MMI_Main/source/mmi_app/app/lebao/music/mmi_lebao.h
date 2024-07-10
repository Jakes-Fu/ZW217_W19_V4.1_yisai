/**
 * @file mmi_lebao.h
 *
 */

#ifndef __MMI_LEBAO_H
#define __MMI_LEBAO_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef KAL_ON_RTT //PLATFORM_UIX8910
#endif

#include "ctrl/lebao_ctrl.h"

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
PUBLIC void MMIAPIMENU_EnterLebao(void);
PUBLIC void MMIAPIMENU_QuitLebao(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__MMI_LEBAO_H*/
