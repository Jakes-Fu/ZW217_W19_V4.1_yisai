/**
 * @file rtmp_demo.h
 *
 */

#ifndef __RTMP_DEMO_H
#define __RTMP_DEMO_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef KAL_ON_RTT //PLATFORM_UIX8910
#endif

#include "api/rtmp_player.h"

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void MMIAPIMENU_EnterRTMPDemo(void);
void MMIAPIMENU_QuitRTMPDemo(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__RTMP_DEMO_H*/
