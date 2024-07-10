/*************************************************************************
 ** File Name:      mmicc_nv.c                                          *
 ** Author:         bruce.chi                                           *
 ** Date:           2006/09/25                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/25     bruce.chi        Create.                              *
*************************************************************************/


#define _MMIBEACON_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "beacon_nv.h"
#include "mmi_modu_main.h"

LOCAL BEACON_MAC_ADDR s_beacon_mac_addr = {0};


/*the length of set nv*/
const uint16 beacon_nv_len[] =
{
    sizeof(uint8),
    sizeof(BEACON_MAC_ADDR),
    sizeof(uint8), //max
};

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIBEACON_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_BEACON, beacon_nv_len, ARR_SIZE(beacon_nv_len));
}

PUBLIC char * MMIBEACON_NVGetMAC(void)
{
    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMI_BEACON_NV_MAC, &s_beacon_mac_addr))
    {
        SCI_MEMSET(&s_beacon_mac_addr, 0, sizeof(BEACON_MAC_ADDR));
#ifdef ZDT_SUPPORT_BEACON_SUNRI
        SCI_MEMCPY(s_beacon_mac_addr.addr, "8C34485836CD", strlen("8C34485836CD"));
#else
#ifdef FILE_LOG_SUPPORT
        //SCI_MEMCPY(s_beacon_mac_addr.addr, "CAA904020204", strlen("CAA904020204"));
        //SCI_MEMCPY(s_beacon_mac_addr.addr, "A9AD04020204", strlen("A9AD04020204"));
        //SCI_MEMCPY(s_beacon_mac_addr.addr, "FF5006020203", strlen("FF5006020203"));
        //SCI_MEMCPY(s_beacon_mac_addr.addr, "36AC04020204", strlen("36AC04020204"));
        SCI_MEMCPY(s_beacon_mac_addr.addr, "DC5D06020203", strlen("DC5D06020203"));
#endif
#endif
        MMI_WriteNVItem(MMI_BEACON_NV_MAC, &s_beacon_mac_addr);
    }
   return s_beacon_mac_addr.addr;
}

PUBLIC void MMIBEACON_NVSetMAC(char *mac_addr, uint16 mac_len)
{
    SCI_MEMSET(&s_beacon_mac_addr, 0, sizeof(BEACON_MAC_ADDR));
    if(mac_len > BEACON_MAC_MAX_LEN)
    {
        mac_len = BEACON_MAC_MAX_LEN;
    }
    SCI_MEMCPY(s_beacon_mac_addr.addr, mac_addr, mac_len);

    MMI_WriteNVItem(MMI_BEACON_NV_MAC, &s_beacon_mac_addr);
}


