/******************************************************************************
 ** File Name:      gea_phy_v5.c                                              *
 ** Author:                                                                   *
 ** DATE:                                                                     *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/17/2010          Create.                                               *
 ******************************************************************************/


#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "gea_reg_v5.h"
#include "clock_drvapi.h"

#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#define GEA_EVENT_TIME_OUT 500

#define GEA_GetMutex(mutex)  {\
        uint32 ret = SCI_ERROR;\
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
        uint32 ret = SCI_ERROR;\
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

#define GEA_DMA_WAIT_TIME   1000


/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/
// these 3 variable used in isr
LOCAL volatile uint32 s_num    = NULL;
LOCAL volatile uint32 s_mode   = NULL;
LOCAL volatile uint8 *s_stream = PNULL;
LOCAL volatile GEA_REG_CFG_T *gea_cfg = (GEA_REG_CFG_T *) ( (volatile uint32 *) GEA_CTRL);
LOCAL SCI_EVENT_GROUP_PTR s_gea_event_ptr = PNULL;
// gea device handle
LOCAL DEVICE_HANDLE s_dev_gea = SCI_NULL;

LOCAL ISR_EXE_T _IrqHandle (uint32 irqnum);
LOCAL SCI_MUTEX_PTR s_gea_mutex_ptr = SCI_NULL;
LOCAL void _GEAHisrFunc (uint32 int_count, void *d);


uint gea_stream (uint32 kc_0,
                 uint32 kc_1,   // kc_0,kc_1 are used for gea1,gea2,gea3
                 uint32 kc_2,
                 uint32 kc_3,   // kc_0,kc_1,kc_2,kc_3 are used for kasumi
                 uint32 in_0,   // in_0 is used for gea1,gea2,gea3
                 uint32 in_1,   // in_0,in_1 are used for kasumi
                 uint32 bDir,   // direction: 0,1
                 uint32 mode,   // 1:gea1, 2:gea2, 3:gea3, 4:kasumi
                 uint32 len,    // in unit of byte
                 uint8 *stream);

/*****************************************************************************/
//  Description: Create Mutex
//  Global resource dependence:
//  Author: hanjun.liu
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          Mutex
/*****************************************************************************/
LOCAL SCI_MUTEX_PTR gea_createmutex (void);

void GEA_DMA_SetRunNumber( uint32 numbers )  
{
    *(volatile uint32*)(GEA_DMA_REG) &= ~GEA_DMA_RUNNUMBER_MASK; 
    *(volatile uint32*)(GEA_DMA_REG) |=  ( numbers<<GEA_DMA_RUNNUMBER_SHIFT);
}

void GEA_DmaEnable( )
{
    *(volatile uint32*)(GEA_DMA_REG) |= GEA_DMA_MODE_EN;
}

void GEA_DmaDisable()
{
    *(volatile uint32*)(GEA_DMA_REG) &= ~GEA_DMA_MODE_EN;
}

/*****************************************************************************/
//  Description: Be called when dma_all_trans done interrupt occure.
//               It will get the gea_out from register.
//  Global resource dependence:
//  Author: lichd
//  Note:
/*****************************************************************************/
volatile uint32 gea_dma_transcount =0;

LOCAL ISR_EXE_T _gea_dma_callback (  uint32 channel )
{
    uint32 id = channel&0xffff;

    //disable gea_dma and its channel
    GEA_DmaDisable();
    DMA_HAL_ChnEnable( id ,  SCI_FALSE);   

    SCI_SetEvent (s_gea_event_ptr, GEA_COMPLETE, SCI_OR);

    return ISR_DONE;
}

/*****************************************************************************/
//  Description: Create Mutex
//  Global resource dependence:
//  Author: hanjun.liu
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          Mutex
/*****************************************************************************/
LOCAL uint32 gea_dma_channel = 0;
PUBLIC void gea_init (void)
{
    CLOCK_STATUS s  = CLOCK_SUCCESS;

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

    ISR_RegHandler_Ex (TB_GEA_INT, (TB_ISR) _IrqHandle, _GEAHisrFunc, CHIPDRV_HISR_PRIO_2, NULL);

    s = DEVICE_Find ("GEA", &s_dev_gea);
    SCI_ASSERT (s == CLOCK_SUCCESS); /*assert verified*/
}
/*****************************************************************************/
//  Description: Create Mutex
//  Global resource dependence:
//  Author: hanjun.liu
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          Mutex
/*****************************************************************************/
LOCAL SCI_MUTEX_PTR gea_createmutex (void)
{
    SCI_MUTEX_PTR   mutex = SCI_NULL;
    mutex = SCI_CreateMutex ("GEASYNC MUTEX", SCI_INHERIT);
    SCI_PASSERT ( (mutex!=NULL), ("gea_createmutex:Great MUTEX fail!"));/*assert verified*/

    return mutex;
}//end of IMG_CreateMutex

LOCAL ISR_EXE_T _IrqHandle (uint32 irqnum)
{
    // clear polling register
    gea_cfg->poll = 0;
    return CALL_HISR;
}

LOCAL void _GEAHisrFunc (uint32 int_count, void *d)
{
  
    volatile uint32 temp_reg=0;
    uint32 i=0;

     //get output
    if (1 == s_mode || 2 == s_mode)
    {
        //data_switch has been set.
        *((uint32*)(s_stream)+i) = gea_cfg->gea_out;  
    }
    else
    {
        //data_switch has been set.
        *((uint32*)(s_stream)+2*i)   = gea_cfg->kasmi_out1;
        *((uint32*)(s_stream)+2*i+1) = gea_cfg->kasmi_out2;
    }

    if (0 == --s_num)                   // last time
    {
        SCI_SetEvent (s_gea_event_ptr, GEA_COMPLETE, SCI_OR);
    }
    else
    {
        //start next
        temp_reg = gea_cfg->ctrl;       // get ctr register
        temp_reg |= GEA_NEXT;
        gea_cfg->ctrl = temp_reg;       // next start
    }
}

//***************************************************************************************
//    Description: GEA cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      note the input cipher buffer length should be double of input
//      cipher data length.so 'stream' length = 2 * 'len'.
//***************************************************************************************
#if 1                
uint gea_stream (uint32 kc_0,
                 uint32 kc_1,   // kc_0,kc_1 are used for gea1,gea2,gea3
                 uint32 kc_2,
                 uint32 kc_3,   // kc_0,kc_1,kc_2,kc_3 are used for kasumi
                 uint32 in_0,   // in_0 is used for gea1,gea2,gea3
                 uint32 in_1,   // in_0,in_1 are used for kasumi
                 uint32 bDir,   // direction: 0,1
                 uint32 mode,   // 1:gea1, 2:gea2, 3:gea3, 4:kasumi
                 uint32 len,    // in unit of byte
                 uint8 *stream)
{
    uint32 temp_reg = 0, i=0;
    uint32 num;     // in unit of word
    uint32 divs;
    uint32 rets = 1;
    uint8 *pBuf = stream;

    GEA_GetMutex (s_gea_mutex_ptr);
    // gea enable
    DEVICE_Open (s_dev_gea);	
    
    if ( len > 1600 )
    {
        GEA_PutMutex (s_gea_mutex_ptr);
        return 0;/*lint !e527 confirmed by windy.wang*/
    }

    // Disable int
    CHIPDRV_DisableIRQINT (TB_GEA_INT);

    divs = (1 == mode || 2 == mode) ? (4) : (8);
    num = len/divs;


    if (len%divs)
    {
        num += 1;
        pBuf = (uint8 *) SCI_ALLOC (num*divs);
        if (PNULL == pBuf)
        {
            //SCI_TRACE_LOW:"GEADRV: memory alloc failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GEA_PHY_V5_283_112_2_17_23_4_40_976,(uint8*)"");
            rets = 0;
        }
        else
            SCI_MEMSET(pBuf, 0, num*divs);
    }

    if (1 == rets)
    {
        s_mode = mode;
        s_num = num;
        //s_stream = pBuf;

        gea_cfg->ctrl = GEA_RESET; //reset

        switch (mode)
        {
            case 1:
                // GEA I
                gea_cfg->count = (bDir<<GEA1_DIR_BIT);  //set direction
                gea_cfg->fresh = in_0;                  //set 32bit input
                gea_cfg->kc0   = kc_0;
                gea_cfg->kc1   = kc_1;                  //set 64bit key
                gea_cfg->ctrl  = MODE_GEA1;             //start gea1
                break;
            case 2:
                // GEA II
                gea_cfg->count = (bDir<<GEA2_DIR_BIT);  //set direction
                gea_cfg->fresh = in_0;                  //set 32bit input
                gea_cfg->kc0   = kc_0;
                gea_cfg->kc1   = kc_1;                  //set 64bit key
                gea_cfg->ctrl  = MODE_GEA2;             //start gea2
                break;
            case 3:
                // GEA III
                gea_cfg->count = in_0;                  //set 32bit input
                gea_cfg->fresh = (bDir<<GEA3_DIR_BIT);  //set direction
                gea_cfg->kc0   = kc_0;
                gea_cfg->kc1   = kc_1;                  //set 64bit key
                gea_cfg->ctrl  = MODE_GEA3;             //start gea3
                break;
            case 4:
                //kasumi
                gea_cfg->count = in_1;
                gea_cfg->fresh = in_0;                  //set 64bit input
                gea_cfg->kc0   = kc_0;
                gea_cfg->kc1   = kc_1;
                gea_cfg->kc2   = kc_2;
                gea_cfg->kc3   = kc_3;                  //set 128bit key
                gea_cfg->ctrl  = MODE_KSMI;             //start kasumi
                break;
            default:
                break;
        }

	    //---------------------for polling mode.
        for(i=0; i<num; i++)
        {
            while( 0 == (gea_cfg->poll&0x07) ) {}
            
            // clear polling register                
            gea_cfg->poll = 0;                                

             //get output 
            if(1 == mode || 2 == mode)
            {
                //DataSwitch ABCD->BADC
                temp_reg = gea_cfg->gea_out; 
                *((uint8*)pBuf+i*4  )   = (temp_reg>>8)&0xFF;  //lint !e613 confirmed by windy.wang
                *((uint8*)pBuf+i*4+1) = (temp_reg>>0)&0xFF;    //lint !e613 confirmed by windy.wang
                *((uint8*)pBuf+i*4+2) = (temp_reg>>24)&0xFF;   //lint !e613 confirmed by windy.wang 
                *((uint8*)pBuf+i*4+3) = (temp_reg>>16)&0xFF;   //lint !e613 confirmed by windy.wang                                     
            }
            else
            {
                //DataSwitch ABCDEFGH->HGFEDCBA  
                temp_reg = gea_cfg->kasmi_out2; 
                *((uint8*)pBuf+i*8 + 0) = (temp_reg>>24)&0xFF; //lint !e613 confirmed by windy.wang
                *((uint8*)pBuf+i*8 + 1) = (temp_reg>>16)&0xFF; //lint !e613 confirmed by windy.wang
                *((uint8*)pBuf+i*8 + 2) = (temp_reg>>8)&0xFF;  //lint !e613 confirmed by windy.wang
                *((uint8*)pBuf+i*8 + 3) = (temp_reg>>0)&0xFF;  //lint !e613 confirmed by windy.wang
 

                temp_reg = gea_cfg->kasmi_out1; 
                *((uint8*)pBuf+i*8 + 4) = (temp_reg>>24)&0xFF; //lint !e613 confirmed by windy.wang 
                *((uint8*)pBuf+i*8 + 5) = (temp_reg>>16)&0xFF; //lint !e613 confirmed by windy.wang
                *((uint8*)pBuf+i*8 + 6) = (temp_reg>>8)&0xFF;  //lint !e613 confirmed by windy.wang
                *((uint8*)pBuf+i*8 + 7) = (temp_reg>>0)&0xFF;  //lint !e613 confirmed by windy.wang
            }
            
            if(i == (num-1))
                    break;                           //last time
            
            //start next
            temp_reg = gea_cfg->ctrl;                // get ctr register
            temp_reg |= GEA_NEXT;
            gea_cfg->ctrl = temp_reg;                // next start                
        }
        //SCI_TRACE_LOW:"GEADRV:...Polling Done-->"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GEA_PHY_V5_381_112_2_17_23_4_41_977,(uint8*)"");
    }
    
    if ( (len%divs) && PNULL != pBuf)
    {
        SCI_MEMCPY (stream, pBuf, len);
        SCI_FREE (pBuf);
    }    
    
    // disable gea
    DEVICE_Close (s_dev_gea);

    GEA_PutMutex (s_gea_mutex_ptr);

    return rets;
}

#else
uint gea_stream (uint32 kc_0,
                 uint32 kc_1,   // kc_0,kc_1 are used for gea1,gea2,gea3
                 uint32 kc_2,
                 uint32 kc_3,   // kc_0,kc_1,kc_2,kc_3 are used for kasumi
                 uint32 in_0,   // in_0 is used for gea1,gea2,gea3
                 uint32 in_1,   // in_0,in_1 are used for kasumi
                 uint32 bDir,   // direction: 0,1
                 uint32 mode,   // 1:gea1, 2:gea2, 3:gea3, 4:kasumi
                 uint32 len,    // in unit of byte
                 uint8 *stream)
{
    uint32 num;
    uint32 divs;
    uint32 actualFlag;
    uint32 rets = 1;

    uint32 gea_dma_channel = 0;
    DMA_CHNCFG_T gea_dma_cfg;

    GEA_GetMutex (s_gea_mutex_ptr);

    gea_cfg->ctrl = GEA_RESET; //reset
    
    // gea enable
    DEVICE_Open(s_dev_gea);

    // Disable int
    CHIP_REG_OR(INT_IRQ_DISABLE, INTCTL_GEA_IRQ);

    if((1 == mode ) || (2 == mode))   //need modify
    {
        //data_switch: ABCD->CDAB, set 2'b010;
        gea_cfg->dma_reg &= ~( BIT_2| BIT_1| BIT_0 );
        gea_cfg->dma_reg |=  (       BIT_1 );  
    }
    else
    {
        //data_switch: ABCDEFGA->EFGAABCD, set 2'b100;
        gea_cfg->dma_reg &= ~( BIT_2| BIT_1| BIT_0 );
        gea_cfg->dma_reg |=  ( BIT_2 );  
    }

    divs = (1 == mode || 2 == mode) ? (4) : (8);

    //data_length with unit of 32bits
    num = len/divs;   

    if (len%divs)
    {
        num += 1;
    }

    //config the dma trans.
    GEA_DMA_SetRunNumber( num );
    //Enable the gea dma
    GEA_DmaEnable();

    {
        s_mode = mode;
        s_num = num;
        s_stream = stream;

        switch (mode)
        {
            case 1:
                // GEA I
                gea_cfg->count = (bDir<<GEA1_DIR_BIT);  //set direction
                gea_cfg->fresh = in_0;                  //set 32bit input
                gea_cfg->kc0   = kc_0;
                gea_cfg->kc1   = kc_1;                  //set 64bit key
                gea_cfg->ctrl  = MODE_GEA1;             //start gea1
                break;
            case 2:
                // GEA II
                gea_cfg->count = (bDir<<GEA2_DIR_BIT);  //set direction
                gea_cfg->fresh = in_0;                  //set 32bit input
                gea_cfg->kc0   = kc_0;
                gea_cfg->kc1   = kc_1;                  //set 64bit key
                gea_cfg->ctrl  = MODE_GEA2;             //start gea2
                break;
            case 3:
                // GEA III
                gea_cfg->count = in_0;                  //set 32bit input
                gea_cfg->fresh = (bDir<<GEA3_DIR_BIT);  //set direction
                gea_cfg->kc0   = kc_0;
                gea_cfg->kc1   = kc_1;                  //set 64bit key
                gea_cfg->ctrl  = MODE_GEA3;             //start gea3
                break;
            case 4:
                //kasumi
                gea_cfg->count = in_1;
                gea_cfg->fresh = in_0;                  //set 64bit input
                gea_cfg->kc0   = kc_0;
                gea_cfg->kc1   = kc_1;
                gea_cfg->kc2   = kc_2;
                gea_cfg->kc3   = kc_3;                  //set 128bit key
                gea_cfg->ctrl  = MODE_KSMI;             //start kasumi
                break;
            default:
                break;
        }

    }

    gea_dma_transcount = num*divs;
    {
        uint32 burst_size =GEA_DMA_FIFO_LEN;
        uint32 stream_addr=0;
        
        //coculate the stream address to store.
        stream_addr = stream + ((num*divs)/16 -gea_dma_transcount/16)*16;  
    
        gea_dma_channel = DMA_HAL_AllocChn(DMA_CHN_TRSF_TYPE_STANDARD);
        GEA_DMA_CONNECT(gea_dma_channel);	
        
        gea_dma_cfg.workmode    = DMA_CHN_WORKMODE_NORMAL;
        gea_dma_cfg.requestmode = DMA_CHN_REQMODE_NORMAL;
        gea_dma_cfg.src_address  = (uint32)GEA_DMA_MEM1;
        gea_dma_cfg.dest_address = (uint32)stream_addr;
        gea_dma_cfg.wrapstart_addr = GEA_DMA_MEM1;
        gea_dma_cfg.wrapend_addr   = GEA_DMA_MEM4;
        gea_dma_cfg.is_srcwrapen   = SCI_TRUE;
        
        gea_dma_cfg.frag_length   = burst_size;
        gea_dma_cfg.block_length  = burst_size;     //GEA_DMA_MEM1~4
        gea_dma_cfg.total_length  = gea_dma_transcount;
        gea_dma_cfg.src_step     = 4;
        gea_dma_cfg.dest_step    = 4;
        gea_dma_cfg.src_fragstep = 0;
        gea_dma_cfg.dest_fragstep= 0;
        gea_dma_cfg.src_blockstep= 0;
        gea_dma_cfg.dest_blockstep= 0;
        
        gea_dma_cfg.src_datawidth = DMA_DATAWIDTH_WORD;
        gea_dma_cfg.dest_datawidth = DMA_DATAWIDTH_WORD;
        gea_dma_cfg.priority = DMA_CHN_PRIORITY_3;

        DMA_HAL_ChnConfig(gea_dma_channel, &gea_dma_cfg);
        // Register DMA callback funtion and enable transaction mode.
        DMA_HAL_ChnIntConfig (gea_dma_channel, DMA_CHN_INT_TYPE_TRANSDONE, _gea_dma_callback);        
        
        DMA_HAL_ChnEnable( gea_dma_channel,  SCI_TRUE );
    } 

    //Use software to adjust the data for gea1/2: ABCD->CDAB, gea3/kasumi: ABCDEFGH->EFGHABCD
    if (SCI_ERROR == SCI_GetEvent (s_gea_event_ptr, GEA_COMPLETE, SCI_AND_CLEAR,
                                   &actualFlag, GEA_EVENT_TIME_OUT))
    {
        //SCI_TRACE_LOW:"GEA Driver wait timeout:%ums!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GEA_PHY_V5_547_112_2_17_23_4_41_978,(uint8*)"u",GEA_EVENT_TIME_OUT);
        rets = 0;
    }
    else
    {
        uint16 temp16 , i;
        uint32 temp32;
        uint16 *p_temp16 = (uint16*)( stream );
        uint32 *p_temp32 = (uint32*)( stream );;

        //gea1/2: adjust to ABCD->CDAB,
        if( (1 == mode) || (2 == mode) )
        {
            for(i=0; i<num; i++)
            {
                temp16        = * p_temp16;
                *(p_temp16)   = *(p_temp16+1);
                *(p_temp16+1) = temp16;

                p_temp16 = p_temp16 + 2;
            }
        }
        else
        {
            for(i=0; i<num; i++)
            {
                temp32        = * p_temp32;
                *(p_temp32)   = *(p_temp32+1);
                *(p_temp32+1) = temp32;

                p_temp32 = p_temp32 + 2;
            }
        }
        //SCI_TraceLow:"GEADRV:...Dma Int Done-->"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GEA_PHY_V5_580_112_2_17_23_4_41_979,(uint8*)"");
    }

    GEA_PutMutex (s_gea_mutex_ptr);

    return rets;
}
#endif

//***************************************************************************************
//    Description: GEA cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : gea1,gea2,gea3 have 64bit key
//
//      "input": gea1,gea2,gea3 have 32bit input
//
//      example(gea3):
//              Kc    0xEFA8B2229E720C2A
//              INPUT 0x4BDBD5E5
//              DIRECTION 1
//              OUTPUT(16bytes):
//              0x4718A2ADFC90590949DDADAB406EC3B9
//
//              gea_stream(kc, 0x4BDBD5E5, 1, 3, 8, stream_out);
//***************************************************************************************
uint gea_get_stream (const uint8 kc[8],  // bit00~63 of parameter kc
                     uint32 in_0,        // bit00~31 of parameter input
                     uint32 bDir,        // direction: 0,1
                     uint32 mode,        // 1:gea1, 2:gea2, 3:gea3, 4:not supported
                     uint32 len,         // in unit of 2bytes
                     uint8 *stream)      // output stream pointer
{
    uint8  *key;
    uint32 kc_word[2];
    uint32 i;

    if (mode > 3 || mode < 1)
    {
        SCI_ASSERT (0);/*assert to do*/
        return 0; /*lint !e527 comfirmed by xuepeng*/
    }

    key = (uint8 *) (&kc[4]);

    for (i=0; i<2; i++)
    {
        kc_word[i] = ( (*key) <<24) | (* (key+1) <<16) | (* (key+2) <<8) | (* (key+3));
        key -= 4;
    }

    return gea_stream (kc_word[0],
                       kc_word[1],  // kc_0,kc_1 are used for gea1,gea2,gea3
                       0,
                       0,      // kc_0,kc_1,kc_2,kc_3 are used for kasumi
                       in_0,   // in_0 is used for gea1,gea2,gea3
                       0,      // in_0,in_1 are used for kasumi
                       bDir,   // direction: 0,1
                       mode,   // 1:gea1, 2:gea2, 3:gea3, 4:kasumi
                       len*2,
                       stream);
}

    
#ifdef   __cplusplus
}
#endif
