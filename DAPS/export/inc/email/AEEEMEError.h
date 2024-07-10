#ifndef _AEEEME_ERROR_H
#define _AEEEME_ERROR_H
/*=====================================================================================
FILE NAME   : AEEEMEError.h
MODULE NAME : Email

    GENERAL DESCRIPTION
      Provide AEE email's error definition.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create AEEEMEError.h
          
=====================================================================================*/


/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/
/*Include Program Global head file*/
/*Include module head file*/

/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
/* normal status */
#define EME_SUCCESS                    0x00000000                 /* No error */
#define EME_FAILED                     0x00000001                 /* General failed */
#define EME_BADPARM                    0x00000002                 /* Invalid parameter */
#define EME_ALREADY                    0x00000003                 /* Operation is already in progress */
#define EME_NOSUCH                     0x00000004                 /* No such path/service exists or valid */
#define EME_NEEDMORE                   0x00000005                 /* Need more resource for continue process */
#define EME_MEETMAX                    0x00000006
#define EME_USERCANCEL                 0x000000ff                 /* User cancel the operation */ 

/* resource errors */
#define EME_ERR_RES_START              0x00000100
#define EME_NOMEMORY                   (EME_ERR_RES_START + 0x01) /* no memory */ 
#define EME_NOSTORAGE                  (EME_ERR_RES_START + 0x02) /* no storage */
#define EME_INVALID_DATA               (EME_ERR_RES_START + 0x03) /* invalidate data */ 

#define EME_NO_NEWMAIL                 (EME_ERR_RES_START + 0x04) /* no new mail */ 
#define EME_MAIL_NOTEXSITED            (EME_ERR_RES_START + 0x05) /* specified mails not existed */ 
#define EME_SIZE_LIMIT_EXCEED          (EME_ERR_RES_START + 0x06) /* new mail exceed size limit */
#define EME_RES_NOT_FOUND              (EME_ERR_RES_START + 0x07)
#define EME_INVALID_EMAIL_ADDR         (EME_ERR_RES_START + 0x08)

/* network errors */
#define EME_ERR_NET_START              0x00000200
#define EME_CONNECT_ERROR              (EME_ERR_NET_START + 0x01) /* connect to mail server failed. */
#define EME_ACCOUNT_INVAL              (EME_ERR_NET_START + 0x02) /* account info mem is NULL */
#define EME_RAWMAIL_INVAL              (EME_ERR_NET_START + 0x03) /* raw mail mem is NULL */ 
#define EME_DNS_ERROR                  (EME_ERR_NET_START + 0x04) /* find mail server name failed. */
#define EME_LOGIN_ERROR                (EME_ERR_NET_START + 0x05) /* user name or password error. */
#define EME_COMMU_ERROR                (EME_ERR_NET_START + 0x06) /* communicate to mail server occur error. */
#define EME_NETWORK_ERROR              (EME_ERR_NET_START + 0x07) /* network abnormal. */
#define EME_FILE_ERROR                 (EME_ERR_NET_START + 0x08) /* file operation error */
#define EME_SERVER_RESPONSE_ERR        (EME_ERR_NET_START + 0x10) /* server response error information */
#define EME_MAILBOX_NOT_EXISTS         (EME_ERR_NET_START + 0x11)
#define EME_MAIL_NOT_EXISTS            (EME_ERR_NET_START + 0x12)
#define EME_SMTP_REQUEST_ABORT         (EME_ERR_NET_START + 0x13)

/* socket errors */
#define EME_ERR_SOCKET_START           0x000000400
#define EME_SOCKET_NET_ERR             (EME_ERR_SOCKET_START + 0x01)
#define EME_SOCKET_SHUTDOWN            (EME_ERR_SOCKET_START + 0x02)    /* Network is shut down */
#define EME_SOCKET_SEND_FAIL           (EME_ERR_SOCKET_START + 0x03)    /* Network error, send data failed */
#define EME_SOCKET_CANCEL              (EME_ERR_SOCKET_START + 0x04)    /* User Cancel */
#define EME_SOCKET_DNS_ERR             (EME_ERR_SOCKET_START + 0x05)    /* DNS error */
#define EME_SOCKET_OPEN_FAILED         (EME_ERR_SOCKET_START + 0x06)    /* Could not open socket */
#define EME_SOCKET_CONNECTION_ERR      (EME_ERR_SOCKET_START + 0x07)    /* Connection error */
#define EME_SOCKET_INVALID_HANDLE      (EME_ERR_SOCKET_START + 0x08)    /* Invalid Socket */
#define EME_SOCKET_INVALID_ADDR        (EME_ERR_SOCKET_START + 0x09)    /* Invalid/UnReachable Addr */
#define EME_SOCKET_INVALID_DATA        (EME_ERR_SOCKET_START + 0x0A)  

#define EME_INFO_SOCKET_START          0x000000800
#define EME_SOCKET_RECV_PARTLINE       (EME_INFO_SOCKET_START + 0x01)   
#define EME_SOCKET_RECV_ONELINE        (EME_INFO_SOCKET_START + 0x02)   
#define EME_SOCKET_RECV_NODATA         (EME_INFO_SOCKET_START + 0x03)   



/*Define Constant Macro end*/

/*Define Macro Function start*/
/*Define Macro Function end*/

/*Define ENUM type start*/
/*Define ENUM type end*/
/*---------------------------------------------------------------------
 *  Here you may want to include extra documentation on data structures
 * ------------------------------------------------------------------*/
/*Define base type start*/
/*Define base type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/
/*Define struct &  union type end*/


#endif             /* #ifndef _AEEEME_ERROR_H */
