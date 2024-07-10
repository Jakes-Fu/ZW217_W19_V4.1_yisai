/*****************************************************************************
** File Name:      mmiwlan_profile.h                                         *
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

#ifndef _MMIWLAN_PROFILE_H_
#define _MMIWLAN_PROFILE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiwifi_export.h"
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
#define MMIWIFI_IP_LEN              17
#define MMIWIFI_IPV6_LEN            64
#define MMIWIFI_MAX_WEP_NUM         4
#define MMIWIFI_MAX_FILE_NAME_LEN   255
#define MMIWIFI_FILE                ":/System/wifi/wifi.dat"
#define MMIWIFI_MAX_PROFILE_NUM     30/* On_air(20) + trusted(10)*/

/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/
typedef enum MMIWIFI_IPGET_TYPE
{
    MMIWIFI_IPGET_AUTO,
    MMIWIFI_IPGET_SELFDEFINED,
    MMIWIFI_IPGET_MAX
}MMIWIFI_IPGET_TYPE_E;

typedef enum MMIWIFI_PROFILE_STATE
{
    MMIWIFI_PROFILE_STATE_DISCONNECTED,
    MMIWIFI_PROFILE_STATE_CONNECTING,
    MMIWIFI_PROFILE_STATE_CONNECTED,
    MMIWIFI_PROFILE_STATE_DHCPING,
    MMIWIFI_PROFILE_STATE_DHCPED,
    MMIWIFI_PROFILE_STATE_DISCONNECTING,

    MMIWIFI_PROFILE_STATE_MAX
}MMIWIFI_PROFILE_STATE_E;

typedef struct MMIWIFI_IP
{
    char    ip_addr[MMIWIFI_IP_LEN + 1];
    char    subnet_mask[MMIWIFI_IP_LEN + 1];
    char    default_gateway[MMIWIFI_IP_LEN + 1];
    char    first_dns[MMIWIFI_IP_LEN + 1];
    char    second_dns[MMIWIFI_IP_LEN + 1];
}MMIWIFI_IP_T;

typedef struct MMIWIFI_IPGET
{
    MMIWIFI_IPGET_TYPE_E    ip_get_type;
    MMIWIFI_IP_T            ip;
}MMIWIFI_IPGET_T;


typedef struct MMIWIFI_PROFILE
{
    BOOLEAN                 is_ssid_hide;
    WIFISUPP_SSID_CONFIG_T  config_info;
    BOOLEAN                 is_wep_ascii[MMIWIFI_MAX_WEP_NUM];
    uint16                  cur_wep_index;
    BOOLEAN                 is_wpa_ascii;
    BOOLEAN                 is_wapi_ascii;
    uint16              parent_file_name[MMIWIFI_MAX_FILE_NAME_LEN + 1];
    uint16              client_file_name[MMIWIFI_MAX_FILE_NAME_LEN + 1];
    MMIWIFI_IPGET_T         ip_get;
    uint32                  rate;
    int32                   rssi;
    BOOLEAN                 is_dhcp_received;
    BOOLEAN                 is_dhcp_success;
    BOOLEAN                 is_dhcp6_received;
    BOOLEAN                 is_dhcp6_success;
    BOOLEAN                 is_auto_connect;
    BOOLEAN                 is_on_air;               /* Can be scaned */
    BOOLEAN                 is_trusted;
    BOOLEAN                 is_auth_input;   /*has edit auth*/
    BOOLEAN                 is_editing;
    MMIWIFI_PROFILE_STATE_E state;
}MMIWIFI_PROFILE_T;

typedef BOOLEAN (*TRAV_PF_CALLBACK)(MMIWIFI_PROFILE_T *pstProfile, void *find_data);

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : initialize the wifi profile list
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ProfileInitialize(void);

/****************************************************************************/
//  Description : finalize the wifi profile list
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ProfileFinalize(void);

/****************************************************************************/
//  Description : get the wifi profile num
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC uint32 MMIWIFI_GetProfileNum(void);

/****************************************************************************/
//  Description : get the wifi profile by index
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T* MMIWIFI_GetProfileByIndex(MMIWIFI_INDEX_T index);

/****************************************************************************/
//  Description : get the connected wifi profile index
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void MMIWIFI_GetConnectedProfileIndex( MMIWIFI_INDEX_T* index_ptr );

/****************************************************************************/
//  Description : update the wifi profile by index
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_UpdateProfileByIndex(MMIWIFI_INDEX_T index, const MMIWIFI_PROFILE_T* wifi_profile_ptr);

/****************************************************************************/
//  Description : delete the wifi profile by index
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DeleteProfileByIndex(MMIWIFI_INDEX_T index);

/****************************************************************************/
//  Description : clear wifi
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_Clear(void);

/****************************************************************************/
//  Description : update wifi profile file
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_UpdateProfileFile(void);

/****************************************************************************/
//  Description : add the wifi profile to wifi list
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_AddProfileToList(MMIWIFI_PROFILE_T* wifi_profile_ptr);

/****************************************************************************/
//  Description : 遍历list中的所有profile
//  Global resource dependence : 
//  Author:George.Liu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_TraverseProfile(TRAV_PF_CALLBACK ptrCallFunc, void *find_data);

/****************************************************************************/
//  Description : Check ap is cmcc
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_APIsCMCC(MMIWIFI_PROFILE_T* profile_ptr);

/****************************************************************************/
//  Description : Check ap is cmcc-edu
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_APIsCMCCEDU(MMIWIFI_PROFILE_T* profile_ptr);

/****************************************************************************/
//  Description : Check the same AP
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_APIsSame(MMIWIFI_PROFILE_T * profile_ptr1, MMIWIFI_PROFILE_T * profile_ptr2);

/****************************************************************************/
//  Description : Find Trusted Profile By Ap
//  Global resource dependence : 
//  Author:Jiaoyou.wu 
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T * MMIWIFI_FindProfileByAP(MMIWIFI_PROFILE_T *profile_ptr);

/****************************************************************************/
//  Description : Update Profile By Ap
//  Global resource dependence : 
//  Author:Jiaoyou.wu 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_UpdateProfileByAP(MMIWIFI_PROFILE_T *des_profile_ptr, MMIWIFI_PROFILE_T *src_profile_ptr);

/****************************************************************************/
//  Description : Find Trusted Profile By Status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T * MMIWIFI_FindProfileByStatus(MMIWIFI_PROFILE_STATE_E status);


/****************************************************************************/
//  Description : Insert after the index
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: Insert ahead of the Head if index is Invalid
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_InsertProfileToList(MMIWIFI_INDEX_T index, MMIWIFI_PROFILE_T* wifi_profile_ptr);

/****************************************************************************/
//  Description : get the trustedwifi profile num
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC uint32 MMIWIFI_GetTrustedProfileNum(void);

/****************************************************************************/
//  Description : MMIWIFI_DeleteFirstTrustedProfile
//  Global resource dependence :
//  Author: bin.wang1
//  Note: 删除profile list中第一个信任且不在周边的热点，如果都在周边删除第一个
/****************************************************************************/
PUBLIC void MMIWIFI_DeleteFirstTrustedProfile( void );

/****************************************************************************/
//  Description : Get num of trusted profiles which are on air
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetOnAirTrustedProfileNum(void);

/****************************************************************************/
//  Description : MMIWIFI_DeleteLastTrustedProfile
//  Global resource dependence : 
//  Author:
//  Note: 删除profile list中最后一个信任接入点
/****************************************************************************/
PUBLIC void MMIWIFI_DeleteLastTrustedProfile(void);

/****************************************************************************/
//  Description : get num of display
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: On air and connected trusted
/****************************************************************************/
PUBLIC uint32 MMIWIFI_DisplayProfileNum(void);

/****************************************************************************/
//  Description : wifi update profile list when WLAN off,delete all AP profile on air
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_DeleteOnAirProfile(void);

/****************************************************************************/
//  Description : delete the wifi profile by index
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DeleteProfileByAp(MMIWIFI_PROFILE_T* wifi_profile_ptr);

/****************************************************************************/
//  Description : Swap Profile
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_SwapProfile(MMIWIFI_PROFILE_T *profile_ptr1, MMIWIFI_PROFILE_T *profile_ptr2);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsTrustedProfile(MMIWIFI_PROFILE_T* profile_ptr);

/****************************************************************************/
//  Description : is editing
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsEditing(MMIWIFI_PROFILE_T* profile_ptr);

/****************************************************************************/
//  Description : Is On Air
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsOnAir(MMIWIFI_PROFILE_T* profile_ptr);

/****************************************************************************/
//  Description : Is CMCC AP Available
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsCMCCAvailable(void);

/****************************************************************************/
//  Description : Get CMCC AP
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T* MMIWIFI_GetCMCCProfile(void);

#ifdef   __cplusplus
    }
#endif


#endif 



/* wifisupp_api.h
1.
} WIFISUPP_WPA_CREDENTIAL_T;    //WPA credential information

typedef struct
{
    WIFISUPP_WAPI_CREDENTIAL_TYPE_E  credential_type;
    union
    {
        WIFISUPP_WPA_PSK_T  wapi_psk_info;
        WIFISUPP_WAPI_EAP_T  wapi_certi_info;//证书
    }credential_info;
} WIFISUPP_WAPI_CREDENTIAL_T;    //WAPI credential information


2.
}WIFISUPP_WPA_CREDENTIAL_TYPE_E;

typedef enum
{
    WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK,   //WAPI using PSK
    WIFISUPP_WAPI_CREDENTIAL_TYPE_CERTI,   //WAPI using 证书
    WIFISUPP_WAPI_CREDENTIAL_TYPE_MAX
}WIFISUPP_WAPI_CREDENTIAL_TYPE_E;

3.
} WIFISUPP_WPA_EAP_T; //WPA EAP information

typedef struct
{
    CFLWCHAR    certificate_client_file_path[WIFISUPP_MAX_FILE_PATH_LEN]; //WAPI客户端证书
    CFLWCHAR    certificate_parent_file_path[WIFISUPP_MAX_FILE_PATH_LEN];  //WAPI根证书
} WIFISUPP_WAPI_EAP_T; //WAPI

*/
