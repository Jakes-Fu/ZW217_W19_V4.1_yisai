/****************************************************************************
** File Name:       audio_express_demo.c                                     *
** Author:                                                                   *
** Date:           11/10/2010                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    the file of use audio express     
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
#include "apm_express.h"
#include "agc_exp.h"
#include "adm.h"
#include "aud_gen.h"
#include "audio_api.h"
#include "audio_config.h"
#include "dal_audio.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#define DEBUG_EXPRESS_DEMO
    
#ifdef DEBUG_EXPRESS_DEMO
    #ifndef EXPRESS_DEMO_PRINT


    #define EXPRESS_DEMO_PRINT( _format_string )   SCI_TRACE_LOW _format_string
    #endif
#else
    #ifndef EXPRESS_DEMO_PRINT


    #define EXPRESS_DEMO_PRINT( _format_string ) 
    #endif
#endif
    
#ifndef EXPRESS_DEMO_ASSERT
#define EXPRESS_DEMO_ASSERT SCI_PASSERT 
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define AMRSTRING_TRACKBUFSIZE_APP       (80 * 150)

/*******************************************************************************
 *  static variable
 *******************************************************************************/
LOCAL uint8   express_process_type=1;
LOCAL HAUDIO  mp3_handle = 0;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

extern uint32 VBDecodeOuputBuf[];

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn LOCAL void  EXPRESS_DEMO_Process(
//!    AUDIO_OBJECT_T *audio_obj_ptr,
//!    int16* psSrcLeftData,
//!    int16* psSrcRightData,
//!    uint32 uiSrcCount,
//!    int16* psDestLeftData,
//!    int16* psDestRightData,
//!    uint32* puiDestCount
//!    )
//! \param audio_obj_ptr audio object.
//! \param psSrcLeftData input left channel data.
//! \param psSrcRightData input right channel data.
//! \param psSrcRightData length of input data.
//! \param psDestLeftData output left channel data.
//! \param psDestRightData output right channel data.
//! \param puiDestCount length of output data.
//! \return void no return
//! \brief  Description:  This function is to Karaoke_Process the source data. 
//! \author Author:  hai.li
//! \note   Note:Nothing.
/*****************************************************************************/
LOCAL AUDIO_RESULT_E  EXPRESS_DEMO_Process(
    AUDIO_OBJECT_T *audio_obj_ptr,
    int16* psSrcLeftData,
    int16* psSrcRightData,
    uint32 uiSrcCount,
    int16* psDestLeftData,
    int16* psDestRightData,
    uint32* puiDestCount
    )
{
    /*EXPRESS_DEMO_ASSERT(PNULL!=audio_obj_ptr, ("%s, %d", __MODULE__,__LINE__));
    EXPRESS_DEMO_ASSERT(PNULL!=psSrcLeftData, ("%s, %d", __MODULE__,__LINE__));
    EXPRESS_DEMO_ASSERT(PNULL!=psSrcRightData, ("%s, %d", __MODULE__,__LINE__));
    EXPRESS_DEMO_ASSERT(PNULL!=psDestLeftData, ("%s, %d", __MODULE__,__LINE__));
    EXPRESS_DEMO_ASSERT(PNULL!=psDestRightData, ("%s, %d", __MODULE__,__LINE__));
    EXPRESS_DEMO_ASSERT(PNULL!=puiDestCount, ("%s, %d", __MODULE__,__LINE__));
    
    EXPRESS_DEMO_PRINT(("[audio_express_demo]: EXPRESS_DEMO_Process uiSrcCount=%d,express_process_type=%d",uiSrcCount,express_process_type));*/

    if(express_process_type==1) //only play left channel   
    {
        SCI_MEMCPY(psDestLeftData,  psSrcLeftData, uiSrcCount*2);
        SCI_MEMCPY(psDestRightData, psSrcLeftData, uiSrcCount*2);		
        *puiDestCount = uiSrcCount;
    }
    else if (express_process_type==2) //only play right channel 
    {
        SCI_MEMCPY(psDestLeftData,  psSrcRightData, uiSrcCount*2);
        SCI_MEMCPY(psDestRightData, psSrcRightData, uiSrcCount*2);	
        *puiDestCount = uiSrcCount;
    }
    else //cannot do any process,only copy data. 
    {
        SCI_MEMCPY(psDestLeftData,  psSrcLeftData, uiSrcCount*2);
        SCI_MEMCPY(psDestRightData, psSrcRightData, uiSrcCount*2);	
        *puiDestCount = uiSrcCount;
    }

    //EXPRESS_DEMO_PRINT:"[audio_express_demo]: EXPRESS_DEMO_Process *puiDestCount=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_112_112_2_18_2_18_20_274,(uint8*)"d",*puiDestCount);
    return AUDIO_NO_ERROR;
}
/*****************************************************************************/
//  Description:    express demo set param
//  Author:         
//  Note:
//****************************************************************************/  
LOCAL AUDIO_RESULT_E  EXPRESS_DEMO_SetPara(
    uint32 argc,
    void *argv
    )
{
    uint8 *para;
    para = (uint8 *)argv;
   
    if(((*para)>0) && ((*para)<4))
    {
      express_process_type=*para;
      
      //EXPRESS_DEMO_PRINT:"[audio_express_demo]: EXPRESS_DEMO_SetPara express_process_type=%d "
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_131_112_2_18_2_18_20_275,(uint8*)"d",express_process_type);

      return AUDIO_NO_ERROR;
    }
    else
    {
       //EXPRESS_DEMO_PRINT:"[audio_express_demo]: EXPRESS_DEMO_SetPara is err! para=%d "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_137_112_2_18_2_18_20_276,(uint8*)"d",*para);
    }
    return AUDIO_ERROR;
}

/*****************************************************************************/
//  Description:    express demo deinit param
//  Author:        
//  Note:
//****************************************************************************/  
LOCAL AUDIO_RESULT_E  EXPRESS_DEMO_DeinitPara(
    AUDIO_OBJECT_T *audio_obj_ptr
    )
{
    EXPRESS_DEMO_ASSERT(audio_obj_ptr != SCI_NULL,("[audio_express_demo]:EXPRESS_DEMO_DeinitPara audio_obj_ptr is PNULL!")); /*assert to do*/

    return  AUDIO_NO_ERROR;

}
/*****************************************************************************/
//! \fn PUBLIC void  EXPRESS_DEMO_InitPara(
//!    AUDIO_OBJECT_T *audio_obj_ptr
//!    )
//! \param audio_obj_ptr audio object info.
//! \return void no return
//! \brief  Description:  This function is to init para.
//! \author Author:  Hai.Li
//! \note   Note:Nothing.
/*****************************************************************************/
LOCAL AUDIO_RESULT_E  EXPRESS_DEMO_InitPara(
    AUDIO_OBJECT_T *audio_obj_ptr
    )
{
  EXPRESS_DEMO_ASSERT(audio_obj_ptr != SCI_NULL,("[audio_express_demo]:EXPRESS_DEMO_InitPara audio_obj_ptr is PNULL!")); /*assert to do*/

  return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//! \fn PUBLIC HAUDIOEXP CatMv_RegExpPlugger(
//!    void
//!    )
//! \param void no para
//! \return return express handle
//! \brief  Description:  This function is to register an express plugger.
//! \author Author:  Hai.Li
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC HAUDIOEXP EXPRESS_DEMO_RegExpPlugger(void)
{
        AUDIO_EXPRESS_T ptAudioExp={0};
        ptAudioExp.pusExpName = ANSI2UINT16("EXPRESS_DEMO");
        ptAudioExp.pusExpDescribe = SCI_NULL;
        ptAudioExp.process =  EXPRESS_DEMO_Process;
        ptAudioExp.set_para =  EXPRESS_DEMO_SetPara;
        ptAudioExp.init_para = EXPRESS_DEMO_InitPara;
        ptAudioExp.deinit_para = EXPRESS_DEMO_DeinitPara;
        
        return AUDIO_PM_RegExpressPlugger(&ptAudioExp);
}



#define DEMO_APP_CODE

#define IDLE      0
#define INIT      1
#define START     2
#define DEMO_PROC_EXP_PARA_COUNT 1	

typedef struct
{
    uint16            demo_state;
    
}DEC_CTX_T;

//extern HAUDIOCODEC hMP3Codec;
static DEC_CTX_T  s_dec_ctx = {0};
HAUDIOEXP hDemoExp;

/*****************************************************************************/
//  Description:    Reg express  funcation
//  Author:        
//  Note:
//****************************************************************************/  
PUBLIC void AUDIO_Express_Demo_RegExp(void)
{
    //EXPRESS_DEMO_PRINT:"[audio_express_demo]: AUDIO_Express_Demo_RegExp"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_218_112_2_18_2_18_20_277,(uint8*)"");

    if(s_dec_ctx.demo_state == IDLE)
    {
       hDemoExp = EXPRESS_DEMO_RegExpPlugger();
       s_dec_ctx.demo_state = INIT;
    }
}


/*****************************************************************************/
//  Description:   UnReg express funcation
//  Author:        
//  Note:
//****************************************************************************/  
PUBLIC void AUDIO_Express_Demo_UnRegExp(void)
{
   AUDIO_RESULT_E audio_result;

   //EXPRESS_DEMO_PRINT:"[audio_express_demo]: AUDIO_Express_Demo_UnRegExp"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_236_112_2_18_2_18_20_278,(uint8*)"");

   if(s_dec_ctx.demo_state == INIT)
   {
       audio_result=AUDIO_PM_UnRegExpressPlugger (hDemoExp);

       if(audio_result!=AUDIO_NO_ERROR)
       {
            //EXPRESS_DEMO_ASSERT(0, ("%s, %d,audio_result=%d", __MODULE__,__LINE__,audio_result));
       }
       
       s_dec_ctx.demo_state = IDLE;
   }
   else
   {
         //EXPRESS_DEMO_PRINT:"[audio_express_demo]: AUDIO_Express_Demo_UnRegExp Err,demo_state had INIT!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_251_112_2_18_2_18_20_279,(uint8*)"");
   }
}

/*****************************************************************************/
//  Description : API of  start demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AUDIO_Express_Demo_Start(void)
{
    AUDIO_RESULT_E audio_result;

    //EXPRESS_DEMO_PRINT:"[audio_express_demo]: AUDIO_Express_Demo_Start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_262_112_2_18_2_18_20_280,(uint8*)"");

    AUDIO_Express_Demo_RegExp();
    
    if(s_dec_ctx.demo_state == INIT)
    {
       audio_result=AUDIO_PM_AddCodecExpress (hMP3Codec, hDemoExp);

       if(audio_result!=AUDIO_NO_ERROR)
       {
           // EXPRESS_DEMO_ASSERT(0, ("%s, %d,audio_result=%d", __MODULE__,__LINE__,audio_result));
       }
       
       s_dec_ctx.demo_state = START;
    }
    else
    {
         //EXPRESS_DEMO_PRINT:"[audio_express_demo]: AUDIO_Express_Demo_Start Err,demo_state had start!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_279_112_2_18_2_18_20_281,(uint8*)"");
    }

}

/*****************************************************************************/
//  Description : API of stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AUDIO_Express_Demo_Stop(void)
{
   AUDIO_RESULT_E audio_result;

   //EXPRESS_DEMO_PRINT:"[audio_express_demo]: AUDIO_Express_Demo_Stop"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_291_112_2_18_2_18_20_282,(uint8*)"");

   if(s_dec_ctx.demo_state == START)
   {
       audio_result=AUDIO_PM_DeleteCodecExpress (hMP3Codec, hDemoExp);

       if(audio_result!=AUDIO_NO_ERROR)
       {
            //EXPRESS_DEMO_ASSERT(0, ("%s, %d,audio_result=%d", __MODULE__,__LINE__,audio_result));
       }

       s_dec_ctx.demo_state = INIT;
   }
   else
   {
         //EXPRESS_DEMO_PRINT:"[audio_express_demo]: AUDIO_Express_Demo_Stop Err,demo_state had stop!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_306_112_2_18_2_18_20_283,(uint8*)"");
   }
}

/*****************************************************************************/
//  Description:   set express par
//  Author:        
//  Note:
//****************************************************************************/  
PUBLIC void AUDIO_Express_Demo_SetPar(uint8 aud_para)
{
   AUDIO_RESULT_E audio_result;

   //EXPRESS_DEMO_PRINT:"[audio_express_demo]: AUDIO_Express_Demo_SetPar aud_para=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_318_112_2_18_2_18_20_284,(uint8*)"d",aud_para);

   if(s_dec_ctx.demo_state != IDLE)
   {
       audio_result=AUDIO_ExeExpSetParaOpe (
                     hDemoExp,
                     PNULL,
                     DEMO_PROC_EXP_PARA_COUNT,
                     &aud_para
                 );
                 
        if(audio_result!=AUDIO_NO_ERROR)
        {
#ifndef WIN32
            EXPRESS_DEMO_ASSERT(0, ("%s, %d,audio_result=%d", __MODULE__,__LINE__,audio_result));/*assert verified*/
#endif 
        }
       
    }
    else
    {
         //EXPRESS_DEMO_PRINT:"[audio_express_demo]: AUDIO_Express_Demo_SetPar Err!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_339_112_2_18_2_18_21_285,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description:   play mp3 on sd card
//  Author:        
//  Note:
//****************************************************************************/  
extern PUBLIC uint16 GUI_GB2UCS(					//return the number of UCS2 code that have been converted.
                         uint16 *ucs2_ptr, 	//destination: string pointer to UCS2 string.
                         const uint8 *gb_ptr,//source: string pointer to GB string
                         uint16 len			//the length to convert from GB to UCS2.
                         );

PUBLIC BOOLEAN MP3_Express_Demo_Start(void)
{

    uint8  iis_file_name[32] = {0};
    uint16 ucs2_iis_file_name[32] = {0}; 
    uint16 result_temp=0; 
        
    sprintf((char *)iis_file_name,"E:\\LRTrack1.mp3");

    //SCI_TRACE_LOW:"YM %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_361_112_2_18_2_18_21_286,(uint8*)"s",iis_file_name);

    result_temp=GUI_GB2UCS((uint16*)ucs2_iis_file_name, (uint8*)iis_file_name, (uint16)(sizeof(iis_file_name)));

    mp3_handle = AUDIO_CreateFileHandle(hMP3DSPCodec,
                                            PNULL,
                                            hARMVB,
                                            PNULL,
                                            (const int16*)ucs2_iis_file_name,
                                            VBDecodeOuputBuf,
                                            AMRSTRING_TRACKBUFSIZE_APP,
                                            NULL
                                            );
    if(mp3_handle != 0)
    {
        AUDIO_Play(mp3_handle,0);
        AUDIO_SetVolume(5);
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"file is not exit!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_EXPRESS_DEMO_382_112_2_18_2_18_21_287,(uint8*)"");
        return FALSE; //error
    }
}
/*****************************************************************************/
//  Description:   stop mp3 on sd card
//  Author:        
//  Note:
//****************************************************************************/  

PUBLIC void MP3_Express_Demo_Stop(void)
{
    AUDIO_Stop(mp3_handle);
}

