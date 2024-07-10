/*******************************************************************************

           (c) Cambridge Silicon Radio Limited 2009

           All rights reserved and confidential information of CSR

*******************************************************************************/
#include "os_api.h"
#include "tb_drv.h"
#include "tb_comm.h"
#include "wifi_drv.h"

#include "csr_gpio_debug.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_framework_ext.h"
#include "csr_spi_master.h"
#include "csr_sdio_debug.h"
/*lint -e1778*/
/* gpio_prod.h */
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"

/******************************/
/********* Parameters *********/
/******************************/
#define DEVICE_COUNT 1

#undef MHZ
#define MHZ  (1000000)

#ifdef PLATFORM_SC6600L
    #define MAX_FREQUENCY (5*MHZ)   //Limit to 5MHz, while platform can do up to 24 MHz
#elif defined(PLATFORM_SC8800G)
    #define MAX_FREQUENCY (48*MHZ)
#else
    #define MAX_FREQUENCY (24*MHZ)
#endif

#define CSR_SPI_DMA_THRESHOLD 128
#define CSR_SPI_POLL_32BIT_THRESHOLD 5000

/**************************************/
/********* Host Configuration *********/
/**************************************/

// CSR: Add guard against FIQ, in case this is important
#define INT_ATOMIC_EXECUTE_HEAD() do{SCI_DisableIRQ(); SCI_DisableFIQ();}while(0)
#define INT_ATOMIC_EXECUTE_TAIL() do{SCI_RestoreFIQ(); SCI_RestoreIRQ();}while(0)
/*******************************/
/********* Static Data *********/
/*******************************/

#define SPIM_STATE_STOPPED              0
#define SPIM_STATE_IDLE                 1
#define SPIM_STATE_BURST_INPUT          2
#define SPIM_STATE_BURST_OUTPUT         3
static CsrSpiMasterDevice spimDevices[DEVICE_COUNT];

static CsrUint32 state = SPIM_STATE_STOPPED;

static SCI_EVENT_GROUP_PTR eventHandle;

static CsrSpiMasterDsrCallback eventInterruptDsrCallback[DEVICE_COUNT] = {NULL};

// CSR: static variable to store the ID needed to reference the GPIO IRQ
static CsrUint32 gpio_int_id;

// CSR: Note that a number of unused functions have been removed
#define CSR_SPI_MASTER_EVENT_RUN_HISR 1
#define CSR_SPI_MASTER_EVENT_UNINSTAL_HISR  2

/*NOTE: This function is called directry from the ISR. Minimum processing should be done */
static CsrSpiMasterCallback function2CallInISR = NULL;

static void ioInit(void);

/* CSR: Added direct IRQ handler. This one takes over the responsibility for unmasking the IRQ 
   from the lower layers. In CSR SPI, unmask happens in CsrSpiMasterInterruptAcknowledge() */
LOCAL void GPIO_CsrSpiIntHander (uint32 gpio_id, uint32 gpio_state)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_IRQ_HANDLER);
    /* HACK: The GPIO IRQ does not actually support pure level-trigger, so we might 
       get an interrupt on a high level. Just force the trigger to low level, unmask and return.
       NOTE: This will only happen once at initialization */
/* t*/
    if (gpio_state)
    {
        GPIO_SetInterruptSense(gpio_int_id, GPIO_INT_LEVEL_LOW);
        GPIO_EnableIntCtl(gpio_int_id);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IRQ_HANDLER);
        return;
    }

    if(!(function2CallInISR && eventInterruptDsrCallback[0]))
    {
        SCI_TraceLow("CSR: %s - No ISR handlers registered", __func__);
    }                                                                                
    //SCI_PASSERT(function2CallInISR && eventInterruptDsrCallback[0], ("No ISR handlers registered"));

    if (function2CallInISR(&spimDevices[0]))
    {
        eventInterruptDsrCallback[0](&spimDevices[0]);
    }
    // By doing nothing here, the IRQ just stays masked, ensuring that there is no looping on the level-triggered IRQ
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IRQ_HANDLER);
}

void CsrSpiMasterInitialise(CsrUint8 *devicesCount, CsrSpiMasterDevice **devices)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT);
    CSR_SDIO_PRINTF(("CsrSpiMasterInitialise"));

    if (state == SPIM_STATE_STOPPED)
    {
        CsrUint8 deviceIndex;
        GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_WIFI_INT_ID); // CSR: Structure for storing the GPIO ID needed when accessing GPIO IRQ APIs

        *devicesCount = DEVICE_COUNT;
        *devices = spimDevices;

        ioInit();

        // CSR: Take the GPIO ID into a local variable for later reference
        gpio_int_id = cfg_ptr->gpio_num;

        // CSR: Override standard interrupt control
/* t */
		GPIO_DisableIntCtl(gpio_int_id);
		GPIO_DeleteFromIntTable(gpio_int_id);
		GPIO_PROD_RegGpio(GPIO_PROD_WIFI_INT_ID,SCI_FALSE,SCI_FALSE,SCI_FALSE,0,(GPIO_PROD_CALLBACK)GPIO_CsrSpiIntHander);
		GPIO_DisableIntCtl(gpio_int_id);
		GPIO_SetLisrInt(gpio_int_id, TRUE);

        eventHandle = SCI_CreateEvent("SPI_EVT");
        SCI_SetEvent(eventHandle, 0, SCI_AND);

        // CSR: Old IRQ code
        //WIFI_interrupt_enable(FALSE);

        /* Only one port is open (DEVICE_COUNT=1*/
        for (deviceIndex = 0; deviceIndex < DEVICE_COUNT; deviceIndex++)
        {
            spimDevices[deviceIndex].deviceIndex = deviceIndex;
            spimDevices[deviceIndex].busIndex = 0;
            spimDevices[deviceIndex].features = 0;
            spimDevices[deviceIndex].driverData = NULL;
            spimDevices[deviceIndex].priv = NULL;

		// t	if (WIFI_Open(WIFI_PORT_TYPE_SPI, GPIO_CsrSpiIntHander) != WIFI_RET_OK)
          if (WIFI_Open(WIFI_PORT_TYPE_SPI, NULL) != WIFI_RET_OK)
            {
                *devicesCount = 0;
                *devices = NULL;
                CSR_SDIO_PRINTF(("Failed to Open SPI Port"));
                
                CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT);
                return ;
            }

            // CSR: Negate CS directly to avoid hitting a bad mutex state.
            WIFI_Ctl_Bus_No_Mutex(FALSE);
        } /* for */
        
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_INIT_TEST);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT_TEST);
        
        /* Reset state */	
        state = SPIM_STATE_IDLE;
        CSR_SDIO_PRINTF(("Failed to Open SPI Port"));
    }/* if */
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_INIT);
}

void CsrSpiMasterDeinitialise(void)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_DEINIT);
    CSR_SDIO_PRINTF(("CsrSpiMasterDeinitialise"));
    
    if (state != SPIM_STATE_STOPPED)
    {
        // CSR: Unregister the GPIO IRQ
        GPIO_DeleteFromIntTable(gpio_int_id);

        // CSR: No need to negate CS, only place this can be in a bad state is if the system actually crashed, and then it doesn't matter...

        SCI_DeleteEvent(eventHandle);
        eventHandle = NULL;

        // CSR: Old IRQ code
        //WIFI_interrupt_enable(FALSE);
        WIFI_Close();
        state = SPIM_STATE_STOPPED;
    }
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_DEINIT);
}

static void verifyDevice(CsrSpiMasterDevice *device)
{
    CSR_SDIO_ASSERT(device, "CsrSpiMasterDevice == NULL");/*lint !e666*/
    CSR_SDIO_ASSERT((device->priv == NULL), "CsrSpiMasterDevice.priv != NULL");/*lint !e666 !e613*/
    CSR_SDIO_ASSERT((device->features == 0), "CsrSpiMasterDevice.features != 0");/*lint !e666 !e613*/
    CSR_SDIO_ASSERT(device->deviceIndex == 0 || device->deviceIndex == 1, "Unknown deviceIndex");/*lint !e666 !e613*/
    CSR_SDIO_ASSERT(device->busIndex == 0, "Unknown busIndex");/*lint !e666 !e613*/
}

static int32 opCallback(uint32 *arg)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_OP_CALLBACK);
    SCI_SetEvent(eventHandle, 1, SCI_OR);
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_OP_CALLBACK);
    return 0;
}

void CsrSpiMasterRead(CsrSpiMasterDevice *device, CsrUint8 *data, CsrUint32 length)
{
	CsrUint32 result;
	
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_READ);
    CsrMemSet(data,0,length);
    result = WIFI_Read(data, length);
    CSR_SDIO_ASSERT(result == WIFI_RET_OK, "SPI Read failed");/*lint !e666 !e613*/
    //SCI_TraceLow("%s r %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:",__func__,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9]);
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_READ);
}

void CsrSpiMasterReadWrite(CsrSpiMasterDevice *device, CsrUint8 *data_in, CsrUint8 *data_out, CsrUint32 length)
{
	CsrUint32 result;
	
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_READ_WRITE);

    //SCI_TraceLow("%s w %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:" ,__func__,data_out[0],data_out[1],data_out[2],data_out[3],data_out[4],data_out[5],data_out[6],data_out[7],data_out[8],data_out[9]);
	result = WIFI_ReadWrite( data_in, data_out, length);	 
	CSR_SDIO_ASSERT(result == WIFI_RET_OK, "SPI Read/Write failed");/*lint !e666 !e613*/	
    //SCI_TraceLow("%s r %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:",__func__,data_in[0],data_in[1],data_in[2],data_in[3],data_in[4],data_in[5],data_in[6],data_in[7],data_in[8],data_in[9]);
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_READ_WRITE);
}

void CsrSpiMasterWrite(CsrSpiMasterDevice *device, const CsrUint8 *data, CsrUint32 length)
{
	CsrUint32 result;
	
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_WRITE);
    //SCI_TraceLow("%s w %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:", __func__,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9]);
    result = WIFI_Write((uint8*)data, length);
    CSR_SDIO_ASSERT(result == WIFI_RET_OK, "SPI Write failed");/*lint !e666 !e613*/   
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_WRITE);
}   

void CsrSpiMasterChipSelectAssert(CsrSpiMasterDevice *device)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_CS_ASSERT);
#ifdef CSP_SPI_SHARE_WITH_OTHER_DEVICE
    // CSR: Just use the mutex directly in case sharing must be used, is faster than the old multiple turn-arounds
    WIFI_Ctl_Bus(TRUE);
#else
    WIFI_Ctl_Bus_No_Mutex(TRUE);
#endif
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_CS_ASSERT);
}

void CsrSpiMasterChipSelectNegate(CsrSpiMasterDevice *device)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_CS_NEGATE);
#ifdef CSP_SPI_SHARE_WITH_OTHER_DEVICE
    // CSR: Just use the mutex directly in case sharing must be used, is faster than the old multiple turn-arounds
    WIFI_Ctl_Bus(FALSE);
#else
    WIFI_Ctl_Bus_No_Mutex(FALSE);
#endif
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_CS_NEGATE);
}


typedef int32 (*ioProcessFunction)(CsrSpiMasterOperation *op);

static int32 ioProcessor(uint32 *arg);

static CsrSpiMasterOperation *opList; 
static CsrUint32 opCount;
static CsrUint32 opIdx;

static CsrUint8 wBuffer[32];
static CsrUint8 cmdBuffer[32];
static CsrUint8 resBuffer[32];

static uint32 poll_len, dma_len, len32 = 32, len8 = 8, csHigh = SPI_CS_HIGH, csLow = SPI_CS_LOW, mutexGet = SPI_MUTEX_GET, mutexPut = SPI_MUTEX_PUT;
static CsrUint8 *opData;
#define CSR_SPI_MASTER_SUCCESS CSR_RESULT_SUCCESS
#define CSR_SPI_MASTER_PENDING 1

static int32 iobulkReadComplete(uint32 *arg)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_IO_BULK_READ_COMPLETE);
#if 1
	if (poll_len)
		WIFI_Read(&opData[dma_len], poll_len);
	WIFI_Ctl_Bus_No_Mutex(FALSE);
#else
    SPI_HAL_Ioctl(0, SPI_CTL_S_BITLEN, &len8);
    if (poll_len)
    {
        SPI_HAL_Read(0, &opData[dma_len], poll_len);
    }
    SPI_HAL_Ioctl(0, SPI_CTL_S_CS_NO_MUTEX, &csHigh);
#endif	
    if (CSR_SPI_MASTER_SUCCESS == ioProcessor(NULL))
    {
        SCI_SetEvent(eventHandle, 1, SCI_OR);
    }
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_READ_COMPLETE);
    return 0;
}

static int32 iobulkWriteComplete(uint32 *arg)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_IO_BULK_WRITE_COMPLETE);
#if 1
	if (poll_len)
		WIFI_Write((uint8*)(&opData[dma_len]), poll_len);
	WIFI_Ctl_Bus_No_Mutex(FALSE);
#else
    SPI_HAL_Ioctl(0, SPI_CTL_S_BITLEN, &len8);
    if (poll_len)
    {
        SPI_HAL_Write(0, &opData[dma_len], poll_len);
    }
    SPI_HAL_Ioctl(0, SPI_CTL_S_CS_NO_MUTEX, &csHigh);
#endif		
    if (CSR_SPI_MASTER_SUCCESS == ioProcessor(NULL))
    {
        SCI_SetEvent(eventHandle, 1, SCI_OR);
    }
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_WRITE_COMPLETE);
    return 0;
}

static int32 ioProcessBulkRead(CsrSpiMasterOperation *op)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_IO_BULK_READ);
	cmdBuffer[0] = (CsrUint8) (0x50 | (op->functionId & 0x7));
	cmdBuffer[1] = (CsrUint8) ((op->address >> 16) & 0xFF);
	cmdBuffer[2] = (CsrUint8) ((op->address >> 8) & 0xFF);
	cmdBuffer[3] = (CsrUint8) ((op->address >> 0) & 0xFF); /*lint !e835*/

#if 1
	WIFI_Ctl_Bus_No_Mutex(TRUE);
	WIFI_Write(cmdBuffer, CSR_CSPI_BURST_READ_HEADER_LENGTH); /*lint !e835*/
	if (op->length < CSR_SPI_DMA_THRESHOLD)
	{
		WIFI_Read(op->data, op->length);
		WIFI_Ctl_Bus_No_Mutex(FALSE);
		CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_READ);
		return CSR_SPI_MASTER_SUCCESS;
	}
	else
	{
		poll_len = op->length & 63;
		dma_len = op->length - poll_len;
		opData = op->data;
		//add by yanyu.lu
		WIFI_Read_Asyn(opData, dma_len, iobulkReadComplete);
		//end by yanyu.lu
		CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_READ);
		return CSR_SPI_MASTER_PENDING;
	}
#else
    SPI_HAL_Ioctl(0, SPI_CTL_S_CS_NO_MUTEX, &csLow);
    SPI_HAL_Write(0, cmdBuffer, CSR_CSPI_BURST_READ_HEADER_LENGTH); /*lint !e835*/
    if (op->length < CSR_SPI_DMA_THRESHOLD)
    {
        if (op->length >= CSR_SPI_POLL_32BIT_THRESHOLD)
        {
            poll_len = op->length & 3;
            dma_len = op->length - poll_len;
            SPI_HAL_Ioctl(0, SPI_CTL_S_BITLEN, &len32);
            SPI_HAL_Read(0, op->data, dma_len);
            SPI_HAL_Ioctl(0, SPI_CTL_S_BITLEN, &len8);
            if (poll_len)
            {
                SPI_HAL_Read(0, &op->data[dma_len], poll_len);
            }
        }
        else
        {
            SPI_HAL_Read(0, op->data, op->length);
        }
        SPI_HAL_Ioctl(0, SPI_CTL_S_CS_NO_MUTEX, &csHigh);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_READ);
        return CSR_SPI_MASTER_SUCCESS;
    }
    else
    {
        poll_len = op->length & 63;
        dma_len = op->length - poll_len;
        opData = op->data;
        SPI_HAL_Ioctl(0, SPI_CTL_S_BITLEN, &len32);
        //add by yanyu.lu
        WIFI_Read_Asyn(opData, dma_len, iobulkReadComplete);
        //end by yanyu.lu
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_READ);
        return CSR_SPI_MASTER_PENDING;
    }
#endif	
}

static int32 ioProcessBulkWrite(CsrSpiMasterOperation *op)
{
    CsrUint8 *writeData = op->data - CSR_CSPI_BURST_WRITE_HEADER_LENGTH; /*lint !e835*/
    CsrUint8 *writeHdr = writeData;
    CsrUint32 length = op->length + CSR_CSPI_BURST_WRITE_HEADER_LENGTH; /*lint !e835*/

    if(!writeData) /*lint !e774*/
    {
        SCI_TraceLow("CSR: %s - NULL data pointer!", __func__);
    }    
    //SCI_PASSERT(writeData, ("NULL data pointer"));

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_IO_BULK_WRITE);

	writeHdr[0]  = (CsrUint8) (0x60 | (op->functionId & 0x7));
	writeHdr[1] =  (CsrUint8) ((op->address >> 16) & 0xFF);
	writeHdr[2] =  (CsrUint8) ((op->address >>  8) & 0xFF);
	writeHdr[3] =  (CsrUint8) ((op->address >>  0) & 0xFF); /*lint !e835*/

    WIFI_Ctl_Bus_No_Mutex(TRUE);
#if 1	
	if (length < CSR_SPI_DMA_THRESHOLD)
    {
       	WIFI_Write(writeData, length);
        WIFI_Ctl_Bus_No_Mutex(FALSE);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_WRITE);
        return CSR_SPI_MASTER_SUCCESS;
    }
    else
    {
        poll_len = length & 63;
        dma_len = length - poll_len;
        opData = writeData;
        
        //add by yanyu.lu
        WIFI_Write_Asyn( writeData, dma_len, iobulkWriteComplete);
        //end by yanyu.lu
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_WRITE);
        return CSR_SPI_MASTER_PENDING;
    }
	
#else
    if (length < CSR_SPI_DMA_THRESHOLD)
    {
        if (length >= CSR_SPI_POLL_32BIT_THRESHOLD)
        {
            poll_len = length & 3;
            dma_len = length - poll_len;
            SPI_HAL_Ioctl(0, SPI_CTL_S_BITLEN, &len32);
            SPI_HAL_Write(0, writeData, dma_len);
            SPI_HAL_Ioctl(0, SPI_CTL_S_BITLEN, &len8);
            if (poll_len)
            {
                SPI_HAL_Write(0, &writeData[dma_len], poll_len); /*lint !e613*/
            }
        }
        else
        {
            SPI_HAL_Write(0, writeData, length);
        }
        SPI_HAL_Ioctl(0, SPI_CTL_S_CS_NO_MUTEX, &csHigh);
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_WRITE);
        return CSR_SPI_MASTER_SUCCESS;
    }
    else
    {
        poll_len = length & 63;
        dma_len = length - poll_len;
        opData = writeData;
        SPI_HAL_Ioctl(0, SPI_CTL_S_BITLEN, &len32);
        
        //add by yanyu.lu
        WIFI_Write_Asyn( writeData, dma_len, iobulkWriteComplete);
        //end by yanyu.lu
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_BULK_WRITE);
        return CSR_SPI_MASTER_PENDING;
    }
#endif	
}

static int32 ioProcessRegRead(CsrSpiMasterOperation *op)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_IO_REG_READ);
	cmdBuffer[0] = (CsrUint8) (0x10 | (op->functionId & 0x7)); /* Command: STOP:nREG/BURST:WRITE:READ:0:F2:F1:F0 */
	cmdBuffer[1] = (CsrUint8) ((op->address >> 16) & 0xFF);
	cmdBuffer[2] = (CsrUint8) ((op->address >> 8) & 0xFF);
	cmdBuffer[3] = (CsrUint8) ((op->address >> 0) & 0xFF);  /*lint !e835*/
	
	WIFI_Ctl_Bus_No_Mutex(TRUE);
    WIFI_ReadWrite(resBuffer, cmdBuffer, 4 + 1 + CSR_CSPI_PADDING_REGISTER + 2);
    WIFI_Ctl_Bus_No_Mutex(FALSE);

	op->regData = ( resBuffer[4+ 1 + CSR_CSPI_PADDING_REGISTER] << 8) |  resBuffer[4+ 2 + CSR_CSPI_PADDING_REGISTER];  

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_REG_READ);
    return CSR_SPI_MASTER_SUCCESS;
}

static int32 ioProcessRegWrite(CsrSpiMasterOperation *op)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_IO_REG_WRITE);
	wBuffer[0] = (CsrUint8) (0x20 | (op->functionId & 0x7)); /* Command: STOP:nREG/BURST:WRITE:READ:0:F2:F1:F0 */;
	wBuffer[1] = (CsrUint8) ((op->address >> 16) & 0xFF);
	wBuffer[2] = (CsrUint8) ((op->address >>  8) & 0xFF);
	wBuffer[3] = (CsrUint8) ((op->address >>  0) & 0xFF);  /*lint !e835*/
	wBuffer[4] = (CsrUint8) ((op->regData >>  8) & 0xFF);
	wBuffer[5] = (CsrUint8) (op->regData & 0xFF);

    WIFI_Ctl_Bus_No_Mutex(TRUE);
	WIFI_Write(wBuffer, 6);
    WIFI_Ctl_Bus_No_Mutex(FALSE);

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO_REG_WRITE);
    return CSR_SPI_MASTER_SUCCESS;
}


static const ioProcessFunction ioProcessors[] =
{
    ioProcessBulkRead,
    ioProcessBulkWrite,
    ioProcessRegRead,
    ioProcessRegWrite
};

static int32 ioProcessor(uint32 *arg)
{
    int32 result = CSR_SPI_MASTER_SUCCESS;

    while ((result == CSR_SPI_MASTER_SUCCESS) && (opIdx < opCount))
    {
        ++opIdx;
        result = ioProcessors[opList[opIdx - 1].operation](&opList[opIdx - 1]);
    }

    return result;
}

CsrResult CsrSpiMasterIo(CsrSpiMasterOperation *operations, CsrUint32 operationCount)
{
    uint32 actualFlags;

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_IO);

    if (!operationCount)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO);
        return CSR_RESULT_SUCCESS;
    }

#ifdef CSP_SPI_SHARE_WITH_OTHER_DEVICE
	WIFI_Ctl_Signal(TRUE);
#endif

#if 1
    opList = operations;
    opCount = operationCount;
    opIdx = 0;

    if (CSR_SPI_MASTER_PENDING == ioProcessor(NULL))
    {
        SCI_GetEvent(eventHandle, 1, SCI_OR_CLEAR, &actualFlags, SCI_WAIT_FOREVER);
    }
#else
    {
        CsrUint32 i;
        for (i = 0; i < operationCount; i++)
        {
            opList = &operations[i];
            opCount = 1;
            opIdx = 0;
            if (CSR_SPI_MASTER_PENDING == ioProcessor(NULL))
            {
                SCI_GetEvent(eventHandle, 1, SCI_OR_CLEAR, &actualFlags, SCI_WAIT_FOREVER);
            }
        }
    }
#endif

#ifdef CSP_SPI_SHARE_WITH_OTHER_DEVICE
    WIFI_Ctl_Signal(FALSE);
#endif

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IO);
    return CSR_RESULT_SUCCESS;
}

static void ioInit(void)
{
    CsrMemSet(cmdBuffer,0xFF,32);
}

void CsrSpiMasterBusClockFrequencySet(CsrSpiMasterDevice *device, CsrUint32 *frequency)
{
    CsrUint32 hz = (*frequency > MAX_FREQUENCY)? MAX_FREQUENCY : *frequency;    
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_CLOCK_FREQUENCY_SET);

    //CSR_SDIO_PRINTF:"CsrSpiMasterBusClockFrequencySet: %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CSR_SPI_MASTER_760_112_2_18_0_35_16_2146,(uint8*)"d", *frequency);
    WIFI_Set_Clock_Frequency(hz);
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_CLOCK_FREQUENCY_SET);
}

void CsrSpiMasterCallbackRegister(CsrSpiMasterDevice *device, CsrSpiMasterCallback interruptCallback, CsrSpiMasterDsrCallback interruptDsrCallback, CsrSpiMasterDsrCallback operationDsrCallback)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_CALLBACK_REGISTER);
    
    CSR_SDIO_PRINTF(("CsrSpiMasterCallbackRegister"));
    verifyDevice(device);

    // CSR: Never enable IRQ here. Just enable it on the following CsrSpiMasterInterruptAcknowledge(). Only disable when unregistering
    if (!interruptCallback)
    {
        GPIO_DisableIntCtl(gpio_int_id);
    }
    function2CallInISR = interruptCallback;
    eventInterruptDsrCallback[0] = interruptDsrCallback;

    CSR_SDIO_PRINTF(("CsrSpiMasterCallbackRegister End"));

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_CALLBACK_REGISTER);
}

void CsrSpiMasterInterruptAcknowledge(CsrSpiMasterDevice *device)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_SPI_IRQ_ACKNOWLEDGE);
    /* Unmask */
    //SCI_TRACE_LOW("CsrSpiMasterInterruptAcknowledge");

    // CSR: Unmask the IRQ here.
    GPIO_EnableIntCtl(gpio_int_id);

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_SPI_IRQ_ACKNOWLEDGE);
}

