#ifndef __MET_SOCKET_SIM_H__
#define __MET_SOCKET_SIM_H__
/******************************************************************************
 * File Name:   met_socket_sim.c                                              *
 * Author:      LeoFeng                                                       *
 * DATE:        04/13/2011                                                    *
 * CopyRight:   2010 Me-Tech CORP. All Rights Reserved.                       *
 * Description: Implement Mex Platform Network Simulator(For Spreqadtrum)     *
 ******************************************************************************
 
 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/31/2010     Leo.Feng         Create.                                   *
 *****************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
**----------------------------------------------------------------------------*/
#include "MexLib.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#if (defined(MEX_PLATFORM_SIMULATOR))
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **--------------------------------------------------------------------------*/ 

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Functions                                        *
 **--------------------------------------------------------------------------*/
int    met_sock_init(void);
int    met_sock_socket(int family,int type, int proto);
int    met_sock_socketclose(int socket);
int    met_sock_bind(int socket,int addr,int size);
int    met_sock_listen(int socket,int flag);
int    met_sock_accept(int socket,int addr,int size);
int    met_sock_setsockopt(int socket,int option,int *val);
int    met_sock_getsockopt(int socket,int option,int *val);
int    met_sock_select(void *read,void *write,void *exception,int *time_wait);
int    mex_sock_getsockstate(int socket,short *state);
void   met_FD_ZERO(void *event_group);
int    met_FD_ISSET(int socket,void *event_group);
void   met_FD_SET(int socket,void *event_group);
int    mex_sock_recv(int socket, unsigned char *buf, int len, unsigned char flags);
int    mex_sock_send(int socket, unsigned char *buf, int len, unsigned char flags);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#endif  //(defined(MEX_PLATFORM_SIMULATORS))

#ifdef __cplusplus

}
#endif

#endif

