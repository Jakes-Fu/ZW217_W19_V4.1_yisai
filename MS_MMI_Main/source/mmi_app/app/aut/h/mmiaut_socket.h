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
    //uint32                  name_index;		                                    //�������õ���������
    uint8					username[MMIAUT_LINKSETTING_DETAIL_MAX_LEN + 1];	//�û���
    uint8					password[MMIAUT_LINKSETTING_DETAIL_MAX_LEN + 1];	//����
    uint8					apn[MMIAUT_LINKSETTING_DETAIL_MAX_LEN + 1];			//�����
    uint8					gateway[MMIAUT_LINKSETTING_DETAIL_MAX_LEN + 1];		//���ص�ַ
    uint16					port;										        //�˿ں�
    uint8					homepage[MMIAUT_URL_MAX_LEN + 1];                   //��ҳ
}MMIAUT_LINKSETTING_DETAIL_T;//���������������õĽṹ
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
* �������ƣ� MMISocket_ActiveGprs
* ���������� ����pdp
* ����˵���� (IN)   ��                
*            (OUT)  ��
* �� �� ֵ�� ��
* ����˵���� ��
**************************************************************************/
PUBLIC void MMISocket_ActiveGprs(void* pUser);

/**************************************************************************
* �������ƣ� MMISocket_ActiveGprsTimeOut
* ���������� ����pdp��ʱ
* ����˵���� (IN)   ��                
*            (OUT)  ��
* �� �� ֵ�� ��
* ����˵���� ��
**************************************************************************/
PUBLIC void MMISocket_ActiveGprsTimeOut(void* pUser);

/**************************************************************************
* �������ƣ� MMISocket_ActiveGprsCallBack
* ���������� ����pdp �ص�
* ����˵���� (IN)   ��                
*            (OUT)  ��
* �� �� ֵ�� ��
* ����˵���� ��
**************************************************************************/
PUBLIC void MMISocket_ActiveGprsCallBack(MMIGPRS_STATE Msg);

/**************************************************************************
* �������ƣ� MMISocket_Init
* ���������� Init
* ����˵���� (IN)   ��                
*            (OUT)  ��
* �� �� ֵ�� 
* ����˵���� ��
**************************************************************************/
PUBLIC BOOLEAN MMISocket_Init(MN_DUAL_SYS_E active_sim);

/**************************************************************************
* �������ƣ� MMISocket_Free
* ���������� Init
* ����˵���� (IN)   ��                
*            (OUT)  ��
* �� �� ֵ�� 
* ����˵���� ��
**************************************************************************/
PUBLIC BOOLEAN MMISocket_Free(void);

/**************************************************************************
* �������ƣ� INETMGR_OpenSocket
* ���������� creat
* ����˵���� (IN)   netmgr: netmgrָ�룬��չ�ã���ʱ����PNULL
*                   type:   socket���� SOCK_STREAM/SOCK_DGRAM 
*            (OUT)  ��
* �� �� ֵ�� �ɹ��򷵻ش�����socketָ�룬���򷵻�PNULL
* ����˵���� ��
**************************************************************************/
PUBLIC MMI_ISocket* MMISocket_Open(MMI_INetMgr* netmgr, int type);

/**************************************************************************
* �������ƣ� MMISocket_SetIPAddressAndPort
* ���������� connect
* ����˵���� (IN)   ip: Ip��ַ
*                   port: �˿�
*                   address:  ip ,exam:"10.0.0.172"
*                   pUser:   ��չָ��
*            (OUT)  ��
* �� �� ֵ��
* ����˵���� ��
**************************************************************************/
PUBLIC void MMISocket_SetIPAddressAndPort(MMI_ISocket* pMe, char *address, uint16 port);

/**************************************************************************
* �������ƣ� MMISocket_Connect
* ���������� connect
* ����˵���� (IN)   ip: Ip��ַ
*                   port:   �˿�
*                   pfn:   connect call back fuc
*                   pUser:   ��չָ��
*            (OUT)  ��
* �� �� ֵ��
* ����˵���� ��
**************************************************************************/
PUBLIC void MMISocket_Connect(MMI_ISocket* pMe);

/**************************************************************************
* �������ƣ� MMISocket_GetSendBuf
* ���������� GetSendBuf
* ����˵���� (IN)   ��
*            (OUT)  send_buf
* �� �� ֵ��
* ����˵���� ��
**************************************************************************/
PUBLIC void MMISocket_GetSendBuf(char* send_buf);

/**************************************************************************
* �������ƣ� MMISocket_Read
* ���������� Read
* ����˵���� (IN)   size: read size
*            (OUT)  buf : read buf 
* �� �� ֵ��
* ����˵���� ��
**************************************************************************/
PUBLIC int	 MMISocket_Read(MMI_ISocket* pMe, char* buf, int size);

/**************************************************************************
* �������ƣ� MMISocket_Readable
* ���������� ���ö�����
* ����˵���� (IN)   pfn: read call back
*                   pUser:  ��չָ��
*            (OUT)  ��
* �� �� ֵ��
* ����˵���� ��
**************************************************************************/
PUBLIC void MMISocket_Readable(MMI_ISocket* pMe, ReadCB_Fuc pfn, void* pUser);

/**************************************************************************
* �������ƣ� MMISocket_Write
* ���������� Write
* ����˵���� (IN)   size: write size
*            (OUT)  buf : write buf 
* �� �� ֵ��
* ����˵���� ��
**************************************************************************/
PUBLIC int  MMISocket_Write(MMI_ISocket* pMe, char* buf, int size);

/**************************************************************************
* �������ƣ� MMISocket_Writeable
* ���������� ����д����
* ����˵���� (IN)   pfn: write call back
*                   pUser:  ��չָ��
*            (OUT)  ��
* �� �� ֵ�� �ɹ��򷵻ش�����socketָ�룬���򷵻�PNULL
* ����˵���� ��
**************************************************************************/
PUBLIC void MMISocket_Writeable(MMI_ISocket* pMe, WriteCB_Fuc pfn, void* pUser);

/**************************************************************************
* �������ƣ� MMISocket_Release
* ���������� Release
* ����˵���� (IN)   pMe: ��Ҫ�ͷŵ�socket
*            (OUT)  ��
* �� �� ֵ��
* ����˵���� ��
**************************************************************************/
PUBLIC void MMISocket_Release(MMI_ISocket* pMe);

/**************************************************************************
* �������ƣ� MMISocket_HandleVirWinMsg
* ���������� 
* ����˵���� (IN)   DPARAM: 
*            (OUT)  ��
* �� �� ֵ��
* ����˵���� ��
**************************************************************************/
PUBLIC MMI_RESULT_E MMISocket_HandleVirWinMsg(MMI_MESSAGE_ID_E msg_id, DPARAM   param);

/**************************************************************************
* �������ƣ� MMISocket_Insert
* ���������� ���������һ��socket
* ����˵���� (IN)   ��                
*            (OUT)  ��
* �� �� ֵ�� ��
* ����˵���� TRUE/FALSE (�ɹ�/ʧ��)
**************************************************************************/
PUBLIC BOOLEAN MMISocket_Insert(MMI_ISocket* pMe);

/**************************************************************************
* �������ƣ� MMISocket_Remove
* ���������� �������Ƴ�һ��socket
* ����˵���� (IN)   ��                
*            (OUT)  ��
* �� �� ֵ�� ��
* ����˵���� TRUE/FALSE (�ɹ�/ʧ��)
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
* �������ƣ� MMIAPIAUT_GetSocketSelectSIM
* ����˵���� (IN)   ��                
*            (OUT)  MN_DUAL_SYS_E
* �� �� ֵ�� MN_DUAL_SYS_E
* ����˵���� ��
**************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIAUT_GetSocketSelectSIM(void);

/**************************************************************************
* �������ƣ� MMIAPIAUT_SetSocketSelectSIM
* ����˵���� (IN)   sim_index                
*            (OUT)  none
* �� �� ֵ�� none
* ����˵���� ��
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
