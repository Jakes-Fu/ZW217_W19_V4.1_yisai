/*****************************************************************************
 ** File Name:     msdev_param.h                                             *
 ** Author:        yifeng.wang                                               *
 ** Date:          2010/08/18                                                *
 ** Copyright:     2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:   interfaces for simulator parameters.                      *
 *****************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                        *
 ** 2010/08/18     yifeng.wang            Create                             *
 *****************************************************************************/

#ifndef _MSDEV_PARAM_H_
#define _MSDEV_PARAM_H_


/*---------------------------------------------------------------------------*
 *                         MACRO DEFINITION                                  *
 *---------------------------------------------------------------------------*/
#define ETHER_MAC_LEN       6       // 网卡物理地址长度（字节）
#define DEVICE_DESC_LEN     256     // 网卡描述说明长度（字节）
#define DEVICE_NAME_LEN     128     // 网卡设备名长度（字节）


/*---------------------------------------------------------------------------*
 *                         TYPE DEFINITION                                   *
 *---------------------------------------------------------------------------*/
// net loading param
typedef struct msdev_netparam_tag
{
    BOOLEAN bInuse;
    uint32  uNetid;
    uint32  uIpAddr;
    uint32  uSnmask;
    uint32  uGateway;
    uint32  uDns1;
    uint32  uDns2;
    uint32  uDhcp;
    BOOLEAN bUseDhcp;
    BOOLEAN bDeviceOpen;
    uint8   szMacAddr[ETHER_MAC_LEN];
    char    szDeviceDesc[DEVICE_DESC_LEN];
    char    szDeviceName[DEVICE_NAME_LEN];
} MSDEV_NETPARAM_T;

// net state param
typedef struct msdev_netstate_tag
{
    uint32  uNetid;
    uint32  uIpAddr;
    uint32  uSnmask;
    uint32  uGateway;
    uint32  uDns1;
    uint32  uDns2;
    uint32  uDhcp;
    uint8   szMacAddr[ETHER_MAC_LEN];
    char    szDeviceDesc[DEVICE_DESC_LEN];
    char    szDeviceName[DEVICE_NAME_LEN];
    int8    iNetState;
    int8    iFlagDhcp;
    int8    iFlagSend;
    int8    iFlagRecv;
} MSDEV_NETSTATE_T;

/*---------------------------------------------------------------------------*
 *                         FUNCTION DECLARE                                  *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : get configuration file path
// Global : none
// Author : yifeng.wang
// Note : PARAM1: strFilePath (IN/OUT) -- simulator parameter file path 
//        RETURN: TRUE - get file path succeedl; FALSE - failed
/*****************************************************************************/
PUBLIC BOOLEAN MSDEVPARAM_GetFilePath( CString& strFilePath );

/*****************************************************************************/
// Description : get net state file path
// Global : none
// Author : yifeng.wang
// Note : PARAM1: strFilePath (IN/OUT) -- simulator net state file path 
//        RETURN: TRUE - get file path succeedl; FALSE - failed
/*****************************************************************************/
PUBLIC BOOLEAN MSDEVPARAM_GetStatePath( CString& strFilePath );

/*****************************************************************************/
// Description : load simulator parameters from file
// Global : s_netparam[]
// Author : yifeng.wang
// Note :
/*****************************************************************************/
PUBLIC void MSDEVPARAM_LoadFromFile( void );

/*****************************************************************************/
// Description : get net parameter pointer
// Global : s_netparam[]
// Author : yifeng.wang
// Note : RETURN: net parameter pointer
/*****************************************************************************/
PUBLIC const MSDEV_NETPARAM_T* MSDEVPARAM_GetParam( void );

/*****************************************************************************/
// Description : MSDEV param initialize from register
// Global : none
// Author : yifeng.wang
// Note : PARAM1: mac_ptr(OUT) - given MAC pointer
//        PARAM2: mac_len(IN) - given MAC length
//        PARAM3: ip_ptr(OUT) - ip pointer
/*****************************************************************************/
PUBLIC void MSDEVPARAM_LoadFromReg( uint8* mac_ptr, uint32 mac_len, uint32* ip_ptr );

/*****************************************************************************/
// Description : get first net state parameters from file
// Global : s_netstate[]
// Author : yifeng.wang
// Note :
/*****************************************************************************/
PUBLIC const MSDEV_NETSTATE_T* MSDEVPARAM_GetState( void );


///////////////////////////////////////////////////////////////////////////////
#endif /* _MSDEV_PARAM_H_ */