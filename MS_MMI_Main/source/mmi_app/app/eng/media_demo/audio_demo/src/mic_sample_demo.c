/****************************************************************************
** File Name:      MIC_SAMPLE_DEMO.c                                                                            *
** Author:                                                                                                                      *
** Date:           11/12/2010                                                                                            *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.                               *
** Description:    the file mic sample demo     
*****************************************************************************
**                         Important Edit History                                                                         *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                     *
** 12/2010        hai.li           Create
******************************************************************************/
#include "mmi_app_eng_trc.h"
#ifdef MIC_SAMPLE_SUPPORT

#define _MIC_SAMPLE_DEMO_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                                                                       *
 **--------------------------------------------------------------------------*/
#include "sci_api.h"
#include "os_api.h"
#include "deep_sleep.h"
#include "tasks_id.h"
#include "mic_sample.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmisms_internal.h"
#include "mmieng_id.h"
#include "mmk_msg.h"
#include "audio_api.h"
#include "audio_config.h"
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                                                                    *
 **---------------------------------------------------------------------------*/

#define MICSAMPLE_DEMO_STACK_SIZE      (1024)
#define MICSAMPLE_DEMO_QUEUE_NUM       (2)
#define MICSAMPLE_DEMO_TASK            (FORBIDDEN_USE10)
#define MICSAMPLE_DEMO_DEFAULT_TASK_ID (0xFFFFFFFF)

/**---------------------------------------------------------------------------*
 **                          Variables                                                                                             *
 **---------------------------------------------------------------------------*/
#pragma arm section zidata = "DEMO_TASK_STACK_AND_QUEUE"
    uint32    MICSAMPLE_DEMO_stack_addr[MICSAMPLE_DEMO_STACK_SIZE >> 2];
    uint32    MICSAMPLE_DEMO_queue_addr[MICSAMPLE_DEMO_QUEUE_NUM * SCI_QUEUE_ITEM_SIZE];
#pragma arm section zidata


LOCAL BLOCK_ID micsample_demo_thread=0;
LOCAL BOOLEAN  micsample_demo_flag=0;


/*****************************************************************************/
//  Description:   mic sample thread entry
//  Author:        
//  Note:           
/*****************************************************************************/
LOCAL void MicSample_Thread_Entry(        
    uint32 argc,     
    void* argv  
)
{
    int16 get_time=0,get_db=0;

    uint8 sampling_type=MIC_SAMPLE_128;
    int32  show_dB=0;
    BOOLEAN ret_temp=0;
    

    //SCI_TRACE_LOW:"mic_sample_demo, thread entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MIC_SAMPLE_DEMO_64_112_2_18_2_18_23_288,(uint8*)"");

    while (1) //lint !e716
    {  
      
#if 0 
        SCI_Sleep(50);
        get_db=AUD_GetMic_dB(sampling_type);
        show_dB=get_db; 

#else //for show dB value in menu
        SCI_Sleep(100);
        get_db=AUD_GetMic_dB(sampling_type);
        show_dB=get_db; 
        ret_temp= MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_MEDIA_DEMO_SHOW_DATE,&show_dB, sizeof(int32));
        MMI_TriggerMMITask();
#endif      
        //SCI_TRACE_LOW:"[MicSample Demo]: get_time=%d, sample type=%d, get_db=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MIC_SAMPLE_DEMO_81_112_2_18_2_18_23_289,(uint8*)"ddd", get_time,sampling_type , get_db);
        get_time++;

        //每采样300次，改变一次采样类型，用于DEMO, 实际使用时依据情况选用一种即可
        //    MIC_SAMPLE_128,               //get the value of db from 128 sample date
        //    MIC_SAMPLE_256,               //get the value of db from 256 sample date
        //    MIC_SAMPLE_AVERAGE_128,       //get the average value of db from 128 sample date (two times) 
        //    MIC_SAMPLE_MAX                // Reserved, user can't use it.
        if(get_time==300)
        { 
            get_time=0;
            sampling_type++;
            sampling_type=((sampling_type)<MIC_SAMPLE_MAX)?(sampling_type):(MIC_SAMPLE_128);
        }
    }

}

/*****************************************************************************/
//  Description : API of   start demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/

PUBLIC void MIC_Sample_Demo_Start(void)
{ 
    uint32 uiPriority = 0, ret_value=0;
    AUDIO_RESULT_E ret_temp;
    //SCI_TRACE_LOW:"mic_sample_demo, start in"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MIC_SAMPLE_DEMO_107_112_2_18_2_18_24_290,(uint8*)"");
    
    if(micsample_demo_flag)
    {
      //SCI_TRACE_LOW:"mic_sample_demo have started"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MIC_SAMPLE_DEMO_111_112_2_18_2_18_24_291,(uint8*)"");
      return ;
    }
    micsample_demo_flag = TRUE;
    
    ret_temp=AUD_EnableMicSample(SCI_TRUE); 

    SCI_ASSERT(ret_temp==AUDIO_NO_ERROR); /*assert to do*/

   /*-------------create static task for get mic sample dB -------------*/ 

    SCI_MEMSET((void *)MICSAMPLE_DEMO_stack_addr, 0x0, sizeof(MICSAMPLE_DEMO_stack_addr));
    SCI_MEMSET((void *)MICSAMPLE_DEMO_queue_addr, 0x0, sizeof(MICSAMPLE_DEMO_queue_addr));
                
    uiPriority=27UL;  
    
    ret_value=SCI_CreateStaticThread(MICSAMPLE_DEMO_TASK, 
                        "T_MICSAMPLE", 
                        MicSample_Thread_Entry, 
                        0, 
                        NULL, 
                        MICSAMPLE_DEMO_stack_addr, 
                        MICSAMPLE_DEMO_STACK_SIZE, 
                        uiPriority, 
                        SCI_PREEMPT,
                        "Q_MICSAMPLE", 
                        MICSAMPLE_DEMO_queue_addr, 
                        MICSAMPLE_DEMO_QUEUE_NUM, 
                        SCI_AUTO_START); 


    SCI_ASSERT(ret_value == SCI_SUCCESS); /*assert verified*/
    
    micsample_demo_thread=MICSAMPLE_DEMO_TASK;

    //SCI_TRACE_LOW:"mic_sample_demo, start end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MIC_SAMPLE_DEMO_146_112_2_18_2_18_24_292,(uint8*)"");

}

/*****************************************************************************/
//  Description : API of   stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void MIC_Sample_Demo_Stop(void)
{
    AUDIO_RESULT_E ret_temp;
    //SCI_TRACE_LOW:"mic_sample_demo, close in"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MIC_SAMPLE_DEMO_156_112_2_18_2_18_24_293,(uint8*)"");

    if (!micsample_demo_flag)
    {
       //SCI_TRACE_LOW:"mic_sample_demo have clsoed"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MIC_SAMPLE_DEMO_160_112_2_18_2_18_24_294,(uint8*)"");
       return;
    }

    micsample_demo_flag = FALSE;

    if(MICSAMPLE_DEMO_DEFAULT_TASK_ID != micsample_demo_thread)
    {
        SCI_TerminateThread(micsample_demo_thread);// must terminate it for deleting.
        SCI_DeleteThread(micsample_demo_thread);   // Delete tha task.
        micsample_demo_thread = MICSAMPLE_DEMO_DEFAULT_TASK_ID;
    }

    ret_temp=AUD_EnableMicSample(SCI_FALSE); 


    SCI_ASSERT(ret_temp==AUDIO_NO_ERROR); /*assert to do*/

        
    //SCI_TRACE_LOW:"mic_sample_demo, close end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MIC_SAMPLE_DEMO_179_112_2_18_2_18_24_295,(uint8*)"");
}

#endif
