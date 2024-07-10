
// testpattern
#include "pah8009_testpattern_SPO2.h"
// platform
#include <stdlib.h>
#include "pah800x_main.h"
#include "pah800x_bsp.h"
#include <math.h>  


#define array_szie(arr) (sizeof(arr) / sizeof(arr[0]))

static SpO2_mems_t  mems_data_raw[3];        //add item

typedef struct
{
    void *pBuffer;
    void *hr_pBuffer;
} pxialg_spo2_state;



//#define debug_fpritnf(...) pah_print(__LINE__, __func__, __FILENAME__, __VA_ARGS__)

//void log_printf(char *s, float f)
//{
//    debug_fpritnf("%s = %f\n", s, f);
//}



  
void test_library_SPO2(void)
{
    int i               = 0;
    int j               = 0;
    int32_t version     = 0;
    float   mySpO2      = 0;
    uint8_t ReadyFlag   = 0;
    int     buffer_size = 0;
    void *pBuffer = NULL;
    void *hr_pBuffer = NULL;
  

    static ppg_mems_data_t ppg_mems_data_alg;
    pah8series_data_t   pxialg_data;
    sRaw_Data_13Bytes hr_data_offline;
     
    version     = SpO2_Version();
    LOG_PRINT("SPO2_version =%d \n",version);
    buffer_size = SpO2_OpenSize();
    LOG_PRINT("SPO2_size = %d  \n", buffer_size);
    pBuffer = malloc(buffer_size);
  

     //nrf_delay_ms(50);
    if (!pBuffer)
    {
        LOG_PRINT("Out of memory. \n");
        return;
    }
     //nrf_delay_ms(50);
    if (SpO2_Open(pBuffer) != SPO2_MSG_SUCCESS)
    {
        LOG_PRINT("SpO2_Open() failed. \n");
        free(pBuffer);
        return;
    }
    SpO2_SetInputRatioHighBound(96.0f);
    SpO2_SetInputRatioLowBound(92.0f);
    SpO2_SetInputModelScale(0.5f);

    SpO2_SetMotionDetectionLevel(2.0);
       
    SpO2_SetInputCoefHigh0(-34034.835111f);
    SpO2_SetInputCoefHigh1(1059.182624f);
    SpO2_SetInputCoefHigh2(-10.974873f);
    SpO2_SetInputCoefHigh3(0.037965f);

    SpO2_SetInputCoefMid0(57266.566962f);
    SpO2_SetInputCoefMid1(-1813.030574f);
    SpO2_SetInputCoefMid2(19.149586f);
    SpO2_SetInputCoefMid3(-0.067372f);

    SpO2_SetInputCoefLow0(135.917239f);
    SpO2_SetInputCoefLow1(-4.805347f);
    SpO2_SetInputCoefLow2(0.082882f);
    SpO2_SetInputCoefLow3(-0.000396f);
    
    SpO2_SetIIRFilterRatio(0.5f);
    SpO2_SetInputCorrelationTh(0.949999988079071);
    SpO2_SetInputRatioLow(0.0500000007450581f);    

    SpO2_SetInputFlagCheckHr(1);
    SpO2_Set_Update_Sec(1.0);   
    SpO2_Set_Init_Low_SpO2_Constrain(1.0);
       
    SpO2_SetOutputHighBound(100.0f);
    SpO2_SetOutputLowBound(60.0f);
       
    SpO2_Set_Keep_Init_SpO2_Bound(94.80f);
    SpO2_Set_Keep_Init_SpO2_Count_Th(60.0f);
    SpO2_Set_SpO2_Bias_Value(0.5f);
    SpO2_Set_SpO2_Outlier_Smooth_Coef(5.5f);
    SpO2_Set_SpO2_Decrease_Coef(0.5f);
    SpO2_SetSkipDataLen(25);
 
       
       
    /// static  Hr
    version = PxiAlg_Version();
    //LOG_PRINT("HR_Version = %d \n", version);    
   
    uint32_t Query_Mem_Size = PxiAlg_Query_Mem_Size();
    //LOG_PRINT("HR_Mem_Size = %d \n", Query_Mem_Size);
    
    hr_pBuffer = malloc(Query_Mem_Size);  
    
    if(!PxiAlg_Open_Mem(hr_pBuffer))
      DEBUG_PRINT("PxiAlg_Open_Mem fail \n");
   
    //V301009 parameter defeine
        
    PxiAlg_SetQualityThreshold(0.35f); 
    PxiAlg_SetHRSmoothWeight(0.9f); 
    PxiAlg_SetProcWindow(5.0f, 5.0f); 
    PxiAlg_SetTimeoutLength(30.0f);
    PxiAlg_SetMemsScale(1);          //MEMS 2/4/8/16G : 0/1/2/3
    PxiAlg_SetSkipSampleNum(10); 
    PxiAlg_SetMemsScaleBase(1, 0);  //MEMS 2/4/8/16G : 0/1/2/3
    PxiAlg_SetForceOutputLink(15, 150, 40, 1, 0.9f); 
    PxiAlg_SetNormalOutput(200, 40, 0); 
    PxiAlg_SetForceRipple(3, 1); 
    //PxiAlg_SetForceOutput(30, 120, 45);
    PxiAlg_SetForceOutputTime(15);
    PxiAlg_SetPackage(0, 1);


       

//-----------------------------------------------//
     
    int32_t  alg_count=0;
    //int32_t  mems_index_count = 0 ;
    LOG_PRINT("ARRAY_SIZE = %d\n",array_szie(PAH8112_rawdata_SPO2));
//******************************************************************     
    float   correlation= 0.0f;
    float   trust_flag = 0.0f;
    float   MEMS_RMS   = 0.0f;
    float   myHR       = 0.0f;
    int     ret_value  = 0   ;
    float   hr_grader  = 0.0f;
    
//*******************************************************************     
     
     //nrf_delay_ms(50);
     

    for( i = 0 ; i <array_szie(PAH8112_rawdata_SPO2) ; i ++)
    {
        
        memset(&hr_data_offline, 0, sizeof(hr_data_offline));
        hr_data_offline.loss        = over_flow[i]+1;
        //LOG_PRINT("over_flow = %d \n ", over_flow[i]);
        hr_data_offline.rawData     = PAH8112_rawdata_SPO2[i][0] ;
        //LOG_PRINT("rawData = %d \n ", hr_data_offline.rawData);
        hr_data_offline.non_zero1   = 1; 
        hr_data_offline.non_zero2   = 2;
        hr_data_offline.non_zero3   = 3;
        hr_data_offline.frame_count = frame_SPO2[i];
        //LOG_PRINT("frame_count = %d  ", hr_data.frame_count);
        hr_data_offline.time        = duration_SPO2[i];  
        //LOG_PRINT("time = %d \n ", hr_data_offline.time );
        hr_data_offline.rsv         = 0;
        hr_data_offline.touch       = touch_SPO2[i] > 0 ? 0x80 : 0x00;
        //LOG_PRINT("touch = %d  ", hr_data.touch);
        hr_data_offline.rsv2        = 0;
        int       exposure_time      = Expo_time[i][0];
    
        int32_t ret = PxiAlg_Process_Expo((uint8_t*)&hr_data_offline, ppg_mems_data_alg.MEMS_Data, exposure_time);

         
        //if( ret == FLAG_DATA_READY) 
        if(PxiAlg_GetHrUpdateFlag())
        {
            PxiAlg_HrGet(&myHR);
            uint8_t ready_flag = PxiAlg_GetReadyFlag();
            PxiAlg_GetSigGrade(&hr_grader);
        }  
         
        uint32_t  touch_flag         = touch_SPO2[i];         
        uint32_t  time               = duration_SPO2[i];        
        //int       ppg_count_per_ch   = NF_PPG_PER_CHANNEL;  
        int       frame_number       = frame_SPO2[i]; 
        //int       mems_number        = NF_PPG_PER_CHANNEL;     
        int32_t   RAW_DATA [3]       = {0}; 
        //float     MEMS_DATA[3]       = {0.0f};
        //int       exposure_time      = Expo_time[i][0];
        //LOG_PRINT("exposure_time = %d  \n",exposure_time);

        for (j=0;j<3; j++)
        {
            RAW_DATA[j] = PAH8112_rawdata_SPO2[i][j] ;
            //LOG_PRINT("RAW_DATA = %d\n", RAW_DATA[j]);
        }

        mems_data_raw[0].data[0]=PAH8112_mems_SPO2[i][0];
        mems_data_raw[0].data[1]=PAH8112_mems_SPO2[i][1];
        mems_data_raw[0].data[2]=PAH8112_mems_SPO2[i][2];   
    
        ppg_mems_data_alg.MEMS_Data[0]=PAH8112_mems_SPO2[i][0];
        ppg_mems_data_alg.MEMS_Data[1]=PAH8112_mems_SPO2[i][1]; 
        ppg_mems_data_alg.MEMS_Data[2]=PAH8112_mems_SPO2[i][2]; 
            
        //PxiAlg_HrGet(&myHR);
        //LOG_PRINT("ret_HR %d  \n",ret);


        //nrf_delay_ms(40);
        pxialg_data.touch_flag         = touch_flag;
        pxialg_data.time               = time;
        pxialg_data.ppg_data           = (int32_t*)RAW_DATA;
        pxialg_data.nf_ppg_channel     = 2;
        pxialg_data.nf_ppg_per_channel = 1;
        pxialg_data.frame_count        = frame_number;
        pxialg_data.nf_mems            = 1;
        pxialg_data.mems_data          = (int16_t*)mems_data_raw;
 
        //ret_value = PxiAlg_Entrance(&pxialg_data); 
         
        alg_count ++ ;
        //nrf_delay_ms(40);
         
        //nrf_delay_ms(100);
        SpO2_SetHrValue(myHR);
        SpO2_SetMems(mems_data_raw, pxialg_data.nf_mems);
        ret_value = SpO2_Process(1, NF_PPG_CHANNEL, pxialg_data.ppg_data, pxialg_data.nf_ppg_per_channel);
        SpO2_GetSpO2(&mySpO2);
        SpO2_GetReadyFlag(&ReadyFlag);
        SpO2_info_t    _spo2_detect_item;
        SpO2_GetInfo(&_spo2_detect_item);
        correlation=_spo2_detect_item.Noise;
        trust_flag =_spo2_detect_item.Output[3];
        MEMS_RMS  =_spo2_detect_item.RMS;
           
                  
         //nrf_delay_ms(100);
        if( ret_value == 0  && (alg_count>=25) )
        {        
            alg_count = alg_count -25;
            if (ReadyFlag > 0)
            {
                LOG_PRINT("Ready = %d,", ReadyFlag);
                       // nrf_delay_ms(100);
                LOG_PRINT("myHR = %f ",myHR);
                        //nrf_delay_ms(100);
                LOG_PRINT("grade = %f ",hr_grader);
                        //nrf_delay_ms(100);
                LOG_PRINT(",mySpO2 = %f ",mySpO2);
                        //nrf_delay_ms(100);
                LOG_PRINT(",correlation = %f ",correlation);
                         //nrf_delay_ms(100);
                LOG_PRINT(",trust_flag = %f ",trust_flag);
                        //nrf_delay_ms(100);
                LOG_PRINT(",MEMS_RMS    = %f ",MEMS_RMS); 
                
                LOG_PRINT(",ret    = %d  \n",ret); 
            
            }
            else
            {
                LOG_PRINT("Ready = %d,", ReadyFlag);
                        //nrf_delay_ms(100);
                LOG_PRINT("myHR = %f ",myHR);
                        //nrf_delay_ms(100);
                LOG_PRINT("grade = %f ",hr_grader);
                        //nrf_delay_ms(100);
                LOG_PRINT(",mySpO2 = %f ",mySpO2);
                        //nrf_delay_ms(100);
                LOG_PRINT(",correlation = %f ",correlation);
                         //nrf_delay_ms(100);
                LOG_PRINT(",trust_flag = %f ",trust_flag);
                        //nrf_delay_ms(100);
                LOG_PRINT(",MEMS_RMS    = %f   ",MEMS_RMS);

                LOG_PRINT(",ret    = %d  \n",ret);             
            } 
        }
        else
        {
            switch (ret_value)
            {
                case SPO2_MSG_ALG_NOT_OPEN:
                  LOG_PRINT("Algorithm is not initialized. \n");
                break;
                    
                case SPO2_MSG_ALG_REOPEN:
                  LOG_PRINT("Algorithm is re initialized. \n");
                break;
                    
                case SPO2_MSG_ALG_NOT_SUPPORT:
                  LOG_PRINT("Algorithm is not support. \n");
                break;
                    
                default:
                         //LOG_PRINT("Algorithm unhandle error = %d \n", ret_value);
                break;
            }
        }
        //NRF_LOG_FLUSH();
        //nrf_delay_ms(40); 
  }
  
    //LOG_PRINT("Out of memory. \n");
    //nrf_delay_ms(10);
    SpO2_Close();
   PxiAlg_Close();
    free(pBuffer);
    free(hr_pBuffer);
    return;
}
