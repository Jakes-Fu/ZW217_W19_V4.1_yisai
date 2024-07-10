/*******************************************************************************
 ** FileName:      dcamera_misc.c                                              *
 ** Author:        Eddy.wei                                                    *
 ** Date:          2012/01/18                                                  *
 ** CopyRight:     2012, Spreatrum, Incoporated, All right reserved            *
 ** Description:                                                               *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2012/01/18    Eddy.wei                  Created                            *
 *******************************************************************************/

/*-----------------------------------------------------------------------------*
**                        Dependencies                                         *
**-----------------------------------------------------------------------------*/
#include "sci_api.h"
#include "dcamera_misc.h"
/**----------------------------------------------------------------------------*
**                        Compiler Flag                                        *
**-----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

 /*----------------------------------------------------------------------------*
 **                         Local Data                                         *
 **----------------------------------------------------------------------------*/
LOCAL SCI_TIMER_PTR s_dcamera_timer=PNULL;
LOCAL SCI_EVENT_GROUP_PTR s_dcamera_event=PNULL;

/**----------------------------------------------------------------------------*
 **                         Function Prototypes                                *
 **----------------------------------------------------------------------------*/

/******************************************************************************/
// Description: create dcamera timer
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_CreateTimer(void)
{
    if(NULL==s_dcamera_timer)
    {
        s_dcamera_timer=SCI_CreateTimer("DCAMERA Timer", NULL, 0, DCAMERA_WAIT_TIME, SCI_NO_ACTIVATE);
    }
}

/******************************************************************************/
// Description: delete dcamera timer
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_DeleteTimer(void)
{
    if(NULL!=s_dcamera_timer)
    {
        if(SCI_IsTimerActive(s_dcamera_timer))
        {
            SCI_DeactiveTimer(s_dcamera_timer);
        }

        SCI_PASSERT((SCI_SUCCESS==SCI_DeleteTimer(s_dcamera_timer)), ("DCAMERA: del timer fail error !"));/*assert verified*/

        s_dcamera_timer=NULL;
    }
}

/******************************************************************************/
// Description: enable dcamera timer
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      timer_expire: timer timeout expire
//                      *fun)(): function for timer
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_TimerActive(uint32 timer_expire, uint32(*fun)())
{
    if (NULL!= s_dcamera_timer)
    {
        SCI_ChangeTimer(s_dcamera_timer, fun, timer_expire);/*lint !e64*/
        SCI_ActiveTimer(s_dcamera_timer);
    }
}

/******************************************************************************/
// Description: disable dcamera timer
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_TimerDactive(void)
{    
    if(NULL!=s_dcamera_timer)
    {
        if(SCI_IsTimerActive(s_dcamera_timer))
        {
            SCI_DeactiveTimer(s_dcamera_timer);
        }
    }
}

/******************************************************************************/
// Description: dc set event
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      uint32 set_flags : event put flags
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_SetEvent(uint32 set_flags)
{
    if(PNULL!=s_dcamera_event)
    {   
        SCI_SetEvent(s_dcamera_event, set_flags, SCI_OR);
    }
}

/******************************************************************************/
// Description: Isp get event
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      uint32 requested_flags : event request flags
//                      uint32 wait_option : event wait option
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail   
// Note:
/******************************************************************************/
PUBLIC uint32 DCAMERA_GetEvent(uint32 requested_flags, uint32 wait_option)
{
    uint32 actual_event = 0;
    uint32 status = SCI_ERROR;

    if(NULL==s_dcamera_event)
    {
        s_dcamera_event=SCI_CreateEvent("DC_EVENT"); 
    }

    if(PNULL!=s_dcamera_event)
    {  
        status = SCI_GetEvent(s_dcamera_event, 
                                requested_flags, 
                                SCI_OR_CLEAR,
                                &actual_event, 
                                0);
        status = SCI_GetEvent(s_dcamera_event, 
                                requested_flags, 
                                SCI_OR_CLEAR,
                                &actual_event, 
                                wait_option);
    }

    return status;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif// End
