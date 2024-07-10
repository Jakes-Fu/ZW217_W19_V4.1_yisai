/****************************************************************************
** File Name:      mmi_socket.h                              
** Author:                                                                  
** Date:           2010/07/01
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is mmi socket api file.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 07/01/2010        xiaoming             Create
** 
****************************************************************************/

#ifndef MMI_SOCKET_H
#define MMI_SOCKET_H

#ifdef MMI_AUTO_UPDATE_TIME_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "socket_api.h"
#include "socket_types.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiconnection_export.h"
#include "mmi_appmsg.h"
#include "mmipdp_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
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
#define AEE_NET_SUCCESS          (0)   //successful operation
#define AEE_NET_ERROR            (-1)  //unsuccessful operation
#define AEE_NET_WOULDBLOCK       (-2)

#define AEEDNSMAXADDRS           4
#define AEE_htons(a)             ((((a&0xFF00)>>8)|((a&0x00FF)<<8))&0x0000FFFF)
#define CHANGEIP(x)			     ((x>>24)&0xff)|(((x>>16)&0xff)<<8)|(((x>>8)&0xff)<<16)|((x&0xff)<<24)

#define MMIAUT_LINKSETTING_DETAIL_MAX_LEN   (30)
#define MMIAUT_URL_MAX_LEN                  (512)


typedef unsigned long MMI_INetMgr;
typedef unsigned long MMI_INAddr;


typedef void (*ConnectCB_Fuc)(void* pData, int Err);
typedef void (*WriteCB_Fuc)(void* pData);
typedef void (*ReadCB_Fuc)(void* pData);
typedef void (*TIMER_NOTIFY)(void* pUser);

typedef enum 
{
    MMI_GPRS_STATUS_ACTIVE,
    MMI_GPRS_STATUS_CONNECTED,
    MMI_GPRS_STATUS_UNCONNECT,
    MMI_GPRS_STATUS_CONNECTING,
    MMI_GPRS_STATUS_BLOCK,
    MMI_GPRS_STATUS_DEACTIVE
}MMIGPRS_STATE;

typedef enum 
{
    MMISOCKET_NOTCONNECTED,
    MMISOCKET_CONNETCTING,
    MMISOCKET_INPROGRESS,
    MMISOCKET_CONNECTED,
    MMISOCKET_EXCEPTION
}MMISOCKET_STATE;
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef struct _MMI_ISocket
{
    
    int		             socketid;
    ConnectCB_Fuc        pfnconnect;
    WriteCB_Fuc          pfnwrite;
    ReadCB_Fuc			 pfnRead;
    
    struct _MMI_ISocket* pNext;
    int			         iState;
    void* 			     pUser;
    unsigned long        ip;
    unsigned short       port;
}MMI_ISocket;

typedef struct _MMI_Sockets
{
    
    MMI_ISocket*	     head;
    int32				 size;
    MMIGPRS_STATE		 gprs_status;
    uint8                retry_cnt;
}MMI_Sockets;

typedef struct _MMI_Timer
{
    uint8	             id;
    TIMER_NOTIFY         pfn;
    void* 			     pUser;
    uint32               time_out;
    BOOLEAN              bIsLoop;
}MMI_Timer;

typedef struct
{
    //uint32                  name_index;		                                    //连接设置的名字索引
    uint8					username[MMIAUT_LINKSETTING_DETAIL_MAX_LEN + 1];	//用户名
    uint8					password[MMIAUT_LINKSETTING_DETAIL_MAX_LEN + 1];	//密码
    uint8					apn[MMIAUT_LINKSETTING_DETAIL_MAX_LEN + 1];			//接入点
    uint8					gateway[MMIAUT_LINKSETTING_DETAIL_MAX_LEN + 1];		//网关地址
    uint16					port;										        //端口号
    uint8					homepage[MMIAUT_URL_MAX_LEN + 1];                   //主页
}MMIAUT_LINKSETTING_DETAIL_T;//网络配置连接设置的结构
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**************************************************************************
* 函数名称： MMISocket_ActiveGprs
* 功能描述： 激活pdp
* 参数说明： (IN)   无                
*            (OUT)  无
* 返 回 值： 无
* 其它说明： 无
**************************************************************************/
PUBLIC void MMISocket_ActiveGprs(void* pUser);

/**************************************************************************
* 函数名称： MMISocket_ActiveGprsTimeOut
* 功能描述： 激活pdp超时
* 参数说明： (IN)   无                
*            (OUT)  无
* 返 回 值： 无
* 其它说明： 无
**************************************************************************/
PUBLIC void MMISocket_ActiveGprsTimeOut(void* pUser);

/**************************************************************************
* 函数名称： MMISocket_ActiveGprsCallBack
* 功能描述： 激活pdp 回调
* 参数说明： (IN)   无                
*            (OUT)  无
* 返 回 值： 无
* 其它说明： 无
**************************************************************************/
PUBLIC void MMISocket_ActiveGprsCallBack(MMIGPRS_STATE Msg);

/**************************************************************************
* 函数名称： MMISocket_Init
* 功能描述： Init
* 参数说明： (IN)   无                
*            (OUT)  无
* 返 回 值： 
* 其它说明： 无
**************************************************************************/
PUBLIC BOOLEAN MMISocket_Init(MN_DUAL_SYS_E active_sim);

/**************************************************************************
* 函数名称： MMISocket_Free
* 功能描述： Init
* 参数说明： (IN)   无                
*            (OUT)  无
* 返 回 值： 
* 其它说明： 无
**************************************************************************/
PUBLIC BOOLEAN MMISocket_Free(void);

/**************************************************************************
* 函数名称： INETMGR_OpenSocket
* 功能描述： creat
* 参数说明： (IN)   netmgr: netmgr指针，扩展用，暂时传入PNULL
*                   type:   socket类型 SOCK_STREAM/SOCK_DGRAM 
*            (OUT)  无
* 返 回 值： 成功则返回创建的socket指针，否则返回PNULL
* 其它说明： 无
**************************************************************************/
PUBLIC MMI_ISocket* MMISocket_Open(MMI_INetMgr* netmgr, int type);

/**************************************************************************
* 函数名称： MMISocket_SetIPAddressAndPort
* 功能描述： connect
* 参数说明： (IN)   ip: Ip地址
*                   port: 端口
*                   address:  ip ,exam:"10.0.0.172"
*                   pUser:   扩展指针
*            (OUT)  无
* 返 回 值：
* 其它说明： 无
**************************************************************************/
PUBLIC void MMISocket_SetIPAddressAndPort(MMI_ISocket* pMe, char *address, uint16 port);

/**************************************************************************
* 函数名称： MMISocket_Connect
* 功能描述： connect
* 参数说明： (IN)   ip: Ip地址
*                   port:   端口
*                   pfn:   connect call back fuc
*                   pUser:   扩展指针
*            (OUT)  无
* 返 回 值：
* 其它说明： 无
**************************************************************************/
PUBLIC void MMISocket_Connect(MMI_ISocket* pMe);

/**************************************************************************
* 函数名称： MMISocket_GetSendBuf
* 功能描述： GetSendBuf
* 参数说明： (IN)   无
*            (OUT)  send_buf
* 返 回 值：
* 其它说明： 无
**************************************************************************/
PUBLIC void MMISocket_GetSendBuf(char* send_buf);

/**************************************************************************
* 函数名称： MMISocket_Read
* 功能描述： Read
* 参数说明： (IN)   size: read size
*            (OUT)  buf : read buf 
* 返 回 值：
* 其它说明： 无
**************************************************************************/
PUBLIC int	 MMISocket_Read(MMI_ISocket* pMe, char* buf, int size);

/**************************************************************************
* 函数名称： MMISocket_Readable
* 功能描述： 设置读参数
* 参数说明： (IN)   pfn: read call back
*                   pUser:  扩展指针
*            (OUT)  无
* 返 回 值：
* 其它说明： 无
**************************************************************************/
PUBLIC void MMISocket_Readable(MMI_ISocket* pMe, ReadCB_Fuc pfn, void* pUser);

/**************************************************************************
* 函数名称： MMISocket_Write
* 功能描述： Write
* 参数说明： (IN)   size: write size
*            (OUT)  buf : write buf 
* 返 回 值：
* 其它说明： 无
**************************************************************************/
PUBLIC int  MMISocket_Write(MMI_ISocket* pMe, char* buf, int size);

/**************************************************************************
* 函数名称： MMISocket_Writeable
* 功能描述： 设置写参数
* 参数说明： (IN)   pfn: write call back
*                   pUser:  扩展指针
*            (OUT)  无
* 返 回 值： 成功则返回创建的socket指针，否则返回PNULL
* 其它说明： 无
**************************************************************************/
PUBLIC void MMISocket_Writeable(MMI_ISocket* pMe, WriteCB_Fuc pfn, void* pUser);

/**************************************************************************
* 函数名称： MMISocket_Release
* 功能描述： Release
* 参数说明： (IN)   pMe: 需要释放的socket
*            (OUT)  无
* 返 回 值：
* 其它说明： 无
**************************************************************************/
PUBLIC void MMISocket_Release(MMI_ISocket* pMe);

/**************************************************************************
* 函数名称： MMISocket_HandleVirWinMsg
* 功能描述： 
* 参数说明： (IN)   DPARAM: 
*            (OUT)  无
* 返 回 值：
* 其它说明： 无
**************************************************************************/
PUBLIC MMI_RESULT_E MMISocket_HandleVirWinMsg(MMI_MESSAGE_ID_E msg_id, DPARAM   param);

/**************************************************************************
* 函数名称： MMISocket_Insert
* 功能描述： 链表中添加一个socket
* 参数说明： (IN)   无                
*            (OUT)  无
* 返 回 值： 无
* 其它说明： TRUE/FALSE (成功/失败)
**************************************************************************/
PUBLIC BOOLEAN MMISocket_Insert(MMI_ISocket* pMe);

/**************************************************************************
* 函数名称： MMISocket_Remove
* 功能描述： 链表中移除一个socket
* 参数说明： (IN)   无                
*            (OUT)  无
* 返 回 值： 无
* 其它说明： TRUE/FALSE (成功/失败)
**************************************************************************/
PUBLIC BOOLEAN MMISocket_Remove(MMI_ISocket* pMe);

/********************************************************************************
 NAME:			MMISocket_GetAUTData
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		dave.ruan
 DATE:			
********************************************************************************/
PUBLIC void MMISocket_GetAUTData(void);

/**************************************************************************
* 函数名称： MMIAPIAUT_GetSocketSelectSIM
* 参数说明： (IN)   无                
*            (OUT)  MN_DUAL_SYS_E
* 返 回 值： MN_DUAL_SYS_E
* 其它说明： 无
**************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIAUT_GetSocketSelectSIM(void);

/**************************************************************************
* 函数名称： MMIAPIAUT_SetSocketSelectSIM
* 参数说明： (IN)   sim_index                
*            (OUT)  none
* 返 回 值： none
* 其它说明： 无
**************************************************************************/
PUBLIC void MMIAPIAUT_SetSocketSelectSIM(MN_DUAL_SYS_E sim_index);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /*MMI_AUTO_UPDATE_TIME_SUPPORT*/
#endif //MMI_SOCKET_H
