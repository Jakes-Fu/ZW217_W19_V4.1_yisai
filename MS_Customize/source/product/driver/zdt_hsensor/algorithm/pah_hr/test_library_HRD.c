
#include "pah8009_testpattern_HRD.h"
// platform
#include <stdlib.h>
#include "pah800x_main.h"
#include "pah800x_bsp.h"
#include <math.h>  
//#include "nrf_delay.h"
#define array_szie(arr) (sizeof(arr) / sizeof(arr[0]))




void test_library_HRD(void)
{
	int i               = 0;
  void *hr_pBuffer = NULL;
	static ppg_mems_data_t ppg_mems_data_alg;

	sRaw_Data_13Bytes hr_data;
		 
	
	// static  Hr-V301009
		
		PxiAlg_Close();
		int32_t version = PxiAlg_Version();
		DEBUG_PRINT("HR_Version = %d \r\n", version);
       

		uint32_t Query_Mem_Size = PxiAlg_Query_Mem_Size();
		DEBUG_PRINT("HR_Mem_Size = %d \r\n", Query_Mem_Size);
		
		hr_pBuffer = malloc(Query_Mem_Size);
		
   
		if(!PxiAlg_Open_Mem(hr_pBuffer))
				DEBUG_PRINT("PxiAlg_Open_Mem fail \n");
	
			
		PxiAlg_SetQualityThreshold(0.35f); 
		PxiAlg_SetHRSmoothWeight(0.9f); 
		PxiAlg_SetProcWindow(5.0f, 5.0f); 
	  PxiAlg_SetTimeoutLength(30.0f);
		PxiAlg_SetMemsScale(1);		      //MEMS 2/4/8/16G : 0/1/2/3
		PxiAlg_SetSkipSampleNum(10); 
		PxiAlg_SetMemsScaleBase(1, 0);	//MEMS 2/4/8/16G : 0/1/2/3
		PxiAlg_SetForceOutputLink(0, 150, 40, 1, 0.9f); 
		PxiAlg_SetNormalOutput(200, 40, 0); 
		PxiAlg_SetForceRipple(3, 1); 
    //PxiAlg_SetForceOutput(30, 120, 45);
		//PxiAlg_SetForceOutputTime(10);
		PxiAlg_SetPackage(0, 1);
	
	
	  
//-----------------------------------------------//
	   
		LOG_PRINT("ARRAY_SIZE = %d\n",array_szie(PAH8009_rawdata_HRD));
//******************************************************************		 
	   
		float   hr         = 0.0f;
		float   hr_grader  = 0.0f;
	  
//*******************************************************************		 
		 
	   //nrf_delay_ms(50);
		 

		for( i = 0 ; i <array_szie(PAH8009_rawdata_HRD) ; i ++)
		{
			  
						memset(&hr_data, 0, sizeof(hr_data));
        		hr_data.loss        = 1;
        		hr_data.rawData     = (uint32_t)PAH8009_rawdata_HRD[i][1] ;
				    //LOG_PRINT("rawData = %d \n ", hr_data.rawData);
        		hr_data.non_zero1   = 1;
        		hr_data.non_zero2   = 2;
        		hr_data.non_zero3   = 3;
        		hr_data.frame_count = frame_HRD[i];
						//LOG_PRINT("frame_count = %d  ", hr_data.frame_count);
        		hr_data.time        = duration_HRD[i];	
						//LOG_PRINT("time = %d  ", hr_data.time);
        		hr_data.rsv         = 0;
        		hr_data.touch       = touch_HRD[i] > 0 ? 0x80 : 0x00;
						//LOG_PRINT("touch = %d  ", hr_data.touch);
        		hr_data.rsv2        = 0;

						int exposure_time      = Expo_time[i][1];
            //LOG_PRINT("exposure_time = %d  \n",exposure_time);

						ppg_mems_data_alg.MEMS_Data[0]=PAH8009_mems_HRD[i][0];
						ppg_mems_data_alg.MEMS_Data[1]=PAH8009_mems_HRD[i][1]; 
						ppg_mems_data_alg.MEMS_Data[2]=PAH8009_mems_HRD[i][2]; 
						
						int32_t ret = PxiAlg_Process_Expo((uint8_t*)&hr_data, ppg_mems_data_alg.MEMS_Data, exposure_time);

            //LOG_PRINT("ret_HR %d  \n",ret);

						if(PxiAlg_GetHrUpdateFlag())
						{
							PxiAlg_HrGet(&hr);
							PxiAlg_GetSigGrade(&hr_grader);
						}

						LOG_PRINT("ret  %d  , hr  %f ,  hr_grade  %f  \n", ret,  hr, hr_grader);
			 
		}
	 
    PxiAlg_Close();

    return;
}
