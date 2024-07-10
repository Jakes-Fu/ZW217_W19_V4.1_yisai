/******************************************************************************
 ** File Name:      DoIdleOnPage.c                                            *
 ** Author:         Lin.liu                                                   *
 ** DATE:           05/29/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    Do Idle when layer1 receive new page.                     *
 **                 do something instead of create a timer to saving capacity *
 **                 the space between page :                                  *
 **                   (2--9) x 51 x 4.615ms == (2--9) x 235ms = 470ms--2118ms *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/29/2003     Lin.liu          Create.                                   *
 ******************************************************************************/
 
#ifndef DO_IDLE_ON_PAGE_H
#define DO_IDLE_ON_PAGE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

//@Liangwen 061208
typedef uint32 (*DOIDLE_CALLBACK_FUNC)(uint32 param);

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/ 
/*****************************************************************************/
//  Description:    This function is used to register doidle callback    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN DoIdle_RegisterCallback(DOIDLE_CALLBACK_FUNC func);

/*****************************************************************************/
//  Description:    This function is used to unregister DoIdle callback    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN DoIdle_UnRegisterCallback(DOIDLE_CALLBACK_FUNC func);
//
//  Init Do Idle system. alloc resource.
//
int DoIdle_Init( void );

//
//  Release the resource.
//  
//void DoIdle_Close( void );

//
// This function be called after layer1 process one page.
//
void DoIdle_OnPage( void );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


#endif // DO_IDLE_ON_PAGE_H





