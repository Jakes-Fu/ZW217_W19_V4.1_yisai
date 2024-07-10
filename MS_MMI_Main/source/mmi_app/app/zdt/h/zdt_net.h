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
#ifndef  _MMI_ZDT_NET_H_    
#define  _MMI_ZDT_NET_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "zdt_common.h"

#ifdef ZDT_UDP_SUPPORT
#include "zdt_udp.h"
#endif
#ifdef ZDT_TCP_SUPPORT
#include "zdt_tcp.h"
#endif

#ifdef ZDT_PLAT_SHB_SUPPORT
#define  ZDT_N_STATION_NUM		6
#else
#define  ZDT_N_STATION_NUM		5
#endif
#define  ZDT_RFID_LEN 50
#define  ZDT_IMEI_LEN 15
#define  ZDT_IMSI_LEN 15
#define  ZDT_ICCID_LEN 20

#define  ZDT_WATER_LEN 20

#define  ZDT_SIM_NUMBER_LEN 128

typedef struct _ZDT_ICCD
{
    char num[ZDT_ICCID_LEN];
} ZDT_ICCID;

typedef struct _ZDT_RFID
{
    char num[ZDT_RFID_LEN];
} ZDT_RFID;

typedef struct MMIZDT_S_STATION_DATA_t
{
    uint8                                      is_valid;
    uint16                                      rssiLevel;
    uint16						mcc;
    uint16						mnc;
    uint16						mnc_digit_num;
    uint16						lac;    //位置区码
    uint32						cell_id; //基站ID
    MN_DUAL_SYS_E                    dual_sys;
} MMIZDT_S_STATION_DATA_T;

typedef struct MMIZDT_N_STATION_DATA_t
{
    MN_DUAL_SYS_E                    dual_sys;
    uint8                                      is_valid;
    uint8                                      rssiLevel;
    uint32						cell_id; //基站ID
} MMIZDT_N_STATION_DATA_T;

typedef struct MMIZDT_STATION_DATA_t
{
    MMIZDT_S_STATION_DATA_T   s_cell;
    MMIZDT_N_STATION_DATA_T   n_cell[ZDT_N_STATION_NUM];
} MMIZDT_STATION_DATA_T;

#ifdef MEID_WRITE_SUPPORT
extern char         g_zdt_phone_meid[ZDT_IMEI_LEN+1];
#endif
extern char*         g_zdt_phone_imei;
extern char         g_zdt_phone_imei_1[ZDT_IMEI_LEN+1];
extern char         g_zdt_phone_imei_2[ZDT_IMEI_LEN+1];
extern char         g_zdt_sim_imsi[ZDT_IMSI_LEN+1];
extern char         g_zdt_sim_iccid[ZDT_ICCID_LEN+1];
extern uint32         g_zdt_net_cell_id;
extern char         g_zdt_sim_number[ZDT_SIM_NUMBER_LEN+1];
extern ZDT_RFID         g_zdt_phone_rfid;

extern MMIZDT_STATION_DATA_T g_zdt_station_data;
extern MMIZDT_S_STATION_DATA_T g_zdt_s_station_data;

PUBLIC uint32 MMIZDT_Net_GetNetID(void);
PUBLIC BOOLEAN MMIZDT_Net_PdpStateGet(void);
PUBLIC BOOLEAN MMIZDT_Net_IsActiving(void);
PUBLIC BOOLEAN MMIZDT_Net_IsClosing(void);
PUBLIC BOOLEAN MMIZDT_Net_IsInit(void);
PUBLIC BOOLEAN MMIZDT_Net_Init(void);
PUBLIC BOOLEAN MMIZDT_Net_Reset(BOOLEAN is_now);
PUBLIC void MMIZDT_Net_Reset_Restore(void);
PUBLIC BOOLEAN MMIZDT_Net_Open(void);
PUBLIC BOOLEAN MMIZDT_Net_Close(void);

PUBLIC MN_DUAL_SYS_E MMIZDT_Net_GetActiveSys(void);
PUBLIC void MMIZDT_Net_SetActiveSys(MN_DUAL_SYS_E set_sys);

PUBLIC MMI_RESULT_E MMIZDT_Net_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

PUBLIC BOOLEAN MMIZDT_Net_StatusInd(APP_MN_NETWORK_STATUS_IND_T *status_ptr);
PUBLIC void MMIZDT_Net_RssiInd(MN_DUAL_SYS_E dual_sys,uint8 rssi);
PUBLIC void MMIZDT_Station_Rssi_Update(uint8 rssi);
PUBLIC BOOLEAN MMIZDT_Net_APP_Init(void);
PUBLIC BOOLEAN MMIZDT_Net_GPRS_SimSwitch(MN_DUAL_SYS_E dual_sys);
PUBLIC BOOLEAN MMIZDT_Net_GPRS_IsOn(void);
#ifdef ZDT_GPS_SUPPORT_AGPS
#ifdef ZDT_TCP_SUPPORT
PUBLIC void UBX_AGPS_TCP_Send(void);
PUBLIC void TD_AGPS_TCP_Send(void);
PUBLIC void CASIC_AGPS_TCP_Send(void);
PUBLIC void ALLYSTAR_AGPS_TCP_Send(void);
PUBLIC BOOLEAN AGPS_TCP_Init(void);
#endif
#endif




#ifdef __cplusplus
    }
#endif

#endif
