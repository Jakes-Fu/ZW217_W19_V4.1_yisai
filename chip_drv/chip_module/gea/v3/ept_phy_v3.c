/******************************************************************************
 ** File Name:      ept_drv_v3.c                                                 *
 ** Author:                                             *
 ** DATE:                                                           *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the operation about memory interface.   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                                     *
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "dma_drv_internal.h"
#include "ept_drvapi.h"

#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define GEA_OS_THREADx

#ifdef GEA_OS_THREADx
#define GEA_EVENT_TIME_OUT 500

#define GEA_GetMutex(mutex)  {\
        ret = SCI_ERROR;\
        if( NULL == mutex )\
        {\
            SCI_PASSERT(0,("Mutex is null!"));/*assert verified*/\
        }\
        if(SCI_InThreadContext())\
        {\
            ret = SCI_GetMutex(mutex, SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);\
        }\
        SCI_ASSERT( ret == SCI_SUCCESS );/*assert verified*/\
    }

#define GEA_PutMutex(mutex)  {\
        ret = SCI_ERROR;\
        if( NULL == mutex )\
        {\
            SCI_PASSERT(0,("Mutex is null!"));/*assert verified*/\
        }\
        if(SCI_InThreadContext())\
        {\
            ret = SCI_PutMutex( mutex );\
        }\
        SCI_ASSERT( ret == SCI_SUCCESS );/*assert verified*/\
    }
#endif

#define MODE_KSMI           0
#define MODE_F8             1
#define MODE_F9             2
#define MODE_GEA3           3
#define MODE_GEA1           5
#define MODE_GEA2           6

#define GEA_RESET           BIT_31
#define GEA_NEXT            BIT_5
#define GEA1_DIR_BIT        15
#define GEA2_DIR_BIT        15
#define GEA3_DIR_BIT        0
#define F8_DIR_BIT          0
#define F8_BEARER_BIT       1
#define GEA_NOT_COMPLETE    0
#define GEA_COMPLETE        1

#define GEA_BUF_SIZE        (512*4)    // 512*4 bytes
#define KSM_INPUT_LEN       8          // byte
#define REG_LEN_PER_GRP     (6*4)      // register length per-group for f8: 6*uint32
#define GEA1_2_MAX_LEN      1600
#define GEA3_MAX_LEN        65536
#define F8_MAX_LEN          (20000/8)
#define F9_MAX_LEN          65536

#define GEA_DMA_EN          BIT_6//NULL//

#define GEA_RX_DMA_UID      9
#define GEA_TX_DMA_UID      10

typedef struct GEA_REG_CFG_TAG    //8d00_0000
{
    volatile uint32 ctrl;        //- 0
    volatile uint32 data_size;    //- 4
    volatile uint32 count;        //- 8
    volatile uint32 fresh;        //- c
    volatile uint32 kc0;        //-10
    volatile uint32 kc1;        //-14
    volatile uint32 kc2;        //-18
    volatile uint32 kc3;        //-1c
    volatile uint32 poll;        //-20
    volatile uint32 reserve1;    //-24
    volatile uint32 kasmi_out1;    //-28
    volatile uint32 kasmi_out2;    //-2c
    volatile uint32 grp_start;    //-30
    volatile uint32 switch_ctr;       //-34
    volatile uint32 reserve3;    //-38
    volatile uint32 reserve4;    //-3c
    volatile uint32 memory;        //-40 (40-840 total 512*32bytes)
} GEA_REG_CFG_T;

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/
LOCAL void __EPTHisrFunc (uint32 int_count, void *d);

// these 3 variable used in isr
LOCAL volatile uint32 s_remain_size = NULL; // bytes
LOCAL volatile uint32 s_cur_size    = NULL;
LOCAL volatile uint32 s_mode        = NULL;
LOCAL volatile uint8 *s_stream      = PNULL;
LOCAL volatile uint8 *s_stream_out  = PNULL;
LOCAL volatile uint8 *s_stream_reg  = PNULL;
LOCAL volatile uint32 s_f9_len      = NULL; // bit
LOCAL volatile uint32 s_f9_dir      = NULL; // bit
LOCAL BOOLEAN s_f9_repeat   = FALSE;
LOCAL BOOLEAN s_f9_boundary = FALSE;
LOCAL __align (4) uint8 s_f9_buf[GEA_BUF_SIZE] = {0};
LOCAL __align (4) uint8 s_f8_buf[GEA_BUF_SIZE] = {0};
LOCAL __align (4) uint8 s_final_msg[16] = {0};
LOCAL DEVICE_HANDLE s_dev_gea = SCI_NULL;

#ifdef GEA_OS_THREADx
LOCAL SCI_EVENT_GROUP_PTR s_gea_event_ptr = PNULL;
LOCAL SCI_MUTEX_PTR    s_gea_mutex_ptr = SCI_NULL;
LOCAL SCI_MUTEX_PTR gea_createmutex (void);
#else
LOCAL volatile uint32 s_gea_event = NULL;
#endif

#if (!GEA_DMA_EN)
#define GEA_MAX_LEN 65536
LOCAL uint32 s_aligned_addr[GEA_MAX_LEN];
#else
LOCAL uint32 s_aligned_addr[GEA_BUF_SIZE/4];
#endif
LOCAL uint32 s_gea_out[GEA_BUF_SIZE/4];

#ifdef GEA_OS_THREADx
/*****************************************************************************/
//  Description: Create Mutex
//    Global resource dependence:
//  Author: hanjun.liu
//    Note:
//        input:
//            none
//        output:
//            none
//        return:
//            Mutex
/*****************************************************************************/
LOCAL SCI_MUTEX_PTR gea_createmutex (void)
{
    SCI_MUTEX_PTR    mutex = SCI_NULL;
    mutex = SCI_CreateMutex ("GEASYNC MUTEX", SCI_INHERIT);
    SCI_PASSERT ( (mutex!=NULL), ("gea_createmutex:Great MUTEX fail!"));/*assert verified*/

    return mutex;
}//end of IMG_CreateMutex
#endif

LOCAL void __DMA_Config (uint32 uid,
                         uint32 src_addr,
                         uint32 des_addr,
                         uint32 tot_len)
{
    DMA_DATAWIDTH_E src_width, des_width;
    uint32          src_step, des_step;
    //uint32          i;
    DMA_CHNCFG_T    dma_ctl_normalsoft = {0};

    switch ( (src_addr&0x3) | (tot_len&0x3))
    {
        case 0:
            src_width = DMA_DATAWIDTH_WORD;
            src_step  = 4;
            break;

        case 2:
            src_width = DMA_DATAWIDTH_HALFWORD;
            src_step  = 2;
            break;

        case 1:
        case 3:
            src_width = DMA_DATAWIDTH_BYTE;
            src_step  = 1;
            break;

        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    switch ( (des_addr&0x3) | (tot_len&0x3))
    {
        case 0:
            des_width = DMA_DATAWIDTH_WORD;
            des_step  = 4;
            break;

        case 2:
            des_width = DMA_DATAWIDTH_HALFWORD;
            des_step  = 2;
            break;

        case 1:
        case 3:
            des_width = DMA_DATAWIDTH_BYTE;
            des_step  = 1;
            break;

        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    dma_ctl_normalsoft.type           = DMA_CHN_TYPE_HARD;
    dma_ctl_normalsoft.priority       = DMA_CHN_PRIORITY_0;
    dma_ctl_normalsoft.workmode       = DMA_CHN_WORKMODE_NORMAL;
    dma_ctl_normalsoft.src_datawidth  = src_width;
    dma_ctl_normalsoft.dest_datawidth = des_width;
    dma_ctl_normalsoft.src_address    = src_addr;
    dma_ctl_normalsoft.dest_address   = des_addr;
    dma_ctl_normalsoft.requestmode    = DMA_CHN_REQMODE_TRASACTION;
    dma_ctl_normalsoft.block_length   = 512;
    dma_ctl_normalsoft.total_length   = tot_len;
    dma_ctl_normalsoft.src_step       = src_step;
    dma_ctl_normalsoft.dest_step      = des_step;
    dma_ctl_normalsoft.src_blockmode  = DMA_BLOCKMODE_INCR16;
    dma_ctl_normalsoft.dest_blockmode = DMA_BLOCKMODE_INCR16;

    DMA_HAL_ChnIntConfig (uid, DMA_CHN_INT_TYPE_NONE, NULL);
    DMA_HAL_ChnConfig (uid, &dma_ctl_normalsoft);
}

LOCAL uint32 __update_size (uint32 *cur_size,
                            uint32 *remain_size,
                            volatile uint8 **cur_in_p,
                            volatile uint8 **cur_out_p)
{
    uint32 divisor;
    uint32 num;
    uint32 offset,tail,end,i;
    uint8  FinalBit[8] = {0x80, 0x40, 0x20, 0x10, 8,4,2,1};
    uint8  ZeroBit[8]  = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE};

    s_stream     += s_cur_size;
    s_stream_out += s_cur_size;

    divisor = (1 == s_mode || 2 == s_mode) ? (2) : (4);

    if (s_remain_size >= GEA_BUF_SIZE)
    {
        num = GEA_BUF_SIZE/divisor;
        s_cur_size = GEA_BUF_SIZE;
        s_remain_size -= GEA_BUF_SIZE;
    }
    else
    {
        num = ( (s_remain_size%divisor) ?
                ( (s_remain_size/divisor) +1) :
                (s_remain_size/divisor));
        s_cur_size = s_remain_size;
        s_remain_size = 0;
    }

    *cur_size    = s_cur_size;
    *remain_size = s_remain_size;
    *cur_in_p   = s_stream;
    *cur_out_p   = s_stream_out;

    if (9 == s_mode) // f9: should add dir,1 and pad 0
    {

        // Now add in the final dir and '1' bit.  The problem here
        // is if the message length happens to be n*(GEA_BUF_SIZE*8)-1 bits.
        // If so we need to process this block and then
        // create a new input block of 0x8000000000000000.

        if (s_f9_boundary)
        {
            if (s_f9_len > ( (GEA_BUF_SIZE*8)-1))
            {
                s_f9_len -= s_cur_size*8;
            }
            else
            {
                *cur_in_p = s_f9_buf;

                if (s_f9_len == ( (GEA_BUF_SIZE*8)-1))
                {
                    SCI_ASSERT (GEA_BUF_SIZE == s_cur_size);/*assert verified*/
                    s_f9_len = 0;
                    SCI_MEMCPY (s_f9_buf, (void *) s_stream, s_cur_size);

                    if (s_f9_dir)
                    {
                        s_f9_buf[GEA_BUF_SIZE-1] |= 0x1;
                    }
                    else
                    {
                        s_f9_buf[GEA_BUF_SIZE-1] &= ~0x1;
                    }
                }
                else if (s_f9_len == 0)
                {
                    s_f9_buf[0] = 0x80;
                    SCI_MEMSET (&s_f9_buf[1], 0, 7);
                }
                else
                {
                    SCI_ASSERT (0);/*assert verified*/
                }
            }
        }
        else
        {
            if (0 == s_remain_size)
            {
                *cur_in_p = s_f9_buf;
                SCI_MEMCPY (s_f9_buf, (void *) s_stream, s_cur_size);

                // add direction bit
                tail   = s_f9_len/8;     // the tail byte
                offset = s_f9_len%8;     // the position of end bit

                if (s_f9_dir)
                {
                    s_f9_buf[tail] |= FinalBit[offset];
                }
                else
                {
                    s_f9_buf[tail] &= ~ (FinalBit[offset]);
                }

                // add 1
                tail   = (s_f9_len+1) /8;    // the tail byte
                offset = (s_f9_len+1) %8;    // the position of end bit
                s_f9_buf[tail] |= FinalBit[offset];

                // pad 0 if needed
                if ( (s_f9_len+2) %64)
                {
                    tail   = (s_f9_len+2) /8;    // the tail byte
                    offset = (s_f9_len+2) %8;    // the position of end bit
                    s_f9_buf[tail] &= ZeroBit[offset];

                    end    = (s_f9_len+2) /64 + 1;
                    end    = end*64/8;

                    for (i=tail+1; i<end; i++)
                    {
                        s_f9_buf[i] = 0;
                    }
                }

            }
            else
            {
                s_f9_len -= s_cur_size*8;
            }
        }
    }

    return num;
}

LOCAL ISR_EXE_T __IrqHandle (uint32 irqnum)
{
    volatile GEA_REG_CFG_T *gea_cfg = (GEA_REG_CFG_T *) ( (volatile uint32 *) GEA_BASE);
    // clear polling register
    gea_cfg->poll = 0;
    return CALL_HISR;
}

/*****************************************************************************/
//  Description: Hisr function
//  Author: Mingwei.Zhang
//    Note:
//        input:
//            none
//        output:
//            none
//        return:
/*****************************************************************************/

LOCAL void __EPTHisrFunc (uint32 int_count, void *d)
{
    volatile GEA_REG_CFG_T *gea_cfg = (GEA_REG_CFG_T *) ( (volatile uint32 *) GEA_BASE);
    volatile uint32 temp_reg;
    uint32 i;
    uint32 num;
    uint32 tmp_val;
    uint32 cur_size         = s_cur_size;
    uint32 remain_size      = s_remain_size;
    uint32 mode             = s_mode;
    volatile uint8 *src_buf = PNULL;
    volatile uint8 *des_buf = PNULL;  // src & des is to chip and DMA
    volatile uint8 *out_buf = PNULL;  // in & out is to driver and upper level
    DMA_CAHCECONFIG_T cacheConfig[1];

    if (0xff == s_mode) // for group f8
    {
#ifdef GEA_OS_THREADx
        SCI_SetEvent (s_gea_event_ptr, GEA_COMPLETE, SCI_OR);
#else
        s_gea_event = GEA_COMPLETE;
#endif

        return ;
    }

    // get stream
    src_buf = (uint8 *) (& (gea_cfg->memory));

    if (4 == s_mode || 9 == s_mode)
    {
        des_buf = s_stream_reg;
    }
    else
    {
        des_buf = s_stream_out;
    }

    switch (mode)
    {
        case 1:// GEA I
        case 2:// GEA II
            SCI_MEMCPY (s_gea_out, (void *) src_buf, (cur_size+3) /4*4);
            break;

        case 3:// GEA III
        case 8:// F8
#if (!GEA_DMA_EN)
            SCI_MEMCPY (des_buf, src_buf, cur_size);
#else
#endif
            break;

        case 4://kasumi
            tmp_val = (gea_cfg->kasmi_out2);

            for (i=0; i<4; i++)
            {
                *des_buf++ = (tmp_val>> (24-i*8)) &0xff;
            }

            tmp_val = (gea_cfg->kasmi_out1);

            for (i=0; i<4; i++)
            {
                *des_buf++ = (tmp_val>> (24-i*8)) &0xff;
            }

            break;

        case 9:

            if (0 == remain_size && !s_f9_repeat)
            {
                tmp_val = (gea_cfg->kasmi_out2);

                for (i=0; i<4; i++)
                {
                    *des_buf++ = (tmp_val>> (24-i*8)) &0xff;
                }
            }

            break;

        default:
            break;
    }

    // input data and start next
    if (0 != remain_size)
    {
        SCI_ASSERT (3 == mode || 8 == mode || 9 == mode);/*assert verified*/
        des_buf = (uint8 *) (& (gea_cfg->memory));
        num = __update_size (&cur_size, &remain_size, &src_buf, &out_buf);

        if (3 == mode)
        {
            SCI_MEMSET ( (uint16 *) src_buf, NULL, cur_size); //clear memory
        }

#if (!GEA_DMA_EN)
        SCI_MEMCPY (des_buf, src_buf, num*4);
#else
        __DMA_Config (GEA_RX_DMA_UID, (uint32) src_buf, (uint32) des_buf, num*4);
        {
            cacheConfig[0].bufferAddr = (uint32) src_buf;
            cacheConfig[0].bufferSize = num*4;
            cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
            DMA_HAL_ChnEnable_Ex (GEA_RX_DMA_UID, SCI_TRUE, cacheConfig, 1);
            //        DMA_HAL_ChnEnable (GEA_RX_DMA_UID,TRUE);
        }

        if (3 == mode || 8 == mode)
        {
            __DMA_Config (GEA_TX_DMA_UID, (uint32) des_buf, (uint32) out_buf, cur_size);
            {
                cacheConfig[0].bufferAddr = (uint32) out_buf;
                cacheConfig[0].bufferSize = cur_size;
                cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
                DMA_HAL_ChnEnable_Ex (GEA_TX_DMA_UID, SCI_TRUE, cacheConfig, 1);
                //            DMA_HAL_ChnEnable (GEA_TX_DMA_UID,TRUE);
            }

        }

#endif
        gea_cfg->data_size = num&0x3ff;
        //start next
        temp_reg = gea_cfg->ctrl;        // get ctr register
        temp_reg |= GEA_NEXT;
        gea_cfg->ctrl = temp_reg;        // next start
    }
    else                                // last time
    {
        if (9 == mode && s_f9_repeat)
        {
            s_f9_repeat = FALSE;
            gea_cfg->data_size = 0;
            //start next
            temp_reg = gea_cfg->ctrl;        // get ctr register
            temp_reg = MODE_F9|GEA_NEXT;
            gea_cfg->ctrl = temp_reg;        // next start
        }
        else
        {
#ifdef GEA_OS_THREADx
            SCI_SetEvent (s_gea_event_ptr, GEA_COMPLETE, SCI_OR);
#else
            s_gea_event = GEA_COMPLETE;
#endif
        }
    }

    return ;
}


/*****************************************************************************/
//  Description: Create Mutex
//    Global resource dependence:
//  Author: hanjun.liu
//    Note:
//        input:
//            none
//        output:
//            none
//        return:
//            Mutex
/*****************************************************************************/
PUBLIC void gea_init (void)
{
    CLOCK_STATUS s  = CLOCK_SUCCESS;

#ifdef GEA_OS_THREADx

    if (NULL == s_gea_mutex_ptr)
    {
        s_gea_mutex_ptr = gea_createmutex();
        SCI_PASSERT (s_gea_mutex_ptr, ("gea_init: Create Mutex is fail"));/*assert verified*/
    }

    if (NULL == s_gea_event_ptr)
    {
        s_gea_event_ptr = SCI_CreateEvent ("GEA_DRV_EVENT_OUTPUT_END");
        SCI_PASSERT (s_gea_event_ptr, ("gea_init: Create Event is fail"));/*assert verified*/
    }

    SCI_SetEvent (s_gea_event_ptr, GEA_NOT_COMPLETE, SCI_AND);

#else
    s_gea_event = GEA_NOT_COMPLETE;
#endif

    ISR_RegHandler_Ex (TB_EPT_INT, (TB_ISR) __IrqHandle, __EPTHisrFunc, CHIPDRV_HISR_PRIO_2, NULL);

    s = DEVICE_Find ("GEA", &s_dev_gea);
    SCI_ASSERT (s == CLOCK_SUCCESS); /*assert verified*/

    CHIPDRV_EnableIRQINT (TB_EPT_INT);          // int enbale
}


//***************************************************************************************
//    Description: EPT cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : gea1,gea2,gea3 have 64bit key, so only used kc_0 and kc_1
//               kasumi has 128bit key, so used kc_0,kc_1,kc_2,kc_3
//      "input": gea1,gea2,gea3 have 32bit input, so only used in_0
//               kasumi has 64bit input, so used in_0 and in_1
//
//      example(gea3):
//              Kc    0xEFA8B2229E720C2A
//              INPUT 0x4BDBD5E5
//              DIRECTION 1
//              OUTPUT(16bytes):
//              0x4718A2ADFC90590949DDADAB406EC3B9
//
//              gea_stream(0x9E720C2A, 0xEFA8B222, 0, 0, 0x4BDBD5E5, 0, 1, 3, 16, stream_out);
//***************************************************************************************
uint gea_stream (uint32 kc_0,
                 uint32 kc_1,   // kc_0,kc_1 are used for gea1,gea2,gea3
                 uint32 kc_2,
                 uint32 kc_3,   // kc_0,kc_1,kc_2,kc_3 are used for kasumi
                 uint32 in_0,   // in_0 is used for gea1,gea2,gea3
                 uint32 in_1,   // in_0,in_1 are used for kasumi
                 uint32 bDir,   // direction: 0,1; kasumi-type: 0:0, 1:0x55, 2:0xaa, 3:0xff
                 uint32 mode,   // 1:gea1, 2:gea2, 3:gea3, 4:kasumi
                 uint32 len,    // in unit of byte(except f9:in unit of bit)
                 uint8 *data_in,
                 uint8 *data_out)
{
    uint32 num;
    uint32 cur_size;
    uint32 remain_size;
    uint32 ret = 1;
    uint32 actualFlag;
    uint32 km_type;
    //uint32 f9_len = len;
    volatile uint8 *src_buf = PNULL;
    volatile uint8 *des_buf = PNULL;  // src & des is to chip and DMA
    volatile uint8 *out_buf = PNULL;  // in & out is to driver and upper level
    volatile GEA_REG_CFG_T *gea_cfg = (GEA_REG_CFG_T *) ( (volatile uint32 *) GEA_BASE);

    DMA_CAHCECONFIG_T cacheConfig[1];

    switch (mode)
    {
        case EPT_MODE_GEA1:
        case EPT_MODE_GEA2:

            if (len > GEA1_2_MAX_LEN)
            {
                //SCI_TraceLow:"err: GEA1/2 length bigger than %d!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,EPT_PHY_V3_645_112_2_17_23_4_38_969,(uint8*)"d",GEA1_2_MAX_LEN);
                SCI_ASSERT (0);/*assert to do*/
                return 0; /*lint !e527 confirmed by xuepeng*/
            }

            break;
        case EPT_MODE_GEA3:

            if (len > GEA3_MAX_LEN)
            {
                //SCI_TraceLow:"err: GEA3 length bigger than %d!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,EPT_PHY_V3_655_112_2_17_23_4_38_970,(uint8*)"d",GEA3_MAX_LEN);
                SCI_ASSERT (0);/*assert to do*/
                return 0; /*lint !e527 confirmed by xuepeng*/
            }

            break;
        case EPT_MODE_KSMI:

            if (KSM_INPUT_LEN != len)
            {
                //SCI_TraceLow:"err: Kasumi length is not %d!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,EPT_PHY_V3_665_112_2_17_23_4_38_971,(uint8*)"d",KSM_INPUT_LEN);
                SCI_ASSERT (0);/*assert to do*/
                return 0; /*lint !e527 confirmed by xuepeng*/
            }

            break;
        case EPT_MODE_F8:

            if (len > F8_MAX_LEN)
            {
                //SCI_TraceLow:"err: F8 length bigger than %d!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,EPT_PHY_V3_675_112_2_17_23_4_38_972,(uint8*)"d",F8_MAX_LEN);
                SCI_ASSERT (0);/*assert to do*/
                return 0; /*lint !e527 confirmed by xuepeng*/
            }

            break;
        case EPT_MODE_F9:

            if (len > F9_MAX_LEN)
            {
                //SCI_TraceLow:"err: F9 length bigger than %d!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,EPT_PHY_V3_685_112_2_17_23_4_38_973,(uint8*)"d",F9_MAX_LEN);
                SCI_ASSERT (0);/*assert to do*/
                return 0; /*lint !e527 confirmed by xuepeng*/
            }

            break;
        default:
            break;

    }

#ifdef GEA_OS_THREADx
    GEA_GetMutex (s_gea_mutex_ptr);
#endif

    if (9 == mode)
    {
        s_f9_dir = bDir&0x1;
        s_f9_len = len;

        if ( ( (GEA_BUF_SIZE*8)-1) == (s_f9_len% (GEA_BUF_SIZE*8)))
        {
            s_f9_boundary = TRUE;
        }
        else
        {
            s_f9_boundary = FALSE;
        }

        len  = (len+2+63) /64;   // in unit of 64bits
        len *= 64;               // bit
        len /= 8;                // in unit of byte

        if (0 == (len%GEA_BUF_SIZE))
        {
            s_f9_repeat = TRUE;
        }
        else
        {
            s_f9_repeat = FALSE;
        }
    }


    gea_cfg->ctrl = GEA_RESET; //reset

#if !defined(CHIP_ENDIAN_LITTLE)
    gea_cfg->switch_ctr = 1;       //data switch for endian issue
#endif

    s_mode   = mode;
    s_remain_size = len;
    s_cur_size    = 0;
    s_stream_out  = data_out;
    s_stream_reg  = data_out;
#if (!GEA_DMA_EN)

    if ( ( (uint32) data_in) %4)
    {
        SCI_MEMCPY (s_aligned_addr, data_in, s_remain_size);
        s_stream = (uint8 *) s_aligned_addr;
    }
    else
    {
        s_stream = data_in;
    }

#else

    if ( ( (uint32) data_in) %4 && (1 == mode || 2 == mode))
    {
        SCI_MEMCPY (s_aligned_addr, data_in, s_remain_size);
        s_stream = (uint8 *) s_aligned_addr;
    }
    else
    {
        s_stream = data_in;
    }

#endif

    num     = __update_size (&cur_size, &remain_size, &src_buf, &out_buf);
    des_buf = (uint8 *) (& (gea_cfg->memory));

    switch (mode)
    {
        case 1:
            // GEA I
            SCI_MEMSET ( (uint16 *) des_buf, NULL, num*4);
            gea_cfg->count = ( (num&0x7fff) | ( (bDir&0x1) <<GEA1_DIR_BIT)); //set direction and data size
            gea_cfg->fresh = in_0;                  //set 32bit input
            gea_cfg->kc0   = kc_0;
            gea_cfg->kc1   = kc_1;                  //set 64bit key
            gea_cfg->ctrl  = (MODE_GEA1);           //start gea1
            break;
        case 2:
            // GEA II
            SCI_MEMSET ( (uint16 *) des_buf, NULL, num*4);
            gea_cfg->count = ( (num&0x7fff) | ( (bDir&0x1) <<GEA2_DIR_BIT)); //set direction and data size
            gea_cfg->fresh = in_0;                  //set 32bit input
            gea_cfg->kc0   = kc_0;
            gea_cfg->kc1   = kc_1;                  //set 64bit key
            gea_cfg->ctrl  = (MODE_GEA2);           //start gea2
            break;
        case 3:
            // GEA III
#if (!GEA_DMA_EN)
            SCI_MEMSET ( (uint16 *) des_buf, NULL, num*4);
#else
            SCI_MEMSET ( (uint16 *) src_buf, NULL, cur_size);
            __DMA_Config (GEA_RX_DMA_UID, (uint32) src_buf, (uint32) des_buf, num*4);
            {
                cacheConfig[0].bufferAddr = (uint32) src_buf;
                cacheConfig[0].bufferSize = num*4;
                cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
                DMA_HAL_ChnEnable_Ex (GEA_RX_DMA_UID, SCI_TRUE, cacheConfig, 1);
                //           DMA_HAL_ChnEnable (GEA_RX_DMA_UID,TRUE);
            }

            __DMA_Config (GEA_TX_DMA_UID, (uint32) des_buf, (uint32) out_buf, cur_size);
            {
                cacheConfig[0].bufferAddr = (uint32) out_buf;
                cacheConfig[0].bufferSize = cur_size;
                cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
                DMA_HAL_ChnEnable_Ex (GEA_TX_DMA_UID, SCI_TRUE, cacheConfig, 1);
                //            DMA_HAL_ChnEnable (GEA_TX_DMA_UID,TRUE);
            }
#endif
            gea_cfg->data_size = num&0x3ff;         //set block data size,in unit of 32bits
            gea_cfg->count = in_0;                  //set 32bit input
            gea_cfg->fresh = ( (bDir&0x1) <<GEA3_DIR_BIT); //set direction
            gea_cfg->kc0   = kc_0;
            gea_cfg->kc1   = kc_1;                  //set 64bit key
            gea_cfg->ctrl  = (MODE_GEA3|GEA_DMA_EN);//start gea3
            break;
        case 4:
            //kasumi
            SCI_MEMSET ( (uint16 *) des_buf, NULL, cur_size);
            km_type = bDir;                         // 0:0, 1:0x55, 2:0xaa, 3:0xff
            gea_cfg->count = in_1;
            gea_cfg->fresh = in_0;                  //set 64bit input
            gea_cfg->kc0   = kc_0;
            gea_cfg->kc1   = kc_1;
            gea_cfg->kc2   = kc_2;
            gea_cfg->kc3   = kc_3;                  //set 128bit key
            gea_cfg->ctrl  = (MODE_KSMI| ( (km_type&0x3) <<3));          //start kasumi
            break;
        case 8:
            // F8
#if (!GEA_DMA_EN)
            SCI_MEMCPY (des_buf, src_buf, num*4);
#else
            __DMA_Config (GEA_RX_DMA_UID, (uint32) src_buf, (uint32) des_buf, num*4);
            {
                cacheConfig[0].bufferAddr = (uint32) src_buf;
                cacheConfig[0].bufferSize = num*4;
                cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
                DMA_HAL_ChnEnable_Ex (GEA_RX_DMA_UID, SCI_TRUE, cacheConfig, 1);
                //            DMA_HAL_ChnEnable (GEA_RX_DMA_UID,TRUE);
            }
            __DMA_Config (GEA_TX_DMA_UID, (uint32) des_buf, (uint32) out_buf, cur_size);
            {
                cacheConfig[0].bufferAddr = (uint32) out_buf;
                cacheConfig[0].bufferSize = cur_size;
                cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
                DMA_HAL_ChnEnable_Ex (GEA_TX_DMA_UID, SCI_TRUE, cacheConfig, 1);
                //            DMA_HAL_ChnEnable (GEA_TX_DMA_UID,TRUE);
            }
#endif
            gea_cfg->data_size = num&0x3ff;         //set block data size,in unit of 32bits
            gea_cfg->count = in_0;                  //set 32bit input
            gea_cfg->fresh = ( ( (bDir&0x1) <<F8_DIR_BIT) | ( (in_1&0x1f) <<F8_BEARER_BIT)); //set direction
            gea_cfg->kc0   = kc_0;
            gea_cfg->kc1   = kc_1;
            gea_cfg->kc2   = kc_2;
            gea_cfg->kc3   = kc_3;                  //set 128bit key
            gea_cfg->ctrl  = (MODE_F8|GEA_DMA_EN);  //start F8
            break;
        case 9:
            // F9
#if (!GEA_DMA_EN)
            SCI_MEMCPY (des_buf, src_buf, num*4);
#else
            __DMA_Config (GEA_RX_DMA_UID, (uint32) src_buf, (uint32) des_buf, num*4);
            {
                cacheConfig[0].bufferAddr = (uint32) src_buf;
                cacheConfig[0].bufferSize = num*4;
                cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
                DMA_HAL_ChnEnable_Ex (GEA_RX_DMA_UID, SCI_TRUE, cacheConfig, 1);
                //            DMA_HAL_ChnEnable (GEA_RX_DMA_UID,TRUE);
            }
#endif
            gea_cfg->data_size = num&0x3ff;         //set block data size,in unit of 32bits
            gea_cfg->count = in_0;                  //set 32bit input
            gea_cfg->fresh = in_1;  //set direction
            gea_cfg->kc0   = kc_0;
            gea_cfg->kc1   = kc_1;
            gea_cfg->kc2   = kc_2;
            gea_cfg->kc3   = kc_3;                  //set 128bit key
            gea_cfg->ctrl  = (MODE_F9|GEA_DMA_EN);  //start F9
            break;
        default:
            break;
    }

#ifdef GEA_OS_THREADx

    if (SCI_ERROR == SCI_GetEvent (s_gea_event_ptr, GEA_COMPLETE, SCI_AND_CLEAR,
                                   &actualFlag, GEA_EVENT_TIME_OUT))
    {
        gea_cfg->ctrl = GEA_RESET; //reset
        //SCI_TRACE_LOW:"GEA Driver wait timeout:%ums!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,EPT_PHY_V3_896_112_2_17_23_4_38_974,(uint8*)"u",GEA_EVENT_TIME_OUT);
        SCI_ASSERT (0);/*assert to do*/
        ret = 0; /*lint !e527 confirmed by xuepeng*/
    }

#else

    while (GEA_COMPLETE != s_gea_event)
    {
        //SCI_TraceLow("wait gea complete...");
    }

    s_gea_event = GEA_NOT_COMPLETE;
#endif

#if (!GEA_DMA_EN)
#else
    DMA_HAL_ChnEnable (GEA_RX_DMA_UID,FALSE);
    DMA_HAL_ChnEnable (GEA_TX_DMA_UID,FALSE);
#endif

    if (1 == s_mode || 2 == s_mode)
    {
        uint32 i;
        uint32 *tmp_buf = s_gea_out;

        src_buf = (uint8 *) s_gea_out;
        des_buf = s_stream_out;

        for (i = 0; i < (len+3) /4; i++)
        {
            *tmp_buf ++ = ( ( (*tmp_buf) <<16) &0xffff0000) | ( ( (*tmp_buf) >>16) &0x0000ffff); /*lint !e564*/
        }

        SCI_MEMCPY ( (uint16 *) des_buf, (void *) src_buf, len);
    }

#ifdef GEA_OS_THREADx
    GEA_PutMutex (s_gea_mutex_ptr);
#endif

    return ret;
}

uint __ept_f8_grp (EPT_F8_GRP_T *start_ptr, uint32 num)
{
    EPT_F8_GRP_T *grp_ptr = start_ptr;
    uint32 actualFlag;
    uint8 *des_buf;
    uint32 *reg_ptr;
    uint32 len, i, total_num;
    uint8 *key_ptr;
    volatile GEA_REG_CFG_T *gea_cfg = (GEA_REG_CFG_T *) ( (volatile uint32 *) GEA_BASE);
    int ret = 1;

#ifdef GEA_OS_THREADx
    GEA_GetMutex (s_gea_mutex_ptr);
#endif

    gea_cfg->ctrl = GEA_RESET; //reset

#if !defined(CHIP_ENDIAN_LITTLE)
    gea_cfg->switch_ctr = 1;       //data switch for endian issue
#endif

    s_mode = 0xff;

    total_num = num;
    des_buf = (uint8 *) (& (gea_cfg->memory));

    do
    {
        len = (grp_ptr->len+3) /4*4;
        //if(len < 1024)
        //{
        SCI_MEMCPY (des_buf, grp_ptr->data_in, len);
        //}
        //else
        //{
        //    DMA_memcpy(des_buf, grp_ptr->data_in, len);
        //}
        des_buf += (grp_ptr->len+3) /4*4;
        grp_ptr = grp_ptr->next_grp;
    }
    while (--total_num > 0);

    total_num = num;
    grp_ptr = start_ptr;
    reg_ptr = (uint32 *) des_buf;

    do
    {
        len = (grp_ptr->len+3) /4;
        *reg_ptr++ = ( (grp_ptr->bearer&0x1f) <<17) | ( (grp_ptr->dir&0x01) <<16) | (len);
        *reg_ptr++ = grp_ptr->count;
        key_ptr = & (grp_ptr->key[12]);

        for (i=0; i<4; i++)
        {
            *reg_ptr++ = ( (*key_ptr) <<24) | (* (key_ptr+1) <<16) | (* (key_ptr+2) <<8) | (* (key_ptr+3));
            key_ptr -= 4;
        }

        grp_ptr = grp_ptr->next_grp;
    }
    while (--total_num > 0);

    *reg_ptr = 0xffffffff;

    gea_cfg->grp_start = (uint32) des_buf;

#ifdef GEA_OS_THREADx

    if (SCI_ERROR == SCI_GetEvent (s_gea_event_ptr, GEA_COMPLETE, SCI_AND_CLEAR,
                                   &actualFlag, GEA_EVENT_TIME_OUT))
    {
        gea_cfg->ctrl = GEA_RESET; //reset
        //SCI_TRACE_LOW:"GEA Driver wait timeout:%ums!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,EPT_PHY_V3_1013_112_2_17_23_4_38_975,(uint8*)"u",GEA_EVENT_TIME_OUT);
        SCI_ASSERT (0);/*assert to do*/
        ret = 0; /*lint !e527 confirmed by xuepeng*/
    }

#else

    while (GEA_COMPLETE != s_gea_event)
    {
        //SCI_TraceLow("wait gea complete...");
    }

    s_gea_event = GEA_NOT_COMPLETE;
#endif

    total_num = num;
    grp_ptr = start_ptr;
    des_buf = (uint8 *) (& (gea_cfg->memory));

    do
    {
        len = (grp_ptr->len+3) /4*4;
        //if(grp_ptr->len < 1024)
        //{
        SCI_MEMCPY (grp_ptr->data_out, des_buf, grp_ptr->len);
        //}
        //else
        //{
        //    DMA_memcpy(grp_ptr->data_out, des_buf, grp_ptr->len);
        //}
        des_buf += len;
        grp_ptr = grp_ptr->next_grp;
    }
    while (--total_num > 0);


#ifdef GEA_OS_THREADx
    GEA_PutMutex (s_gea_mutex_ptr);
#endif

    return ret;
}

uint __ept_f8_single (EPT_F8_GRP_T *start_ptr)
{
    uint8  *kc;
    uint32 kc_word[4];
    uint32 i;

    kc = & (start_ptr->key[12]);

    for (i=0; i<4; i++)
    {
        kc_word[i] = ( (*kc) <<24) | (* (kc+1) <<16) | (* (kc+2) <<8) | (* (kc+3));
        kc -= 4;
    }

    return gea_stream (kc_word[0],
                       kc_word[1],
                       kc_word[2],
                       kc_word[3],
                       start_ptr->count,
                       start_ptr->bearer,
                       start_ptr->dir,
                       8,
                       start_ptr->len,
                       start_ptr->data_in,
                       start_ptr->data_out);
}
//***************************************************************************************
//    Description: GEA cipher function for GEA1/2
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : gea1,gea2 have 64bit key
//
//      "input": gea1,gea2 have 32bit input
//
//      example(gea1):
//              Kc    0xac245504a30f9b3e
//              INPUT 0xb208858a
//              DIRECTION 0
//              OUTPUT(16bytes):
//              0xe0,0xf8,0x03,0x7a,0x8d,0x40,0x85,0x26,0x5f,0x1c,0x27,0x50,0x6e,0x28,0xdb,0x63
//
//              gea_get_stream(kc, in_0, 0, 1, 8, stream_out);
//***************************************************************************************
PUBLIC uint gea_get_stream (const uint8 key[8],  // bit00~63 of parameter kc
                            uint32 in_0,    // bit00~31 of parameter input
                            uint32 bDir,    // direction: 0,1
                            uint32 mode,    // 1:gea1, 2:gea2, 3:gea3, 4:not supported
                            uint32 len,     // in unit of 2bytes
                            uint8 *stream)  // output stream pointer
{
    uint8  *kc;
    uint32 kc_word[2];
    uint32 i;

    if (mode > 3 || mode < 1)   // for only support gea1 - gea3
    {
        SCI_ASSERT (0);/*assert to do*/
        return 0; /*lint !e527 confirmed by xuepeng*/
    }

    kc = (uint8 *) (&key[4]);

    for (i=0; i<2; i++)
    {
        kc_word[i] = ( (*kc) <<24) | (* (kc+1) <<16) | (* (kc+2) <<8) | (* (kc+3));
        kc -= 4;
    }

    return gea_stream (kc_word[0],
                       kc_word[1],   // kc_0,kc_1 are used for gea1,gea2,gea3
                       0,
                       0,      // kc_2,kc_3 are not used
                       in_0,   // in_0 is used for gea1,gea2,gea3
                       0,      // in_1 are not used
                       bDir,   // direction: 0,1
                       mode,   // 1:gea1, 2:gea2, 3:gea3
                       (len*2),
                       stream,
                       stream);
}

//***************************************************************************************
//    Description: GEA3 cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : gea3 have 64bit key
//
//      "input": gea3 have 32bit input
//
//      example(gea3):
//              Kc    0xEFA8B2229E720C2A
//              INPUT 0x4BDBD5E5
//              DIRECTION 1
//              OUTPUT(16bytes):
//              0x4718A2ADFC90590949DDADAB406EC3B9
//
//              gea3_get_stream(kc, 0x4BDBD5E5, 1, 16, stream_out);
//***************************************************************************************
PUBLIC uint gea3_get_stream (const uint8 key[8],  // bit00~63 of parameter kc
                             uint32 in_0,    // bit00~31 of parameter input
                             uint32 bDir,    // direction: 0,1
                             uint32 len,     // in unit of byte
                             uint8 *stream)  // output stream pointer
{
    uint8  *kc;
    uint32 kc_word[2];
    uint32 i;

    kc = (uint8 *) (&key[4]);

    for (i=0; i<2; i++)
    {
        kc_word[i] = ( (*kc) <<24) | (* (kc+1) <<16) | (* (kc+2) <<8) | (* (kc+3));
        kc -= 4;
    }

    return gea_stream (kc_word[0],
                       kc_word[1],   // kc_0,kc_1 are used for gea1,gea2,gea3
                       0,
                       0,      // kc_2,kc_3 are not used
                       in_0,   // in_0 is used for gea1,gea2,gea3
                       0,      // in_1 are not used
                       bDir,   // direction: 0,1
                       3,      // 1:gea1, 2:gea2, 3:gea3
                       len,
                       stream,
                       stream);
}

//***************************************************************************************
//    Description: kasumi cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : kasumi have 128bit key
//
//      "input": kasumi have 64bit input
//
//***************************************************************************************
PUBLIC uint kasumi_get_stream (const uint8 key[16],  // bit00~128 of parameter kc
                               const uint8 in[8],   // bit00~64 of parameter input
                               uint32 bDir,    // direction: 0,1
                               uint8 stream[8])  // 8byte value, no endian issue
{
    uint8  *kc, *input;
    uint32 kc_word[4], in_word[2];
    uint32 i;

    kc = (uint8 *) (&key[12]);

    for (i=0; i<4; i++)
    {
        kc_word[i] = ( (*kc) <<24) | (* (kc+1) <<16) | (* (kc+2) <<8) | (* (kc+3));
        kc -= 4;
    }

    input = (uint8 *) (&in[4]);

    for (i=0; i<2; i++)
    {
        in_word[i] = ( (*input) <<24) | (* (input+1) <<16) | (* (input+2) <<8) | (* (input+3));
        input -= 4;
    }

    return gea_stream (kc_word[0],
                       kc_word[1],
                       kc_word[2],
                       kc_word[3],
                       in_word[0],
                       in_word[1],
                       bDir,
                       4,
                       KSM_INPUT_LEN,
                       stream,
                       stream);
}

//***************************************************************************************
//    Description: F8 cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : F8 have 128bit key
//
//      "input": F8 have 64bit input
//
//***************************************************************************************
PUBLIC void F8_get_stream (EPT_F8_GRP_T *grp_head)
{
    uint32 grp_len, grp_num;
    EPT_F8_GRP_T *grp_ptr, *cur_ptr, *start_ptr;
    //volatile GEA_REG_CFG_T *gea_cfg = (GEA_REG_CFG_T *) ( (volatile uint32 *) GEA_BASE);

    SCI_ASSERT (PNULL != grp_head);/*assert verified*/
    grp_len = 0;
    grp_num = 0;
    grp_ptr   = grp_head;
    start_ptr = grp_head;

    do
    {
        SCI_ASSERT (grp_ptr->len > 0);/*assert verified*/
        cur_ptr = grp_ptr;
        grp_len += (cur_ptr->len+3) /4*4 + REG_LEN_PER_GRP; // data_len + reg_len
        grp_num ++;

        if (grp_len > (GEA_BUF_SIZE - 4))   // not contain 0xffffffff
        {
            if (grp_num > 2)                // f8 group except the last one
            {
                __ept_f8_grp (start_ptr,grp_num-1);
                start_ptr = cur_ptr;
                grp_len   = (cur_ptr->len+3) /4*4 + REG_LEN_PER_GRP;
                grp_num   = 1;
            }
            else if (grp_num == 2)          // f8 single except the last one
            {
                __ept_f8_single (start_ptr);
                start_ptr = cur_ptr;
                grp_len   = (cur_ptr->len+3) /4*4 + REG_LEN_PER_GRP;
                grp_num   = 1;
            }
            else                            // f8 single
            {
                __ept_f8_single (start_ptr);
                start_ptr = cur_ptr->next_grp;
                grp_len   = 0;
                grp_num   = 0;
            }
        }
    }
    while ( (grp_ptr = grp_ptr->next_grp) !=NULL);

    // if grp_num == 0 , do nothing
    if (1 == grp_num)
    {
        __ept_f8_single (cur_ptr);
    }
    else if (grp_num > 1)
    {
        __ept_f8_grp (start_ptr,grp_num);
    }
}

//***************************************************************************************
//    Description: F9 cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : F9 have 128bit key
//
//
//
//***************************************************************************************
PUBLIC uint F9_get_stream (const uint8 key[16],  // bit00~128 of parameter kc
                           uint32 count,
                           uint32 fresh,
                           uint32 bDir,    // direction: 0,1
                           uint32 len,     // in unit of byte
                           uint8 *stream,  // in stream pointer
                           uint8  mac[4])  // 4byte value, no endian issue
{
    uint8  *kc;
    uint32 kc_word[4];
    uint32 i;

    kc = (uint8 *) (&key[12]);

    for (i=0; i<4; i++)
    {
        kc_word[i] = ( (*kc) <<24) | (* (kc+1) <<16) | (* (kc+2) <<8) | (* (kc+3));
        kc -= 4;
    }

    return gea_stream (kc_word[0],
                       kc_word[1],
                       kc_word[2],
                       kc_word[3],
                       count,
                       fresh,
                       bDir,
                       9,
                       len*8,
                       stream,
                       mac);
}

#ifdef   __cplusplus
}
#endif

