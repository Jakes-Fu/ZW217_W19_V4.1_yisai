/******************************************************************************
** File Name:      mmiwww_slim_peer_system.c                                  *
** Author:                                                                    *
** Date:           06/15/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the slim system peer for WWW *
**                 Browser                                                    *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create                                    *
******************************************************************************/
#define _MMIWWW_SLIM_PEER_SYSTEM_C_  

/**--------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
#include "mmiwww_slim_peer.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

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
// Description : Initializes system
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimSystemInitializePeer(void)
{
    PEER_DP(("slimSystemInitializePeer"));

    return;
}

/*****************************************************************************/
// Description : Finalizes system
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimSystemFinalizePeer(void)
{
    PEER_DP(("slimSystemFinalizePeer"));

    return;
}

/*****************************************************************************/
// Description : Gets Tick (elapsed time)
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int slimGetTickCountPeer(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance)
    {
        return 0;
    }

    return (slim_int)((slim_int)SCI_GetTickCount() - www_instance->suspend_tickcount);
}

/*****************************************************************************/
// Description : Suspends Tick
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimSuspendTickCountPeer(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance)
    {
        return;
    }

    PEER_DP(("slimSuspendTickCountPeer"));
    www_instance->suspend_time = slimGetTickCountPeer();
    www_instance->is_suspend = slim_true;

    return;
}

/*****************************************************************************/
// Description : Resumes Tick
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimResumeTickCountPeer(void)
{
    slim_int resTime = 0;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    PEER_DP(("slimResumeTickCountPeer"));
    if (PNULL == www_instance || !www_instance->is_suspend)
    {
        return;
    }

    resTime = slimGetTickCountPeer();
    www_instance->suspend_tickcount += (resTime - www_instance->suspend_time);
    www_instance->is_suspend = slim_false;

    return;
}

#ifdef SLIM_CONFIG_USE_IS_YIELDING_PEER
/*****************************************************************************/
// Description : Is Yielding
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool slimIsYieldingPeer(void)
{	
    return slim_false;
}
#endif

#endif//#ifdef BROWSER_SUPPORT_NETFRONT
