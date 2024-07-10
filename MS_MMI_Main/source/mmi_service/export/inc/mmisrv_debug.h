/****************************************************************************
** File Name:      mmisrv_debug.h                                                              *
** Author:          Yintang.ren                                                               *
** Date:             27/04/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is to declare some definition about the MMI service module.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 27/04/2011       Yintang.ren         Create
** 
****************************************************************************/
#ifndef _MMISRV_DEBUG_H_
#define _MMISRV_DEBUG_H_

#include "os_api.h"
#include "sci_types.h"
#include "mmk_type.h"

#define MMISRV_TRACE_LOW  SCI_TRACE_LOW 
#define MMISRV_TRACE_MID  SCI_TRACE_MID
#define MMISRV_TRACE_HIGH  SCI_TRACE_LOW
//#define MMISRV_TRACE_ERR   SCI_TRACE_ERROR
#define MMISRV_TRACE_ERR   SCI_TRACE_LOW

#define MMISRV_DEBUG 

#ifdef MMISRV_DEBUG
#define _DBG(x) x
#else
#define _DBG(x)
#endif


#endif


