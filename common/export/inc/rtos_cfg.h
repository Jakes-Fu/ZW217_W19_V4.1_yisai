/******************************************************************************
 ** File Name:      rtos_cfg.h                                                *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           07/08/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain some config information on RTOS.        *
 **                 User can modify it.                                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/08/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

#ifndef _RTOS_CFG_H
#define _RTOS_CFG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/
// Use memory debug code.
#if defined(MEMORY_DEBUG_SUPPORT) || defined(PLATFORM_SC8800G) || defined(PLATFORM_SC8800H)

	#define SCI_MEMORY_DEBUG
	
#else	
 #if !defined(WIN32) && !defined(COMPILER_RVCT)
	#ifdef __MODULE__
	#undef __MODULE__
	#define   __MODULE__  ""
	#endif
	
	#ifdef __LINE__
	#undef __LINE__
	#define  __LINE__  0
	#endif
 #endif
#endif

#if defined(WIN32) || defined(TRACE_INFO_SUPPORT)
// Trace out log message.
#define SCI_TRACE_MODE   
#else
#undef  SCI_TRACE_MODE
#endif

#ifndef TRACE_INFO_SUPPORT
	//#define   __ASSERT_RELEASE_MODE__
	
	#if !defined(WIN32) && !defined(COMPILER_RVCT)
		#ifdef __FILE__
		#undef __FILE__
		#define   __FILE__   ""
		#endif
	#endif
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* End rtos_cfg.h*/
