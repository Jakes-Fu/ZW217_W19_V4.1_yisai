/****************************************************************************************
** File Name:      wifieut_main.c                                                       *
** Author:         George.Liu                                                            *
** Date:           20010.10.18                                                           *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    特别注意!!!这只是一个dummy文件，为了解决编译问题。                   *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2010.10       George.Liu        Create                                                *
**                                                                                      *
*****************************************************************************************/
 

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "wifisupp_eut.h"
#include "wifisupp_eut_adapter.h"

#include "os_api.h"
#include "sci_api.h"

#include "csr_framework_ext.h"
#include "csr_wifi_ptest_lib.h"
#include "csr_wifi_ptest_prim.h"
#include "csr_wifi_ptest_task.h"
#include "csr_wifi_ptest_app_task.h"
#include "csr_wifi_sync_op.h"

#include "frw_csr/csr_rel_ver.h"

#define CSR_WIFI_PTEST_LIMIT_DATA_LENGTH_FOR_DSSS_PACKETS
#define CSR_WIFI_TURN_OFF_NORMAL_MODE_AUTOMATICALLY

#ifdef CSR_WIFI_TURN_OFF_NORMAL_MODE_AUTOMATICALLY
#include "csr_wifi_sme_lib.h"
#include "wifisupp_adap_csr_sef.h"
#endif

#include "wifisupp_adap_csr_config.h"
/*lint -e835, -e838 -e843*/
/******************************************************************************
 *                         Define
 ******************************************************************************
 */
/* As csr_wifi_firmware_ptest.h is removed from Synergy, therefore moves the CSR_WIFI_FIRMWARE_PTEST defines to here */
#define CSR_WIFI_FIRMWARE_PTEST_SUCCESS              0
#define CSR_WIFI_FIRMWARE_PTEST_FW_NOT_FOUND        -1
#define CSR_WIFI_FIRMWARE_PTEST_FW_LENGTH_NOT_FOUND -2
#define CSR_WIFI_FIRMWARE_PTEST_FAILURE             -3

#define PTEST_TX_FRAME_LEN_DEFAULT  (1000)

#define FUNC_ENTER()        SCI_TRACE_LOW("wifi: Enter %s:%d\n", __FILE__, __LINE__)
#define PTEST_DEBUG_PRINT(a) SCI_TRACE_LOW a



#define PTEST_RADIO_RX_READ(p_status)	do {\
				csr_sync_setup (sync, CSR_WIFI_PTEST_RADIO_RX_READ_CFM, CsrWifiPtestRadioRxReadCfmHandler);	\
			    CsrWifiPtestRadioRxReadReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE);	\
				csr_sync_wait_cfm (sync, p_status);\
			} while (0)

#define PTEST_CHECK_RESULT(status)	do { \
				if (status != 0) {	\
					extern const char * ptutGetLastError(void); \
					PTEST_DEBUG_PRINT (("***** ERROR, %d, Synergy_result = %d\n", __LINE__, status)); \
					PTEST_DEBUG_PRINT (("\t\t  %s\n", ptutGetLastError())); \
					return -1; \
				} \
			} while (0)

#define SYNCP(action,cfm,status)	do { \
				csr_sync_setup (sync, cfm, NULL); \
				action; \
				csr_sync_wait_cfm (sync, &status); \
				PTEST_CHECK_RESULT (status); \
			} while (0)

#define FUNC_NOT_SUPPORTED()	do { \
				PTEST_DEBUG_PRINT (("Error, %d, NOT-SUPPORTED\n", __LINE__));}while(0)



/******************************************************************************
 *                        Local
 ******************************************************************************
 */
/*
 * Device Configuration Method
 */
#define DEV_CONF_METHOD_BOARD_PROFILE	(0)
#define DEV_CONF_METHOD_EEPROM			(1)
static CsrUint8 gDevConfMethod = DEV_CONF_METHOD_BOARD_PROFILE;

/*
 * Sync operation
 */
static csr_sync_t *sync = NULL;

/*
 * Chip Info
 */
static CsrUint16 gChipVersion; /* 0x2822 for a05, 0x3a22 for b07 */

/*
 * Operation status
 */
static CsrBool gRadioInited = FALSE;
static CsrBool gTxStarted = FALSE;
static CsrBool gRxStarted = FALSE;

/*
 * TX, RX Test Configuration
 */
static CsrUint16 gTestBand = 1; /* 1=2.4G, 2=5G */
static CsrUint16 gTestChannel = 6;
static CsrUint16 gTestSFactor = 8000;   /* 8000 ~ 10000, unit:500, channel starting factor, only used if iface is  11a */
static CsrUint16 gTestRate = 11;    /* 1, 2, 5, 11, 6, 9, 12, etc */

static CsrUint16 gTxTestPowerLevel = 1; /* 1 ~ 8 ?? PowerTableIndex? or dBm ?? 
                                           PowerTableEntry index @profile ? then how to set power to max ?
                                           1: OK (Only this value has no error !!!! Why?)
                                           2: FAIL
                                           8: Invalid value error from F/W
                                           15: Invalid range error from PUniTestEngine::PowerLevelOk() in PTest
                                         */
static CsrUint16 gTxTestLength = PTEST_TX_FRAME_LEN_DEFAULT;  /* 0 ~ 2304 */
static CsrBool gTxTestEnable11bCsTestMode = FALSE;
static CsrUint32 gTxTestInterval = 20; /* interval between the start of each packet (us) */
static CsrWifiMacAddress gTxTestMacAddr = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static CsrWifiPtestPreamble gTxPreamble = CSR_WIFI_PTEST_PREAMBLE_LONG;
static CsrUint16 gTxAmplitude = 8; /* Amplitude setting for the signal (0...16). */
/*
 * RX statistics
 */
static CsrInt32 gGoodFrames = 0;
static CsrInt32 gBadFrames = 0;
static CsrInt32 gTestRssi = 0;


/******************************************************************************
 *                  Synergy-related Functions
 ******************************************************************************
 */
typedef struct
{
    CsrUint16 type;
    CsrUint8 result;
} CsrWifiPtestGeneralCfm;

/*
 * Synergy Module Definition
 * =================================
 */
void CsrWifiPtestAppInit(void **gash)
{
}

void CsrWifiPtestAppDeinit(void **gash)
{
}

void CsrWifiPtestAppHandler(void **gash)
{
#ifndef WIN32
    CsrUint16 eventClass;
    CsrWifiPtestGeneralCfm *cfm;
    void *message;

    if (!CsrSchedMessageGet(&eventClass, &message))
        return;
    if (eventClass != CSR_WIFI_PTEST_PRIM)
        return;

    cfm = (CsrWifiPtestGeneralCfm *) message;
	  PTEST_DEBUG_PRINT (("+++++ 0x%X, Result=%d +++++\n", cfm->type, cfm->result));
    csr_sync_check_cfm(sync, message);

    CsrWifiPtestFreeUpstreamMessageContents(eventClass, message);
    CsrPmemFree(message);
#endif
    return;
}

static void CsrWifiPtestChipIdGetCfmHandler(void *ctx, void *message)
{
#ifndef WIN32
    CsrWifiPtestChipIdGetCfm *prim = (CsrWifiPtestChipIdGetCfm *) message;
    gChipVersion = prim->chipId;
    PTEST_DEBUG_PRINT(("PtestChipID: 0x%04X\n", prim->chipId));
#endif
}


static void CsrWifiPtestFwBuildIdGetCfmHandler(void *ctx, void *message)
{
#ifndef WIN32
    CsrWifiPtestFwBuildIdGetCfm *prim = (CsrWifiPtestFwBuildIdGetCfm *) message;

    PTEST_DEBUG_PRINT(("PtestFwBuildId: %s\n", (char *)prim->buildId));
    
#endif
}

CsrUint32 ptest_totolGood;
static void CsrWifiPtestRadioRxReadCfmHandler(void *ctx, void *message)
{
#ifndef WIN32
    CsrWifiPtestRadioRxReadCfm *prim = (CsrWifiPtestRadioRxReadCfm *) message;
    gGoodFrames = prim->totalGood;
    gBadFrames = (CsrInt32)prim->total - gGoodFrames;
    gTestRssi = prim->rssiAv;
#endif
}


/******************************************************************************
 *                       API Implementation
 ******************************************************************************
 */
static int CsrAppPtestInitFlag = 0;

int CsrAppPtestInit(void)
{
#ifndef WIN32
    CsrUint32 result;
    CsrUint8 status;
    CsrUint32 CsrWifiFirmwarePtestLength;   
    CsrUint8 *CsrWifiFirmwarePtest;
    CsrUint8 *pCustomProfileData;
    CsrUint8* pProfileValues;
    int profileLengthInBytes;    
#ifdef CSR_WIFI_TURN_OFF_NORMAL_MODE_AUTOMATICALLY
    extern int wifi_supp_is_on;
#ifdef CSR_WIFI_6030
    extern void CsrThreadSleep(CsrUint16 sleepTimeInMs);
#else
	extern void CsrThreadSleep(CsrUint32 sleepTimeInMs);
#endif
#endif

#if 0 /* debugging purpose */
    {
	extern void CsrWifiSmeConfigureLog(void);
    extern void CsrWifiHipConfigureLog (void);
	CsrWifiSmeConfigureLog ();
    CsrWifiHipConfigureLog ();
    }
#endif

    FUNC_ENTER ();


    if (CsrAppPtestInitFlag)
        return 0;

#ifdef CSR_WIFI_TURN_OFF_NORMAL_MODE_AUTOMATICALLY
    if (wifi_supp_is_on) {
        CsrWifiSmeWifiOffReqSend(CSR_WIFI_SUPPLICANT_IFACEQUEUE);
        CsrThreadSleep (200); /* wait 200 ms */
    }
#endif
     
    CsrAppPtestInitFlag = 1;

    /* Init synchronization object */
    sync = csr_sync_create(NULL);

    /* Activate Synergy ptest module */
	SYNCP(CsrWifiPtestActivateReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE),
			CSR_WIFI_PTEST_ACTIVATE_CFM, status);

    /* Get Chip ID */
    SYNC(CsrWifiPtestChipIdGetReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE),
        CSR_WIFI_PTEST_CHIP_ID_GET_CFM, sync, status, CsrWifiPtestChipIdGetCfmHandler);

    /* Load F/W */
    result = CsrWifiFirmwarePtestAcquire(gChipVersion, &CsrWifiFirmwarePtestLength, &CsrWifiFirmwarePtest);
    if (result != CSR_WIFI_FIRMWARE_PTEST_SUCCESS) {
        return -1;
    }

    SYNCP(CsrWifiPtestFwLoadReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE, CsrWifiFirmwarePtestLength,
            (CsrUint8 *) CsrWifiFirmwarePtest), CSR_WIFI_PTEST_FW_LOAD_CFM, status);

    /* Set Profile values */
    
    if (CsrWifiGetCustomBoardProfileData (gChipVersion, &pCustomProfileData, &profileLengthInBytes) == FALSE) {
        return -1;
    }
  
    pProfileValues = CsrPmemAlloc(profileLengthInBytes);
    CsrMemCpy(pProfileValues, pCustomProfileData, profileLengthInBytes); 
    SYNCP(CsrWifiPtestCfgValueSetReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE,       ((unsigned int)profileLengthInBytes / sizeof(CsrUint16)), (CsrUint16 *)pProfileValues),
            CSR_WIFI_PTEST_CFG_VALUE_SET_CFM, status);
    /*
     * Don't init radio here.
     * Radio should be inited just before starting TX or RX,
     * because some configuration such as MAC are not allowed to change after radio initialization.
     */
#endif     
    return 0;
}


int CsrAppPtestDeinit(void)
{
#ifndef WIN32
    CsrUint8 status;

    FUNC_ENTER ();


    if (!CsrAppPtestInitFlag)
        return 0;

    /* stop radio */
	if (gTxStarted || gRxStarted) {
		SYNCP (CsrWifiPtestRadioStopReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE),
			CSR_WIFI_PTEST_RADIO_STOP_CFM, status);
    }

    gRadioInited = FALSE;
    gTxStarted = FALSE;
    gRxStarted = FALSE;

    /* deactivate ptest module */
	SYNCP (CsrWifiPtestDeactivateReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE),
		CSR_WIFI_PTEST_DEACTIVATE_CFM, status);

    csr_sync_destory(sync);

    CsrAppPtestInitFlag = 0;
#endif
    return 0;
}


static void TxTestParamsPrint(void)
{
#ifndef WIN32
    #define PUT_D(x)	PTEST_DEBUG_PRINT (("\t" #x "=%d\n", x))
    PUT_D(gTestBand);
    PUT_D(gTestChannel);
    PUT_D(gTestSFactor);
    PUT_D(gTestRate);
    PUT_D(gTxTestPowerLevel);
    PUT_D(gTxTestLength);
    PUT_D(gTxTestEnable11bCsTestMode);
    PUT_D(gTxTestInterval);
#endif    
}


int CsrAppPtestTxStart(int tx_packet_mode)
{
#ifndef WIN32
    CsrUint8 status;
    #define CMD_TX_MODE_TXFRAME (0)
    #define CMD_TX_MODE_TX99    (1)
    #define CMD_TX_MODE_TX100   (2)
    #define CMD_TX_MODE_SINE    (3)

    FUNC_ENTER();


    if (gTxStarted)
        return 0;

    /* Init radio : Should be called only once. */
	if (!gRadioInited) {
		SYNCP (CsrWifiPtestRadioInitReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE),
			CSR_WIFI_PTEST_RADIO_INIT_CFM, status);
        gRadioInited = TRUE;
    }

#ifdef CSR_WIFI_PTEST_LIMIT_DATA_LENGTH_FOR_DSSS_PACKETS
    /* The maxPacketLength in N4010 can support 5.192ms. But,
     * The data length of 1Mbps with 1000 bytes takes around 8.5 ms.
     */
    if (gTestRate == 1 || gTestRate == 2)
        gTxTestLength = 400;
    else
        gTxTestLength = PTEST_TX_FRAME_LEN_DEFAULT;
#endif

    TxTestParamsPrint();
    if (tx_packet_mode == CMD_TX_MODE_SINE) {
    	SYNCP (CsrWifiPtestRadioCwStartReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE, gTestBand, 
                gTestChannel, gTestSFactor, 0, 0, gTxAmplitude), CSR_WIFI_PTEST_RADIO_CW_START_CFM, status);
    }
    else {
        /* Configure TX parameters. NOTE. This should be called after radio initialization. */
    	SYNCP (CsrWifiPtestRadioTxParamsSetReqSend(
    			CSR_WIFI_PTEST_APP_IFACEQUEUE, 
    			gTestBand, 
    			gTestChannel, 
    			gTestSFactor, 
    			gTestRate, 
    			gTxTestPowerLevel, 
    			gTxTestLength, 
    			gTxTestEnable11bCsTestMode,
    			gTxTestInterval,
    			gTxTestMacAddr,
    			gTxPreamble),
    		    CSR_WIFI_PTEST_RADIO_TX_PARAMS_SET_CFM, 
    		    status);
        /* Start TX */
    	SYNCP (CsrWifiPtestRadioTxStartReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE),
    		CSR_WIFI_PTEST_RADIO_TX_START_CFM, status);
    }
    
    gTxStarted = TRUE;

#endif    
    return 0;
}


int CsrAppPtestTxStop(int tx_packet_mode)
{
#ifndef WIN32
    CsrUint8 status;
    
    FUNC_ENTER();


    if (!gTxStarted)
        return 0;

    if (tx_packet_mode == CMD_TX_MODE_SINE) {
    	SYNCP (CsrWifiPtestRadioStopReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE),
    		CSR_WIFI_PTEST_RADIO_STOP_CFM, status);
    }
    else {
    	SYNCP (CsrWifiPtestRadioStopReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE),
    		CSR_WIFI_PTEST_RADIO_STOP_CFM, status);
    }

    gTxStarted = FALSE;
#endif    
    return 0;
}


int CsrAppPtestRxStart(void)
{
#ifndef WIN32
    CsrUint8 status;

    FUNC_ENTER();


    if (gRxStarted)
        return 0;

    /* Init radio : Should be called only once. */
    if (!gRadioInited) {
        SYNCP(CsrWifiPtestRadioInitReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE), CSR_WIFI_PTEST_RADIO_INIT_CFM, status);
        gRadioInited = TRUE;
    }

    /* Start RX */
    SYNCP(CsrWifiPtestRadioRxStartReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE, gTestBand, 
            gTestChannel, gTestSFactor /* only for 11a */, 
            0,    /* frequency */
            0 /* filteringEnable */
            ), CSR_WIFI_PTEST_RADIO_RX_START_CFM, status);

    gRxStarted = TRUE;
#endif    
    return 0;
}


int CsrAppPtestRxStop(void)
{
#ifndef WIN32
    CsrUint8 status;

    FUNC_ENTER();


    if (!gRxStarted)
        return 0;
    /*
     * Read RX statistics before stopping RX, 
     * because those are not available after after stopping RX in case of UniFi.
     * The read values will be stored at gBadFrames, gGoodFrames, etc.
     */
    PTEST_RADIO_RX_READ(&status);
    PTEST_CHECK_RESULT(status);

    /* Stop radio */
    SYNCP(CsrWifiPtestRadioStopReqSend(CSR_WIFI_PTEST_APP_IFACEQUEUE), CSR_WIFI_PTEST_RADIO_STOP_CFM, status);

    gRxStarted = FALSE;
#endif    
    return 0;
}


int CsrAppPtestRxResult(CsrUint32 *pGoodFrames, CsrUint32 *pBadFrames, CsrUint32 *pTestRssi)
{
#ifndef WIN32

    CsrUint8 status;

    if (gRxStarted) {
        PTEST_RADIO_RX_READ(&status);
        PTEST_CHECK_RESULT(status);
    } 

    *pGoodFrames = gGoodFrames;
    *pBadFrames = gBadFrames;
    *pTestRssi = gTestRssi;

    PTEST_DEBUG_PRINT(("last rx-result info\r\n"));
    PTEST_DEBUG_PRINT(("  good:  %d\r\n", gGoodFrames));
    PTEST_DEBUG_PRINT(("   bad: %d\r\n", gBadFrames));
    PTEST_DEBUG_PRINT(("total:    %d\r\n", gGoodFrames + gBadFrames));
    
    if ((gGoodFrames + gBadFrames) != 0)
        PTEST_DEBUG_PRINT(("   PER:  %d %%\r\n", 100*gBadFrames/(gGoodFrames + gBadFrames)));
#endif
    return 0;
}




/****************************************************************************/
//Description : enter eut mode
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTEnterMode(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_enter)
{
    WIFISUPP_BOOL is_success = TRUE;
#ifndef WIN32
    FUNC_ENTER ();


    /* enter */
    if (is_enter) {
        if (CsrAppPtestInit () != 0)
            is_success = FALSE;
    }
    /* exit */
    else {
        if (CsrAppPtestDeinit () != 0)
            is_success = FALSE;
    }
#endif
    WIFISUPP_EUTEnterModeCnf (handle, is_success);
    return TRUE;
}

/****************************************************************************/
//Description : set channel 1~13
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetChannel(WIFISUPP_EUT_HANDLE_T handle, long channel)
{
#ifndef WIN32
     gTestChannel = channel;
     PTEST_DEBUG_PRINT(("<WIFISUPP_EUTSetChannel = %d>\r\n", channel));
#endif
    WIFISUPP_EUTSetChannelCnf (handle, TRUE);
    return TRUE;
}

/****************************************************************************/
//Description : set data ratio, 1Mbps, 2Mbps, 5.5, 11. 6, 9, 12, 18, 24, 36, 48, 54Mbps
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetDataRatio(WIFISUPP_EUT_HANDLE_T handle, long ratio)
{
#ifndef WIN32
     gTestRate = ratio & 0xffff;
    PTEST_DEBUG_PRINT(("<WIFISUPP_EUTSetDataRatio, gTestRate = %d (long ratio=%d)>\r\n", gTestRate, ratio));
#endif
    WIFISUPP_EUTSetDataRatioCnf(handle, TRUE);
    return TRUE;
}

/****************************************************************************/
//Description : set wifi mode
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetMode(WIFISUPP_EUT_HANDLE_T handle, WIFI_MODE_E mode)
{
#ifndef WIN32
    /*
    	WIFI_MODE_DSSS_BPSK = 0,
    	WIFI_MODE_DSSS_DQPSK,
    	WIFI_MODE_DSSS_CCK,
    	WIFI_MODE_OFDM_BPSK,
    	WIFI_MODE_OFDM_DQPSK,
    	WIFI_MODE_16QAM,
    	WIFI_MODE_64QAM,
    	WIFI_MODE_MAX
    */
    PTEST_DEBUG_PRINT(("<WIFISUPP_EUTSetMode = %d>\r\n", mode));
#endif
    /* According to Spreadtrum, WIFI_MODE configuration can be ignored */
    WIFISUPP_EUTSetModeCnf(handle, TRUE);
    return TRUE;
}

/****************************************************************************/
//Description : set tx factor
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTxFactor(WIFISUPP_EUT_HANDLE_T handle, long pcl, long tx_factor)
{
#ifndef WIN32
    PTEST_DEBUG_PRINT(("<WIFISUPP_EUTSetTxFactor, pcl = %d, tx_factor = %d>\r\n", pcl, tx_factor));    
#endif
    WIFISUPP_EUTSetTxFactorCnf(handle, TRUE);
    return TRUE;
}

/****************************************************************************/
//Description :  set pcl
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetPCL(WIFISUPP_EUT_HANDLE_T handle, long pcl)
{
#ifndef WIN32
    PTEST_DEBUG_PRINT(("<WIFISUPP_EUTSetPCL, pcl = %d\r\n", pcl));
#endif
    WIFISUPP_EUTSetPCLCnf(handle, TRUE);
    return TRUE;
}

/****************************************************************************/
//Description : set tx on/off
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTx(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_tx_on, int tx_packet_mode)
{
#ifndef WIN32
    PTEST_DEBUG_PRINT(("<WIFISUPP_EUTSetTx, is_tx_on = %d, tx_packet_mode = %d\r\n", is_tx_on, tx_packet_mode));
    if (is_tx_on)
        CsrAppPtestTxStart(tx_packet_mode);
    else 
        CsrAppPtestTxStop(tx_packet_mode);
    
#endif
    WIFISUPP_EUTSetTxCnf(handle, TRUE);
    return TRUE;
}

/****************************************************************************/
//Description : set rx on/off
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetRx(WIFISUPP_EUT_HANDLE_T handle, int rx_command)
{
    CsrUint32 good, bad, rssi;
    WIFISUPP_BOOL is_success = TRUE;

#ifndef WIN32
    
    /* 0: off, 1: on, 2: clear */
    #define CMD_RX_OFF      (0)
    #define RX_CMD_ON       (1)
    #define CMD_RX_CLEAR    (2)
    
    PTEST_DEBUG_PRINT(("<WIFISUPP_EUTSetRx, rx_command = %d\r\n", rx_command));
    if (rx_command == CMD_RX_OFF) {
        /* Before stop, let's update the RX results */
        CsrAppPtestRxResult (&good, &bad, &rssi);
        if (CsrAppPtestRxStop () != 0)
            is_success = FALSE;
    }
    else if (rx_command == RX_CMD_ON) {
        if (CsrAppPtestRxStart() != 0)
            is_success = FALSE;
    }
    else if (rx_command == CMD_RX_CLEAR) {
        gGoodFrames = 0;
        gBadFrames = 0;
        gTestRssi = 0;
    }
#endif    
    WIFISUPP_EUTSetRxCnf(handle, is_success);
    return TRUE;
}

/****************************************************************************/
//Description : read rx total packets and error packets
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadRxVal(WIFISUPP_EUT_HANDLE_T handle)
{
    CsrUint32 good, bad, rssi;
#ifndef WIN32
    FUNC_ENTER ();


    CsrAppPtestRxResult (&good, &bad, &rssi);
#endif
    WIFISUPP_EUTReadRxValCnf(handle, TRUE, (good+bad), bad);
    return TRUE;
}

unsigned char wifi_eut_setmac(unsigned char* mac_addr)
{
#ifndef WIN32
    FUNC_ENTER ();


    /* Nothing to do */
#endif
    return TRUE;
}

/* ptest read and write register routines, exported */
extern int unihosttran_write_block(uint32 addr, uint16 numDwords, uint16  *pData);
extern int unihosttran_read_block(uint32 addr, uint16 numDwords, uint16  *pData);

PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadRegister(WIFISUPP_EUT_HANDLE_T handle, uint32 addr, uint32 *pData)
{
#ifndef WIN32
    int rc;
    uint16 my_to_read;
    
    rc = unihosttran_read_block( addr, 1, &my_to_read );
    
    *pData = my_to_read;
    if( rc )
    {
        return FALSE;
    }
#endif
    return TRUE;
}

PUBLIC WIFISUPP_BOOL WIFISUPP_EUTWriteRegister(WIFISUPP_EUT_HANDLE_T handle, uint32 addr, uint32 Data)
{
#ifndef WIN32
    int rc;
    uint16 my_to_write = Data & 0xffff;
    
            
    rc = unihosttran_write_block( addr, 1, &my_to_write);
    
    if( rc )
    {
        return FALSE;
    }
#endif
    return TRUE;
}

/*请注意，这个函数应该在第三方库中定义而不应该在这里*/
CsrCharString CsrWiFiRelVer[128] = "CSR SM "CSR_RELEASE_VERSION": ";
char * WIFI_GetVersion(void)
{
    SCI_TraceLow("CSR:WIFI_GetVersion - %s", CsrWiFiRelVer);
    return CsrWiFiRelVer;
}

