/*****************************************************************************
** File Name:      mmiwifi_profile.c                                         *
** Author:         li.li                                                     *
** Date:           2009.11.13                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009.11.13     li.li            Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_wlan_trc.h"
#include "wifisupp_api.h"
#include "sci_types.h"
#include "cfl_wchar.h"
#include "mmiwlan_profile.h"
#include "mmiwlan_nv.h"
#include "cfl_list.h"
#include "mmifmm_export.h"
#include "mmiwifi_export.h"
#include "mmicom_trace.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWIFI_AP_CMCC     "CMCC"
#define MMIWIFI_AP_CMCC_EDU     "CMCC-EDU"
/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL CFL_LIST_HANDLE s_mmiwifi_list = 0;

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
/****************************************************************************/
//  Description : find wifi node by index
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL CFL_LIST_NODE_HANDLE WifiFindNodeByIndex(MMIWIFI_INDEX_T index);

/****************************************************************************/
//  Description : free wifi profile
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiFreeProfile(MMIWIFI_PROFILE_T* profile_ptr);

/****************************************************************************/
//  Description : Create wifi list
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiCreateList(void);

/****************************************************************************/
//  Description : destroy wifi list
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiDestroyList(void);

/****************************************************************************/
//  Description : get wifi device sys name
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC uint16* WifiGetSysDevName(uint16 *device_name_len_ptr);

/****************************************************************************/
//  Description : Equal Status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN mmiwifi_EqualWifiStatus(CFL_NODE_OBJECT data, void *find_data);

/****************************************************************************/
//  Description : Equal AP
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN mmiwifi_EqualWifiAP(CFL_NODE_OBJECT data, void *find_data);

/****************************************************************************/
//  Description : Equal On air
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN mmiwifi_EqualOnAir(CFL_NODE_OBJECT data, void *find_data);

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : initialize the wifi profile list
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ProfileInitialize(void)
{
    SFS_HANDLE          file_handle = 0;
    BOOLEAN             result = TRUE;
    uint16*             device_name_ptr=PNULL;
    uint16              device_name_len=0;
    uint16              file_name[MMIWIFI_MAX_FILE_NAME_LEN + 1] = {0};
    MMIWIFI_PROFILE_T   profile = {0};
    int32               read_len = 0;
    int32               buf_len = 0;
#ifdef  WLAN_PORTAL_SUPPORT    
    BOOLEAN         is_have_CMCC = FALSE;
    BOOLEAN         is_have_CMCC_EDU = FALSE;
#endif
    //create wifi list
    if(!WifiCreateList())
    {
        result = FALSE;
    }
    else
    {
        //find device name
        device_name_ptr = WifiGetSysDevName(&device_name_len);
        //create and read wifi profile from wifi file
        if(PNULL != device_name_ptr)
        {
            CFLWCHAR_StrNCat(file_name,device_name_ptr, MMIWIFI_MAX_FILE_NAME_LEN + 1);
            CFLWCHAR_StrNCatWithAsciiStr(file_name,MMIWIFI_FILE, MMIWIFI_MAX_FILE_NAME_LEN + 1);
            file_handle = SFS_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
            if(0 != file_handle)
            {
                if (SFS_ERROR_NONE == SFS_GetFileSize(file_handle,(uint32*)(&buf_len)) && buf_len > 0)
                {
                    do{
                        if(SFS_ERROR_NONE == SFS_ReadFile(file_handle, (char*)&profile, sizeof(MMIWIFI_PROFILE_T), (uint32*)(&read_len), PNULL))
                        {
                            //add wifi profile to wifi list
                            if ((uint32)read_len == sizeof(MMIWIFI_PROFILE_T))
                            {
                                profile.state = MMIWIFI_PROFILE_STATE_DISCONNECTED;
                                profile.is_dhcp_success = FALSE;
                                profile.is_on_air = FALSE;
                                profile.is_trusted = TRUE;
                                MMIWIFI_AddProfileToList(&profile);
                                buf_len -= read_len;
#ifdef WLAN_PORTAL_SUPPORT    
                                //初始化时，确保信任列表中有CMCC和CMCC-EDU项
                                if(!is_have_CMCC && MMIWIFI_APIsCMCC(&profile))
                                {
                                    is_have_CMCC = TRUE;
                                }
                                if(!is_have_CMCC_EDU && MMIWIFI_APIsCMCCEDU(&profile))
                                {
                                    is_have_CMCC_EDU = TRUE;
                                }
#endif
                            }
                            else
                            {
                                SFS_CloseFile(file_handle);
                                break;
                            }
                        }
                        else
                        {
                            SFS_CloseFile(file_handle);
                            break;
                        }
                    }while(buf_len > 0);
                }
                else
                {
                    SFS_CloseFile(file_handle);
                }
            }
            SFS_CloseFile(file_handle);
        }

        result = TRUE;
    }
#ifdef WLAN_PORTAL_SUPPORT    
    if(!is_have_CMCC)
    {
        SCI_MEMSET(&profile, 0, sizeof(profile));
        profile.config_info.ssid.ssid_len = strlen((char *)MMIWIFI_AP_CMCC);
        SCI_MEMCPY(profile.config_info.ssid.ssid, MMIWIFI_AP_CMCC, profile.config_info.ssid.ssid_len);
        profile.is_ssid_hide = FALSE;
        profile.config_info.network_mode = WIFISUPP_NETWORK_MODE_INFRASTRUCTURE;
        profile.config_info.encryp_protocol = WIFISUPP_ENCRYP_PROTOCOL_OPENSYS;
        profile.is_auto_connect= 0;
        profile.state = MMIWIFI_PROFILE_STATE_DISCONNECTED;
        profile.is_dhcp_success = FALSE;
        profile.is_on_air = FALSE;
        profile.is_trusted = TRUE;
        profile.is_auto_connect = TRUE;

        if (MMIWIFI_AddProfileToList(&profile))
        {
            MMIWIFI_UpdateProfileFile();
        }
    }
    if(!is_have_CMCC_EDU)
    {
        SCI_MEMSET(&profile, 0, sizeof(profile));
        profile.config_info.ssid.ssid_len = strlen((char *)MMIWIFI_AP_CMCC_EDU);
        SCI_MEMCPY(profile.config_info.ssid.ssid, MMIWIFI_AP_CMCC_EDU, profile.config_info.ssid.ssid_len);
        profile.is_ssid_hide = FALSE;
        profile.config_info.network_mode = WIFISUPP_NETWORK_MODE_INFRASTRUCTURE;
        profile.config_info.encryp_protocol = WIFISUPP_ENCRYP_PROTOCOL_OPENSYS;
        profile.is_auto_connect= 0;
        profile.state = MMIWIFI_PROFILE_STATE_DISCONNECTED;
        profile.is_dhcp_success = FALSE;
        profile.is_on_air = FALSE;
        profile.is_trusted = TRUE;
        profile.is_auto_connect = TRUE;

        if (MMIWIFI_AddProfileToList(&profile))
        {
            MMIWIFI_UpdateProfileFile();
        }
    }
#endif
    return result;
}

/****************************************************************************/
//  Description : finalize the wifi profile list
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ProfileFinalize(void)
{
    uint32              i = 0;
    SFS_HANDLE          file_handle = 0;
    uint16*             device_name_ptr=PNULL;
    uint16              device_name_len=0;
    uint16              file_name[MMIWIFI_MAX_FILE_NAME_LEN + 1] = {0};
    MMIWIFI_PROFILE_T*  profile_ptr = PNULL;
    uint32              bytes_writen = 0;
    uint32              num = MMIWIFI_GetProfileNum();
    
    device_name_ptr = WifiGetSysDevName(&device_name_len);
    //create and save wifi profile to wifi file
    if(PNULL != device_name_ptr)
    {
        CFLWCHAR_StrNCat(file_name,device_name_ptr, MMIWIFI_MAX_FILE_NAME_LEN + 1);
        CFLWCHAR_StrNCatWithAsciiStr(file_name,MMIWIFI_FILE, MMIWIFI_MAX_FILE_NAME_LEN + 1);
        file_handle = SFS_CreateFile(file_name, SFS_MODE_WRITE| SFS_MODE_CREATE_ALWAYS, PNULL, PNULL);
        if(0 == file_handle)
        {
            WifiDestroyList();
            return FALSE;
        }
    }
    if(0 != num)
    {
        for(i = 0; i<num; i++)
        {
            profile_ptr = MMIWIFI_GetProfileByIndex(i);
            if(PNULL != profile_ptr && profile_ptr->is_trusted)
            {
                //save wifi profile to wifi file
                if (SFS_NO_ERROR != SFS_WriteFile(file_handle,profile_ptr,sizeof(MMIWIFI_PROFILE_T),&bytes_writen,PNULL))
                {
                    SFS_CloseFile(file_handle);
                    SFS_DeleteFile(file_name,PNULL);
                    WifiDestroyList();
                    return FALSE;
                }
            }
        }
        
    }
    SFS_CloseFile(file_handle);
    WifiDestroyList();
    return TRUE;
}

/****************************************************************************/
//  Description : get the wifi profile num
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC uint32 MMIWIFI_GetProfileNum(void)
{
    uint32 number = 0;
    
    if(0 != s_mmiwifi_list)
    {
        number = CFL_ListGetMemberCount(s_mmiwifi_list);
    }
    return number;
}

/****************************************************************************/
//  Description : get the wifi profile by index
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T* MMIWIFI_GetProfileByIndex(MMIWIFI_INDEX_T index)
{
    MMIWIFI_PROFILE_T* node_data_ptr = PNULL;

    CFL_LIST_NODE_HANDLE list_node_handle = WifiFindNodeByIndex(index);
    if(0 != list_node_handle)
    {
        node_data_ptr = CFL_ListGetNodeData(list_node_handle);
    }
    return node_data_ptr;
}

/****************************************************************************/
//  Description : get the connected wifi profile index
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void MMIWIFI_GetConnectedProfileIndex( MMIWIFI_INDEX_T* index_ptr )
{
    uint32              num = 0;
    uint32              index = 0;
    MMIWIFI_PROFILE_T*  pProfile = PNULL;

    TRACE_APP_WLAN("index before %d", *index_ptr);

    num = MMIWIFI_GetProfileNum();

    for( index = 0; index < num; index++ )
    {
        pProfile = MMIWIFI_GetProfileByIndex( index );

        if( PNULL != pProfile )
        {
            if( (MMIWIFI_PROFILE_STATE_CONNECTED == pProfile->state) || (MMIWIFI_PROFILE_STATE_DHCPED == pProfile->state) )
            {
                *index_ptr = index;

                TRACE_APP_WLAN("connected index after %d", index);
                return;
            }
        }
    }

    TRACE_APP_WLAN("not find connected profile!!");
}

/****************************************************************************/
//  Description : add the wifi profile to wifi list
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_AddProfileToList(MMIWIFI_PROFILE_T* wifi_profile_ptr)
{
    CFL_LIST_NODE_HANDLE node_handle = 0;
    BOOLEAN IP_type = FALSE;
    uint32 temp_IP, temp_GateWay, temp_Submask, temp_DNS1, temp_DNS2 = 0;
    char *ip_ptr = PNULL;

    if (0 == s_mmiwifi_list)
    {
        s_mmiwifi_list = CFL_ListCreat(sizeof(MMIWIFI_PROFILE_T),MMIWIFI_MAX_PROFILE_NUM);
        if(0 == s_mmiwifi_list)
        {
            return FALSE;
        }
    }
    node_handle = CFL_ListGetTail(s_mmiwifi_list);
    //add profile to list tail
    IP_type = MMIWIFI_GetIPType();
    temp_IP = MMIWIFI_GetIPData();
    temp_GateWay = MMIWIFI_GetGateWay();
    temp_Submask = MMIWIFI_GetSubMask();
    temp_DNS1 = MMIWIFI_GetDNS1();
    temp_DNS2 = MMIWIFI_GetDNS2();

    if(IP_type)
    {//设置当前profile的静态IP
        uint16 ip_len = 0;
        wifi_profile_ptr->ip_get.ip_get_type = MMIWIFI_IPGET_SELFDEFINED;//@discuss 0105

        ip_ptr = inet_ntoa(ntohl(temp_IP));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(wifi_profile_ptr->ip_get.ip.ip_addr,0,sizeof((wifi_profile_ptr->ip_get.ip.ip_addr)));
            ip_len = strlen(ip_ptr);
            strncpy(wifi_profile_ptr->ip_get.ip.ip_addr, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }

        ip_ptr = inet_ntoa(ntohl(temp_GateWay));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(wifi_profile_ptr->ip_get.ip.default_gateway,0,sizeof((wifi_profile_ptr->ip_get.ip.default_gateway)));
            ip_len = strlen(ip_ptr);
            strncpy(wifi_profile_ptr->ip_get.ip.default_gateway, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }

        ip_ptr = inet_ntoa(ntohl(temp_Submask));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(wifi_profile_ptr->ip_get.ip.subnet_mask,0,sizeof((wifi_profile_ptr->ip_get.ip.subnet_mask)));
            ip_len = strlen(ip_ptr);
            strncpy(wifi_profile_ptr->ip_get.ip.subnet_mask, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }

        ip_ptr = inet_ntoa(ntohl(temp_DNS1));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(wifi_profile_ptr->ip_get.ip.first_dns,0,sizeof((wifi_profile_ptr->ip_get.ip.first_dns)));
            ip_len = strlen(ip_ptr);
            strncpy(wifi_profile_ptr->ip_get.ip.first_dns, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }

        ip_ptr = inet_ntoa(ntohl(temp_DNS2));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(wifi_profile_ptr->ip_get.ip.second_dns,0,sizeof((wifi_profile_ptr->ip_get.ip.second_dns)));
            ip_len = strlen(ip_ptr);
            strncpy(wifi_profile_ptr->ip_get.ip.second_dns, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }
    }
    else
    {//设置当前profile的动态IP
        wifi_profile_ptr->ip_get.ip_get_type = MMIWIFI_IPGET_AUTO;//@discuss 0105
    }

    //add profile to list tail
    if(CFL_ListInsertNode(s_mmiwifi_list,node_handle,(CFL_NODE_OBJECT)wifi_profile_ptr,sizeof(MMIWIFI_PROFILE_T)))

    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************/
//  Description : update the wifi profile by index
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_UpdateProfileByIndex(MMIWIFI_INDEX_T index, const MMIWIFI_PROFILE_T* wifi_profile_ptr)
{
    MMIWIFI_PROFILE_T* profile_ptr = MMIWIFI_GetProfileByIndex(index);
    
    if(PNULL != profile_ptr && PNULL != wifi_profile_ptr)
    {
        SCI_MEMCPY(profile_ptr, wifi_profile_ptr, sizeof(MMIWIFI_PROFILE_T));
        if(wifi_profile_ptr->is_trusted)
        {
            MMIWIFI_UpdateProfileFile();
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************/
//  Description : delete the wifi profile by index
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DeleteProfileByIndex(MMIWIFI_INDEX_T index)
{
    BOOLEAN                 result = FALSE;
    CFL_LIST_NODE_HANDLE    list_node_handle = 0;
    BOOLEAN                 is_save_to_file = FALSE;
    MMIWIFI_PROFILE_T*      profile_ptr = MMIWIFI_GetProfileByIndex(index);

    if(PNULL != profile_ptr)
    {
        if(profile_ptr->is_trusted)
        {
            is_save_to_file = TRUE;
        }

        //free profile
        WifiFreeProfile(profile_ptr);
        //delete the node from list
        list_node_handle = WifiFindNodeByIndex(index);
        if(0 != list_node_handle)
        {
            CFL_ListDeleteNode(s_mmiwifi_list,list_node_handle);
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
        if(result == TRUE && is_save_to_file == TRUE)
        {
            MMIWIFI_UpdateProfileFile();
        }

    }
    else
    {
        result = FALSE;
    }
    return result;
}

/****************************************************************************/
//  Description : find wifi node by index
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL CFL_LIST_NODE_HANDLE WifiFindNodeByIndex(MMIWIFI_INDEX_T index)
{
    uint32                  i = 0;
    CFL_LIST_NODE_HANDLE    list_node_handle = 0;
    uint32                  number = MMIWIFI_GetProfileNum();

    if(index > number - 1)
    {
        return PNULL;
    }
    else
    {
        list_node_handle = CFL_ListGetHead(s_mmiwifi_list);
        for(i = 0; i < index; i++)
        {
            list_node_handle = CFL_ListGetNextNode(list_node_handle);
        }
    }
    return list_node_handle;
}

/****************************************************************************/
//  Description : free wifi profile
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiFreeProfile(MMIWIFI_PROFILE_T* profile_ptr)
{
/*
    BOOLEAN result = TRUE;

    if (PNULL == profile_ptr)
    {
        return result;
    }
    //we not use ptr,so memset
    SCI_MEMSET(profile_ptr,0x00,sizeof(MMIWIFI_PROFILE_T));
    return result;
*/
    return TRUE;
}

/****************************************************************************/
//  Description : destroy wifi list
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiDestroyList(void)
{
    if(0 != s_mmiwifi_list)
    {
        CFL_ListDestroy(s_mmiwifi_list);
        s_mmiwifi_list = 0;
    }
}

/****************************************************************************/
//  Description : clear wifi
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_Clear(void)
{
    uint16*             device_name_ptr=PNULL;
    uint16              device_name_len=0;
    uint16              file_name[MMIWIFI_MAX_FILE_NAME_LEN + 1] = {0};
    
    device_name_ptr = WifiGetSysDevName(&device_name_len);
    //create and save wifi profile to wifi file
    if(PNULL != device_name_ptr)
    {
        CFLWCHAR_StrNCat(file_name,device_name_ptr, MMIWIFI_MAX_FILE_NAME_LEN + 1);
        CFLWCHAR_StrNCatWithAsciiStr(file_name,MMIWIFI_FILE, MMIWIFI_MAX_FILE_NAME_LEN + 1);
        SFS_DeleteFile(file_name,PNULL);
    }
    else
    {
        TRACE_APP_WLAN("device_name_ptr is PNULL!!");
    }
    WifiDestroyList();
}

/****************************************************************************/
//  Description : get wifi device sys name
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC uint16* WifiGetSysDevName(uint16 *device_name_len_ptr)
{
	 MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SYSTEM;
    *device_name_len_ptr=0;

	
	for(file_dev = MMI_DEVICE_SYSTEM; file_dev < MMI_DEVICE_NUM; file_dev++)
	{
		if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(file_dev), MMIAPIFMM_GetDevicePathLen(file_dev)))
		{
			*device_name_len_ptr=MMIAPIFMM_GetDevicePathLen(file_dev);
			return (uint16*)MMIAPIFMM_GetDevicePath(file_dev);
		}
	}
	return PNULL;
}
/****************************************************************************/
//  Description : update wifi profile file
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_UpdateProfileFile(void)
{
    uint32              i = 0;
    SFS_HANDLE          file_handle = 0;
    // BOOLEAN             result = TRUE;
    wchar*              device_name_ptr=PNULL;
    uint16              device_name_len=0;
    wchar               file_name[MMIWIFI_MAX_FILE_NAME_LEN + 1] = {0};
    MMIWIFI_PROFILE_T*  profile_ptr = PNULL;
    uint32              bytes_writen = 0;
    uint32              num = MMIWIFI_GetProfileNum();
    
    device_name_ptr = WifiGetSysDevName(&device_name_len);
    //create and save wifi profile to wifi file
    if(PNULL != device_name_ptr)
    {
        CFLWCHAR_StrNCat(file_name,device_name_ptr, MMIWIFI_MAX_FILE_NAME_LEN + 1);
        CFLWCHAR_StrNCatWithAsciiStr(file_name,MMIWIFI_FILE, MMIWIFI_MAX_FILE_NAME_LEN + 1);
        file_handle = SFS_CreateFile(file_name, SFS_MODE_WRITE| SFS_MODE_CREATE_ALWAYS, PNULL, PNULL);
        if(0 == file_handle)
        {
            TRACE_APP_WLAN("file_handle is PNULL!!");
            return FALSE;
        }
    }
    else
    {
        TRACE_APP_WLAN("device_name_ptr is PNULL!!");
    }

    if(0 != num)
    {
        for(i = 0; i<num; i++)
        {
            profile_ptr = MMIWIFI_GetProfileByIndex(i);
            if(PNULL != profile_ptr && profile_ptr->is_trusted)
            {
                //save wifi profile to wifi file
                if (SFS_NO_ERROR != SFS_WriteFile(file_handle,profile_ptr,sizeof(MMIWIFI_PROFILE_T),&bytes_writen,PNULL))
                {
                    SFS_CloseFile(file_handle);
                    SFS_DeleteFile(file_name,PNULL);
                    TRACE_APP_WLAN("SFS_WriteFile fail!!");
                    return FALSE;
                }
            }
        }
    }
    SFS_CloseFile(file_handle);
    return TRUE;
}


/*****************************************************************************
 Prototype    : MMIWIFI_TraverseProfile
 Description  : 遍历profile
 Input        : TRAV_PF_CALLBACK ptrCallFunc  
 Output       : None
 Return Value : PUBLIC
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/10/3
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
PUBLIC void MMIWIFI_TraverseProfile(TRAV_PF_CALLBACK ptrCallFunc, void *find_data)
{
    (void)CFL_ListFindNode(s_mmiwifi_list, find_data, (LIST_NODE_QUERY_CALLBACK)ptrCallFunc);
}

/****************************************************************************/
//  Description : Check ap is cmcc
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_APIsCMCC(MMIWIFI_PROFILE_T* profile_ptr)
{
    if (PNULL == profile_ptr)
    {
        return FALSE;
    }

    if (profile_ptr->config_info.ssid.ssid_len == strlen((char *)MMIWIFI_AP_CMCC)
        && 0 == strncmp((char *)profile_ptr->config_info.ssid.ssid, (char *)MMIWIFI_AP_CMCC, profile_ptr->config_info.ssid.ssid_len)
        && WIFISUPP_NETWORK_MODE_INFRASTRUCTURE == profile_ptr->config_info.network_mode
        && WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == profile_ptr->config_info.encryp_protocol)
    {
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************/
//  Description : Check ap is cmcc-edu
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_APIsCMCCEDU(MMIWIFI_PROFILE_T* profile_ptr)
{
    if (PNULL == profile_ptr)
    {
        return FALSE;
    }

    if (profile_ptr->config_info.ssid.ssid_len == strlen((char *)MMIWIFI_AP_CMCC_EDU)
        && 0 == strncmp((char *)profile_ptr->config_info.ssid.ssid, (char *)MMIWIFI_AP_CMCC_EDU, profile_ptr->config_info.ssid.ssid_len)
        && WIFISUPP_NETWORK_MODE_INFRASTRUCTURE == profile_ptr->config_info.network_mode
        && WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == profile_ptr->config_info.encryp_protocol)
    {
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************/
//  Description : Check the same AP
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_APIsSame(MMIWIFI_PROFILE_T * profile_ptr1, MMIWIFI_PROFILE_T * profile_ptr2)
{
    WIFISUPP_WAPI_CREDENTIAL_TYPE_E     type1 = WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK;
    WIFISUPP_WAPI_CREDENTIAL_TYPE_E     type2 = WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK;

    if (PNULL == profile_ptr1 || PNULL == profile_ptr2)
    {
        return FALSE;
    }

    type1 = profile_ptr1->config_info.credential.wapi_credential.credential_type;
    type2 = profile_ptr2->config_info.credential.wapi_credential.credential_type;

    if (profile_ptr1->config_info.ssid.ssid_len == profile_ptr2->config_info.ssid.ssid_len
        && !memcmp(profile_ptr1->config_info.ssid.ssid, profile_ptr2->config_info.ssid.ssid, profile_ptr1->config_info.ssid.ssid_len)
        && profile_ptr1->config_info.network_mode == profile_ptr2->config_info.network_mode
        && profile_ptr1->config_info.encryp_protocol == profile_ptr2->config_info.encryp_protocol
        && ((WIFISUPP_ENCRYP_PROTOCOL_WAPI != profile_ptr1->config_info.encryp_protocol) || (WIFISUPP_ENCRYP_PROTOCOL_WAPI == profile_ptr1->config_info.encryp_protocol &&  type1 == type2)))
    {
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************/
//  Description : Find Trusted Profile By Ap
//  Global resource dependence : 
//  Author:Jiaoyou.wu 
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T * MMIWIFI_FindProfileByAP(MMIWIFI_PROFILE_T *profile_ptr)
{
    MMIWIFI_PROFILE_T *find_profile_ptr = PNULL;
    CFL_LIST_NODE_HANDLE node_found = 0;

    if (PNULL == profile_ptr)
    {
        return PNULL;
    }
    //SCI_TRACE_LOW:"MMIWIFI_FindProfileByAP"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PROFILE_717_112_2_18_3_9_50_170,(uint8*)"");
    node_found = CFL_ListFindNode(s_mmiwifi_list, (void *)profile_ptr, (LIST_NODE_QUERY_CALLBACK)mmiwifi_EqualWifiAP);
    if(0 != node_found)
    {
        find_profile_ptr = (MMIWIFI_PROFILE_T *)CFL_ListGetNodeData(node_found);
    }

    return find_profile_ptr;
}

/****************************************************************************/
//  Description : Update Profile By Ap
//  Global resource dependence : 
//  Author:Jiaoyou.wu 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_UpdateProfileByAP(MMIWIFI_PROFILE_T *des_profile_ptr, MMIWIFI_PROFILE_T *src_profile_ptr)
{
    if (PNULL != des_profile_ptr && PNULL !=src_profile_ptr && des_profile_ptr != src_profile_ptr)
    {
        SCI_MEMCPY(des_profile_ptr, src_profile_ptr, sizeof(MMIWIFI_PROFILE_T));
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************/
//  Description : Find Trusted Profile By Status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T * MMIWIFI_FindProfileByStatus(MMIWIFI_PROFILE_STATE_E status)
{
    MMIWIFI_PROFILE_T * profile_ptr = PNULL;
    CFL_LIST_NODE_HANDLE node_found = 0;

    //SCI_TRACE_LOW:"MMIWIFI_FindProfileByAP: status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PROFILE_751_112_2_18_3_9_51_171,(uint8*)"d", status);

    node_found = CFL_ListFindNode(s_mmiwifi_list, (void *)status, (LIST_NODE_QUERY_CALLBACK)mmiwifi_EqualWifiStatus);
    if(0 != node_found)
    {
        profile_ptr = (MMIWIFI_PROFILE_T *)CFL_ListGetNodeData(node_found);
    }

    return profile_ptr;
}

/****************************************************************************/
//  Description : Equal Status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN mmiwifi_EqualWifiStatus(CFL_NODE_OBJECT data, void *find_data)
{
    if(0 == data)
    {
        return FALSE;
    }
 
    return (((MMIWIFI_PROFILE_T *)data)->state == (uint32)find_data);
}

/****************************************************************************/
//  Description : Equal AP
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN mmiwifi_EqualWifiAP(CFL_NODE_OBJECT data, void *find_data)
{
    if(0 == data)
    {
        return FALSE;
    }

    return MMIWIFI_APIsSame((MMIWIFI_PROFILE_T *)data, (MMIWIFI_PROFILE_T *)find_data);
}

/****************************************************************************/
//  Description : Equal On air
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN mmiwifi_EqualOnAir(CFL_NODE_OBJECT data, void *find_data)
{
    if(0 == data)
    {
        return FALSE;
    }

    return MMIWIFI_IsOnAir((MMIWIFI_PROFILE_T *)data);
}

/****************************************************************************/
//  Description : Insert after the index
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: Insert ahead of the Head if index is Invalid
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_InsertProfileToList(MMIWIFI_INDEX_T index, MMIWIFI_PROFILE_T *profile_ptr)
{
    CFL_LIST_NODE_HANDLE node_handle = 0;
    BOOLEAN IP_type = FALSE;
    uint32 temp_IP, temp_GateWay, temp_Submask, temp_DNS1, temp_DNS2 = 0;
    char *ip_ptr = PNULL;

    
    if (0 == s_mmiwifi_list)
    {
        if(!WifiCreateList())
        {
            return FALSE;
        }
    }

    node_handle = WifiFindNodeByIndex(index);

    IP_type = MMIWIFI_GetIPType();
    temp_IP = MMIWIFI_GetIPData();
    temp_GateWay = MMIWIFI_GetGateWay();
    temp_Submask = MMIWIFI_GetSubMask();
    temp_DNS1 = MMIWIFI_GetDNS1();
    temp_DNS2=MMIWIFI_GetDNS2();

    if(IP_type)
    {//设置当前profile的静态IP
        uint16 ip_len = 0;
        profile_ptr->ip_get.ip_get_type = MMIWIFI_IPGET_SELFDEFINED;//@discuss 0105

        ip_ptr = inet_ntoa(ntohl(temp_IP));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(profile_ptr->ip_get.ip.ip_addr,0,sizeof((profile_ptr->ip_get.ip.ip_addr)));
            ip_len = strlen(ip_ptr);
            strncpy(profile_ptr->ip_get.ip.ip_addr, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }

        ip_ptr = inet_ntoa(ntohl(temp_GateWay));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(profile_ptr->ip_get.ip.default_gateway,0,sizeof((profile_ptr->ip_get.ip.default_gateway)));
            ip_len = strlen(ip_ptr);
            strncpy(profile_ptr->ip_get.ip.default_gateway, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }

        ip_ptr = inet_ntoa(ntohl(temp_Submask));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(profile_ptr->ip_get.ip.subnet_mask,0,sizeof((profile_ptr->ip_get.ip.subnet_mask)));
            ip_len = strlen(ip_ptr);
            strncpy(profile_ptr->ip_get.ip.subnet_mask, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }

        ip_ptr = inet_ntoa(ntohl(temp_DNS1));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(profile_ptr->ip_get.ip.first_dns,0,sizeof((profile_ptr->ip_get.ip.first_dns)));
            ip_len = strlen(ip_ptr);
            strncpy(profile_ptr->ip_get.ip.first_dns, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }

        ip_ptr = inet_ntoa(ntohl(temp_DNS2));
        if (PNULL != ip_ptr)
        {
            SCI_MEMSET(profile_ptr->ip_get.ip.second_dns,0,sizeof((profile_ptr->ip_get.ip.second_dns)));
            ip_len = strlen(ip_ptr);
            strncpy(profile_ptr->ip_get.ip.second_dns, ip_ptr, MIN(ip_len, MMIWIFI_IP_LEN));
        }
    }
    else
    {//设置当前profile的动态IP
        profile_ptr->ip_get.ip_get_type = MMIWIFI_IPGET_AUTO;//@discuss 0105
    }

    return PNULL != CFL_ListInsertNode(s_mmiwifi_list, node_handle, (CFL_NODE_OBJECT)profile_ptr, sizeof(MMIWIFI_PROFILE_T));

}

/****************************************************************************/
//  Description : Create wifi list
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiCreateList(void)
{
    WifiDestroyList();

    s_mmiwifi_list = CFL_ListCreat(sizeof(MMIWIFI_PROFILE_T), MMIWIFI_MAX_PROFILE_NUM);

    return 0 != s_mmiwifi_list;
}

/****************************************************************************/
//  Description : get the wifi profile num
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC uint32 MMIWIFI_GetTrustedProfileNum(void)
{
    uint32 total_num = 0;
    uint32 trusted_num = 0;
    MMIWIFI_PROFILE_T *profile_ptr = NULL;
    uint32 i = 0;

    total_num = MMIWIFI_GetProfileNum();

    for(i = 0; i < total_num; i++)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex((MMIWIFI_INDEX_T)i);
        if(MMIWIFI_IsTrustedProfile(profile_ptr))
        {
            trusted_num++;
        }
    }

    return trusted_num;
}

/****************************************************************************/
//  Description : Get num of trusted profiles which are on air
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetOnAirTrustedProfileNum(void)
{
    uint32 total_num = 0;
    uint32 onair_truste_num = 0;
    MMIWIFI_PROFILE_T *profile_ptr = NULL;
    uint32 i = 0;

    total_num = MMIWIFI_GetProfileNum();

    for(i = 0; i < total_num; i++)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex((MMIWIFI_INDEX_T)i);

        if(MMIWIFI_IsTrustedProfile(profile_ptr) && MMIWIFI_IsOnAir(profile_ptr))
        {
            onair_truste_num++;
        }
    }

    return onair_truste_num;
}

/****************************************************************************/
//  Description : MMIWIFI_DeleteLastTrustedProfile
//  Global resource dependence : 
//  Author:
//  Note: 删除profile list中最后一个信任接入点
/****************************************************************************/
PUBLIC void MMIWIFI_DeleteLastTrustedProfile(void)
{
    uint32 total_num = 0;
    uint32 trusted_index = 0;
    MMIWIFI_PROFILE_T *profile_ptr = NULL;
    uint32 i = 0;

    total_num = MMIWIFI_GetProfileNum();
    trusted_index = MMIWIFI_GetTrustedProfileNum();

    for(i = 0; i < total_num; i++)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex((MMIWIFI_INDEX_T)i);
        if(MMIWIFI_IsTrustedProfile(profile_ptr))
        {
            if(1 == trusted_index)//找到最后一个信任接入点
            {
                if(PNULL != profile_ptr && profile_ptr->is_on_air)
                {
                    profile_ptr->is_trusted = FALSE;
                }
                else
                {
                    MMIWIFI_DeleteProfileByAp(profile_ptr);
                }
                break;
            }
            else
            {
                trusted_index--;
            }
        }
    }

}

LOCAL BOOLEAN mmiwifi_DeleteFirstTrustedProfile( BOOLEAN check_on_air )
{
    uint32              i = 0;
    uint32              total_num = 0;
    BOOLEAN             isDelete = FALSE;
    BOOLEAN             result = FALSE;
    MMIWIFI_PROFILE_T*  profile_ptr = NULL;

    TRACE_APP_WLAN("enter check_on_air %d", (int32)check_on_air);

    total_num = MMIWIFI_GetProfileNum();

    for( i = 0; i < total_num; i++ )
    {
        profile_ptr = MMIWIFI_GetProfileByIndex( (MMIWIFI_INDEX_T)i );
        if( PNULL == profile_ptr )
        {
            TRACE_APP_WLAN("list error");
            continue;
        }

        if( TRUE == MMIWIFI_IsTrustedProfile(profile_ptr) )
        {
            if( TRUE == profile_ptr->is_on_air )
            {
                if( TRUE == check_on_air )
                {
                    TRACE_APP_WLAN("%s is on air not delete it", profile_ptr->config_info.ssid.ssid);
                    continue;
                }
                else
                {
                    TRACE_APP_WLAN("%s change to not trusted", profile_ptr->config_info.ssid.ssid);
                    profile_ptr->is_trusted = FALSE;
                }
            }
            else
            {
                TRACE_APP_WLAN("%s is deleted", profile_ptr->config_info.ssid.ssid);
                MMIWIFI_DeleteProfileByAp(profile_ptr);
            }

            isDelete = TRUE;
            break;
        }
    }

    return isDelete;
}
/****************************************************************************/
//  Description : MMIWIFI_DeleteFirstTrustedProfile
//  Global resource dependence :
//  Author: bin.wang1
//  Note: 删除profile list中第一个信任且不在周边的热点，如果都在周边删除第一个
/****************************************************************************/
PUBLIC void MMIWIFI_DeleteFirstTrustedProfile( void )
{
    BOOLEAN result = FALSE;

    TRACE_APP_WLAN("enter");

    result = mmiwifi_DeleteFirstTrustedProfile( TRUE );
    if( FALSE == result )
    {
        TRACE_APP_WLAN("all trusted profile is on air, delete first one");
        result = mmiwifi_DeleteFirstTrustedProfile( FALSE );

        if( FALSE == result )
        {
            TRACE_APP_WLAN("not delete, no trusted profile");
        }
    }
}

/****************************************************************************/
//  Description : get num of display
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: On air and connected trusted
/****************************************************************************/
PUBLIC uint32 MMIWIFI_DisplayProfileNum(void)
{
    uint32 total_num = 0;
    uint32 on_air_num = 0;
    MMIWIFI_PROFILE_T *profile_ptr = NULL;
    uint32 i = 0;

    total_num = MMIWIFI_GetProfileNum();

    for(i = 0; i < total_num; i++)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex((MMIWIFI_INDEX_T)i);
        if(PNULL != profile_ptr && (profile_ptr->is_on_air || (MMIWIFI_IsTrustedProfile(profile_ptr) && MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state)))
        {
            on_air_num++;
        }
    }

    return on_air_num;
}

/****************************************************************************/
//  Description : wifi update profile list when WLAN off,delete all AP profile on air
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_DeleteOnAirProfile(void)
{
    uint32 total_num = 0;
    int32 i = 0;
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;

    total_num = MMIWIFI_GetProfileNum();
    for(i = total_num - 1 ; i >= 0; i--)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(i);
        if(PNULL != profile_ptr)
        {
            if(profile_ptr->is_trusted)
            {
                if(profile_ptr->is_on_air)
                { //若该AP同时是信任接入点，不删除，只置位
                    profile_ptr->is_on_air = FALSE;
                }
            }
            else if(profile_ptr->is_on_air)
            {
                //MMIWIFI_DeleteProfileByAp(profile_ptr); FIX185063
            }
        }
    }
}

/****************************************************************************/
//  Description : delete the wifi profile by index
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DeleteProfileByAp(MMIWIFI_PROFILE_T *profile_ptr)
{
    CFL_LIST_NODE_HANDLE node_found = 0;

    //SCI_TRACE_LOW:"MMIWIFI_FindProfileByAP"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PROFILE_1055_112_2_18_3_9_51_172,(uint8*)"");

    if (PNULL == profile_ptr)
    {
        return FALSE;
    }

    node_found = CFL_ListFindNode(s_mmiwifi_list, (void *)profile_ptr, (LIST_NODE_QUERY_CALLBACK)mmiwifi_EqualWifiAP);
    if(0 != node_found)
    {
        CFL_ListDeleteNode(s_mmiwifi_list, node_found);
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************/
//  Description : delete the wifi profile by index
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_APIsAvailable(void)
{
    CFL_LIST_NODE_HANDLE node_found = 0;

    //SCI_TRACE_LOW:"MMIAPIWIFI_APIsAvailable"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PROFILE_1080_112_2_18_3_9_51_173,(uint8*)"");

    node_found = CFL_ListFindNode(s_mmiwifi_list, NULL, (LIST_NODE_QUERY_CALLBACK)mmiwifi_EqualOnAir);
    if(0 != node_found)
    {
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************/
//  Description : Equal On air
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN mmiwifi_CMCCOnAir(CFL_NODE_OBJECT data, void *find_data)
{
    if(0 == data)
    {
        return FALSE;
    }

    if (MMIWIFI_IsOnAir((MMIWIFI_PROFILE_T *)data)
        && MMIWIFI_APIsCMCC((MMIWIFI_PROFILE_T *)data))
    {
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************/
//  Description : CMCC Available
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN mmiwifi_CMCCAvailable(CFL_NODE_OBJECT data, void *find_data)
{
    if(0 == data)
    {
        return FALSE;
    }

    if (MMIWIFI_IsOnAir((MMIWIFI_PROFILE_T *)data)
        && MMIWIFI_APIsCMCC((MMIWIFI_PROFILE_T *)data)
        && ((MMIWIFI_PROFILE_STATE_CONNECTED == ((MMIWIFI_PROFILE_T *)data)->state)
            || (MMIWIFI_PROFILE_STATE_DHCPED == ((MMIWIFI_PROFILE_T *)data)->state)))
    {
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************/
//  Description : Is CMCC AP Available
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsCMCCAvailable(void)
{
    CFL_LIST_NODE_HANDLE node_found = 0;

    //SCI_TRACE_LOW:"MMIWIFI_IsCMCCAvailable"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PROFILE_1141_112_2_18_3_9_51_174,(uint8*)"");

    node_found = CFL_ListFindNode(s_mmiwifi_list, NULL, (LIST_NODE_QUERY_CALLBACK)mmiwifi_CMCCAvailable);
    if(0 != node_found)
    {
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************/
//  Description : Get CMCC AP
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T* MMIWIFI_GetCMCCProfile(void)
{
    CFL_LIST_NODE_HANDLE node_found = 0;

    //SCI_TRACE_LOW:"MMIWIFI_GetCMCCProfile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_PROFILE_1160_112_2_18_3_9_51_175,(uint8*)"");

    node_found = CFL_ListFindNode(s_mmiwifi_list, NULL, (LIST_NODE_QUERY_CALLBACK)mmiwifi_CMCCOnAir);
    if(0 != node_found)
    {
        return (MMIWIFI_PROFILE_T *)CFL_ListGetNodeData(node_found);
    }

    return PNULL;
}

/****************************************************************************/
//  Description : add the wifi profile to wifi list
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_SwapProfile(MMIWIFI_PROFILE_T *profile_ptr1, MMIWIFI_PROFILE_T *profile_ptr2)
{
    MMIWIFI_PROFILE_T temp_profile = {0};

    if (PNULL == profile_ptr1 || PNULL ==profile_ptr2)
    {
        return FALSE;
    }

    SCI_MEMCPY(&temp_profile, profile_ptr1, sizeof(MMIWIFI_PROFILE_T));
    SCI_MEMCPY(profile_ptr1,profile_ptr2, sizeof(MMIWIFI_PROFILE_T));
    SCI_MEMCPY(profile_ptr2, &temp_profile, sizeof(MMIWIFI_PROFILE_T));
    return TRUE;
}

/****************************************************************************/
//  Description : handle wifi unknown window
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsTrustedProfile(MMIWIFI_PROFILE_T* profile_ptr)
{
    if (PNULL == profile_ptr)
    {
        return FALSE;
    }

     return profile_ptr->is_trusted;
}

/****************************************************************************/
//  Description : is editing
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsEditing(MMIWIFI_PROFILE_T* profile_ptr)
{
    if (PNULL == profile_ptr)
    {
        return FALSE;
    }

     return profile_ptr->is_editing;
}

/****************************************************************************/
//  Description : Is On Air
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsOnAir(MMIWIFI_PROFILE_T* profile_ptr)
{
    if (PNULL == profile_ptr)
    {
        return FALSE;
    }

     return profile_ptr->is_on_air;
}


