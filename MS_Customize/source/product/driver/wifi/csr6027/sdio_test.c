/****************************************************************************

                (c) CSR plc 2009

                All rights reserved

FILE:           csr_sdio_test.c

DESCRIPTION:    Simple program to exercise UF6026/UF6028 through CSR SDIO API

REVISION:       $Revision: #1 $

****************************************************************************/

#include <stdio.h>
#include "csr_types.h"
#include "csr_sdio.h"
#include "csr_pmem.h"
#include "csr_framework_ext.h"
#include "platform/csr_framework_ext_init.h"
#include "os_api.h"

/* CSR6026 */
#define CSR6026_BUFFER_SIZE               2048

/* Utility Functions */
#define POPULATE_BUFFER(a_, b_, c_)     for ((a_) = 0; (a_) < (b_); (a_)++) (c_)[a_] = (CsrUint8) (a_)
#define CoalAssert(a,b,c,d)            SCI_TraceLow("%s", d) //SCI_Assert(a, b, c)
#define BchsTextLog                     printf

static CsrSdioFunctionId ids[] =
{
    {0x032A, 0x0007, 1, CSR_SDIO_ANY_SDIO_INTERFACE}
};
#define CSR_SDIO_TEST_NUMBER_OF_IDS (sizeof(ids)/sizeof(CsrSdioFunctionId))
#define CSR_SDIO_TEST_FUNCTIONS 1

static CsrSdioFunction      *sdioFunctions[CSR_SDIO_TEST_FUNCTIONS];
static CsrUint8             sdioFunctionCount;
static volatile CsrBool     sdioTestRun;

static CsrUint32 totalTestCount = 0;

typedef struct
{
    CsrEventHandle          interruptEvent;
    CsrUint32               interruptAcknowledged;
} CsrSdioTestInstanceData;

#define CSR_SDIO_TEST_FUNCTION_DRIVER_DATA(function)    ((CsrSdioTestInstanceData *)function->driverData)
#define CSR_SDIO_TEST_DATA_INTERRUPT_EVENT(function)    (CSR_SDIO_TEST_FUNCTION_DRIVER_DATA(function)->interruptEvent)
#define CSR_SDIO_TEST_DATA_INTERRUPT_ACK(function)      (CSR_SDIO_TEST_FUNCTION_DRIVER_DATA(function)->interruptAcknowledged)

static void BchsArrayLog(char *txt, CsrUint8 *dat, CsrUint32 length)
{
    CsrUint32 i;
    SCI_TraceLow("%s",txt);
    for (i = 0; i < length; i++)
    {
        SCI_TraceLow("%02X:", dat[i]);
    }
    SCI_TraceLow("\n");
}

static void inserted(CsrSdioFunction *function)
{
    function->driverData = CsrMemSet(CsrPmemAlloc(sizeof(CsrSdioTestInstanceData)), 0x00, sizeof(CsrSdioTestInstanceData));
    if (sdioFunctionCount < CSR_SDIO_TEST_FUNCTIONS)
    {
        sdioFunctions[sdioFunctionCount++] = function;
    }
    
    CSR_SDIO_TEST_DATA_INTERRUPT_ACK(function) = TRUE;
    CsrEventCreate(&CSR_SDIO_TEST_DATA_INTERRUPT_EVENT(function));
    
    CsrSdioInsertedAcknowledge(function, CSR_RESULT_SUCCESS);
}

static void removed(CsrSdioFunction *function)
{
    CsrUint8 i;
    for (i = 0; i < CSR_SDIO_TEST_FUNCTIONS; i++)
    {
        if (sdioFunctions[i] == function)
        {
            sdioFunctions[i] = NULL;
            break;
        }
    }
    CsrEventDestroy(&CSR_SDIO_TEST_DATA_INTERRUPT_EVENT(function));
    CsrPmemFree(function->driverData);
    function->driverData = NULL;
    CsrSdioRemovedAcknowledge(function);
}

static CsrSdioInterruptDsrCallback interruptISR(CsrSdioFunction *function)
{
    CoalAssert(CSR_SDIO_TEST_DATA_INTERRUPT_ACK(function), __FILE__, __LINE__, "Interrupt received before acknowledge\n");
    CSR_SDIO_TEST_DATA_INTERRUPT_ACK(function) = FALSE;
    CsrEventSet(&CSR_SDIO_TEST_DATA_INTERRUPT_EVENT(function), 1);
    return NULL;
}

static CsrSdioFunctionDriver fd =
{
    inserted,
    removed,
    interruptISR,
    NULL,
    NULL,
    ids,
    CSR_SDIO_TEST_NUMBER_OF_IDS
};

static void registerDevice(CsrSdioTestInstanceData *instance)
{
    CsrUint8 i;
    CsrResult result;
    result = CsrSdioFunctionDriverRegister(&fd);

    SCI_TraceLow("Waiting for SDIO functions to register: %d\n", result);
    while (sdioFunctionCount < CSR_SDIO_TEST_FUNCTIONS)
    {
        ;
    }

    for (i = 0; i < sdioFunctionCount; i++)
    {
        SCI_TraceLow("Device information:\n");
        SCI_TraceLow("Manufacturer: 0x%04X\n", sdioFunctions[i]->sdioId.manfId);
        SCI_TraceLow("Card: 0x%04X\n", sdioFunctions[i]->sdioId.cardId);
        SCI_TraceLow("Function: %d\n", sdioFunctions[i]->sdioId.sdioFunction);
        SCI_TraceLow("Standard Interface: %d\n", sdioFunctions[i]->sdioId.sdioInterface);
        SCI_TraceLow("Blocksize: %d\n", sdioFunctions[i]->blockSize);
        SCI_TraceLow("Features:0x%08X\n", sdioFunctions[i]->features);
    }
}

static void functionEnable(CsrSdioFunction *function)
{
    CsrInt32 result;
    CsrUint8 data;

    SCI_TraceLow("Testing enable function %d\n", function->sdioId.sdioFunction);
    result = CsrSdioFunctionEnable(function);
    CoalAssert(result == CSR_RESULT_SUCCESS, __FILE__, __LINE__, "Unable to enable function\n");

    result = CsrSdioF0Read8(function, 0x02, &data);
    CoalAssert(result == CSR_RESULT_SUCCESS, __FILE__, __LINE__, "Unable to read IOE\n");
    CoalAssert(data & (1 << function->sdioId.sdioFunction), __FILE__, __LINE__, "IOE register is wrong\n");

    result = CsrSdioF0Read8(function, 0x03, &data);
    CoalAssert(result == CSR_RESULT_SUCCESS, __FILE__, __LINE__, "Unable to read IOR\n");
    CoalAssert(data & (1 << function->sdioId.sdioFunction), __FILE__, __LINE__, "IOR register is wrong\n");

    SCI_TraceLow("Enable function is verified\n");
}

static void functionDisable(CsrSdioFunction *function)
{
    CsrInt32 result;
    CsrUint8 data;

    SCI_TraceLow("Testing disable function %d\n", function->sdioId.sdioFunction);
    result = CsrSdioFunctionDisable(function);
    CoalAssert(result == CSR_RESULT_SUCCESS, __FILE__, __LINE__, "Unable to disable function\n");

    result = CsrSdioF0Read8(function, 0x02, &data);
    CoalAssert(result == CSR_RESULT_SUCCESS, __FILE__, __LINE__, "Unable to read IOE\n");
    CoalAssert(data & (1 << function->sdioId.sdioFunction), __FILE__, __LINE__, "IOE register is wrong\n");

    result = CsrSdioF0Read8(function, 0x03, &data);
    CoalAssert(result == CSR_RESULT_SUCCESS, __FILE__, __LINE__, "Unable to read IOR\n");
    CoalAssert(data & (1 << function->sdioId.sdioFunction), __FILE__, __LINE__, "IOR register is wrong\n");

    SCI_TraceLow("Disable function is verified\n");
}

static void frequencySet(CsrSdioFunction *function, CsrUint32 frequency)
{
    CsrInt32 result;

    SCI_TraceLow("Testing set frequency, function %d, frequency %d\n", function->sdioId.sdioFunction, frequency);
    result = CsrSdioMaxBusClockFrequencySet(function, frequency);
    CoalAssert(result == CSR_RESULT_SUCCESS, __FILE__, __LINE__, "CsrSdioMaxBusClockFrequency returned error\n");
}

static CsrBool validateBuffer(CsrUint8 *buffer, CsrUint8 *reference, CsrUint32 bufferLength)
{
    CsrUint32 i;
    for (i = 0; i < bufferLength; i++)
    {
        if (buffer[i] != reference[i])
        {
            SCI_TraceLow("validateBuffer failed at byte %d\n", i);
            return FALSE;
        }
    }
    return TRUE;
}

static void testReadWrite(CsrSdioFunction *function, CsrUint32 testCount, CsrUint32 testLength)
{
    #define RW_REFCOUNT 3
    CsrUint8 *refs[RW_REFCOUNT];
    CsrUint8 *buf = CsrPmemAlloc(CSR6026_BUFFER_SIZE);
    CsrUint32 i, j, k;

    for (i = 0; i < RW_REFCOUNT; i++)
    {
        refs[i] = CsrPmemAlloc(CSR6026_BUFFER_SIZE);
    }
    
    /* populate data buffer */
    POPULATE_BUFFER(i, CSR6026_BUFFER_SIZE, refs[0]);
    CsrMemSet(refs[1], 0x55, CSR6026_BUFFER_SIZE);
    CsrMemSet(refs[2], 0xAA, CSR6026_BUFFER_SIZE);

    SCI_TraceLow("Testing CsrSdioRead/Write length %d, totalTestCount: %d\n", testLength, totalTestCount++);
    for (i = 0; i < testCount; i++)
    {
        for (j = 0; j < RW_REFCOUNT; j++)
        {
            for (k = 0; k < CSR6026_BUFFER_SIZE; k += testLength)
            {
                CsrSdioWrite(function, 1, &refs[j][k], testLength);
            }
            if (CSR6026_BUFFER_SIZE % testLength)
            {
                SCI_TraceLow("Writing remainder, which is %d bytes\n", CSR6026_BUFFER_SIZE % testLength);
                CsrSdioWrite(function, 1, &refs[j][CSR6026_BUFFER_SIZE - CSR6026_BUFFER_SIZE % testLength], CSR6026_BUFFER_SIZE % testLength);
            }
            for (k = 0; k < CSR6026_BUFFER_SIZE; k += testLength)
            {
                CsrSdioRead(function, 1, &buf[k], testLength);
            }
            if (CSR6026_BUFFER_SIZE % testLength)
            {
                SCI_TraceLow("Reading remainder, which is %d bytes\n", CSR6026_BUFFER_SIZE % testLength);
                CsrSdioRead(function, 1, &buf[CSR6026_BUFFER_SIZE - CSR6026_BUFFER_SIZE % testLength], CSR6026_BUFFER_SIZE % testLength);
            }
            if (!validateBuffer(buf, refs[j], CSR6026_BUFFER_SIZE))
            {
                BchsArrayLog("Reference\n", refs[j], CSR6026_BUFFER_SIZE);
                BchsArrayLog("Buffer\n", buf, CSR6026_BUFFER_SIZE);
                CoalAssert(0, __FILE__, __LINE__, "Data verification failed in testReadWrite\n");
            }
        }
    }

    for (j = 0; j < RW_REFCOUNT; j++)
    {
        CsrPmemFree(refs[j]);
    }
    CsrPmemFree(buf);
}

static void testRun(void *inst)
{
    CsrUint32 i;

    registerDevice((CsrSdioTestInstanceData *)inst);

    frequencySet(sdioFunctions[0], 25000000);
    functionEnable(sdioFunctions[0]);

    for (i = 0; i < 100; i++)
    {
        testReadWrite(sdioFunctions[0], 100, CSR6026_BUFFER_SIZE);
        CsrThreadSleep(100);
    }

    functionDisable(sdioFunctions[0]);
    CsrSdioFunctionDriverUnregister(&fd);
}

void CsrSdioTestStart(void)
{
    CsrThreadHandle handle;
    CsrGlobalMutexCreate();
    CsrThreadCreate(testRun, CsrMemSet(CsrPmemAlloc(sizeof(CsrSdioTestInstanceData)), 0x00, sizeof(CsrSdioTestInstanceData)), 1024, 2, "CsrSdioTest thread\n", &handle);
}
