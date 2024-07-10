/*****************************************************************************
** File Name:      mmiwlan_win.h                                             *
** Author:         li.li                                                     *
** Date:           2009/11/18                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe wifi manager window         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/11/18     li.li          Create                                      *
******************************************************************************/

#ifndef _MMIWLAN_WIN_H_
#define _MMIWLAN_WIN_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "app_tcp_if.h"

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


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum MMIWIFI_UI_OPTION_FOR_WIFION
{
    MMIWIFI_UI_OPTION_NONE,
    MMIWIFI_UI_OPTION_PROMPT,

    MMIWIFI_UI_OPTION_MAX
}MMIWIFI_OPTION_FOR_WIFION_E;

 typedef struct MMIWIFI_UI_CONTEXT
{
    MMIWIFI_OPTION_FOR_WIFION_E option;
    MMIWIFI_PROFILE_T s_mmiwifi_profile;
    TCPIP_NETIF_IPADDR_T addrs;
}MMIWIFI_UI_CONTEXT_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/****************************************************************************/
//  Description : open wifi edit window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenEditWin(MMIWIFI_PROFILE_T *profile_ptr);

/****************************************************************************/
//  Description : open wifi new window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenNewWin(void);

/****************************************************************************/
//  Description : open wifi preshare key input for connect window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenPreshareKeyInputForConnWin(void);

/****************************************************************************/
//  Description : open wifi wep key input for connect window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenWep64BitForConnWin(void);

/****************************************************************************/
//  Description : open wifi wep key input for connect window
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenWepForConnWin(void);

/****************************************************************************/
//  Description : open wifi WPA key input for connect window
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenWPAForConnWin(void);

/****************************************************************************/
//  Description : open wifi wapi psk input for connect window
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenWAPIPskForConnWin(void);

/****************************************************************************/
//  Description : open wifi wapi certificate input for connect window
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenWAPICertificateForConnWin(void);

/****************************************************************************/
//  Description : open select ap win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenSelectAPWin(void);

#ifdef WLAN_RESELECT_SUPPORT
/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenSelectServiceWin(void);
#endif /* WLAN_RESELECT_SUPPORT */

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsNeedServiceChange(void);

/****************************************************************************/
//  Description : open wifi connect wait window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenConnectWaitWin(void);

/****************************************************************************/
//  Description : open wifi open wait window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenOnWaitWin(void);

/****************************************************************************/
//  Description : open wifi scan wait window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenScanWaitWin(void);

/****************************************************************************/
//  Description : open wifi open wait window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenOnWaitWin(void);

/*****************************************************************************/
//  Description : MMIAPIWIFI_IsChlidListWinOpen
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_IsChildListWinFocus(void);

/*****************************************************************************/
//  Description : Check List Win
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsListWinOpen(void);

/*****************************************************************************/
//  Description : Set addrs
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_SetIpAddress(TCPIP_NETIF_IPADDR_T *addrs);

/****************************************************************************/
//  Description : Reselect Ap
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ReSelectAP(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ChangeServiceToGPRS(void);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:rong.gu
//  Note:
/****************************************************************************/
PUBLIC MMI_IMAGE_ID_T WifiGetConnectedRssiImage(int32 rssi,BOOLEAN isEncrypt );

/*****************************************************************************/
//  Description : open the sim select window of browser
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWLAN_OpenSimSelectWin(ADD_DATA user_data);

/*****************************************************************************/
//  Description : check whether user is operating wifi list
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_IsOperating(void);

/*****************************************************************************/
//  Description : slelect Type
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWLAN_OpenEAPTypeSelectWin(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
