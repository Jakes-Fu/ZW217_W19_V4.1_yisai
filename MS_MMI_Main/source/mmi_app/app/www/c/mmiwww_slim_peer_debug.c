/******************************************************************************
** File Name:      mmiwww_slim_peer_debug.c                                   *
** Author:                                                                    *
** Date:           06/15/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the slim debug peer for WWW  *
**                 Browser                                                    *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create                                    *
******************************************************************************/
#define _MMIWWW_SLIM_PEER_DEBUG_C_  

/**--------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/
#include "mmi_app_www_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
#include "mmiwww_slim_peer.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIWWW_DEBUG_BUFFER_MAX (180)
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Constant Variables                               *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : Outputs debug(with linefeed)
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimDebugPrintfPeer(char *in_format, ...)
{
#ifdef SLIM_CONFIG_DEBUG
    char msg[MMIWWW_DEBUG_BUFFER_MAX + 1] = {0};
    va_list ap = PNULL;

    va_start(ap, in_format);/*lint !e718 !e64*/
    _vsnprintf(msg, MMIWWW_DEBUG_BUFFER_MAX, in_format, ap);

    //SCI_TRACE_LOW:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_DEBUG_60_112_2_18_3_11_51_124,(uint8*)"s",msg);
    va_end(ap);
#endif
}

/*****************************************************************************/
// Description : Outputs debug(without linefeed)
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimDebugPrintfWPeer(char *in_format, ...)
{
#ifdef SLIM_CONFIG_DEBUG
    char msg[MMIWWW_DEBUG_BUFFER_MAX + 1] = {0};
    va_list ap = PNULL;

    va_start(ap, in_format);/*lint !e718 !e64*/
    _vsnprintf(msg, MMIWWW_DEBUG_BUFFER_MAX, in_format, ap);
    //SCI_TRACE_LOW:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_DEBUG_77_112_2_18_3_11_51_125,(uint8*)"s",msg);
    va_end(ap);
#endif
}

/*****************************************************************************/
// Description : Gives notification that assertion failed
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimDebugAssertFailPeer(char  *in_condition,
                                    char  *in_filename,
                                    int    in_lineno,
                                    int    in_arg,
                                    char  *in_msg)
{
#ifdef SLIM_CONFIG_DEBUG
    char msg[MMIWWW_DEBUG_BUFFER_MAX + 1] = {0};
#if 0
    char *m = PNULL;
#endif

    if (PNULL == in_filename || PNULL == in_condition)
    {
        return;
    }

#if 0
    m = (in_msg) ? in_msg : "";

    _snprintf(msg, MMIWWW_DEBUG_BUFFER_MAX, "%s(%d): %s -- assertion failed.\t%s", in_filename,
              in_lineno, in_condition, m);
#else
    //SCI_TRACE_LOW:"%x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_DEBUG_109_112_2_18_3_11_51_126,(uint8*)"dddd", in_condition, in_filename, in_lineno, in_msg);
    SCI_MEMCPY(msg, in_filename, MMIWWW_DEBUG_BUFFER_MAX);
#endif

    //SCI_TRACE_LOW:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_SLIM_PEER_DEBUG_113_112_2_18_3_11_51_127,(uint8*)"s", msg);
#endif
}
#endif //#ifdef BROWSER_SUPPORT_NETFRONT