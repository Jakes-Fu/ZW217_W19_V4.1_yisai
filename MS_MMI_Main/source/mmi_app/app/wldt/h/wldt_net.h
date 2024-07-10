/****************************************************************************
** File Name:      mmiphs_app.h                                                *
** Author:                                                                 *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshengqi         Create
** 
****************************************************************************/
#ifndef  _MMI_WLDT_NET_H_    
#define  _MMI_WLDT_NET_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "wldt_common.h"

#ifdef WLDT_TCP_SUPPORT
#include "wldt_tcp.h"
#endif
#define  WLDT_N_STATION_NUM		5
#define  WLDT_IMEI_LEN 15
#define  WLDT_IMSI_LEN 15
#define  WLDT_ICCID_LEN 20

typedef struct MMIWLDT_S_STATION_DATA_t
{
    uint8                                      is_valid;
    uint16                                      rssiLevel;
    uint16						mcc;
    uint16						mnc;
    uint16						mnc_digit_num;
    uint16						lac;    //位置区码
    uint32						cell_id; //基站ID
    MN_DUAL_SYS_E                    dual_sys;
} MMIWLDT_S_STATION_DATA_T;

typedef struct MMIWLDT_N_STATION_DATA_t
{
    MN_DUAL_SYS_E                    dual_sys;
    uint8                                      is_valid;
    uint8                                      rssiLevel;
    uint32						cell_id; //基站ID
} MMIWLDT_N_STATION_DATA_T;

typedef struct MMIWLDT_STATION_DATA_t
{
    MMIWLDT_S_STATION_DATA_T   s_cell;
    MMIWLDT_N_STATION_DATA_T   n_cell[WLDT_N_STATION_NUM];
} MMIWLDT_STATION_DATA_T;

extern char*         g_wldt_phone_imei;
extern char         g_wldt_phone_imei_1[WLDT_IMEI_LEN+1];
extern char         g_wldt_phone_imei_2[WLDT_IMEI_LEN+1];
extern char         g_wldt_sim_imsi[WLDT_IMSI_LEN+1];
extern char         g_wldt_sim_iccid[WLDT_ICCID_LEN+1];
extern uint32         g_wldt_net_cell_id;

extern MMIWLDT_STATION_DATA_T g_wldt_station_data;
extern MMIWLDT_S_STATION_DATA_T g_wldt_s_station_data;

PUBLIC uint32 MMIWLDT_Net_GetNetID(void);
PUBLIC BOOLEAN MMIWLDT_Net_PdpStateGet(void);
PUBLIC BOOLEAN MMIWLDT_Net_IsActiving(void);
PUBLIC BOOLEAN MMIWLDT_Net_IsClosing(void);
PUBLIC BOOLEAN MMIWLDT_Net_IsInit(void);
PUBLIC BOOLEAN MMIWLDT_Net_Init(void);
PUBLIC BOOLEAN MMIWLDT_Net_Reset(BOOLEAN is_now);
PUBLIC void MMIWLDT_Net_Reset_Restore(void);
PUBLIC BOOLEAN MMIWLDT_Net_Open(void);
PUBLIC BOOLEAN MMIWLDT_Net_Close(void);

PUBLIC MN_DUAL_SYS_E MMIWLDT_Net_GetActiveSys(void);
PUBLIC void MMIWLDT_Net_SetActiveSys(MN_DUAL_SYS_E set_sys);

PUBLIC MMI_RESULT_E MMIWLDT_Net_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

PUBLIC BOOLEAN MMIWLDT_Net_StatusInd(APP_MN_NETWORK_STATUS_IND_T *status_ptr);
PUBLIC void MMIWLDT_Net_RssiInd(MN_DUAL_SYS_E dual_sys,uint8 rssi);
PUBLIC void MMIWLDT_Station_Rssi_Update(uint8 rssi);
PUBLIC BOOLEAN MMIWLDT_Net_APP_Init(void);
PUBLIC BOOLEAN MMIWLDT_Net_GPRS_SimSwitch(MN_DUAL_SYS_E dual_sys);
PUBLIC BOOLEAN MMIWLDT_Net_GPRS_IsOn(void);




#ifdef __cplusplus
    }
#endif

#endif
