// pah
#include "pah800x_main.h"
#include "pah_8009_reg_arr_default_20211101.h"

//============================================================================
// Static Global Variable
//============================================================================
static main_state _main;
static ppg_mems_data_t ppg_mems_data_buf[FIFO_SIZE] ;
static int _read_index = 0 ;
static int _write_index = 0 ;
    
// hr algotirhm
static float hr = 0.0f;
static float hr_grader =0.0f;
static ppg_mems_data_t ppg_mems_data_alg;

// spo2 algorithm
void *pBuffer                 = NULL;
void *hr_pBuffer              = NULL;    
static float myHR             = 0.0f;
static float mySpO2           = 0.0f;
static float correlation      = 0.0f;
static float trust_flag       = 0.0f;
static float MEMS_RMS         = 0.0f;
uint8_t      mySPO2_ReadyFlag = 0   ;
static  uint32_t   data_buf_count_spo2_alg=0;

static  SpO2_info_t    _spo2_detect_item;
pah8series_data_t      pxialg_data_spo2;
static  SpO2_mems_t    mems_data_raw[150];  
#ifdef PPG_MEMS_Synchronize 
int16_t mems_int_data_in[90] ={0};
int16_t mems_int_data_out[90]={0};
int time_temp=0;  
static uint8_t ppg_data_num=0;
uint8_t mems_data_polling_ready = 0 ;  
#endif
// Stress Algorithm 
//static hrv_state_s _hrv_state;
void *stress_pBuffer              = NULL;    
#define HRV_ENABLE_LHR                1
#define HRV_SAMPLING_RATE                20
#define HRV_COLLECTION_SECONDS          180   
static     uint32_t  duration_total  = 0;
uint8_t SDNN_ReadyFlag               = 0;
uint8_t Get_Stress                   = 0;
static  float Stress                 = 0.0f;
static  uint8_t HRAvg                = 0;
static  float SQ_Stress              = 0.0f;
static  uint8_t AnomalyFlag          = 0;
uint8_t Stress_ReadyFlag             = 0;
uint8_t Weak_Flag                    = 0;
float pah8009_hr_data = 0;
float pah8009_spo2_data           = 0;
//============================================================================
// Private function
//============================================================================
static bool sensor_basic_verification(void);    
static void process(uint64_t timestamp);
static void ppg_sensor_enable(void);
static void ppg_sensor_disable(void);
static void touch_sensor_enable(void);
static void touch_sensor_disable(void);

static void report_touch_data(uint64_t timestamp, const int8_t *touch_flag);
static void report_ppg_data(const ppg_mems_data_t *ppg_mems_data_alg);
static void report_hr_data(const ppg_mems_data_t *ppg_mems_data_alg, int32_t ret, float hr, float hr_grader);
static void report_spo2_data(const ppg_mems_data_t *ppg_mems_data_alg, int32_t ret, float myHR, float mySpO2,  float correlation, float trust_flag, float MEMS_RMS, int data_buf_count_spo2_alg);
static void report_stress_data(const ppg_mems_data_t *ppg_mems_data_alg, uint8_t Stress_ReadyFlag, uint32_t measuretime, float collect_time);

static void error_handle(void);


/*****************************
FW FIFO Function
*****************************/
bool isFIFOEmpty(void)
{
    return (_write_index == _read_index) ;
}

bool pah_push(ppg_mems_data_t *data) //Push return data must be true. If return false, please increase FIFO size.  
{
    int tmp = _write_index ;
    tmp++;
    if(tmp >= FIFO_SIZE)
        tmp = 0 ;
    if(tmp == _read_index)
        return false;
    ppg_mems_data_buf[tmp] = *data ;
    _write_index = tmp ;
    return true;
}

bool pah_pop(ppg_mems_data_t *data)
{
    int tmp ;
    if(isFIFOEmpty())
        return false;
    tmp = _read_index + 1;
    if(tmp >= FIFO_SIZE)
        tmp = 0 ;    
    _read_index = tmp ;
    *data = ppg_mems_data_buf[_read_index] ;
    
    return true;
}


//============================================================================
// Public function
//============================================================================
void pah_sensor_init(reg_config_type type)
{
    int ch=0;
    pah_800x_reg_setting  reg_setting;
    pah_800x_config     pah_800x_cfg;
    memset(&_main, 0, sizeof(_main));

    memset(&reg_setting, 0, sizeof(reg_setting));
    
    if (!sensor_basic_verification())
        error_handle();

    // pah_800x
    
    pah_800x_get_default_config(&pah_800x_cfg);
    LOG_PRINT("PPG_SETTING_VESION = %d \n" ,PPG_SETTING_VERSION);
    
#if (ENABLE_AE==0)
    pah_800x_cfg.enable_ppg_convergence[0] = false;
    pah_800x_cfg.enable_ppg_convergence[1] = false;
    pah_800x_cfg.enable_ppg_convergence[2] = false;
#endif

#if (ENABLE_AE_RECONVERGENCE==0)
    pah_800x_cfg.enable_ppg_reconvergence_if_out_of_bounds[0] = false;
    pah_800x_cfg.enable_ppg_reconvergence_if_out_of_bounds[1] = false;
    pah_800x_cfg.enable_ppg_reconvergence_if_out_of_bounds[2] = false;
#endif

    if (type == pc_4mm_2g_ir_hrd)
    {
#if (PC_4MM_2G_IR_HRD==1)
        PAH_REG_ARR_SETUP(reg_setting.startup_init, PAH8009_REG_DEVICE_INIT);
        PAH_REG_ARR_SETUP(reg_setting.ppg_enable, PAH8009_REG_PC_4MM_2G_IR_PPG);
        PAH_REG_ARR_SETUP(reg_setting.touch_enable, PAH8009_REG_PC_4MM_2G_IR_TOUCH);
#endif
    }
    else if (type == diw_4mm_g_ir_hrd)
    {
#if (DIW_4MM_G_IR_HRD==1)
        PAH_REG_ARR_SETUP(reg_setting.startup_init, PAH8009_REG_DEVICE_INIT);
        PAH_REG_ARR_SETUP(reg_setting.ppg_enable, PAH8009_REG_DIW_4MM_G_IR_PPG);
        PAH_REG_ARR_SETUP(reg_setting.touch_enable, PAH8009_REG_DIW_4MM_G_IR_TOUCH);
#endif
    }
    else if (type == diw_4mm_ir_r_ir_spo2)
    {
#if (DIW_4MM_IR_R_IR_SPO2==1)
        for(ch=0;ch<PAH_800x_MAX_CH_NUM;ch++)
        {
            pah_800x_cfg.ppg_convergence[ch].t_weight_normal_mode = 4;
            pah_800x_cfg.ppg_convergence[ch].t_weight_fast_mode = 8;
            pah_800x_cfg.ppg_convergence[ch].t_max = 150;
        }
        PAH_REG_ARR_SETUP(reg_setting.startup_init, PAH8009_REG_DEVICE_INIT);
        PAH_REG_ARR_SETUP(reg_setting.ppg_enable, PAH8009_REG_DIW_4MM_IR_R_IR_PPG);
        PAH_REG_ARR_SETUP(reg_setting.touch_enable, PAH8009_REG_DIW_4MM_IR_R_IR_TOUCH);
#endif
    }
    else if (type == pc_22mm_2g_ir_hrd)
    {
#if (PC_22MM_2G_IR_HRD==1)
        for(ch=0;ch<PAH_800x_MAX_CH_NUM;ch++)
        {
            pah_800x_cfg.ppg_convergence[ch].t_max = 1280;
            pah_800x_cfg.ppg_convergence[ch].raw_outer_high_bound = 14848;
            pah_800x_cfg.ppg_convergence[ch].raw_inner_high_target = 13824;
            pah_800x_cfg.ppg_convergence[ch].raw_inner_low_target = 10752;
            pah_800x_cfg.ppg_convergence[ch].raw_outer_low_bound = 9728;

        }
        PAH_REG_ARR_SETUP(reg_setting.startup_init, PAH800x_REG_DEVICE_INIT);
        PAH_REG_ARR_SETUP(reg_setting.ppg_enable, PAH800x_REG_PC_22MM_2G_IR_PPG);
        PAH_REG_ARR_SETUP(reg_setting.touch_enable, PAH800x_REG_PC_22MM_2G_IR_TOUCH);
#endif
    }
    else if (type == di_18mm_g_g_hrd)
    {
#if (DI_18MM_G_G_HRD==1)
        for(ch=0;ch<PAH_800x_MAX_CH_NUM;ch++)
        {
            pah_800x_cfg.ppg_convergence[ch].t_weight_normal_mode = 15;
            pah_800x_cfg.ppg_convergence[ch].t_weight_fast_mode = 25;
            pah_800x_cfg.ppg_convergence[ch].t_max = 2048;
        }
        PAH_REG_ARR_SETUP(reg_setting.startup_init, PAH8001_REG_DEVICE_INIT);
        PAH_REG_ARR_SETUP(reg_setting.ppg_enable, PAH8001_REG_DI_18MM_G_G_PPG);
        PAH_REG_ARR_SETUP(reg_setting.touch_enable, PAH8001_REG_DI_18MM_G_G_TOUCH);
#endif
    }

    _main.pah_800x.is_powered= false ;
    pah_800x_init(&_main.pah_800x,  &pah_800x_cfg);
    pah_800x_device_init(&_main.pah_800x, &reg_setting);

}

void touch_only_sensor_start(void)
{
    pah_800x_power_toggle(&_main.pah_800x, true);
    _main.is_Auto_mode = 0;
    touch_sensor_enable();
    _main.debug_print_header = true;

}

void ppg_only_sensor_start(void)
{
    pah_800x_power_toggle(&_main.pah_800x, true);
    
    ppg_sensor_enable();
   
    //_main.ppg_only_is_enabled = 1 ;
    _main.is_Auto_mode = 0;
    _main.debug_print_header = true;
    
}

void auto_mode_sensor_start(void)
{
    pah_800x_power_toggle(&_main.pah_800x, true);
    
    touch_sensor_enable();

    _main.is_Auto_mode = 1;
    _main.debug_print_header = true;
    
}

void pah_sensor_stop(void)
{
    if (_main.ppg_is_enabled)
    {
        ppg_sensor_disable();
    }
    if (_main.touch_is_enabled)
    {
        touch_sensor_disable();
    }
    pah_800x_deinit(&_main.pah_800x);      
      
}

void ppg_interrupt_handle(void)
{
    _main.has_interrupt = true;
    //_main.interrupt_timestamp = pah_get_tick_count();
}

void ppg_sensor_task_interrupt(void)
{
    if (_main.has_interrupt)
    {
        //LOG_PRINT("ppg_sensor_task_interrupt111111\n");
        _main.has_interrupt = false;
        _main.interrupt_timestamp = pah_get_tick_count();
        process(_main.interrupt_timestamp);
    }

} 

void ppg_sensor_task_polling(void)
{
    _main.interrupt_timestamp = pah_get_tick_count();
    process(_main.interrupt_timestamp);
} 

uint8_t Check_alg_enable(void)
{
    return _main.alg_is_enabled;
}

void Clear_alg_enable(void)
{
    _main.alg_is_enabled = MSG_ALG_NOCHANGE;
}


// HR
void hr_algorithm_enable(void)
{
    int version = 0;
    uint32_t Query_Mem_Size = 0;
    if (_main.hr_is_enabled)
        return;

    // static  Hr-V301009
        
    PxiAlg_Close();
    version = PxiAlg_Version();
    DEBUG_PRINT("HR_Version = %d \n", version);
   

    Query_Mem_Size = PxiAlg_Query_Mem_Size();
    DEBUG_PRINT("HR_Mem_Size = %d \n", Query_Mem_Size);
    
    hr_pBuffer = SCI_ALLOC_APPZ(Query_Mem_Size);
    if(hr_pBuffer == NULL)
    {
        DEBUG_PRINT("HR_Mem_malloc failed. \n");
    }
   
    if(!PxiAlg_Open_Mem(hr_pBuffer))
        DEBUG_PRINT("PxiAlg_Open_Mem fail \n");
                
            
    PxiAlg_SetQualityThreshold(0.35f); 
    PxiAlg_SetHRSmoothWeight(0.9f); 
    PxiAlg_SetProcWindow(5.0f, 5.0f); 
    PxiAlg_SetTimeoutLength(30.0f);
    PxiAlg_SetMemsScale(1);              //MEMS 2/4/8/16G : 0/1/2/3
    PxiAlg_SetSkipSampleNum(10); 
    PxiAlg_SetMemsScaleBase(1, 0);    //MEMS 2/4/8/16G : 0/1/2/3
    PxiAlg_SetForceOutputLink(0, 150, 40, 1, 0.9f); 
    PxiAlg_SetNormalOutput(200, 40, 0); 
    PxiAlg_SetForceRipple(3, 1); 
    //PxiAlg_SetForceOutput(30, 120, 45);
    //PxiAlg_SetForceOutputTime(10);
    PxiAlg_SetPackage(0, 1);

    _main.hr_is_enabled = true;
}

void hr_algorithm_disable(void)
{
    if (!_main.hr_is_enabled)
        return;

    PxiAlg_Close();
    SCI_FREE(hr_pBuffer);
    hr = 0.0f;
    hr_grader =0.0f;
    _main.hr_is_enabled = false;
}

void hr_alg_task(void)
{
    int32_t ret = 0;
    if(Check_alg_enable())
    {
        if(Check_alg_enable()==MSG_ALG_ENABLE)
        {
            hr_algorithm_enable();
        }else if(Check_alg_enable()==MSG_ALG_DISABLE)
        {
            hr_algorithm_disable();
        }
        Clear_alg_enable();
    }

    if(!isFIFOEmpty())
    {
        if(pah_pop(&ppg_mems_data_alg))    //Get data from FIFO
        {
            if(_main.hr_is_enabled)
            {
                // 13Bytes format to hr lib;
                sRaw_Data_13Bytes hr_data;

                memset(&hr_data, 0, sizeof(hr_data));
                hr_data.loss = (uint8_t)ppg_mems_data_alg.ppg_data.overflow_num + 1;
                hr_data.rawData = ppg_mems_data_alg.ppg_data.data[1];
                hr_data.non_zero1 = 1;
                hr_data.non_zero2 = 2;
                hr_data.non_zero3 = 3;
                hr_data.frame_count = ppg_mems_data_alg.ppg_frame_count;
                hr_data.time = ppg_mems_data_alg.duration;
                hr_data.rsv = 0;
                hr_data.touch = (ppg_mems_data_alg.touch_flag > 0 ? 0x80 : 0x00);
                hr_data.rsv2 = 0;
  
  
                ret = PxiAlg_Process_Expo((uint8_t*)&hr_data, ppg_mems_data_alg.MEMS_Data, ppg_mems_data_alg.ppg_data.exposure_time[1]);
  

                if(PxiAlg_GetHrUpdateFlag())
                {
                    PxiAlg_HrGet(&hr);
                    PxiAlg_GetSigGrade(&hr_grader);
                }
                if(ppg_mems_data_alg.touch_flag==0)
                {
                    hr = 0 ;
                    hr_grader = 0;
                }
           
                report_hr_data(&ppg_mems_data_alg, ret, hr, hr_grader);
    
            }
            else
            {
                report_ppg_data(&ppg_mems_data_alg);
            }
        }
    }
}

// HRD- ALG-  Task2
void hr_alg_task2(uint8_t ppg_num)
{
#ifdef PPG_MEMS_Synchronize
    int i=0;
    int32_t ret = 0;
    if(Check_alg_enable())
    {
        if(Check_alg_enable()==MSG_ALG_ENABLE)
        {
            hr_algorithm_enable();
        }else if(Check_alg_enable()==MSG_ALG_DISABLE)
        {
            hr_algorithm_disable();
        }
        Clear_alg_enable();
    }
  
    if(!isFIFOEmpty())
    {
        for (i=0; i<ppg_num; i++)
        {
              
            if(pah_pop(&ppg_mems_data_alg))  //Get data from FIFO
            {
                if(_main.hr_is_enabled)
                {
                    // 13Bytes format to hr lib;
                    sRaw_Data_13Bytes hr_data;
                    
                    ppg_mems_data_alg.MEMS_Data[0]=mems_int_data_out[3*i];
                    ppg_mems_data_alg.MEMS_Data[1]=mems_int_data_out[3*i+1];
                    ppg_mems_data_alg.MEMS_Data[2]=mems_int_data_out[3*i+2];
            
      
                    memset(&hr_data, 0, sizeof(hr_data));
                    hr_data.loss = (uint8_t)ppg_mems_data_alg.ppg_data.overflow_num + 1;
                    hr_data.rawData = ppg_mems_data_alg.ppg_data.data[1];
                    hr_data.non_zero1 = 1;
                    hr_data.non_zero2 = 2;
                    hr_data.non_zero3 = 3;
                    hr_data.frame_count = ppg_mems_data_alg.ppg_frame_count;
                    hr_data.time = ppg_mems_data_alg.duration;
                    hr_data.rsv = 0;
                    hr_data.touch = (ppg_mems_data_alg.touch_flag > 0 ? 0x80 : 0x00);
                    hr_data.rsv2 = 0;
      
                    ret = PxiAlg_Process_Expo((uint8_t*)&hr_data, ppg_mems_data_alg.MEMS_Data, ppg_mems_data_alg.ppg_data.exposure_time[1]);
  

                    if(PxiAlg_GetHrUpdateFlag())
                    {
                        PxiAlg_HrGet(&hr);
                        PxiAlg_GetSigGrade(&hr_grader);
                    }
               
                    report_hr_data(&ppg_mems_data_alg, ret, hr, hr_grader);
      
                }
                else
                {
                    report_ppg_data(&ppg_mems_data_alg);
                }
            }
        }
    }
#endif
}



// SPO2
void spo2_algorithm_enable(void)
{
    int ret = 0;
    int version = 0;
    uint32_t buffer_size = 0;
    uint32_t Query_Mem_Size = 0;
    if (_main.spo2_is_enabled)
    {
        return;
    }
    version = SpO2_Version();
    buffer_size = SpO2_OpenSize();

    DEBUG_PRINT("PxiAlg_Version() = %d\n", version);
        
    DEBUG_PRINT("SpO2_Open buffer_size = %d \n", buffer_size);

    pBuffer = SCI_ALLOC_APPZ(buffer_size);
    if(pBuffer == NULL)
    {
        DEBUG_PRINT("SpO2_malloc failed. \n");
        return;
    }
    
    ret = SpO2_Open(pBuffer);
    if (ret != SPO2_MSG_SUCCESS)
    {
        DEBUG_PRINT("SpO2_Open() failed. ret=%d",ret);
        SCI_FREE(pBuffer);
        return;
    }
        

    SpO2_SetInputRatioHighBound(96.0f);
    SpO2_SetInputRatioLowBound(92.0f);
    SpO2_SetInputModelScale(0.5f);
    
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
    SpO2_SetInputCorrelationTh(0.949999988079071f);
    SpO2_SetInputRatioLow(0.05f);        
    
    SpO2_SetInputFlagCheckHr(1);
    SpO2_SetMotionDetectionLevel(2.0f);
    SpO2_Set_Update_Sec(1);
    
    SpO2_Set_Init_Low_SpO2_Constrain(1.0);  
    SpO2_SetOutputHighBound(100.0f);
    SpO2_SetOutputLowBound(60.0f);
            
    SpO2_Set_Keep_Init_SpO2_Bound(94.80f);    //add For V571
    SpO2_Set_Keep_Init_SpO2_Count_Th(60.0f);  //add For V571
    SpO2_Set_SpO2_Bias_Value(0.5f);           //add For V571
    SpO2_Set_SpO2_Outlier_Smooth_Coef(5.5f);  //add For V571
    SpO2_Set_SpO2_Decrease_Coef(0.5f);        //add For V571
    
    SpO2_SetSkipDataLen(25);
    
#ifndef DISABLE_STATIC_HR_IN_SPO2		// disable static HR function in SpO2 process
    // static  Hr-V301009
    PxiAlg_Close();
    version = PxiAlg_Version();
    DEBUG_PRINT("HR_Version = %d \n", version);
        
    
    Query_Mem_Size = PxiAlg_Query_Mem_Size();
    DEBUG_PRINT("HR_Mem_Size = %d \n", Query_Mem_Size);
    
    hr_pBuffer = SCI_ALLOC_APPZ(Query_Mem_Size);
     if(hr_pBuffer == NULL)
    {
        DEBUG_PRINT("HR_Mem_malloc failed. \n");
    }
       
    
    if(!PxiAlg_Open_Mem(hr_pBuffer))
          DEBUG_PRINT("PxiAlg_Open_Mem fail \n");
             
            
    PxiAlg_SetQualityThreshold(0.35f); 
    PxiAlg_SetHRSmoothWeight(0.9f); 
    PxiAlg_SetProcWindow(5.0f, 5.0f); 
    PxiAlg_SetTimeoutLength(30.0f);
    PxiAlg_SetMemsScale(1);              //MEMS 2/4/8/16G : 0/1/2/3
    PxiAlg_SetSkipSampleNum(10); 
    PxiAlg_SetMemsScaleBase(1, 0);    //MEMS 2/4/8/16G : 0/1/2/3
    PxiAlg_SetForceOutputLink(15, 150, 40, 1, 0.9f); 
    PxiAlg_SetNormalOutput(200, 40, 0); 
    PxiAlg_SetForceRipple(3, 1); 
    //PxiAlg_SetForceOutput(30, 120, 45);
    PxiAlg_SetForceOutputTime(15);
    PxiAlg_SetPackage(0, 1);
#endif
    
    _main.spo2_is_enabled = true;
}

void spo2_algorithm_disable(void)
{
    if (!_main.spo2_is_enabled)
        return;
#ifndef DISABLE_STATIC_HR_IN_SPO2
    PxiAlg_Close();
#endif
    SpO2_Close();
    SCI_FREE(pBuffer);
#ifndef DISABLE_STATIC_HR_IN_SPO2
    SCI_FREE(hr_pBuffer);
#endif
    myHR                   = 0.0f;
    mySpO2                 = 0.0f;
    correlation            = 0.0f;
    trust_flag             = 0.0f;
    MEMS_RMS               = 0.0f;
    mySPO2_ReadyFlag       = 0;
    hr_grader              = 0.0f;
    data_buf_count_spo2_alg=0;
        
    _main.spo2_is_enabled = false;
}

void spo2_alg_task(void)
{
    int32_t ret = 0;
    if(Check_alg_enable())
    {
        if(Check_alg_enable()==MSG_ALG_ENABLE)
        {
            spo2_algorithm_enable();
        }else if(Check_alg_enable()==MSG_ALG_DISABLE)
        {
            spo2_algorithm_disable();
        }
        Clear_alg_enable();
    }

    if(!isFIFOEmpty())
    {
        if(pah_pop(&ppg_mems_data_alg))    //Get data from FIFO
        {
            if(_main.spo2_is_enabled)
            {
                // 13Bytes format to hr lib;
                    
                int32_t  RAW_DATA[2]  ;
                sRaw_Data_13Bytes hr_data;
                int32_t spo2_raw_1 = 0;
                int32_t spo2_raw_2 = 0;
                int  ret_value = 0;
                
                memset(&hr_data, 0, sizeof(hr_data));
                hr_data.loss = (uint8_t)ppg_mems_data_alg.ppg_data.overflow_num + 1;
                hr_data.rawData = ppg_mems_data_alg.ppg_data.data[0];
                hr_data.non_zero1 = 1;
                hr_data.non_zero2 = 2;
                hr_data.non_zero3 = 3;
                hr_data.frame_count = ppg_mems_data_alg.ppg_frame_count;
                hr_data.time = ppg_mems_data_alg.duration;
                hr_data.rsv = 0;
                hr_data.touch = (ppg_mems_data_alg.touch_flag > 0 ? 0x80 : 0x00);
                hr_data.rsv2 = 0;
           
			#ifndef DISABLE_STATIC_HR_IN_SPO2
                ret = PxiAlg_Process_Expo((uint8_t*)&hr_data, ppg_mems_data_alg.MEMS_Data, ppg_mems_data_alg.ppg_data.exposure_time[0]);
                        
                if(PxiAlg_GetHrUpdateFlag())
                {
                    PxiAlg_HrGet(&myHR);
                    PxiAlg_GetSigGrade(&hr_grader);
                }
			#else
				ret = 0;		// manually assign 0 to ret(let SpO2 process keep going)
				myHR = 87.654321f;		// manually assign fake HR value to myHR(let SpO2 process keep going)
			#endif

                spo2_raw_1=ppg_mems_data_alg.ppg_data.data[0];
                spo2_raw_2=ppg_mems_data_alg.ppg_data.data[1];
                
                RAW_DATA[0]=spo2_raw_1;
                RAW_DATA[1]=spo2_raw_2;

                mems_data_raw[0].data[0]=ppg_mems_data_alg.MEMS_Data[0];
                mems_data_raw[0].data[1]=ppg_mems_data_alg.MEMS_Data[1] ;
                mems_data_raw[0].data[2]=ppg_mems_data_alg.MEMS_Data[2];            

                // SPO2 fill data on pah8series structure
                pxialg_data_spo2.touch_flag         =  ppg_mems_data_alg.touch_flag ;
                pxialg_data_spo2.time               =  ppg_mems_data_alg.duration ;;
                pxialg_data_spo2.ppg_data           =  (int32_t*)RAW_DATA;
                pxialg_data_spo2.nf_ppg_channel     =  2;
                pxialg_data_spo2.nf_ppg_per_channel =  1;
                pxialg_data_spo2.frame_count        =  ppg_mems_data_alg.ppg_frame_count ;
                pxialg_data_spo2.nf_mems            =  1;
                pxialg_data_spo2.mems_data          = (int16_t*)mems_data_raw;

                //DEBUG_PRINT("data_buf_count_spo2_alg = %d \n", data_buf_count_spo2_alg);

                SpO2_SetHrValue(myHR);
                SpO2_SetMems(mems_data_raw, 1);
                ret_value = SpO2_Process(ppg_mems_data_alg.touch_flag , 2, pxialg_data_spo2.ppg_data, 1);
    
                SpO2_GetSpO2(&mySpO2);
                SpO2_GetReadyFlag(&mySPO2_ReadyFlag);
                SpO2_GetInfo(&_spo2_detect_item);
                correlation =_spo2_detect_item.Noise;
                trust_flag  =_spo2_detect_item.Output[3];
                MEMS_RMS    =_spo2_detect_item.RMS;

                data_buf_count_spo2_alg++;
                        
                if( (ret_value == 0  ) && (data_buf_count_spo2_alg>=25) )
                {   
                    report_spo2_data(&ppg_mems_data_alg, ret, mySpO2, myHR, correlation, trust_flag, MEMS_RMS,data_buf_count_spo2_alg);
                    data_buf_count_spo2_alg = data_buf_count_spo2_alg -25;
                }
                else
                {
                    report_spo2_data(&ppg_mems_data_alg, ret, mySpO2, myHR, correlation, trust_flag, MEMS_RMS,data_buf_count_spo2_alg);

                    switch (ret_value)
                    {
                        case SPO2_MSG_ALG_NOT_OPEN:
                            DEBUG_PRINT("Algorithm is not initialized. \n");
                        break;
                     
                        case SPO2_MSG_ALG_REOPEN:
                            DEBUG_PRINT("Algorithm is re initialized. \n");
                        break;
                     
                        case SPO2_MSG_ALG_NOT_SUPPORT:
                            DEBUG_PRINT("Algorithm is not support. \n");
                        break;
                    
                        default:
                            //DEBUG_PRINT("Algorithm unhandle error = %d \n", ret_value);
                        break;
                    }
                    //return 1 ;
                }
            }
            else
            {
                report_ppg_data(&ppg_mems_data_alg);
            }
        }
    }
}



// SPO2- ALG-  Task2
void spo2_alg_task2(uint8_t ppg_num)
{
    int k=0;
#ifdef PPG_MEMS_Synchronize
    if(Check_alg_enable())
    {
        if(Check_alg_enable()==MSG_ALG_ENABLE)
        {
            spo2_algorithm_enable();
        }else if(Check_alg_enable()==MSG_ALG_DISABLE)
        {
            spo2_algorithm_disable();
        }
        Clear_alg_enable();
    }
  
    if(!isFIFOEmpty())
    {
        for (k=0; k<ppg_num; k++)
        {
            if(pah_pop(&ppg_mems_data_alg))  //Get data from FIFO
            {
                if(_main.spo2_is_enabled)
                {
                    // 13Bytes format to hr lib;
                    int32_t  RAW_DATA[2]={0}  ;
                    sRaw_Data_13Bytes hr_data;
                    int32_t ret = 0;
                    int32_t spo2_raw_1=0;
                    int32_t spo2_raw_2=0;
                    int time_bbb=0;
                    int ret_value = 0;
                    int time_eee=0;
                    
                    ppg_mems_data_alg.MEMS_Data[0]=mems_int_data_out[3*k];
                    ppg_mems_data_alg.MEMS_Data[1]=mems_int_data_out[3*k+1];
                    ppg_mems_data_alg.MEMS_Data[2]=mems_int_data_out[3*k+2];
          
                 
                    memset(&hr_data, 0, sizeof(hr_data));
                    hr_data.loss = (uint8_t)ppg_mems_data_alg.ppg_data.overflow_num + 1;
                    hr_data.rawData = ppg_mems_data_alg.ppg_data.data[0];
                    hr_data.non_zero1 = 1;
                    hr_data.non_zero2 = 2;
                    hr_data.non_zero3 = 3;
                    hr_data.frame_count = ppg_mems_data_alg.ppg_frame_count;
                    hr_data.time = ppg_mems_data_alg.duration;
                    hr_data.rsv = 0;
                    hr_data.touch = (ppg_mems_data_alg.touch_flag > 0 ? 0x80 : 0x00);
                    hr_data.rsv2 = 0;
          
                    ret = PxiAlg_Process_Expo((uint8_t*)&hr_data, ppg_mems_data_alg.MEMS_Data, ppg_mems_data_alg.ppg_data.exposure_time[0]);

                    if(PxiAlg_GetHrUpdateFlag())
                    {
                        PxiAlg_HrGet(&myHR);
                        PxiAlg_GetSigGrade(&hr_grader);
                    }
          
                    spo2_raw_1 = ppg_mems_data_alg.ppg_data.data[0];
                    spo2_raw_2 = ppg_mems_data_alg.ppg_data.data[1];
                
                    RAW_DATA[0]=spo2_raw_1;
                    RAW_DATA[1]=spo2_raw_2;
          
                    mems_data_raw[0].data[0]=ppg_mems_data_alg.MEMS_Data[0];
                    mems_data_raw[0].data[1]=ppg_mems_data_alg.MEMS_Data[1];
                    mems_data_raw[0].data[2]=ppg_mems_data_alg.MEMS_Data[2];
                    
                    // SPO2 fill data on pah8series structure
                    pxialg_data_spo2.touch_flag         =  ppg_mems_data_alg.touch_flag ;
                    pxialg_data_spo2.time               =  ppg_mems_data_alg.duration ;;
                    pxialg_data_spo2.ppg_data           =  (int32_t*)RAW_DATA;
                    pxialg_data_spo2.nf_ppg_channel     =  2;
                    pxialg_data_spo2.nf_ppg_per_channel =  1;
                    pxialg_data_spo2.frame_count        =  ppg_mems_data_alg.ppg_frame_count ;
                    pxialg_data_spo2.nf_mems            =  1;
                    pxialg_data_spo2.mems_data          = (int16_t*)mems_data_raw;
           
                    //DEBUG_PRINT("data_buf_count_spo2_alg = %d \r\n", data_buf_count_spo2_alg);
                    
                    SpO2_SetHrValue(myHR);
                    SpO2_SetMems(mems_data_raw, 1);
                    time_bbb=pah_get_tick_count();
                    ret_value = SpO2_Process(ppg_mems_data_alg.touch_flag , 2, pxialg_data_spo2.ppg_data, 1);
                    time_eee=pah_get_tick_count();
                    //LOG_PRINT("SPO2_Run_Time  %d  \n",time_eee-time_bbb);
                   
                    SpO2_GetSpO2(&mySpO2);
                    SpO2_GetReadyFlag(&mySPO2_ReadyFlag);
                    SpO2_GetInfo(&_spo2_detect_item);
                    correlation =_spo2_detect_item.Noise;
                    trust_flag  =_spo2_detect_item.Output[3];
                    MEMS_RMS    =_spo2_detect_item.RMS;
          
                    data_buf_count_spo2_alg++;
                
                    //int ret=1;
                    if( (ret_value == 0  ) && (data_buf_count_spo2_alg>=25) )
                    {   
                        report_spo2_data(&ppg_mems_data_alg, ret, mySpO2, myHR, correlation, trust_flag, MEMS_RMS,data_buf_count_spo2_alg);
                        data_buf_count_spo2_alg = data_buf_count_spo2_alg -25;
                    }
                    else
                    {
                        report_spo2_data(&ppg_mems_data_alg, ret, mySpO2,myHR,  correlation, trust_flag, MEMS_RMS,data_buf_count_spo2_alg);
                  
                        switch (ret_value)
                        {
                            case SPO2_MSG_ALG_NOT_OPEN:
                                  DEBUG_PRINT("Algorithm is not initialized. \n");
                            break;
                           
                            case SPO2_MSG_ALG_REOPEN:
                                  DEBUG_PRINT("Algorithm is re initialized. \n");
                            break;
                           
                            case SPO2_MSG_ALG_NOT_SUPPORT:
                                  DEBUG_PRINT("Algorithm is not support. \n");
                            break;
                          
                            default:
                              //DEBUG_PRINT("Algorithm unhandle error = %d \n", ret_value);
                            break;
                        }
                        //return 1 ;
                    }
                }
                else
                {
                    report_ppg_data(&ppg_mems_data_alg);
                }
            }
        }
    }
#endif
}



// Stress
void stress_algorithm_enable(void)
{
    #if 0
    int buffer_size = 0;
    if (_main.stress_is_enable)
    {
    return;
    }

    LOG_PRINT("HRVGetVersion() = %d \n", HRVGetVersion());
    buffer_size = HRVGetMemorySize(HRV_ENABLE_LHR, HRV_COLLECTION_SECONDS, HRV_SAMPLING_RATE);
    LOG_PRINT("HRVGetMemorySize(), open_size = %d \n", buffer_size);

    if (!buffer_size)
    {
        LOG_PRINT("HRVGetMemorySize(), failed to get memory size \n");
        return ;
    }

    stress_pBuffer = (uint8_t*)SCI_ALLOC_APPZ(buffer_size);
    if (!stress_pBuffer)
    {
        LOG_PRINT("Out of memory \n");
        return ;
    }
#if 0
    if (!HRVOpen(stress_pBuffer))
    {
        LOG_PRINT("HRVOpen() failed. \n");
        return ;
    }
#endif
    duration_total=0;
 
    _main.stress_is_enable = true;
    #endif
}

void stress_algorithm_disable(void)
{
    #if 0
    if (!_main.stress_is_enable)
        return;
    HRVClose();
    SCI_FREE(stress_pBuffer);
    
    duration_total            = 0;
    SDNN_ReadyFlag            = 0;
    Get_Stress                = 0;
    Stress                    = 0.0f;
    HRAvg                     = 0;
    SQ_Stress                 = 0.0f;
    AnomalyFlag               = 0;
    Stress_ReadyFlag          = 0;
    Weak_Flag                 = 0;                
    _main.stress_is_enable    = false;
    #endif
}

void stress_alg_task(int grade)
{
#if 0
    if(Check_alg_enable())
    {
        if(Check_alg_enable()==MSG_ALG_ENABLE)
        {
            stress_algorithm_enable();
        }else if(Check_alg_enable()==MSG_ALG_DISABLE)
        {
            stress_algorithm_disable();
        }
        Clear_alg_enable();
    }

    if(!isFIFOEmpty())
    {
        if(pah_pop(&ppg_mems_data_alg))    //Get data from FIFO
        {
            if(_main.stress_is_enable)
            {
                uint32_t raw_data = ppg_mems_data_alg.ppg_data.data[1];
                uint8_t timeInterval = ppg_mems_data_alg.duration;
                uint8_t touch_flag = ppg_mems_data_alg.touch_flag;

                // If using Interrupt mode call this API
                HRVSetDiffTime(timeInterval,1);
  
                HRVProcess(touch_flag, raw_data);

                duration_total+=timeInterval;
              
              
                if(Stress_ReadyFlag==0)
                {
                    Stress_ReadyFlag=HRVGetReadyFlag();
                }
            
                if((duration_total>25000)&& Stress_ReadyFlag==0)
                {
                    LOG_PRINT("PPG Raw data is too Weak\n");
                    LOG_PRINT("TEST Stop\n");
                    stress_algorithm_disable();
                    Weak_Flag=1;
                    return  ;
                }    
                
                if(Stress_ReadyFlag  == 1 || HRVGetReadyFlag())
                {
                    Stress_ReadyFlag=1;
                    _hrv_state.collect_duration += timeInterval;
  
                    if(_hrv_state.collect_duration > (HRV_COLLECTION_SECONDS * 1000))
                    {
                        //int grade=100;
                        HRAvg = HRVGetHRAvg();  
                        Stress= HRVGetStress(grade);                                        
                        SQ_Stress=HRVGetRRIQuality();                                        
                        AnomalyFlag= HRVGetAnomalyFlag();
                        LOG_PRINT("HRAvg = %d  Stress = %f  HRV_Quality = %f  AnomalyFlag= %d \n", HRAvg ,Stress,SQ_Stress,AnomalyFlag);

                        if(SQ_Stress>0.4f)
                        {
                            LOG_PRINT("HRV_Quality Good\n");
                        }
                        else
                        {
                            LOG_PRINT("HRV_Quality Weak\n");
                        }
                        
                        
                        if(Stress >=80 &&  Stress<=100)
                        {
                            LOG_PRINT("Stress_Level High\n");
                        }
                        else if(Stress >=60 &&  Stress<80)
                        {
                            LOG_PRINT("Stress_Level Medium\n");
                        }
                        else if(Stress >=30 &&  Stress<60)
                        {
                            LOG_PRINT("Stress_Level Normal\n");
                        }
                        else
                        {
                            LOG_PRINT("Stress_Level Relaxed\n");
                        }                                    
        
                        stress_algorithm_disable();
                        _hrv_state.has_got_sdnn = 1;
                        Get_Stress=1;
                        return ;
                    }
                }    
                report_stress_data(&ppg_mems_data_alg,Stress_ReadyFlag, duration_total, _hrv_state.collect_duration );
            }
            else
            {
                report_ppg_data(&ppg_mems_data_alg);
                LOG_PRINT("Stress_Level Normal\n");
            }
        }
    }
#endif
}




//============================================================================
// Private function
//============================================================================
static bool sensor_basic_verification(void)
{
    uint16_t product_id = 0;
    pah_800x_get_product_id(&product_id);
    if (product_id != PAH_800x_PRODUCT_ID)
    {
        pah_800x_power_toggle(&_main.pah_800x, true);
        pah_800x_get_product_id( &product_id);
    }

    DEBUG_PRINT("product_id = 0x%04X\n", product_id);
    if (product_id != PAH_800x_PRODUCT_ID)
    {
        DEBUG_PRINT("Failed to verify product_id\n");
        return false;
    }
	
#ifdef BURST_READ_VERIFY
	/* verify pah8009 i2c burst read operation */
	pah_800x_i2c_burst_read_verify();
#endif
    
    return true;
}

static void process(uint64_t timestamp)
{
    float mems_data[3] = { 0.0f, 0.0f, 0.0f };
    int8_t touch_flag = -1;
    // Task
    pah_ret ret = pah_800x_task(&_main.pah_800x);
    if (PAH_FAILED(ret))
    {
        DEBUG_PRINT("pah_800x_task() failed, ret = %d\n", ret);
        return;
    }

    pah_800x_get_touch_flag(&_main.pah_800x, &touch_flag);
    
    if (_main.ppg_is_enabled)
    {
        ppg_mems_data_t ppg_mems_data;
        pah_800x_ppg_data ppg_data;
        pah_800x_ppg_get_data(&_main.pah_800x, &ppg_data);

        if (ppg_data.has_data)
        {
            //LOG_PRINT("ppg data %d %d %d %d %d %d %d %d\n",ppg_data.has_data,ppg_data.overflow_num,ppg_data.data[0],ppg_data.data[1],ppg_data.data[2],
            //ppg_data.exposure_time[0],ppg_data.exposure_time[1],ppg_data.exposure_time[2]);
            //LOG_PRINT("ppg data %d %d %d %d %d %d\n",ppg_data.frame_period[0],ppg_data.frame_period[1],ppg_data.frame_period[2]
            //,ppg_data.adc[0],ppg_data.adc[1],ppg_data.adc[2]);
            uint64_t duration = timestamp - _main.ppg_prev_report_timestamp;
            if (!_main.ppg_has_prev_report_timestamp)
            {
                _main.ppg_has_prev_report_timestamp = true;
                duration = 50;
            }
            _main.ppg_prev_report_timestamp = timestamp;
            
            if (duration > UINT8_MAX)
            {
                DEBUG_PRINT("duration > UINT8_MAX !!!\n");
                return;
            }

            /***********************G sensor explanation start***********************************/
            //If no G sensor, please set ppg_mems_data.MEMS_Data[3] = {0};
            //Please set G sensor default range = ±2G.
            //If G sensor output data format is not 16bit, please change to 16bit format.
            //For example: G sensor is 12bit
            //ReadGSensor(&mems_data[0],&mems_data[1],&mems_data[2])
            //MEMS_SensorX() absolute data must be largest when let hand down at static state.
            //MEMS_SensorZ() absolute data must be largest when let hand horizontal on the table at static state.
            /***********************G sensor explanation end***********************************/
                
            ppg_mems_data.MEMS_Data[0] = mems_data[0];//ReadGSensorX();
            ppg_mems_data.MEMS_Data[1] = mems_data[1];//ReadGSensorY();
            ppg_mems_data.MEMS_Data[2] = mems_data[2];//ReadGSensorZ();
            ppg_mems_data.duration = duration;
            
            ppg_mems_data.ppg_data = ppg_data ;
            ppg_mems_data.touch_flag = touch_flag ;
            ppg_mems_data.ppg_frame_count = _main.ppg_frame_count;    //SD-revise-20200320
                                        
            pah_push(&ppg_mems_data);

#ifdef PPG_MEMS_Synchronize          
            time_temp+=duration;
            mems_data_polling_ready=0;
            if(time_temp>=Time_Interval)
            {
              mems_data_polling_ready=1;
              time_temp =0;
            }
            ppg_data_num++;
#endif            
            ++_main.ppg_frame_count;
            
            //report_ppg_data(&ppg_mems_data);

        }
    }
    
    if (_main.is_Auto_mode || _main.touch_is_enabled)//SD-reivse-20191205
    {
        if ( _main.touch_last_is_touched != touch_flag)
        {
            bool is_touched;
            _main.touch_last_is_touched = touch_flag;   
                
            report_touch_data(timestamp, &touch_flag);
                
            is_touched = (touch_flag >= 1);
            if (_main.is_Auto_mode && is_touched)//SD-revise-20191205
            {             
                //touch_sensor_disable();
                ppg_sensor_enable();
                _main.alg_is_enabled = MSG_ALG_ENABLE;
                
            }
            else if (_main.is_Auto_mode && !is_touched)
            {
                ppg_sensor_disable();  
                pah_800x_update_touch_setting(&_main.pah_800x);
                //touch_sensor_enable();
                _main.alg_is_enabled = MSG_ALG_DISABLE;
                
            }
        }
    }
}

static void ppg_sensor_enable(void)
{
    if (_main.ppg_is_enabled)
        return;
    
    pah_800x_ppg_toggle(&_main.pah_800x, true);

    _main.debug_print_header = true;
    _main.ppg_has_prev_report_timestamp = false;
    _main.ppg_prev_report_timestamp = pah_get_tick_count();
    _main.ppg_frame_count = 0;
    
    _main.ppg_is_enabled = true;
    
    _read_index = 0 ;
    _write_index = 0 ;
#ifdef PPG_MEMS_Synchronize
    ppg_data_num = 0 ;
#endif    
    //LOG_PRINT("ppg_sensor_enable\n");
}

static void ppg_sensor_disable(void)
{
    if (!_main.ppg_is_enabled)
        return;
    
    pah_800x_ppg_toggle(&_main.pah_800x, false);

    _main.ppg_is_enabled = false;
}

static void touch_sensor_enable(void)
{
    if (_main.touch_is_enabled)
        return;
    
    pah_800x_touch_toggle(&_main.pah_800x, true);

    _main.touch_last_is_touched = 0;//default no touch => touch mode
    
    _main.touch_is_enabled = true;
}

static void touch_sensor_disable(void)
{
    if (!_main.touch_is_enabled)
        return;
    
    pah_800x_touch_toggle(&_main.pah_800x, false);

    _main.touch_is_enabled = false;
}

static void report_touch_data(uint64_t timestamp, const int8_t *touch_flag)
{
    LOG_PRINT("TOUCH: time = { %llu }, touch_flag = { %d } \n", timestamp, *touch_flag );
}

static void report_ppg_data(const ppg_mems_data_t *ppg_mems_data_alg)
{
    if (_main.debug_print_header == true)
    {
        LOG_PRINT("Frame, Time, Touch_Flag, Data_Ready, Overflow_Flag, Overflow_Num, ppg0, ppg1, ppg2, Expo_A, Expo_B, Expo_C, MEMS_X, MEMS_Y, MEMS_Z\n");
        _main.debug_print_header = false;
    }

    LOG_PRINT("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
                ppg_mems_data_alg->ppg_frame_count, ppg_mems_data_alg->duration, ppg_mems_data_alg->touch_flag,
                ppg_mems_data_alg->ppg_data.has_data, ppg_mems_data_alg->ppg_data.has_overflow, ppg_mems_data_alg->ppg_data.overflow_num,
                ppg_mems_data_alg->ppg_data.data[0], ppg_mems_data_alg->ppg_data.data[1], ppg_mems_data_alg->ppg_data.data[2],
                ppg_mems_data_alg->ppg_data.exposure_time[0], ppg_mems_data_alg->ppg_data.exposure_time[1], ppg_mems_data_alg->ppg_data.exposure_time[2],
                (int)(ppg_mems_data_alg->MEMS_Data[0]), (int)(ppg_mems_data_alg->MEMS_Data[1]), (int)(ppg_mems_data_alg->MEMS_Data[2]));
}

static void report_hr_data(const ppg_mems_data_t *ppg_mems_data_alg, int32_t ret, float hr, float hr_grader)
{
    pah8009_hr_data = hr;
    if (_main.debug_print_header == true)
    {
        LOG_PRINT("Frame, Time, Touch_Flag, Data_Ready, Overflow_Flag, Overflow_Num, PPG0, PPG1, PPG2, Expo_A, Expo_B, Expo_C, MEMS_X, MEMS_Y, MEMS_Z, ret, hr, hr_grade\n");

        _main.debug_print_header = false;
    }
  
    LOG_PRINT("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
                ppg_mems_data_alg->ppg_frame_count, ppg_mems_data_alg->duration, ppg_mems_data_alg->touch_flag,
                ppg_mems_data_alg->ppg_data.has_data, ppg_mems_data_alg->ppg_data.has_overflow, ppg_mems_data_alg->ppg_data.overflow_num,
                ppg_mems_data_alg->ppg_data.data[0], ppg_mems_data_alg->ppg_data.data[1], ppg_mems_data_alg->ppg_data.data[2],
                ppg_mems_data_alg->ppg_data.exposure_time[0], ppg_mems_data_alg->ppg_data.exposure_time[1], ppg_mems_data_alg->ppg_data.exposure_time[2],
                (int)(ppg_mems_data_alg->MEMS_Data[0]), (int)(ppg_mems_data_alg->MEMS_Data[1]), (int)(ppg_mems_data_alg->MEMS_Data[2]), ret);

}



static void report_spo2_data(const ppg_mems_data_t *ppg_mems_data_alg, int32_t ret, float mySpO2,  float myHR, float correlation, float trust_flag, float MEMS_RMS, int data_buf_count_spo2_alg)
{
    pah8009_spo2_data = mySpO2;
    pah8009_hr_data = myHR;
    if (_main.debug_print_header == true)
    {
        
         LOG_PRINT("Frame, Time, Touch_Flag, Data_Ready, Overflow_Flag, Overflow_Num, PPG0, PPG1, PPG2, Expo_A, Expo_B, Expo_C, MEMS_X, MEMS_Y, MEMS_Z, ret, spo2, myHR, hr_grader, correlation, MEMS_RMS, trust \n");       
        _main.debug_print_header = false;
    }

  if(data_buf_count_spo2_alg>=25)
    {
                LOG_PRINT("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
                ppg_mems_data_alg->ppg_frame_count, ppg_mems_data_alg->duration, ppg_mems_data_alg->touch_flag,
                ppg_mems_data_alg->ppg_data.has_data, ppg_mems_data_alg->ppg_data.has_overflow, ppg_mems_data_alg->ppg_data.overflow_num,
                ppg_mems_data_alg->ppg_data.data[0], ppg_mems_data_alg->ppg_data.data[1], ppg_mems_data_alg->ppg_data.data[2],
                ppg_mems_data_alg->ppg_data.exposure_time[0], ppg_mems_data_alg->ppg_data.exposure_time[1], ppg_mems_data_alg->ppg_data.exposure_time[2],
                (int)(ppg_mems_data_alg->MEMS_Data[0]), (int)(ppg_mems_data_alg->MEMS_Data[1]), (int)(ppg_mems_data_alg->MEMS_Data[2]), ret);
    }
    else
    {
                LOG_PRINT("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
                ppg_mems_data_alg->ppg_frame_count, ppg_mems_data_alg->duration, ppg_mems_data_alg->touch_flag,
                ppg_mems_data_alg->ppg_data.has_data, ppg_mems_data_alg->ppg_data.has_overflow, ppg_mems_data_alg->ppg_data.overflow_num,
                ppg_mems_data_alg->ppg_data.data[0], ppg_mems_data_alg->ppg_data.data[1], ppg_mems_data_alg->ppg_data.data[2],
                ppg_mems_data_alg->ppg_data.exposure_time[0], ppg_mems_data_alg->ppg_data.exposure_time[1], ppg_mems_data_alg->ppg_data.exposure_time[2],
                (int)(ppg_mems_data_alg->MEMS_Data[0]), (int)(ppg_mems_data_alg->MEMS_Data[1]), (int)(ppg_mems_data_alg->MEMS_Data[2]), ret);

        
    }
}

static void report_stress_data(const ppg_mems_data_t *ppg_mems_data_alg, uint8_t Stress_ReadyFlag, uint32_t measuretime, float collect_time)
{
#if 0
    if (_main.debug_print_header == true)
    {
        LOG_PRINT("Frame, Time, Touch_Flag, Data_Ready, Overflow_Flag, Overflow_Num, PPG0, PPG1, PPG2, Expo_A, Expo_B, Expo_C, MEMS_X, MEMS_Y, MEMS_Z, Stress_Ready, measure_time, collect_time\n");

        _main.debug_print_header = false;
    }
  
    LOG_PRINT("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %f\n",
                ppg_mems_data_alg->ppg_frame_count, ppg_mems_data_alg->duration, ppg_mems_data_alg->touch_flag,
                ppg_mems_data_alg->ppg_data.has_data, ppg_mems_data_alg->ppg_data.has_overflow, ppg_mems_data_alg->ppg_data.overflow_num,
                ppg_mems_data_alg->ppg_data.data[0], ppg_mems_data_alg->ppg_data.data[1], ppg_mems_data_alg->ppg_data.data[2],
                ppg_mems_data_alg->ppg_data.exposure_time[0], ppg_mems_data_alg->ppg_data.exposure_time[1], ppg_mems_data_alg->ppg_data.exposure_time[2],
                (int)(ppg_mems_data_alg->MEMS_Data[0]), (int)(ppg_mems_data_alg->MEMS_Data[1]), (int)(ppg_mems_data_alg->MEMS_Data[2]), Stress_ReadyFlag,duration_total, _hrv_state.collect_duration  );
#endif
}

#ifdef PPG_MEMS_Synchronize 
uint8_t  get_ppg_data_num(void)
{
    return ppg_data_num;
}
void  clear_ppg_data_num(void)
{
    ppg_data_num = 0;
}

void mems_interpolation(uint8_t ppg_num)
{
  
    // Customer Define MEMS-input X, Y, Z
    // Put data in mems_int_data_in ,and mems_count
    // Then call the PXI_MEMS_interp to do interpolation 
    
    PXI_MEMS_interp(mems_int_data_in, mems_count, mems_int_data_out, ppg_num);
  
}
#endif

static void error_handle(void)
{
    while (true)
    {
    }
}
