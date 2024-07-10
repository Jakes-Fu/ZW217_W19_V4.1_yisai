/*******************************************************************************
 *
 * Copyright (c) 2014 Bosch Software Innovations GmbH, Germany.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bosch Software Innovations GmbH - Please refer to git log
 *
 *******************************************************************************/
/*
 *  event_handler.c
 *
 *  Callback for value changes.
 *
 *  Created on: 21.01.2015
 *  Author: Achim Kraus
 *  Copyright (c) 2014 Bosch Software Innovations GmbH, Germany. All rights reserved.
 */

 
#include "liblwm2m.h"
#include "lwm2mclient.h"
#include "mn_type.h"
#include "internals.h"
//#include "osi_api.h"      //modify by xiameng.yang
//#include "osi_log.h"      //modify by xiameng.yang
//#include "cfw.h"               //modify by xiameng.yang
//#include "lwip/netif.h"  //modify by xiameng.yang

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "cfw_nb_nv_api.h"  //modify by xiameng.yang
//#include "netutils.h"  //modify by xiameng.yang
//extern struct netif *netif_default;  //modify by xiameng.yang

#ifdef LWM2M_EMBEDDED_MODE

static void prv_value_change(void* context,
                             const char* uriPath,
                             const char * value,
                             size_t valueLength)
{
    lwm2m_uri_t uri;
    if (lwm2m_stringToUri(uriPath, strlen(uriPath), &uri))
    {
        handle_value_changed(context, &uri, LWM2M_TYPE_OPAQUE - 3, value, valueLength);
    }
}

void init_value_change(lwm2m_context_t * lwm2m)
{
    //system_setValueChangedHandler(lwm2m, prv_value_change);    //xiameng.yang
    SCI_TRACE_LOW(" LWM2MDEBUG  [ %s  %d   ]",__FUNCTION__,__LINE__);
}

#else

void init_value_change(lwm2m_context_t * lwm2m)
{
}

void system_reboot()
{
    // exit(1);
    //osiShutdown(OSI_SHUTDOWN_RESET);  //xiameng.yang 
    SCI_TRACE_LOW(" LWM2MDEBUG  [ %s  %d   ]",__FUNCTION__,__LINE__);
}

#ifdef CONFIG_LWM2M_CTWING_SUPPORT
extern uint32_t CFW_CfgGetIMSI(uint8_t pIMSI[15], CFW_SIM_ID nSimID);
extern uint32_t CFW_GetNetinfo(CFW_NET_INFO_T *pNetinfo, CFW_SIM_ID nSimID);
extern uint8_t * CFW_GetICCID(CFW_SIM_ID nSimID);
void sulgsmbcd2asciiEx(uint8_t *pBcd, uint16_t nBcdLen, uint8_t *pNumber)
{
    uint16_t i;
    uint8_t *pTmp = pNumber;
    for (i = 0; i < nBcdLen; i++)
    {
        uint16_t high4bits = pBcd[i] >> 4;
        uint16_t low4bits = pBcd[i] & 0x0F;

        if (low4bits < 0x0A)
            *pTmp++ = low4bits + '0'; // 0 - 0x09
        else if (low4bits < 0x0F)     // 0x0A - 0x0F
            *pTmp++ = low4bits - 0x0A + 'A';

        if (high4bits < 0x0A)
            *pTmp++ = high4bits + '0'; // 0 - 9
        else if (high4bits < 0x0F)     // 0x0A - 0x0F
            *pTmp++ = high4bits - 0x0A + 'A';
    }
}


bool getSimIccid(uint8_t simId, uint8_t *simiccid, uint8_t *len)
{
    #if 0
     uint8_t *pICCID;
    if (*len < 20)
    {
        OSI_LOGI(0x10007531, "getSimIccid params error:%d", *len);
        return false;
    }
    pICCID = CFW_GetICCID(simId);
    if (pICCID != NULL)
    {
        *len = Sulgsmbcd2asciiEx(pICCID, 10, simiccid);
        OSI_LOGXI(0, 0x10007532, "getSimIccid:%s, %d", simiccid, *len);
    }

    #endif
    return true;
}

bool getSimImei(uint8_t simId, uint8_t *imei, uint8_t *len)
{
    if (*len < 16)
    {
        OSI_LOGI(0x10007533, "getSimImei params error:%d", *len);
        return false;
    }

    CFW_EmodGetIMEI(imei, len, simId);
    OSI_LOGXI(0, 0x10007534, "getSimImei:%s, %d", imei, *len);
    return true;
}

void cfwIMSItoASC(uint8_t *InPut, uint8_t *OutPut, uint8_t *OutLen)
{
    uint8_t i;

    OutPut[0] = ((InPut[1] & 0xF0) >> 4) + 0x30;
    for (i = 2; i < InPut[0] + 1; i++)
    {
        OutPut[2 * (i - 1) - 1] = (InPut[i] & 0x0F) + 0x30;
        OutPut[2 * (i - 1)] = ((InPut[i] & 0xF0) >> 4) + 0x30;
    }
    OutPut[2 * InPut[0] - 1] = 0x00;
    *OutLen = 2 * InPut[0] - 1;
}

bool getSimImsi(uint8_t simId, uint8_t *imsi, uint8_t *len)
{
    uint8_t nPreIMSI[16] = {0};
    uint32_t nRet = CFW_CfgGetIMSI(nPreIMSI, simId);
    if (ERR_SUCCESS == nRet)
    {
        if (*len < 16)
        {
            OSI_LOGI(0x10007535, "getSimImsi params error:%d", *len);
            return false;
        }
        cfwIMSItoASC(nPreIMSI, imsi, len);
        OSI_LOGXI(0, 0x10007536, "getSimImsi:%s ,%d", imsi, *len);

        return true;
    }
    return false;
}

bool isIpDigital(char *ipAddrStr)
{
    char *tmp = NULL;
    if (ipAddrStr == NULL)
        return false;

    
    for (tmp = ipAddrStr; *tmp != 0; tmp++)
    {
        if (*tmp == '.' || *tmp == ':')
        {
            return false;
        }
    }
    return true;
}

uint8_t * system_getIMSI()
{
    //if(netif_default == NULL)
        //return (uint8_t*)" ";
    uint8_t imsiLen = 16;
    int sim =MN_DUAL_SYS_1;
    static uint8_t pIMSI[16];
    memset(pIMSI, 0, 16);
    
    if(getSimImsi(sim, pIMSI, &imsiLen))
    {
        OSI_LOGXI(0, 0x10007bc0, "pIMSI =%s\n", pIMSI);
        return pIMSI;
    }
    return (uint8_t*)" ";
}



uint8_t* system_getICCID()
{
    //if(netif_default == NULL)
    //return (uint8_t*)"89860485192072216741";  //modify by xiameng.yang
    #if 1
    int sim =MN_DUAL_SYS_1;
    static uint8_t ICCID[23];
    uint8_t *pICCID = CFW_GetICCID(sim);
    //uint8_t *pICCID = "89860485192072216741";    //examples
    if(pICCID == NULL)
        return (uint8_t*)" ";
   
    memset(ICCID, 0, 23);
    sulgsmbcd2asciiEx(pICCID, 10, ICCID);
    OSI_LOGXI(0, 0x10004397, "pICCID : %s  ,ICCID =%s\n", pICCID,ICCID);
    return ICCID;
    #endif
}

uint8_t* system_getAPN()
{
    //if(netif_default == NULL)
       // return (uint8_t*)" ";
    CFW_GPRS_PDPCONT_INFO_V2 pdp_cont;
    int cid = 1;//netif_default->sim_cid & 0x0f;
    int sim = MN_DUAL_SYS_1;  //sim  0

     OSI_LOGXI(0, 0x10004397, "cid =%d   sim :%d \n", cid,sim);
    if (CFW_GprsGetPdpCxtV2(cid, &pdp_cont, sim) == 0)
    {
        static uint8_t nTmpApn[THE_APN_MAX_LEN];
        memset(nTmpApn, 0, THE_APN_MAX_LEN);
        memcpy(nTmpApn, pdp_cont.pApn, pdp_cont.nApnSize);
        return nTmpApn;
    }
    return (uint8_t*)" ";
}

uint8_t* system_getRSRP()
{
    //if(netif_default == NULL)
    //{
     //   return (uint8_t*)" ";
    //}
    static uint8_t rsrp[20];
    CFW_NET_INFO_T *pNetinfo;
    uint8_t nSim= MN_DUAL_SYS_1;
    memset(rsrp, 0, 20);
#if 1//defined(CONFIG_SOC_8910) || defined(CONFIG_SOC_8850)
    pNetinfo = (CFW_NET_INFO_T *)malloc(sizeof(CFW_NET_INFO_T));
    memset(pNetinfo, 0, sizeof(CFW_NET_INFO_T));
    
    if (0 != CFW_GetNetinfo(pNetinfo, nSim))
    {
        free(pNetinfo);
        return (uint8_t*)" ";
    }
    sprintf((char *)rsrp,"%d",pNetinfo->nwCapabilityLte.lteScell.rsrp);
    free(pNetinfo);
#endif
#ifdef CONFIG_SOC_8811
    CFW_UE_STATS_RPT *ueStatsRpt = nbiot_atGetUeStatsPara();
    sprintf((char *)rsrp,"%d",(ueStatsRpt->cellStats[0].rsrp / 10));
#endif
    OSI_LOGXI(0, 0x10009854, "rsrp =%s\n", rsrp);
    return (uint8_t*)rsrp;
}

uint8_t* system_getSINR()
{
    //if(netif_default == NULL)
    //{
      //  return (uint8_t*)" ";
    //}
    static uint8_t sinr[20];
    CFW_NET_INFO_T *pNetinfo;
    uint8_t nSim= MN_DUAL_SYS_1;
    memset(sinr, 0, 20);
#if  1//defined(CONFIG_SOC_8910) || defined(CONFIG_SOC_8850)
    pNetinfo = (CFW_NET_INFO_T *)malloc(sizeof(CFW_NET_INFO_T));
    memset(pNetinfo, 0, sizeof(CFW_NET_INFO_T));
    
    if (0 != CFW_GetNetinfo(pNetinfo, nSim))
    {
        free(pNetinfo);
        return (uint8_t*)" ";
    }
    sprintf((char *)sinr,"%d",pNetinfo->nwCapabilityLte.lteScell.SINR);
    free(pNetinfo);
#endif
#ifdef CONFIG_SOC_8811
    CFW_UE_STATS_RPT *ueStatsRpt = nbiot_atGetUeStatsPara();
    sprintf((char *)sinr,"%d",(ueStatsRpt->cellStats[0].snr / 10));
#endif
    OSI_LOGXI(0, 0x10009855, "sinr =%s\n", sinr);
    return (uint8_t*)sinr;
}

uint8_t* system_getTXPOWER()
{
    //if(netif_default == NULL)
    //{
      //  return (uint8_t*)" ";
    //}
    static uint8_t txpower[20];
    CFW_NET_INFO_T *pNetinfo;
    uint8_t nSim=MN_DUAL_SYS_1;
    memset(txpower, 0, 20);
#if 1//defined(CONFIG_SOC_8910) || defined(CONFIG_SOC_8850)
    pNetinfo = (CFW_NET_INFO_T *)malloc(sizeof(CFW_NET_INFO_T));
    memset(pNetinfo, 0, sizeof(CFW_NET_INFO_T));
   
    if (0 != CFW_GetNetinfo(pNetinfo, nSim))
    {
        free(pNetinfo);
        return (uint8_t*)" ";
    }
    sprintf((char *)txpower,"%d",pNetinfo->nwCapabilityLte.lteScell.txPower);
    free(pNetinfo);
#endif
#ifdef CONFIG_SOC_8811
    CFW_UE_STATS_RPT *ueStatsRpt = nbiot_atGetUeStatsPara();
    sprintf((char *)txpower,"%d",(ueStatsRpt->radioStats.cur_TX_power / 10));
#endif
    OSI_LOGXI(0, 0x10009856, "txpower =%s\n", txpower);
    return (uint8_t*)txpower;
}

uint8_t* system_getCELLID()
{
    //if(netif_default == NULL)
    //{
      //  return (uint8_t*)" ";
    //}
    static uint8_t cellid[20];
    CFW_NET_INFO_T *pNetinfo;
    uint8_t nSim=MN_DUAL_SYS_1;
    memset(cellid, 0, 20);
#if 1//defined(CONFIG_SOC_8910) || defined(CONFIG_SOC_8850)
    pNetinfo = (CFW_NET_INFO_T *)malloc(sizeof(CFW_NET_INFO_T));
    memset(pNetinfo, 0, sizeof(CFW_NET_INFO_T));
    
    if (0 != CFW_GetNetinfo(pNetinfo, nSim))
    {
        free(pNetinfo);
        return (uint8_t*)" ";
    }
    sprintf((char *)cellid,"%ld", (uint32_t)((pNetinfo->nwCapabilityLte.lteScell.enodeBID << 8) + pNetinfo->nwCapabilityLte.lteScell.cellId));
    free(pNetinfo);
#endif
#ifdef CONFIG_SOC_8811
    CFW_UE_STATS_RPT *ueStatsRpt = NULL;
    ueStatsRpt = nbiot_atGetUeStatsPara();
    sprintf((char *)cellid,"%ld",ueStatsRpt->radioStats.last_SIB1_cell_id);
#endif
    OSI_LOGXI(0, 0x10009857, "cellid =%s\n", cellid);
    return (uint8_t*)cellid;
}
#endif
#endif
