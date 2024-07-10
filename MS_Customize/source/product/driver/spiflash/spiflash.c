/******************************************************************************
 ** File Name:      spiflash.c                                             *
 ** Author:         Fei.Zhang                                              *
 ** DATE:           01/15/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 SPI flash Low-level drvier.                             *
 **                 This driver support ALL SPI flash                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01-15-2010     Fei.Zhang          Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "arm_reg.h"
#include "tb_hal.h"
#include "sc_reg.h"
#include "sfc_drvapi.h"
#include "spiflash.h"
#include "flash_drvapi.h"
#include "spi_flash.h"
#include "spi_flash_internal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif


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

/**---------------------------------------------------------------------------*
 **              variable declare                                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variable                                   *
 **---------------------------------------------------------------------------*/
#pragma arm section code = "SPIFLASH_CODE"
PUBLIC struct spi_flash* myflash[2]={0};
/*lint -esym(551, pExtcfg, spiflash_error_type, cmd_4pp, cmd_4read, s_flash_flash_size, s_flash_erasing_addr, s_flash_init_flag, flash_cs1_desity, QPI_support, ExitQPI, EnterQPI, reset_support) */
LOCAL int s_erase_cs = SPIFLASH_CS0;
LOCAL uint32 flash_cs0_desity = 0;
LOCAL uint32 flash_cs1_desity = 0;

#ifdef SFC_DEBUG
typedef struct trace_point {
    uint8 id;
    uint32  msg;
} TRACE_POINT_T;

#define TRACE_POINT_DEPTH  50
LOCAL uint8         s_trace_point_cnt=0;
LOCAL TRACE_POINT_T s_trace_point_buf[TRACE_POINT_DEPTH] = {0};
#endif

void _AddPoint(uint8 id, uint32 msg)
{
#ifdef SFC_DEBUG
    uint32 i=0;
    for(i=0; i<TRACE_POINT_DEPTH-1; i++) {
        s_trace_point_buf[i] = s_trace_point_buf[i+1];
    }

    /* Save the new point*/
    s_trace_point_buf[i].id = id;
    s_trace_point_buf[i].msg= msg;
#endif
}

#if defined(CHIP_VER_UWS6121E)
#define FLASH2_START_ADRESS            0x70000000     //phy_addr from chip
#define SPIFLASH_CS0_BASE_ADDRESS	    (0x60000000)
#define SPIFLASH_CS1_BASE_ADDRESS	    (FLASH2_START_ADRESS)
#else
#define SPIFLASH_CS1_BASE_ADDRESS    flash_cs0_desity
#endif

#define CMD_READ            0x03
#define CMD_DP              0xB9
#define CMD_RDP             0xAB

#define CMD_RDUNID          0x4B
#define CMD_ENTERSECURITY   0xB1
#define CMD_EXITSECURITY    0xC1

LOCAL uint8 s_unid[9] = {0};

#define SFC_CLK_DIV_0  0
#define SFC_CLK_DIV_2  2

#define SFC_CLK_SAMPLE_DELAY_SEL  4
#define SFC_CLK_2X_EN             BIT_10
#define SFC_CLK_OUT_2X_EN         BIT_9
#define SFC_CLK_SAMPLE_2X_PHASE   BIT_8
#define SFC_CLK_SAMPLE_2X_EN      BIT_7

LOCAL uint8 s_nMID[SFC_MAX_CS] = {0};           /* nMID*/
LOCAL uint8 s_nDID1[SFC_MAX_CS] = {0};              /* nDID1*/
LOCAL uint8 s_nDID2[SFC_MAX_CS] = {0};              /* nDID1*/
LOCAL uint8 nQEPos[SFC_MAX_CS] = {0};           /* QE bit Poisition, 0xFF indicates no QE bit*/
LOCAL uint8 nWIPPos[SFC_MAX_CS] = {0};        /* WIP bit Poisition, 0xFF indicates no WIP bit*/
LOCAL uint8 EnterQPI[SFC_MAX_CS] = {0};     /* Command to enter QPI mode*/
LOCAL uint8 ExitQPI[SFC_MAX_CS] = {0};          /* Command to exit QPI mode,  0x00 indicates not support*/
LOCAL uint8 cmd_4pp[SFC_MAX_CS] = {0};      /* Command to write in 4 I/O mode */
LOCAL uint8 cmd_4read[SFC_MAX_CS] = {0};      /* Command to read in 4 I/O mode */
LOCAL BOOLEAN reset_support[SFC_MAX_CS] = {0};  /* sw reset support*/
LOCAL BOOLEAN QPI_support[SFC_MAX_CS] = {0};    /* QPI mode support*/
LOCAL uint8 CMD_PES[SFC_MAX_CS] = {0};      /* Program/Erase Suspend Command */
LOCAL uint8 CMD_PER[SFC_MAX_CS] = {0};      /* Program/Erase Resume Command */
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#if defined(CHIP_VER_UWS6121E)

#if defined(FLASH_SIZE_128MBITX64MBIT_NEW) || defined(FLASH_SIZE_128MBITX64MBIT_OLD) || defined(FLASH_SIZE_128MBITX32MBIT)
LOCAL BYTE_NUM_E   s_address_cycle[SFC_MAX_CS] = {BYTE_NUM_3, BYTE_NUM_3}; /* address cycle info */
LOCAL uint32 s_address_mask[SFC_MAX_CS] = {SPIFLASH_3_ADDR_CYCLE_MASK, SPIFLASH_3_ADDR_CYCLE_MASK}; /* address cycle info */
LOCAL EXT_CFG_PTR    pExtcfg[SFC_MAX_CS] = {0, 0}; /* extension cfg info */
LOCAL uint8 work_mode[SFC_MAX_CS] = {SPI_MODE, SPI_MODE};	/* work mode: SPI mode or QPI mode*/
#elif FLASH_SIZE_128MBIT
LOCAL BYTE_NUM_E   s_address_cycle[SFC_MAX_CS] = {BYTE_NUM_3}; /* address cycle info */
LOCAL uint32 s_address_mask[SFC_MAX_CS] = {SPIFLASH_3_ADDR_CYCLE_MASK}; /* address cycle info */
LOCAL EXT_CFG_PTR    pExtcfg[SFC_MAX_CS] = {0}; /* extension cfg info */
LOCAL uint8 work_mode[SFC_MAX_CS] = {SPI_MODE};   /* work mode: SPI mode or QPI mode*/
#endif

#else
LOCAL BYTE_NUM_E   s_address_cycle[SFC_MAX_CS] = {BYTE_NUM_3}; /* address cycle info */
LOCAL uint32 s_address_mask[SFC_MAX_CS] = {SPIFLASH_3_ADDR_CYCLE_MASK}; /* address cycle info */
LOCAL EXT_CFG_PTR    pExtcfg[SFC_MAX_CS] = {0}; /* extension cfg info */

LOCAL uint8 work_mode[SFC_MAX_CS] = {SPI_MODE};   /* work mode: SPI mode or QPI mode*/
#endif
#else
LOCAL BYTE_NUM_E   s_address_cycle[SFC_MAX_CS] = {BYTE_NUM_3, BYTE_NUM_3}; /* address cycle info */
LOCAL uint32 s_address_mask[SFC_MAX_CS] = {SPIFLASH_3_ADDR_CYCLE_MASK, SPIFLASH_3_ADDR_CYCLE_MASK}; /* address cycle info */
LOCAL EXT_CFG_PTR    pExtcfg[SFC_MAX_CS] = {0}; /* extension cfg info */

LOCAL uint8 work_mode[SFC_MAX_CS] = {SPI_MODE, SPI_MODE};   /* work mode: SPI mode or QPI mode*/
#endif
#define BLOCK_SIZE     s_flash_sector_size
#define BLOCK_MASK    (BLOCK_SIZE - 1)


//Flash type define
#define SPIFLASH_GD_ID         0xC8
#define SPIFLASH_EON_ID         0x1C
#define SPIFLASH_MICRON_ID      0x2C
#define SPIFLASH_PCT_ID         0xBF
#define SPIFLASH_MXIC_ID        0xC2
#define SPIFLASH_WINBOND_ID     0xEF
#define SPIFLASH_FIDELIX_ID     0xF8
#define SPIFLASH_ESMT_ID        0x8C
#define SPIFLASH_BG_ID        0xE0

#define UNSUPPORT_CMD  0xFF


#define SR_CMP      BIT_14
#define SR_SPR1     BIT_8
#define SR_SPR0     BIT_7
#define SR_BP4      BIT_6
#define SR_BP3      BIT_5
#define SR_BP2      BIT_4
#define SR_BP1      BIT_3
#define SR_BP0      BIT_2
#define SR_WEL      BIT_1

#define SR_TB       BIT_5
#define SR_SEC      BIT_6
#define WB_256MB_SR_TB      BIT_6
#define SR_BP3_BIT6      BIT_6
#define SR_SRWD     BIT_7

LOCAL uint32 dummyclk_freq[SFC_MAX_CS] = {0};    /* work mode: SPI mode or QPI mode*/

LOCAL uint32 s_flash_init_flag = 0;
LOCAL uint32 s_flash_flash_size;
LOCAL uint32 s_flash_sector_size;

LOCAL FLASH_PTR_TYPE s_flash_erasing_addr;


LOCAL uint32 flash_store_optr_array[2];

LOCAL uint32 * flash_store_optr_ptr;

// store the current interrupt flag
LOCAL uint32 s_flash_interrupt_save;

//LOCAL uint32 s_flash_timing_save;

#define STORE_SFC_OPTR(optr)    STORE_FLASH_OPTR(optr , flash_store_optr_ptr)
#define GET_SFC_STORE_OPTR     GET_FLASH_STORE_OPTR(flash_store_optr_ptr)

// NON-zero indicate need monitor interrupt flag and handle it while erasing/programing.
// Zero     indicate needn't monitor the interrupt flag
LOCAL BOOLEAN      b_need_monitor_int = 0;

LOCAL uint32 SPIFLASH_GetPollStatus(struct spi_flash *flash);
LOCAL void SPIFLASH_WriteEnable(int cs);
LOCAL BOOLEAN SPIFLASH_PESuspend(int cs);
LOCAL BOOLEAN SPIFLASH_PEResume(int cs);

LOCAL uint32 _get_current_system_clock(void);

#define BLOCK_SIZE_64K      0x0010000UL
#define BLOCK_SIZE_32K      0x0008000UL
#define BLOCK_SIZE_4K       0x0001000UL

typedef enum{
    SPIFLASH_NOERROR = 0,
    SPIFLASH_WIP_WAIT_TIMEOUT,
    SPIFLASH_WEL_WAIT_TIMEOUT,
    SPIFLASH_UNRESPONSE,
    SPIFLASH_CANNOT_WRITE,
    SPIFLASH_WRITE_FAIL,
    SPIFLASH_ERASE_FAIL,
    SFC_WAIT_INT_TIMEOUT,
    SPIFLASH_MAX_ERROR_CNT
}SPIFLASH_ERROR_TYPE_E;

LOCAL SPIFLASH_ERROR_TYPE_E spiflash_error_type = SPIFLASH_NOERROR;


void  SPIFLASH_WEL_CHECKSTATUS(struct spi_flash *flash)
{
    uint32 cs = flash->spi->cs;
    do{
        uint32 tick = 0;
        uint32 sts0 = 0;
        tick = _get_current_system_clock();
        do{
            if((_get_current_system_clock() - tick) > SPI_FLASH_WEL_TIMEOUT)
                {
                    spiflash_error_type = SPIFLASH_WEL_WAIT_TIMEOUT;
                    SPIFLASH_ResetRegister(cs);
                    SCI_PASSERT(0,("spiflash write enable latch timeout!"));/*assert verified*/
                }
            SPIFLASH_WriteEnable(cs);
            sts0 = SPIFLASH_GetPollStatus(flash);
        }while(!(sts0 & BIT_1));
    }while(0);
}

void SPIFLASH_WIP_CHECKSTATUS(struct spi_flash *flash)
{
    uint32 cs = flash->spi->cs;
    do{
        uint32 tick = 0;
        uint32 sts0 = 0;
        tick = _get_current_system_clock();
        do{
            if((_get_current_system_clock() - tick) > SPI_FLASH_WIP_TIMEOUT)
                {
                    spiflash_error_type = SPIFLASH_WIP_WAIT_TIMEOUT;
                    SPIFLASH_ResetRegister(cs);
                    SCI_PASSERT(0,("spiflash write in process timeout!"));/*assert verified*/
                }
            sts0 = SPIFLASH_GetPollStatus(flash);
        }while(sts0 & (1<<nWIPPos[cs]));
    }while(0);
}

void WAIT_SFC_INT(uint32 cs)
{
#if !defined(PLATFORM_UWS6121E)
    do{
        uint32 tick = 0;
        tick = _get_current_system_clock();
        do{
            if((_get_current_system_clock() - tick) > SPI_FLASH_SFC_INT_TIMEOUT)
                {
                    spiflash_error_type = SFC_WAIT_INT_TIMEOUT;
                    SPIFLASH_ResetRegister(cs);
                    SCI_PASSERT(0,("spiflash controller wait interrupt timeout!"));/*assert verified*/
                }
        }while(!(SFC_INT_STATUS & SFCDRV_GetStatus()));
    }while(0);
#endif
}

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

PUBLIC void *_memcpy(void * dst, const void *src, uint32 count)
{
    char *tmp = (char *) dst, *s = (char *) src;

    while (count--)
        *tmp++ = *s++;

    return dst;
}

#define MAX_TIMER_CNT          0xFFFF
#define TIMER_LOAD_DELAY    50
//#define REG32(x)    (*(volatile uint32 *)(x))
#if defined(PLATFORM_UWS6121E)
#define TIMER_TICK_COUNT               64      // us
#else
#define TIMER1_TICK_COUNT               32      // us
#endif
#define TIMER1_INT_COUNT                0x7fffff

LOCAL uint32 __time1_init(void)
{
    uint32 cnt = 0;
    while(REG32(TM1_CLR) & BIT_4)  // if busy, then wait for a while
    {
        cnt++;
        if(cnt > MAX_TIMER_CNT)
        {
            break;  // timeout and break
        }
    }
    REG32(TM1_LOAD) = TIMER1_INT_COUNT;    // timer load

    cnt = 0;
    while(REG32(TM1_CLR) & BIT_4)  // if busy, then wait for a while
    {
        cnt++;
        if(cnt > MAX_TIMER_CNT)
        {
            break;  // timeout and break
        }
    }
    // Delay 50us for timer1 load value
    FLASH_DELAY_US(TIMER_LOAD_DELAY);

    REG32(TM1_CTL) |= BIT_7;          // timer start

    return SCI_TRUE;
}

LOCAL uint32 __time1_get_value(void)
{
    volatile uint32 tmp_tick1;
    volatile uint32 tmp_tick2;

    tmp_tick1 = REG32(TM1_VALUE);
    tmp_tick2 = REG32(TM1_VALUE);

    while (tmp_tick1 != tmp_tick2)
    {
        tmp_tick1 = tmp_tick2;
        tmp_tick2 = REG32(TM1_VALUE);
    }

    return tmp_tick1;
}

#define FLASH_MAX_PROGRAM_TIME   100    // 100 ms
#define FLASH_MAX_ERASE_TIME     20000   // 20000 ms
#define CHECKSTATUS

LOCAL uint32 _get_current_system_clock(void)
{
    volatile uint32 tmp_tick1;
    volatile uint32 tmp_tick2;

    tmp_tick1 = SYSTEM_CURRENT_CLOCK;
    tmp_tick2 = SYSTEM_CURRENT_CLOCK;

    while (tmp_tick1 != tmp_tick2)
    {
        tmp_tick1 = tmp_tick2;
        tmp_tick2 = SYSTEM_CURRENT_CLOCK;
    }

    return tmp_tick1;
}

LOCAL uint32 SPIFLASH_GetPollStatus(struct spi_flash *flash)
{
    int   cs = flash->spi->cs;
    uint8 cmd=0;
    uint8 status1 = 0, status2 = 0;
    uint32 status = 0;
    BIT_MODE_E bitmode = BIT_MODE_1;
    if(QPI_MODE == work_mode[cs])
    {
        bitmode = BIT_MODE_4;
    }

    cmd = CMD_READ_STATUS1;
    spiflash_cmd_read(flash, &cmd,1, &status1, 1);

    if(nQEPos[cs]>=8 && nQEPos[cs]!=UNSUPPORT_CMD)
    {
        cmd = CMD_READ_STATUS2;
        spiflash_cmd_read(flash, &cmd,1, &status2, 1);
    }

    status = (status1 | status2<<8);
    //SCI_TRACE_LOW("spiflash: status1=0x%x,status2=0x%x",status1,status2);

    return status;
}


#define DELAY_CHECK_INT_INTERVAL (100) /*us*/

LOCAL uint32 SPIFLASH_RS_Delay(uint32 delay_us)
{
   uint32 delay_count = 0;

   for(;;)
   {
         FLASH_DELAY_US(DELAY_CHECK_INT_INTERVAL);
/*lint -e628*/
#if !defined(PLATFORM_UWS6121E)
        if((GET_INTERRUPT_FLAG & (1<<TB_DSP_INT)) != 0)/*lint !e718 !e746 !e628 confirmed by windy.wang*/
        {
/*lint -e628*/
            break;
        }
#endif
        if((delay_count * DELAY_CHECK_INT_INTERVAL) > delay_us)
        {
            break;
        }
        delay_count++;
    }

    return SCI_TRUE;
}

/*****************************************************************************/
// Description : check the optr status.
//
// Return      : 0 : fail,  1 : success
// When call this function, the caller has call the DisableIRQ,
// So this function needn't call the DisableIRQ
//
// Notes:
//     Before call this function, make sure the irq has been disabled.
//     This function need be located in SRAM or other space.
//     This function mustn't call any function( or use any data ) that located in the flash area.
/*****************************************************************************/
#if defined(PLATFORM_UWS6121E)
#define REG_TIMER1_BASE            (0x08808000)
#define REG_TIMER2_BASE            (0x08809000)
uint32 SPIFLASH_TimRealTickGet(uint32 timer_id)
{
    uint32 timer_ctl_base;
    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_base = REG_TIMER1_BASE;
            break;
        }
        case TIMER_2:
        {
            timer_ctl_base = REG_TIMER2_BASE;
            break;
        }
        default:
        {
            timer_ctl_base = REG_TIMER2_BASE;
            break;
        }
     }
    return *(volatile unsigned int *)(timer_ctl_base + 0x14);
}

PUBLIC int SPIFLASH_CheckStatus(struct spi_flash *flash, uint  max_tick)
{
    uint32           cs = flash->spi->cs;
    int              result = 0;
    uint32           suspend_time;
    volatile uint32  old_tick, cur_tick, start_tick, last_resume_tick,i;
    volatile uint32   cur_timer1_tick;
    uint32 suspend_to_resume_latency, suspend_to_resume_cnt;  //us
    uint32 max_tick_us;
    uint32 tick_cnt;

    if(s_nMID[cs] == SPIFLASH_WINBOND_ID)
    {
        suspend_to_resume_latency = 2500;
    }
    else if(s_nMID[cs]  == SPIFLASH_MXIC_ID)
    {
        suspend_to_resume_latency = 1000;
    }
    else if(s_nMID[cs]  == SPIFLASH_GD_ID)
    {
        suspend_to_resume_latency = 2500;
    }
    else
    {
        suspend_to_resume_latency = 2500;
    }

    max_tick_us = (uint64)max_tick << 10;    /*__aeabi_uidivmod in flash, so use 1024*/
    max_tick_us = (max_tick_us > suspend_to_resume_latency) ? (max_tick_us) : (suspend_to_resume_latency);

    suspend_to_resume_cnt = suspend_to_resume_latency / TIMER_TICK_COUNT + 2;   /*__aeabi_uidivmod in flash, so use 64, need add 2tick as max=2500us*/
    start_tick        = SPIFLASH_TimRealTickGet(OS_TIMER);

    //@lin.liu. CR30772. should init the value
    cur_tick          = start_tick;
    suspend_time      = 0;
    last_resume_tick = SPIFLASH_TimRealTickGet(OS_TIMER);
    max_tick_us /= TIMER_TICK_COUNT;    //us to tick

    tick_cnt = (cur_tick >= start_tick) ? (cur_tick - start_tick) : (0xFFFFFFFF - start_tick + cur_tick);
    while( tick_cnt < (max_tick_us + suspend_time) )
    {
        result = SPIFLASH_GetPollStatus(flash);
        if(!(result & BIT_0))
        {
            result = 0x5a5a;//result = 1;
            break;
        }
        cur_tick = SPIFLASH_TimRealTickGet(OS_TIMER);
        if( b_need_monitor_int && GET_INTERRUPT_FLAG )
        {
            // One or more interrupt need to be handled,
            // suspend the current optr
            cur_timer1_tick = SPIFLASH_TimRealTickGet(OS_TIMER);

            tick_cnt = (cur_timer1_tick >= last_resume_tick) ? (cur_timer1_tick - last_resume_tick) : (0xFFFFFFFF - last_resume_tick + cur_timer1_tick);
            if(tick_cnt < suspend_to_resume_cnt)
            {
                SPIFLASH_RS_Delay(TIMER_TICK_COUNT * (suspend_to_resume_cnt - tick_cnt));
            }
            old_tick = SPIFLASH_TimRealTickGet(OS_TIMER);

            SPIFLASH_PESuspend(cs);
            STORE_SFC_OPTR(FLASH_STORE_OPTR_SUSPEND);
            SPIFLASH_ResetRegister(cs);
            // now enable the interrupt to handle it
            // then disable again.
            FLASH_RESTORE_IRQ;
             // make sure the interrupt can be handled.
            for( i = 0; i < 10; i++ ){};

            FLASH_DISABLE_IRQ;

            //if(FLASH_GetSuspendFlag())
            {
                SPIFLASH_PEResume(cs);
                STORE_SFC_OPTR(FLASH_STORE_OPTR_ERASING);
            }

            cur_tick   = SPIFLASH_TimRealTickGet(OS_TIMER);
            last_resume_tick = SPIFLASH_TimRealTickGet(OS_TIMER);
            tick_cnt = (cur_tick >= old_tick) ? (cur_tick - old_tick) : (0xFFFFFFFF - old_tick + cur_tick);
            suspend_time += tick_cnt;

        }
    } // end while

    if(result == 0x5a5a)
    {
        result = 1;
    }
    else
    {
        result = 0;
        //s_flash_checkstatus_timeout_cnt++;
    }
    return result;
}
#else
PUBLIC int SPIFLASH_CheckStatus(struct spi_flash *flash, uint  max_tick)
{
    uint32           cs = flash->spi->cs;
    int              result = 0;
    uint32           suspend_time;
    volatile uint32  old_tick, cur_tick, start_tick, last_resume_tick,i;
    volatile uint32   cur_timer1_tick;
    uint32 suspend_to_resume_latency, suspend_to_resume_cnt;  //us

    if(s_nMID[cs] == SPIFLASH_WINBOND_ID)
    {
        suspend_to_resume_latency = 2500;
    }
    else if(s_nMID[cs]  == SPIFLASH_MXIC_ID)
    {
        suspend_to_resume_latency = 1000;
    }
    else if(s_nMID[cs]  == SPIFLASH_GD_ID)
    {
        suspend_to_resume_latency = 2500;
    }
    else
    {
        suspend_to_resume_latency = 2500;
    }

    suspend_to_resume_cnt = suspend_to_resume_latency / TIMER1_TICK_COUNT;
    start_tick        = _get_current_system_clock();

    //@lin.liu. CR30772. should init the value
    cur_tick          = start_tick;
    suspend_time      = 0;
    last_resume_tick = TIMER1_INT_COUNT;

    while( (cur_tick - start_tick) < (max_tick + suspend_time) )
    {
        result = SPIFLASH_GetPollStatus(flash);
        if(!(result & BIT_0))
        {
            result = 0x5a5a;//result = 1;
            break;
        }
        cur_tick = _get_current_system_clock();
        if( b_need_monitor_int && GET_INTERRUPT_FLAG )
        {
             if(last_resume_tick == TIMER1_INT_COUNT)
            {
                __time1_init();
            }

            // One or more interrupt need to be handled,
            // suspend the current optr
            cur_timer1_tick = __time1_get_value();

            if(((last_resume_tick - cur_timer1_tick) < suspend_to_resume_cnt) && ((last_resume_tick >= cur_timer1_tick)))
            {
                SPIFLASH_RS_Delay(TIMER1_TICK_COUNT * (suspend_to_resume_cnt - (last_resume_tick - cur_timer1_tick)));
            }
            old_tick = _get_current_system_clock();

            SPIFLASH_PESuspend(cs);
            STORE_SFC_OPTR(FLASH_STORE_OPTR_SUSPEND);
            SPIFLASH_ResetRegister(cs);
            // now enable the interrupt to handle it
            // then disable again.
            FLASH_RESTORE_IRQ;
             // make sure the interrupt can be handled.
            for( i = 0; i < 10; i++ ){};

            FLASH_DISABLE_IRQ;

            //if(FLASH_GetSuspendFlag())
            {
                SPIFLASH_PEResume(cs);
                STORE_SFC_OPTR(FLASH_STORE_OPTR_ERASING);
            }

            cur_tick   = _get_current_system_clock();
            last_resume_tick = __time1_get_value();
            suspend_time += (cur_tick - old_tick + 1);

        }
    } // end while

    if(result == 0x5a5a)
    {
        result = 1;
    }
    else
    {
        result = 0;
        //s_flash_checkstatus_timeout_cnt++;
    }
    return result;
}
#endif

LOCAL void SPIFLASH_WriteEnable(int cs)
{
    spiflash_write_enable (myflash[cs]);
}

LOCAL void SPIFLASH_EnterSPIRead(int cs)
{
    /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    /* Set clock dividor to 0, running at (78/4)MHz */
    //SFCDRV_ClkCfg(cs, SFC_CLK_SAMPLE_DELAY_SEL<<9 | SFC_CLK_DIV_2);

    /* set AHB Read mode, 4 bit mode */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_READ, BIT_MODE_1, INI_CMD_BUF_7);

    /* write cmd 0xEB, big endian, 1byte, 1 bit mode */
    SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_READ);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);
    /* write address, big endian, 3byte, 1 bit mode */
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, BIT_MODE_1, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

	//CMD_READ requires clock under 50MHz
#if !defined(PLATFORM_UWS6121E)
	SFCDRV_ClkCfg(cs, SFC_CLK_DIV_2);
#endif

}

PUBLIC void SPIFLASH_ResetRegister(int cs)
{
#if !defined(PLATFORM_UWS6121E)
    if(QPI_MODE == myflash[cs]->work_mode)
    {
        myflash[cs]->enter_4read(myflash[cs]);
	FLASH_DELAY_US(30)
    }
    else
    {
       // myflash[cs]->enter_read(myflash[cs]);
       SPIFLASH_EnterSPIRead(cs);
	FLASH_DELAY_US(30)
    }
#endif
}



LOCAL BOOLEAN _SPIFLASH_EnterSecurity(int cs)
{
    /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    /* set write command */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

    /* cmd buf 0: write cmd 0xB1, big endian, 1byte, 1 bit mode */
    SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_ENTERSECURITY);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    /* clear int status */
    SFCDRV_IntClr(cs);

    /* start soft request */
#if defined(CHIP_VER_UWS6121E)
    SFCDRV_SoftReq(cs);
#else
    SFCDRV_SoftReq();
#endif

    //while(!(SFC_INT_STATUS & SFCDRV_GetStatus()));
  	WAIT_SFC_INT(cs);
    /* clear int status */
    SFCDRV_IntClr(cs);

    return TRUE;

}

LOCAL BOOLEAN _SPIFLASH_ExitSecurity(int cs)
{
    /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    /* set write command */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

    /* cmd buf 0: write cmd 0xB1, big endian, 1byte, 1 bit mode */
    SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_EXITSECURITY);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    /* clear int status */
    SFCDRV_IntClr(cs);

    /* start soft request */
#if defined(CHIP_VER_UWS6121E)
    SFCDRV_SoftReq(cs);
#else
    SFCDRV_SoftReq();
#endif

    //while(!(SFC_INT_STATUS & SFCDRV_GetStatus()));
  	WAIT_SFC_INT(cs);
    /* clear int status */
    SFCDRV_IntClr(cs);

    return TRUE;

}

LOCAL BOOLEAN _ReadUNID(uint32 cs)
{
    uint32 unid0 = 0x0, unid1 = 0x0;
    //uint32 cs = 0x0;

    if(s_nMID[cs] == SPIFLASH_WINBOND_ID)
    {
        /* clear cmd buffer and type buffer*/
        SFCDRV_TypeBufClr(cs);
        SFCDRV_CMDBufClr(cs);

        /* set write command */
        SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

        /* cmd buf 0: write cmd 0x9F, big endian, 1byte, 1 bit mode */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDUNID);
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

        /* cmd buf 1: write address, big endian, 3byte, 1 bit mode */
        //SFCDRV_SetCMDBuf(cs, CMD_BUF_1, 0x0);
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, BIT_MODE_1, BYTE_NUM_4, CMD_MODE_WRITE, SEND_MODE_0);

        /* cmd buf 7: read, big endian, 4byte ID, 1 bit mode */
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_7, BIT_MODE_1, BYTE_NUM_4, CMD_MODE_READ, SEND_MODE_0);
        /* cmd buf 8: read, big endian, 4byte ID, 1 bit mode */
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_8, BIT_MODE_1, BYTE_NUM_4, CMD_MODE_READ, SEND_MODE_0);

        /* clear int status */
        SFCDRV_IntClr(cs);

        /* start soft request */
#if defined(CHIP_VER_UWS6121E)
        SFCDRV_SoftReq(cs);
#else
        SFCDRV_SoftReq();
#endif

        //while(!(SFC_INT_STATUS & SFCDRV_GetStatus()));
      	WAIT_SFC_INT(cs);

        /* clear int status */
        SFCDRV_IntClr(cs);

        unid0 = SFCDRV_GetCMDBuf(cs, CMD_BUF_7);
        unid1 = SFCDRV_GetCMDBuf(cs, CMD_BUF_8);

        SPIFLASH_EnterSPIRead(cs);

        memcpy(s_unid+4, &unid0, 4);
        memcpy(s_unid, &unid1, 4);
    }

    return SCI_TRUE;
}

PUBLIC BOOLEAN SPIFLASH_ReadUNID(uint8 *pUNID)
{
    if(PNULL == pUNID)
        return SCI_FALSE;

    memcpy(pUNID, s_unid, 9);
    return SCI_TRUE;
}

PUBLIC BOOLEAN SPIFLASH_IsOldUNID(void)
{
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    return SCI_FALSE;
#else
    uint32 cs = 0x0;
    if(s_nMID[cs] == SPIFLASH_WINBOND_ID)
        return SCI_TRUE;
    else
        return SCI_FALSE;
#endif
}

/*****************************************************************************/
// Description :    This function is used to init MX flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :
/*****************************************************************************/
PUBLIC int SPIFLASH_Init(int cs)  /* If Init success, return true, else return false */
{
    uint8   nMID, nDID1, nDID2;
    uint8  i;
    SPIFLASH_SPEC_T *pSpiFlashSpec = NULL;

#if defined(CHIP_VER_6531)
    {
        uint32 j;

    	j = REG32 (APB_CLK_CFG1);

    	j &=  ~CLK_SFC_SEL_MSK;/*lint !e737*/
    	j |= 0x1 << CLK_SFC_SEL_SHIFT;
    	REG32 (APB_CLK_CFG1) = j;

    	for (j=0; j<100; j++) {}
    }
#endif

    s_flash_init_flag = 1;

    for(i=0; i<100; i++){};

    flash_store_optr_ptr = flash_store_optr_array;

#if defined(PLATFORM_UWS6121E)
    SFCDRV_ClkCfg(cs, (2<<4) | (8<<8));
#endif
    spiflash_reset_anyway(cs);

    work_mode[cs] = SPI_MODE;
    if (SPIFLASH_ReadID(cs, &nMID, &nDID1, &nDID2) != TRUE)
    {
        return SCI_FALSE;
    }

    pSpiFlashSpec = SPIFLASH_SPEC_Get(nMID, nDID1, nDID2);
    if(pSpiFlashSpec == NULL)
    {
            return SCI_FALSE;
    }

    nQEPos[cs] = pSpiFlashSpec->nQEPos;
    nWIPPos[cs] = pSpiFlashSpec->nWIPPos;
    EnterQPI[cs] = pSpiFlashSpec->EnterQPI;
    ExitQPI[cs] = pSpiFlashSpec->ExitQPI;
    cmd_4pp[cs] = pSpiFlashSpec->write_4pp;
    cmd_4read[cs] = pSpiFlashSpec->cmd_4read;
    reset_support[cs] = pSpiFlashSpec->reset_support;
    QPI_support[cs]  = pSpiFlashSpec->QPI_support;
    s_nMID[cs] = pSpiFlashSpec->nMID;
    s_nDID1[cs] = pSpiFlashSpec->nDID1;
    s_nDID2[cs] = pSpiFlashSpec->nDID2;
    CMD_PES[cs] = pSpiFlashSpec->CMD_PES;
    CMD_PER[cs] = pSpiFlashSpec->CMD_PER;
    pExtcfg[cs] = pSpiFlashSpec->pExtcfg;

    s_flash_sector_size = pSpiFlashSpec->spiflash_cfg->sect_size;
    s_flash_flash_size = pSpiFlashSpec->spiflash_cfg->flash_size;

    if(cs == 0)
    {
        flash_cs0_desity = ((Spiflash_ExtCfg_PRT)(pSpiFlashSpec->pExtcfg))->desity;
    }
    else if (cs == 1)
    {
        flash_cs1_desity = ((Spiflash_ExtCfg_PRT)(pSpiFlashSpec->pExtcfg))->desity;
    }

    if(pSpiFlashSpec->pExtcfg != SCI_NULL)
    {
        struct SPIFLASH_ExtCfg* cfg = (struct SPIFLASH_ExtCfg*)(pSpiFlashSpec->pExtcfg);

        if(cfg->voltage == SPIFLASH_VOLTAGE_3V)
        {
#if !defined(PLATFORM_UWS6121E)
            REG32(PIN_ESMCSN0_REG) |= BIT_10;
            REG32(PIN_ESMD0_REG) |= BIT_10;
            REG32(PIN_ESMD1_REG) |= BIT_10;
            REG32(PIN_ESMD2_REG) |= BIT_10;
            REG32(PIN_ESMD3_REG) |= BIT_10;
            REG32(PIN_ESMCLK_REG) |= BIT_10;
#else
#if defined(CHIP_VER_UIX8910MPW)
            REG32(REG_IOMUX_PAD_SPI_FLASH_CS0_CFG_REG) |= IOMUX_PAD_SPI_FLASH_CS0_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_0_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_0_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_1_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_1_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_2_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_2_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_3_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_3_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_CLK_CFG_REG) |= IOMUX_PAD_SPI_FLASH_CLK_DRV_STRENGTH(1);
#endif
#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
            REG32(REG_IOMUX_PAD_SPI_FLASH_CS_CFG_REG) |= IOMUX_PAD_SPI_FLASH_CS_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_0_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_0_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_1_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_1_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_2_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_2_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_3_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_3_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_CLK_CFG_REG) |= IOMUX_PAD_SPI_FLASH_CLK_DRV_STRENGTH(1);
#endif

#endif
        }
    }

    _ReadUNID(cs);

#if !defined(PLATFORM_UWS6121E)
    SFCDRV_ClkCfg(cs, SFC_CLK_DIV_0 | SFC_CLK_2X_EN | SFC_CLK_OUT_2X_EN | SFC_CLK_SAMPLE_2X_PHASE | SFC_CLK_SAMPLE_2X_EN);
#endif

    myflash[cs] = spi_flash_probe(0, cs, 0, 0);
    if (myflash[cs]) {
        spi_flash_common_operation(myflash[cs]);
        //SPIFLASH_ResetRegister(cs);
    } else {
        //SCI_TraceLow("Failed to initialize SPI flash at %u:%u\n", 0, cs);
        return SCI_FALSE;
    }
    work_mode[cs] = myflash[cs]->work_mode ;

#if defined(PLATFORM_UWS6121E)
#if !defined(FPGA_SUPPORT)
#if defined(CHIP_VER_UIX8910)      /*8910m evb*/
    SFCDRV_ClkCfg(cs, (2<<4) | (4<<8));
#else
    SFCDRV_ClkCfg(cs, (2<<4) | (2<<8));
#endif
#else
    SFCDRV_ClkCfg(cs, (2<<4) | (2<<8));   /*fpga ku095  68MHz*/
#endif
#endif

    SPIFLASH_ResetRegister(cs);

    return SCI_TRUE;
}

/*****************************************************************************/
// Description :    This function is used to close MX flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :           it is called when power down
/*****************************************************************************/
LOCAL void SPIFLASH_Close(void)
{
    s_flash_init_flag = 0;
}

LOCAL void SPIFLASH_Config(NOR_FLASH_CONFIG_PTR t1,NORMCP_SPEC_PTR t2)
{

}
/*lint -save -e533 */
PUBLIC BOOLEAN SPIFLASH_GetID(uint cs, uint8 *pMID, uint8 *pDID1, uint8 *pDID2)
{
    SCI_ASSERT(cs < SFC_MAX_CS); /*assert verified*/

    *pMID = s_nMID[cs];
    *pDID1 = s_nDID1[cs];
    *pDID2 = s_nDID2[cs];
    return SCI_TRUE;//coverity warnning
}
/*lint -restore*/
/*****************************************************************************/
// Description :    This function is used to read manufacturer ID and device ID
// Global resource dependence :
// Author :         Fei.Zhang
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_ReadID(int cs, uint8 *pMID, uint8 *pDID1, uint8 *pDID2)
{
    uint32 id = 0;

    SFCDRV_CSCfg(cs);

    /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    /* set write command */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

    /* cmd buf 0: write cmd 0x9F, big endian, 1byte, 1 bit mode */
    SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_READ_ID);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);
    /* cmd buf 7: read, big endian, 3byte ID(1byte manufacturer ID + 2byte device ID), 1 bit mode */
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_7, BIT_MODE_1, BYTE_NUM_3, CMD_MODE_READ, SEND_MODE_0);

    /* clear int status */
    SFCDRV_IntClr(cs);

    /* start soft request */
#if defined(CHIP_VER_UWS6121E)
    SFCDRV_SoftReq(cs);
#else
    SFCDRV_SoftReq();
#endif

    WAIT_SFC_INT(cs);

    /* clear int status */
    SFCDRV_IntClr(cs);

    id = SFCDRV_GetCMDBuf(cs, CMD_BUF_7);
#if !defined(PLATFORM_UWS6121E)
    *pMID = (id&0xFF000000)>>24;
    *pDID1 = (id&0x00FF0000)>>16;
    *pDID2 = (id&0x0000FF00)>>8;
#else
    *pDID2 = (id&0xFF000000)>>24;
    *pDID1 = (id&0x00FF0000)>>16;
    *pMID = (id&0x0000FF00)>>8;
#endif
    SPIFLASH_EnterSPIRead(cs);

    return SCI_TRUE;
}


/*****************************************************************************/
// Description :    write function
// Global resource dependence :
//
// Author :         henry.he
// Parameter:
// Note :
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_WriteByte_CS(int cs, uint32 addr, const uint8 * buf, uint32 counter)
{
#ifdef PLATFORM_SC6530
    if((cs > 0) && (s_nMID[SPIFLASH_CS1] == 0))
    {
        SCI_ASSERT(0); /*assert verified*/
    }
#endif

    SFCDRV_CSCfg(cs);

    SCI_ASSERT(counter <= 4);/*assert verified*/

    FLASH_DISABLE_IRQ; // disable the irq and save the prev state;
    // For the program only need 700us(max),
    // needn't monitor the interrupt status.
    b_need_monitor_int = 0;

    STORE_SFC_OPTR( FLASH_STORE_OPTR_PROGRAMING )

    /* write data to flash */
    myflash[cs]->write(myflash[cs], addr, counter, buf);

    SPIFLASH_ResetRegister(cs);

    STORE_SFC_OPTR( FLASH_STORE_OPTR_IDLE )

    FLASH_RESTORE_IRQ;

    return SCI_TRUE;

}

/*****************************************************************************/
// Description :    buffer write function
// Global resource dependence :
// s_flash_sector_size
// Author :         Younger.yang
// Parameter:
// Note :
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_WriteByte(    // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8  * buf,             // Pointer to the buffer containing the data to be written.
    uint32       counter)           // number of byte to write.
{
    uint8 buffer[4] = {0};
    BOOLEAN ret = TRUE;
#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
    uint32 cs1_cnt = 0;
#endif

    uint32 verify_addr = addr;

    SCI_ASSERT(counter <= 4);/*assert verified*/

    SCI_MEMCPY(buffer, buf, counter);

    //if(addr >= SPIFLASH_CS1_BASE_ADDRESS)
#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
    if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
    {
        addr -= SPIFLASH_CS1_BASE_ADDRESS;

        ret = SPIFLASH_WriteByte_CS(SPIFLASH_CS1, addr, buffer, counter);
    }
    else if(addr + counter > SPIFLASH_CS1_BASE_ADDRESS)
    {
        cs1_cnt = addr + counter - SPIFLASH_CS1_BASE_ADDRESS;
        counter = SPIFLASH_CS1_BASE_ADDRESS - addr;

        ret = SPIFLASH_WriteByte_CS(SPIFLASH_CS0, addr, buffer, counter);
        ret = SPIFLASH_WriteByte_CS(SPIFLASH_CS1, SPIFLASH_CS1_BASE_ADDRESS, buffer+counter, cs1_cnt);
    }
    else
#endif
    {
        ret = SPIFLASH_WriteByte_CS(SPIFLASH_CS0, addr, buffer, counter);
    }

    // write success judgement
    {
        uint8 tmp = 0;
        uint32 i= 0;
        for(i = 0; i < counter; i++)
        {
            tmp = *(volatile uint8 *) (verify_addr + i);
            if(tmp != buffer[i])
            {
                spiflash_error_type = SPIFLASH_WRITE_FAIL;
                SCI_PASSERT(0,("Flash write byte fail at addr=0x%x, w_data=0x%x, r_data=0x%x", verify_addr+i, buffer[i], tmp)); /*assert verified*/
            }
        }
    }

    return ret;
}

#if !defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description :    write function
// Global resource dependence :
//
// Author :         henry.he
// Parameter:
// Note :
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_WriteBuffer_CS(int cs, uint32 addr, const uint8 * buf, uint32 counter)
{
#ifdef PLATFORM_SC6530
    if((cs > 0) && (s_nMID[SPIFLASH_CS1] == 0))
    {
        SCI_ASSERT(0); /*assert verified*/
    }
#endif

    SCI_ASSERT(!(addr    % 4));/*assert verified*/
    SCI_ASSERT(!(counter % 4));/*assert verified*/
    SCI_ASSERT(counter <= 40);/*assert verified*/

    FLASH_DISABLE_IRQ; // disable the irq and save the prev state;

    // For the program only need 700us(max),
    // needn't monitor the interrupt status.
    b_need_monitor_int = 0;

    /* write data to flash */
    myflash[cs]->write(myflash[cs], addr, counter, buf);

    SPIFLASH_ResetRegister(cs);

    STORE_SFC_OPTR( FLASH_STORE_OPTR_IDLE )
    FLASH_RESTORE_IRQ;

    return SCI_TRUE;

}

/*****************************************************************************/
// Description :    buffer write function
// Global resource dependence :
// s_flash_sector_size
// Author :         Younger.yang
// Parameter:
// Note :
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_WriteBuffer(    // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8  * buf,             // Pointer to the buffer containing the data to be written.
    uint32       counter)           // number of byte to write.
{
    uint8 buffer[40] = {0};
#ifdef PLATFORM_SC6530
    uint32 cs1_cnt = 0;
#endif
//#if !defined(PLATFORM_UWS6121E)
    uint32 verify_addr = addr;
//#else
//    uint32 verify_addr = SPIFLASH_READ_BASE + addr;
//#endif
    BOOLEAN ret = TRUE;

    SCI_ASSERT(!(addr    % 4));/*assert verified*/
    SCI_ASSERT(!(counter % 4));/*assert verified*/
    SCI_ASSERT(counter <= 40);/*assert verified*/

    SCI_MEMCPY(buffer, buf, counter);

#ifdef PLATFORM_SC6530
    if(addr >= SPIFLASH_CS1_BASE_ADDRESS)
    {
        addr -= SPIFLASH_CS1_BASE_ADDRESS;

        ret = SPIFLASH_WriteBuffer_CS(SPIFLASH_CS1, addr, buffer, counter);
    }
    else if(addr + counter > SPIFLASH_CS1_BASE_ADDRESS)
    {
        cs1_cnt = addr + counter - SPIFLASH_CS1_BASE_ADDRESS;
        counter = SPIFLASH_CS1_BASE_ADDRESS - addr;

        ret = SPIFLASH_WriteBuffer_CS(SPIFLASH_CS0, addr, buffer, counter);
        ret = SPIFLASH_WriteBuffer_CS(SPIFLASH_CS1, SPIFLASH_CS1_BASE_ADDRESS, buffer+cs1_cnt, cs1_cnt);
    }
    else
#endif
    {
        ret = SPIFLASH_WriteBuffer_CS(SPIFLASH_CS0, addr, buffer, counter);
    }


    // write success judgement
    {
        uint8 tmp = 0;
        uint32 i = 0;
        for(i = 0; i < counter; i++)
        {
            tmp = *(volatile uint8 *) (verify_addr + i);
            if(tmp != buffer[i])
            {
                spiflash_error_type = SPIFLASH_WRITE_FAIL;
                SCI_PASSERT(0,("Flash write buffer fail at addr=0x%x, w_data=0x%x, r_data=0x%x", verify_addr+i, buffer[i], tmp)); /*assert verified*/
            }
        }
    }


    return ret;
}
#else
LOCAL BOOLEAN SPIFLASH_WriteBuffer_CS(int cs, uint32 addr, const uint8 * buf, uint32 counter)
{
    FLASH_DISABLE_IRQ; // disable the irq and save the prev state;

    // For the program only need 700us(max),
    // needn't monitor the interrupt status.
    b_need_monitor_int = 0;

    /* write data to flash */
    myflash[cs]->write(myflash[cs], addr, counter, buf);

    SPIFLASH_ResetRegister(cs);

    STORE_SFC_OPTR( FLASH_STORE_OPTR_IDLE )
    FLASH_RESTORE_IRQ;

    return SCI_TRUE;

}

LOCAL BOOLEAN SPIFLASH_WriteBuffer(    // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8  * buf,             // Pointer to the buffer containing the data to be written.
    uint32       counter)           // number of byte to write.
{
    uint32 verify_addr = addr;
    BOOLEAN ret = TRUE;
    int result;
    uint32 cs1_cnt = 0;

#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
    if(addr >= SPIFLASH_CS1_BASE_ADDRESS)
    {
        addr -= SPIFLASH_CS1_BASE_ADDRESS;
        ret = SPIFLASH_WriteBuffer_CS(SPIFLASH_CS1, addr, buf, counter);
    }
    else if(addr + counter > SPIFLASH_CS1_BASE_ADDRESS)    /*Á¬ÐøµØÖ·*/
    {
        cs1_cnt = addr + counter - SPIFLASH_CS1_BASE_ADDRESS;
        counter = SPIFLASH_CS1_BASE_ADDRESS - addr;

        ret = SPIFLASH_WriteBuffer_CS(SPIFLASH_CS0, addr, buf, counter);
        ret = SPIFLASH_WriteBuffer_CS(SPIFLASH_CS1, SPIFLASH_CS1_BASE_ADDRESS, buf+counter, cs1_cnt);
    }
    else
    {
        addr -= SPIFLASH_CS0_BASE_ADDRESS;
        ret = SPIFLASH_WriteBuffer_CS(SPIFLASH_CS0, addr, buf, counter);
    }

    result = memcmp((uint8 *)verify_addr, (uint8 *)buf, counter);
    if (result)
    {
         SCI_PASSERT(0,("Flash write buffer fail"));
    }
    if (cs1_cnt != 0)
    {
        result = memcmp((uint8 *)SPIFLASH_CS1_BASE_ADDRESS, (uint8 *)(buf+counter), cs1_cnt);
        if (result)
        {
             SCI_PASSERT(0,("Flash write2 buffer fail"));
        }
    }

#else
    ret = SPIFLASH_WriteBuffer_CS(SPIFLASH_CS0, addr, buf, counter);

    result = memcmp((uint8 *)verify_addr, (uint8 *)buf, counter);
    if (result)
    {
         SCI_PASSERT(0,("Flash write buffer fail"));
    }

#if 0
    // write success judgement
    {
        uint8 tmp = 0;
        uint32 i = 0;
        for(i = 0; i < counter; i++)
        {
            tmp = *(volatile uint8 *) (verify_addr + i);
            if(tmp != buf[i])
            {
                spiflash_error_type = SPIFLASH_WRITE_FAIL;
                SCI_PASSERT(0,("Flash write buffer fail at addr=0x%x, w_data=0x%x, r_data=0x%x", verify_addr+i, buf[i], tmp)); /*assert verified*/
            }
        }
    }
#endif
#endif
    return ret;
}
#endif

/*****************************************************************************/
//     Description :
//      The function write word to flash.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//      The address MUST be word boundary.
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written
//      is not correct.
/*****************************************************************************/
PUBLIC BOOLEAN  SPIFLASH_WriteWord(
    uint32      addr,
    uint16      data)
{
    uint8  * buf = (uint8*)(&data);
    FLASH_PTR_TYPE          addr_ptr = (FLASH_PTR_TYPE)addr;
    volatile uint16         tmp;

    int cs = SPIFLASH_CS0;

#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
    if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
    {
        cs = SPIFLASH_CS1;
        addr -= SPIFLASH_CS1_BASE_ADDRESS;
    }
#endif

    SCI_ASSERT( !(addr & 1));/*assert verified*/
    FLASH_DISABLE_IRQ;
    tmp = *addr_ptr;
    FLASH_RESTORE_IRQ;
    if( tmp == data )
    {
       // It is same, needn't write
       return SCI_TRUE;
    }
    else if( (tmp & data) != data )
    {
        // some bits from 0 to 1, can't write
#if 1
        return SCI_FALSE;
#endif
    }

    FLASH_DISABLE_IRQ; // disable the irq and save the prev state;
    // For the program only need 700us(max),
    // needn't monitor the interrupt status.
    b_need_monitor_int = 0;

    /* write data to flash */
    myflash[cs]->write(myflash[cs], addr, 2, buf);


    SPIFLASH_ResetRegister(cs);

    STORE_SFC_OPTR( FLASH_STORE_OPTR_IDLE )
    FLASH_RESTORE_IRQ;

    tmp = FLASH_RD(addr_ptr);

    return ( (tmp == data)?SCI_TRUE:SCI_FALSE );
}

LOCAL BOOLEAN SPIFLASH_PESuspend(int cs)
{
    int   ret = TRUE;
    //spiflash_cmd_suspend(myflash[cs], CMD_PES[cs]);
    u8 cmd = CMD_PES[cs];
    spiflash_cmd_write(myflash[cs], &cmd, 1, NULL, 0);

    FLASH_DELAY_US(30)
    SPIFLASH_WIP_CHECKSTATUS(myflash[cs]);
    return (BOOLEAN)ret;
}

LOCAL BOOLEAN SPIFLASH_PEResume(int cs)
{
    int   ret = TRUE;
    //spiflash_cmd_resume(myflash[cs], CMD_PER[cs]);
    u8 cmd = CMD_PER[cs];
    spiflash_cmd_write(myflash[cs], &cmd, 1, NULL, 0);

    FLASH_DELAY_US(30)
    //SPIFLASH_WIP_CHECKSTATUS(myflash[cs]);
    return (BOOLEAN)ret;
}


LOCAL void SPIFLASH_Suspend(void)
{
    int cs = s_erase_cs;
    uint32  status;
    status = GET_SFC_STORE_OPTR;

    if((FLASH_STORE_OPTR_PROGRAMING == status)
    || (FLASH_STORE_OPTR_ERASING    == status))
    {
        SPIFLASH_PESuspend(cs);
  //      FLASH_DELAY_US(20)
        SPIFLASH_ResetRegister(cs);

        STORE_SFC_OPTR(FLASH_STORE_OPTR_SUSPEND);
    }
}

LOCAL void SPIFLASH_Resume(void)
{
    int cs = s_erase_cs;

    if(FLASH_STORE_OPTR_SUSPEND == GET_SFC_STORE_OPTR)
    {

        FLASH_DISABLE_IRQ;// disable the irq and save the prev state;

        // If the irq/fiq has been disabled before call this function,
        // needn't monitor the interrupt status.
        b_need_monitor_int = (s_flash_interrupt_save ^ FLASH_IRQ_FIQ_DISABLE);

      //b_need_monitor_int = 1;

        SPIFLASH_PEResume(cs);
  //      FLASH_DELAY_US(30)

        STORE_SFC_OPTR( FLASH_STORE_OPTR_ERASING )
        SPIFLASH_CheckStatus(myflash[cs], FLASH_MAX_ERASE_TIME);

        SPIFLASH_ResetRegister(cs);

        STORE_SFC_OPTR( FLASH_STORE_OPTR_IDLE )

        FLASH_RESTORE_IRQ;
    }
}


/*****************************************************************************/
//     Description :
//      The function write data to flash.
//
//    Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//    Note:
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written
//      is not correct.
/*****************************************************************************/
#if !defined(PLATFORM_UWS6121E)
LOCAL BOOLEAN SPIFLASH_Write(    // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8      * buf,   // Pointer to the buffer containing the data to be written.
    uint32       len)   // number of bytes to write.
{
    int tmp;
    uint8 *data_buf = (uint8 *)buf;
#if 0
    if(SPIFLASH_IsStartAddOfSector(addr))
    {
        if( 0 == SPIFLASH_EraseBlock(addr, BLOCK_SIZE) )
        {
            return SCI_FALSE;
        }
    }
#endif
    if((tmp = (4 - (addr & 0x03))) != 4)
    {
        if((uint32) tmp < len)
        {
            SPIFLASH_WriteByte(addr, data_buf, tmp);
            addr += (uint32) tmp;
            data_buf += (uint32) tmp;
            len -= (uint32) tmp;
        }
        else
        {
            SPIFLASH_WriteByte(addr, data_buf, len);
            addr += len;
            data_buf += len;
            len = 0;
        }
    }

    while(len >= 32)
    {
        /* 1 sector 256 bytes align */
        if(((uint32)(addr & 0xFF) + 32) > 0x100)
        {
            uint32 size = 0x100 - addr & 0xFF;

            SPIFLASH_WriteBuffer(addr, data_buf, size);
            addr += (uint32) size;
            data_buf += (uint32) size;
            len -= (uint32) size;
        }
        else
        {
            SPIFLASH_WriteBuffer(addr, data_buf, 32);
            addr += 32;
            data_buf += 32;
            len -= 32;
         }
    }

    tmp = len / 4;
    while(tmp > 0)
    {
        /* 1 sector 256 bytes align */
        if(((addr & 0xFF) + (uint32) tmp*4) > 0x100)
        {
            uint32 size = 0x100 - addr & 0xFF;

            SPIFLASH_WriteBuffer(addr, data_buf, size);
            addr += size;
            data_buf += size;
            len -= size;
        }
        else
        {
            SPIFLASH_WriteBuffer(addr, data_buf, tmp*4);
            addr += (uint32) tmp*4;
            data_buf += tmp*4;
            len-= (uint32) tmp*4;
        }
        tmp = len / 4;
    }

    if(len > 0)
        SPIFLASH_WriteByte(addr, data_buf, len);

    return SCI_TRUE;
}
#else
LOCAL BOOLEAN SPIFLASH_Write(    // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8      * buf,   // Pointer to the buffer containing the data to be written.
    uint32       len)   // number of bytes to write.
{
    while(len)
    {
        /* 1 sector 256 bytes align */
        int size = 0x100 - (addr & (0x100 - 1));
        size = (size >= len) ? (len) : (size);
        SPIFLASH_WriteBuffer(addr, buf, size);
        addr += size;
        buf += size;
        len -= size;
    }
    return SCI_TRUE;
}
#endif


// enter_read/exit_read should be called with pair
//
LOCAL void SPIFLASH_EnterRead( void )
{
    uint32  status;
    status = GET_SFC_STORE_OPTR;

    if((FLASH_STORE_OPTR_PROGRAMING == status)
    || (FLASH_STORE_OPTR_ERASING    == status))
    {
        // the flash being erase, so suspend first
    }
}

LOCAL void SPIFLASH_ExitRead( void )
{
    uint32  status;
    status = GET_SFC_STORE_OPTR;

    if(FLASH_STORE_OPTR_SUSPEND == status)
    {
        // the flash being suspended, so resume it.
    }
}

/*****************************************************************************/
//     Description :
//      Read word(two byte) from the given address.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//      the address MUST be word boundary.
/*****************************************************************************/
LOCAL uint16 SPIFLASH_ReadWord(  // The word that has read from the given address.
    uint32 addr)        // the address that will be read.
{
    uint16   read_data; // the data that be read.

    // Word boundary.
    SCI_ASSERT( !(addr & 1) );/*assert verified*/

    read_data = *((FLASH_PTR_TYPE)addr);

    return read_data;
}

/*****************************************************************************/
//     Description :
//      The function reads up to size bytes from the device and stores them in buffer.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_Read(      // If read successfully, return true, else false;
    uint32    addr,      // the start address that will be read
    uint8   * buf,       // store the data that being read to this buffer.
    uint32    read_len)  // the count of bytes that will be read.
{
    uint32 i;

    for(i=0; i<read_len; i++)
    {
        *(buf + i) = *(uint8 *)(addr + i);
    }

    return SCI_TRUE;
}

/*****************************************************************************/
//     Description :
//      The function unlock the sector that contain the address.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//
/*****************************************************************************/
LOCAL void SPIFLASH_Unlock(
    uint32   addr)  // the sector that contain the address will be unlock.
{
    int cs = SPIFLASH_CS0;

#ifdef PLATFORM_SC6530
    if(addr >= SPIFLASH_CS1_BASE_ADDRESS)
    {
        cs = SPIFLASH_CS1;
        addr -= SPIFLASH_CS1_BASE_ADDRESS;
    }
#endif

    /* For spiflash, we will unlock all chip
       no matter which region was locked before!
       The param of addr/size is invalid here! */
    myflash[cs]->unlock(myflash[cs], addr, 0);

    SPIFLASH_ResetRegister(cs);

}

/*****************************************************************************/
//     Description :
//      The function lock down the sector that contain the address.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//
/*****************************************************************************/
LOCAL void SPIFLASH_Lock(uint32  addr )
{
/* verify */
}

/*****************************************************************************/
//    Description :
//      The function lock down the region size that from the address.
//
//    Global resource dependence :
//      None
//    Author:
//      henry.he
//    Note:
//      It don't support lock aregion across cs0 and cs1!!
/*****************************************************************************/
PUBLIC uint32 SPIFLASH_LockRegion(uint32 addr, uint32 end_addr)
{
    uint32 size = end_addr-addr+1;

    // Change the addr to cs0/cs1
    int cs = SPIFLASH_CS0;
#ifdef PLATFORM_SC6530
    if(addr >= SPIFLASH_CS1_BASE_ADDRESS)
    {
        cs = SPIFLASH_CS1;
        addr -= SPIFLASH_CS1_BASE_ADDRESS;
    }
#endif

    /* Unlock all chip firstly. */
    //myflash[cs]->unlock(myflash[cs], addr, size);
    SPIFLASH_ResetRegister(cs);

    if(myflash[cs]->lock(myflash[cs], addr, size))
    {
        SPIFLASH_ResetRegister(cs);
        /* Lock the specified region fail! */
        SCI_PASSERT(0,("spiflash Lock Fail addr=0x%x size=0x%x!",addr,size)); /*assert verified*/
    }

    SPIFLASH_ResetRegister(cs);

    return SCI_TRUE;
}

/*****************************************************************************/
//     Description :
//      Is the last erase optr completed?
//
//    Global resource dependence :
//      None
//
//    Return :
//      If the erase has been completely, return true, other false
//  Author:
//      Lin.liu
//  Note:
//      This function MUST be called when system assert only,
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_IsEraseCompleted(void)
{
    uint32  status;
    status = GET_SFC_STORE_OPTR;

    if((FLASH_STORE_OPTR_PROGRAMING == status)
    || (FLASH_STORE_OPTR_ERASING    == status)
    || (FLASH_STORE_OPTR_SUSPEND    == status))
    {
        return SCI_FALSE;
    }
    // Other case, we think has been completed.
    return SCI_TRUE;
}

/*****************************************************************************/
//     Description :
//      Erase the whole chip.
//
//    Global resource dependence :
//      None
//
//  Author:
//      Fei.zhang
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_EraseChip(int cs)
{
    FLASH_DISABLE_IRQ;// disable the irq and save the prev state;
    // If the irq/fiq has been disabled before call this function,
    // needn't monitor the interrupt status.
    b_need_monitor_int = (s_flash_interrupt_save ^ FLASH_IRQ_FIQ_DISABLE);

    myflash[cs]->erase_chip(myflash[cs]);

    SPIFLASH_ResetRegister(cs);

    STORE_SFC_OPTR( FLASH_STORE_OPTR_IDLE )
    FLASH_RESTORE_IRQ;

    return SCI_TRUE;
}

/*****************************************************************************/
//     Description :
//      Erase the sector that contain the address.
//
//    Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_Erase(
    uint32 addr)
{
    FLASH_PTR_TYPE      addr_ptr;
    int   ret=SCI_TRUE;

    int cs = SPIFLASH_CS0;

#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
    if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
    {
        cs = SPIFLASH_CS1;
        addr -= SPIFLASH_CS1_BASE_ADDRESS;
    }
#endif

    FLASH_DISABLE_IRQ;// disable the irq and save the prev state;
    // If the irq/fiq has been disabled before call this function,
    // needn't monitor the interrupt status.
    b_need_monitor_int = (s_flash_interrupt_save ^ FLASH_IRQ_FIQ_DISABLE);

    STORE_SFC_OPTR( FLASH_STORE_OPTR_ERASING )

    /*
    only make sure the address is even.
    */
    addr_ptr = (FLASH_PTR_TYPE)(addr & 0xFFFFFFFE);

    s_flash_erasing_addr = addr_ptr;

    myflash[cs]->erase(myflash[cs], addr, BLOCK_SIZE);

    SPIFLASH_ResetRegister(cs);

    STORE_SFC_OPTR( FLASH_STORE_OPTR_IDLE )
    FLASH_RESTORE_IRQ;

    return (BOOLEAN)ret;
}

LOCAL BOOLEAN SPIFLASH_EnterDP(int cs)
{
    u8 cmd = CMD_RDP;
    spiflash_cmd_write(myflash[cs], &cmd, 1, NULL, 0);
    return SCI_TRUE;
}

LOCAL BOOLEAN SPIFLASH_ExitDP(int cs)
{
    u8 cmd = CMD_DP;
    spiflash_cmd_write(myflash[cs], &cmd, 1, NULL, 0);
    return SCI_TRUE;
}


#if 1
void SPIFLASH_Open(void)
{

}
//MX driver struct.
const NOR_FLASH_DRIVER_T  g_spiflash_driver =
{
    SPIFLASH_Open,
    SPIFLASH_ReadWord,
    SPIFLASH_Read,
    SPIFLASH_WriteWord,
    SPIFLASH_Write,
    SPIFLASH_Erase,
    SPIFLASH_IsEraseCompleted,
    SPIFLASH_Suspend,
    SPIFLASH_Resume,
    SPIFLASH_Unlock,
    SPIFLASH_Lock,
    SPIFLASH_EnterRead,
    SPIFLASH_ExitRead,
    SPIFLASH_Close,
    SPIFLASH_Config,
    /*start added by victorxu*/
    NULL
    /*end added by victorxu*/
};
#endif

PUBLIC uint32 SPIFLASH_GetFlashTotalSize(void)
{
#ifdef MULTI_FLASH_SUPPORT
#ifdef CHIP_VER_UWS6121E
     return (MAX_HW_FLASH_SIZE + PHY_FLASH_SIZE_B);
#else
     return (MAX_HW_FLASH_SIZE);
#endif
#else
    return (MAX_HW_FLASH_SIZE);
#endif
}

#pragma arm section code

#pragma arm section code = "SPIFLASH_INIT_IN_FLASH"
#ifdef PLATFORM_SC6530
PUBLIC uint32 SPIFLASH_SetCS1StartAddr(void)
{
#ifdef MODEM_TYPE_ZBX
    //for ZBX project's requirement
    SFCDRV_SetCS1StartAddr(0x1000000);
#else
    SPIFLASH_TYPE_CONFIG_T* s_spiflash_type_cfg_ptr = NULL;

    s_spiflash_type_cfg_ptr = (SPIFLASH_TYPE_CONFIG_T*)SPIFLASH_GetFlashType();

    if(s_spiflash_type_cfg_ptr == NULL)
    {
        return SCI_FALSE;
    }

    SFCDRV_SetCS1StartAddr(s_spiflash_type_cfg_ptr->cs0_desity);
#endif
    return SCI_TRUE;
}
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
PUBLIC uint32 SPIFLASH_SetCS0SpaceSize(void)
{

    SPIFLASH_TYPE_CONFIG_T* s_spiflash_type_cfg_ptr = NULL;
    extern uint32 SFCDRV_SetCS0SpaceSize(uint32 flash_size);

    s_spiflash_type_cfg_ptr = (SPIFLASH_TYPE_CONFIG_T*)SPIFLASH_GetFlashType();

    if(s_spiflash_type_cfg_ptr == NULL)
    {
        return SCI_FALSE;
    }

    SFCDRV_SetCS0SpaceSize(s_spiflash_type_cfg_ptr->cs0_desity);

    return SCI_TRUE;
}
#endif
#pragma arm section code
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

/*      End flash.c       */

