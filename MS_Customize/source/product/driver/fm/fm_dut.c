#include "os_api.h"
#include "diag.h"
#include "cmddef.h"

#include "fm_dut.h"

#include "fm_drv.h"

#include "gpio_prod_api.h"

#include "audio_api.h"

#include "audio_config.h"

static HAUDIO s_fm_handle;

static BOOLEAN               headset_is_in = FALSE;

static FM_ALL_CHANNEL_INFO_T channel_info = {0};

extern uint16 FM_SPRD_RecRSSI(void);
extern uint8 FM_SPRD_RecStereo(void);

typedef struct 
{
  uint8   nErrorCode;   //DIAG_FM_REG_ERR_E
  uint32  nStartAddr;   
  uint32  nUintCount;
}FM_RW_REG_T;

typedef enum
{
  FM_REG_SUCCESS = 0,
  FM_REG_ADDR_ERR,
  FM_REG_SIZE_ERR,
  FM_REG_RANGE_ERR,
  FM_REG_OTHER_ERR
}DIAG_FM_REG_ERR_E;

typedef struct _FM_SIGNAL_PARAM_T
{
  uint8   nOperInd;        //0: Executed successfully,1: Executed unsuccessfully
  uint8   nStereoInd;      //0: Mono; Other: Stereo
  uint16  nRssi;           //Rssi value
  uint32  nFreqValue;      //Frequency, Unit:100KHZ
  uint32  nPwrIndicator;   //0x00c0---register address
  uint32  nFreqOffset;     //0x00bc---register address
  uint32  nPilotDet;       //0x00b0---register address
  uint32  nNoDacLpf;       //mo-adc-lpf register address
}FM_SIGNAL_PARAM_T;

#define REG_INPWR_STS         (FM_REG_BASE + 0x00C0)
#define REG_FREQ_OFF_STS      (FM_REG_BASE + 0x00BC)
#define REG_RSSI_STS          (FM_REG_BASE + 0x00B0)
#define REG_NO_LPF_STS        (FM_REG_BASE + 0x00C8)

/*FM commands*/
#define DIAG_FM_TEST_F   0x41
#define FM_CMD_STATE     0x00
#define FM_CMD_VALUME     0x01
#define FM_CMD_MUTE      0x02
#define FM_CMD_TUNE      0x03
#define FM_CMD_SEEK      0x04
#define FM_CMD_READ_REG  0x05
#define FM_CMD_WRITE_REG 0x06

#define MAKEWORD(a, b)      ((uint16)(((uint8)(a)) | ((uint16)((uint8)(b))) << 8))
#define MAKELONG(a, b)      ((uint32)(((uint16)(a)) | ((uint32)((uint16)(b))) << 16))

static uint32  _fm_dut_handler(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len);

LOCAL void Control_FM_State(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len);
LOCAL void Control_FM_Volume(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len);
LOCAL void Set_FM_Mute(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len);
LOCAL void Get_FM_Tune_Info(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len);
LOCAL void FM_Seek(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len);
LOCAL void FM_Read_Reg(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len);
LOCAL void FM_Write_Reg(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len);

PUBLIC void FM_DUT_Init(void)
{
    DIAG_RegisterCmdRoutine(DIAG_FM_TEST_F,_fm_dut_handler);
}

uint32  _fm_dut_handler(uint8 **dest,uint16 *dest_len,const uint8 *src_ptr,uint16 src_len)
{
    MSG_HEAD_T *msg_head    = (MSG_HEAD_T *)src_ptr;

	SCI_TRACE_LOW("[_dut_handler] subtype = %d", msg_head->subtype);

	switch(msg_head->subtype)
	{
	   case FM_CMD_STATE:
		    Control_FM_State(dest,dest_len,src_ptr,src_len);
		  // ((MSG_HEAD_T*)(*dest))->subtype = 0;
	   	   break;
	   case FM_CMD_VALUME:
	   	   Control_FM_Volume(dest,dest_len,src_ptr,src_len);
		  // ((MSG_HEAD_T*)(*dest))->subtype = 0;
	   	   break;
	   case FM_CMD_MUTE:
	   	   Set_FM_Mute(dest,dest_len,src_ptr,src_len);
		  // ((MSG_HEAD_T*)(*dest))->subtype = 0;
	   	   break;	
	   case FM_CMD_TUNE:
	   	   Get_FM_Tune_Info(dest,dest_len,src_ptr,src_len);
		 // ((MSG_HEAD_T*)(*dest))->subtype = 0;
	   	   break;
	   case FM_CMD_SEEK:
	   	   FM_Seek(dest,dest_len,src_ptr,src_len);
		//   ((MSG_HEAD_T*)(*dest))->subtype = 0;
	   	   break;
	   case FM_CMD_READ_REG:
	   	   FM_Read_Reg(dest,dest_len,src_ptr,src_len);
		 //  ((MSG_HEAD_T*)(*dest))->subtype = 0;
	   	   break;
	   case FM_CMD_WRITE_REG:
	   	   FM_Write_Reg(dest,dest_len,src_ptr,src_len);
		//   ((MSG_HEAD_T*)(*dest))->subtype = 0;
	   	   break;
	   default:
	   	   break;
	}
	return 1;
}

/*****************************************************************************/
//  Description : Audio dummy notify callback function.
//  Global resource dependence : none
//  Author: luther.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}

/*****************************************************************************/
//  Description:    This function init fm state 
//                  
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN Init_FM_State()
{
      BOOLEAN ret = FALSE;

	  HAUDIOCODEC hAudioCodec = hNullCodec;
	  const uint16* pusCodecName = PNULL;
	  HAUDIODEV     hDevice = hFMDev;
	  const uint16* pusDevName = PNULL;
	  BOOLEAN       result     = FALSE;
	
	  s_fm_handle = AUDIO_CreateNormalHandle
	  	(hAudioCodec,  pusCodecName, hDevice,  pusDevName, AudioNotifyCallback);

     // AUDIO_SetDevMode(AUDIO_DEVICE_MODE_EARFREE);
	  
      AUDIO_SetVolume(2);
   
      if(FM_SUCCESS == FM_ManualSeek(990,FM_SCAN_UP, 0, NULL))
      {

           result = AUDIO_Play(s_fm_handle,0);	   
           ret = TRUE;
      }
	return ret;
}


/*****************************************************************************/
//  Description:    This function control fm volume 
//                  
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/ 
LOCAL void Control_FM_Volume(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
   MSG_HEAD_T *head_ptr;
   uint8      volumeValue   = 0;
   uint8      setVolume_result     = 1;
#ifndef WIN32
  
   volumeValue = *(src_ptr + sizeof(MSG_HEAD_T));

   setVolume_result = (uint8)AUDIO_SetVolume((int32)volumeValue);

   *dest_ptr = (MSG_HEAD_T*)SCI_ALLOC_APP(sizeof(uint8) + sizeof(MSG_HEAD_T));
   SCI_ASSERT(SCI_NULL!=*dest_ptr);/*assert verified*/

   SCI_MEMCPY((void*)*dest_ptr,(void*)src_ptr,sizeof(MSG_HEAD_T));
   SCI_MEMCPY((void*)(*dest_ptr+1),(void*)&setVolume_result,sizeof(uint8));

   head_ptr = (MSG_HEAD_T*)*dest_ptr;
   head_ptr->len = (uint16) (sizeof(uint8) + sizeof(MSG_HEAD_T));
  // *dest_ptr = (uint8*)(*dest_ptr);
   *dest_len_ptr = head_ptr->len;

 #endif  
}

/*****************************************************************************/
//  Description:    This function set fm to mute 
//                  
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Set_FM_Mute(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
   MSG_HEAD_T *head_ptr;
   uint8      setFMMute_result     = 0;

#ifndef WIN32
  
   setFMMute_result = (uint8)AUDIO_SetVolume((int32)0);
     
   *dest_ptr  = (MSG_HEAD_T*)SCI_ALLOC_APP(sizeof(uint8) + sizeof(MSG_HEAD_T));
   SCI_ASSERT(SCI_NULL!=*dest_ptr);/*assert verified*/

   SCI_MEMCPY((void*)*dest_ptr ,(void*)src_ptr,sizeof(MSG_HEAD_T));

   SCI_MEMCPY((void*)(*dest_ptr +1),(void*)&setFMMute_result,sizeof(uint8));

   head_ptr = (MSG_HEAD_T*)*dest_ptr;
   
   head_ptr->len = (uint16) (sizeof(uint8) + sizeof(MSG_HEAD_T));
   
   //*dest_ptr = (uint8*)(*dest_ptr);
   *dest_len_ptr = head_ptr->len;

 #endif   
}


/*****************************************************************************/
//  Description:    This function search fm frequency 
//                  
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL uint16 SearchValidFreq(uint32 freq,uint8 direct)
{
    uint16             s_start_freq = freq;
    FM_SCAN_DIR_E      dir          = (FM_SCAN_DIR_E)direct;
    BOOLEAN            result       = FALSE;
    uint32             ret          = FM_SUCCESS;
    
    //volume = MMISRVAUD_GetVolume(MMIFM_GetPlayHandle());
    if(dir == FM_SCAN_UP)
    {
       //search next
        if(s_start_freq >= 1080)
        {
            //current freqency is highest. freqency. must start from lowest
            s_start_freq = 875;
        }
    }
    else if(dir == FM_SCAN_DOWN)
    {
        //search previous 
        if(s_start_freq <= 875)
        {
             //current freqency is lowest, search from highest
            s_start_freq = 1080;
        }
    }
    s_start_freq = MIN(1080, s_start_freq);
    s_start_freq = MAX(875, s_start_freq);
    ret = FM_AutoSeek((uint16)s_start_freq, dir, FM_SCAN_ONE, &channel_info);

	if(FM_SUCCESS == ret)// FM_SCAN_ON
     {
          result = 2;	
     }
     else if(FM_NOT_COMPLETE == ret )
     { 
		  result = 3;
     }
     else
     {
          result = 0;
     }
     return result;
}

LOCAL uint32 translateNum(uint8* s)
{
   int m = 0;
   uint32 temp=0;
   uint16 a = 0;
   uint16 b = 0;
   m = 4;
   
   a = MAKEWORD(s[0],s[1]);
   b = MAKEWORD(s[2],s[3]);
   temp = (uint32)MAKELONG(a, b);

   return temp;
}

/*****************************************************************************/
//  Description:    This function get fm tune info 
//                  
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Get_FM_Tune_Info(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len)
{
   MSG_HEAD_T          *head_ptr;

   uint16               resultRssi       = 0;
   uint8               resultnStereoInd = 0;

   uint8               result           = 0;
   FM_SIGNAL_PARAM_T   signalValue;

   uint8               *strValue;
   uint32              freqValue   = 0;
   uint32              rssiStatusValue = 0;
   uint32              noLpfStatusValue = 0;
   #ifndef WIN32
   
   *dest_ptr = (MSG_HEAD_T*)SCI_ALLOC_APP(sizeof(FM_SIGNAL_PARAM_T) + sizeof(MSG_HEAD_T));

   SCI_ASSERT(SCI_NULL!=*dest_ptr);/*assert verified*/
   
   strValue = (uint8*)SCI_ALLOC_APP(4);
   SCI_ASSERT(NULL!=strValue);/*assert verified*/
  
   SCI_MEMCPY((void*)strValue,(void*)(src_ptr+sizeof(MSG_HEAD_T)),sizeof(uint32));
 
   freqValue = translateNum(strValue);

   freqValue /= 100;
   
   SCI_FREE(strValue);
 
   FM_ManualSeek((uint16)freqValue, FM_SCAN_UP, 0, NULL);

   resultRssi       = FM_SPRD_RecRSSI();
   resultnStereoInd = FM_SPRD_RecStereo();
   
   signalValue.nFreqValue = freqValue * 100;

   signalValue.nRssi      = resultRssi;
   signalValue.nStereoInd = resultnStereoInd;
   
   result = resultRssi && resultnStereoInd;
   signalValue.nOperInd      = result;//0---SUCCESSED,1----FAILED
  
   signalValue.nPwrIndicator = TB_REG_GET(REG_INPWR_STS);
   signalValue.nFreqOffset   = TB_REG_GET(REG_FREQ_OFF_STS);
 
   rssiStatusValue = TB_REG_GET(REG_RSSI_STS);
   noLpfStatusValue = TB_REG_GET(REG_NO_LPF_STS);

   signalValue.nPilotDet     = (TB_REG_GET(REG_INPWR_STS)&(BIT_19|BIT_18));
   signalValue.nNoDacLpf     = (TB_REG_GET(REG_NO_LPF_STS)&(BIT_1|BIT_0));
   
   SCI_MEMCPY((void*)(uint8*)(*dest_ptr),(void*)src_ptr,sizeof(MSG_HEAD_T));

   head_ptr = (MSG_HEAD_T*)*dest_ptr;
   head_ptr->len = (uint16)(sizeof(FM_SIGNAL_PARAM_T)+sizeof(MSG_HEAD_T));
   
   *dest_ptr = (uint8*)(head_ptr);
   
   SCI_MEMCPY((void*)(*dest_ptr+8),(void*)((uint8*)&signalValue),sizeof(FM_SIGNAL_PARAM_T));

   *dest_ptr = (uint8*)(head_ptr);
   *dest_len_ptr =  head_ptr->len;

   AUDIO_Play(s_fm_handle,0);

    //MMIAPIFM_Play(MMIFM_GetPlayHandle(),freqValue);
     
   #endif
}


/*****************************************************************************/
//  Description:    This function control fm device 
//                  
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Control_FM_State(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
   MSG_HEAD_T *head_ptr;
   uint8      ctl_fm_state   = 5;

   uint8      ctl_result     = 0;

#ifndef WIN32
  
   ctl_fm_state = *(src_ptr + sizeof(MSG_HEAD_T));

   headset_is_in = GPIO_CheckHeadsetStatus();
   if (!FM_IsInit() && headset_is_in)
   {
       FM_Init();
   }

   if ( 0 == ctl_fm_state)//close device
   {
       FM_Init();
	   
      AUDIO_CloseHandle(s_fm_handle);
      ctl_result = FM_Close();
  
   }else if ( 1 == ctl_fm_state )//open device
   {
       FM_Init();
	   
       ctl_result = FM_Open();
       Init_FM_State();
   }


   *dest_ptr = (MSG_HEAD_T*)SCI_ALLOC_APP(sizeof(uint8) + sizeof(MSG_HEAD_T));
   SCI_ASSERT(SCI_NULL!=*dest_ptr);/*assert verified*/

   SCI_MEMCPY((void*)(*dest_ptr),(void*)src_ptr,sizeof(MSG_HEAD_T));

   SCI_MEMCPY((void*)(*dest_ptr + 1),(void*)&ctl_result,sizeof(uint8));

   head_ptr = (MSG_HEAD_T*)*dest_ptr;
   
   head_ptr->len = (uint16) (sizeof(uint8) + sizeof(MSG_HEAD_T));
   
   //*dest_ptr = (uint8*)(*dest_ptr);
   *dest_len_ptr = head_ptr->len;
 
   //head_ptr->len = *dest_len_ptr;
 #endif  
}

/*****************************************************************************/
//  Description:    This function control fm seek 
//                  
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void FM_Seek(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
   MSG_HEAD_T        *head_ptr      = NULL;

   FM_SIGNAL_PARAM_T signalValue;
 
   uint16              resultRssi       = 0;
   uint8             resultnStereoInd = 0;
   
   uint8              result;
   uint8             strValue[4]    ;
   uint32            freqValue    = 0;
   uint32                startFreq   = 0;
   uint8                 direct      = 0;

   uint16            s_search_ret = 0;

   uint32              rssiStatusValue = 0;
   uint32              noLpfStatusValue = 0;
   
  #ifndef WIN32
   //MMIFM_FreePlaySrv();
  
   *dest_ptr = (MSG_HEAD_T*)SCI_ALLOC_APP(sizeof(FM_SIGNAL_PARAM_T) + sizeof(MSG_HEAD_T));

   SCI_ASSERT(SCI_NULL!=*dest_ptr);/*assert verified*/
   
  // strValue = (uint8*)SCI_ALLOC_APP(4);
  // SCI_ASSERT(NULL!=strValue);
  
   SCI_MEMCPY((void*)strValue,(void*)(src_ptr+sizeof(MSG_HEAD_T)),sizeof(uint32));
 
   freqValue = translateNum(strValue);

   freqValue /= 100;
   
  // if ( NULL != strValue )
   {
   //   SCI_FREE(strValue);
   }
   
   startFreq  = freqValue;
   direct = *(src_ptr + sizeof(MSG_HEAD_T)+ sizeof(uint32));

   s_search_ret = SearchValidFreq(freqValue,direct);
   
   startFreq = (uint32)(channel_info.auto_freq_tab_ptr[0]);

   if (s_search_ret != 2)
   {
      result = 1;
   }
   else
   {
      result = 0;
   }
  
   signalValue.nFreqValue = startFreq * 100;

  // resultRssi       = (uint16)(FM_Ioctl(FM_GET_RSSI,nRssiPtr));//get rssi value
  // resultnStereoInd = (uint8)(FM_Ioctl(FM_GET_RSSI,nStereoIndPtr));//get stereoInd;

   resultRssi       = FM_SPRD_RecRSSI();
   resultnStereoInd = FM_SPRD_RecStereo();
   
   signalValue.nOperInd      = result;//0---SUCCESSED,1----FAILED
   signalValue.nRssi      =    resultRssi;
   signalValue.nStereoInd =    resultnStereoInd;
   signalValue.nPwrIndicator = TB_REG_GET(REG_INPWR_STS);
   signalValue.nFreqOffset   = TB_REG_GET(REG_FREQ_OFF_STS);

   rssiStatusValue = TB_REG_GET(REG_RSSI_STS);
   noLpfStatusValue = TB_REG_GET(REG_NO_LPF_STS);

   signalValue.nPilotDet     = (TB_REG_GET(REG_INPWR_STS)&(BIT_19|BIT_18));

   signalValue.nNoDacLpf     = (TB_REG_GET(REG_NO_LPF_STS)&(BIT_1|BIT_0));


   SCI_MEMCPY((void*)(*dest_ptr),(void*)src_ptr,sizeof(MSG_HEAD_T));


   head_ptr = (MSG_HEAD_T*)*dest_ptr;
   head_ptr->len = (uint16)(sizeof(FM_SIGNAL_PARAM_T)+sizeof(MSG_HEAD_T));
   
   *dest_ptr = (uint8*)(head_ptr);
   SCI_MEMCPY((void*)(*dest_ptr+8),(void*)((uint8*)&signalValue),sizeof(FM_SIGNAL_PARAM_T));

   *dest_ptr = (uint8*)(head_ptr);
   *dest_len_ptr =  head_ptr->len;
 
   if ( s_search_ret == 2 )
   {
      FM_ManualSeek(signalValue.nFreqValue,FM_SCAN_UP, 0, NULL);
      AUDIO_Play(s_fm_handle,0);		    
   }
 #endif 
}

/*****************************************************************************/
//  Description:    This function read fm register 
//                  
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void FM_Read_Reg(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len)
{
   MSG_HEAD_T        *head_ptr      = NULL;

   uint32            nStartAddr;
   uint32 	         nUintCount;

   uint32              i;
   
   uint16*           regStrValue  = NULL;
   
   
  #ifndef WIN32  
  
   nStartAddr = MAKELONG(MAKEWORD(src_ptr[12],src_ptr[13]),MAKEWORD(src_ptr[14],src_ptr[15]));
   nUintCount = MAKELONG(MAKEWORD(src_ptr[16],src_ptr[17]),MAKEWORD(src_ptr[18],src_ptr[19]));

   *dest_ptr = (MSG_HEAD_T*)SCI_ALLOC_APP(nUintCount *sizeof(uint16) + sizeof(FM_RW_REG_T) + sizeof(MSG_HEAD_T));

   SCI_ASSERT(SCI_NULL!=*dest_ptr);/*assert verified*/
   SCI_MEMSET(*dest_ptr,0,nUintCount *sizeof(uint16) + sizeof(FM_RW_REG_T) + sizeof(MSG_HEAD_T));

   regStrValue = (uint16*)SCI_ALLOC_APP(nUintCount * sizeof(uint16));
   SCI_ASSERT(NULL!=regStrValue);/*assert verified*/

   for(i = 0 ; i < nUintCount;++i)
   {
      regStrValue[i] = (uint16)TB_REG_GET(nStartAddr + i * 4);/*lint !e613*/
   }

   //*dest_ptr    = (uint8*)(*dest_ptr);

 //  regStrValue  = (uint8*)(regStrValue);

   SCI_MEMCPY((void*)*dest_ptr,(void*)src_ptr,20);
   
   SCI_MEMCPY((void*)(*dest_ptr + 20),(uint8*)regStrValue,sizeof(uint16) * nUintCount);

   SCI_FREE(regStrValue);
  
   
   head_ptr = (MSG_HEAD_T*)(*dest_ptr);
   head_ptr->len = (uint16)(sizeof(uint16) * nUintCount + sizeof(FM_RW_REG_T) + sizeof(MSG_HEAD_T));
   
   *dest_ptr = (uint8*)(head_ptr);
   
   *dest_len_ptr =  head_ptr->len;
 
 #endif 
}

/*****************************************************************************/
//  Description:    This function write fm register 
//                  
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void FM_Write_Reg(uint8 **dest_ptr,uint16 *dest_len_ptr,const uint8 *src_ptr,uint16 src_len)
{
   MSG_HEAD_T        *head_ptr      = NULL;
  // BOOLEAN           srcResult     = 0;

  // uint8 	         nErrorCode;   	// DIAG_FM_REG_ERR_E
   uint32            nStartAddr;
   uint32 	         nUintCount;
   uint32              i;
   
   uint16*           regStrValue  = NULL;
   

  #ifndef WIN32  
  
   nStartAddr = MAKELONG(MAKEWORD(src_ptr[12],src_ptr[13]),MAKEWORD(src_ptr[14],src_ptr[15]));
   nUintCount = MAKELONG(MAKEWORD(src_ptr[16],src_ptr[17]),MAKEWORD(src_ptr[18],src_ptr[19]));
   
   *dest_ptr = (MSG_HEAD_T*)SCI_ALLOC_APP(sizeof(FM_RW_REG_T) + sizeof(MSG_HEAD_T));

   SCI_MEMSET(*dest_ptr,0,sizeof(FM_RW_REG_T) + sizeof(MSG_HEAD_T));

   SCI_ASSERT(SCI_NULL!=*dest_ptr);/*assert verified*/
  
   regStrValue = (uint16*)SCI_ALLOC_APP(nUintCount * sizeof(uint16));

   SCI_ASSERT(NULL!=regStrValue);/*assert verified*/
   
   //regStrValue = (uint8*)regStrValue;
   SCI_MEMCPY((uint8*)regStrValue,(void*)(src_ptr + 20),sizeof(uint16)*nUintCount);
  // regStrValue = (uint16*)regStrValue;


   for(i = 0 ; i < nUintCount;++i)
   {
	  	TB_REG_SET(nStartAddr + i * 4,(uint32)regStrValue[i]);/*lint !e613*/
   }
  
   //*dest_ptr    = (uint8*)(*dest_ptr);

   SCI_MEMCPY((void*)*dest_ptr,(void*)src_ptr,20);
   
   SCI_FREE(regStrValue);
   
   head_ptr = (MSG_HEAD_T*)(*dest_ptr);
   head_ptr->len = (uint16)(sizeof(FM_RW_REG_T) + sizeof(MSG_HEAD_T));
   
   *dest_ptr = (uint8*)(head_ptr);
   
   *dest_len_ptr =  head_ptr->len;
 
 #endif
}
