
#ifdef WIN32
#include <stdio.h>
#include "sci_types.h"
#include "mn_type.h"

typedef int int32_t;
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned long int   uint32_t;

typedef struct _CFW_EQOS
{
    uint8_t nQci;
    uint32_t nDlGbr;
    uint32_t nUlGbr;
    uint32_t nDlMbr;
    uint32_t nUlMbr;
} CFW_EQOS;

typedef enum
{
    CFW_SIM_0 = 0x00,
    CFW_SIM_1 = 0x01,
    CFW_SIM_END = 0xFF,
    CFW_SIM_ENUM_FILL = 0x7FFFFFFF
} CFW_SIM_ID;

typedef enum
{
    L1_RW_RF_REG_OK,
    L1_RW_RF_REG_FAILED,
    L1_RW_RF_REG_PENDING
}L1_RW_RF_REG_RESULT_E;

typedef enum
{
	MAIN_LCD_ID = 0,				//Main lcd id
	SUB_LCD_ID,						//Sub  lcd id
	MAX_LCD_ID						//Reserved, can not be used by user.
} LCD_ID_E; 

typedef enum
{
	ERR_NONE=0,
		ERR_NOT_ENOUGH_RESOURCE,
		ERR_INVALID_PARAM,
		ERR_NV_ACCESS,
		ERR_NOT_ALLOWED
}ERR_DEVOPER_E;

typedef enum
{
	ERR_LCD_NONE = SCI_SUCCESS,		// Success,no error
	ERR_LCD_FUNC_NOT_SUPPORT,		// LCD not support this function
	ERR_LCD_PARAMETER_WRONG,		// Parameter is wrong 
	ERR_LCD_POINTER_NULL,			// Input pointer is PNULL
	ERR_LCD_OPERATE_FAIL,			// Operate fail 
	ERR_LCD_NOT_FOUND				// LCD not found 
} ERR_LCD_E;

typedef enum
{
	AUDIO_DEVICE_MODE_HANDHOLD,
	AUDIO_DEVICE_MODE_HANDFREE,
	AUDIO_DEVICE_MODE_EARPHONE,
	AUDIO_DEVICE_MODE_EARFREE,
	AUDIO_DEVICE_MODE_TVOUT,
	AUDIO_DEVICE_MODE_BLUEPHONE,
	AUDIO_DEVICE_MODE_MAX
}AUDIO_DEVICE_MODE_TYPE_E;

typedef enum
{
    AUD_INPUT_TYPE_MAINMIC,
    AUD_INPUT_TYPE_AUXMIC,
    AUD_INPUT_TYPE_DUALMIC,
    AUD_INPUT_TYPE_HPMIC_USE_L,
    AUD_INPUT_TYPE_HPMIC_USE_R,
} AUD_INPUT_TYPE_T;

typedef struct _BT_HFG_WB_FLAG 
{
    BOOLEAN is_wb_call_flag;//TRUE means WB Call;  FALSE means NB CALL
    BOOLEAN is_nrec_flag;// TRUE: The Bluetooth headset support NREC;  FALSE: The Bluetooth headset cannot support NREC;
} BT_HFG_WB_FLAG;

typedef struct //this must be the same with api_NwStoreListInd_t
{
    uint8 Plmn[3];
    uint8 nCount;
    uint16 ArfcnL[33];
}MN_StoredPlmnList;

typedef enum _NVITEM_ERROR {
    NVERR_NONE  = 0,   			/* Success */    
    NVERR_SYSTEM,      			/* System error, e.g. hardware failure */
    NVERR_INVALID_PARAM,
    NVERR_NO_ENOUGH_RESOURCE,
    NVERR_NOT_EXIST,
    NVERR_ACCESS_DENY,
    NVERR_INCOMPATIBLE,
    NVERR_NOT_OPENED
}NVITEM_ERROR_E;

typedef enum
{
    /// No error occured
    AUD_ERR_NO,

    /// An attempt to access a busy resource failed
    /// (Resource use not possible at the time)
    AUD_ERR_RESOURCE_BUSY,

    /// Attempt to open a resource already opened,
    /// (no need to open again to use).
    AUD_ERR_ALREADY_OPENED,

    /// Timeout while trying to access the resource
    AUD_ERR_RESOURCE_TIMEOUT,

    /// Invalid parameter
    AUD_ERR_BAD_PARAMETER,

    /// The specified interface does not exist
    AUD_ERR_NO_ITF,

    /// Unspecified error
    AUD_ERR_UNKNOWN,

    AUD_ERR_QTY
} AUD_ERR_T;
uint32 s_mmistk_selectitemtr_result;

 void mon_Event(uint32 evt)
{

 }
uint32_t CFW_EmodGetNvVersion( void )
{
        return 0;
}
int32_t CFW_EmodSpecialNvProcess( uint8_t operType, uint8_t dataType, uint16_t offset, uint16_t dataLength, uint8_t * data, uint8_t nSim )
{
        return 0;
}
int32_t CFW_EmodSaveNvData( uint8_t dataType, uint8_t nSim )
{
        return 0;
}
int32_t CFW_EmodSetNvData( uint8_t dataType, uint16_t offset, uint16_t dataLength, uint8_t * data, uint8_t nSim )
{
        return 0;
}

uint8_t * CFW_EmodGetNvData( uint8_t dataType, uint8_t nSim )
{
        return NULL;
}

uint32_t CFW_EmodEngineerCmd( uint32_t module, uint32_t cmd, uint32_t v1, uint32_t v2, uint32_t v3 )
{
        return 0;
}

uint32_t CSW_SetAudioCodecCalibParam( uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength )
{
        return 0;
}

uint32_t CSW_SetAudioZspVqeCalibParam( uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength )
{
        return 0;
}

uint32_t CSW_SetAudioZspPreProcessCalibParam( uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength )
{
        return 0;
}

uint32_t CSW_SetAudioZspPostProcessCalibParam( uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength )
{
        return 0;
}

uint32_t CSW_WriteCalibParamToFlash( uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength )
{
        return 0;
}

uint32_t CSW_ExportCalibOrImportCalibFlashParam( uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint16_t nOffset, uint16_t nlength, uint8_t * nParam, uint16_t nParamLength )
{
        return 0;
}

uint32_t CSW_DumpPcmDataToTflash( uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength )
{
        return 0;
}


uint32_t CSW_VersionControl( uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength )
{
        return 0;
}

void CFW_EmodGetIMEI(uint8_t pImei[16], uint8_t *pImeiLen, CFW_SIM_ID nSimID)
{
}

void CFW_EmodSaveIMEI(uint8_t pImei[16], uint8_t pImeiLen, uint8_t nSimID)
{
}

uint32_t CFW_ImsSetVolte(uint8_t bSet, uint16_t nUTI, CFW_SIM_ID nSimID)
{
	return 0;
}

uint8 PHONE_GetFreeUti(void)
{
	return 0;
}

void L1API_SetPagingWakeUpReport(BOOLEAN flag)
{
    
}

BOOLEAN L1API_GetPagingWakeUpReport(void)
{
	return TRUE;
}


ERR_MNGPRS_CODE_E MNGPRS_ReadPdpContextEx(                                           //RETURN: whether success or failure
                                        MN_DUAL_SYS_E        dual_sys,               //IN:
                                        MN_GPRS_PDP_CON_T    **ret_pdp_context_ptr, //OUT: the memory save the result
                                        uint32                *pdp_context_num       //OUT: num of exist context
                                        )
{
	return 0;
}

L1_RW_RF_REG_RESULT_E L1API_ReadWriteRfRegisterResult(uint16 p0,uint16 *reg_val)
{
	return 0;
}

void L1API_ReadWriteRfRegister(uint16 p0, uint16 p1, uint16 p2, uint16 p3)
{
    return ;
}

BOOLEAN L1API_GetGsmRXPwrResult(
                        uint16 arfcn,
                        uint8  band,
                        int16* result)
{
    return SCI_FALSE;
}

void L1API_GsmRXCWPwrTest( BOOLEAN is_debug_on, 
                        uint16 arfcn,
                        uint8 band,
                        uint16 gain)
{
    return ;
}

void L1API_GsmTxCWPwrTest( BOOLEAN is_debug_on, 
                         uint16 arfcn,
                         uint8 band,
                         uint16 gainorpower
                        )
{
    return ;
}

void ATC_ProcessGLPDNS()
{

}

void DSPVB_PcmDumpStop()
{

}

void DSPVB_PcmDumpEnable()
{

}

void CHIP_ResetMCUSoft (void)
{
}
void USB_Connect (void)
{
}
void BOOT_SetResetMode (uint32 val)
{
}

void ARMVB_SetBtWorkMode_Direct (BT_HFG_WB_FLAG *btHfgWB)
{
}


void LCD_PictureConverter(uint8 src_fmt, uint32  *src_addr, uint32 src_w, uint32 src_h, uint32 stride, 
			uint32  *dst_addr, uint32 dst_w, uint32 dst_h, uint8 rotation)
{
}

ERR_LCD_E LCD_SetImageLayerSourceSize(LCD_ID_E lcd_id, uint32 width, uint32 height)
{
	return SCI_SUCCESS;
}
ERR_LCD_E LCD_SetVBuffer (LCD_ID_E lcd_id, uint32 blk_num, uint32 *buf_ptr)
{
	return SCI_SUCCESS;
}
ERR_LCD_E LCD_SetUVBuffer (LCD_ID_E lcd_id,uint32 blk_num,uint32 *buf_ptr)
{
	return SCI_SUCCESS;
}
uint16 ARMVB_getRecordLenth(void)
{
	return 0;
}


void ADI_Analogdie_reg_write (uint32 addr, uint16 data)
{
}
uint16 ADI_Analogdie_reg_read (uint32 addr)
{
	return 0;
}
BOOLEAN EFUSE_HAL_GetRGLdoRtcCal(uint32 *rgrtccal_data)
{
}
BOOLEAN EFUSE_HAL_GetDCXOTrim(uint32 *dcxotrim_data)
{
}
uint32 MNPHONE_SetPowerOffFlag(uint8 bPoweroff,MN_DUAL_SYS_E dual_sys)
{
	return 0;
}

void ipc_notify_cp_stop_log(uint32 is_off)
{
}

NVITEM_ERROR_E NVITEM_UpdateCali(uint16 ItemID, uint16 cchSize,  const void *pBuf)
{
	return 0;
}

uint32 ATC_ProcessCGTFT(void * a,void *b)
{
	return 0;
}
PUBLIC void GPIO_SetLcdBackLightSleepEnable (void)
{
}

void hal_GoudaChangeClkWhenRecord(BOOLEAN record)
{
}

uint32 SCI_InitAtc(void)
{
    return 0;
}

uint32_t CFW_SimGetACM(uint16_t nUTI, CFW_SIM_ID nSimID)
{
	return 0;
}
void * WDG_TimerStop(void)
{

}
void IMEITransformBcdToStr(
             uint8                  *bcd_ptr, // in the BCD array
             int32                  length,      // in the length of BCD array
             uint8                  *tel_str     // out the telephone number string
             )
{
}
uint32_t CFW_GprsSetCtxEQos(uint8_t nCid, CFW_EQOS *pQos, uint8_t nSimID)
{
	return 0;
}
uint32_t CFW_GprsGetCtxEQos(uint8_t nCid, CFW_EQOS *pQos, CFW_SIM_ID nSimID)
{
	return 0;
}
void SCI_MMI_EnableDeepSleep (uint32 mode)
{
}
void SCI_ENGINEER_EnableDeepSleep (uint32 mode)
{
}
uint32 MNPHONE_ChangeRat(uint8 nRat, MN_DUAL_SYS_E dual_sys)
{
	return 0;
}
uint32 A2DP_getSamplerate(void)
{
    return 44100;
}
void XF_SavePrintTraceToMem(
    char * string,
    uint32 size,
    uint32 type    
)
{
}

AUD_ERR_T aud_HeadsetConfig(UINT32 isInsert)
{

}
void aud_CodecSC2720HeadsetMicAndButtonDetect(VOID)
{
	return 0;
}

uint32 EFUSE1_HAL_Read (int page_index)
{
	return 0;
}

BOOLEAN RM_GetCalibrationPostMode()
{
	return FALSE;
}

uint8 MNPHONE_GetAttState(MN_DUAL_SYS_E dual_sys)
{
	return 0;
}

uint32 MNPHONE_FacGetNetInfo(void *pNetInfo, MN_DUAL_SYS_E dual_sys)
{
	return 0;
}

uint32 MNPHONE_FacGetNetInfoStatistic(void *pNetinfoStatistic, MN_DUAL_SYS_E dual_sys)
{
	return 0;
}
PUBLIC uint8 * HWID_Read(void)
{
	return NULL;
}

uint32 MNPHONE_NwSetLockBCCH(BOOLEAN bLock, MN_DUAL_SYS_E dual_sys)
{
	return 0;
}

uint32 MNPHOE_CfgSetStoredPlmnList(MN_StoredPlmnList *pPlmnL, MN_DUAL_SYS_E dual_sys)
{
	return 0;
}


BOOLEAN AutoTestLoopDirectPcm(uint8 xx, uint8 yy, BOOLEAN bStart)
{
	return FALSE;
}

MN_GMMREG_RAT_E MNPHONE_GetCurrentRAT(MN_DUAL_SYS_E  dual_sys)
{
	return 0;
}

uint32_t CFW_SSCheckTestSim(CFW_SIM_ID nSimID)
{
	return 0;
}

uint32_t CFW_GprsCtxEpsModify(uint16_t nUTI, uint8_t nCid, CFW_SIM_ID nSimID)
{
	return 0;
}
void ARMVB_SetShakingMode(BOOL shaking)
{

}
uint32 MNSS_CheckTestSim(MN_DUAL_SYS_E dual_sys)
{
	return 0;
}
void SCI_LCDBacklightBrightless_EnableDeepSleep (uint32 mode)
{

}

void BOOT_SetBLStatus (uint32 val)
{

}

ERR_DEVOPER_E LP_CODE_Read(uint8 * data)
{

}

void GPIO_SetFmLNA(BOOLEAN is_high)
{

}

void SIO_PPP_UsbPlugOut (void)
{

}


void  LCD_CheckLcdESD (void)
{
}
BOOLEAN Get_AutosmsEnableEndFlag(void)
{
}
BOOLEAN SCI_ReadAUTOSMSALIVEInfo(char *buf)
{

}
BOOLEAN SCI_ReadAUTOSMSRESETInfo(char *buf)
{

}


PUBLIC uint32 PROD_GetApnMemSection(uint8** addr)
{
	return 0;
}

void ARMVB_SetToneMode(UINT32 isToneMode)
{

}
void TransformBcdToStr( uint8   *bcd_ptr,   int32 length,  uint8 *tel_str)
{
}
uint32 MNPHONE_ImsNvParamSet(uint16 param_id, uint8 *value_ptr, MN_DUAL_SYS_E dual_sys)
{
	return 0;
}

uint32 MNPHONE_ImsNvParamGet(uint16 param_id, uint8 *value_ptr, MN_DUAL_SYS_E dual_sys)
{
	return 0;
}

PUBLIC uint32 PROD_GetConfigNVMemSection(uint8** addr)
{
	return 0;
}
PUBLIC uint32 PROD_GetImsMemSection(uint8** addr)
{
	return 0;
}

INT32 AUDIO_ZSP_MusicVibrateStartAllTime(void)
{
	return 0;
}
void ARMVB_SetZspVibratePara_Direct(BOOL shaking)
{

}
void ARMVB_SetCompositionofRingandVibFlag(BOOL composition)
{
    return;
}
INT32 AUDIO_ZSP_MusicVibrateStop(void)
{
	return 0;
}
uint32 MNPHONE_ResetImsDelta(void)
{
	return 0;
}


uint32 MNPHONE_SetGcfTest(MN_DUAL_SYS_E dual_sys, uint8 flag)
{
	return 0;
}

int32 ipc_ch_write(struct smd_ch *ch, const void *data, uint32 len)
{
	return 0;
}

int32 ipc_alloc_ul_ps_buf(struct smd_ch *ch, uint32 size)
{
	return 0;
}

void sio_ppp_tx_enable(int enable_flag)
{
	return 0;
}
uint8 *sio_ppp_alloc_tx_buf(void)
{
	return 0;
}
uint32 sio_ppp_tx_freebuf_cnt(void)
{
	return 0;
}
INT32 AUDIO_ZSP_MusicVibrateStart(UINT32 time)
{
	return 0;
}


uint32 PROD_GetImsToggleMemSection(uint8** addr)
{

}
void MNPHONE_ResetNetMode(void)
{

}

uint32 BT_Stop_Ex(void)
{

}

#if defined(BLUETOOTH_SUPPORT)
BOOLEAN BT_IsEnabled(void)
{

}
#endif


void FLASH_GetFotaNvitemAddr (uint32 *start_addr, uint32 *end_addr)
{

}

#ifndef __MN_SIM_ADP_CFW__
uint8 s_cfw_call_index[MULTI_SYS_NUM][16] = {0,};
uint32_t CFW_SatResponse(uint8_t nCmdType, uint8_t nStatus, uint8_t nItemId, void * pInputString, uint8_t InputStrLen, uint16_t nUTI, CFW_SIM_ID nSimID)
{}
#endif


const char*  VERSION_GetInfo(uint32 type)
{
    return "Version number is xxx";
}

BOOLEAN MNPHONE_ImsIsSet(MN_DUAL_SYS_E dual_sys)
{
	return FALSE;
}
BOOLEAN MNPHONE_ImsIsSupport(MN_DUAL_SYS_E dual_sys)
{
	return FALSE;
}


#ifndef __MN_PHONE_ADP_CFW__
void MNPHONE_SetLteLockFreq(uint8 number, uint32 * freq, MN_DUAL_SYS_E  dual_sys)
{
	return;
}
void MNPHONE_GetLteLockFreq(uint8 * number, uint32 * freq, MN_DUAL_SYS_E  dual_sys)
{
	return;
}
void MNPHONE_SetLteLockBand(uint8 number, uint16 * band, MN_DUAL_SYS_E  dual_sys)
{
	return;
}
void MNPHONE_GetLteLockBand(uint8 * number, uint16 * band, MN_DUAL_SYS_E  dual_sys)
{
	return;
}
void MNPHONE_SetGsmBand(uint8 nBand, MN_DUAL_SYS_E dual_sys)
{
	return;
}
void MNPHONE_GetGsmBand(uint8* pBand, MN_DUAL_SYS_E dual_sys)
{
	return;
}
void MNPHONE_SetRat(uint8 nRat, MN_DUAL_SYS_E dual_sys)
{
	return;
}
uint8 MNPHONE_GetRat(MN_DUAL_SYS_E dual_sys)
{
	return 0;
}
void MNPHONE_GetCEMode(uint8* pMode, MN_DUAL_SYS_E dual_sys)
{
	return;
}
void MNPHONE_SetCEMode(uint8 nMode, MN_DUAL_SYS_E dual_sys)
{
	return;
}
void MNPHONE_GetVoicePrefer(uint8* pVoicePrefer, MN_DUAL_SYS_E dual_sys)
{
	return;
}
void MNPHONE_SetVoicePrefer(uint8 nVoicePrefer, MN_DUAL_SYS_E dual_sys)
{
	return;
}
BOOLEAN MNPHONE_IsSupprtImsEmc(MN_DUAL_SYS_E dual_sys)
{
    return FALSE;
}

#endif
#endif

