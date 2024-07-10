#include "sci_types.h"
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#include "com_drvapi.h"
#endif
//void RTC_SetRtcInt()
//{}

void CHIP_ChangeCoreVoltage(void)
{
}

void CHIP_RestoreARMClk(void)
{
}

void XSR_Init(void)
{
}

uint32 CHIP_DetectMemType(void)
{
	return 0;
}

void BSD_Initialize(void)
{
}

void NANDCTL_Init(void)
{
}

void DMA_init(void)
{
}

void RM_GetVcomRunMode(void)
{
}

void RM_GetRunModeResponse(void)
{
}

void RM_SetIQCTestFlag(BOOLEAN is_iqctest)
{
}

void RM_SetPinTestFlag(BOOLEAN is_pintest_mode)
{
}

void SCI_Calibration_EnableDeepSleep(uint32 mode)
{
}

void RM_SetCalibrationPostMode(BOOLEAN is_calibration)
{
}

uint32 CHIP_GetAhbClk(void)
{
    return 0;
}

void CHIP_BootReset(void)
{
}

void Syscnt_Init(void)
{
}

void RM_SetCalibrationPostNoLcmMode(
	BOOLEAN is_calibrationpostnolcm			//calibraton post mode flag to set
	)
{

}

uint8  NVITEM_UpdateProductInfo(uint32 start_addr, uint8 *pSrc, uint32 len)
{
    return 0;
}

void CHIP_DisableFIQ(void)
{
}

uint32 PROD_InitByStep(uint32  step)
{
    return 1;
}

BOOLEAN MEM_IsRORegion(uint32 data_addr)
{
    return 1;
}

BOOLEAN MEM_IsRWRegion(uint32 data_addr)
{
    return 1;
}

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
PUBLIC void UART1ArbitrateSwitch(UART_CONTROL_TYPE_E control_type)
{
    //dummy
}
#else
void DSPLogUARTSwitchOn(void)
{
    //dummy
}

void DSPLogUARTSwitchOff(void)
{
    //dummy
}
#endif
