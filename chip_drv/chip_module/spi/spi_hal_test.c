/******************************************************************************
 ** File Name:      SPI_hal_test.c                                                *
 ** Author:         liuhao                                                   *
 ** DATE:           06/07/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the hal layer of SPI device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/07/2011     wenjun.shi           Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "spi_drvapi.h"
//#include "pwm_drvapi.h"
//#include "deepsleep_drvapi.h"
#include "spi_phy.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
typedef struct
{
    float r_sync_rate;
    float w_sync_rate;
    float r_async_rate;
    float w_async_rate;
}SPI_THPUT_T;

#define LEN_MAX 1024 //byte
#define ITERATION_MAX 100
#define BIT_OF_BYTE 8

// Spi Throughput Test Begin
uint32 fifo_type[] = {/*1, */4, 0xffffffff};
uint32 bitlen_type[] = {32, 16, 8, 0xffffffff};
uint32 clk_type[] = {12000000, 24000000, /*48000000,*/ 0xffffffff};
/* To pass all test of 32/16/8 bitlen, total len must be multiple of 4(byte)*/
uint32 total_type[] = {1024, /*2048, 4096, 8192, 16384, 32768,*/  0xffffffff};
//uint32 total_type[] = {1024, 2137, 3593, 5621, 17715, 32768,  0xffffffff};
uint32 *g_spi_read_rate_ptr;
uint32 *g_spi_write_rate_ptr;


//float *g_spi_r_rate_ptr;
//float *g_spi_w_rate_ptr;
volatile uint32 g_spi_tx_event;
volatile uint32 g_spi_rx_event;
LOCAL SCI_EVENT_GROUP_PTR  spi_tx_event = PNULL;
LOCAL SCI_EVENT_GROUP_PTR  spi_rx_event = PNULL;


uint32 _sum(uint32 *array_ptr)
{
    uint32 i;
    uint32 sum = 0;
    for (i = 0; array_ptr[i] != 0xffffffff; i++)
    {
        sum += array_ptr[i];
    }
    return sum;
}
int32 spi_tx_int_cb(uint32 *arg)
{
    //g_spi_tx_event = 1;
    SCI_SetEvent(spi_tx_event, 1, SCI_OR);
    return 0;
}

int32 spi_rx_int_cb(uint32 *arg)
{
    //g_spi_rx_event = 1;
    SCI_SetEvent(spi_rx_event, 1, SCI_OR);
    return 0;
}
/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
uint32 SPI_HAL_Write_Async_DUMMY(uint32 logic_id, uint8 *buffer_ptr, uint32 len, _spi_callback tx_cb)
{
    uint32 timeout;
    uint32 status;
    uint32 actual_event = 0;

    g_spi_tx_event = 0;
    SPI_HAL_Write_Async (logic_id, buffer_ptr, len,  tx_cb);
    timeout = 0;
    status = SCI_GetEvent(spi_tx_event, 
             1, 
             SCI_AND_CLEAR, 
             &actual_event, 
             SCI_WAIT_FOREVER);
    SCI_ASSERT(!status);

#if 0
    while(g_spi_tx_event == 0)
    {
        timeout++;
        if (timeout > SPI_DMA_TIME_OUT << 6)
        {
            SCI_ASSERT(0);
        }
    }    
    g_spi_tx_event = 0;
#endif /* 0 */
    return 0;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
uint32 SPI_HAL_Read_Async_DUMMY(uint32 logic_id, uint8 *buffer_ptr, uint32 len, _spi_callback rx_cb)
{
    uint32 timeout;
    uint32 status;
    uint32 actual_event = 0;
    g_spi_rx_event = 0;
    SPI_HAL_Read_Async (logic_id, buffer_ptr, len,  rx_cb);
    timeout = 0;
    status = SCI_GetEvent(spi_rx_event, 
             1, 
             SCI_AND_CLEAR, 
             &actual_event, 
             SCI_WAIT_FOREVER);
    SCI_ASSERT(!status);
#if 0
    while(g_spi_rx_event == 0)
    {
        timeout++;
        if (timeout > SPI_DMA_TIME_OUT << 6)
        {
            SCI_ASSERT(0);
        }
    }    
    g_spi_rx_event = 0;
#endif /* 0 */
    return 0;
}

/*********************************************************************************************************
** Function name:
** Descriptions: For each 'bitlen, clk' pair, spi read/write operation will be tested with several different
**              length of data. Then will calculate an average read/write rate.
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
uint32 Spi_Thput_Test()
{
    uint32 fifo_i;
    uint32 bitlen_i;
    uint32 clk_i;
    uint32 total_i;
    uint32 pre_tick;
    uint32 cur_tick;
    uint32 case_size;
    uint32 result_size;
    uint32 i;
    //uint32 rec_i;
    uint32 total_bit;
    float r_sync_time;
    float w_sync_time;
    float r_async_time;
    float w_async_time;
    uint8 *buf_ptr;
    SPI_DEV spi_dev;
    SPI_THPUT_T spi_thput;
    
    
    buf_ptr = (uint8 *)SCI_ALLOC(LEN_MAX);
    case_size = (sizeof(uint32))*(sizeof(fifo_type)/(sizeof(uint32)) - 1)*(sizeof(bitlen_type)/(sizeof(uint32)) - 1)*(sizeof(clk_type)/(sizeof(uint32)) - 1)*(sizeof(total_type)/(sizeof(uint32)) - 1);
    result_size = (sizeof(uint32))*(sizeof(fifo_type)/(sizeof(uint32)) - 1)*(sizeof(bitlen_type)/(sizeof(uint32)) - 1)*(sizeof(clk_type)/(sizeof(uint32)) - 1);
    //g_spi_read_rate_ptr = (uint32 *)SCI_ALLOC(case_size); 
    //g_spi_write_rate_ptr = (uint32 *)SCI_ALLOC(case_size); 
    //g_spi_r_rate_ptr = (float *)SCI_ALLOC(result_size); 
    //g_spi_w_rate_ptr = (float *)SCI_ALLOC(result_size); 
    
    //SCI_TraceLow:"Spi_Thput_Test: case_size %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_HAL_TEST_206_112_2_17_23_6_59_1316,(uint8*)"d", case_size );
    
    /*initialize spi params*/
	spi_dev.id = 0; /* logic ID */
	spi_dev.mode = 0; /* No sck reverse(Bit13:0),  Enable NG_Tx(Bit1:1),Disable NG_Rx(Bit0:0) */
	spi_dev.tx_bit_length = 32;
    spi_dev.spi_cs_cb = PNULL;
    spi_dev.spi_rx_cb = spi_rx_int_cb;
    spi_dev.spi_tx_cb = spi_tx_int_cb;
	spi_dev.freq = 10000000;
    SPI_HAL_Open(&spi_dev);
    
    /*total number of bits that transfered*/
    total_bit = _sum(total_type)*ITERATION_MAX*BIT_OF_BYTE;
	//SCI_TraceLow("Spi_Thput_Test: sizeof %d, %d, %d, %d", sizeof(total_type), sizeof(clk_type), sizeof(bitlen_type), sizeof(fifo_type));
    for (fifo_i = 0; fifo_type[fifo_i] != 0xffffffff; fifo_i++)
    {
        for (bitlen_i = 0; bitlen_type[bitlen_i] != 0xffffffff; bitlen_i++)
        {
            /*set spi bit length*/
            SPI_HAL_Ioctl (spi_dev.id, SPI_CTL_S_BITLEN, &bitlen_type[bitlen_i]);
            for (clk_i = 0; clk_type[clk_i] != 0xffffffff; clk_i++)
            {
                w_sync_time = r_sync_time = w_async_time = r_async_time = 0;
                /*set spi clk*/
                SPI_HAL_Ioctl (spi_dev.id, SPI_CTL_S_FREQ, &clk_type[clk_i]);
                for (total_i = 0; total_type[total_i] != 0xffffffff; total_i++)
                {
                    #if 0
                    /*locate iteration index*/
                    rec_i = fifo_i*(sizeof(bitlen_type)/(sizeof(uint32)) - 1)*(sizeof(clk_type)/(sizeof(uint32)) - 1)*(sizeof(total_type)/(sizeof(uint32)) - 1)
                            + bitlen_i*(sizeof(clk_type)/(sizeof(uint32)) - 1)*(sizeof(total_type)/(sizeof(uint32)) - 1)
                            + clk_i*(sizeof(total_type)/(sizeof(uint32)) - 1)
                            + total_i;
                    //SCI_TraceLow("Spi_Thput_Test: rec_i %d", rec_i );
                    #endif 
                    pre_tick = SCI_GetTickCount();
                    /*repeat read/write operation many times to get effective value, 
                    *  or value is too small to take into account
                    */
                    for ( i = 0; i < ITERATION_MAX; i++)
                    {
                        /*spi sync mode(polling)*/
                        SPI_HAL_Write(spi_dev.id, buf_ptr, total_type[total_i]);
                    }
                    cur_tick = SCI_GetTickCount();
                    w_sync_time += (float) (cur_tick - pre_tick);
                    
                    pre_tick = SCI_GetTickCount();
                    for ( i = 0; i < ITERATION_MAX; i++)
                    {
                        /*spi async mode(interrupt)*/
                        SPI_HAL_Write_Async_DUMMY(spi_dev.id, buf_ptr, total_type[total_i], spi_tx_int_cb);
                    }
                    cur_tick = SCI_GetTickCount();
                    w_async_time += (float) (cur_tick - pre_tick);

                    #if 0
                    {
                        //SCI_TraceLow:"Spi_Thput_Test: write: pre_tick %d, cur_tick=%d, time:%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_HAL_TEST_265_112_2_17_23_6_59_1317,(uint8*)"ddd", pre_tick, cur_tick,  cur_tick-pre_tick);
                        g_spi_write_rate_ptr[rec_i] = /*total_type[total_i] /*/ (cur_tick - pre_tick) ;
                    }
                    #endif 
                    pre_tick = SCI_GetTickCount();
                    for ( i = 0; i < ITERATION_MAX; i++)
                    {
                        SPI_HAL_Read(spi_dev.id, buf_ptr, total_type[total_i]);
                        //SPI_HAL_Read_Async_DUMMY(spi_dev.id, buf_ptr, total_type[total_i], spi_rx_int_cb);
                    }
                    cur_tick = SCI_GetTickCount();
                    r_sync_time += (float) (cur_tick - pre_tick);
                    
                    pre_tick = SCI_GetTickCount();
                    for ( i = 0; i < ITERATION_MAX; i++)
                    {
                        SPI_HAL_Read_Async_DUMMY(spi_dev.id, buf_ptr, total_type[total_i], spi_rx_int_cb);
                    }
                    cur_tick = SCI_GetTickCount();
                    r_async_time += (float) (cur_tick - pre_tick);
                    #if 0
                    {
                        //SCI_TraceLow:"Spi_Thput_Test: read: pre_tick %d, cur_tick=%d, time:%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_HAL_TEST_287_112_2_17_23_6_59_1318,(uint8*)"ddd", pre_tick, cur_tick, cur_tick-pre_tick);
                        g_spi_read_rate_ptr[rec_i] = /*total_type[total_i] /*/ (cur_tick - pre_tick);
                    }
                    #endif 
                    //w_sync_time += (float)g_spi_write_rate_ptr[rec_i];
                    //r_sync_time += (float)g_spi_read_rate_ptr[rec_i];
                }//total_i
                //SCI_TraceLow("Spi_Thput_Test: sum:%d, w_sync_time=%f, r_sync_time=%f",  (_sum(total_type)), w_sync_time, r_sync_time);
                /*calculate spi throughput*/
                SCI_ASSERT(r_async_time*w_async_time*r_sync_time*w_sync_time != 0);
                spi_thput.r_async_rate = total_bit/r_async_time;
                spi_thput.w_async_rate = total_bit/w_async_time;
                spi_thput.r_sync_rate = total_bit/r_sync_time;
                spi_thput.w_sync_rate = total_bit/w_sync_time;
                SCI_TraceLow(\


                            "Spi_Thput_Test: bitlen:%d, clk:%d, write sync rate:%f, read sync rate:%f, write async rate:%f, read async rate:%f", \
                            bitlen_type[bitlen_i], \
                            clk_type[clk_i], \
                            spi_thput.w_sync_rate, \
                            spi_thput.r_sync_rate, \
                            spi_thput.w_async_rate, \
                            spi_thput.r_async_rate
                            );
            }//clk_i
        }//bitlen_i
    }//fifo_i
    SPI_HAL_Close(spi_dev.id);
    SCI_FREE(buf_ptr);
    return 0;
}


LOCAL void SPI_HAL_TestThread(uint32 argc, void* argv)
{
    if(PNULL == spi_tx_event)
    {
        spi_tx_event = SCI_CreateEvent("SPI_Tx_Event");
    }
    if(PNULL == spi_rx_event)
    {
        spi_rx_event = SCI_CreateEvent("SPI_Rx_Event");
    }
    while(1)
    {
      SCI_Sleep(15000);
      Spi_Thput_Test();
    }
}

#define SPI_TEST_STACK_SIZE       (1024*2)
#define SPI_TEST_QUEUE_NUM        16
#define TASK_SPI_TEST_SRV_PRI 20
LOCAL BLOCK_ID spi_test_task_id;

PUBLIC uint32 SPI_TEST_INIT()
{
    spi_test_task_id  = SCI_CreateThread("SPI_TEST_SRV",
									"Q_SPI_TEST_SRV",
									SPI_HAL_TestThread,
									0,
									0,
									SPI_TEST_STACK_SIZE ,
									SPI_TEST_QUEUE_NUM,
									TASK_SPI_TEST_SRV_PRI ,
									SCI_PREEMPT,
									SCI_AUTO_START);
    if (SCI_INVALID_BLOCK_ID == spi_test_task_id)
    {
        return FALSE;
    }
    return TRUE;
}


// Spi Throughput Test End


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/*  End Of File */
