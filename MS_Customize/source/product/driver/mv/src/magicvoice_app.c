/******************************************************************************
 ** File Name:      magicvoice.c                                                                *
 ** Author:                                                      *
 ** DATE:                                                          *
 ** Copyright:               *
 ** Description:    .* 
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **                                 *
 ******************************************************************************/
  
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "ms_customize_trc.h"
#include "tb_drv.h"
#include "os_api.h"
#include "dal_audio.h"
#include "vb_drv.h"
//#include "aud_gen.h" //for pclint
#include "adm.h"
#include "layer1_audio.h"
//#include "Sig_code.h"  // for pclint
#include "Tasks_id.h"
#include "chng_freq.h"
#include "freq_cfg.h"

#include "magicvoice_app.h"

#include "ivMagicVoice.h"
#include "iram_mgr.h"
#include "block_mem_def.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                         Definition                                   *
 **---------------------------------------------------------------------------*/
// char pHeal[20000];
LOCAL char *pHeap = PNULL;//add LOCAL for pclint

LOCAL ivTMVUserSys tUserSys;

LOCAL ivTMVResPackDesc pBg[2];
LOCAL void *pMVHANDLE;
//int16* tran_buf; for pclint no used

LOCAL volatile BLOCK_ID s_mv_thread_id = 0;
LOCAL uint32 first_can_read =1;
LOCAL uint32 count_can_read =0;
//PUBLIC uint32 s_dsp_read =0;  for pclint not used

LOCAL SFS_HANDLE      resourcefd = 0;
LOCAL const uint16 filename_resource[] = {'E',':','\\','i','v','M','V','R','e','s','.','i','r','f',0x00};

LOCAL SFS_HANDLE      logfd = 0;
LOCAL const uint16 filename_log[] = {'E',':','\\','l','o','g','.','t','x','t',0x00};

LOCAL uint8* res_Buffer=NULL;

LOCAL uint32 mv_resource = 1;

LOCAL uint32 mv_change_value= ivMV_CHANGE_NONE;
LOCAL uint32 mv_role_value = ivMV_ROLE_NONE;
LOCAL uint32 mv_effect_value = ivMV_VE_NONE;
LOCAL uint32 mv_eqid_value = ivMV_EQID_NONE;
LOCAL uint32 mv_bgid_value = ivMV_BGID_NONE;
LOCAL uint32 mv_bgvolume_value = 0;
LOCAL uint32 mv_mvolume_value = 0;//add LOCAL for pclint

#define TASK_MV_SCHED_PRI  PRI_MV_SCHED_TASK    //25

#define mv_bgvolume_min -3
#define mv_bgvolume_max 3
#define mv_mvolume_min -3
#define mv_mvolume_max 3

#define DEFAULT_UPLINK_GAIN   0x16a7//@cherry needs check
#define DEFAULT_DOWNLINK_GAIN 0x5a7//@cherry needs check

LOCAL uint16 data_buffer_raw[160]={0};
LOCAL uint16 data_buffer_changed[160]={0};

#ifdef MAGIC_VOICE_IN_IRAM
extern uint32 Image$$IRAM_MAGIC_VOICE$$Base;
extern uint32 Image$$IRAM_MAGIC_VOICE$$Length;
extern uint32 Load$$IRAM_MAGIC_VOICE$$Base;
ILOCK_HANDLE s_ma_ilock_handle = IRAM_INVALID_HANDLE;
const IRAM_AREA g_ma_iram_area[] = {&Image$$IRAM_MAGIC_VOICE$$Base, &Image$$IRAM_MAGIC_VOICE$$Length, (void*)&Load$$IRAM_MAGIC_VOICE$$Base, IRAM_ATTRIBUTE_NORMAL};
#endif

LOCAL void MV_DEMO_Open_Resource(void);
	
LOCAL void MV_DEMO_Init(void);

LOCAL void MV_DEMO_Start(void);

LOCAL ivInt32 ivCall ReadResCB(
	ivPointer		pParameter,		/* [in] user callback parameter */
	ivPointer		pBuffer,		/* [out] read resource buffer */
	ivResAddress	iPos,			/* [in] read start position */
	ivResSize		nSize )	;		/* [in] read size */

LOCAL ivInt32 ivCall WriteLogCB(
					  ivPointer		pParameter,		/* [in] user callback parameter */
					  ivPointer		pBuffer,		/* [out] write source buffer */
					  ivResSize		nSize );		/* [in] write size */

LOCAL void MV_DEMO_Stop(void);

LOCAL void  MV_DEMO_callback(void);    


PUBLIC void MV_DEMO_Open(BOOLEAN isSwitch)
{
    //SCI_TRACE_LOW:" MV_DEMO_Open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_115_112_2_18_0_34_26_1959,(uint8*)"");
    
#ifdef MAGIC_VOICE_IN_IRAM
     s_ma_ilock_handle = IRAM_LockArea((const IRAM_AREA *)&(g_ma_iram_area), sizeof(g_ma_iram_area)/sizeof(IRAM_AREA), IRAM_WAIT_FOR_EVER);
     SCI_ASSERT(s_ma_ilock_handle!=NULL);/*assert verified*/
#endif
		
    MV_DEMO_Start();

    if (!isSwitch)
    {
#ifndef PRODUCT_DM
           LAYER1_StartMagicVoiceFunc(MV_DEMO_callback,0x100,DEFAULT_UPLINK_GAIN,DEFAULT_DOWNLINK_GAIN);
#else
           LAYER1_StartMagicVoiceFunc(MV_DEMO_callback,0x01,DEFAULT_UPLINK_GAIN,DEFAULT_DOWNLINK_GAIN);
#endif
    }	
}   

PUBLIC void MV_DEMO_Close(BOOLEAN isSwitch)
{
    //SCI_TRACE_LOW:" MV_DEMO_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_136_112_2_18_0_34_27_1960,(uint8*)"");

    if (!isSwitch)
    {
           LAYER1_StopMagicVoiceFunc(); 
    }
	
    MV_DEMO_Stop();
    
#ifdef MAGIC_VOICE_IN_IRAM
     if(IRAM_INVALID_HANDLE != s_ma_ilock_handle)
    {
		IRAM_UnlockArea (s_ma_ilock_handle);
		s_ma_ilock_handle = IRAM_INVALID_HANDLE;
     } 
#endif
 }

PUBLIC void MV_DEMO_SetParam(uint32  paramid,int32 paramvalue)
{
     switch(paramid)
     {
         case ivMV_PARAM_CHANGE:
	  {
		   if	(paramvalue>=ivMV_CHANGE_NONE && paramvalue <=ivMV_CHANGE_YOUNG)
		   {
		         mv_change_value = paramvalue;
		   }
		   else
		   {
		        //SCI_ASSERT(0);
		        //SCI_TRACE_LOW:"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_CHANGE\r\n"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_167_112_2_18_0_34_27_1961,(uint8*)"" );
		   }
		   break; 	
         }

         case ivMV_PARAM_ROLE:
	  {
		   if	(paramvalue>=ivMV_ROLE_NONE && paramvalue <=ivMV_ROLE_RAPWOMAN)
		   {
		         mv_role_value = paramvalue;
		   }
		   else
		   {
		        //SCI_ASSERT(0);
		        //SCI_TRACE_LOW:"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_ROLE\r\n"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_181_112_2_18_0_34_27_1962,(uint8*)"" );
		   }
		   break; 	
         }

         case ivMV_PARAM_EFFECT:
	  {
		   if	(paramvalue>=ivMV_VE_NONE && paramvalue <=ivMV_VE_UWATER)
		   {
		         mv_effect_value = paramvalue;
		   }
		   else
		   {
		        //SCI_ASSERT(0);
		        //SCI_TRACE_LOW:"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_EFFECT\r\n"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_195_112_2_18_0_34_27_1963,(uint8*)"" );
		   }
		   break; 	
         }

         case ivMV_PARAM_EQID:
	  {
		   if	(paramvalue>=ivMV_EQID_NONE && paramvalue <=ivMV_EQID_SPEECH2)
		   {
		         mv_eqid_value = paramvalue;
		   }
		   else
		   {
		        //SCI_ASSERT(0);
		        //SCI_TRACE_LOW:"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_EQID\r\n"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_209_112_2_18_0_34_27_1964,(uint8*)"" );
		   }
		   break; 	
         }

         case ivMV_PARAM_BGID:
	  {
		   if	(paramvalue>=ivMV_BGID_NONE && paramvalue <=ivMV_BGID_CRICKET)
		   {
		         mv_bgid_value = paramvalue;
		   }
		   else
		   {
		        //SCI_ASSERT(0);
		        //SCI_TRACE_LOW:"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_BGID\r\n"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_223_112_2_18_0_34_27_1965,(uint8*)"" );
		   }
		   break; 	
         }

         case ivMV_PARAM_BGVolume:
	  {
		   if	(paramvalue>=mv_bgvolume_min && paramvalue <=mv_bgvolume_max)
		   {
		         mv_bgvolume_value = paramvalue;
		   }
		   else
		   {
		        //SCI_ASSERT(0);
		        //SCI_TRACE_LOW:"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_BGVolume\r\n"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_237_112_2_18_0_34_27_1966,(uint8*)"" );
		   }
		   break; 	
         }

         case ivMV_PARAM_MVolume:
	  {
		   if	(paramvalue>=mv_mvolume_min && paramvalue <=mv_mvolume_max)
		   {
		         mv_mvolume_value = paramvalue;
		   }
		   else
		   {
		        //SCI_ASSERT(0);
		        //SCI_TRACE_LOW:"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:ivMV_PARAM_MVolume\r\n"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_251_112_2_18_0_34_27_1967,(uint8*)"" );
		   }
		   break; 	
         }

	  default:
	          //SCI_ASSERT(0);
		        //SCI_TRACE_LOW:"[DRV_MAGICVOICE_APP]<WARNING> MV_DEMO_SetParam:paramid is invalid\r\n"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_258_112_2_18_0_34_27_1968,(uint8*)"" );
	          break;
	 } 
}

PUBLIC uint32 MV_DEMO_Get_ResourceStatus(void)
{
#ifdef MV_FILE_RES
         SFS_HANDLE      resourcestatusfd = 0;
   
	   resourcestatusfd  = SFS_CreateFile((uint16 *)filename_resource, SFS_MODE_READ |SFS_MODE_OPEN_EXISTING, NULL, NULL);
       
       if(resourcestatusfd==NULL)
       {
       	   mv_resource = 0;
       }
       else
       {
       	   mv_resource = 1;
       	   
       	   SFS_CloseFile(resourcestatusfd);
       }

       // 先try一下是否可以分配足够的空间，如果不可以就在界面上灰化背景音选项，使mv_bgid_value=0
       // 从而MV_DEMO_Open_Resource中就不用为背景音分配内存了，从而保证了不会出现内存不足的问题
       if (1 == mv_resource && ALLOC_BY_OTHERS == BL_GetStatus(BLOCK_MEM_POOL_MV_RES))
       {
           mv_resource = 0;
       }
	 
       return mv_resource;
#else
    return (uint32)(ALLOC_BY_OTHERS != BL_GetStatus(BLOCK_MEM_POOL_MV_RES));
#endif
}

LOCAL void MV_DEMO_Open_Resource(void)
{    
#ifdef MV_FILE_RES
    uint32 bytes_read ;
#endif
    uint32 resource_count = 0;
    
    if ((ivMV_BGID_NONE != mv_bgid_value) && (1 == MV_DEMO_Get_ResourceStatus()))//@fen.xie for MS00186567:增加resource status的判断
    {
    #ifdef MV_FILE_RES
        {
            resourcefd  = SFS_CreateFile((uint16 *)filename_resource, SFS_MODE_READ |SFS_MODE_OPEN_EXISTING, NULL, NULL);
            
            if(resourcefd==NULL)
            {
                resource_count = 0;
                
                mv_resource = 0;
                //SCI_TRACE_LOW:"MV_DEMO_Open_Resource: open res fail or no card"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_312_112_2_18_0_34_27_1969,(uint8*)"");
                
            }
            else
            {
                res_Buffer = (uint8 *)BL_Malloc(BLOCK_MEM_POOL_MV_RES);
                
                if(SFS_ERROR_NONE != SFS_ReadFile(resourcefd, (uint8 *)res_Buffer, 150*1024, &bytes_read, NULL))
                {
                    SCI_Assert("read fail", __FILE__, __LINE__);/*assert verified*/
                }
                //SCI_TRACE_LOW:"read bytes_read = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_323_112_2_18_0_34_27_1970,(uint8*)"d",bytes_read);
                
                pBg[0].pCBParam = (void *)resourcefd;
                pBg[0].pfnRead = ReadResCB;
                
                resource_count = 1 ;
                mv_resource = 1;	  
            }		  
        }
    #else
        {
            extern PUBLIC uint8* MMIAPISET_GetMagicVoiceRes(void);
            
            res_Buffer = (uint8 *)BL_Malloc(BLOCK_MEM_POOL_MV_RES);
            SCI_MEMCPY(res_Buffer, MMIAPISET_GetMagicVoiceRes(), 150*1024);
            
            pBg[0].pCBParam = (ivPointer)1;
            pBg[0].pfnRead = ReadResCB;
            resource_count = 1 ;
        }
    #endif

    }
    
    logfd  = SFS_CreateFile((uint16 *)filename_log, SFS_MODE_WRITE |SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655 */
    
    if(logfd==NULL)
    {
        //SCI_TRACE_LOW:"MV_DEMO_Open_Resource: open log fail or no card"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_351_112_2_18_0_34_27_1971,(uint8*)"");
    }

    pHeap = SCI_ALLOC_APP(ivMV_SUGGEST_HEAPSIZE);
    SCI_MEMSET(pHeap,0,ivMV_SUGGEST_HEAPSIZE);
	   
    tUserSys.lpHeap = (ivPointer)pHeap;
    tUserSys.nHeapSize = ivMV_SUGGEST_HEAPSIZE;
    tUserSys.pLogCBParam = 0;//(ivPointer)logfd;
    tUserSys.pLogWrite = 0;//WriteLogCB;
    tUserSys.pMVResPackDesc = pBg;
    tUserSys.nMVResPackDesc = resource_count;
    
}

LOCAL void MV_DEMO_Close_Resource(void)
{    
    if (res_Buffer!=NULL)
    {
         BL_Free(BLOCK_MEM_POOL_MV_RES);
         res_Buffer = NULL;
    }
    
    #ifdef MV_FILE_RES
    if (resourcefd !=NULL)
    {		 
          SFS_CloseFile(resourcefd);		            		  
          resourcefd = NULL;
    }
    #endif

    if (logfd !=NULL)
    {
          SFS_CloseFile(logfd);         			  
          logfd = NULL;
    }

    if (pHeap)
    {
    	SCI_FREE(pHeap);
    }
}


LOCAL void MV_DEMO_Init(void)
{
       pMVHANDLE = 0;
   
   	if (ivMV_OK != ivMV_Init(&pMVHANDLE,&tUserSys))
	{
		SCI_ASSERT("DEMO_Init fail");  /*assert verified*/
	}
	
	ivMV_SetParam(pMVHANDLE,ivMV_PARAM_CHANGE, mv_change_value);
	ivMV_SetParam(pMVHANDLE,ivMV_PARAM_ROLE, mv_role_value);
	ivMV_SetParam(pMVHANDLE,ivMV_PARAM_EFFECT, mv_effect_value);
	ivMV_SetParam(pMVHANDLE,ivMV_PARAM_EQID, mv_eqid_value);
	ivMV_SetParam(pMVHANDLE,ivMV_PARAM_BGID, mv_bgid_value);
	ivMV_SetParam(pMVHANDLE,ivMV_PARAM_BGVolume,mv_bgvolume_value);
	ivMV_SetParam(pMVHANDLE,ivMV_PARAM_MVolume, mv_mvolume_value);	
}

LOCAL void MV_Os_Sched_Task(uint32 argc, void *argv)
{
     ivStatus  iVcstatus;	 
      
     for(;;)
     {  

	    iVcstatus =  ivMV_RunStep(pMVHANDLE); 

	   if(ivMV_OK != iVcstatus) 
	   {
	       //SCI_TRACE_LOW("*****ivMV_RunStep RETRUN  = %d*******",iVcstatus);
	       SCI_Sleep(10);
	   }     
     }
}



LOCAL void MV_DEMO_Start(void)
{
     MV_DEMO_Open_Resource();
     
     MV_DEMO_Init();
     
    s_mv_thread_id = SCI_CreateThread("T_MV_SCHED", "Q_MV_SCHED", MV_Os_Sched_Task, 0, NULL, 1024*4, 
									10, TASK_MV_SCHED_PRI, SCI_PREEMPT, /*SCI_AUTO_START*/ SCI_DONT_START);
    SCI_ASSERT(SCI_INVALID_BLOCK_ID != s_mv_thread_id);/*assert verified*/
    
    //SCI_TRACE_LOW:"s_mv_thread_id = 0x%X\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAGICVOICE_APP_442_112_2_18_0_34_27_1972,(uint8*)"d", s_mv_thread_id);

    SCI_ResumeThread(s_mv_thread_id);


}

LOCAL ivInt32 ivCall ReadResCB(
	ivPointer		pParameter,		/* [in] user callback parameter */
	ivPointer		pBuffer,		/* [out] read resource buffer */
	ivResAddress	iPos,			/* [in] read start position */
	ivResSize		nSize )			/* [in] read size */
{	
	SCI_MEMCPY(pBuffer,res_Buffer+iPos,nSize);

	return 0;
}

LOCAL ivInt32 ivCall WriteLogCB(
					  ivPointer		pParameter,		/* [in] user callback parameter */
					  ivPointer		pBuffer,		/* [out] write source buffer */
					  ivResSize		nSize )			/* [in] write size */
{
    uint32 bytes_written;
	SFS_HANDLE pFile = (SFS_HANDLE)pParameter;
	
	SFS_WriteFile(pFile, (const void *) pBuffer,nSize, &bytes_written, NULL);

	return 0;
}

LOCAL void MV_DEMO_Stop(void)
{   		 
     if (s_mv_thread_id!=0)
     {
     	    SCI_DeleteThread(s_mv_thread_id);			
     }

     MV_DEMO_Close_Resource();
}

LOCAL void  MV_DEMO_callback(void)
{
	ivStatus iVcstatus;
	ivSize size =160;
	
#ifndef PRODUCT_DM
	LAYER1_ReadVoiceData(data_buffer_raw ,160);	
#else
	LAYER1_GetOrgMagicVoiceData(data_buffer_raw ,160);	
#endif
	iVcstatus = ivMV_AppendData(pMVHANDLE, (ivPCInt16)data_buffer_raw, (ivUInt32)160);
	//SCI_TraceLow("***********Magic_voice_callback ivMV_AppendData iVcstatus=%d",iVcstatus);				
	iVcstatus = ivMV_GetData(pMVHANDLE,(ivPInt16)data_buffer_changed,(ivPUInt32)&size);
	//SCI_TraceLow("***********Magic_voice_callback ivMV_GetData iVcstatus=%d",iVcstatus);					
       if(iVcstatus == 0)
       {
#ifndef PRODUCT_DM
	  	LAYER1_WriteVoiceData(data_buffer_changed,160);
#else
	  	LAYER1_SetProMagicVoiceData(data_buffer_changed,160);
#endif
       }

}	

