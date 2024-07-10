/*****************************************************************************
** File Name:       tcpip_config.h
** Author:          Yifeng Wang
** Date:            2010/08/12
** Copyright:       2010 Spreadtrum, Incorporated. All Rights Reserved.      
** Description:     tcpip configurations that can be modified by user
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION                          
** 2010/08/12       Yifeng.Wang         Create
** 2010/10/21       Yifeng.Wang         Modify( add TCP window size )
** 2012/05/16       Yifeng.Wang         Remove DNS static buffer
******************************************************************************/

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "tcpip_config.h"/*lint -esym(766,tcpip_config.h)*/

#ifdef TCPIP_SUPPORT
/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
/* CONFIGURE NOTES:
   FLOWRATE_LEVEL_X - smaller X means the lower flowrate
 */

/* TCPIP_SOCKBUF_SIZE - tcpip static sockbuf size, unit: bytes */
/* used for tcpip packet buffers */
#ifdef FLOWRATE_LEVEL_3
/* recommended level3 memory size - 57900 Bytes with
 * - bigfree = 200, lilfree = 120, mbuff = 648 */
/* recommended no less than 40000 */
#define TCPIP_SOCKBUF_SIZE      173700
#endif /* FLOWRATE_LEVEL_3 */

#ifdef FLOWRATE_LEVEL_2
/* recommended level2 memory size - 29000 Bytes with
 * - bigfree = 100, lilfree = 60, mbuff = 325 */
/* recommended no less than 20000 */
#define TCPIP_SOCKBUF_SIZE      24000
#endif /* FLOWRATE_LEVEL_2 */

#ifdef FLOWRATE_LEVEL_1
/* recommended level1 memory size - 9300 Bytes with
 * - bigfree =  30, lilfree = 20, mbuff = 108 */
/* recommended no less than 8000 */
#define TCPIP_SOCKBUF_SIZE      9300
#endif /* FLOWRATE_LEVEL_1 */

/* TCP_RECV_WIN_SIZE - TCP receive window size, unit: bytes */
/* the more tcp receive window size, the more data that socket buffer can hold */
/* cr248371, increase TCP receive window to 60KB thus to improve TCP download
   capacity such as in high bandwith-delay product link. */
/* window range 16000 ~ 64000 */
#define TCP_RECV_WIN_SIZE       64000


/*user can change the stack size of tcpip_main task by this parameter*/
#define TCPIP_NET_STACK_SIZE    3072

/*user can change the stack size of tcpip_clock task by this parameter*/
#define TCPIP_CLOCK_STACK_SIZE  2560

/*user can change the stack size of ping client task by this parameter*/
#define TCPIP_PING_STACK_SIZE   1024


/*---------------------------------------------------------------------------*
 *                          TYPE DEFINITION                                  *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                          GLOBAL DEFINITION                                *
 *---------------------------------------------------------------------------*/
/* TCPIP static memory for packet buffer use */
#ifdef WIN32
#define TCPIP_SOCKBUF_SIZE      24000
#endif
LOCAL uint8 s_tcpip_sockbuf[TCPIP_SOCKBUF_SIZE];


/*---------------------------------------------------------------------------*
 *                          FUNCTION DEFINITIONS                             *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : get tcpip static memory information
// Global resource : s_tcpip_sockbuf[]
// Author : yifeng.wang
// Note : PARAM1: mem_size_ptr (OUT) -- static memory size pointer
//        RETURN: static memory pointer
/*****************************************************************************/
PUBLIC uint8* TCPIPCFG_GetSockbufInfo( uint32* mem_size_ptr )
{
    if( NULL == mem_size_ptr )
        return NULL;

    *mem_size_ptr = TCPIP_SOCKBUF_SIZE;
    return s_tcpip_sockbuf;
}

/*****************************************************************************/
// Description : get tcp receive window size
// Global resource : none
// Author : yifeng.wang
// Note : RETURN: window size (unit: bytes)
/*****************************************************************************/
PUBLIC uint32 TCPIPCFG_GetTcpRecvWindow( void )
{
    return (uint32)(TCP_RECV_WIN_SIZE);
}

/*****************************************************************************/
// Description : get tcpip net stack size
// Global resource : none
// Author : wei.chen
// Note : RETURN: net stack size (unit: bytes)
/*****************************************************************************/
PUBLIC uint32 TCPIPCFG_GetNetStackInfo( void )
{
    return (uint32)TCPIP_NET_STACK_SIZE;
}

/*****************************************************************************/
// Description : get tcpip clock stack size
// Global resource : none
// Author : wei.chen
// Note : RETURN: clock stack size (unit: bytes)
/*****************************************************************************/
PUBLIC uint32 TCPIPCFG_GetClockStackInfo( void )
{
    return (uint32)TCPIP_CLOCK_STACK_SIZE;
}

/*****************************************************************************/
// Description : get tcpip ping stack size
// Global resource : none
// Author : wei.chen
// Note : RETURN: ping stack size (unit: bytes)
/*****************************************************************************/
PUBLIC uint32 TCPIPCFG_GetPingStackInfo( void )
{
    return (uint32)TCPIP_PING_STACK_SIZE;
}

/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif /* TCPIP_SUPPORT */
