/*****************************************************************************
 ** File Name:     msdev_param.c                                             *
 ** Author:        yifeng.wang                                               *
 ** Date:          2008/02/27                                                *
 ** Copyright:     2008 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:   interfaces for windows network interface.                 *
 *****************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                        *
 ** 2008/02/27     yifeng.wang            Create                             *
 ** 2008/03/16     yifeng.wang            Add network connection states API  *
 ** 2010/08/31     yifeng.wang            Add more convenient interfaces.    *
 *****************************************************************************/

#include "stdafx.h"
#include <windows.h>
#include "iphlpapi.h"

#include <Wininet.h>    // Def of InternetGetConnectedState()
#include <WINSOCK2.H>
#include "msnet_param.h"

/*---------------------------------------------------------------------------*
 *                         MACRO DEFINITION                                  *
 *---------------------------------------------------------------------------*/
#define FILE_NAME       "msnet_param.cpp"


/*---------------------------------------------------------------------------*
 *                         GLOBAL DEFINITION                                 *
 *---------------------------------------------------------------------------*/
LOCAL PFIXED_INFO s_pNetInfo = NULL;
LOCAL PIP_ADAPTER_INFO s_pAdaptInfo = NULL;


/*---------------------------------------------------------------------------*
 *                         FUNCTION DEFINITION                               *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 获得本地电脑的网络参数 
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
PUBLIC PFIXED_INFO MSNET_GetNetworkParams( void )
{
    DWORD dwErr = 0, dwFixedInfoSize = 0;  
    PIP_ADDR_STRING pAddrStr = NULL;

    // Free old memory
    if( NULL != s_pNetInfo )
    {
        return s_pNetInfo; 
    }

    // Get the main IP configuration information for this machine using a FIXED_INFO structure
    dwErr = GetNetworkParams( NULL, &dwFixedInfoSize );
    if( (0 != dwErr) && (ERROR_BUFFER_OVERFLOW != dwErr) )
    {
        TRACE("%s(%d) : GetNetworkParams() - failed with error %d\n", FILE_NAME, __LINE__, dwErr);
        return NULL;
    }

    // Allocate memory from sizing information
    s_pNetInfo = (PFIXED_INFO)GlobalAlloc(GPTR, dwFixedInfoSize);
    if( s_pNetInfo == NULL )
    {
        TRACE("%s(%d) : Memory allocation error\n", FILE_NAME, __LINE__);
        return NULL;
    }

    // Get network params
    TRACE("\nWindows IP Configuration\n\n");
    dwErr = GetNetworkParams(s_pNetInfo, &dwFixedInfoSize);
    if( 0 == dwErr )
    {
        PFIXED_INFO  pFixedInfo = s_pNetInfo;
        TRACE("\tHost Name . . . . . . . . . : %s\n", pFixedInfo->HostName);
        TRACE("\tPrimary DNS Suffix  . . . . : %s\n", pFixedInfo->DomainName);
        TRACE("\tDNS Servers . . . . . . . . : %s\n", pFixedInfo->DnsServerList.IpAddress.String);
        pAddrStr = pFixedInfo->DnsServerList.Next;
        while(pAddrStr)
        {
            TRACE("%46s\n", pAddrStr->IpAddress.String);
            pAddrStr = pAddrStr->Next;
        }
        
        TRACE("\tNode Type . . . . . . . . . : ");
        switch (pFixedInfo->NodeType)
        {
        case 1:     TRACE("%s\n", "Broadcast");        break;
        case 2:     TRACE("%s\n", "Peer to peer");     break;
        case 4:     TRACE("%s\n", "Mixed");            break;
        case 8:     TRACE("%s\n", "Hybrid");           break;
        default:    TRACE("%s\n", "Unknown");          break;
        }
        
        TRACE("\tNetBIOS Scope ID. . . . . . : %s\n", pFixedInfo->ScopeId);
        TRACE("\tIP Routing Enabled. . . . . : %s\n", (pFixedInfo->EnableRouting ? "Yes" : "No"));
        TRACE("\tWINS Proxy Enabled. . . . . : %s\n", (pFixedInfo->EnableProxy ? "Yes" : "No"));
        TRACE("\tNetBIOS Resolution Uses DNS : %s\n", (pFixedInfo->EnableDns ? "Yes" : "No"));
        return s_pNetInfo;
    } 
    else
    {
        TRACE("%s(%d) : GetNetworkParams failed with error %d!\n", FILE_NAME, __LINE__, dwErr);
        return NULL;
    } 
}

/*****************************************************************************/
// Description : 获得本地电脑的网络设备信息 
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
PUBLIC PIP_ADAPTER_INFO MSNET_GetAdaptersInfo( void )
{
    DWORD       dwErr = 0, dwAdapterInfoSize = 0;  
    PIP_ADDR_STRING     pAddrStr = NULL;
    PIP_ADAPTER_INFO    pAdapt = NULL;
    struct tm*  newtime = NULL;
    UINT        i = 0;

    if( NULL != s_pAdaptInfo )
    {
        return s_pAdaptInfo; 
    }

    // Enumerate all of the adapter specific information using the IP_ADAPTER_INFO structure.
    dwErr = GetAdaptersInfo(NULL, &dwAdapterInfoSize);
    if( (dwErr!=0) && (dwErr!=ERROR_BUFFER_OVERFLOW) )
    {
        TRACE("%s(%d) : GetAdaptersInfo() sizing failed with error %d!\n", FILE_NAME, __LINE__, dwErr);
        return NULL;
    }

    // Allocate memory from sizing information
    s_pAdaptInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, dwAdapterInfoSize);
    if( s_pAdaptInfo == NULL )
    {
        TRACE("%s(%d) : Memory allocation error!\n", FILE_NAME, __LINE__);
        return NULL;
    }
    
    // Get actual adapter information
    dwErr = GetAdaptersInfo(s_pAdaptInfo, &dwAdapterInfoSize);
    if( 0 != dwErr )
    {
        TRACE("%s(%d) : GetAdaptersInfo failed with error %d!\n", FILE_NAME, __LINE__, dwErr);
        return NULL;
    }
        
    // Note: IP_ADAPTER_INFO contains a linked list of adapter entries.
    pAdapt = s_pAdaptInfo;
    while( NULL != pAdapt )
    {
        switch(pAdapt->Type)
        {
        case MIB_IF_TYPE_ETHERNET:      TRACE("\nEthernet adapter ");      break;
        case MIB_IF_TYPE_TOKENRING:     TRACE("\nToken Ring adapter ");    break;
        case MIB_IF_TYPE_FDDI:          TRACE("\nFDDI adapter ");          break;
        case MIB_IF_TYPE_PPP:           TRACE("\nPPP adapter ");           break;
        case MIB_IF_TYPE_LOOPBACK:      TRACE("\nLoopback adapter ");      break;
        case MIB_IF_TYPE_SLIP:          TRACE("\nSlip adapter ");          break;
        case MIB_IF_TYPE_OTHER:        
        default:            
            TRACE("\nOther adapter ");            break;
        }
        
        TRACE("%s: \n", pAdapt->AdapterName);
        TRACE("\tDescription . . . . . . . . : %s\n", pAdapt->Description); 
        TRACE("\tPhysical Address. . . . . . : ");       
        for ( i=0; i<pAdapt->AddressLength; i++)
        {
            if (i == (pAdapt->AddressLength - 1))
                TRACE("%.2X\n",(int)pAdapt->Address[i]);
            else
                TRACE("%.2X-",(int)pAdapt->Address[i]);
        } 
        
        TRACE("\tDHCP Enabled. . . . . . . . : %s\n", (pAdapt->DhcpEnabled ? "Yes" : "No"));
        
        pAddrStr = &(pAdapt->IpAddressList);

        while(pAddrStr)
        {
            TRACE("\tIP Address. . . . . . . . . : %s\n", pAddrStr->IpAddress.String);
            TRACE("\tSubnet Mask . . . . . . . . : %s\n", pAddrStr->IpMask.String);
            pAddrStr = pAddrStr->Next;
        }
        
        TRACE("\tDefault Gateway . . . . . . : %s\n", pAdapt->GatewayList.IpAddress.String);
        pAddrStr = pAdapt->GatewayList.Next;
        
        while(pAddrStr)
        {
            TRACE("%52s\n", pAddrStr->IpAddress.String);
            pAddrStr = pAddrStr->Next;
        }
        
        TRACE("\tDHCP Server . . . . . . . . : %s\n", pAdapt->DhcpServer.IpAddress.String);
        TRACE("\tPrimary WINS Server . . . . : %s\n", pAdapt->PrimaryWinsServer.IpAddress.String);
        TRACE("\tSecondary WINS Server . . . : %s\n", pAdapt->SecondaryWinsServer.IpAddress.String);
        
        // Display coordinated universal time - GMT 
        newtime = gmtime(&pAdapt->LeaseObtained); 
        TRACE("\tLease Obtained. . . . . . . : %s", asctime(newtime));
        
        newtime = gmtime(&pAdapt->LeaseExpires); 
        TRACE("\tLease Expires . . . . . . . : %s\n", asctime(newtime));
        
        pAdapt = pAdapt->Next;
    }
    return s_pAdaptInfo;
}

/*****************************************************************************/
// Description : 获得本地电脑的默认网络设备 
// Global : 
// Author : yifeng.wang
// Note : 智能选择可用网络设备
/*****************************************************************************/
PUBLIC PIP_ADAPTER_INFO MSNET_GetDefaultAdapter( void )
{
    unsigned uIpAddr = 0;
    unsigned uHighScore = 0, uCurrScore = 0;
    PIP_ADAPTER_INFO pAdapt = NULL, pAdaptBest = NULL;
    PIP_ADDR_STRING  pAddrStr = NULL;

    if( NULL == s_pAdaptInfo )
    {
        return NULL; 
    }

    // 通过打分方式选取最优设备
    pAdapt = s_pAdaptInfo;
    while( NULL != pAdapt )
    {
        uCurrScore = 0;

        // 只选择以太网
        switch(pAdapt->Type)
        {
        case MIB_IF_TYPE_ETHERNET:
            break;
          
        default:
            pAdapt = pAdapt->Next;
            continue;
        }

        // count in DHCP flag
        uCurrScore += (pAdapt->DhcpEnabled)? 1 : 0;

        // count in IP and subnet-mask
        pAddrStr = &(pAdapt->IpAddressList);
        if( NULL != pAddrStr )
        {
            uIpAddr = inet_addr(pAddrStr->IpAddress.String);
            uCurrScore += (0 != uIpAddr)? 100 : 0;

            uIpAddr = inet_addr(pAddrStr->IpMask.String);
            uCurrScore += (0 != uIpAddr)? 100 : 0;
        }

        // count in gateway
        uIpAddr = inet_addr(pAdapt->GatewayList.IpAddress.String);
        uCurrScore += (0 != uIpAddr)? 100 : 0;
        
        // count in DHCP
        uIpAddr = inet_addr(pAdapt->DhcpServer.IpAddress.String);
        if( 0 != uIpAddr )
        {
            uCurrScore += 1;
            if( 0xFFFFFFFF != uIpAddr )
            {
                uCurrScore += 1;
            }
        }

        // record high adapter
        if( uCurrScore > uHighScore )
        {
            uHighScore = uCurrScore;
            pAdaptBest = pAdapt;
        }
        
        pAdapt = pAdapt->Next;
    }

    return pAdaptBest;
}


/*****************************************************************************/
// Description : 清除网络参数记录 
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
PUBLIC void MSNET_ReleaseNetworkParams( void )
{
    if( NULL != s_pNetInfo )
    {
        GlobalFree( s_pNetInfo );
        s_pNetInfo = NULL; 
    }
}

/*****************************************************************************/
// Description : 清除网络设备信息记录 
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
PUBLIC void MSNET_ReleaseAdaptersInfo( void )
{
    if( NULL != s_pAdaptInfo )
    {
        GlobalFree( s_pAdaptInfo );
        s_pAdaptInfo = NULL; 
    }
}

/*****************************************************************************/
// Description : 获得匹配网络设备名的网络设备信息
// Global : s_pAdaptInfo
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
PUBLIC PIP_ADAPTER_INFO MSNET_GetAdapterByName( CString strName )
{
    CString strCmp("");
    PIP_ADAPTER_INFO pAdapt = s_pAdaptInfo;

    // input check
    if( 0 == strName.GetLength() )
    {
        return NULL;
    }

    // search in adapter info list
    while( NULL != pAdapt )
    {
        strCmp.Format("%s", pAdapt->AdapterName);
        if( strCmp == strName )
        {
            break;
        }
        pAdapt = pAdapt->Next;
    }
    return pAdapt;
}

/*****************************************************************************/
// Description : 查询本地电脑的连接状态
// Global : none
// Author : yifeng.wang
// Note : 该函数只能检测系统网络设置是否连通，而不能检测物理连接，
//        而且该功能的正常工作需要系统开启 System Event Notification 服务；
//        若函数返回FALSE，网络一定不通；返回TRUE，网络不一定通
/*****************************************************************************/
BOOL MSNET_IsNetworkConnected( void )
{
    /*  CONNECTION STATUS
        INTERNET_CONNECTION_MODEM           1
        INTERNET_CONNECTION_LAN             2
        INTERNET_CONNECTION_PROXY           4
        INTERNET_CONNECTION_MODEM_BUSY      8
    */
    
    DWORD flags;          // 上网方式 
    BOOL  bOnline = TRUE; // 是否在线  
    
    bOnline = InternetGetConnectedState( &flags, 0 );   
    if( bOnline )   // 在线   
    {   
        if( flags & INTERNET_CONNECTION_MODEM )
        {
			//TRACE("MSNET_IsNetworkConnected - ONLINE : MODEM\n");
		}

		if( flags & INTERNET_CONNECTION_LAN )
		{
			//TRACE("MSNET_IsNetworkConnected - ONLINE : LAN\n");
		}

		if( flags & INTERNET_CONNECTION_PROXY )
		{
			//TRACE("MSNET_IsNetworkConnected - ONLINE : PROXY\n");
		}

		if( flags & INTERNET_CONNECTION_MODEM_BUSY)
		{
			//TRACE("MSNET_IsNetworkConnected - ONLINE : MODEM BUSY\n");
		}
    }
    else
    {
        TRACE("MSNET_IsNetworkConnected - NOT ONLINE\n");
    }
    
    return bOnline;
}
