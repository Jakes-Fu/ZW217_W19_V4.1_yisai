/*******************************************************************************

           (c) Cambridge Silicon Radio Limited 2009

           All rights reserved and confidential information of CSR

*******************************************************************************/
#include "os_api.h"
#include "tb_drv.h"
#include "tb_comm.h"
#include "wifi_drv.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_framework_ext.h"
#include "csr_spi_master.h"
#include "csr_sdio_debug.h"

/*lint -e1778*/
/* gpio_prod.h */

/******************************/
/********* Parameters *********/
/******************************/
#define DEVICE_COUNT 1

#undef MHZ
#define MHZ  (1000000)

#ifdef PLATFORM_SC6600L
#define MAX_FREQUENCY (5*MHZ)   //Limit to 5MHz, while platform can do up to 24 MHz
#else
#define MAX_FREQUENCY (24*MHZ)
//#define MAX_FREQUENCY 6000000 
#endif

//#define CSID_FOR_WIFI_ASSERT 0x0e
//#define CSID_FOR_WIFI_NEGATE 0x0f

/**************************************/
/********* Host Configuration *********/
/**************************************/

#define INT_ATOMIC_EXECUTE_HEAD SCI_DisableIRQ
#define INT_ATOMIC_EXECUTE_TAIL SCI_RestoreIRQ
/*******************************/
/********* Static Data *********/
/*******************************/

#define SPIM_STATE_STOPPED              0
#define SPIM_STATE_IDLE                 1
#define SPIM_STATE_BURST_INPUT          2
#define SPIM_STATE_BURST_OUTPUT         3
static CsrSpiMasterDevice spimDevices[DEVICE_COUNT];

static CsrUint32 state = SPIM_STATE_STOPPED;

static CsrSpiMasterCallback eventInterruptCallback[DEVICE_COUNT] = {NULL};
static CsrSpiMasterDsrCallback eventInterruptDsrCallback[DEVICE_COUNT] = {NULL};

#define CSR_SPI_MASTER_EVENT_RUN_HISR 1
#define CSR_SPI_MASTER_EVENT_UNINSTAL_HISR  2

/*NOTE: This function is called directry from the ISR. Minimum processing should be done */
static CsrSpiMasterCallback function2CallInISR = NULL;

LOCAL uint32 CsrSpiMasterGpioIrq(uint32  param)
{
    if (function2CallInISR != NULL)
    {
        // This runs the low level interrupt function
        CsrBool executeHISR = function2CallInISR(&spimDevices[0]);
        if (TRUE == executeHISR)
        {  
            if (eventInterruptDsrCallback[0] != NULL)
            {
                eventInterruptDsrCallback[0](&spimDevices[0]);
            }
        }
    }
}

CsrResult CsrSpiMasterPowerOn(CsrSpiMasterDevice *device)
{
    /* WiFi internal LDO control is done at WIFI_Open/Close */
    return CSR_SPI_MASTER_RESULT_NOT_RESET;
}

void CsrSpiMasterPowerOff(CsrSpiMasterDevice *device)
{
    /* WiFi internal LDO control is done at WIFI_Open/Close */
}

void CsrSpiMasterInitialise(CsrUint8 *devicesCount, CsrSpiMasterDevice **devices)
{
    CSR_SDIO_PRINTF(("CsrSpiMasterInitialise"));
    
    if (state == SPIM_STATE_STOPPED)
    {
        CsrUint8 deviceIndex;
        *devicesCount = DEVICE_COUNT;
        *devices = spimDevices;

        /* Disable IRQ from WiFi card */
        WIFI_interrupt_enable(FALSE);
        
        eventInterruptCallback[0] = NULL;
        
        /* Only one port is open (DEVICE_COUNT=1*/
        for (deviceIndex = 0; deviceIndex < DEVICE_COUNT; deviceIndex++)
        {
            spimDevices[deviceIndex].deviceIndex = deviceIndex;
            spimDevices[deviceIndex].busIndex = 0;
            spimDevices[deviceIndex].features = 0;
            spimDevices[deviceIndex].driverData = NULL;
            spimDevices[deviceIndex].priv = NULL;

            if (WIFI_Open(WIFI_PORT_TYPE_SPI, CsrSpiMasterGpioIrq) != WIFI_RET_OK)
            {
                *devicesCount = 0;
                *devices = NULL;
                CSR_SDIO_PRINTF(("Failed to Open SPI Port"));
                return ;
            }
            /* Reset Host Hardware */
            CsrSpiMasterChipSelectNegate(&spimDevices[deviceIndex]);           
        } /* for */
        
        /* Reset state */	
        state = SPIM_STATE_IDLE;
        CSR_SDIO_PRINTF(("SPI Port initialised"));        
    }/* if */
}

void CsrSpiMasterDeinitialise(void)
{
    CSR_SDIO_PRINTF(("CsrSpiMasterDeinitialise"));
    if (state != SPIM_STATE_STOPPED)
    {
        CsrUint8 deviceIndex;

        for (deviceIndex = 0; deviceIndex < DEVICE_COUNT; deviceIndex++)
        {
            /* Reset Host Hardware */
            CsrSpiMasterChipSelectNegate(&spimDevices[deviceIndex]);            
        }
        WIFI_interrupt_enable(FALSE);
        WIFI_Close();

        eventInterruptCallback[0] = NULL;
        
        state = SPIM_STATE_STOPPED;
    }
}

static void verifyDevice(CsrSpiMasterDevice *device)
{
    CSR_SDIO_ASSERT(device, "CsrSpiMasterDevice == NULL");/*lint !e666*/
    CSR_SDIO_ASSERT((device->priv == NULL), "CsrSpiMasterDevice.priv != NULL");/*lint !e666 !e613*/
    CSR_SDIO_ASSERT((device->features == 0), "CsrSpiMasterDevice.features != 0");/*lint !e666 !e613*/
    CSR_SDIO_ASSERT(device->deviceIndex == 0 || device->deviceIndex == 1, "Unknown deviceIndex");/*lint !e666 !e613*/
    CSR_SDIO_ASSERT(device->busIndex == 0, "Unknown busIndex");/*lint !e666 !e613*/
}

void CsrSpiMasterRead(CsrSpiMasterDevice *device, CsrUint8 *data, CsrUint32 length)
{
    CsrUint32 result;
    result = WIFI_Read(data, length);
    CSR_SDIO_ASSERT(result == WIFI_RET_OK, "SPI Read failed");/*lint !e666 !e613*/
}

void CsrSpiMasterReadAsync(CsrSpiMasterDevice *device, CsrUint8 *data, CsrUint32 length, CsrSpiMasterCallback operationCallback)
{
	CSR_SDIO_PRINTF(("CsrSpiMasterReadAsync"));
    CSR_SDIO_PRINTF(("Not implemented"));
}

void CsrSpiMasterWrite(CsrSpiMasterDevice *device, const CsrUint8 *data, CsrUint32 length)
{
    CsrUint32 result;
    result = WIFI_Write((uint8*)data, length);
    CSR_SDIO_ASSERT(result == WIFI_RET_OK, "SPI Write failed");/*lint !e666 !e613*/ 
}

void CsrSpiMasterWriteAsync(CsrSpiMasterDevice *device, const CsrUint8 *data, CsrUint32 length, CsrSpiMasterCallback operationCallback)
{
	CSR_SDIO_PRINTF(("CsrSpiMasterWriteAsync"));
    CSR_SDIO_PRINTF(("Not implemented"));
}

void CsrSpiMasterChipSelectAssert(CsrSpiMasterDevice *device)
{
    WIFI_Ctl_Bus_No_Mutex(TRUE);
}

void CsrSpiMasterChipSelectNegate(CsrSpiMasterDevice *device)
{
    WIFI_Ctl_Bus_No_Mutex(FALSE);
}

void CsrSpiMasterBusClockFrequencySet(CsrSpiMasterDevice *device, CsrUint32 *frequency)
{
    CsrUint32 hz = (*frequency > MAX_FREQUENCY)? MAX_FREQUENCY : *frequency; 
    	
	//CSR_SDIO_PRINTF(("CsrSpiMasterBusClockFrequencySet: %d", hz));
    WIFI_Set_Clock_Frequency(hz);
}

static void setupServiceRoutine(void)
{
    if (eventInterruptCallback[0])
    {
        function2CallInISR = eventInterruptCallback[0];
    }
    else
    {
       function2CallInISR = NULL;
    }
}

void CsrSpiMasterCallbackRegister(CsrSpiMasterDevice *device, CsrSpiMasterCallback interruptCallback, CsrSpiMasterDsrCallback interruptDsrCallback, CsrSpiMasterDsrCallback operationDsrCallback)
{
    CSR_SDIO_PRINTF(("CsrSpiMasterCallbackRegister"));
    INT_ATOMIC_EXECUTE_HEAD();

    verifyDevice(device);
	
    eventInterruptDsrCallback[0] = interruptDsrCallback;

    if (interruptCallback != NULL)
    {
        if (eventInterruptCallback[0] != NULL)
        {
            /* Already registered - just replace the handler */
            eventInterruptCallback[0] = interruptCallback;
        }
        else
        {			
            /* New registration */
            eventInterruptCallback[0] = interruptCallback;

            /* Setup appropriate service routine */
            setupServiceRoutine();
            /* Clear and unmask */
            WIFI_interrupt_enable(TRUE);
        }
    }
    else
    {
        if (eventInterruptCallback[0] != NULL)
        {
            eventInterruptCallback[0] = NULL;

            /* Mask and clear */
            WIFI_interrupt_enable(FALSE);
            /* Setup appropriate service routine */
            setupServiceRoutine();
        }
    }
    INT_ATOMIC_EXECUTE_TAIL();
    CSR_SDIO_PRINTF(("CsrSpiMasterCallbackRegister End"));
}

void CsrSpiMasterInterruptAcknowledge(CsrSpiMasterDevice *device)
{
    verifyDevice(device);

    /* Unmask */
   WIFI_interrupt_enable(TRUE);
}

static CsrUint32 nestCount = 0;
void CsrSpiMasterCallbackInhibitEnter(CsrSpiMasterDevice *device)
{
    if (nestCount == 0)
    {
        SCI_DisableIRQ();
    }
    nestCount++ ;
}

void CsrSpiMasterCallbackInhibitLeave(CsrSpiMasterDevice *device)
{
    nestCount --;

    if (nestCount == 0)
    {
        SCI_RestoreIRQ();
    }
}

