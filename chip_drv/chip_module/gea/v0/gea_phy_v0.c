/******************************************************************************
 ** File Name:      gea_phy_v0.c                                                    *
 ** Author:                                                       *
 ** DATE:                                                           *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/17/2010          Create.                                   *
 ******************************************************************************/


#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "gea_reg_v0.h"
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
/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/
// these 3 variable used in isr
LOCAL volatile uint32 s_num    = NULL;
LOCAL volatile uint32 s_mode   = NULL;
LOCAL volatile uint8 *s_stream = PNULL;
LOCAL volatile GEA_REG_CFG_T *gea_cfg = (GEA_REG_CFG_T *) ( (volatile uint32 *) GEA_BASE);
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

void gea_close (void)
{
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

    ISR_RegHandler_Ex (TB_GEA_POLL_INT, (TB_ISR) _IrqHandle, _GEAHisrFunc, CHIPDRV_HISR_PRIO_2, NULL);

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

//static   volatile uint32  tmp; // defined as static,

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

LOCAL ISR_EXE_T _IrqHandle (uint32 irqnum)
{
    // clear polling register
    gea_cfg->poll = 0;
    return CALL_HISR;
}

LOCAL void _GEAHisrFunc (uint32 int_count, void *d)
{
    volatile uint32 temp_reg;
    uint32 i, tmp_val;
    uint16 tmp16;

    //get output
    if (1 == s_mode || 2 == s_mode)
    {
        tmp16 = (gea_cfg->gea_out) &0xffff;

        for (i=0; i<2; i++)
        {
            *s_stream++ = (tmp16>> (8-i*8)) &0xff;
        }

        tmp16 = ( (gea_cfg->gea_out) >>16) &0xffff;

        for (i=0; i<2; i++)
        {
            *s_stream++ = (tmp16>> (8-i*8)) &0xff;
        }
    }
    else
    {
        tmp_val = (gea_cfg->kasmi_out2);

        for (i=0; i<4; i++)
        {
            *s_stream++ = (tmp_val>> (24-i*8)) &0xff;
        }

        tmp_val = (gea_cfg->kasmi_out1);

        for (i=0; i<4; i++)
        {
            *s_stream++ = (tmp_val>> (24-i*8)) &0xff;
        }
    }

    if (0 == --s_num)                   // last time
    {
        SCI_SetEvent (s_gea_event_ptr, GEA_COMPLETE, SCI_OR);
        return ;
    }
    else
    {
        //start next
        temp_reg = gea_cfg->ctrl;       // get ctr register
        temp_reg |= GEA_NEXT;
        gea_cfg->ctrl = temp_reg;       // next start
    }

    return ;
}


//***************************************************************************************
//    Description: GEA cipher function
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
                 uint32 bDir,   // direction: 0,1
                 uint32 mode,   // 1:gea1, 2:gea2, 3:gea3, 4:kasumi
                 uint32 len,    // in unit of byte
                 uint8 *stream)
{
    uint32 num;
    uint32 divs;
    uint32 actualFlag;
    uint32 rets = 1;
    uint8 *pBuf = stream;

    GEA_GetMutex (s_gea_mutex_ptr);

    // gea enable
    DEVICE_Open (s_dev_gea);

    // int enbale
    CHIPDRV_EnableIRQINT (TB_GEA_POLL_INT);

    divs = (1 == mode || 2 == mode) ? (4) : (8);

    num = len/divs;

    if (len%divs)
    {
        num += 1;
        pBuf = (uint8 *) SCI_ALLOC (num*divs);

        if (PNULL == pBuf)
        {
            //SCI_TRACE_LOW:"GEA Driver memory alloc failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GEA_PHY_V0_332_112_2_17_23_4_35_966,(uint8*)"");
            rets = 0;
        }
    }

    if (1 == rets)
    {
        s_mode = mode;
        s_num = num;
        s_stream = pBuf;

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

        if (SCI_ERROR == SCI_GetEvent (s_gea_event_ptr, GEA_COMPLETE, SCI_AND_CLEAR,
                                       &actualFlag, GEA_EVENT_TIME_OUT))
        {
            //SCI_TRACE_LOW:"GEA Driver wait timeout:%ums!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GEA_PHY_V0_388_112_2_17_23_4_35_967,(uint8*)"u",GEA_EVENT_TIME_OUT);
            rets = 0;
        }
    }

    if ( (len%divs) && PNULL != pBuf)
    {
        SCI_MEMCPY (stream, pBuf, len);
        SCI_FREE (pBuf);
    }

    // disable int
    CHIPDRV_DisableIRQINT(TB_GEA_POLL_INT);

    // disable gea
    DEVICE_Close (s_dev_gea);

    GEA_PutMutex (s_gea_mutex_ptr);

    return rets;
}

#ifdef   __cplusplus
}
#endif
