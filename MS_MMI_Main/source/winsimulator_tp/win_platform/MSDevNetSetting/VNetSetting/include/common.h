/*****************************************************************************
 ** File Name:     common.h                                                  *
 ** Author:        yifeng.wang                                               *
 ** Date:          2010/08/18                                                *
 ** Copyright:     2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:   common interfaces and parameters.                         *
 *****************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                        *
 ** 2010/08/18     yifeng.wang            Create                             *
 *****************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
#ifndef PUBLIC
#define PUBLIC
#endif /* no PUBLIC */

#ifndef LOCAL
#define LOCAL static
#endif /* no LOCAL */

#ifndef TRUE
#define TRUE    1
#endif /* no TRUE */

#ifndef FALSE
#define FALSE   0
#endif /* no FALSE */

// data types
typedef signed char     int8;
typedef short           int16;
typedef long int        int32;

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned long int    uint32;

typedef uint8           BOOLEAN;


///////////////////////////////////////////////////////////////////////////////
// ip addresses
#define IP_NULL                 0x00000000
#define IP_BROADCAST            0xFFFFFFFF
#define IP_SUB_LOOPBACK         0x7F000000
#define IP_MASK_A               0x80000000
#define IP_MASK_B               0xC0000000
#define IP_MASK_C               0xE0000000
#define IP_ADDR_A               IP_NULL
#define IP_ADDR_B               0x80000000
#define IP_ADDR_C               0xC0000000
#define IP_SNMASK_A             0xFF000000
#define IP_SNMASK_B             0xFFFF0000
#define IP_SNMASK_C             0xFFFFFF00

#define PUSH_IPADDR(_ip)        (BYTE)((_ip)>>24), (BYTE)((_ip)>>16), (BYTE)((_ip)>>8), (BYTE)(_ip)

// register key path & key name
#define REG_KEYPATH             "SOFTWARE\\Spreadtrum\\MSDEVNetParam"
#define REG_KEYNAME_VIP         "VIP"
#define REG_KEYNAME_VMAC1       "VMac1"
#define REG_KEYNAME_VMAC2       "VMac2"
#define REG_KEYNAME_VMAC3       "VMac3"
#define REG_KEYNAME_VMAC4       "VMac4"
#define REG_KEYNAME_VMAC5       "VMac5"
#define REG_KEYNAME_VMAC6       "VMac6"

// ethernet configuration file name, section name & key name
#define CONFIG_FILE_NAME        "ethernet_cfg.ini"
#define SECTION_NAME_BASE       "MSDEVNetParam"
#define KEY_NAME_IP             "ip"
#define KEY_NAME_MAC            "mac"
#define KEY_NAME_SNMASK         "snmask"
#define KEY_NAME_GATEWAY        "gateway"
#define KEY_NAME_DNS_PRI        "dns_pri"
#define KEY_NAME_DNS_SEC        "dns_sec"
#define KEY_NAME_DHCP           "dhcp"
#define KEY_NAME_DEVICE_NAME    "device_name"
#define KEY_NAME_DEVICE_DESC    "device_desc"

// network state file name, section name & key name
#define STATE_FILE_NAME         "ethernet_state.ini"
#define STATE_SECTION_NAME      "MSDEVNetState"
#define KEY_NAME_NETIF_ID       "netid"
#define KEY_NAME_STATE_NETIF    "state_net"
#define KEY_NAME_STATE_DHCP     "state_dhcp"
#define KEY_NAME_STATE_SEND     "state_send"
#define KEY_NAME_STATE_RECV     "state_recv"

#define STATE_TRUE              1
#define STATE_FALSE             0
#define STATE_UNKNOWN           (-1)
#define STATE_NETOPEN           1
#define STATE_NETERROR          0

// MSDEV network setting file name, section name & key name
#define SETTING_FILE_NAME       "msdev_nettest.ini"
#define SETTING_SECTION_NAME    "MSDEVNetSetting"
#define KEY_NAME_ENABLE_SEND    "enable_send"
#define KEY_NAME_ENABLE_RECV    "enable_recv"


///////////////////////////////////////////////////////////////////////////////
#endif /* _COMMON_H_ */