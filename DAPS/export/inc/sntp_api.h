/*****************************************************************************
** File Name:      sntp_api.h                                             *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    sntp network time API                                 *
******************************************************************************/
#ifndef _SNTP_API_H_
#define _SNTP_API_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                          Enum			                                */
/*---------------------------------------------------------------------------*/

typedef enum
{
    SNTP_HD_SUCCESS,    //SNTP  HANDLE SUCCESS
    SNTP_HD_FAILED,     //SNTP  HANDLE FAILED
    SNTP_HD_INVALID_PARAM,  //SNTP INVALID PARAM
    SNTP_HD_NO_MEMERY,      //SNTP SYSTEM NO MEMERY
    SNTP_HD_DNS_ERROR,      //SNTP DNS ERROR
    SNTP_HD_SOCKET_ERROR,   //SNTP SYSTEM SOCKET ERROR
    SNTP_HD_SOCKET_SEND_ERROR,  //SNTP SYSTEM SOCKET SEND ERROR
    SNTP_HD_SOCKET_RECEIVE_ERROR,   //SNTP SYSTEM SOCKET RECEIVE ERROR
}SNTP_ERROR_E; 

/*---------------------------------------------------------------------------*/
/*                          Struct				                             */
/*---------------------------------------------------------------------------*/
typedef struct _SNTP_GET_TIME_INFO
{
    char *url_addr_ptr; //NTP SERVER URL ADDRESS
    uint32 ip_addr; //NTP SERVER IP ADDRESS
    uint32 time_zone;   //NTP SERVER TIME ZONE
    uint32  net_id;     //NTP NET ID
    BOOLEAN is_url_type;    //NTP SERVER ADDRESS TYPE ,TURE IS URL
}SNTP_GET_TIME_INFO_T;
typedef struct _SNTP_GET_TIME_SIG_REQ
{
    _SIGNAL_VARS
    uint32  ip_addr; //NTP SERVER IP ADDRESS
    uint32 time_zone;   //NTP SERVER TIME ZONE
    BOOLEAN is_url_type;    //NTP SERVER TIME ZONE
    uint32  net_id;         //NTP NET ID
    char  *url_addr_ptr;    //NTP SERVER URL ADDRESS
}SNTP_GET_TIME_SIG_REQ_T;



//SNTP NTP SIG CNF
typedef struct _SNTP_GET_TIME_SIG_CNF
{
    _SIGNAL_VARS
    uint64 net_time;       //NTP SERVER TIME 
    SNTP_ERROR_E error_code;       //HANDLE CODE
}SNTP_GET_TIME_SIG_CNF_T;


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: get ntp time request
//  LOCAL resource dependence : 
//  Author: 
//  Note: async function
/*****************************************************************************/
PUBLIC SNTP_ERROR_E SNTP_GetNTPTime(const SNTP_GET_TIME_INFO_T *net_time_ptr);

/*****************************************************************************/
//  Description : SNTP_INIT Task Mgr
//  LOCAL resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void SNTP_InitTaskMGR(void);


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/


#ifdef __cplusplus
    }
#endif


#endif //_SNTP_API_H_

