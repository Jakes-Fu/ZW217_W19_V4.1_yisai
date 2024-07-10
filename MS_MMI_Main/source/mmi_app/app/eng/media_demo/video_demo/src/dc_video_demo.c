/****************************************************************************
** File Name:      DC_video_demo.c                                             *
** Author:                                                                   *
** Date:           11/10/2010                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    the file CATMV_DEOM    
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2010        hai.li          Create
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_eng_trc.h"
#include "sci_api.h"
#include "os_api.h"
#include "sensor_drv.h"
#include "jpeg_interface.h"
#include "deep_sleep.h"
#include "dc_common_6600l.h"
#include "dc_app_6x00.h"
#include "sci_api.h"
#include "os_api.h"
#include "deep_sleep.h"
#include "tasks_id.h"
//#include "sig_code.h"
#include "dc_misc.h"
#include "dc_bufmgr.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C"
    {
#endif

#define DC_VIDEO_DEMO_ASSERT_MODE

#ifdef DC_VIDEO_DEMO_ASSERT_MODE
    #define DCVIDEODEMO_ASSERT    SCI_ASSERT
    #define DCVIDEODEMO_PASSERT   SCI_PASSERT 
#else
    #define DCVIDEODEMO_ASSERT  
    #define DCVIDEODEMO_PASSERT 
#endif


#define DC_VIDEO_DEMO_DEBUG

#ifdef DC_VIDEO_DEMO_DEBUG
    #ifndef DCVIDEODEMO_PRINT


        #define DCVIDEODEMO_PRINT(format) SCI_TRACE_LOW format
    #endif
#else
    #ifndef DCVIDEODEMO_PRINT


        #define DCVIDEODEMO_PRINT(format)	
    #endif    
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#ifndef DCVIDEODEMO_FREE
	#define DCVIDEODEMO_FREE(PTR)	SCI_DisableIRQ(); SCI_FREE(PTR); SCI_RestoreIRQ();
#endif


#define IDLE          0
#define PLAY          1

#define DCVIDEODEMO_STACK_SIZE (2 * 1024)
#define DCVIDEODEMO_QUEUE_NUM  (10)
#define DCVIDEODEMO_DEFAULT_TASK_ID (0xFFFFFFFF)
//#define DC_RIGHT_TRUNK(x, y)    (((x)+(y)-1)/(y)*(y))
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
}DCVIDEO_DEMO_GETSRC_REQ_MSG_T;

/*! \struct DCVIDEO_DEMO_REQ_MSG_T
 *  \brief Request to decode src data(codec->codec).
 */
typedef struct
{
    SIGNAL_VARS        /*!<Signal Header>*/
}DCVIDEO_DEMO_REQ_MSG_T;


/*! \union CatMvDEMO_Task_Msg_union
 *  \brief Collect all IIS related messages' struct definition.
 */
typedef union CatMvDEMO_Task_Msg_union
{
    DCVIDEO_DEMO_REQ_MSG_T  tDecodeReq;
    DCVIDEO_DEMO_GETSRC_REQ_MSG_T  tGerSrcReq;
}DCVIDEODEMO_TASK_MSG_U;

/*! signal code definition
 *  
 */
typedef enum DCVIDEODEMO_SIGNAL_CODE_enum
{
    DCVIDEODEMO_PLAY_REQ,   //as -> codec
    DCVIDEODEMO_STOP_REQ   //as -> codec  codec -> codec
}DCVIDEODEMO_SIGNAL_CODE_E;

typedef struct 
{
    void** ppvSigIn;    /*!< Signal_in pointer that audio task received. We should free it when audio task is deleted.*/
    void** ppvSigOut;   /*!< Signal_out pinter that audio task prepared to send out when audio task is deleted. We should free it.*/	
}DCVIDEO_OBJECT_T;



LOCAL void DCVIDEODEMO_Thread_Entry(        
    uint32 argc,     
    void* ptDcVideoObj  
);

LOCAL int DCVIDEODEMO_SendSignal(
    BLOCK_ID tThreadId,
    DCVIDEODEMO_SIGNAL_CODE_E eSigCode,
    void* pSigPara
);


/*******************************************************************************
 *  type declare
 *******************************************************************************/
typedef struct
{
    uint16   demo_state;
    uint16   stop_flag;
}PCM_DEC_CTX_T;

/*******************************************************************************
 *  static variable
 *******************************************************************************/
extern PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void);

LOCAL BLOCK_ID hdcvideo_demo_thread=0;
LOCAL DCVIDEO_OBJECT_T pt_CatMvObj;
LOCAL PCM_DEC_CTX_T  s_dec_ctx = {0};


/*****************************************************************************/
//  Description:    This function is used to init dc.
//  Author:         
//  Note:           
/*****************************************************************************/
void DCVIDEODEMO_Sensor_Init(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    DCAMERA_ExtOpenSensor(DCAMERA_IMG_SENSOR_MAIN,DCAMERA_VIDEO_MODE);
    DC_Malloc(DC_STATE_CAPTURING); 
    //DC_Malloc(DC_STATE_VIDEO_REVIEW); 
    
    pContext=NULL; //for pclint
}

/*****************************************************************************/
//  Description:    This function is used to deconstruct buffer after stop.
//  Author:         
//  Note:           
/*****************************************************************************/
void DCVIDEODEMO_Sensor_Close(void)
{
 //   _DCAMRscFree(NULL);
	MM_Exit();
	DCAMERA_ExtCloseSensor();
}

/*****************************************************************************/
//  Description:    This function is used to get and show sensor data.
//  Author:         
//  Note:           
/*****************************************************************************/
void DCVIDEODEMO_GetData_And_Show(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    DCAMERA_EXT_GET_DATA_T get_data_buf={0x00};
    DCAMERA_EXT_DISPLAY_T display_param={0x00};
    DCAMERA_EXT_GET_DATA_T_PTR get_data_ptr=&get_data_buf;
    DCAMERA_YUV_CHN_ADDR_T rtn_image_addr={0x00};
    DCAMERA_SIZE_T rtn_size={0x00};
	

    get_data_ptr->data_format=DCAMERA_DATA_TYPE_YUV422;
    get_data_ptr->img_size.w=96;
    get_data_ptr->img_size.h=96;
    
    get_data_ptr->is_display=SCI_TRUE;
    get_data_ptr->display_rect.x=20;
    get_data_ptr->display_rect.y=130;
    get_data_ptr->display_rect.w=96;
    get_data_ptr->display_rect.h=96;
    
    get_data_ptr->target_rect.x=0;
    get_data_ptr->target_rect.y=0;
    get_data_ptr->target_rect.w=96;
    get_data_ptr->target_rect.h=96;
    get_data_ptr->lcd_id=MAIN_LCD_ID;

  
    get_data_ptr->buf_addr=(uint32)pContext->jpeg_buf_ptr;
 
    //get_data_ptr->buf_len=300*1024;
    get_data_ptr->buf_len=DC_RIGHT_TRUNK((350*1024),256);
    get_data_ptr->display_rotation=DCAMERA_ROTATION_0;


    display_param.src_format = DCAMERA_DATA_TYPE_YUV422;
    display_param.src_img_size.w = 96;//192;//160;
    display_param.src_img_size.h  = 96;//144;//120;

    display_param.display_rect.x= 124;
    display_param.display_rect.y =130; 
    display_param.display_rect.w = 96;
    display_param.display_rect.h = 96;


    display_param.target_rect.x=0;
    display_param.target_rect.y=0;
    display_param.target_rect.w=96;
    display_param.target_rect.h=96;

    display_param.display_addr=(get_data_ptr->buf_addr+  get_data_ptr->buf_len);//(ext_buf_addr+(400*1024));
    display_param.display_buf_len=DC_RIGHT_TRUNK( (display_param.display_rect.w*display_param.display_rect.h),256)*2;

    display_param.rotation_addr=display_param.display_addr+display_param.display_buf_len;
    display_param.lcd_id =MAIN_LCD_ID;
    display_param.display_rotation=DCAMERA_ROTATION_0;

    //while(1)
    {
         //DCVIDEODEMO_PRINT:"[dc_video_demo]: GetData size=%d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_233_112_2_18_2_18_26_322,(uint8*)"d",sizeof(DCAMERA_EXT_GET_DATA_T));
    	 DCAMERA_ExtGetData(get_data_ptr, &rtn_image_addr, &rtn_size);
         display_param.src_yuv_addr=rtn_image_addr;
	     display_param.src_yuv_buf_len=DC_RIGHT_TRUNK(rtn_size.w*rtn_size.h,256)*2;
         DCAMERA_ExtDisplay(&display_param);
    }
   // SCI_Sleep(15);
}


/*****************************************************************************/
//  Description:    This function is used to construct before play.
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL void DCVIDEODEMO_PlayConstruct (DCVIDEO_OBJECT_T* ptCatMvObj)
{ 

/*-------------create static task -------------*/ 
    uint32 uiPriority = 0;
        
    // other init
    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_PlayConstruct"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_252_112_2_18_2_18_26_323,(uint8*)"");
    
   
    uiPriority=PRI_CATMV_TASK;   
    
     hdcvideo_demo_thread = SCI_CreateAppThread(
            "T_CatMvDEMO",           // Name string of the thread
            "Q_CatMvDEMO",            // Queue name string of the thread
            DCVIDEODEMO_Thread_Entry,    // Entry function of the thread
            0,                       // First parameter for entry function,
            (void*)ptCatMvObj,       // Second parameter for entry function,
            DCVIDEODEMO_STACK_SIZE,          // Size of the thread stack in bytes
            DCVIDEODEMO_QUEUE_NUM,           // Number of messages which can be enqueued
            uiPriority,             // Prority of the thread.
            SCI_PREEMPT,            // Indicates if the thread is preemptable.
            SCI_AUTO_START
            );

    if(DCVIDEODEMO_DEFAULT_TASK_ID != hdcvideo_demo_thread)
    {
    	 //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_PlayConstruct create  dynamic task!"
    	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_272_112_2_18_2_18_26_324,(uint8*)"");
    }
    else
    {
	     //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_PlayConstruct can not create  dynamic task!"
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_276_112_2_18_2_18_26_325,(uint8*)"");
		 DCVIDEODEMO_ASSERT(0); /*assert verified*/
    }
}


/*****************************************************************************/
//  Description:    This function is used to deconstruct after stop.
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL void DCVIDEODEMO_StopDeconstruct (DCVIDEO_OBJECT_T* ptCatMvObj )
{
   void*  sig_ptr = SCI_NULL;

   //DCVIDEODEMO_PRINT:"[dc_video_demo]:DCVIDEODEMO_StopDeconstruct CatMv_demo hdcvideo_demo_thread=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_289_112_2_18_2_18_26_326,(uint8*)"d",hdcvideo_demo_thread);
/*-------------------check input paras ------------------*/
    if(SCI_NULL == ptCatMvObj) 
    {
      DCVIDEODEMO_PASSERT(SCI_FALSE,("SCI_NULL == ptCatMvObj.")); /*assert verified*/   
    }
    
    DCVIDEODEMO_ASSERT(DCVIDEODEMO_DEFAULT_TASK_ID != hdcvideo_demo_thread);/*assert verified*/
 
    /*----------------------delete task----------------------*/
    //to delete codec task.
    if(ptCatMvObj->ppvSigIn != SCI_NULL)
    {
        if(*(ptCatMvObj->ppvSigIn) != SCI_NULL)      // Free the processing signal
        {
            DCVIDEODEMO_FREE(*(ptCatMvObj->ppvSigIn));
        }
        
        ptCatMvObj->ppvSigIn = SCI_NULL;
    }

    // Empty the signal queue.
    do
    {
        sig_ptr = (void*)SCI_PeekSignal(hdcvideo_demo_thread);
        if (sig_ptr != SCI_NULL)
        {
            DCVIDEODEMO_FREE(sig_ptr);
        }
    }while (sig_ptr != SCI_NULL);
    
    // Free the signal space which has been alloced but not free.
    if(ptCatMvObj->ppvSigOut != SCI_NULL)
    {
        if(*(ptCatMvObj->ppvSigOut) != SCI_NULL)
        {
            DCVIDEODEMO_FREE(*(ptCatMvObj->ppvSigOut));
        }
        
        ptCatMvObj->ppvSigOut = SCI_NULL;
    }
    
    SCI_TerminateThread(hdcvideo_demo_thread);// must terminate it for deleting.
    SCI_DeleteThread(hdcvideo_demo_thread);  // Delete tha audio task.
    hdcvideo_demo_thread = DCVIDEODEMO_DEFAULT_TASK_ID;
       
    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_StopDeconstruct hdcvideo_demo_thread=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_334_112_2_18_2_18_26_327,(uint8*)"d",hdcvideo_demo_thread);
}

/*****************************************************************************/
//  Description:    This function starts to play .
//  Author:         Cherry.Liu
//	Note:           
/*****************************************************************************/
LOCAL void DCVIDEODEMO_Play( void)
{   
    int result = 0;
    
    //DCVIDEODEMO_PRINT:"[dc_video_demo]:DCVIDEODEMO_Play hdcvideo_demo_thread=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_344_112_2_18_2_18_26_328,(uint8*)"d",hdcvideo_demo_thread);
    

    if(DCVIDEODEMO_DEFAULT_TASK_ID==hdcvideo_demo_thread) 
    {
        return;
    }
       
    /*---Create a signal and send it to codec task to play the music--*/
    result = DCVIDEODEMO_SendSignal(hdcvideo_demo_thread,DCVIDEODEMO_PLAY_REQ,SCI_NULL);

}
 
/*****************************************************************************/
//  Description:   This function is used to stop  play.  
//  Author:        Cherry.Liu
//  Note:           
/*****************************************************************************/
LOCAL void DCVIDEODEMO_Stop( void)
{   
    int result = 0;


    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_Stop, hdcvideo_demo_thread=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_365_112_2_18_2_18_26_329,(uint8*)"d",hdcvideo_demo_thread);
    
	if(DCVIDEODEMO_DEFAULT_TASK_ID==hdcvideo_demo_thread) 
    {
       //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_Stop, error"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_369_112_2_18_2_18_26_330,(uint8*)"");
        return;
    } 

    result = DCVIDEODEMO_SendSignal(hdcvideo_demo_thread,DCVIDEODEMO_STOP_REQ,SCI_NULL);

    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_Stop, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_375_112_2_18_2_18_26_331,(uint8*)"d",result);
    
}


      
/*****************************************************************************/
//  Description:    This function sends a signal.
//  Author:         
//	Note:           
/*****************************************************************************/
LOCAL int DCVIDEODEMO_SendSignal(
    BLOCK_ID tThreadId,
    DCVIDEODEMO_SIGNAL_CODE_E eSigCode,
    void* pSigPara
)
{   
    int ret=0;
    /*-------------------check input paras ------------------*/
    if(DCVIDEODEMO_DEFAULT_TASK_ID == tThreadId) 
    {
      return 1;
    }

    /*-----Create a signal and send it to codec task-------*/
    switch(eSigCode)
    {
        case DCVIDEODEMO_PLAY_REQ:
            {   
                DCVIDEO_DEMO_REQ_MSG_T* sig_ptr = SCI_NULL;
                sig_ptr = (DCVIDEO_DEMO_REQ_MSG_T *)SCI_ALLOC_APP(sizeof(DCVIDEO_DEMO_REQ_MSG_T));
                if(SCI_NULL == sig_ptr)
                {
                    //DCVIDEODEMO_PRINT:"[dc_video_demo]:  DCVIDEODEMO_SendSignal-1- alloc failed!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_406_112_2_18_2_18_26_332,(uint8*)"");
                    return 2;
                }
                sig_ptr->SignalSize = sizeof(DCVIDEO_DEMO_REQ_MSG_T);
                sig_ptr->SignalCode = DCVIDEODEMO_PLAY_REQ;
                sig_ptr->Sender     = SCI_IdentifyThread();
                
                SCI_SendSignal((xSignalHeader)sig_ptr, tThreadId);
            }
            break;
        case DCVIDEODEMO_STOP_REQ:
            {
                DCVIDEO_DEMO_REQ_MSG_T* sig_ptr = SCI_NULL;               
                sig_ptr = (DCVIDEO_DEMO_REQ_MSG_T *)SCI_ALLOC_APP(sizeof(DCVIDEO_DEMO_REQ_MSG_T));
                if(SCI_NULL == sig_ptr)
                {
                    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_SendSignal-2-  alloc failed!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_422_112_2_18_2_18_26_333,(uint8*)"");
                    return 2;
                }
                sig_ptr->SignalSize = sizeof(DCVIDEO_DEMO_REQ_MSG_T);
                sig_ptr->SignalCode = DCVIDEODEMO_STOP_REQ;
                sig_ptr->Sender     = SCI_IdentifyThread();

                SCI_SendSignal((xSignalHeader)sig_ptr, tThreadId);
            }
            break;
	
        default:
			{
				//DCVIDEODEMO_ASSERT(SCI_FALSE);
				ret=1;
			}
            break;
    }

    return ret; 
}


/*****************************************************************************/
//  Description:    DCVIDEODEMO_Thread_Entry 
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL void DCVIDEODEMO_Thread_Entry(        
    uint32 argc,     
    void* ptDcVideoObj  
)
{

    DCVIDEO_OBJECT_T* audio_obj = (DCVIDEO_OBJECT_T*)ptDcVideoObj;
    DCVIDEODEMO_TASK_MSG_U* sig_in_ptr  = SCI_NULL;
    DCVIDEODEMO_TASK_MSG_U* sig_out_ptr = SCI_NULL;    
    // Memorize the sig_in_ptr and sig_out_ptr. 
    audio_obj->ppvSigIn = (void**)&sig_in_ptr;     
    audio_obj->ppvSigOut = (void**)&sig_out_ptr;
    
    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_Thread_Entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_461_112_2_18_2_18_26_334,(uint8*)"");
	
    while(1) //lint !e716	
    {  	
        // Get signal.
        sig_in_ptr = (DCVIDEODEMO_TASK_MSG_U*)SCI_GetSignal(SCI_IdentifyThread());

        // Process it according to the signal code.
        switch (((DCVIDEO_DEMO_REQ_MSG_T *)sig_in_ptr)->SignalCode)
        { 			
  				
            case DCVIDEODEMO_PLAY_REQ: 
                 if(s_dec_ctx.demo_state==PLAY)
                 {
		            //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_Thread_Entry,DCVIDEODEMO_PLAY_REQ"
		            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_475_112_2_18_2_18_26_335,(uint8*)"");
                    DCVIDEODEMO_GetData_And_Show();
                    DCVIDEODEMO_SendSignal(hdcvideo_demo_thread,DCVIDEODEMO_PLAY_REQ,NULL);
                  }
            break;
				
            case DCVIDEODEMO_STOP_REQ: 
                    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_Thread_Entry,DCVIDEODEMO_STOP_REQ"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_482_112_2_18_2_18_26_336,(uint8*)"");
		     
		            if(s_dec_ctx.demo_state==PLAY)
		            {
                       DCVIDEODEMO_Sensor_Close();
                       s_dec_ctx.stop_flag=1;
                    }
                    
		            s_dec_ctx.demo_state=IDLE; 

                break;

            default:

				//DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEODEMO_Thread_Entry,other invalid signal code!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_496_112_2_18_2_18_26_337,(uint8*)"");
		   
                break;
        }  
        // Free signal.
        DCVIDEODEMO_FREE(sig_in_ptr);
    }
    
}

/*****************************************************************************/
//  Description : API of  start demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void DCVIDEO_Demo_Start(void)
{ 	

    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEO_Demo_Start, s_dec_ctx.demo_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_512_112_2_18_2_18_26_338,(uint8*)"d",s_dec_ctx.demo_state);

    if(s_dec_ctx.demo_state != IDLE)
	{
		return ;
	}

    DCVIDEODEMO_Sensor_Init();
    s_dec_ctx.demo_state=PLAY;
    s_dec_ctx.stop_flag=0;
	DCVIDEODEMO_PlayConstruct(&pt_CatMvObj);
	DCVIDEODEMO_Play();
	
}

/*****************************************************************************/
//  Description : API of  stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void DCVIDEO_Demo_Stop(void)
{

    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEO_Demo_Stop, s_dec_ctx.demo_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_533_112_2_18_2_18_27_339,(uint8*)"d",s_dec_ctx.demo_state);
	//SCI_ASSERT(0);  
	if (s_dec_ctx.demo_state == IDLE)
	{
       return;
	}
	else
	{	
		s_dec_ctx.stop_flag=0;
		DCVIDEODEMO_Stop();
		do
		{
            SCI_Sleep(15);
            
		}while(s_dec_ctx.stop_flag==0);
		
        DCVIDEODEMO_StopDeconstruct(&pt_CatMvObj);
        
	    s_dec_ctx.demo_state = IDLE;
	    //DCVIDEODEMO_PRINT:"[dc_video_demo]: DCVIDEO_Demo_Stop out"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_VIDEO_DEMO_552_112_2_18_2_18_27_340,(uint8*)"");
	}    
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif   

