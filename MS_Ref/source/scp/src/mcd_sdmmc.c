/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/





#include "mcd_m.h"
#include "mcdp_sdmmc.h"

#include "hal_sdmmc.h"

#include "stdlib.h"
#include "stdio.h"
#include "os_api.h"
#include "timer_drvapi.h"

/*
#include "ifc_service.h"
#include "lp_timer_service.h"
*/


//#include "sxs_io.h"
//#include "sxr_sbx.h"

//#include "cos.h"

// =============================================================================
//  MACROS
// =============================================================================
//#define _TEST_SDMMC_

#define BROAD_ADDR  0
#define NOT_SDMMC_OVER  (1<<0)

#define MCD_MAX_BLOCK_NUM       128

#define MCD_PROFILE_FUNCTION_EXIT(fun)
#define MCD_PROFILE_FUNCTION_ENTER(fun)


#define MCD_SDMMC_OCR_TIMEOUT   16384 //(SECONDS)  // the card is supposed to answer within 1s
//  - Max wait is 128 * 10ms=1,28s
//  TODO: where those 10ms come from ??

// Command 8 things: cf spec Vers.2 section 4.3.13
#define MCD_CMD8_CHECK_PATTERN      0xaa
#define MCD_CMD8_VOLTAGE_SEL        (0x1<<8)
#define MCD_OCR_HCS                 (1<<30)
#define MCD_OCR_CCS_MASK            (1<<30)

#define SECONDS (16384)
// Timeouts for V1
#define MCD_CMD_TIMEOUT_V1        ( SECONDS / 2 )
#define MCD_RESP_TIMEOUT_V1       ( SECONDS / 2 )
#define MCD_TRAN_TIMEOUT_V1       ( SECONDS / 2 )
#define MCD_READ_TIMEOUT_V1       (2* SECONDS )
#define MCD_WRITE_TIMEOUT_V1      (2* SECONDS )

// Timeouts for V2
#define MCD_CMD_TIMEOUT_V2        ( SECONDS / 10 )
#define MCD_RESP_TIMEOUT_V2       ( SECONDS / 10 )
#define MCD_TRAN_TIMEOUT_V2       ( SECONDS / 10 )
#define MCD_READ_TIMEOUT_V2       ( SECONDS / 2 )
#define MCD_WRITE_TIMEOUT_V2      (SECONDS / 2 )
#define MCD_ERASE_TIMEOUT         (SECONDS *2 )

#define COS_LOGI(index,format,...)        SCI_TraceLow( format, ##__VA_ARGS__)

#define SCI_TICKTOMILLISECOND(_TICK)        (((uint64)_TICK *1000)  / 16384)

#ifdef UART_DEBUG_POWER_ON

#ifdef SCI_ASSERT
#undef SCI_ASSERT
#define SCI_ASSERT(_EXP)  if((_EXP) == 0){SCI_TraceLow_Test("func:%s, line:%d", __func__, __LINE__);while(1);}
#endif

#ifdef SCI_PASSERT
#undef SCI_PASSERT
#define SCI_PASSERT(x, y)  if((x) == 0){SCI_TraceLow_Test("func:%s, line:%d", __func__, __LINE__);while(1);}
#endif

#else   //UART_DEBUG_POWER_ON

#ifdef SCI_ASSERT
#undef SCI_ASSERT
#define SCI_ASSERT(_EXP)   NULL
#endif

#endif //UART_DEBUG_POWER_ON
#if !defined(OS_NONE)
extern uint32 DIAG_need_SCM_sleep;
#endif
// =============================================================================
// TYPES
// =============================================================================

typedef struct
{
    uint8   mid;
    uint32  oid;
    uint32  pnm1;
    uint8   pnm2;
    uint8   prv;
    uint32  psn;
    uint32  mdt;
    uint8   crc;
} MCD_CID_FORMAT_T;

MCD_HANDLE_CDS_T g_handleCsd;

BOOLEAN g_small_card = FALSE;
// =============================================================================
// Global variables
// =============================================================================

// Spec Ver2 p96
#define MCD_SDMMC_OCR_VOLT_PROF_MASK  0x00ff8000

uint32 g_mcdOcrReg = MCD_SDMMC_OCR_VOLT_PROF_MASK;

/// Relative Card Address Register
/// Nota RCA is sixteen bit long, but is always used
/// as the 16 upper bits of a 32 bits word. Therefore
/// is variable is in fact (RCA<<16), to avoid operation
/// (shift, unshift), to always place the RCA value as the
/// upper bits of a data word.
uint32 g_mcdRcaReg = 0x00000000;

// Driver Stage Register p.118
// (To adjust bus capacitance)
// TODO Tune and put in calibration ?
uint32 g_mcdSdmmcDsr = 0x04040000;


uint32 g_mcdSdmmcFreq = 200000;

MCD_CID_FORMAT_T  g_mcdCidReg;
uint32            g_mcdBlockLen   = 0;
uint32            g_mcdNbBlock    = 0;
BOOLEAN              g_mcdCardIsSdhc = FALSE;
BOOLEAN              g_mcdCardIsBusy = FALSE;
BOOLEAN              g_mcdCardIsError = FALSE;

MCD_STATUS_T      g_mcdStatus = MCD_STATUS_NOTOPEN_PRESENT;

// CONST TGT_MCD_CONFIG_T*     g_mcdConfig=NULL;
//MCD_CARD_DETECT_HANDLER_T   g_mcdCardDetectHandler=NULL;

/// Semaphore to ensure proper concurrency of the MCD accesses
/// among all tasks.
uint32           g_mcdSemaphore   = 0xFF;
void mmc_event(uint32 e)
{
    mon_Event(e);
}

/// Current in-progress transfer, if any.
HAL_SDMMC_TRANSFER_T g_mcdSdmmcTransfer =
{
    0,// .sysMemAddr = 0,
    0,// .sdCardAddr = 0,
    0,//.blockNum   = 0,
    0,//.blockSize  = 0,
    HAL_SDMMC_DIRECTION_WRITE,// .direction  = HAL_SDMMC_DIRECTION_WRITE,
};

MCD_CSD_T g_mcdLatestCsd;

MCD_CARD_VER g_mcdVer = MCD_CARD_V2;
// =============================================================================
// Functions
// =============================================================================
#define MCD_ASSERT  SCI_ASSERT
// =============================================================================
// Functions for the HAL Driver ?
// =============================================================================

#define PMD_POWER_SDMMC 0
#define REG_RDA2720M_GLOBAL_BASE  0xc00
#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E) 
#define LDO_SD_PD_REG 0xF4
#else
#define LDO_SD_PD_REG 0x100
#define LDO_SDIO_PD_REG 0x10c
#endif

extern PUBLIC void SCI_SDIO_EnableDeepSleep (uint32 mode);
SCI_SEMAPHORE_PTR   s_sdmmc_sm_ptr = PNULL;

uint32 hal_SysEnterCriticalSection1(void)
{
    SCI_DisableIRQ();/*lint !e527*/
    //SCI_DisableFIQ();
    return 0;
}
void hal_SysExitCriticalSection1(uint32 status)
{
    SCI_RestoreIRQ();
    //SCI_RestoreFIQ();

}
#if 0
/// Macro to easily implement concurrency in the MCD driver.
/// Enter in the 'critical section'.
#define MCD_CS_ENTER do {SCI_GetSemaphore(s_sdmmc_sm_ptr, SCI_WAIT_FOREVER) ;} while(0) 


/// Macro to easily implement concurrency in the MCD driver.
/// Exit in the 'critical section'.
#define MCD_CS_EXIT  do {SCI_PutSemaphore( s_sdmmc_sm_ptr ); } while(0)
#else

#define MCD_CS_ENTER

/// Macro to easily implement concurrency in the MCD driver.
/// Exit in the 'critical section'.
#define MCD_CS_EXIT
#endif

MCD_ERR_T  mcd_SdmmcModeSwitch(uint32 arg);


void mcd_delay(uint32 ms)
{
#if 0
    uint32 wait_time = 0;

    wait_time = timer_TimRealTickGet(OS_TIMER);
    if((timer_TimRealTickGet(OS_TIMER) - wait_time) > ms)
    {
        return;
    }
#else
    SCI_Sleep(SCI_TICKTOMILLISECOND(ms));   //ms is tick(16384)
#endif
}

void mcd_hal_delay_sd1(uint32 tktnum)
{


#if 0
    uint32 wait_time = 0;

    wait_time = timer_TimRealTickGet(OS_TIMER);
    while(1)
    {
        if((timer_TimRealTickGet(OS_TIMER) - wait_time) > tktnum)
        {
            //mmc_event(0xdd000007);

            return;
        }
    }
#else
    SCI_Sleep(SCI_TICKTOMILLISECOND(tktnum));   //tktnum is tick(16384)
#endif
}




void mcd_hal_delay(uint32 ms)
{
#if 1
#if 0
    uint32 wait_time = 0;

    wait_time = timer_TimRealTickGet(OS_TIMER);
    while(1)
    {
        if((timer_TimRealTickGet(OS_TIMER) - wait_time) > ms)
        {
            return;
        }
    }
#else
    SCI_Sleep(SCI_TICKTOMILLISECOND(ms));   //ms is tick(16384)
#endif
#endif
}


MCD_ERR_T mcd_GetCardSize(MCD_CARD_SIZE_T* size)
{
    MCD_CS_ENTER;

    size->blockLen = g_mcdBlockLen;
    size->nbBlock  = g_mcdNbBlock;

    MCD_CS_EXIT;
    return MCD_ERR_NO;
}

// Wait for a command to be done or timeouts
MCD_ERR_T mcd_SdmmcWaitCmdOver(void)
{
    uint32 startTime ;
    uint32 time_out;
    startTime = timer_TimRealTickGet(OS_TIMER);
    // time_out =  (MCD_CARD_V1 == g_mcdVer) ? MCD_CMD_TIMEOUT_V1:MCD_CMD_TIMEOUT_V2;
    time_out =  MCD_CMD_TIMEOUT_V1;//(MCD_CARD_V1 == g_mcdVer) ? MCD_CMD_TIMEOUT_V1:MCD_CMD_TIMEOUT_V2;

    while(timer_TimRealTickGet(OS_TIMER) - startTime < time_out && !hal_SdmmcCmdDone());

    if (!hal_SdmmcCmdDone())
    {
        SCI_TraceLow("SDC Waiting is Time out");
        return MCD_ERR_CARD_TIMEOUT;
    }
    else
    {
        return MCD_ERR_NO;
    }
}

/// Update g_mcdStatus
/// @return TRUE is card present (only exact when GPIO is used for card detect.)
BOOLEAN mcd_CardDetectUpdateStatus(void)
{

#if 0
    if(NULL == g_mcdConfig)
    {
        g_mcdConfig = tgt_GetMcdConfig();
    }

    if(g_mcdConfig->cardDetectGpio != HAL_GPIO_NONE)
    {
        BOOLEAN gpioState = !!hal_GpioGet(g_mcdConfig->cardDetectGpio);
        // CARD ?
        if(gpioState == g_mcdConfig->gpioCardDetectHigh)
        {
            // card detected
            switch(g_mcdStatus)
            {
                case MCD_STATUS_OPEN_NOTPRESENT: // wait for the close !
                case MCD_STATUS_OPEN:
                    // No change
                    break;
                default:
                    g_mcdStatus = MCD_STATUS_NOTOPEN_PRESENT;
            }
            return TRUE;
        }
        else
        {
            // no card
            switch(g_mcdStatus)
            {
                case MCD_STATUS_OPEN_NOTPRESENT:
                case MCD_STATUS_OPEN:
                    g_mcdStatus = MCD_STATUS_OPEN_NOTPRESENT;
                    break;
                default:
                    g_mcdStatus = MCD_STATUS_NOTPRESENT;
            }
            return FALSE;
        }
    }
    // estimated
    switch(g_mcdStatus)
    {
        case MCD_STATUS_OPEN:
        case MCD_STATUS_NOTOPEN_PRESENT:
            return TRUE;
        default:
            return FALSE;
    }
#else
    return TRUE;
#endif
}

void mcd_CardDetectHandler(void)
{
    //BOOLEAN CardPresent = mcd_CardDetectUpdateStatus();

    //g_mcdCardDetectHandler(CardPresent);
}

// =============================================================================
// mcd_SetCardDetectHandler
// -----------------------------------------------------------------------------
/// Register a handler for card detection
///
/// @param handler function called when insertion/removal is detected.
// =============================================================================
MCD_ERR_T mcd_SetCardDetectHandler(MCD_CARD_DETECT_HANDLER_T handler)
{
    /*
        if(NULL == g_mcdConfig)
        {
            g_mcdConfig = tgt_GetMcdConfig();
        }

        if(g_mcdConfig->cardDetectGpio == HAL_GPIO_NONE)
        {
            return MCD_ERR_NO_HOTPLUG;
        }

        if(NULL != handler)
        {
            HAL_GPIO_CFG_T cfg  =
            {
                .direction      = HAL_GPIO_DIRECTION_INPUT,
                .irqMask        =
                {
                    .rising     = TRUE,
                    .falling    = TRUE,
                    .debounce   = TRUE,
                    .level      = FALSE
                },
                .irqHandler     = (HAL_GPIO_IRQ_HANDLER_T)mcd_CardDetectHandler
            };

            hal_GpioOpen(g_mcdConfig->cardDetectGpio, &cfg);
            g_mcdCardDetectHandler = handler;
        }
        else
        {
            hal_GpioClose(g_mcdConfig->cardDetectGpio);
            g_mcdCardDetectHandler = NULL;
        }
    */
    return MCD_ERR_NO;
}

// =============================================================================
// mcd_CardStatus
// -----------------------------------------------------------------------------
/// Return the card status
///
/// @return Card status see #MCD_STATUS_T
// =============================================================================
#if 0
MCD_STATUS_T mcd_CardStatus(MCD_CARD_ID mcdId)
{
    mcd_CardDetectUpdateStatus();
    return g_mcdStatus;
}
#endif
// =============================================================================
// mcd_SdmmcWaitResp
// -----------------------------------------------------------------------------
/// Wait for a response for a time configured by MCD_RESP_TIMEOUT
/// @return MCD_ERR_NO if a response with a good crc was received,
///         MCD_ERR_CARD_NO_RESPONSE if no response was received within the
/// driver configured timeout.
//          MCD_ERR_CARD_RESPONSE_BAD_CRC if the received response presented
//  a bad CRC.
// =============================================================================
MCD_ERR_T mcd_SdmmcWaitResp(void)
{
    HAL_SDMMC_OP_STATUS_T status ;
    uint32 startTime ;
    uint32 rsp_time_out;
    uint32 counttimer=0xffff;
    startTime = timer_TimRealTickGet(OS_TIMER);

    status = hal_SdmmcGetOpStatus();
    //  rsp_time_out =  (MCD_CARD_V1 == g_mcdVer) ? MCD_RESP_TIMEOUT_V1:MCD_RESP_TIMEOUT_V2;
    rsp_time_out = MCD_RESP_TIMEOUT_V1;
    while((timer_TimRealTickGet(OS_TIMER) - startTime < rsp_time_out )&&status.fields.noResponseReceived)
    {
#if !defined(OS_NONE)
       if (DIAG_need_SCM_sleep)
            mcd_hal_delay_sd1(17);
#endif
        status = hal_SdmmcGetOpStatus();
    }

    if (status.fields.noResponseReceived)
    {
        SCI_TraceLow("Response is Time out");
        return MCD_ERR_CARD_NO_RESPONSE;
    }

    if(status.fields.responseCrcError)
    {
        SCI_TraceLow("Response CRC is Error");
        return MCD_ERR_CARD_RESPONSE_BAD_CRC;
    }

    return MCD_ERR_NO;
}



//=============================================================================
// mcd_SdmmcReadCheckCrc
//-----------------------------------------------------------------------------
/// Check the read state of the sdmmc card.
/// @return MCD_ERR_NO if the Crc of read data was correct,
/// MCD_ERR_CARD_RESPONSE_BAD_CRC otherwise.
//=============================================================================
MCD_ERR_T mcd_SdmmcReadCheckCrc(void)
{
    HAL_SDMMC_OP_STATUS_T operationStatus;
    operationStatus = hal_SdmmcGetOpStatus();
    if (operationStatus.fields.dataError != 0) // 0 means no CRC error during transmission
    {
        // SCI_TraceLow(0x10001936, "sdC status:%0x", operationStatus.reg);
        return MCD_ERR_CARD_RESPONSE_BAD_CRC;
    }
    else
    {
        return MCD_ERR_NO;
    }
}


//=============================================================================
// mcd_SdmmcWriteCheckCrc
//-----------------------------------------------------------------------------
/// Check the crc state of the write operation of the sdmmc card.
/// @return MCD_ERR_NO if the Crc of read data was correct,
/// MCD_ERR_CARD_RESPONSE_BAD_CRC otherwise.
//=============================================================================
MCD_ERR_T mcd_SdmmcWriteCheckCrc(void)
{
    HAL_SDMMC_OP_STATUS_T operationStatus;
    operationStatus = hal_SdmmcGetOpStatus();

    if (operationStatus.fields.crcStatus != 2) // 0b010 = transmissionRight TODO a macro ?
    {
        //SCI_TraceLow(0x10001936, "sdC status:%0x", operationStatus.reg);
        return MCD_ERR_CARD_RESPONSE_BAD_CRC;
    }
    else
    {
        return MCD_ERR_NO;
    }
}

void   pmd_EnablePower(uint8 sdId,BOOLEAN on)
{

    if(on == FALSE)
    {
        mcd_hal_delay_sd1(50);
#if defined(OS_NONE)
        CHIP_PHY_SetANAReg(REG_RDA2720M_GLOBAL_BASE+LDO_SD_PD_REG,1);
        SCI_TraceLow("SDMMC Poweroff");
#else
        ANA_REG_SET(REG_RDA2720M_GLOBAL_BASE+LDO_SD_PD_REG,1);
#endif
#if !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A) && !defined(CHIP_VER_UWS6121E) 
        mcd_hal_delay_sd1(50);
        ANA_REG_SET(REG_RDA2720M_GLOBAL_BASE+LDO_SDIO_PD_REG,1);
#endif

    }
    else
    {
        mcd_hal_delay_sd1(50);
#if defined(OS_NONE)
        CHIP_PHY_SetANAReg(REG_RDA2720M_GLOBAL_BASE+LDO_SD_PD_REG,0);
        SCI_TraceLow("SDMMC Poweron");
#else
        ANA_REG_SET(REG_RDA2720M_GLOBAL_BASE+LDO_SD_PD_REG,0);
#endif
#if !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIS8910C)  && !defined(CHIP_VER_UIS8910A) && !defined(CHIP_VER_UWS6121E) 
        mcd_hal_delay_sd1(50);
        ANA_REG_SET(REG_RDA2720M_GLOBAL_BASE+LDO_SDIO_PD_REG,0);
#endif

    }

}




// =============================================================================
// mcd_SdmmcSendCmd
// -----------------------------------------------------------------------------
/// Send a command to the card, and fetch the response if one is expected.
/// @param cmd CMD to send
/// @param arg Argument of the ACMD.
/// @param resp Buffer to store card response.
/// @param suspend Not supported.
/// @return MCD_ERR_NO if a response with a good crc was received,
///         MCD_ERR_CARD_NO_RESPONSE if no reponse was received within the
/// driver configured timeout.
///          MCD_ERR_CARD_RESPONSE_BAD_CRC if the received response presented
///  a bad CRC.
///         MCD_ERR_CARD_TIMEOUT if the card timedout during procedure.
// =============================================================================
MCD_ERR_T mcd_SdmmcSendCmd(HAL_SDMMC_CMD_T cmd, uint32 arg,
                           uint32* resp, BOOLEAN suspend)
{
    MCD_ERR_T errStatus = MCD_ERR_NO;
    MCD_CARD_STATUS_T cardStatus = {0};
    uint32  cmd55Response[4] = {0, 0, 0, 0};
    if (cmd & HAL_SDMMC_ACMD_SEL)
    {
        // This is an application specific command,
        // we send the CMD55 first
        hal_SdmmcSendCmd(HAL_SDMMC_CMD_APP_CMD, g_mcdRcaReg, FALSE);

        // Wait for command over
        if (MCD_ERR_CARD_TIMEOUT == mcd_SdmmcWaitCmdOver())
        {
            SCI_TraceLow("Cmd55 Send is Time out");
            return MCD_ERR_CARD_TIMEOUT;
        }

        // Wait for response
        if (hal_SdmmcNeedResponse(HAL_SDMMC_CMD_APP_CMD))
        {
            errStatus = mcd_SdmmcWaitResp();
        }
        if (MCD_ERR_NO != errStatus)
        {
            SCI_TraceLow("cmd55 response error");
            return errStatus;
        }

        // Fetch response
        hal_SdmmcGetResp(HAL_SDMMC_CMD_APP_CMD, cmd55Response, FALSE);

        cardStatus.reg = cmd55Response[0];
        if(HAL_SDMMC_CMD_SEND_OP_COND == cmd) // for some special card
        {
            //if (!(cardStatus.fields.readyForData) || !(cardStatus.fields.appCmd))
            if (!(cardStatus.fields.appCmd))
            {
                SCI_TraceLow("cmd55(acmd411) status=%0x", cardStatus.reg);
                return MCD_ERR;
            }
        }
        else
        {
            if (!(cardStatus.fields.readyForData) || !(cardStatus.fields.appCmd))
            {
                SCI_TraceLow("cmd55 status=%0x", cardStatus.reg);
                return MCD_ERR;
            }

        }
    }
    // Send proper command. If it was an ACMD, the CMD55 have just been sent.
    hal_SdmmcSendCmd(cmd, arg, suspend);

    // Wait for command to be sent
    errStatus = mcd_SdmmcWaitCmdOver();


    if (MCD_ERR_CARD_TIMEOUT == errStatus)
    {
        if (cmd & HAL_SDMMC_ACMD_SEL)
        {
            SCI_TraceLow("ACMD %d Sending Timed out", (cmd & HAL_SDMMC_CMD_MASK));
        }
        else
        {
            SCI_TraceLow("CMD %d Sending Timed out", (cmd & HAL_SDMMC_CMD_MASK));
        }
        return MCD_ERR_CARD_TIMEOUT;
    }

    // Wait for response and get its argument
    if (hal_SdmmcNeedResponse(cmd))
    {
        errStatus = mcd_SdmmcWaitResp();
    }

    if (MCD_ERR_NO != errStatus)
    {
        if (cmd & HAL_SDMMC_ACMD_SEL)
        {
            SCI_TraceLow("ACMD %d Response Error", (cmd & HAL_SDMMC_CMD_MASK));
        }
        else
        {
            SCI_TraceLow("CMD %d Response Error", (cmd & HAL_SDMMC_CMD_MASK));
            return errStatus;
        }
    }

    // Fetch response
    hal_SdmmcGetResp(cmd, resp, FALSE);

    //FOR DEBUG: MCD_TRACE(MCD_INFO_TRC, 0, "CMD %d Response is %#x", (cmd & HAL_SDMMC_CMD_MASK), resp[0]);

    return MCD_ERR_NO;
}




// =============================================================================
// mcd_SdmmcInitCid
// -----------------------------------------------------------------------------
/// Set the CID in the driver from the data read on the card.
/// @param cid 4 word array read from the card, holding the CID register value.
// =============================================================================
MCD_ERR_T mcd_SdmmcInitCid(uint32* cid)
{
    // Fill the structure with the register bitfields value.
    g_mcdCidReg.mid     = (uint8)((cid[3]&(0xff<<24))>>24);
    g_mcdCidReg.oid     = (cid[3]&(0xffff<<8))>>8;
    g_mcdCidReg.pnm2    = (uint8)(cid[3]&0xff);
    g_mcdCidReg.pnm1    = cid[2];
    g_mcdCidReg.prv     = (uint8)((cid[1]&(0xff<<24))>>24);
    g_mcdCidReg.psn     = (cid[1]&0xffffff)<<8;
    g_mcdCidReg.psn     = g_mcdCidReg.psn|((cid[0]&(0xff<<23))>>23);
    g_mcdCidReg.mdt     = (cid[0]&(0xfff<<7))>>7;
    g_mcdCidReg.crc     = (uint8)(cid[0]&0x7f);

    return MCD_ERR_NO;
}


#define MCD_CSD_VERSION_1       0
#define MCD_CSD_VERSION_2       1
// =============================================================================
// mcd_SdmmcInitCsd
// -----------------------------------------------------------------------------
/// Fill MCD_CSD_T structure from the array of data read from the card
///
/// @param csd Pointer to the structure
/// @param csdRaw Pointer to the raw data.
/// @return MCD_ERR_NO
// =============================================================================
MCD_ERR_T mcd_SdmmcInitCsd(MCD_CSD_T* csd, uint32* csdRaw)
{
    // CF SD spec version2, CSD version 1 ?
    csd->csdStructure       = (uint8)((csdRaw[3]&(0x3<<30))>>30);

    // Byte 47 to 75 are different depending on the version
    // of the CSD srtucture.
    csd->specVers           = (uint8)((csdRaw[3]&(0xf<<26))>>26);
    csd->taac               = (uint8)((csdRaw[3]&(0xff<<16))>>16);
    csd->nsac               = (uint8)((csdRaw[3]&(0xff<<8))>>8);
    csd->tranSpeed          = (uint8)(csdRaw[3]&0xff);

    csd->ccc                = (csdRaw[2]&(0xfff<<20))>>20;
    csd->readBlLen          = (uint8)((csdRaw[2]&(0xf<<16))>>16);
    csd->readBlPartial      = (uint8)((csdRaw[2]&(0x1<<15))>>15);
    csd->writeBlkMisalign   = (uint8)((csdRaw[2]&(0x1<<14))>>14);
    csd->readBlkMisalign    = (uint8)((csdRaw[2]&(0x1<<13))>>13);
    csd->dsrImp             = (uint8)((csdRaw[2]&(0x1<<12))>>12);

    if (csd->csdStructure == MCD_CSD_VERSION_1)
    {
        csd->cSize              = (csdRaw[2]&0x3ff)<<2;

        csd->cSize              = csd->cSize|((csdRaw[1]&(0x3<<30))>>30);
        csd->vddRCurrMin        = (uint8)((csdRaw[1]&(0x7<<27))>>27);
        csd->vddRCurrMax        = (uint8)((csdRaw[1]&(0x7<<24))>>24);
        csd->vddWCurrMin        = (uint8)((csdRaw[1]&(0x7<<21))>>21);
        csd->vddWCurrMax        = (uint8)((csdRaw[1]&(0x7<<18))>>18);
        csd->cSizeMult          = (uint8)((csdRaw[1]&(0x7<<15))>>15);

        // Block number: cf Spec Version 2 page 103 (116).
        csd->blockNumber        = (csd->cSize + 1)<<(csd->cSizeMult + 2);
    }
    else
    {
        // csd->csdStructure == MCD_CSD_VERSION_2
        csd->cSize = (((csdRaw[2]&0x3f))<<16)|((csdRaw[1]&(0xffff<<16))>>16);

        // Other fields are undefined --> zeroed
        csd->vddRCurrMin = 0;
        csd->vddRCurrMax = 0;
        csd->vddWCurrMin = 0;
        csd->vddWCurrMax = 0;
        csd->cSizeMult   = 0;

        // Block number: cf Spec Version 2 page 109 (122).
        csd->blockNumber        = (csd->cSize + 1) * 1024;
        //should check incompatible size and return MCD_ERR_UNUSABLE_CARD;
    }

    csd->eraseBlkEnable     = (uint8)((csdRaw[1]&(0x1<<14))>>14);
    csd->sectorSize         = (uint8)((csdRaw[1]&(0x7f<<7))>>7);
    csd->wpGrpSize          = (uint8)(csdRaw[1]&0x7f);

    csd->wpGrpEnable        = (uint8)((csdRaw[0]&(0x1<<31))>>31);
    csd->r2wFactor          = (uint8)((csdRaw[0]&(0x7<<26))>>26);
    csd->writeBlLen         = (uint8)((csdRaw[0]&(0xf<<22))>>22);
    csd->writeBlPartial     = (uint8)((csdRaw[0]&(0x1<<21))>>21);
    csd->fileFormatGrp      = (uint8)((csdRaw[0]&(0x1<<15))>>15);
    csd->copy               = (uint8)((csdRaw[0]&(0x1<<14))>>14);
    csd->permWriteProtect   = (uint8)((csdRaw[0]&(0x1<<13))>>13);
    csd->tmpWriteProtect    = (uint8)((csdRaw[0]&(0x1<<12))>>12);
    csd->fileFormat         = (uint8)((csdRaw[0]&(0x3<<10))>>10);
    csd->crc                = (uint8)((csdRaw[0]&(0x7f<<1))>>1);

#if 0
    SCI_TraceLow(0x1000193f, "CSD:csdStructure = %d", csd->csdStructure        );
    SCI_TraceLow(0x10001940, "CSD:specVers     = %d", csd->specVers           );
    SCI_TraceLow(0x10001941, "CSD:taac         = %d", csd->taac                );
    SCI_TraceLow(0x10001942, "CSD:nsac         = %d", csd->nsac                );
    SCI_TraceLow(0x10001943, "CSD:tranSpeed    = %d", csd->tranSpeed           );
    SCI_TraceLow(0x10001944, "CSD:ccc          = %d", csd->ccc             );
    SCI_TraceLow(0x10001945, "CSD:readBlLen    = %d", csd->readBlLen           );
    SCI_TraceLow(0x10001946, "CSD:readBlPartial = %d", csd->readBlPartial      );
    SCI_TraceLow(0x10001947, "CSD:writeBlkMisalign = %d", csd->writeBlkMisalign    );
    SCI_TraceLow(0x10001948, "CSD:readBlkMisalign  = %d", csd->readBlkMisalign    );
    SCI_TraceLow(0x10001949, "CSD:dsrImp       = %d", csd->dsrImp              );
    SCI_TraceLow(0x1000194a, "CSD:cSize        = %d", csd->cSize               );
    SCI_TraceLow(0x1000194b, "CSD:vddRCurrMin  = %d", csd->vddRCurrMin     );
    SCI_TraceLow(0x1000194c, "CSD:vddRCurrMax  = %d", csd->vddRCurrMax     );
    SCI_TraceLow(0x1000194d, "CSD:vddWCurrMin  = %d", csd->vddWCurrMin     );
    SCI_TraceLow(0x1000194e, "CSD:vddWCurrMax  = %d", csd->vddWCurrMax     );
    SCI_TraceLow(0x1000194f, "CSD:cSizeMult    = %d", csd->cSizeMult           );
    SCI_TraceLow(0x10001950, "CSD:eraseBlkEnable = %d", csd->eraseBlkEnable        );
    SCI_TraceLow(0x10001951, "CSD:sectorSize   = %d", csd->sectorSize          );
    SCI_TraceLow(0x10001952, "CSD:wpGrpSize    = %d", csd->wpGrpSize           );
    SCI_TraceLow(0x10001953, "CSD:wpGrpEnable  = %d", csd->wpGrpEnable     );
    SCI_TraceLow(0x10001954, "CSD:r2wFactor    = %d", csd->r2wFactor           );
    SCI_TraceLow(0x10001955, "CSD:writeBlLen   = %d", csd->writeBlLen          );
#endif
    //SCI_TraceLow( "CSD:writeBlPartial = %d", csd->writeBlPartial        );
    //SCI_TraceLow(  "CSD:fileFormatGrp = %d", csd->fileFormatGrp       );
    //SCI_TraceLow(  "CSD:copy  = %d", csd->copy                 );
    //SCI_TraceLow(  "CSD:permWriteProtect = %d", csd->permWriteProtect      );
    //SCI_TraceLow(  "CSD:tmpWriteProtect  = %d", csd->tmpWriteProtect );
    //SCI_TraceLow(  "CSD:fileFormat       = %d", csd->fileFormat          );
    SCI_TraceLow( "mcd: CSD:tranSpeed    = %d", csd->tranSpeed           );
    SCI_TraceLow(  "mcd: CSD:crc              = %d", csd->crc                );
    SCI_TraceLow(  "mcd: CSD:block number     = %d , about %d M", csd->blockNumber,csd->blockNumber /2048    );

    if(csd->blockNumber < 300000 ) //51200
    {
        if(g_mcdCardIsSdhc == FALSE)
        {
            g_small_card = TRUE;
        }
        //return MCD_ERR_CSD;

    }
    mmc_event(0xccc15555);
    mmc_event(csd->blockNumber);
    return MCD_ERR_NO;
}



// =============================================================================
//  FUNCTIONS (public)
// =============================================================================



// =============================================================================
// mcd_ReadCsd
// -----------------------------------------------------------------------------
/// @brief Read the MMC CSD register
/// @param mcdCsd Pointer to the structure where the MMC CSD register info
/// are going to be written.
// =============================================================================
MCD_ERR_T mcd_ReadCsd(MCD_CSD_T* mcdCsd)
{
    MCD_ERR_T errStatus = MCD_ERR_NO;
    uint32 response[4];

    // Get card CSD
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SEND_CSD, g_mcdRcaReg, response, FALSE);
    if (errStatus == MCD_ERR_NO)
    {
        errStatus = mcd_SdmmcInitCsd(mcdCsd, response);
    }

    // Store it localy
    // FIXME: Is this real ? cf Physical Spec version 2
    // page 59 (72) about CMD16 : default block length
    // is 512 bytes. Isn't 512 bytes a good block
    // length ? And I quote : "In both cases, if block length is set larger
    // than 512Bytes, the card sets the BLOCK_LEN_ERROR bit."
    g_mcdBlockLen = (1 << mcdCsd->readBlLen);
    if (g_mcdBlockLen > 512)
    {
        g_mcdBlockLen = 512;
    }
    g_mcdNbBlock  = mcdCsd->blockNumber * ((1 << mcdCsd->readBlLen)/g_mcdBlockLen);
    g_handleCsd.Capacity = g_mcdNbBlock;
    SCI_TraceLow(  "mcd: Capacity:g_mcdNbBlock = %d",g_mcdNbBlock);

    return errStatus;
}



// =============================================================================
// mcd_Open
// -----------------------------------------------------------------------------
/// @brief Open the SD-MMC chip
/// This function does the init process of the MMC chip, including reseting
/// the chip, sending a command of init to MMC, and reading the CSD
/// configurations.
///
/// @param mcdCsd Pointer to the structure where the MMC CSD register info
/// are going to be written.
///
/// @param mcdVer is t card version.
// =============================================================================


void mcd_sdmmc_rst_volt(void);
extern  void hal_SdmmcSetClkMode(BOOLEAN auto_clk);


extern BOOLEAN g_LdoEmmcPwrOffFlag;
MCD_ERR_T mcd_Open(MCD_CARD_ID mcdId,MCD_CSD_T* mcdCsd, MCD_CARD_VER mcdVer)
{
    MCD_ERR_T                  errStatus   = MCD_ERR_NO;
    uint32                     response[4] = {0, 0, 0, 0};
    MCD_CARD_STATUS_T          cardStatus  = {0};
    BOOLEAN                       isMmc       = FALSE;
    HAL_SDMMC_DATA_BUS_WIDTH_T dataBusWidth;
    uint32 startTime = 0,cs,count;
    MCD_PROFILE_FUNCTION_ENTER(mcd_Open);
    mmc_event(0xfcd111d1);

    if (g_mcdStatus == MCD_STATUS_OPEN)
    {
        if(g_LdoEmmcPwrOffFlag)
        {
            SCI_TraceLow("mcd: Has enter LP mode, close it first,then open again");
            mcd_Close_no_sleep(0);
            g_LdoEmmcPwrOffFlag=FALSE;
        }
		else if(g_mcdCardIsError == TRUE)
		{
			SCI_TraceLow("mcd: the card has some error !");
			mcd_Close(0);
			g_mcdCardIsError=FALSE;
			mmc_event(0xfcd0bad1);
		}
        else
        {
            // Already opened, just return OK
            SCI_TraceLow( "mcd: mcd_Open: Already opened");
            *mcdCsd = g_mcdLatestCsd;

            MCD_CS_EXIT;
            MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
            mmc_event(0xcd000002);

            return MCD_ERR_NO;
        }
    }
    SCI_TraceLow(  "mcd: mcd_Open: opened enter");
    mcd_sdmmc_rst_volt();
    mmc_event(0xfd00c0d4);
    //SCI_TraceLow( "mcd_Open starts ... mcdVer %d",mcdVer);
    //SCI_TraceLow("zhhouqin trace SCI_InitRef == mode =%d",mode);
    // Check concurrency. Only one mcd_Open.
    cs = hal_SysEnterCriticalSection1();
    if (s_sdmmc_sm_ptr == PNULL)
    {

        // Create semaphore and go on with the driver.

        // NOTE: This semaphore is never deleted for now, as
        // 1. sema deletion while other task is waiting for it will cause an error;
        // 2. sema deletion will make trouble if already-open state is considered OK.
        s_sdmmc_sm_ptr   = SCI_CreateSemaphore("SDMMC OPERATION SEMAPHORE", 1);
		 SCI_ASSERT (PNULL != s_sdmmc_sm_ptr);
        SCI_TraceLow(  "mcd: mcd_Open: get the sem =0x%x",s_sdmmc_sm_ptr);
    }

    hal_SysExitCriticalSection1(cs);

    // Following operation should be kept protected
    MCD_CS_ENTER;


#if 0
    if(NULL == g_mcdConfig)
    {
        g_mcdConfig = tgt_GetMcdConfig();
    }

    if(g_mcdConfig->cardDetectGpio != HAL_GPIO_NONE)
    {
        // Only if GPIO for detection exists, else we try to open anyway.
        if(!mcd_CardDetectUpdateStatus())
        {
            SCI_TraceLow(  "mcd_Open: GPIO detection - no card");
            MCD_CS_EXIT;
            MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
            return MCD_ERR_NO_CARD;
        }
    }
#endif



    g_handleCsd.vertion = CARD_V_UNKONWN;


    if(MCD_CARD_V1 == mcdVer)
    {
        hal_SdmmcOpen(0x05);
    }
    else
    {
        hal_SdmmcOpen(1);//(0x1); // fprd electrical specification
    }


    ///@todo: should use g_mcdConfig->dat3HasPullDown if true, we can handle
    /// basic card detection as follows:
    /// call hal_SdmmcGetCardDetectPinLevel() to check if card as changed (back 1)
    /// send command ACMD42 to disable internal pull up (so pin goes to 0)
    /// meaning if pin is 1, there was a removal, file system should be told to
    /// remount the card as it might be a different one (or the content might have
    /// been changed externally).

    // RCA = 0x0000 for card identification phase.
    g_mcdRcaReg = 0;

    g_mcdSdmmcFreq = 200000;
    hal_SdmmcSetClk(g_mcdSdmmcFreq);

    // assume it's not an SDHC card
    g_mcdCardIsSdhc = FALSE;
    g_mcdOcrReg = MCD_SDMMC_OCR_VOLT_PROF_MASK;

    // Send Power On command
    hal_SdmmcSetClkMode(FALSE); // make sure the clk number  much than 74 cycle after the first time.
    mcd_hal_delay_sd1(16);
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_GO_IDLE_STATE, BROAD_ADDR, response, FALSE);
    hal_SdmmcSetClkMode(TRUE);
    if (MCD_ERR_NO != errStatus)
    {
        // some card need the clk lever is high before send this command ,so try again . becaus the first call can set the clk high.
        errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SEND_IF_COND, (MCD_CMD8_VOLTAGE_SEL | MCD_CMD8_CHECK_PATTERN), response, FALSE);
    }


    if (MCD_ERR_NO != errStatus)
    {
        g_mcdStatus = MCD_STATUS_NOTPRESENT;
        SCI_TraceLow(  "mcd: Because Power on, Initialize Failed");
        SCI_TraceLow(  "mcd: Error Status = %d", errStatus);
        hal_SdmmcClose();
        pmd_EnablePower(PMD_POWER_SDMMC, FALSE);

        MCD_CS_EXIT;
        MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
        mmc_event(0xcd000005);

        return errStatus;
    }
    else
    {
        mmc_event(0xcd000045);
        SCI_TraceLow(  "mcd: Card successfully in Idle state");
    }

    // Check if the card is a spec vers.2 one
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SEND_IF_COND, (MCD_CMD8_VOLTAGE_SEL | MCD_CMD8_CHECK_PATTERN), response, FALSE);
    if (MCD_ERR_NO != errStatus)
    {
        SCI_TraceLow(  "mcd: This card doesn't comply to the spec version 2.0");
    }
    else
    {
        // This card comply to the SD spec version 2. Is it compatible with the
        // proposed voltage, and is it an high capacity one ?
        if (response[0] != (MCD_CMD8_VOLTAGE_SEL | MCD_CMD8_CHECK_PATTERN))
        {
            // Bad pattern or unsupported voltage.
            SCI_TraceLow("mcd: Bad pattern or unsupported voltage");
            hal_SdmmcClose();
            pmd_EnablePower(PMD_POWER_SDMMC, FALSE);

            g_mcdStatus = MCD_STATUS_NOTPRESENT;
            //g_mcdVer = MCD_CARD_V1;
            MCD_CS_EXIT;
            MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
            mmc_event(0xcd000dd6);

            return MCD_ERR_UNUSABLE_CARD;
        }
        else
        {
            g_mcdOcrReg |= MCD_OCR_HCS;
        }
    }

    // TODO HCS mask bit to ACMD 41 if high capacity
    // Send OCR, as long as the card return busy
    mcd_hal_delay_sd1(300);
    startTime = timer_TimRealTickGet(OS_TIMER);
    while(1)
    {
        if((timer_TimRealTickGet(OS_TIMER) - startTime) > MCD_SDMMC_OCR_TIMEOUT )

        {
            SCI_TraceLow( "mcd: SD OCR timeout");
            hal_SdmmcClose();
            pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
            g_mcdStatus = MCD_STATUS_NOTPRESENT;

            MCD_CS_EXIT;
            MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
            mmc_event(0xcd000016);

            return MCD_ERR;
        }

        errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SEND_OP_COND, g_mcdOcrReg, response, FALSE);
        if (errStatus == MCD_ERR_CARD_NO_RESPONSE)
        {
            SCI_TraceLow( "mcd: Inserted Card is a MMC");
            // CMD 55 was not recognised: this is not an SD card !
            isMmc = TRUE;
            break;
        }

        // Bit 31 is power up busy status bit(pdf spec p. 109)
        g_mcdOcrReg = (response[0] & 0x7fffffff);

        // Power up busy bit is set to 1,
        // we can continue ...
        if (response[0] & 0x80000000)
        {
            // Version 2?
            if((g_mcdOcrReg & MCD_OCR_HCS) == MCD_OCR_HCS)
            {
                // Card is V2: check for high capacity
                if (response[0] & MCD_OCR_CCS_MASK)
                {
                    g_mcdCardIsSdhc = TRUE;
                    g_handleCsd.vertion = CARD_SD_V2_0_HIGHCAP;
                    SCI_TraceLow( "mcd: Card is SDHC ver %d", g_handleCsd.vertion);
                }
                else
                {
                    g_mcdCardIsSdhc = FALSE;
                    g_handleCsd.vertion = CARD_SD_V2_0_STANDARD;
                    SCI_TraceLow( "mcd: Card is standard capacity SD ver =%d", g_handleCsd.vertion);
                }
            }
            else
            {
                g_handleCsd.vertion = CARD_SD_V1_X;
            }

            SCI_TraceLow( "mcd: Inserted Card is a SD card ver %d", g_handleCsd.vertion);
            break;
        }
    }

    if (isMmc)
    {
        mmc_event(0xcd000007);

        while(1)
        {
            if(timer_TimRealTickGet(OS_TIMER) - startTime > MCD_SDMMC_OCR_TIMEOUT )
            {
                SCI_TraceLow( "mcd: MMC OCR timeout");
                hal_SdmmcClose();
                pmd_EnablePower(PMD_POWER_SDMMC, FALSE);

                g_mcdStatus = MCD_STATUS_NOTPRESENT;

                MCD_CS_EXIT;
                MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
                mmc_event(0xcd000f10);

                return MCD_ERR;
            }

            errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_MMC_SEND_OP_COND, g_mcdOcrReg, response, FALSE);
            if (errStatus == MCD_ERR_CARD_NO_RESPONSE)
            {
                SCI_TraceLow( "mcd: MMC OCR didn't answer");
                hal_SdmmcClose();
                pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
                g_mcdStatus = MCD_STATUS_NOTPRESENT;

                MCD_CS_EXIT;
                MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
                mmc_event(0xcd000f09);

                return MCD_ERR;
            }

            // Bit 31 is power up busy status bit(pdf spec p. 109)
            g_mcdOcrReg = (response[0] & 0x7fffffff);

            // Power up busy bit is set to 1,
            // we can continue ...
            if (response[0] & 0x80000000)
            {
                break;
            }
        }
        g_handleCsd.vertion = CARD_MMC_331;
    }

    // Get the CID of the card.
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_ALL_SEND_CID, BROAD_ADDR, response, FALSE);
    if(MCD_ERR_NO != errStatus)
    {
        SCI_TraceLow( "mcd: Because Get CID, Initialize Failed");
        hal_SdmmcClose();
        pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
        g_mcdStatus = MCD_STATUS_NOTPRESENT;

        MCD_CS_EXIT;
        MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
        mmc_event(0xcd000009);

        return errStatus;
    }
    mcd_SdmmcInitCid(response);

    // Get card RCA
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SEND_RELATIVE_ADDR, BROAD_ADDR, response, FALSE);
    if (MCD_ERR_NO != errStatus)
    {
        SCI_TraceLow( "mcd: Because Get Relative Address, Initialize Failed");
        hal_SdmmcClose(); //pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
        g_mcdStatus = MCD_STATUS_NOTPRESENT;

        MCD_CS_EXIT;
        MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
        mmc_event(0xcd00000a);

        return errStatus;
    }

    // Spec Ver 2 pdf p. 81 - rca are the 16 upper bit of this
    // R6 answer. (lower bits are stuff bits)
    g_mcdRcaReg = response[0] & 0xffff0000;

    // Get card CSD
    errStatus = mcd_ReadCsd(mcdCsd);
    if (errStatus != MCD_ERR_NO)
    {
        SCI_TraceLow( "mcd: Because Get CSD, Initialize Failed");
        hal_SdmmcClose();
        pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
        g_mcdStatus = MCD_STATUS_NOTPRESENT;

        MCD_CS_EXIT;
        MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
        mmc_event(0xcd00000b);

        return errStatus;
    }

    // If possible, set the DSR
    if (mcdCsd->dsrImp)
    {
        errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SET_DSR, g_mcdSdmmcDsr, response, FALSE);
        if (errStatus != MCD_ERR_NO)
        {
            SCI_TraceLow( "mcd: Because Set DSR, Initialize Failed");
            hal_SdmmcClose();
            pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
            g_mcdStatus = MCD_STATUS_NOTPRESENT;

            MCD_CS_EXIT;
            MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
            mmc_event(0xcd00000c);

            return errStatus;
        }
    }

    // Select the card
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SELECT_CARD, g_mcdRcaReg, response, FALSE);
    if (errStatus != MCD_ERR_NO)
    {
        SCI_TraceLow( "mcd: Because Select Card, Initialize Failed");
        SCI_TraceLow( "mcd: errStatus = %d", errStatus);
        hal_SdmmcClose();
        pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
        g_mcdStatus = MCD_STATUS_NOTPRESENT;

        MCD_CS_EXIT;
        MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
        mmc_event(0xcd00000d);

        return errStatus;
    }
    // That command returns the card status, we check we're in data mode.
    cardStatus.reg = response[0];
    if(!(cardStatus.fields.readyForData))
    {
        SCI_TraceLow( "mcd: CMD7 status=%0x", cardStatus.reg);
        hal_SdmmcClose();
        pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
        g_mcdStatus = MCD_STATUS_NOTPRESENT;
        mmc_event(0xcd00000e);

        MCD_CS_EXIT;
        MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
        return MCD_ERR;
    }

    // Set the bus width (use 4 data lines for SD cards only)
    if (isMmc)
    {
        dataBusWidth = HAL_SDMMC_DATA_BUS_WIDTH_1;
        g_handleCsd.bus_width = CARD_WIDTH_1_BIT;
    }
    else
    {

        dataBusWidth = MCD_CARD_V1 == mcdVer ? HAL_SDMMC_DATA_BUS_WIDTH_1 : HAL_SDMMC_DATA_BUS_WIDTH_4;
        g_handleCsd.bus_width = MCD_CARD_V1 == mcdVer ? CARD_WIDTH_1_BIT : CARD_WIDTH_4_BIT;



    }

    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SET_BUS_WIDTH, dataBusWidth,
                                 response, FALSE);
    if (errStatus != MCD_ERR_NO)
    {
        SCI_TraceLow( "mcd: Because Set Bus, Initialize Failed");
        hal_SdmmcClose();
        pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
        g_mcdStatus = MCD_STATUS_NOTPRESENT;
        mmc_event(0xcd00000f);

        MCD_CS_EXIT;
        MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
        return errStatus;
    }

    // That command returns the card status, in tran state ?
    cardStatus.reg = response[0];
    if (   !(cardStatus.fields.appCmd)
           || !(cardStatus.fields.readyForData)
           || (cardStatus.fields.currentState != MCD_CARD_STATE_TRAN))
    {
        SCI_TraceLow( "mcd: ACMD6 status=%0x", cardStatus.reg);
        hal_SdmmcClose();
        pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
        g_mcdStatus = MCD_STATUS_NOTPRESENT;
        mmc_event(0xcd000011);

        MCD_CS_EXIT;
        MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
        return MCD_ERR;
    }

    // Configure the controller to use that many lines.
    hal_SdmmcSetDataWidth(dataBusWidth);
#ifdef _USE_HIGH_SPEED_MODE_
    if(g_mcdCardIsSdhc == TRUE)
    {
        errStatus = mcd_SdmmcModeSwitch(0x80fffff1);
        if (errStatus != MCD_ERR_NO)
        {
            SCI_TraceLow( "mcd: Switch to High Speed Failed");
        }
        else
        {
            // extern int hal_cnt_divider;
            //hal_cnt_divider = 0;
        }
    }
#endif
    // Configure the block lenght
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SET_BLOCKLEN, g_mcdBlockLen, response, FALSE);
    if (errStatus != MCD_ERR_NO)
    {
        SCI_TraceLow( "mcd: Because Set Block Length, Initialize Failed");
        hal_SdmmcClose();
        pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
        g_mcdStatus = MCD_STATUS_NOTPRESENT;
        mmc_event(0xcd000021);

        MCD_CS_EXIT;
        MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
        return errStatus;
    }

    // That command returns the card status, in tran state ?
    cardStatus.reg = response[0];
    {
        MCD_CARD_STATUS_T expectedCardStatus;
        expectedCardStatus.reg  = 0;
        expectedCardStatus.fields.readyForData = 1;
        expectedCardStatus.fields.currentState = MCD_CARD_STATE_TRAN;

        if (cardStatus.reg != expectedCardStatus.reg)
        {
            SCI_TraceLow( "mcd:  CMD16 status=%0x", cardStatus.reg);
            hal_SdmmcClose();
            pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
            g_mcdStatus = MCD_STATUS_NOTPRESENT;
            mmc_event(0xcd000031);

            MCD_CS_EXIT;
            MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
            return MCD_ERR;
        }
    }

    hal_SdmmcEnterDataTransferMode();


    // Set the clock of the SD bus for the fastest possible rate.

    //g_mcdSdmmcFreq = MCD_CARD_V1 == mcdVer ? 6000000 : 25000000;
    g_mcdSdmmcFreq = MCD_CARD_V1 == mcdVer ? 6000000 : 33330000;
    
    // if(g_mcdCardIsSdhc == TRUE) g_mcdSdmmcFreq = 40000000; // max 50M to ver 2.0 card

    if(g_small_card == TRUE)    g_mcdSdmmcFreq = 10000000;
    hal_SdmmcSetClk(g_mcdSdmmcFreq);

    g_mcdLatestCsd = *mcdCsd;
    g_mcdStatus = MCD_STATUS_OPEN;
    hal_SdmmcSleep();
    g_mcdVer = mcdVer;
    g_handleCsd.RCA= g_mcdRcaReg;
    MCD_CS_EXIT;
    MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
    mmc_event(0xcd000999);
    SCI_TraceLow(  "mcd: mcd_Open: opened successed");

    return MCD_ERR_NO;
}


void mcd_sdmmc_Close_all_volt(void)
{

    SCI_TraceLow( "mcd: close all");
    hal_SdmmcClose();
    pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
    mcd_hal_delay_sd1(16384);
}
void mcd_sdmmc_rst_volt(void)
{
	pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
	mcd_hal_delay(800);
    pmd_EnablePower(PMD_POWER_SDMMC, TRUE);
    mcd_hal_delay_sd1(1638);
    mmc_event(0x7788aaa1);
}

// =============================================================================
// mcd_Close
// -----------------------------------------------------------------------------
/// Close MCD.
///
/// To be called at the end of the operations
/// @return MCD_ERR_NO if a response with a good crc was received,
///         MCD_ERR_CARD_NO_RESPONSE if no reponse was received within the
/// driver configured timeout.
///          MCD_ERR_CARD_RESPONSE_BAD_CRC if the received response presented
///  a bad CRC.
///         MCD_ERR_CARD_TIMEOUT if the card timedout during procedure.
// =============================================================================
MCD_ERR_T mcd_Close(MCD_CARD_ID mcdId)
{

    MCD_ERR_T errStatus;
    SCI_TraceLow( "mcd:  mcd_Close");
	SCI_SDIO_EnableDeepSleep(SCI_FALSE);
    g_small_card = FALSE;
    if (s_sdmmc_sm_ptr == PNULL)
    {
        SCI_TraceLow( "mcd: mcd_Close: Never Opened before");
		SCI_SDIO_EnableDeepSleep(SCI_TRUE);
        return MCD_ERR_NO;
    }

    MCD_CS_ENTER;

    errStatus = MCD_ERR_NO;

    MCD_PROFILE_FUNCTION_ENTER(mcd_Close);

    // Don't close the MCD driver if a transfer is in progress,
    // and be definitive about it:
    if (hal_SdmmcIsBusy() == TRUE)
    {
        SCI_TraceLow( "mcd:  Attempt to close MCD while a transfer is in progress");
    }

    hal_SdmmcWakeUp();

    // Brutal force stop current transfer, if any.
    hal_SdmmcStopTransfer(&g_mcdSdmmcTransfer);

    // Close the SDMMC module
    hal_SdmmcClose();

    g_mcdStatus = MCD_STATUS_NOTOPEN_PRESENT; // without GPIO
    mcd_CardDetectUpdateStatus(); // Test GPIO for card detect

    pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
    mcd_hal_delay(800);

    MCD_CS_EXIT;
    MCD_PROFILE_FUNCTION_EXIT(mcd_Close);
    SCI_TraceLow( "mcd_Close: done");
	SCI_SDIO_EnableDeepSleep(SCI_TRUE);
    return errStatus;
}
MCD_ERR_T mcd_Close_no_sleep(MCD_CARD_ID mcdId)
{

    MCD_ERR_T errStatus;
    SCI_TraceLow( "mcd:  mcd_Close");
	//SCI_SDIO_EnableDeepSleep(SCI_FALSE);
    g_small_card = FALSE;
    if (s_sdmmc_sm_ptr == PNULL)
    {
        SCI_TraceLow( "mcd: mcd_Close: Never Opened before");
		SCI_SDIO_EnableDeepSleep(SCI_TRUE);
        return MCD_ERR_NO;
    }

    MCD_CS_ENTER;

    errStatus = MCD_ERR_NO;

    MCD_PROFILE_FUNCTION_ENTER(mcd_Close);

    // Don't close the MCD driver if a transfer is in progress,
    // and be definitive about it:
    if (hal_SdmmcIsBusy() == TRUE)
    {
        SCI_TraceLow( "mcd:  Attempt to close MCD while a transfer is in progress");
    }

    hal_SdmmcWakeUp();

    // Brutal force stop current transfer, if any.
    hal_SdmmcStopTransfer(&g_mcdSdmmcTransfer);

    // Close the SDMMC module
    hal_SdmmcClose();

    g_mcdStatus = MCD_STATUS_NOTOPEN_PRESENT; // without GPIO
    mcd_CardDetectUpdateStatus(); // Test GPIO for card detect

    pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
    mcd_hal_delay(800);

    MCD_CS_EXIT;
    MCD_PROFILE_FUNCTION_EXIT(mcd_Close);
    SCI_TraceLow( "mcd_Close: done");
	//SCI_SDIO_EnableDeepSleep(SCI_TRUE);
    return errStatus;
}
//=============================================================================
// mcd_SdmmcTranState
//-----------------------------------------------------------------------------
/// Blocking function checking that the card is in the transfer state,
/// acknowledging thus that, for example, end of transmission.
/// @param iter Number of attempt to be made.
/// @param duration Sleeping time before the next attempt (in sys ticks).
//=============================================================================
MCD_ERR_T mcd_SdmmcTranState(uint32 iter)
{
    uint32 cardResponse[4] = {0, 0, 0, 0};
    MCD_ERR_T errStatus = MCD_ERR_NO;
    uint32 time_out;
    uint32 startTime;// = timer_TimRealTickGet(OS_TIMER);
    MCD_CARD_STATUS_T cardStatusTranState;
    // Using reg to clear all bit of the bitfields that are not
    // explicitly set.
    cardStatusTranState.reg = 0;
    cardStatusTranState.fields.readyForData = 1;
    cardStatusTranState.fields.currentState = MCD_CARD_STATE_TRAN;

    startTime = timer_TimRealTickGet(OS_TIMER);

    while(1)
    {
        errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SEND_STATUS, g_mcdRcaReg, cardResponse, FALSE);
        if (errStatus != MCD_ERR_NO)
        {
            // error while sending the command
            SCI_TraceLow( "mcd: Sd Tran Read Aft State error! err nb:%d", errStatus);
            return MCD_ERR;
        }
        else if (cardResponse[0] == cardStatusTranState.reg)
        {
            // the status is the expected one - success
            return MCD_ERR_NO;
        }
        else
        {
            // try again
            // check for timeout
            //me_out =  (MCD_CARD_V1 == g_mcdVer) ? MCD_CMD_TIMEOUT_V1:MCD_CMD_TIMEOUT_V2;
            time_out = MCD_CMD_TIMEOUT_V1;
            if (timer_TimRealTickGet(OS_TIMER) - startTime >  time_out )
            {
                SCI_TraceLow( "mcd: Sd Tran don't finished");
                SCI_TraceLow( "mcd: current state =%0x", cardResponse[0]);
                return MCD_ERR;
            }
        }
#ifdef _USE_HIGH_SPEED_MODE_
        mcd_delay(2);
#endif
    }
}

MCD_ERR_T mcd_SdmmcMultBlockErase(uint32 blockAddr,uint32 blockSize)
{
    uint32 cardResponse[4] = {0, 0, 0, 0};
    uint32 cmd_time_out;
    MCD_ERR_T errStatus = MCD_ERR_NO;
    //cmd_time_out =  (MCD_CARD_V1 == g_mcdVer) ? MCD_CMD_TIMEOUT_V1 : MCD_CMD_TIMEOUT_V2;
    cmd_time_out=MCD_CMD_TIMEOUT_V1;
    if (MCD_ERR_NO != mcd_SdmmcTranState(cmd_time_out))
    {
        SCI_TraceLow( "mcd: Erase on Sdmmc while not in Tran state");
        return MCD_ERR_CARD_TIMEOUT;
    }
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_ERASE_BLK_START, blockAddr, cardResponse, FALSE);
     if (errStatus != MCD_ERR_NO)
    {
            // error while sending the command
           SCI_TraceLow( "mcd: Sd Tran Read Aft State error! err nb:%d", errStatus);
           return errStatus;
    }
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_ERASE_BLK_END, blockAddr+blockSize, cardResponse, FALSE);
     if (errStatus != MCD_ERR_NO)
    {
            // error while sending the command
           SCI_TraceLow( "mcd: Sd Tran Read Aft State error! err nb:%d", errStatus);
           return errStatus;
    }
    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_MULTI_BLK_ERASE, 0, cardResponse, FALSE);
     if (errStatus != MCD_ERR_NO)
    {
            // error while sending the command
           SCI_TraceLow( "mcd: Sd Tran Read Aft State error! err nb:%d", errStatus);
           return errStatus;
    }
    if (MCD_ERR_NO != mcd_SdmmcTranState(MCD_ERASE_TIMEOUT))
    {
        SCI_TraceLow( "mcd: Erase timeout");
        return MCD_ERR_CARD_TIMEOUT;
    }

    return MCD_ERR_NO;
}


// send to high mode
static MCD_ERR_T  mcd_SdmmcModeSwitch(uint32 arg)
{
    uint32 startTime,write_time_out;
    uint8 cardResponse[64] = {0};
    MCD_ERR_T errStatus = MCD_ERR_NO;

    //write_time_out =  (MCD_CARD_V1 == g_mcdVer) ? MCD_WRITE_TIMEOUT_V1:MCD_WRITE_TIMEOUT_V2;
    write_time_out = MCD_WRITE_TIMEOUT_V1;
    if (MCD_ERR_NO != mcd_SdmmcTranState(write_time_out))
    {
        SCI_TraceLow( "mcd: Write on Sdmmc while not in Tran state");
        return MCD_ERR_CARD_TIMEOUT;
    }

    errStatus = mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SWITCH,arg, (uint32 *)cardResponse, FALSE);
     if (errStatus != MCD_ERR_NO)
    {
            // error while sending the command
           SCI_TraceLow( "mcd: Sd Tran Read Aft State error! err nb:%d", errStatus);
           return errStatus;
    }
    g_mcdSdmmcTransfer.sysMemAddr = cardResponse;
    g_mcdSdmmcTransfer.blockNum   = 1;
    g_mcdSdmmcTransfer.blockSize  = 64;
    g_mcdSdmmcTransfer.direction  = HAL_SDMMC_DIRECTION_READ;

    hal_SdmmcTransfer(&g_mcdSdmmcTransfer);

    startTime = timer_TimRealTickGet(OS_TIMER);
    while(!hal_SdmmcTransferDone())
    {
        if (timer_TimRealTickGet(OS_TIMER) - startTime >  (write_time_out))
        {
            SCI_TraceLow( "mcd: Switch on Sdmmc timeout");
            hal_SdmmcStopTransfer(&g_mcdSdmmcTransfer);
        }
    #if !defined(OS_NONE)
            SCI_Sleep(1);
    #endif
    }

    return MCD_ERR_NO;
}



//=============================================================================
// mcd_SdmmcMultBlockWrite
//-----------------------------------------------------------------------------
/// Write one or a bunch of data blocks.
/// @param blockAddr Address on the card where to write data.
/// @param pWrite Pointer towards the buffer of data to write.
/// @param blockNum Number of blocks to write.
//=============================================================================
static MCD_ERR_T mcd_SdmmcMultBlockWrite(uint8* blockAddr, CONST uint8* pWrite, uint32 blockNum)
{
    uint32 cardResponse[4] = {0, 0, 0, 0};
    MCD_ERR_T errStatus = MCD_ERR_NO;
    HAL_SDMMC_CMD_T writeCmd;
    uint32 tran_time_out;
    uint32 write_time_out;
    MCD_CARD_STATUS_T cardStatusTranState;
    MCD_CARD_STATUS_T cardStatusResponse = {0,};
    MCD_CARD_STATUS_T cardStatusPreErasedState;
    uint32 startTime = 0;
	hal_SysEnterCriticalSection1();

    // Using reg to clear all bit of the bitfields that are not
    // explicitly set.
    cardStatusTranState.reg = 0;
    cardStatusTranState.fields.readyForData = 1;
    cardStatusTranState.fields.currentState = MCD_CARD_STATE_TRAN;

    cardStatusPreErasedState.reg    = 0;
    cardStatusPreErasedState.fields.appCmd       = 1;
    cardStatusPreErasedState.fields.readyForData = 1;
    cardStatusPreErasedState.fields.currentState = MCD_CARD_STATE_TRAN;

    g_mcdSdmmcTransfer.sysMemAddr = (uint8*) pWrite;
    g_mcdSdmmcTransfer.sdCardAddr = blockAddr;
    g_mcdSdmmcTransfer.blockNum   = blockNum;
    g_mcdSdmmcTransfer.blockSize  = g_mcdBlockLen;
    g_mcdSdmmcTransfer.direction  = HAL_SDMMC_DIRECTION_WRITE;


// FIXME find what the heck is that !:
// program_right_num=0;
    // mmc_event(0xeddd007);

    // Check buffer.
    //MCD_ASSERT(pWrite != NULL, "SDMMC write: Buffer is NULL");
    MCD_ASSERT(pWrite != NULL);//, "SDMMC write: Buffer is NULL");
    // mmc_event(0xeddd008);

#if 0
    MCD_ASSERT(((uint32)pWrite & 0x3) ==0,
               "SDMMC write: buffer is not aligned! addr=%08x", pWrite);
    MCD_ASSERT(blockNum>=1 && blockNum<= MCD_MAX_BLOCK_NUM,
               "Block Num is overflow");
#endif

    MCD_ASSERT(((uint32)pWrite & 0x3) ==0);
    MCD_ASSERT(blockNum>=1 && blockNum<= MCD_MAX_BLOCK_NUM);
    // Check that the card is in tran (Transmission) state.
    tran_time_out =  (MCD_CARD_V1 == g_mcdVer) ? MCD_TRAN_TIMEOUT_V1:MCD_TRAN_TIMEOUT_V2;
    if (MCD_ERR_NO != mcd_SdmmcTranState(tran_time_out))
        // 5200000, 0, initially, that is to say rougly 0.1 sec ?
    {
        mmc_event(0xeddd009);
		hal_SysExitCriticalSection1(0);

        SCI_TraceLow( "mcd: Write on Sdmmc while not in Tran state");
		
        return MCD_ERR_CARD_TIMEOUT;
    }

    // The command for single block or multiple blocks are differents
    if (blockNum == 1)
    {
        writeCmd = HAL_SDMMC_CMD_WRITE_SINGLE_BLOCK;
    }
    else
    {
        writeCmd = HAL_SDMMC_CMD_WRITE_MULT_BLOCK;
    }
    //mmc_event(0xeddd00a);

    // PreErasing, to accelerate the multi-block write operations
    if (blockNum >1)
    {
        if(MCD_ERR == mcd_SdmmcSendCmd(HAL_SDMMC_CMD_SET_WR_BLK_COUNT, blockNum, cardResponse, FALSE))
        {
        hal_SysExitCriticalSection1(0);
            SCI_TraceLow( "mcd: Set Pre-erase Failed");
            return MCD_ERR;
        }

        // Advance compatibility,to support 1.0 t-flash.
        if (cardResponse[0] != cardStatusPreErasedState.reg)
        {
            SCI_TraceLow( "mcd: warning ACMD23 status=%0x", cardResponse[0]);
            // cardResponse[0] = cardStatusPreErasedState.reg;
            // return MCD_ERR;
        }
    }

    //mmc_event(0xeddd00b);

    // Initiate data migration through Ifc.
    if (hal_SdmmcTransfer(&g_mcdSdmmcTransfer) != HAL_ERR_NO)
    {
        SCI_TraceLow( "mcd: write sd no ifc channel");
		hal_SysExitCriticalSection1(0);
        return MCD_ERR_DMA_BUSY;
    }

    // Initiate data migration of multiple blocks through SD bus.
    errStatus = mcd_SdmmcSendCmd(writeCmd,
                                 (uint32)blockAddr,
                                 cardResponse,
                                 FALSE);
    if (errStatus != MCD_ERR_NO)
    {
        SCI_TraceLow( "mcd: Set sd write had error");
        hal_SdmmcStopTransfer(&g_mcdSdmmcTransfer);
		hal_SysExitCriticalSection1(0);
        return MCD_ERR_CMD;
    }
    // mmc_event(0xeddd00c);

    cardStatusResponse.reg = cardResponse[0] ;
    // Check for error, by listing all valid states
    // TODO: FIXME, more states could be legal here
    // The sixteen uper bits are error bits: they all must be null
    // (Physical Spec V.2, p71)
    if ((cardResponse[0] != cardStatusTranState.reg)
        && !((cardStatusResponse.fields.readyForData == 1)
             && (cardStatusResponse.fields.currentState == MCD_CARD_STATE_RCV)
             && ((cardStatusResponse.reg&0xFFFF0000) == 0))
        && !(cardStatusResponse.fields.currentState == MCD_CARD_STATE_RCV
             && ((cardStatusResponse.reg&0xFFFF0000) == 0))
        && !(cardStatusResponse.fields.currentState == MCD_CARD_STATE_PRG
             && ((cardStatusResponse.reg&0xFFFF0000) == 0))
       )
    {
        SCI_TraceLow( "mcd: Write block,Card Reponse: %x", cardResponse[0]);
        hal_SdmmcStopTransfer(&g_mcdSdmmcTransfer);
		hal_SysExitCriticalSection1(0);
        return MCD_ERR;
    }

    //mmc_event(0xeddd00d);
	hal_SysExitCriticalSection1(0);

    // Wait
    startTime = timer_TimRealTickGet(OS_TIMER);
    write_time_out = MCD_WRITE_TIMEOUT_V1;// (MCD_CARD_V1 == g_mcdVer) ? MCD_WRITE_TIMEOUT_V1:MCD_WRITE_TIMEOUT_V2;
    while(!hal_SdmmcTransferDone())
    {
        if (timer_TimRealTickGet(OS_TIMER) - startTime >  (write_time_out*blockNum))
        {
            if(!hal_SdmmcTransferDone())
            {
                SCI_TraceLow( "mcd: Write on Sdmmc timeout");
                // Abort the transfert.
                hal_SdmmcStopTransfer(&g_mcdSdmmcTransfer);
                mmc_event(0xeddd0ff);
                return MCD_ERR_CARD_TIMEOUT;
            }
        }
    #if !defined(OS_NONE)
            SCI_Sleep(1);
    #endif
    }

   hal_SysEnterCriticalSection1();

    // Nota: CMD12 (stop transfer) is automatically
    // sent by the SDMMC controller.

    if (mcd_SdmmcWriteCheckCrc() != MCD_ERR_NO)
    {
        SCI_TraceLow( "mcd: SDMMC Write error");
		
		hal_SysExitCriticalSection1(0);
        return MCD_ERR;
    }

    // Check that the card is in tran (Transmission) state.
    if (MCD_ERR_NO != mcd_SdmmcTranState(tran_time_out))
        // 5200000, 0, initially, that is to say rougly 0.1 sec ?
    {
        SCI_TraceLow( "mcd: Write on Sdmmc while not in Tran state");
		
		hal_SysExitCriticalSection1(0);
        return MCD_ERR_CARD_TIMEOUT;
    }
    //
    
	hal_SysExitCriticalSection1(0);
    

    return MCD_ERR_NO;

}



//=============================================================================
// mcd_SdmmcMultBlockRead
//-----------------------------------------------------------------------------
/// Read one or a bunch of data blocks.
/// @param blockAddr Address on the card where to read data.
/// @param pRead Pointer towards the buffer of data to read.
/// @param blockNum Number of blocks to read.
//=============================================================================
static MCD_ERR_T mcd_SdmmcMultBlockRead(uint8* blockAddr, uint8* pRead, uint32 blockNum)
{
    uint32                  cardResponse[4]     = {0, 0, 0, 0};
    MCD_ERR_T               errStatus           = MCD_ERR_NO;
    HAL_SDMMC_CMD_T         readCmd;
    MCD_CARD_STATUS_T cardStatusTranState;
    // Using reg to clear all bit of the bitfields that are not
    // explicitly set.
    uint32 startTime=0;
    uint32 tran_time_out;
    uint32 read_time_out;
    cardStatusTranState.reg = 0;
    cardStatusTranState.fields.readyForData = 1;
    cardStatusTranState.fields.currentState = MCD_CARD_STATE_TRAN;
	//hal_SysEnterCriticalSection1();


    g_mcdSdmmcTransfer.sysMemAddr = (uint8*) pRead;
    g_mcdSdmmcTransfer.sdCardAddr = blockAddr;
    g_mcdSdmmcTransfer.blockNum   = blockNum;
    g_mcdSdmmcTransfer.blockSize  = g_mcdBlockLen;
    g_mcdSdmmcTransfer.direction  = HAL_SDMMC_DIRECTION_READ;

    // Check buffer.
    MCD_ASSERT(pRead != NULL);//, "SDMMC write: Buffer is NULL");
    MCD_ASSERT(((uint32)pRead & 0x3) ==0);//, "SDMMC write: buffer is not aligned");
    MCD_ASSERT(blockNum>=1 && blockNum<= MCD_MAX_BLOCK_NUM);//,         "Block Num is overflow");

    // Command are different for reading one or several blocks of data
    if (blockNum == 1)
    {
        readCmd = HAL_SDMMC_CMD_READ_SINGLE_BLOCK;
    }
    else
    {
        readCmd = HAL_SDMMC_CMD_READ_MULT_BLOCK;
    }
    // Initiate data migration through Ifc.
    if (hal_SdmmcTransfer(&g_mcdSdmmcTransfer) != HAL_ERR_NO)
    {
        SCI_TraceLow( "mcd: write sd no ifc channel");
		//hal_SysExitCriticalSection1(0);
        return MCD_ERR_DMA_BUSY;
    }
    // Initiate data migration of multiple blocks through SD bus.
    errStatus = mcd_SdmmcSendCmd(readCmd,
                                 (uint32)blockAddr,
                                 cardResponse,
                                 FALSE);
    if (errStatus != MCD_ERR_NO)
    {
        SCI_TraceLow( "mcd: Set sd write had error");
        hal_SdmmcStopTransfer(&g_mcdSdmmcTransfer);
		//hal_SysExitCriticalSection1(0);
        return MCD_ERR_CMD;
    }
    if (cardResponse[0] != cardStatusTranState.reg)
    {
        SCI_TraceLow( "mcd: CMD%d status=0x%x",readCmd,cardResponse[0] );
        // hal_SdmmcStopTransfer(&g_mcdSdmmcTransfer);
        // return MCD_ERR;
    }
	//hal_SysExitCriticalSection1(0);
    // Wait
    startTime = timer_TimRealTickGet(OS_TIMER);
    read_time_out = MCD_READ_TIMEOUT_V1;// (MCD_CARD_V1 == g_mcdVer) ? MCD_READ_TIMEOUT_V1:MCD_READ_TIMEOUT_V2;
    while(!hal_SdmmcTransferDone())
    {
        if (timer_TimRealTickGet(OS_TIMER) - startTime > (read_time_out))
        {
            if(!hal_SdmmcTransferDone())
            {
                SCI_TraceLow( "mcd: Read on Sdmmc timeout");
                // Abort the transfert.
                hal_SdmmcStopTransfer(&g_mcdSdmmcTransfer);
                return MCD_ERR_CARD_TIMEOUT;
            }
        }
    #if !defined(OS_NONE)
            SCI_Sleep(1);
    #endif
    }
    // Nota: CMD12 (stop transfer) is automatically
    // sent by the SDMMC controller.
	//hal_SysEnterCriticalSection1();

    if (mcd_SdmmcReadCheckCrc() != MCD_ERR_NO)
    {
        SCI_TraceLow( "mcd: sdc read state error");
		//hal_SysExitCriticalSection1(0);
        return MCD_ERR;
    }
    tran_time_out = (MCD_CARD_V1 == g_mcdVer) ? MCD_TRAN_TIMEOUT_V1:MCD_TRAN_TIMEOUT_V2;
    // Check that the card is in tran (Transmission) state.
    if (MCD_ERR_NO != mcd_SdmmcTranState(tran_time_out))
        // 5200000, 0, initially, that is to say rougly 0.1 sec ?
    {
        SCI_TraceLow( "mcd: Read on Sdmmc while not in Tran state");
		//hal_SysExitCriticalSection1(0);
        return MCD_ERR_CARD_TIMEOUT;
    }
    // Flush Cache
    //  hal_SysInvalidateCache(pRead, blockNum * g_mcdBlockLen);
    //MMU_DmaCacheSync((uint32)pRead, (uint32)(blockNum * g_mcdBlockLen),DMABUFFER_BIDIRECTIONAL);

    //hal_SysExitCriticalSection1(0);
    return MCD_ERR_NO;
}



// =============================================================================
// mcd_Write
// -----------------------------------------------------------------------------
/// @brief Write a block of data to MMC.
///
/// This function is used to write blocks of data on the MMC.
/// @param startAddr Start Adress  of the SD memory block where the
/// data will be written
/// @param blockWr Pointer to the block of data to write. Must be aligned
/// on a 32 bits boundary.
/// @param size Number of bytes to write. Must be an interger multiple of the
/// sector size of the card
// =============================================================================

MCD_ERR_T mcd_Write(uint32 startAddr,  CONST uint8* blockWr, uint32 size)
{

    uint8*      tmpAdd  = NULL;
    MCD_ERR_T   value   = MCD_ERR_NO;
    uint32 i = 0,wait_time,speed,cs;
    MCD_CS_ENTER;
	SCI_SDIO_EnableDeepSleep(SCI_FALSE);

    MCD_PROFILE_FUNCTION_ENTER(mcd_Write);
    SCI_TraceLow(  "mcd: mcd_Write: startAddr=%d, blockWr=0x%x , size=%d",startAddr,blockWr,size);
    MCD_ASSERT(g_mcdBlockLen != 0);//, "mcd_Write called before a successful mcd_Open");
    if (g_mcdCardIsSdhc == FALSE)
    {
// MCD_ASSERT(startAddr % g_mcdBlockLen == 0, "write card address is not aligned");
    }
    SCI_ASSERT((size % g_mcdBlockLen) == 0);//, "mcd_Write size (%d) is not a multiple of"         "the block size (%d)",          size, g_mcdBlockLen);
    if(!mcd_CardDetectUpdateStatus())
    {
        MCD_PROFILE_FUNCTION_EXIT(mcd_Write);
        MCD_CS_EXIT;
		SCI_SDIO_EnableDeepSleep(SCI_TRUE);
		g_mcdCardIsError = TRUE;
        return MCD_ERR_NO_CARD;
    }
	SCI_GetSemaphore(s_sdmmc_sm_ptr,SCI_WAIT_FOREVER);
    wait_time = timer_TimRealTickGet(OS_TIMER);

    cs = hal_SysEnterCriticalSection1();
    if(g_mcdCardIsBusy == FALSE)
    {
        g_mcdCardIsBusy  = TRUE;
    }
    else
    {
        SCI_TraceLow(  "mcd: mcd_Write re Enter. ");
        SCI_PASSERT(FALSE,("Some Task attempt to write Tcard when the card is reading or writing by other task! "));

    }

    hal_SysExitCriticalSection1(cs);


    hal_SdmmcWakeUp();
    // Addresses are block number for high capacity card
    if (g_mcdCardIsSdhc)
    {
        tmpAdd = (uint8*) (startAddr);

    }
    else
    {
        tmpAdd = (uint8*) (startAddr*g_mcdBlockLen);
    }
    if(MCD_CARD_V1 == g_mcdVer)
    {

        for(i = 0; i < size/g_mcdBlockLen; i++)
        {
            value = mcd_SdmmcMultBlockWrite(tmpAdd + i*g_mcdBlockLen, blockWr + i*g_mcdBlockLen, 1);
            if(value != MCD_ERR_NO)
            {
                break;
            }
        }
    }
    else
    {
        value = mcd_SdmmcMultBlockWrite(tmpAdd, blockWr, size/g_mcdBlockLen);
    }

    hal_SdmmcSleep();
    MCD_PROFILE_FUNCTION_EXIT(mcd_Write);
    MCD_CS_EXIT;

    speed = ((timer_TimRealTickGet(OS_TIMER)-wait_time)*1000)/16384;
    SCI_TraceLow(  "mcd: mcd_Write: edn%d, %d",value,speed);
	SCI_SDIO_EnableDeepSleep(SCI_TRUE);
	if(value != MCD_ERR_NO)
		g_mcdCardIsError = TRUE;

	
    cs = hal_SysEnterCriticalSection1();
    g_mcdCardIsBusy = FALSE;
    hal_SysExitCriticalSection1(cs);

	
	SCI_PutSemaphore(s_sdmmc_sm_ptr);
    return value;
}



// =============================================================================
// mcd_Read
// -----------------------------------------------------------------------------
/// @brief Read using pattern mode.
/// @ param startAddr: of the SD memory block where the data
/// will be read
/// @param blockRd Pointer to the buffer where the data will be stored. Must be aligned
/// on a 32 bits boundary.
/// @param size Number of bytes to read. Must be an interger multiple of the
/// sector size of the card.
// =============================================================================
MCD_ERR_T mcd_Read(uint32 startAddr, uint8* blockRd, uint32 size)
{
    uint8*      tmpAdd  = NULL;
    MCD_ERR_T   value   = MCD_ERR_NO;
    uint32 cs,wait_time,speed;
    MCD_CS_ENTER;
	SCI_SDIO_EnableDeepSleep(SCI_FALSE);
    MCD_PROFILE_FUNCTION_ENTER(mcd_Read);
    
    MCD_ASSERT(g_mcdBlockLen != 0);//, "mcd_Read called before a successful mcd_Open_V1 or mcd_Open_V2");
    SCI_TraceLow(  "mcd: mcd_Read block: startAddr=%d, blockRd=0x%x, len =%d",startAddr,blockRd,size);

    if (g_mcdCardIsSdhc == FALSE)
    {
        // SCI_PASSERT(startAddr % g_mcdBlockLen == 0,"read card address is not aligned");
    }

    SCI_ASSERT((size % g_mcdBlockLen) == 0);;//, "mcd_Read size (%d) is not a multiple of"     "the block size (%d)",               size, g_mcdBlockLen);

    if(!mcd_CardDetectUpdateStatus())
    {
        MCD_PROFILE_FUNCTION_EXIT(mcd_Read);
        MCD_CS_EXIT;
		SCI_SDIO_EnableDeepSleep(SCI_TRUE);
		g_mcdCardIsError = TRUE;
        return MCD_ERR_NO_CARD;
    }

	SCI_GetSemaphore(s_sdmmc_sm_ptr,SCI_WAIT_FOREVER);

	
    cs = hal_SysEnterCriticalSection1();
    if(g_mcdCardIsBusy == FALSE)
    {
        g_mcdCardIsBusy  = TRUE;
    }
    else
    {
        SCI_TraceLow(  "mcd: mcd_Read re Enter. ");
        SCI_PASSERT(FALSE,("mcd: Some Task attempt to read Tcard when the card is reading by other task "));

    }

    hal_SysExitCriticalSection1(cs);
	wait_time = timer_TimRealTickGet(OS_TIMER);
    hal_SdmmcWakeUp();
    // Addresses are block number for high capacity card
    if (g_mcdCardIsSdhc)
    {
        tmpAdd = (uint8*) (startAddr);
    }
    else
    {
        tmpAdd = (uint8*) (startAddr*g_mcdBlockLen);
    }
    value = mcd_SdmmcMultBlockRead(tmpAdd, blockRd, size/g_mcdBlockLen);
    hal_SdmmcSleep();
    MCD_PROFILE_FUNCTION_EXIT(mcd_Read);
    MCD_CS_EXIT;
    speed = ((timer_TimRealTickGet(OS_TIMER)-wait_time)*1000)/16384;
    SCI_TraceLow(  "mcd: mcd_Read: end =%d ,%d",value,speed);
	
    cs = hal_SysEnterCriticalSection1();
    g_mcdCardIsBusy = FALSE;
    hal_SysExitCriticalSection1(cs);
	
	SCI_SDIO_EnableDeepSleep(SCI_TRUE);
    if(value != MCD_ERR_NO)
		g_mcdCardIsError = TRUE;

	SCI_PutSemaphore(s_sdmmc_sm_ptr);
    return value;

}


BOOLEAN mcd_IsHighCapacityCard(void)
{
    if(g_mcdCardIsSdhc == TRUE)
    {
        return TRUE;
    }
    return FALSE;

}

#if 0
void mcd_get_MaxClk()
{


    // Set speed to max allowable speed
    // Calculate Divider for 78MHz
    // Rate = SysClk / ((clkDivider+1)*2)
    // clkDivider = (SysClk/Rate)/2-1
    {
        uint32 rate_unit = (mcdCsd->tranSpeed & 0x7);
        uint32 rate_val = (mcdCsd->tranSpeed >>3) & 0xf;
        uint32 rate;
        uint8 clkdivider=1;
        rate_val = hal_CalculateVal(rate_val);
        if ( rate_val == 0 )
        {
            MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
            hal_SpiDeActivateCs(HAL_SPI, g_mcdSpiCs);
            // Indicate that SPI is free
            // g_mcdSpiFree = TRUE;
            hal_SpiClose(HAL_SPI, g_mcdSpiCs);
            return MCD_ERR_CSD;
        }
        switch (rate_unit)
        {
            case 0: // Unit 100kHz
                rate_unit = 10;
                break;
            case 1: // Unit 1MHz
                rate_unit = 100;
                break;
            case 2: // Unit 10MHz
                rate_unit = 1000;
                break;
            case 3: // Unit 100MHz
                rate_unit = 10000;
                break;
            default: // Ohters are reserved
                MCD_PROFILE_FUNCTION_EXIT(mcd_Open);
                hal_SpiDeActivateCs(HAL_SPI, g_mcdSpiCs);
                // Indicate that SPI is free
                // g_mcdSpiFree = TRUE;
                return MCD_ERR_CSD;
        }
        rate = rate_val*rate_unit;
        SCI_TraceLow(0x10001913, "MMC/SD Speed Rating: %d kHz", rate);
        clkdivider = (78000/rate)/2;
        // MCD_TRACE(MCD_INFO_TRC, 0, " Clk divider: %d", clkdivider);
        // SD max frequency: 1.1Mhz: 1100000
        g_spiConfig.spiFreq = 1100000;
        // MMC and Tflash work at minimal frequency of 1.5Mhz-TFlash=>4Mhz
        if (IsMMC == TRUE)
        {
            g_spiConfig.spiFreq = 2000000;
        }
        //MCD_TRACE(MCD_INFO_TRC, 0, "MMC/SD Used Speed @52MHz: %d kHz", 52000/(g_spiConfig.spiFreq+1)/2);
        //MCD_TRACE(MCD_INFO_TRC, 0,"MMC/SD Used Speed @78MHz: %d kHz",78000/(g_spiConfig.spiFreq+1)/2);
        SCI_TraceLow(0x10001914, "MMC/SD Sector size : %d bytes",1<<mcdCsd->readBlLen);
    }
}
#endif

BOOLEAN Sdio_open(void)
{
    MCD_ERR_T ret;
	
	SCI_SDIO_EnableDeepSleep(SCI_FALSE);
    ret = mcd_Open(0,&g_handleCsd.csd,MCD_CARD_V2);
    if(ret != MCD_ERR_NO)
    {
    
		SCI_SDIO_EnableDeepSleep(SCI_TRUE);
        return FALSE;
    }
	SCI_SDIO_EnableDeepSleep(SCI_TRUE);

    return TRUE;


}

MCD_HANDLE_CDS_T * mcd_get_mcdCsd(void)
{
    SCI_TraceLow( "mcd_get_mcdCsd ver %d ",g_handleCsd.vertion);
    SCI_TraceLow( "mcd_get_mcdCsd cc %d ",g_handleCsd.Capacity);
    return &g_handleCsd;
}

#ifdef _TEST_SDMMC_
MCD_ERR_T  test_open(void)
{
    MCD_CSD_T mcdCsd;
    return mcd_Open(0,&mcdCsd,MCD_CARD_V2);

}
void test_mcd_Read(uint32 startAddr, uint8* rdDataBlock, uint32 blockLength)
{
    mcd_Read(startAddr,rdDataBlock,blockLength);
}

void test_mcd_Write(uint32 blockStartAddr, CONST uint8* blockWr, uint32 blockLength)
{

    mcd_Write( blockStartAddr,  blockWr, blockLength);
}

void test_close()
{

    mcd_Close(0);

}

void test_reopenspi()
{


}
void test_mcd_Write_4block(uint32 blockStartAddr, CONST uint8* blockWr, uint32 blockLength)
{
    //mcd_Write_mutil(blockStartAddr,  blockWr, blockLength);
}

///#define HAL_UNCACHED_DATA __attribute__((section(".ucdata")))

//uint8 HAL_UNCACHED_DATA  test_m_w[1024];
//uint8  HAL_UNCACHED_DATA test_m_r[1024];
uint8   test_m_w[1024];
uint8   test_m_r[1024];



void test_mcd_card(void)
{
    uint32 k,m;
    uint32 add=0x1000;
    uint32 wait_time = 0;
    uint32 wait_time2 = 0;
    mmc_event(0xcccccccc);
    for(k=0; k<1024; k++)
    {
        test_m_w[k]=k;
    }
    if(MCD_ERR_NO ==  test_open())
    {
        mmc_event(0xccccccfc);

    }
    else
    {
        mmc_event(0xcccaaaac);
        return;

    }
testmcd :

    for(k=0; k<1024; k++)
    {
        test_m_w[k]=0xa5a5a5a5;
    }
    wait_time = timer_TimRealTickGet(OS_TIMER);

    test_mcd_Write(add,test_m_w,1024);
    wait_time2 = timer_TimRealTickGet(OS_TIMER);
    mmc_event(0xffeeeeee);
    mmc_event(1000/((wait_time2-wait_time)*1000/16384));
    wait_time = timer_TimRealTickGet(OS_TIMER);

    test_mcd_Read(add,test_m_r,1024);
    wait_time2 = timer_TimRealTickGet(OS_TIMER);

    mmc_event(0xffdddddd);
    mmc_event(1000/((wait_time2-wait_time)*1000/16384));


    for(k=0; k<1024; k++)
    {
        if(test_m_w[k]!=test_m_r[k])
        {
            mmc_event(0xffffcdcd);
            mmc_event(k);
            mmc_event(test_m_w[k]);
            mmc_event(test_m_r[k]);
            while(1) {};
            break;
        }
    }


    for(k=0; k<1024; k++)
    {
        test_m_w[k]=0x5a5a5a5a;
    }

    if(add >0xace000) add = 0x1000;
    test_mcd_Write(add,test_m_w,1024);
    test_mcd_Read(add,test_m_r,1024);
    for(k=0; k<1024; k++)
    {
        if(test_m_w[k]!=test_m_r[k])
        {
            mmc_event(0xfff0cdcd);
            mmc_event(k);
            mmc_event(test_m_w[k]);
            mmc_event(test_m_r[k]);
            while(1) {};
            break;
        }
    }
    mmc_event(0xffff40ed);
    mmc_event(add);

    SCI_TraceLow( "test card addr     = %d  \n",add   );

    mcd_hal_delay_sd1(100);
    add+=1024;
    goto testmcd;
    //mcd_Close(0);
    //while(1) {};

}

#endif



