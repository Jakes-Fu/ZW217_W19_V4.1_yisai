/****************************************************************************
** File Name:      CAT_MV_DEOM.c                                             *
** Author:                                                                   *
** Date:           11/10/2010                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    the file CATMV_DEOM    
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2010        hai.li          Create
******************************************************************************/
#ifdef CAT_MVOICE_DEMO_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "asm.h"
#include "sci_api.h"
#include "os_api.h"
#include "deep_sleep.h"
//#include "tasks_id.h"
#include "sig_code.h"
#include "audio_api.h"
#include "audio_prod.h"
#include "asm.h"
#include "wav_adp.h"
#include "cat_mvoice.h"
#include "dal_audio.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C"
    {
#endif

#define TASK_CATMV_PRI    PRI_CATMV_TASK //29

#define CAT_MV_DEMO_ASSERT_MODE

#ifdef CAT_MV_DEMO_ASSERT_MODE
    #define CATMVDEMO_ASSERT    SCI_ASSERT
    #define CATMVDEMO_PASSERT   SCI_PASSERT 
#else
    #define CATMVDEMO_ASSERT  
    #define CATMVDEMO_PASSERT 
#endif


#define CAT_MV_DEMO_DEBUG

#ifdef CAT_MV_DEMO_DEBUG
    #ifndef CATMVDEMO_PRINT
        #define CATMVDEMO_PRINT(format) SCI_TRACE_LOW format
    #endif
#else
    #ifndef CATMVDEMO_PRINT
        #define CATMVDEMO_PRINT(format)	
    #endif    
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#ifndef CatMvDEMO_FREE
    #define CatMvDEMO_FREE(PTR)	SCI_DisableIRQ(); SCI_FREE(PTR); SCI_RestoreIRQ();
#endif


#define IDLE          0
#define DECODING      1
#define RECORD        2
#define DEMOPLAY      3

#define CATMV_STACK_SIZE (2 * 1024)
#define CATMV_QUEUE_NUM  (10)
#define AUDIO_DEFAULT_TASK_ID (0xFFFFFFFF)

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! Request:
 *  Requestion is launched by audio service layer to inform audio codec to do something.
 */
/*----------------------------------------------------------------------------*/
 /*! \struct CatMvDEMO_GETSRC_REQ_MSG_T
 *  \brief Request to decode src data(codec->codec).
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
}CatMvDEMO_GETSRC_REQ_MSG_T;

/*! \struct CatMvDEMO_DECODE_REQ_MSG_T
 *  \brief Request to decode src data(codec->codec).
 */
typedef struct
{
    SIGNAL_VARS        /*!<Signal Header>*/
}CatMvDEMO_DECODE_REQ_MSG_T;


/*! \union CatMvDEMO_Task_Msg_union
 *  \brief Collect all IIS related messages' struct definition.
 */
typedef union 
{
    CatMvDEMO_DECODE_REQ_MSG_T  tDecodeReq;
    CatMvDEMO_GETSRC_REQ_MSG_T  tGerSrcReq;
}CatMvDEMO_TASK_MSG_U;

/*! signal code definition
 *  
 */
typedef enum CatMvDEMO_SigCode_enum
{
    CatMvDEMO_PLAY_REQ,   //as -> codec
    CatMvDEMO_STOP_REQ,   //as -> codec  codec -> codec
    CatMvDEMO_START_MICSAMPLE_REQ, //codec -> codec
    CatMvDEMO_PLAY_END_REQ  //codec -> codec
}CatMvDEMO_SIGNAL_CODE_E;

typedef struct 
{
    void** ppvSigIn;    /*!< Signal_in pointer that audio task received. We should free it when audio task is deleted.*/
    void** ppvSigOut;   /*!< Signal_out pinter that audio task prepared to send out when audio task is deleted. We should free it.*/	
}CatMv_OBJECT_T;



LOCAL void CatMvDEMO_Thread_Entry(        
    uint32 argc,     
    void* ptAudioObj  
);

LOCAL int CatMvDEMO_SendSignal(
    BLOCK_ID tThreadId,
    CatMvDEMO_SIGNAL_CODE_E eSigCode,
    void* pSigPara
);


/*******************************************************************************
 *  type declare
 *******************************************************************************/
typedef struct
{
    HAUDIO          pcm_handle; /*device handle*/
    uint32          source_size;
    uint16          end_flag;
    uint16          stop_flag;
    uint8           play_state;
    uint8           demo_state;
}CAT_DEC_CTX_T;

/*******************************************************************************
 *  static variable
 *******************************************************************************/

LOCAL BLOCK_ID hcatmv_demo_thread=0;
LOCAL CatMv_OBJECT_T pt_CatMvObj;
LOCAL SFS_HANDLE s_catmv_file_handle = SCI_NULL;
LOCAL int8 play_count =0;
LOCAL uint16 data_mod=0;
LOCAL int32 s_data_scale=0;
LOCAL CAT_DEC_CTX_T  s_dec_ctx = {0};

#ifndef WIN32
LOCAL __align(4)  int16  catmv_data_buf[MIC_BUF_LEN+4];//100ms
#else
LOCAL  int16  catmv_data_buf[MIC_BUF_LEN+4];//100ms
#endif

LOCAL void CatMv_Data_Play(void);
LOCAL void CatMv_Data_Stop(void);
LOCAL void  CatMv_EnableCatMicCodec( BOOLEAN is_enable);

//#define _CATMV_AGC_OFF_  //for 6600L ONLYon or off agc funcation
#define CATMV_CHANGE_VOICE //on or off voice change

#ifdef CATMV_CHANGE_VOICE

//#define _CATMV_FUNC_IN_IRAM_  //for 6600L ONLY

#include "scf_api.h"
#include "iram_mgr.h"

LOCAL void  *scf_ptr;
LOCAL int16 data[128*2]={0};

extern uint16 GUI_GB2UCS(                   //return the number of UCS2 code that have been converted.
                         uint16 *ucs2_ptr,  //destination: string pointer to UCS2 string.
                         const uint8 *gb_ptr,//source: string pointer to GB string
                         uint16 len         //the length to convert from GB to UCS2.
                         );

#ifdef _CATMV_FUNC_IN_IRAM_  //for 6600L ONLY
extern uint32 Load$$IRAM_CATMV_VOICE$$Base;
extern uint32 Image$$IRAM_CATMV_VOICE$$Base;
extern uint32 Image$$IRAM_CATMV_VOICE$$Length;

                         
LOCAL const IRAM_AREA s_cat_iram_area[] = {
    {&Image$$IRAM_CATMV_VOICE$$Base, &Image$$IRAM_CATMV_VOICE$$Length, 	(void *)&Load$$IRAM_CATMV_VOICE$$Base, 	IRAM_ATTRIBUTE_NORMAL}
    };

LOCAL ILOCK_HANDLE s_cat_iram_handle = IRAM_INVALID_HANDLE;

LOCAL BOOLEAN CAT_LOCK_IRAM()
{
    if(IRAM_INVALID_HANDLE == s_cat_iram_handle)
    {
        s_cat_iram_handle = IRAM_LockArea((const IRAM_AREA *)&(s_cat_iram_area), 
                                            sizeof(s_cat_iram_area)/sizeof(IRAM_AREA), 
                                            IRAM_NO_WAIT);
    }

    if(IRAM_INVALID_HANDLE == s_cat_iram_handle)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

LOCAL void CAT_UNLOCK_IRAM()
{
    if(IRAM_INVALID_HANDLE != s_cat_iram_handle)
    {
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CAT_UNLOCK_IRAM, UnLock Iram Area!"));
        IRAM_UnlockArea(s_cat_iram_handle);
        s_cat_iram_handle = IRAM_INVALID_HANDLE;
    }

}

#else

LOCAL BOOLEAN CAT_LOCK_IRAM()
{ 
    return  1;
}

LOCAL void CAT_UNLOCK_IRAM()
{  
    return ;
}

#endif

LOCAL void MV_init()
{
/*set up and file info for SCF*/
  //  CAT_LOCK_IRAM();
#ifdef _CATMV_AGC_OFF_  
    MMIAUDIO_StopAGC();
    #ifndef WIN32
    AUD_AGC_LockSwitch(SCI_TRUE);
    #endif
#endif    
    SCF_construct(&scf_ptr);
    SCF_init(scf_ptr);
}

LOCAL void MV_deinit()
{
    /*set up and file info for SCF*/
    SCF_deconstruct(&scf_ptr); 
    CAT_UNLOCK_IRAM();
#ifdef _CATMV_AGC_OFF_ 
    #ifndef WIN32
    AUD_AGC_LockSwitch(SCI_FALSE);
    #endif
#endif

}

#ifdef DEBUG_OUT_FILE
extern  void CatMvDEMO_WriteMvoiceData(int16* pData, uint32 wt_siez);
#endif

LOCAL void MV_Cat_Process(int16 *src_data_pt,int16 *data_out_pt, uint16 src_size,uint16 *out_size)
{  
    uint32 process_count=128*2;
    uint32  j=0;
    uint32 poll_num=0,temp_mod;
    uint32 s_time=0,d_time=0;
    int16  *pdata_in=PNULL, *pdata_out=PNULL;
    
    *out_size=0;
    /*
    if(0)
    {
        SCI_Mem16Cpy(data_out_pt, src_data_pt, src_size*2);
        *out_size=src_size;
        return;
    }
*/
    CAT_LOCK_IRAM();
 
    {
        poll_num=(int)((uint32)(src_size+data_mod)/(uint32)process_count);
        temp_mod=(int)((uint32)(src_size+data_mod)%(uint32)process_count);
        CATMVDEMO_PRINT(("[Cat_mv_demo]: MV_Cat_Process, src_size=%d,poll_num=%d,temp_mod=%d",src_size,poll_num,temp_mod));
    
        if(data_mod!=0)
        {
            SCI_Mem16Cpy((uint16*)(data+data_mod), (uint16*)(src_data_pt+j*process_count), (uint16)((process_count-data_mod)*2));
            SCF_process(scf_ptr,(int16*)(data+data_mod), (int16*)data_out_pt,s_data_scale);
#ifdef DEBUG_OUT_FILE
            CatMvDEMO_WriteMvoiceData(data_out_pt, process_count*2);
#endif               
            for(j=1;j<poll_num;j++)
            {   
                SCI_Mem16Cpy((uint16*)data, (uint16*)(src_data_pt+j*process_count-data_mod), process_count*2);
                SCF_process(scf_ptr, data, (int16*)(data_out_pt+j*process_count), s_data_scale); 
#ifdef DEBUG_OUT_FILE
                CatMvDEMO_WriteMvoiceData((uint16*)(data_out_pt+j*process_count), process_count*2);
#endif
            }

        }
        else
        {
            for(j=0;j<poll_num;j++)
            {   
                pdata_in = (int16*)(src_data_pt+j*process_count);       /*in/out data*/
                pdata_out= (int16*)(data_out_pt+j*process_count); 
                #if 1
                s_time=SCI_GetTickCount();
                SCF_process(scf_ptr,pdata_in,pdata_out,s_data_scale);
                d_time = SCI_GetTickCount() - s_time;
                CATMVDEMO_PRINT(("[Cat_mv_demo]: MV_Cat_Process, SCF_process d_time=%d",d_time));
#ifdef DEBUG_OUT_FILE
                CatMvDEMO_WriteMvoiceData(pdata_out, process_count*2);
#endif
                #else
                    SCF_process(scf_ptr,pdata_in,pdata_out,s_data_scale);
                #endif
            }
        }
    
        if(temp_mod!=0)
        {
            SCI_Mem16Cpy((uint16*)data, (uint16*)(src_data_pt+poll_num*process_count-data_mod), temp_mod*2);
            data_mod=temp_mod;
        }
    }
    *out_size=process_count*poll_num; 
    CAT_UNLOCK_IRAM();
}
#else

LOCAL void MV_init()
{ 
    return;
}

LOCAL void MV_deinit()
{     
   return;
}

LOCAL void MV_Cat_Process(int16 *src_data_pt,int16 *data_out_pt, uint16 src_size,uint16 *out_size)
{  
    
    *out_size=0;
    SCI_Mem16Cpy((uint16*)data_out_pt, (uint16*)src_data_pt, src_size*2);
    *out_size=src_size;
}

#endif

//for debug
//#define DEBUG_OUT_FILE

#ifdef DEBUG_OUT_FILE
LOCAL SFS_HANDLE s_catmv_file_out_handle = SCI_NULL;

LOCAL void CatMvDEMO_OpenMvoiceFile(void)
{     
    uint16 unicode_file_name[64]= {0};
    char char_file_name[64]= {0};
    sprintf((char *)char_file_name, "E:\\mv_mvoice_out%d",play_count);	

    GUI_GB2UCS(unicode_file_name, char_file_name, (uint16)strlen(char_file_name));
    s_catmv_file_out_handle = SFS_CreateFile( (const uint16 *)unicode_file_name, SFS_MODE_CREATE_ALWAYS | SFS_MODE_WRITE,NULL,NULL);
    CATMVDEMO_ASSERT(s_catmv_file_out_handle!= NULL); /*assert to do*/       
}

LOCAL void CatMvDEMO_WriteMvoiceData(int16* pData, uint32 wt_siez)
{  
    uint32 bytes_t= 0; 
     
    if(s_catmv_file_out_handle)
    { 
        SFS_WriteFile(s_catmv_file_out_handle,
                        (uint8 *)pData,
                        wt_siez,
                        &bytes_t,
                        NULL
                        ); 
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_WriteMicData bytes_t=%d",bytes_t));
    }
    else
    {
        CATMVDEMO_ASSERT(s_catmv_file_out_handle!= NULL);/*assert verified*/
    }
} 

LOCAL void CatMvDEMO_CloseMvoiceFile(void)
{  
    CATMVDEMO_ASSERT(s_catmv_file_out_handle!= NULL); /*assert verified*/
    CATMVDEMO_ASSERT(0==SFS_CloseFile(s_catmv_file_out_handle));/*assert verified*/
    s_catmv_file_out_handle = NULL;
}

#endif

LOCAL void CatMvDEMO_OpenWriteFile(void)
{     
    uint16 unicode_file_name[64]= {0};
    char char_file_name[64]= {0};

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_OpenWriteFile %d", s_catmv_file_handle)) ;

    sprintf((char *)char_file_name, "E:\\mv_decode_out%d", play_count);	
    GUI_GB2UCS(unicode_file_name, (const uint8*)char_file_name, (uint16)strlen(char_file_name));
    s_catmv_file_handle = SFS_CreateFile( (const uint16 *)unicode_file_name, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE,NULL,NULL);

    CATMVDEMO_PASSERT(s_catmv_file_handle!= NULL,("[Cat_mv_demo]: CatMvDEMO_OpenWriteFile, No SD card, Please check and try again!"));/*assert to do*/
}



LOCAL void CatMvDEMO_OpenReadMicFile(void)
{     
    uint16 unicode_file_name[64]= {0};
    char char_file_name[64]= {0};

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_OpenReadFile,play_count=%d",play_count));
    sprintf(char_file_name, "E:\\mv_decode_out%d",play_count);	

    GUI_GB2UCS(unicode_file_name, (const uint8*)char_file_name, (uint16)strlen(char_file_name));
    s_catmv_file_handle = SFS_CreateFile( (const uint16 *)unicode_file_name, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ,NULL,NULL);
    CATMVDEMO_ASSERT(s_catmv_file_handle!= NULL); /*assert verified*/

#ifdef DEBUG_OUT_FILE
    CatMvDEMO_OpenMvoiceFile();
#endif
        
}

LOCAL void CatMvDEMO_WriteMicData(int16* pData, uint32 wt_siez)
{  
    uint32 bytes_t= 0; 
     
    if(s_catmv_file_handle)
    { 
        SFS_WriteFile(s_catmv_file_handle,
                        (uint8 *)pData,
                        wt_siez,
                        &bytes_t,
                        NULL
                        ); 
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_WriteMicData bytes_t=%d",bytes_t));
    }
    else
    {
        CATMVDEMO_ASSERT(s_catmv_file_handle!= NULL);/*assert verified*/
    }
}

LOCAL uint32 CatMvDEMO_ReadMicData(int32* pData, uint32 rt_siez)
{  
    uint32 bytes_t= 0; 
    CATMVDEMO_ASSERT(s_catmv_file_handle!= NULL); /*assert verified*/
    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_ReadMicData enter" ));

    if(SFS_ERROR_NONE !=SFS_ReadFile(s_catmv_file_handle,
                (uint8 *)pData,
                rt_siez,
                &bytes_t,
                PNULL
                )) 
    {
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_ReadMicData, bytes_t=%d", bytes_t));
        return bytes_t;
    }

    return bytes_t;
}

LOCAL void CatMvDEMO_CloseWriteMicFile(void)
{  
    CATMVDEMO_ASSERT(s_catmv_file_handle!= NULL);/*assert verified*/
    CATMVDEMO_ASSERT(0==SFS_CloseFile(s_catmv_file_handle));/*assert verified*/
      
    s_catmv_file_handle = NULL;
}



/*****************************************************************************/
//  Description:    This function is used to construct before play.
//  Author:         Cherry.Liu
//  Note:           
/*****************************************************************************/

LOCAL void CatMvDEMO_PlayConstruct (CatMv_OBJECT_T* ptCatMvObj)
{ 

    /*-------------create static task -------------*/ 
    uint32 uiPriority = 0;
        
    // other init
    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_PlayConstruct"));
    
    CatMvDEMO_OpenWriteFile();
    CatMv_EnableCatMicCodec(SCI_TRUE); 
    s_dec_ctx.demo_state = RECORD;

    uiPriority=TASK_CATMV_PRI;   
    
    hcatmv_demo_thread = SCI_CreateAppThread(
            "T_CatMvDEMO",           // Name string of the thread
            "Q_CatMvDEMO",            // Queue name string of the thread
            CatMvDEMO_Thread_Entry,    // Entry function of the thread
            0,                       // First parameter for entry function,
            (void*)ptCatMvObj,       // Second parameter for entry function,
            CATMV_STACK_SIZE,          // Size of the thread stack in bytes
            CATMV_QUEUE_NUM,           // Number of messages which can be enqueued
            uiPriority,             // Prority of the thread.
            SCI_PREEMPT,            // Indicates if the thread is preemptable.
            SCI_AUTO_START
            );

    if(AUDIO_DEFAULT_TASK_ID != hcatmv_demo_thread)
    {
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_PlayConstruct create  dynamic task!"));
    }
    else
    {
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_PlayConstruct can not create  dynamic task!"));
        CATMVDEMO_ASSERT(0); /*assert verified*/
    }
}


/*****************************************************************************/
//  Description:    This function is used to deconstruct after stop.
//  Author:         Cherry.Liu
//  Note:           
/*****************************************************************************/
LOCAL void CatMvDEMO_StopDeconstruct (CatMv_OBJECT_T* ptCatMvObj )
{
    void*  sig_ptr = SCI_NULL;

    CATMVDEMO_PRINT(("[Cat_mv_demo]:CatMvDEMO_StopDeconstruct CatMv_demo hcatmv_demo_thread=%d",hcatmv_demo_thread));
    /*-------------------check input paras ------------------*/
    if(SCI_NULL == ptCatMvObj) 
    {
      CATMVDEMO_PASSERT(SCI_FALSE,("SCI_NULL == ptCatMvObj.")); /*assert verified*/   
      //return ;
    }
    
    CATMVDEMO_ASSERT(AUDIO_DEFAULT_TASK_ID != hcatmv_demo_thread);/*assert verified*/
 
    /*----------------------delete task----------------------*/
    //to delete codec task.
    if(ptCatMvObj->ppvSigIn != SCI_NULL)
    {
        if(*(ptCatMvObj->ppvSigIn) != SCI_NULL)      // Free the processing signal
        {
            CatMvDEMO_FREE(*(ptCatMvObj->ppvSigIn));
        }
        
        ptCatMvObj->ppvSigIn = SCI_NULL;
    }

    // Empty the signal queue.
    while (1)//lint !e716
    {
        sig_ptr = (void*)SCI_PeekSignal(hcatmv_demo_thread);
        if (sig_ptr != SCI_NULL)
        {
            CatMvDEMO_FREE(sig_ptr);
        }
        else
        {
            break;
        }
    }
    // Free the signal space which has been alloced but not free.
    if(ptCatMvObj->ppvSigOut != SCI_NULL)
    {
        if(*(ptCatMvObj->ppvSigOut) != SCI_NULL)
        {
            CatMvDEMO_FREE(*(ptCatMvObj->ppvSigOut));
        }
        
        ptCatMvObj->ppvSigOut = SCI_NULL;
    }
    
    SCI_TerminateThread(hcatmv_demo_thread);// must terminate it for deleting.
    SCI_DeleteThread(hcatmv_demo_thread);  // Delete tha audio task.
    hcatmv_demo_thread = AUDIO_DEFAULT_TASK_ID;
       
    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_StopDeconstruct hcatmv_demo_thread=%d",hcatmv_demo_thread));
}

/*****************************************************************************/
//  Description:    This function starts to play .
//  Author:         Cherry.Liu
//  Note:           
/*****************************************************************************/
LOCAL void CatMvDEMO_Play( void)
{   
    int result = 0;
    
    CATMVDEMO_PRINT(("[Cat_mv_demo]:CatMvDEMO_Play hcatmv_demo_thread=%d",hcatmv_demo_thread));
    

    if(AUDIO_DEFAULT_TASK_ID==hcatmv_demo_thread) 
    {
        return;
    }
    
    s_dec_ctx.stop_flag=0;
    data_mod=0;
    //  s_data_scale=0;
    /*---Create a signal and send it to codec task to play the music--*/
    result = CatMvDEMO_SendSignal(hcatmv_demo_thread,CatMvDEMO_START_MICSAMPLE_REQ,SCI_NULL);

}
 
/*****************************************************************************/
//  Description:   This function is used to stop  play.  
//  Author:        Cherry.Liu
//  Note:           
/*****************************************************************************/
LOCAL void CatMvDEMO_Stop( void)
{   
    int result = 0;

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_Stop, hcatmv_demo_thread=%d",hcatmv_demo_thread));
    
    if(AUDIO_DEFAULT_TASK_ID==hcatmv_demo_thread) 
    {
       CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_Stop, error"));
        return;
    } 

    result = CatMvDEMO_SendSignal(hcatmv_demo_thread,CatMvDEMO_STOP_REQ,SCI_NULL);

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_Stop, result=%d",result));
    
}


      
/*****************************************************************************/
//  Description:    This function sends a signal.
//  Author:         Cherry.Liu
//  Note:           
/*****************************************************************************/
LOCAL int CatMvDEMO_SendSignal(
    BLOCK_ID tThreadId,
    CatMvDEMO_SIGNAL_CODE_E eSigCode,
    void* pSigPara
)
{   
    int ret=AUDIO_NO_ERROR;
    /*-------------------check input paras ------------------*/
    if(AUDIO_DEFAULT_TASK_ID == tThreadId) 
    {
        return 1;
    }

    /*-----Create a signal and send it to codec task-------*/
    switch(eSigCode)
    {
        case CatMvDEMO_PLAY_REQ:
            {   
                CatMvDEMO_DECODE_REQ_MSG_T* sig_ptr = SCI_NULL;
                sig_ptr = (CatMvDEMO_DECODE_REQ_MSG_T *)SCI_ALLOC_APP(sizeof(CatMvDEMO_DECODE_REQ_MSG_T));
                if(SCI_NULL == sig_ptr)
                {
                    CATMVDEMO_PRINT(("[Cat_mv_demo]:  CatMvDEMO_SendSignal-1- alloc failed!"));
                    return AUDIO_NO_ENOUGH_MEMORY;
                }
                sig_ptr->SignalSize = sizeof(CatMvDEMO_GETSRC_REQ_MSG_T);
                sig_ptr->SignalCode = CatMvDEMO_PLAY_REQ;
                sig_ptr->Sender     = SCI_IdentifyThread();
                
                SCI_SendSignal((xSignalHeader)sig_ptr, tThreadId);
            }
            break;
        case CatMvDEMO_STOP_REQ:
            {
                CatMvDEMO_DECODE_REQ_MSG_T* sig_ptr = SCI_NULL;               
                sig_ptr = (CatMvDEMO_DECODE_REQ_MSG_T *)SCI_ALLOC_APP(sizeof(CatMvDEMO_DECODE_REQ_MSG_T));
                if(SCI_NULL == sig_ptr)
                {
                    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_SendSignal-2-  alloc failed!"));
                    return AUDIO_NO_ENOUGH_MEMORY;
                }
                sig_ptr->SignalSize = sizeof(CatMvDEMO_DECODE_REQ_MSG_T);
                sig_ptr->SignalCode = CatMvDEMO_STOP_REQ;
                sig_ptr->Sender     = SCI_IdentifyThread();
                
                
                SCI_SendSignal((xSignalHeader)sig_ptr, tThreadId);
            }
            break;

       
        case CatMvDEMO_START_MICSAMPLE_REQ:
            {
                CatMvDEMO_DECODE_REQ_MSG_T* sig_ptr = SCI_NULL;
                sig_ptr = (CatMvDEMO_DECODE_REQ_MSG_T *)SCI_ALLOC_APP(sizeof(CatMvDEMO_DECODE_REQ_MSG_T));
                if(SCI_NULL == sig_ptr)
                {
                    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_SendSignal-3-  alloc failed!"));
                    return AUDIO_NO_ENOUGH_MEMORY;
                }
                sig_ptr->SignalSize = sizeof(CatMvDEMO_DECODE_REQ_MSG_T);
                sig_ptr->SignalCode = CatMvDEMO_START_MICSAMPLE_REQ;
                sig_ptr->Sender     = SCI_IdentifyThread();
   
                SCI_SendSignal((xSignalHeader)sig_ptr, tThreadId);
            }
            break;
        case CatMvDEMO_PLAY_END_REQ:
            {
                CatMvDEMO_DECODE_REQ_MSG_T* sig_ptr = SCI_NULL;
                sig_ptr = (CatMvDEMO_DECODE_REQ_MSG_T *)SCI_ALLOC_APP(sizeof(CatMvDEMO_DECODE_REQ_MSG_T));
                if(SCI_NULL == sig_ptr)
                {
                    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_SendSignal-4-  alloc failed!"));
                    return AUDIO_NO_ENOUGH_MEMORY;
                }
                sig_ptr->SignalSize = sizeof(CatMvDEMO_DECODE_REQ_MSG_T);
                sig_ptr->SignalCode = CatMvDEMO_PLAY_END_REQ;
                sig_ptr->Sender     = SCI_IdentifyThread();
   
                SCI_SendSignal((xSignalHeader)sig_ptr, tThreadId);
            }
            break;
        default:
            {
                //CATMVDEMO_ASSERT(SCI_FALSE);
                ret=1;
            }
            break;
    }

    return ret; 
}


/*****************************************************************************/
//  Description:    CatMvDEMO_Thread_Entry 
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL void CatMvDEMO_Thread_Entry(        
    uint32 argc,     
    void* ptAudioObj  
)
{

    CatMv_OBJECT_T* audio_obj = (CatMv_OBJECT_T*)ptAudioObj;
    CatMvDEMO_TASK_MSG_U* sig_in_ptr  = SCI_NULL;
    CatMvDEMO_TASK_MSG_U* sig_out_ptr = SCI_NULL;    
    uint32 base_time_count=0;
    uint32 dt_time_count=0;
    uint32 data_ret_value=0;
    // Memorize the sig_in_ptr and sig_out_ptr. 
    audio_obj->ppvSigIn = (void**)&sig_in_ptr;     
    audio_obj->ppvSigOut = (void**)&sig_out_ptr;
    
    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_Thread_Entry"));

    while (1)//lint !e716
    {  
        // Get signal.
        sig_in_ptr = (CatMvDEMO_TASK_MSG_U*)SCI_GetSignal(SCI_IdentifyThread());

        // Process it according to the signal code.
        switch (((CatMvDEMO_DECODE_REQ_MSG_T *)sig_in_ptr)->SignalCode)
        { 
            case CatMvDEMO_START_MICSAMPLE_REQ://start mic sample
                {    
                    CATMVDEMO_PRINT(("[Cat_mv_demo]:CatMvDEMO_Thread_Entry,CatMvDEMO_START_MICSAMPLE_REQ"));
                    //Start mic sample      
                     data_ret_value=CatMv_Dataprocess(CATMV_END_TYPE_AUTO_TIME,20, 5, 5, (uint32*)(&s_data_scale));

                     CATMVDEMO_PRINT(("[Cat_mv_demo]:CatMvDEMO_Thread_Entry,CatMvDEMO_START_MICSAMPLE_REQ data_ret_value=%d",data_ret_value));

                     if(data_ret_value==1)//start record and save the valid data 
                     {
                        // SAVE TO buffer and then write to file 
                       CatMvDEMO_SendSignal(hcatmv_demo_thread,CatMvDEMO_START_MICSAMPLE_REQ,NULL);
                     }
                     else if(data_ret_value==2)// Only ready record.
                     {
                         //start record and have no valid data
                       CatMvDEMO_SendSignal(hcatmv_demo_thread,CatMvDEMO_START_MICSAMPLE_REQ,NULL);
                     }
                     else //stop record
                     {   
                        base_time_count = SCI_GetTickCount();
                        CatMvDEMO_SendSignal(hcatmv_demo_thread,CatMvDEMO_PLAY_REQ,NULL);
                     }

                }
                break;

             case CatMvDEMO_PLAY_END_REQ:
                        base_time_count = SCI_GetTickCount();
                        CatMv_Data_Stop();
                        CatMvDEMO_CloseWriteMicFile();
#ifdef DEBUG_OUT_FILE
                        CatMvDEMO_CloseMvoiceFile();
                        play_count++;
#endif
                        CatMvDEMO_OpenWriteFile();

                        CatMv_EnableCatMicCodec(SCI_TRUE);    
                        s_dec_ctx.demo_state=RECORD;
                        s_dec_ctx.end_flag=0;
                        CatMvDEMO_SendSignal(hcatmv_demo_thread,CatMvDEMO_START_MICSAMPLE_REQ,NULL);                          
                        dt_time_count = SCI_GetTickCount()-base_time_count;
                        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_Thread_Entry, TIME play end--record dt_time_count=%d",dt_time_count));
                break;    

            case CatMvDEMO_PLAY_REQ: //play music
                   if(s_dec_ctx.demo_state != IDLE)
                   {
                        base_time_count = SCI_GetTickCount(); 

                        CatMv_EnableCatMicCodec(SCI_FALSE);
                        CatMvDEMO_CloseWriteMicFile();
                        CatMvDEMO_OpenReadMicFile();

                        CatMv_Data_Play();
                        s_dec_ctx.demo_state=DEMOPLAY;	

                        dt_time_count = SCI_GetTickCount()-base_time_count;

                        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_Thread_Entry,TIME record end--play dt_time_count=%d",dt_time_count));

                        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_Thread_Entry,CatMvDEMO_PLAY_REQ"));
                    }
                break;

            case CatMvDEMO_STOP_REQ: //stop music and mic sample
                    {  
                        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_Thread_Entry,CatMvDEMO_STOP_REQ"));    

                        if(s_dec_ctx.demo_state==RECORD)
                        {
                            CatMv_EnableCatMicCodec(SCI_FALSE);
                            CatMvDEMO_CloseWriteMicFile();
                        }
                        else if(s_dec_ctx.demo_state==DEMOPLAY) 
                        {
                            CatMv_Data_Stop();
#ifdef DEBUG_OUT_FILE
                            CatMvDEMO_CloseMvoiceFile();
#endif
                            CatMvDEMO_CloseWriteMicFile();
                        }


                        s_dec_ctx.demo_state=IDLE; 
                        s_dec_ctx.stop_flag=1;
                    }
                break;

            default:
                {
                        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMvDEMO_Thread_Entry,other invalid signal code!"));
                }
                break;
        }  
        // Free signal.
        CatMvDEMO_FREE(sig_in_ptr);
    }
}


PUBLIC void CatMv_Demo_Start(void)
{ 

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Demo_Start, s_dec_ctx.demo_state=%d",s_dec_ctx.demo_state));

    if(s_dec_ctx.demo_state != IDLE)
    {
        return ;
    }

    MV_init();

    s_dec_ctx.stop_flag=0;
    s_dec_ctx.demo_state = IDLE;//RECORD;

    CatMv_RegCallback(CatMvDEMO_WriteMicData, MV_Cat_Process,(uint16*)catmv_data_buf);
    CatMv_SetPara(17,3);
    CatMv_RegExpOnOff(SCI_TRUE);

    CatMvDEMO_PlayConstruct(&pt_CatMvObj);
    CatMvDEMO_Play();

}

/*****************************************************************************/
//  Description : API of  stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void CatMv_Demo_Stop(void)
{

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Demo_Stop, s_dec_ctx.demo_state=%d",s_dec_ctx.demo_state));
      
    if (s_dec_ctx.demo_state == IDLE)
    {
       return;
    }
    else
    {
        s_dec_ctx.stop_flag=0;
        CatMvDEMO_Stop();
        do
        {
            SCI_Sleep(5);
            
        }while(s_dec_ctx.stop_flag==0);
    
        CatMvDEMO_StopDeconstruct(&pt_CatMvObj);
     
        MV_deinit();
        CatMv_RegExpOnOff(SCI_FALSE);

        s_dec_ctx.demo_state = IDLE;
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Demo_Stop out")); 	
    }    
}


#define CATMV_RECORD_PLAY
// for wav stream play and stop
/**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*/                                                   
#define PCM_DRIVER_BUF_SIZE        (80*150)

extern uint32      VBDecodeOuputBuf[];
//extern HAUDIOCODEC hWAVCodec;
//extern HAUDIODEV   hARMVB;

LOCAL  uint8  demo_source[2048]; //play amr_stream data.
LOCAL  WAV_STREAM_BUFFER_T wav_header_info = {0};

/*****************************************************************************/
//  Description :call back function for wav driver
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E pcm_callback( 
    HAUDIO hAudio, 
    AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
    void *pvOtherInfo
    )
{

    uint32 puiDataLength=0;

    SCI_ASSERT(ptSrcDataInfo!=PNULL); /*assert verified*/
    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Play_Open, callback"));

    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = PNULL;  

    if (s_dec_ctx.play_state != DECODING)
    {
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Play_Open, callback, not in palying status."));
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
        return AUDIO_STREAM_GET_SRC_ERROR;
    }
    else
    {
        ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = demo_source;	

        if(s_dec_ctx.end_flag==0)
        {
            puiDataLength=CatMvDEMO_ReadMicData((int32*)demo_source,1024);  	    
        }
        else
        {
            puiDataLength=0;
            ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =puiDataLength;

            CATMVDEMO_PRINT(("lhhdy [Cat_mv_demo]: CatMv_Play_Open, p_buffer= 0x%x, i_len=%d, end_flag=%d", demo_source, puiDataLength,s_dec_ctx.end_flag));

            return AUDIO_STREAM_GET_SRC_ERROR;
        }
          

        if(puiDataLength<1024)
        {
            s_dec_ctx.end_flag=1;

            if(puiDataLength==0)
            {
                ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =puiDataLength;
                CATMVDEMO_PRINT(("lhhdy [Cat_mv_demo2]: CatMv_Play_Open, p_buffer= 0x%x, i_len=%d, end_flag=%d", demo_source, puiDataLength,s_dec_ctx.end_flag));
                return AUDIO_STREAM_GET_SRC_ERROR;
            }
            
        }
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =puiDataLength;
    }

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Play_Open, p_buffer= 0x%x, i_len=%d, end_flag=%d", demo_source, puiDataLength,s_dec_ctx.end_flag));

    return AUDIO_STREAM_GET_SRC_SUCCESS;
 
}

/*****************************************************************************/
//  Description :call back function for wav play
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void cat_pcm_dec_dummy_callbak(
                HAUDIO haudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //主要处理播放结束
    CATMVDEMO_PRINT(("[Cat_mv_demo]: AudioNotifyCallback:enter:hAudio = 0x%X, notify_info = 0x%X, affix_info = %ld",
    haudio, notify_info, affix_info));

    CATMVDEMO_PRINT(("[Cat_mv_demo]: AudioNotifyCallback:enter:AUDIO_PLAYEND_IND = 0x%X",AUDIO_PLAYEND_IND));

    if(AUDIO_PLAYEND_IND == notify_info && s_dec_ctx.play_state == DECODING)
    {
        CATMVDEMO_PRINT(("[Cat_mv_demo]: cat_pcm_dec_dummy_callbak, AudioNotifyCallback:enter"));
        s_dec_ctx.end_flag=1;
        CatMvDEMO_SendSignal(hcatmv_demo_thread,CatMvDEMO_PLAY_END_REQ,NULL);  
    }
    else
    {
        return;
    }
}

/*****************************************************************************/
//  Description : API of CatMv_Play_Open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CatMv_Data_Open(void)
{
    WAV_STREAM_FORMATINFO_T* wav_format_info_ptr = PNULL;
    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Play_Open"));

    if (s_dec_ctx.play_state == DECODING)
    {
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Play_Open, last operation first not stop"));
        return;
    }
    
    wav_header_info.bType  = SCI_TRUE;
    wav_format_info_ptr = &(wav_header_info.unWavStreamData.tWavFormatInfo);
#if 0
    wav_format_info_ptr->eSubtype = WAV_PCM;
    wav_format_info_ptr->uiBitPerSample  =  16;
    wav_format_info_ptr->uiBlockAlign    = 2;//4
    wav_format_info_ptr->uiChannelNum    = 1;
    wav_format_info_ptr->uiDataSize      =0x1ae08;
    wav_format_info_ptr->uiSampleRate    = MIC_SAMPLERATE;

#else
    wav_format_info_ptr->eSubtype =WAV_PCM_BIGENDIAN;//WAV_PCM;
    wav_format_info_ptr->uiBitPerSample  = 16; // 8;
    wav_format_info_ptr->uiBlockAlign    = 2;//4
    wav_format_info_ptr->uiChannelNum    = 1;
    wav_format_info_ptr->uiDataSize      =0x1ae08;
    wav_format_info_ptr->uiSampleRate    = MIC_SAMPLERATE; //8000;
#endif

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Play_Open, uiBitPerSample[%d],uiBlockAlign[%d],uiChannelNum[%d],uiDataSize[%d],uiSampleRate[%d],eSubtype[%d]",
                                        wav_format_info_ptr->uiBitPerSample, wav_format_info_ptr->uiBlockAlign, wav_format_info_ptr->uiChannelNum,
                                        wav_format_info_ptr->uiDataSize, wav_format_info_ptr->uiSampleRate, wav_format_info_ptr->eSubtype));

    s_dec_ctx.pcm_handle = AUDIO_CreateStreamBufferHandle(
                                                            hWAVCodec,
                                                            PNULL,
                                                            hARMVB,
                                                            PNULL,
                                                            (uint8 *)&wav_header_info,
                                                            sizeof(WAV_STREAM_BUFFER_T),
                                                            (uint32 *)(VBDecodeOuputBuf),
                                                            PCM_DRIVER_BUF_SIZE,
                                                            cat_pcm_dec_dummy_callbak,
                                                            pcm_callback);

    if ( !s_dec_ctx.pcm_handle)
    {
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Play_Open, create handle fail!"));
        return ;
    }

    AUDIO_Play(s_dec_ctx.pcm_handle, 0);

    s_dec_ctx.play_state = DECODING;

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Play_Open start"));	

    return;
}

/*****************************************************************************/
//  Description : API of  Cat_Demo play  
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
LOCAL void CatMv_Data_Play(void)
{
    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Data_Play enter"));  

    s_dec_ctx.stop_flag=0;
    s_dec_ctx.end_flag=0;
    s_dec_ctx.pcm_handle=0; 

    CatMv_Data_Open();
    AUDIO_SetVolume(6);
}

/*****************************************************************************/
//  Description : API of  Cat_Demo stop  
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
LOCAL void CatMv_Data_Stop(void)
{
    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_Data_Stop enter,s_dec_ctx.play_state=%d",s_dec_ctx.play_state));

    if (s_dec_ctx.play_state != IDLE)
    {
        AUDIO_Stop(s_dec_ctx.pcm_handle);
        s_dec_ctx.play_state = IDLE;
    }

    if (s_dec_ctx.pcm_handle)
    {
        AUDIO_CloseHandle(s_dec_ctx.pcm_handle);
        s_dec_ctx.pcm_handle = NULL;
    }
}

LOCAL void CatMvDummyNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}

/*****************************************************************************/
//  Description:    This function start demo.
//  Author:         hai.li
//	Note:           
/*****************************************************************************/
LOCAL void CatMv_EnableCatMicCodec(  // If succeed, return 1,
                                    // else return 0
    BOOLEAN is_enable               // SCI_TRUE: Enable,   SCI_FALSE: Disable
    )
{     
    static char catmv_mic_play_flag=0;

    CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_EnableCatMicCodec is_enable = %d", is_enable));

    if(hCatMvsample == 0)
    {
        hCatMvsample=	AUDIO_CreateNormalHandle(hCatMvSampleCodec,SCI_NULL, hARMVBRECORD,SCI_NULL, CatMvDummyNotifyCallback);
        CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_EnableCatMicCodec CreateHandle = 0x%08x", (int)hCatMvsample));
    }

    if(catmv_mic_play_flag==1)
    {
        if(is_enable)
        {
            CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_EnableCatMicCodec have open!!"));
            return ;
        }
    }
    else
    {
        if(!is_enable)
        {
            CATMVDEMO_PRINT(("[Cat_mv_demo]: CatMv_EnableCatMicCodec have close!!"));
            return ;
        }
    }


    if(is_enable)
    {       
        AUDIO_Play(hCatMvsample, 0 );
        AUDIO_SetVolume(5);
        catmv_mic_play_flag=1;
    }
    else
    {
        AUDIO_Stop(hCatMvsample);
        catmv_mic_play_flag=0;
    }  
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif   

#endif /* CAtMV_SUPPORT */
