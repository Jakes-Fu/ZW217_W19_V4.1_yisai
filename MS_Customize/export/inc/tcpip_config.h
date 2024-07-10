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

#ifndef _TCPIP_CONFIG_H_
#define _TCPIP_CONFIG_H_

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/

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


/*---------------------------------------------------------------------------*
 *                          GLOBAL DECLARATION                               *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                          FUNCTIONS                                        *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : get tcpip static sockbuf information
// Global resource : s_tcpip_sockbuf[]
// Author : yifeng.wang
// Note : PARAM1: mem_size_ptr (OUT) -- static memory size pointer
//        RETURN: static sockbuf pointer 
/*****************************************************************************/
PUBLIC uint8* TCPIPCFG_GetSockbufInfo( uint32* mem_size_ptr );

/*****************************************************************************/
// Description : get tcp receive window size
// Global resource : none
// Author : yifeng.wang
// Note : RETURN: window size (unit: bytes)
/*****************************************************************************/
PUBLIC uint32 TCPIPCFG_GetTcpRecvWindow( void );

/*****************************************************************************/
// Description : get tcpip net stack size
// Global resource : none
// Author : wei.chen
// Note : RETURN: net stack size (unit: bytes)
/*****************************************************************************/
PUBLIC uint32 TCPIPCFG_GetNetStackInfo( void );

/*****************************************************************************/
// Description : get tcpip clock stack size
// Global resource : none
// Author : wei.chen
// Note : RETURN: clock stack size (unit: bytes)
/*****************************************************************************/
PUBLIC uint32 TCPIPCFG_GetClockStackInfo( void );

/*****************************************************************************/
// Description : get tcpip ping stack size
// Global resource : none
// Author : wei.chen
// Note : RETURN: ping stack size (unit: bytes)
/*****************************************************************************/
PUBLIC uint32 TCPIPCFG_GetPingStackInfo( void );


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif

#endif  /* _TCPIP_CONFIG_H_ */