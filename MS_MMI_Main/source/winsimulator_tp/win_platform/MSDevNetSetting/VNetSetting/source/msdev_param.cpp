/*****************************************************************************
 ** File Name:     msdev_param.cpp                                           *
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

#include <stdafx.h>
#include <WINSOCK2.H>
#include "common.h"
#include "config_file.h"
#include "file_path.h"
#include "RegOpt.h"
#include "msdev_param.h"


/*---------------------------------------------------------------------------*
 *                         MACRO DEFINITION                                  *
 *---------------------------------------------------------------------------*/
#define FILE_NAME               "msdev_param.cpp"

#define MAX_NETPARAM_NUM        5
#define MAX_SECTION_NAME_LEN    32      /* ini - max SECTION name length */

//#define MAX_SECTION_NAME_LEN    32      /* ini - max SECTION name length */
#define MIN_IP_STRING_LEN       3       /* IP string length */
#define MIN_MAC_STRING_LEN      17      /* mac string length */
#define MAX_KEY_SIZE            256     /* ini - max KEY name length */
#define INVALID_INDEX           (-1)

#define IS_NUM_C(_c)        ((_c)>='0' && (_c)<='9')        /* Number Char */
#define IS_UPP_C(_c)        ((_c)>='A' && (_c)<='Z')        /* Uppercase Char */
#define IS_LOW_C(_c)        ((_c)>='a' && (_c)<='z')        /* Lowercase Char */
#define IS_ESC_C(_c)        ((_c)>=0x07 && (_c)<=0x0d)      /* Escape Char */
#define IS_HEX_C(_c)        (IS_NUM_C(_c) || \
                            ((_c)>='A' && (_c)<='F') || \
                            ((_c)>='a' && (_c)<='f'))       /* Hex Char */


/*---------------------------------------------------------------------------*
 *                         GLOBAL DEFINITION                                 *
 *---------------------------------------------------------------------------*/
// MSDEV net parameter array
LOCAL MSDEV_NETPARAM_T s_netparam[MAX_NETPARAM_NUM] = {0};
LOCAL MSDEV_NETSTATE_T s_netstate = {0};


/*---------------------------------------------------------------------------*
 *                         LOCAL FUNCTION DECLARE                            *
 *---------------------------------------------------------------------------*/
LOCAL BOOLEAN IsSectionExist( CString strPathName, CString strSecName );
LOCAL BOOLEAN LoadNetParam( CString strPathName, CString strSecName );
LOCAL BOOLEAN LoadNetState( CString strPathName, CString strSecName );
LOCAL BOOLEAN IsValidIP( char* str_ptr );
LOCAL BOOLEAN IsValidMAC( char* str_ptr );
LOCAL BOOLEAN str2mac( char* str_ptr, uint8* mac_addr );
LOCAL BOOLEAN str2ip( char* ip_str, uint32* ipaddr );
LOCAL BOOLEAN GetIPFromFile( char* file_ptr, char* sect_ptr, char* key_ptr, uint32* ip_ptr );
LOCAL BOOLEAN GetMACFromFile( char* file_ptr, char* sect_ptr, char* key_ptr, uint8* mac_ptr, int mac_length );
LOCAL BOOLEAN GetNameStringFromFile( char* file_ptr, char* sect_ptr, char* key_ptr, char* name_ptr, int name_len );
LOCAL BOOLEAN GetValueFromFile( char* file_ptr, char* sect_ptr, char* key_ptr, int32* value_ptr );
LOCAL BOOLEAN AddToNetifList(     
    uint32 uIpAddr, uint32 uSnmask, uint32 uGateway, uint32 uDns1, uint32 uDns2,
    uint32 uDhcp, uint8* pMacAddr, char* pAdapterDesc, char* pAdapterName );


/*---------------------------------------------------------------------------*
 *                         FUNCTION DEFINITION                               *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : get net parameter pointer
// Global : s_netparam[]
// Author : yifeng.wang
// Note : RETURN: net parameter pointer
/*****************************************************************************/
PUBLIC const MSDEV_NETPARAM_T* MSDEVPARAM_GetParam( void )
{
    int i = 0;
    for( i = 0 ; i < MAX_NETPARAM_NUM ; i++ )
    {
        if( s_netparam[i].bInuse )
            break;
    }
    return &s_netparam[i];
}

/*****************************************************************************/
// Description : get first net state parameters from file
// Global : s_netstate[]
// Author : yifeng.wang
// Note :
/*****************************************************************************/
PUBLIC const MSDEV_NETSTATE_T* MSDEVPARAM_GetState( void )
{
    CString strPath("");
    if( !MSDEVPARAM_GetStatePath(strPath) )
    {
        return NULL;
    }

    if( !LoadNetState(strPath, STATE_SECTION_NAME) )
    {
        return NULL;
    }
    return &s_netstate;
}

/*****************************************************************************/
// Description : get configuration file path
// Global : none
// Author : yifeng.wang
// Note : PARAM1: strFilePath (IN/OUT) -- simulator parameter file path 
//        RETURN: TRUE - get file path succeedl; FALSE - failed
/*****************************************************************************/
PUBLIC BOOLEAN MSDEVPARAM_GetFilePath( CString& strFilePath )
{
    CString strPath;
    if( !FILEPATH_GetCurrentAppPath(strPath) )
    {
        TRACE("%s(%d) : MSDEVPARAM_GetFilePath() - FILEPATH_GetCurrentAppPath() failed\n", FILE_NAME, __LINE__);
    }
    FILEPATH_FormatPath(strPath);
    strFilePath.Format("%s%s", strPath.GetBuffer(strPath.GetLength()), CONFIG_FILE_NAME);
    return TRUE;
}

/*****************************************************************************/
// Description : get net state file path
// Global : none
// Author : yifeng.wang
// Note : PARAM1: strFilePath (IN/OUT) -- simulator net state file path 
//        RETURN: TRUE - get file path succeedl; FALSE - failed
/*****************************************************************************/
PUBLIC BOOLEAN MSDEVPARAM_GetStatePath( CString& strStatePath )
{
    CString strPath;
    if( !FILEPATH_GetCurrentAppPath(strPath) )
    {
        TRACE("%s(%d) : MSDEVPARAM_GetStatePath() - FILEPATH_GetCurrentAppPath() failed\n", FILE_NAME, __LINE__);
    }
    FILEPATH_FormatPath(strPath);
    strStatePath.Format("%s%s", strPath.GetBuffer(strPath.GetLength()), STATE_FILE_NAME);
    return FILEPATH_IsExisted(strStatePath);
}

/*****************************************************************************/
// Description : load simulator parameters from file
// Global : s_netparam[]
// Author : yifeng.wang
// Note :
/*****************************************************************************/
PUBLIC void MSDEVPARAM_LoadFromFile( void )
{
    int i = 0;
    CString strPathName, strSecName;

    // Default Initialize
    memset(s_netparam, 0, sizeof(s_netparam));

    // get configure file path
    if( !MSDEVPARAM_GetFilePath(strPathName) )
    {
        TRACE("%s(%d) : MSDEVPARAM_LoadFromFile() - MSDEVPARAM_GetFilePath() failed!\n", FILE_NAME, __LINE__);
        return;
    }

    // load configure from file
    for( i = 0 ; i < MAX_NETPARAM_NUM ; i++ )
    {
        if( 0 == i )
        {
            strSecName.Format("%s", SECTION_NAME_BASE);
        }
        else
        {
            strSecName.Format("%s%d", SECTION_NAME_BASE, i);
        }
        if( IsSectionExist(strPathName, strSecName) )
        {
            LoadNetParam(strPathName, strSecName);
        }
    }
    TRACE("%s(%d) : MSDEVPARAM_LoadFromFile() - OK!\n", FILE_NAME, __LINE__);
}

/*****************************************************************************/
// Description : MSDEV param initialize from register
// Global : none
// Author : yifeng.wang
// Note : PARAM1: mac_ptr(OUT) - given MAC pointer
//        PARAM2: mac_len(IN) - given MAC length
//        PARAM3: ip_ptr(OUT) - ip pointer
/*****************************************************************************/
PUBLIC void MSDEVPARAM_LoadFromReg( uint8* mac_ptr, uint32 mac_len, uint32* ip_ptr )
{
    // 打开注册表记录入口
    CRegOpt regOpt( HKEY_LOCAL_MACHINE );
    CString strKeyPath( REG_KEYPATH );
    BOOL bMacOk = TRUE, bIpOk = TRUE;
    CString strMac1(""), strMac2(""), strMac3(""), strMac4(""), strMac5(""), strMac6(""), strIp("");
    int i = 0;
    uint32 u32 = 0, uIp = 0;
    CTime tm = CTime::GetCurrentTime();

    if( NULL == mac_ptr || mac_len != ETHER_MAC_LEN || NULL == ip_ptr )
    {
        TRACE("%s(%d) : MSDEVPARAM_LoadFromReg() - Invalid input!\n", FILE_NAME, __LINE__);
        return;
    }

    if( !regOpt.Open(HKEY_LOCAL_MACHINE, strKeyPath) )
    {
        TRACE("%s(%d) : MSDEVPARAM_LoadFromReg() - Open key path '%s' failed!\n", FILE_NAME, __LINE__, strKeyPath);
        goto DEFAULT_INIT;
    }

    // 从注册表中读入记录
    bMacOk = bMacOk & regOpt.Read( REG_KEYNAME_VMAC1, strMac1 );
    bMacOk = bMacOk & regOpt.Read( REG_KEYNAME_VMAC2, strMac2 );
    bMacOk = bMacOk & regOpt.Read( REG_KEYNAME_VMAC3, strMac3 );
    bMacOk = bMacOk & regOpt.Read( REG_KEYNAME_VMAC4, strMac4 );
    bMacOk = bMacOk & regOpt.Read( REG_KEYNAME_VMAC5, strMac5 );
    bMacOk = bMacOk & regOpt.Read( REG_KEYNAME_VMAC6, strMac6 );
    bIpOk = regOpt.Read( REG_KEYNAME_VIP, strIp );

    if( bMacOk )
    {
        // 将字符串转换成数值类型
        char *pStop = NULL;
        mac_ptr[0] = (BYTE)strtoul( strMac1, &pStop, 16 );
        mac_ptr[1] = (BYTE)strtoul( strMac2, &pStop, 16 );
        mac_ptr[2] = (BYTE)strtoul( strMac3, &pStop, 16 );
        mac_ptr[3] = (BYTE)strtoul( strMac4, &pStop, 16 );
        mac_ptr[4] = (BYTE)strtoul( strMac5, &pStop, 16 );
        mac_ptr[5] = (BYTE)strtoul( strMac6, &pStop, 16 );
    }

    if( bIpOk )
    {
        uIp = inet_addr(strIp.GetBuffer(strIp.GetLength()));
        *ip_ptr = ntohl(uIp);
    }

    if( 0 != mac_ptr[0] || 0 != mac_ptr[1] || 0 != mac_ptr[2] ||
        0 != mac_ptr[3] || 0 != mac_ptr[4] || 0 != mac_ptr[5] )
    {
        return;
    }

DEFAULT_INIT:
    u32 = tm.GetTime();
    //TRACE("%s(%d) - current time tick %08x\n", FILE_NAME, __LINE__, u32);
    for( i = 0 ; i < ETHER_MAC_LEN ; i++ )
    {
        mac_ptr[ETHER_MAC_LEN-i-1] = (uint8)(u32 & 0xFF);
        u32 = u32 >> 8;
    }
    TRACE("%s(%d) - init MAC = '%02X:%02X:%02X:%02X:%02X:%02X'\n", FILE_NAME, __LINE__,
        mac_ptr[0], mac_ptr[1], mac_ptr[2], mac_ptr[3], mac_ptr[4], mac_ptr[5]);
}


///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
// Description : check if section name in file
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN IsSectionExist( CString strPathName, CString strSecName )
{
    char *pFilePath = NULL, *pSecName = NULL;

    // get char pointers
    pFilePath = strPathName.GetBuffer(strPathName.GetLength());
    pSecName = strSecName.GetBuffer(strSecName.GetLength());

    // check if file existed
    if( !FILEPATH_IsExisted(strPathName) )
    {
        TRACE("%s(%d) : IsSectionExist() - file %s is not existed!\n", FILE_NAME, __LINE__, pFilePath);
        return FALSE;
    }

    // check section name
    if( CFG_OK == CONFIGFILE_IsSectionExist(pFilePath, pSecName) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// Description : load network parameters from file
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN LoadNetParam( CString strPathName, CString strSecName )
{
    char*   pFilePath = NULL;
    char*   pSecName = NULL;
    uint32  uIpAddr(0), uSnmask(0), uGateway(0), uDns1(0), uDns2(0), uDhcp(0);
    uint8   szMacAddr[ETHER_MAC_LEN] = {0};
    char    szDeviceName[DEVICE_NAME_LEN] = {0};
    char    szDeviceDesc[DEVICE_DESC_LEN] = {0};
    struct in_addr addr = {0};

    // get char pointers
    pFilePath = strPathName.GetBuffer(strPathName.GetLength());
    pSecName = strSecName.GetBuffer(strSecName.GetLength());
    if( NULL == pFilePath || NULL == pSecName )
    {
        TRACE("%s(%d) : LoadNetParam() - error input, FilePath(%s), SecName(%s)\n", FILE_NAME, __LINE__, pFilePath, pSecName);
        return FALSE;
    }

    // load MAC
    if( !GetMACFromFile(pFilePath, pSecName, KEY_NAME_MAC, szMacAddr, ETHER_MAC_LEN) )
    {
        TRACE("%s(%d) : LoadNetParam() - load MAC from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load IP */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_IP, &uIpAddr) )
    {
        TRACE("%s(%d) : LoadNetParam() - load IP from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load subnet mask */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_SNMASK, &uSnmask) )
    {
        TRACE("%s(%d) : LoadNetParam() - load Subnet mask from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load Gateway */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_GATEWAY, &uGateway) )
    {
        TRACE("%s(%d) : LoadNetParam() - load Gateway from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load primary DNS */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_DNS_PRI, &uDns1) )
    {
        TRACE("%s(%d) : LoadNetParam() - load Primary DNS from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load secondary DNS */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_DNS_SEC, &uDns2) )
    {
        TRACE("%s(%d) : LoadNetParam() - load Secondary DNS from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load DHCP server */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_DHCP, &uDhcp) )
    {
        TRACE("%s(%d) : LoadNetParam() - load Dhcp from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load ethernet adapter description */
    if( !GetNameStringFromFile(pFilePath, pSecName, KEY_NAME_DEVICE_DESC, szDeviceDesc, DEVICE_NAME_LEN) )
    {
        TRACE("%s(%d) : LoadNetParam() - load Device Description from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load ethernet adapter name */
    if( !GetNameStringFromFile(pFilePath, pSecName, KEY_NAME_DEVICE_NAME, szDeviceName, DEVICE_NAME_LEN) )
    {
        TRACE("%s(%d) : LoadNetParam() - load Device Name from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* show parameters loaded from file */
    TRACE("LoadNetParam() - net parameters from section \"%s\"\n", pSecName);
    TRACE(" < MAC . . . . : %02X:%02X:%02X:%02X:%02X:%02X >\n",
        szMacAddr[0], szMacAddr[1], szMacAddr[2], szMacAddr[3], szMacAddr[4], szMacAddr[5]);
    addr.S_un.S_addr = htonl(uIpAddr);
    TRACE(" < IP  . . . . : %s >\n", inet_ntoa(addr));
    addr.S_un.S_addr = htonl(uSnmask);
    TRACE(" < SubnetMask  : %s >\n", inet_ntoa(addr));
    addr.S_un.S_addr = htonl(uGateway);
    TRACE(" < Gateway . . : %s >\n", inet_ntoa(addr));
    addr.S_un.S_addr = htonl(uDns1);
    TRACE(" < DNS Pri . . : %s >\n", inet_ntoa(addr));
    addr.S_un.S_addr = htonl(uDns2);
    TRACE(" < DNS Sec . . : %s >\n", inet_ntoa(addr));
    addr.S_un.S_addr = htonl(uDhcp);
    TRACE(" < DHCP  . . . : %s >\n", inet_ntoa(addr));
    TRACE(" < Device Desc : %s >\n", szDeviceDesc);
    TRACE(" < Device Name : %s >\n", szDeviceName);

    /* add parameters to net interface list */
    return AddToNetifList(uIpAddr, uSnmask, uGateway, uDns1, uDns2, uDhcp, szMacAddr, szDeviceDesc, szDeviceName);
}

/*****************************************************************************/
// Description : add new ethernet interface to netif list
// Global resource : s_ether_netif
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN AddToNetifList(     
    uint32  uIpAddr,
    uint32  uSnmask,
    uint32  uGateway,
    uint32  uDns1,
    uint32  uDns2,
    uint32  uDhcp,
    uint8*  pMacAddr,
    char*   pDeviceDesc,
    char*   pDeviceName )
{
    int i = 0, index = INVALID_INDEX;

    // search for empty ethernet interface
    for( i = 0 ; i < MAX_NETPARAM_NUM ; i++ )
    {
        if( FALSE == s_netparam[i].bInuse )
        {
            index = i;
            break;
        }
    }
    if( INVALID_INDEX == index )
    {
        TRACE("%s(%d) : AddToNetifList() - No free net param found!\n", FILE_NAME, __LINE__);
        return FALSE;
    }

    // set ethernet interface parameters
    s_netparam[index].bInuse = TRUE;
    s_netparam[index].uIpAddr = uIpAddr;
    s_netparam[index].uSnmask = uSnmask;
    s_netparam[index].uGateway = uGateway;
    s_netparam[index].uDns1 = uDns1;
    s_netparam[index].uDns2 = uDns2;
    s_netparam[index].uDhcp = uDhcp;
    s_netparam[index].bUseDhcp = (0==uDhcp)? FALSE:TRUE;

    memcpy(s_netparam[index].szMacAddr, pMacAddr, ETHER_MAC_LEN);
    strcpy(s_netparam[index].szDeviceDesc, pDeviceDesc);
    strcpy(s_netparam[index].szDeviceName, pDeviceName);

    return TRUE;
}

/*****************************************************************************/
// Description : load network state parameters from file
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN LoadNetState( CString strPathName, CString strSecName )
{
    char*   pFilePath = NULL;
    char*   pSecName = NULL;
    uint32  uIpAddr(0), uSnmask(0), uGateway(0), uDns1(0), uDns2(0), uDhcp(0);
    int32   iFlagDhcp(-1), iFlagRecv(-1), iFlagSend(-1), iNetState(-1), iNetid(0);
    uint8   szMacAddr[ETHER_MAC_LEN] = {0};
    char    szDeviceName[DEVICE_NAME_LEN] = {0};
    char    szDeviceDesc[DEVICE_DESC_LEN] = {0};
    struct in_addr addr = {0};

    // get char pointers
    pFilePath = strPathName.GetBuffer(strPathName.GetLength());
    pSecName = strSecName.GetBuffer(strSecName.GetLength());
    if( NULL == pFilePath || NULL == pSecName || !FILEPATH_IsExisted(strPathName) )
    {
        return FALSE;
    }

    // load MAC
    if( !GetMACFromFile(pFilePath, pSecName, KEY_NAME_MAC, szMacAddr, ETHER_MAC_LEN) )
    {
        //TRACE("%s(%d) : LoadNetState - load MAC from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load IP */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_IP, &uIpAddr) )
    {
        //TRACE("%s(%d) : LoadNetState() - load IP from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load subnet mask */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_SNMASK, &uSnmask) )
    {
        //TRACE("%s(%d) : LoadNetState() - load Subnet mask from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load Gateway */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_GATEWAY, &uGateway) )
    {
        //TRACE("%s(%d) : LoadNetState() - load Gateway from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load primary DNS */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_DNS_PRI, &uDns1) )
    {
        //TRACE("%s(%d) : LoadNetState() - load Primary DNS from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load secondary DNS */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_DNS_SEC, &uDns2) )
    {
        //TRACE("%s(%d) : LoadNetState() - load Secondary DNS from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load DHCP server */
    if( !GetIPFromFile(pFilePath, pSecName, KEY_NAME_DHCP, &uDhcp) )
    {
        //TRACE("%s(%d) : LoadNetState() - load Dhcp from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load ethernet adapter description */
    if( !GetNameStringFromFile(pFilePath, pSecName, KEY_NAME_DEVICE_DESC, szDeviceDesc, DEVICE_NAME_LEN) )
    {
        //TRACE("%s(%d) : LoadNetState() - load Device Description from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load ethernet adapter name */
    if( !GetNameStringFromFile(pFilePath, pSecName, KEY_NAME_DEVICE_NAME, szDeviceName, DEVICE_NAME_LEN) )
    {
        //TRACE("%s(%d) : LoadNetState() - load Device Name from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load ethernet state flag */
    if( !GetValueFromFile(pFilePath, pSecName, KEY_NAME_STATE_NETIF, &iNetState) )
    {
        //TRACE("%s(%d) : LoadNetState() - load state from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load netid */
    if( !GetValueFromFile(pFilePath, pSecName, KEY_NAME_NETIF_ID, &iNetid) )
    {
        //TRACE("%s(%d) : LoadNetState() - load netid from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load DHCP enable flag */
    if( !GetValueFromFile(pFilePath, pSecName, KEY_NAME_STATE_DHCP, &iFlagDhcp) )
    {
        //TRACE("%s(%d) : LoadNetState() - load DHCP flag from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load send enable flag */
    if( !GetValueFromFile(pFilePath, pSecName, KEY_NAME_STATE_SEND, &iFlagSend) )
    {
        //TRACE("%s(%d) : LoadNetState() - load send flag from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* load receive enable flag */
    if( !GetValueFromFile(pFilePath, pSecName, KEY_NAME_STATE_RECV, &iFlagRecv) )
    {
        //TRACE("%s(%d) : LoadNetState() - load receive flag from section \"%s\" failed!\n", FILE_NAME, __LINE__, pSecName);
    }

    /* show parameters loaded from file */
    //TRACE("LoadNetState() - net state from section \"%s\"\n", pSecName);

    // set ethernet state parameters
    s_netstate.uNetid = iNetid;
    s_netstate.uIpAddr = uIpAddr;
    s_netstate.uSnmask = uSnmask;
    s_netstate.uGateway = uGateway;
    s_netstate.uDns1 = uDns1;
    s_netstate.uDns2 = uDns2;
    s_netstate.uDhcp = uDhcp;
    memcpy(s_netstate.szMacAddr, szMacAddr, ETHER_MAC_LEN);
    strcpy(s_netstate.szDeviceDesc, szDeviceDesc);
    strcpy(s_netstate.szDeviceName, szDeviceName);
    s_netstate.iNetState = (int8)iNetState;
    s_netstate.iFlagDhcp = (int8)iFlagDhcp;
    s_netstate.iFlagSend = (int8)iFlagSend;
    s_netstate.iFlagRecv = (int8)iFlagRecv;

    return TRUE;
}

/*****************************************************************************/
// Description : check whether it's a valid IP string 
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN IsValidIP( char* str_ptr )
{
    int i = 0, len = 0, temp = 0, dot_num = 0;

    if( NULL == str_ptr )
    {
        return FALSE;
    }

    /* check string length */
    len = strlen(str_ptr);
    if( len < MIN_IP_STRING_LEN )
    {
//        TRACE("%s(%d) : IsValidIP() - length of IP string(%s) not long enough!\n", FILE_NAME, __LINE__, str_ptr);
        return FALSE;
    }

    /* check char type, only 0~9, escape char(/t,/r,etc.), ' '(space) & '.'(dot) is valid */
    for( i = 0 ; i < len ; i++ )
    {
        if( !(IS_NUM_C(str_ptr[i])) && !(IS_ESC_C(str_ptr[i])) && !('.'==str_ptr[i]) && !(' '==str_ptr[i]) )
        {
            TRACE("%s(%d) : IsValidIP() - invalid IP char %c exist in given IP string\n", FILE_NAME, __LINE__, str_ptr[i]);
            return FALSE;
        }
        else
        {   /* count '.' char num */
            if ( '.'==str_ptr[i] )
            {
                dot_num++;
            }
        }
    }

    /* check char '.' num */
    if ( dot_num != 3 )
    {
        TRACE("%s(%d) : IsValidIP() - there should be 3 '.' chars in IP string %s\n", FILE_NAME, __LINE__, str_ptr);
        return FALSE;
    }

    /* adjust the string with only 0 ~ 9 and '.' left */
    temp = 0;
    for( i=0; i<len; i++ )
    {
        if( (IS_NUM_C(str_ptr[i])) || ('.'==str_ptr[i]) )
        {
            str_ptr[temp++] = str_ptr[i];
        }
    } 
    str_ptr[temp] = '\0';

    /* check NUMBER char */
    {
        int  num = 0, value = 0;
        char nchar[4] = {'\0','\0','\0','\0'};
        for( i = 0; i < (int)(strlen(str_ptr)+1) ; i++ )
        {
            /* count continuous effective number */
            if( (IS_NUM_C(str_ptr[i]) && '0' != str_ptr[i]) || 
                (('0'== str_ptr[i]) && (0 != num)) )
            {
                nchar[num++] = str_ptr[i];
                if( num >= 4 )    /* not more than 3 non-zero number (0~999) maybe valid */
                {
                    TRACE("%s(%d) : IsValidIP() - each number should be 3 at the most\n", FILE_NAME, __LINE__, str_ptr);
                    return FALSE; 
                }
            }
            else if( ('.'==str_ptr[i]) || ('\0'==str_ptr[i]) )
            {
                nchar[num] = '\0';
                num = 0;
                value = atoi(nchar);
                if( value < 0 || value > 255 )
                {
                    TRACE("%s(%d) : IsValidIP() - each number should be in 0 ~ 255!\n", FILE_NAME, __LINE__, str_ptr);
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

/*****************************************************************************/
// Description : check if a given string is a valid MAC string 
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN IsValidMAC( char* str_ptr )
{
    int i = 0, len = 0, temp = 0, iso_num = 0;
    if( NULL == str_ptr )
    {
        return FALSE;
    }

    /* check string length */
    len = strlen(str_ptr);
    if( len < MIN_MAC_STRING_LEN )
    {
        TRACE("%s(%d) : IsValidMAC() - %s - string length not enough!\n", FILE_NAME, __LINE__, str_ptr);
        return FALSE;
    }

    /* only 0~9, escape char(/t,/r,etc.), '-' & ' '(space) char is valid */
    for( i = 0 ; i < len ; i++ )
    {
        if( !IS_HEX_C(str_ptr[i]) && !IS_ESC_C(str_ptr[i]) && 
            !('-'==str_ptr[i]) && !(' '==str_ptr[i]) )
        {
            TRACE("%s(%d) : IsValidMAC() - none MAC char exist in %s!\n", FILE_NAME, __LINE__, str_ptr);
            return FALSE;
        }
        else
        {   
            if( '-'==str_ptr[i] )  /* count '-' num */
                iso_num++;
        }
    }

    /* check '-' num */
    if( iso_num != 5 )
    {
        TRACE("%s(%d) : IsValidMAC() - there should be 5 '-' chars in %s!\n", FILE_NAME, __LINE__, str_ptr);
        return FALSE;
    }

    /* adjust the string with only hex chars and '-' left */
    temp = 0;
    for( i = 0 ; i < len ; i++ )
    {
        if( (IS_HEX_C(str_ptr[i])) || ('-'==str_ptr[i]) )
        {
            str_ptr[temp++] = str_ptr[i];
        }
    } 
    str_ptr[temp] = '\0';

    /* check MAC char list */
    {
        int num = 0;
        for( i = 0 ; i < (int)(strlen(str_ptr)+1) ; i++ )
        {
            /* count continuous hex number */
            if( IS_HEX_C(str_ptr[i]) )
            {
                num++;
            }
            else if( ('-'==str_ptr[i]) || ('\0'==str_ptr[i]) )
            {  
                if( 2 != num )
                {
                    TRACE("%s(%d) : IsValidMAC() - invalid MAC string %s!\n", FILE_NAME, __LINE__, str_ptr);
                    return FALSE; 
                }
                num = 0;
            }
        }
    }
    return TRUE;
}

/*****************************************************************************/
// Description : convert string to mac 
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN str2mac( char* str_ptr, uint8* mac_addr )
{
    int i, len = 0, num = 0;
    char str_sec[3] = {'\0', '\0', '\0'};
    char *str_stop;

    if( NULL == str_ptr || NULL == mac_addr )
        return FALSE;

    for( i = 0; i < (int)(strlen(str_ptr)+1) ; i++)
    {
        if( IS_HEX_C(str_ptr[i]) )
        {
            if( num < 3 )
            {
                str_sec[num++] = str_ptr[i];    /*lint !e661 */
            }
        }
        else if( ('-'==str_ptr[i]) || ('\0'==str_ptr[i]) )
        {  
            if( len < ETHER_MAC_LEN )
            {
                /* convert string to unsigned int value */  
                mac_addr[len++] = (unsigned char)strtoul(str_sec, &str_stop, 16);
            }
            num = 0;
        }    
    }
    return TRUE;
}

/*****************************************************************************/
// Description : convert string to IP 
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN str2ip( char* ip_str, uint32* ipaddr )
{
    if( NULL == ip_str || NULL == ipaddr )
        return FALSE;

    *ipaddr = inet_addr(ip_str);
    *ipaddr = ntohl(*ipaddr);
    return TRUE;
}

/*****************************************************************************/
// Description : get IP from file 
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN GetIPFromFile( 
    char* file_ptr, char* sect_ptr, char* key_ptr, uint32* ip_ptr )
{
    int  value = 0;
    char str_ip[MAX_KEY_SIZE];
    
    if( NULL == file_ptr || NULL == sect_ptr || NULL == key_ptr || NULL == ip_ptr )
    {
        return FALSE;
    }

    /* load string from configure file */
    value = CONFIGFILE_GetKeyValue( file_ptr, sect_ptr, key_ptr, str_ip );
    if( CFG_OK == value && IsValidIP(str_ip) )
    {
        return str2ip(str_ip, ip_ptr);
    }
    else
    {   
//        TRACE("%s(%d) : GetIPFromFile() - Get IP string from file failed!\n", FILE_NAME, __LINE__);
        return FALSE;
    }
}

/*****************************************************************************/
// Description : get MAC from file 
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN GetMACFromFile( 
    char* file_ptr, char* sect_ptr, char* key_ptr, uint8* mac_ptr, int mac_length )
{
    int  value = 0;
    char str_mac[MAX_KEY_SIZE];
    
    if( NULL == file_ptr || NULL == sect_ptr || NULL == key_ptr 
        || NULL == mac_ptr || ETHER_MAC_LEN != mac_length )
    {
        return FALSE;
    }

    /* load string from configure file */
    value = CONFIGFILE_GetKeyValue( file_ptr, sect_ptr, key_ptr, str_mac );
    if( CFG_OK == value && IsValidMAC(str_mac) )
    {
        return str2mac(str_mac, mac_ptr);
    }
    else
    {
        TRACE("%s(%d) : GetMACFromFile() - Get MAC string from file failed!\n", FILE_NAME, __LINE__);
        return FALSE;
    }
}

/*****************************************************************************/
// Description : get string from file
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN GetNameStringFromFile( 
    char* file_ptr, char* sect_ptr, char* key_ptr, char* name_ptr, int name_len )
{
    int value = 0;
    if( NULL == file_ptr || NULL == sect_ptr || NULL == key_ptr || 
        NULL == name_ptr || DEVICE_NAME_LEN < name_len )
    {
        return FALSE;
    }

    /* load string from configure file */
    value = CONFIGFILE_GetKeyValue( file_ptr, sect_ptr, key_ptr, name_ptr );
    if( CFG_OK == value )
    {
        return TRUE;
    }
    else
    {
        TRACE("%s(%d) : GetNameStringFromFile() - Get string from file failed!\n", FILE_NAME, __LINE__);
        return FALSE;
    }
}

/*****************************************************************************/
// Description : get key value from file 
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
LOCAL BOOLEAN GetValueFromFile( 
    char* file_ptr, char* sect_ptr, char* key_ptr, int32* value_ptr )
{
    int  value = 0;
    char str_ip[MAX_KEY_SIZE];
    
    if( NULL == file_ptr || NULL == sect_ptr || NULL == key_ptr || NULL == value_ptr )
    {
        return FALSE;
    }

    /* load string from configure file */
    value = CONFIGFILE_GetKeyValue( file_ptr, sect_ptr, key_ptr, str_ip );
    if( CFG_OK == value )
    {
        *value_ptr = atoi(str_ip);
        return TRUE;
    }
    else
    {
        TRACE("%s(%d) : GetValueFromFile() - Get string from file failed!\n", FILE_NAME, __LINE__);
        return FALSE;
    }
}
