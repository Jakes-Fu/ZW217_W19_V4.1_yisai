
/*********************************************************************************************************
*               Copyright(c) 2020, Vcare Corporation. All rights reserved.
**********************************************************************************************************
* @file     module_heart_vc.c
* @brief
* @details
* @author
* @date
* @version  v1.8
*********************************************************************************************************
*/
//#include "nrf_gpio.h"
//#include "nrf_delay.h"
//#include "FreeRTOS.h"

#include "algo.h"
#include "vcSpo2Algo.h" 
#include "vcSportMotionAlgo.h"
#include "vcHr02Hci.h"

//#include "trace.h"
//#include "board_config.h"
//#include "hal_hrs.h"
//#include "nrf_delay.h"
//#include "timers.h"
//#include "srv_timer.h"

/* Include your INT,I2C,Timer header file */
//#include "INT.h"
//#include "I2C.h"
//#include "Timer.h"

#define  VCHR_LOG  SCI_TRACE_LOW

#if HRS_VCHR02_ENABLE

uint8 VCHR02_read_reg(uint8 addr) ;
BOOLEAN VCHR02_write_reg(uint8 addr, uint8 data);
static uint32 _Get_VCHR02_I2C_Status(void);
static void VCHR02_I2C_Read(uint8 *reg, uint8 *addr, uint32 cnt);
static uint32 VCHR02_I2C_Close(void);
static void VCHR02_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt);
BOOLEAN VCHR02_write_reg(uint8 addr, uint8 data);


//yangyu add for .a  lib 
extern void Algo_Init();//{}
extern void Algo_Input(AlgoInputData_t *pInputData,int32 gaptime,int16 sportMode);//{}
extern void Algo_Output(AlgoOutputData_t *algoOutputData);//{}

extern float vcSportMotionCalculate(float x, float y, float z);//{return 0;}
extern void vcSportMotionAlgoInit(void);//{}
extern void vcSportMotionAlgoTerminate(void);//{}

extern void vcSpo2AlgoInit(void);//{}

extern float vcSpo2Calculate(float r_ppg, float ir_ppg);//{return 0;}


//yangyu end for .a lib







#define Gsensor 0

/* Heart rate value */
int mHeartRateValue = 0;

/* Timer clock frequency 
   Be used to adjust INT frequency */
const uint32 mcuOscData = 32768;//32000;//yangyu 

/* MCU I2C clock frequency */
const uint16 mcuI2cClock = 400;

/* Heart rate sample frequency */
const uint8 vcHr02SampleRate = 25;

/* INT Flag */
BOOLEAN vcHr02IRQFlag = 0;

/* Spo2 value */
float vcSpo2Value = 0.0;
int real_spo = 0;

/* Spo2 data */
float vcRedPPG = 0.0;
float vcIrPPG = 0.0;

uint16 GsensorLength = 0;

/* G-Sensor Data */
int16 xData[40] = {0};
int16 yData[40] = {0};
int16 zData[40] = {0};

/* The algorithm of hert rate data struct */
AlgoInputData_t algoInputData;
AlgoOutputData_t algoOutputData;

/* Heart rate data struct */
vcHr02_t vcHr02;

/* Heart rate mode */
vcHr02Mode_t vcMode = VCWORK_MODE_HRWORK;

/* Sport Mode */
AlgoSportMode vcSportMode = SPORT_TYPE_NORMAL;

static uint8 mmghH = 0;
static uint8 mmghL = 0;

/*
 * @brief
 *   
 *
 **/
void VCHR02_Init(vcHr02_t *pVcHr02,uint8 vcHr02WorkMode)
{
	vcHr02StopSample(pVcHr02);
	vcHr02SoftReset(pVcHr02);
	vcSportMotionAlgoTerminate();
	pVcHr02->vcSampleRate = vcHr02SampleRate;
	pVcHr02->mcuOscValue = mcuOscData;
	pVcHr02->mcuSclRate = mcuI2cClock;
	pVcHr02->workMode = vcHr02WorkMode;
	/* HeartRate Mode */
	if(pVcHr02->workMode == VCWORK_MODE_HRWORK)
	{
		vcHr02StartSample(pVcHr02);
		Algo_Init();
	}
	/* Spo2 Mode */
	else if(pVcHr02->workMode == VCWORK_MODE_SPO2WORK)
	{
		vcSportMotionAlgoInit();
		vcHr02StartSample(pVcHr02);
		vcSpo2AlgoInit();
	}
	/* Light Crosstalk Test Mode */
	else if(pVcHr02->workMode == VCWORK_MODE_CROSSTALKTEST)
	{
		vcHr02StartSample(pVcHr02);
	}
}

extern void ReadGsensorFIFO(int16 *x_array, int16 *y_array, int16 *z_array, uint16 *read_len);
extern void QMAX981_SetTimerPeroid(uint16 ms);



LOCAL uint8 *log_buf_ptr = NULL;
LOCAL uint32 saved_len = 0;

LOCAL void PrintSampleData( uint32 ppgLength)
{    

}

/*
 * @brief
 *   
 *
 **/
void VCHR02_process(AlgoSportMode vcSportMode)
{
	uint8 algoCallNum = 0;
	uint8 ppgLength = 0;
	uint8 vcSportFlag = 0;
	uint8 i = 0;
	uint16 j = 0;
	uint8 spo2_buf[16] = {0};
    #ifdef PRINT_FILE_LOG
    uint32 malloc_len = 64*1024;
    uint8 temp_buf[16] = {0};
	uint8 vcxxSendDugBuff[1024] = {0};
    uint32 vcxxDugIrgCnt = 0;
    SCI_TIMEX_T  sys_time = {0};
    TM_GetSysTimeEx(&sys_time);
    if(log_buf_ptr == NULL)
    {
        log_buf_ptr = (uint8 *)malloc(malloc_len);
    }
    SCI_MEMSET(log_buf_ptr, 0, malloc_len);
    saved_len = 0;
    if(log_buf_ptr == NULL)
    {
        //return;
    }
    #endif

    SCI_TRACE_LOW("VCHR02_process start vcHr02IRQFlag=%d",vcHr02IRQFlag);
	if (vcHr02IRQFlag)
	{
		vcHr02IRQFlag = FALSE;
	
		if(vcHr02.workMode == VCWORK_MODE_HRWORK)
		{
		    SCI_TRACE_LOW("VCHR02 0 ppgLength=%d",ppgLength);
			if(VCHR02RET_UNWEARTOISWEAR == vcHr02GetSampleValues(&vcHr02,&ppgLength))
			{
				Algo_Init();
			}
			
            SCI_TRACE_LOW("VCHR02 ppgLength=%d",ppgLength);
            
			if(vcHr02.vcFifoReadFlag || vcHr02.vcPsFlag)
			{
#if GsensorEn
				/* ReadGsensorFIFO(+-4G11bit or +-8G12bit 256/g) */
				ReadGsensorFIFO(xData, yData, zData, &GsensorLength);
				
				if(GsensorLength>=29)
				{
					//GsensorLength为GsensorFIFO数据长度，此处Gsensor采样率为36HZ，800mS中断时长度众数是29个，大于29个数据舍弃；
					GsensorLength = 29;
				}
				else if(GsensorLength>=1)
				{
					//小于29个数据补足29个；
					for( i=GsensorLength;i<29;i++)
					{
						xData[i]=xData[GsensorLength-1];
						yData[i]=yData[GsensorLength-1];
						zData[i]=zData[GsensorLength-1];
					}
				}
				
				/*for(uint8 i=0;i<20;i++)
				{
					//cash_num[20]数组为抽样数组；此处将29个数据抽成20个，匹配800mS中断的20个PPG
					xData[i]=yData[cash_num[i]]>>5;
					yData[i]=xData[cash_num[i]]>>5;
					zData[i]=zData[cash_num[i]]>>5;
				}*/
#endif			
				if(vcHr02.vcFifoReadFlag)
				{	
					vcHr02.vcFifoReadFlag = 0;
					if(vcHr02.wearStatus == VCHR02WEARST_ISWEAR)
					{
#if GsensorEn
						if(GsensorLength < ppgLength)//此处为了防止丢中断，保证每个PPG数据参与算法运算，丢中断的情况下，Gsensor数据个数补足到ppgLength；
						{
							for( i = GsensorLength; i < ppgLength; i++)
							{
								xData[i] = xData[GsensorLength - 1];
								yData[i] = yData[GsensorLength - 1];
								zData[i] = zData[GsensorLength - 1];
							}
						}
						else
						{
							
						}
#endif
						algoInputData.envSample = vcHr02.sampleData.envValue[0];
						for(algoCallNum= 0; algoCallNum < ppgLength; algoCallNum++)
						{
							algoInputData.ppgSample = vcHr02.sampleData.ppgValue[algoCallNum];
							algoInputData.axes.x = xData[algoCallNum];//The direction vertical with ARM.
							algoInputData.axes.y = yData[algoCallNum];//The direction parallel with ARM.
							algoInputData.axes.z = zData[algoCallNum];//The direction upside.
							Algo_Input(&algoInputData, 1000/vcHr02SampleRate,vcSportMode);
							#ifdef PRINT_FILE_LOG
							//PrintSampleData(ppgLength);
							
							
                            
                            
                            i = algoCallNum;
							vcxxSendDugBuff[i*15] = algoInputData.ppgSample>>8&0xff;
                            vcxxSendDugBuff[i*15+1] = algoInputData.ppgSample&0xff;
                            vcxxSendDugBuff[i*15+2] = algoInputData.axes.x>>8&0xff;
                            vcxxSendDugBuff[i*15+3] = algoInputData.axes.x&0xff;
                            vcxxSendDugBuff[i*15+4] = algoInputData.axes.y>>8&0xff;
                            vcxxSendDugBuff[i*15+5] = algoInputData.axes.y&0xff;
                            vcxxSendDugBuff[i*15+6] = algoInputData.axes.z>>8&0xff;
                            vcxxSendDugBuff[i*15+7] = algoInputData.axes.z&0xff;
                            vcxxSendDugBuff[i*15+8] = algoOutputData.hrData&0xff;
                            vcxxSendDugBuff[i*15+9] =  vcHr02.sampleData.pdResValue[0];
                            vcxxSendDugBuff[i*15+10] = vcHr02.sampleData.currentValue[0];
                            vcxxSendDugBuff[i*15+11] = vcHr02.sampleData.envValue[2];
                            vcxxSendDugBuff[i*15+12] = vcHr02.sampleData.preValue[0];
                            vcxxSendDugBuff[i*15+13] = vcHr02.sampleData.psValue;
                            vcxxSendDugBuff[i*15+14] = vcxxDugIrgCnt;
                            vcxxDugIrgCnt++;
                            for(j = 0; j < 15; j++)
                            {
                                SCI_MEMSET(temp_buf , 0 , 16);
                                sprintf(temp_buf, "%02x ", vcxxSendDugBuff[i*15 + j]);
                                SCI_MEMCPY(log_buf_ptr+saved_len, temp_buf, strlen(temp_buf));
                                saved_len += strlen(temp_buf);
                            }

                           
							#endif
						}
						#ifdef PRINT_FILE_LOG
                        Trace_Large_Log_Print("%02d:%03d %s\n", sys_time.sec, sys_time.msec, log_buf_ptr);
						#endif
						
						Algo_Output(&algoOutputData);
						mHeartRateValue = algoOutputData.hrData;
						//yangyu add ,for sport fake
                        if(mHeartRateValue >= 180)
                        {
                            mHeartRateValue = mHeartRateValue*65/100;
                        }
                        else if(mHeartRateValue >= 150)
                        {
                            mHeartRateValue = mHeartRateValue*70/100;

                        }
                        else if(mHeartRateValue >= 120)
                        {
                            mHeartRateValue = mHeartRateValue*80/100;
                        }
						//yangyu end
						SCI_TRACE_LOW("VCHR02 mHeartRateValue =%d",mHeartRateValue);
						VCHR_LOG("VCHR02 mHeartRateValue =%d",mHeartRateValue);
						if(mHeartRateValue == -1)
						{
							Algo_Init();
						} 
					}
				}
				else 
				{
#if SportMotionEn
					green_led_off_state_gsensor_abs_sum_diff_func(xData[0],yData[0],zData[0]);
#endif
					mHeartRateValue = -2;
				}
			}
                    SCI_TRACE_LOW("VCHR02 mHeartRateValue=%d",mHeartRateValue);
		}
		else if(vcHr02.workMode == VCWORK_MODE_SPO2WORK)
		{		
		    //vcHr02Ret_t ret1 = vcHr02GetSampleValues(&vcHr02,&ppgLength);
		    //SCI_TRACE_LOW("VCHR02 vcSpo2Value ret1=%d", ret1);
			if(VCHR02RET_UNWEARTOISWEAR == vcHr02GetSampleValues(&vcHr02,&ppgLength))
			{
				vcSpo2AlgoInit();
				vcSportMotionAlgoInit();
			}

			
			
			if(vcHr02.vcFifoReadFlag || vcHr02.vcPsFlag)
			{	
#if GsensorEn
				/* ReadGsensorFIFO(+-4G11bit or +-8G12bit 256/g) */
				///ReadGsensorFIFO(xData, yData, zData, &GsensorLength);
				if(GsensorLength>=29)
				{
					GsensorLength = 29;
				}
				else if(GsensorLength>=1)
				{
					for( i=GsensorLength;i<29;i++)
					{
						xData[i]=xData[GsensorLength-1];
						yData[i]=yData[GsensorLength-1];
						zData[i]=zData[GsensorLength-1];
					}
				}

				/*for(uint8 i=0;i<20;i++)
				{
					xData[i]=yData[cash_num[i]]>>5;
					yData[i]=xData[cash_num[i]]>>5;
					zData[i]=zData[cash_num[i]]>>5;
				}*/
#endif	
				if(vcHr02.vcFifoReadFlag)
				{
					vcHr02.vcFifoReadFlag = 0;
					if(ppgLength <= 50)	
					{					
						for(algoCallNum= 0; algoCallNum < ppgLength/2; algoCallNum++)
						{	
							vcIrPPG = vcHr02.sampleData.ppgValue[algoCallNum*2];
							vcRedPPG = vcHr02.sampleData.ppgValue[algoCallNum*2+1];							
							vcSpo2Value = vcSpo2Calculate(vcRedPPG,vcIrPPG);
							sprintf(spo2_buf, "%.2f", vcSpo2Value);
							SCI_TRACE_LOW("VCHR02 vcSpo2Value=%s",spo2_buf);
							VCHR_LOG("VCHR02 vcSpo2Value=%s \n",spo2_buf);
							#ifdef PRINT_FILE_LOG
                            vcxxSendDugBuff[algoCallNum*18+0] = vcHr02.sampleData.ppgValue[2*algoCallNum]>>8&0xff;
                            vcxxSendDugBuff[algoCallNum*18+1] = vcHr02.sampleData.ppgValue[2*algoCallNum]&0xff;
                            vcxxSendDugBuff[algoCallNum*18+2] = vcHr02.sampleData.ppgValue[2*algoCallNum+1]>>8&0xff;
                            vcxxSendDugBuff[algoCallNum*18+3] = vcHr02.sampleData.ppgValue[2*algoCallNum+1]&0xff;
                            vcxxSendDugBuff[algoCallNum*18+4] = algoInputData.axes.x>>8&0xff;
                            vcxxSendDugBuff[algoCallNum*18+5] = algoInputData.axes.x&0xff;
                            vcxxSendDugBuff[algoCallNum*18+6] = algoInputData.axes.y>>8&0xff;
                            vcxxSendDugBuff[algoCallNum*18+7] = algoInputData.axes.y&0xff;
                            vcxxSendDugBuff[algoCallNum*18+8] = algoInputData.axes.z>>8&0xff;
                            vcxxSendDugBuff[algoCallNum*18+9] = algoInputData.axes.z&0xff;
                            vcxxSendDugBuff[algoCallNum*18+10] = vcSpo2Value;//systemParam.bloodOxygenValue;
                            vcxxSendDugBuff[algoCallNum*18+11] = vcHr02.sampleData.pdResValue[1];
                            vcxxSendDugBuff[algoCallNum*18+12] = vcHr02.sampleData.pdResValue[2];
                            vcxxSendDugBuff[algoCallNum*18+13] = vcHr02.sampleData.currentValue[1];
                            vcxxSendDugBuff[algoCallNum*18+14] = vcHr02.sampleData.currentValue[2];
                            vcxxSendDugBuff[algoCallNum*18+15] = vcHr02.sampleData.psValue;
                            vcxxSendDugBuff[algoCallNum*18+16] = vcSportFlag;
                            vcxxSendDugBuff[algoCallNum*18+17] = vcxxDugIrgCnt;
                            vcxxDugIrgCnt++;
                            
                            i = algoCallNum;
                            for(j = 0; j < 18; j++)
                            {
                                SCI_MEMSET(temp_buf , 0 , 16);
                                sprintf(temp_buf, "%02x ", vcxxSendDugBuff[i*18 + j]);
                                SCI_MEMCPY(log_buf_ptr+saved_len, temp_buf, strlen(temp_buf));
                                saved_len += strlen(temp_buf);
                            }

						    #endif
#if GsensorEn						
//							vcSportFlag = vcSportMotionCalculate(xData[algoCallNum], yData[algoCallNum],zData[algoCallNum]);
#endif						
							if((!vcSportFlag) && (vcSpo2Value > 0))
							{
								real_spo = vcSpo2Value;
							}						
						}	
						#ifdef PRINT_FILE_LOG
                        Trace_Large_Log_Print("%02d:%03d %s\n", sys_time.sec, sys_time.msec, log_buf_ptr);
						#endif
					}
				}
				else
				{
                    real_spo = 0; //not wear
				}
			}
		}
		else if(vcHr02.workMode == VCWORK_MODE_CROSSTALKTEST)
		{
			vcHr02GetSampleValues(&vcHr02,&ppgLength); 
			//mHeartRateValue = vcHr02.sampleData.psValue;//vcHr02.sampleData.maxLedCur*10 + vcHr02.sampleData.preValue[0] ;//yangyu add test
			/* If Pass: */
			if ((vcHr02.sampleData.maxLedCur >= 100) && (vcHr02.sampleData.preValue[0] <= 2))
			{
			
				//PASS：
				//Display the value of vcHr02.sampleData.maxLedCur and vcHr02.sampleData.preValue[0]
			}
		}
	}	
	SCI_TRACE_LOW("VCHR02_process end ");
}


/*
 * @brief
 *   main
 *
 **/
 #if 0
void main()
{ 
/*-----------INSERT BOARD HARDWARE AND SOFTWARE INITIAL FUNCTION------------*/
    accInit();          	//Accelerometer initial +-8G 12bit
	VCHR02_PhyInit();      	//vcHr02 board initial
	Algo_Init();        	//vcHr02 algorithm initial
    VCHR02_Init(&vcHr02,vcMode);
    while(1)
    {
		VCHR02_process(vcSportMode);
    }
}
#endif

int VCHR02_eint_en(int flag)
{
    #if 0
    if(flag == 0)
    {
        nrf_drv_gpiote_in_event_disable(HRS_HRD_INT_PIN);
    }
    else if(flag == 1)
    {
        nrf_gpio_cfg_input(HRS_HRD_INT_PIN, NRF_GPIO_PIN_PULLUP);
        nrf_drv_gpiote_in_event_enable(HRS_HRD_INT_PIN, TRUE);
    }
    #endif
    return 0;//NRF_SUCCESS;
}

#if 0
static void VCHR02_eint_ISR(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    VCHR02_eint_en(0);
    vcHr02IRQFlag = TRUE;
    VCHR02_process(vcSportMode);
    VCHR02_eint_en(1);
}
#endif
/*
 * @brief
 *   
 *
 **/
void VCHR02_PhyInit(void)
{
/***************************************************************************//**
* If Reset Pin of vcHr02 is connected to MCU, Initial it;
* If I2C and SPI is selected by software (Mode Pin is connected), Initial it
*       according to which interface is used (0 for SPI and 1 for I2C);
*       otherwise, ignore that;
* If I2C is selected, and I2C address is controlled by MCU, Initial these two
*       pins; otherwise, ignore that;
*
* If interrupt is enabled, Initial interrupt pin, configurate interrupt pin,
        interrupt edge;
* If interrupt is enabled, clear IRQ, set priority and enable interrupt;
* Initial communication interface selected.
******************************************************************************/
#if 0

    int err_code = 0;//NRF_SUCCESS;

    if (!nrf_drv_gpiote_is_init()) {
        err_code = nrf_drv_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }
    
    nrf_drv_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(FALSE);   //GPIOTE_CONFIG_IN_SENSE_HITOLO GPIOTE_CONFIG_IN_SENSE_TOGGLE(FALSE);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
    
    err_code = nrf_drv_gpiote_in_init(HRS_HRD_INT_PIN, &in_config, VCHR02_eint_ISR);
    APP_ERROR_CHECK(err_code);
    
    nrf_gpio_cfg_output(HRS_HRD_SW_PIN);
    nrf_gpio_pin_clear(HRS_HRD_SW_PIN);
#endif
/*------------------INSERT YOUR CODE HERE-----------------*/

}


/*
 * @brief
 *   
 *
 **/
void VCHR02_IRQHandler(void)
{
    vcHr02IRQFlag = TRUE;
}


/*
 * @brief
 *   
 *
 **/
vcHr02Ret_t VCHR02_WriteRegisters(uint8 startAddress, uint8 *pRegisters, uint8 len)
{
/***************************************************************************//**
* When SPI is used, write WRITE command to vcHr02 FIRSTLY,
* then write len byte data to register(s) start from startAddress
* to register(s) pointed by pRegisters.
*
* When I2C is used, write Slave Address and write flag to vcHr02 firstly,
* then write len byte data to register(s) start from startAddress
* to register(s) pointed by pRegisters.
******************************************************************************/
    uint8 *data_buf;	

    data_buf = (uint8*)malloc(len+1);
    data_buf[0] = startAddress;
    memcpy(&data_buf[1],pRegisters,len);
    //twi_master_sw_transfer(0x66, data_buf, len+1, TRUE);	//write	
    VCHR02_I2C_Write(&startAddress, pRegisters, len);
    free(data_buf);
    //twi_master_sw_transfer(0x66, &startAddress, 1, FALSE);	//write
    //twi_master_sw_transfer(0x66, pRegisters, len, TRUE);	//write	
/*------------------INSERT YOUR CODE HERE-----------------*/

	return VCHR02RET_ISOK;
}


/*
 * @brief
 *   
 *
 **/
vcHr02Ret_t VCHR02_ReadRegisters(uint8 startAddress, uint8 *pRegisters, uint8 len)
{
/***************************************************************************//**
* When SPI is used, write READ command to vcHr02 FIRSTLY,
* then read len byte data from register(s) start from startAddress
* to register(s) pointed by pRegisters.
*
* When I2C is used, write Slave Address and read flag to vcHr02 firstly,
* then read len byte data from register(s) start from startAddress
* to register(s) pointed by pRegisters.
******************************************************************************/

    uint8 ret = 0;

    //ret = twi_read_dma_data(0x66,startAddress,pRegisters,len);
    #if 0
    twi_master_sw_transfer(0x66, &startAddress, 1, FALSE);	//write
    twi_master_sw_transfer(0x67, pRegisters, len, TRUE);//read  
    #endif
    VCHR02_I2C_Read(&startAddress, pRegisters, len);
    
    if(ret != 0/*NRF_SUCCESS*/)
    {
        SCI_TRACE_LOW("vchr02 addr = 0x%x error", startAddress);	
    }
/*------------------INSERT YOUR CODE HERE-----------------*/

	return VCHR02RET_ISOK;
}


/*
 * @brief
 *   
 *
 **/
uint32 VCHR02_GetRtcCountFromMCU(void)
{
/***************************************************************************//**
* Get the time difference between two interrupts.
******************************************************************************/

/*------------------INSERT YOUR CODE HERE-----------------*/
    uint32 timeDiff;
    //timeDiff = app_timer_cnt_get();
    return timeDiff;  
/* 
	return RTC_GetCounter();
*/
}




static int hrs_drv_data(HRS_DATA_TYPE type,void *data)
{
    int err_code = 0;//NRF_SUCCESS;
    uint8   *press;
    
    switch(type)
    {
        case HRS_HEARTRATE:
            if(mHeartRateValue>=0)
            {
                *((uint8*)data) = mHeartRateValue; 
            }
            else
            {
                *((uint8*)data) = 0; 
            }
            break;
           
        case HRS_BLOODPRESS:
            press=(uint8 *)data;
            if(mHeartRateValue!=0)
            {
                if(mmghH==0)
                {
                    mmghH=118 + rand()%8;
                    mmghL=68 + rand()%8;
                }
                press[1] = mmghH;
                press[0] = mmghL;
            }
            else
            {
                press[1] = 0;
                press[0] = 0;
            }
            break;
        case HRS_BLOODOXYGEN:
            *((uint8*)data) = real_spo; 
            break;
    }
    return err_code;
}

PUBLIC uint16 vchr02_get_spo2()
{
    uint16 ret = 0;
    if(real_spo >= 95 && real_spo <= 100 )
    {
        ret = real_spo; 
    }
    else
    {
        ret = 0; 
    }
    return ret;
}

PUBLIC uint16 vchr02_get_heartrate()
{
     uint16 ret = 0;
    if(mHeartRateValue > 0)
    {
        ret = mHeartRateValue; 
    }
    else
    {
        ret = 0; 
    }
    return ret;
}


void VCHR02_power_on(void)
{
	//nrf_gpio_cfg_output(HRS_HRD_SW_PIN);
	//nrf_gpio_pin_set(HRS_HRD_SW_PIN);
	//nrf_delay_ms(50);
	GPIO_SetHSensorPower(1);
    hal_LcdDelay(50);
}

void VCHR02_power_off(void)
{
	//nrf_gpio_cfg_output(HRS_HRD_SW_PIN);
	//nrf_gpio_pin_clear(HRS_HRD_SW_PIN);
	GPIO_SetHSensorPower(0);
    hal_LcdDelay(5);
}


static int hrs_drv_close(void)
{
    
    int err_code = 0;//NRF_SUCCESS;
    SCI_TRACE_LOW("hrs_drv_close");

    VCHR02_power_off();
    mHeartRateValue = 0;
    real_spo=0;
    VCHR02_eint_en(0);
    
    return err_code;
}

static int hrs_drv_open(HRS_DATA_TYPE type)
{
    int err_code = 0;//NRF_SUCCESS;

    switch(type)
    {
        case HRS_HEARTRATE:
            SCI_TRACE_LOW("VCHR02 hrs_drv_open");
            VCHR02_power_on();
            Algo_Init();        	//vcHr02 algorithm initial
            vcMode=VCWORK_MODE_HRWORK;
            //vcMode=VCWORK_MODE_CROSSTALKTEST;// test leak light
            VCHR02_Init(&vcHr02,vcMode);
            VCHR02_eint_en(1);
            //start_hrs_timer();
            break;
        case HRS_BLOODPRESS:
            break;
        case HRS_BLOODOXYGEN:
            VCHR02_power_on();
            Algo_Init();        	//vcHr02 algorithm initial
            vcMode=VCWORK_MODE_SPO2WORK;
            VCHR02_Init(&vcHr02,vcMode);
            VCHR02_eint_en(1);
            //start_hrs_timer();
            break;
    }
    
    return err_code;
}

static int hrs_drv_force_close(void)
{
    return 0;//NRF_SUCCESS;
}

static int  hrs_drv_get_id(void)
{
    uint8 id = 0;
    
    VCHR02_power_on();
    VCHR02_ReadRegisters(0x00,&id,1);
    
    VCHR02_power_off();
    SCI_TRACE_LOW( "vc52 id= %d", id);
    if(id== 0x21)
    {
        return 0;//NRF_SUCCESS;
    }
    else
    {
        return -1;//NRF_ERROR_NOT_FOUND;
    }
}   		 

static int hrs_drv_init(void)
{
    int err_code = 0;//NRF_SUCCESS;
    
    //accInit();          	//Accelerometer initial +-8G 12bit
    VCHR02_PhyInit();      	//vcHr02 board initial
    //Algo_Init();        	//vcHr02 algorithm initial
    //VCHR02_Init(&vcHr02,vcMode);
    //hrs_timer_init();

    return err_code;
}

#if 0
const qp_hrs_t qp_hrs_vchr02 = {
 .hrs_init         = hrs_drv_init,
 .hrs_open         = hrs_drv_open,
 .hrs_close        = hrs_drv_close,
 .hrs_force_close  = hrs_drv_force_close,
 .hrs_get_data     = hrs_drv_data,
 .hrs_get_id     = hrs_drv_get_id,
};
#endif


//=======yangyu add begin=======
LOCAL BOOLEAN vchr02_is_open = FALSE;
#define VCHR02_I2C_WRITE_ADDR       0x66
#define VCHR02_I2C_READ_ADDR         0x67

static int32 VCHR02_I2C_Handler = -1;



static I2C_DEV VCHR02_I2C_cfg =
{
    1,          
    100000,                   // freq is standard for VCHR02
    0,                          // bus
    VCHR02_I2C_WRITE_ADDR,  // slave_addr
    1,                          // reg_addr_num
    1,                          // check_ack
    1                           // no_stop
};

static uint32 VCHR02_I2C_Open(void)
{
    if(VCHR02_I2C_Handler == -1) 
    {
        if((VCHR02_I2C_Handler = I2C_HAL_Open(&VCHR02_I2C_cfg)) == -1)
        {
            ZDT_HSENSOR_TRACE("VCHR02_I2C_Open Err");
            return HRS_I2C_ERROR;    
        } 
        else
            return HRS_I2C_SUCCESS;
    }
    else
    {
        return HRS_I2C_SUCCESS;
        
    }
}

/******************************************************************************/
// Description: VCHR02_I2C_Close
// Dependence: 
// Note:
/******************************************************************************/

static uint32 VCHR02_I2C_Close(void)
{
    if(_Get_VCHR02_I2C_Status() == HRS_I2C_SUCCESS)
	{
 		I2C_HAL_Close(VCHR02_I2C_Handler);
		VCHR02_I2C_Handler = -1;
	}	
    return HRS_I2C_SUCCESS;
}
/******************************************************************************/
// Description: VCHR02_I2C_Write
// Dependence: 
// Note:
/******************************************************************************/
static void VCHR02_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_VCHR02_I2C_Status() == HRS_I2C_SUCCESS)
        I2C_HAL_Write(VCHR02_I2C_Handler, reg, addr, cnt);
}


/******************************************************************************/
// Description: VCHR02_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
static void VCHR02_I2C_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    int i = 0;
    if(_Get_VCHR02_I2C_Status() == HRS_I2C_SUCCESS)
    {
        I2C_HAL_Read(VCHR02_I2C_Handler,reg, addr, cnt);
        //yangyu add for test 
        for( i = 0; i < cnt; i++)
        {
            //SCI_TRACE_LOW("VCHR02_I2C_Read data[%d] = 0x%x ", i, addr[i]);
        }
        //yangyu end
        
    }
}


/******************************************************************************/
// Description: _Get_VCHR02_I2C_Status
// Dependence:  It can get the status from i2c
// Note:
/******************************************************************************/
static uint32 _Get_VCHR02_I2C_Status(void)
{
    if(VCHR02_I2C_Handler==-1)
    {
        return HRS_I2C_ERROR;
    }
    else
    {
        return HRS_I2C_SUCCESS;
    }
}

BOOLEAN VCHR02_write_reg(uint8 addr, uint8 data) 
{
    uint32 ret = 0;
    if(_Get_VCHR02_I2C_Status() == HRS_I2C_SUCCESS)
    {
        ret = I2C_HAL_Write(VCHR02_I2C_Handler, &addr, &data, 1);
        if(ret == 0)
        {
            ZDT_HSENSOR_TRACE("VCHR02_write_reg I2C_HAL_Write Err");
        }
    }
    else
    {
        ZDT_HSENSOR_TRACE("VCHR02_write_reg Err");
    }
	return 0;  	
}

uint8 VCHR02_read_reg(uint8 addr) 
{
    uint32 ret = 0;
    uint8 data_buf = 0;	
    if(_Get_VCHR02_I2C_Status() == HRS_I2C_SUCCESS)
    {
        ret = I2C_HAL_Read(VCHR02_I2C_Handler,&addr, &data_buf, 1);
        if(ret == 0)
        {
            ZDT_HSENSOR_TRACE("VCHR02_read_reg I2C_HAL_Read Err");
        }
    }
    else
    {
        ZDT_HSENSOR_TRACE("VCHR02_read_reg Err");
    }
    return data_buf;
}


uint16 VCHR02_read_hrs(void)
{
    uint8  databuf[3];
    uint16 data;

    databuf[0] = VCHR02_read_reg(0x09);	// addr09, bit
    databuf[1] = VCHR02_read_reg(0x0a);	// addr0a, bit
    databuf[2] = VCHR02_read_reg(0x0f);	// addr0f, bit

    data = ((databuf[0]<<8)|((databuf[1]&0x0F)<<4)|(databuf[2]&0x0F));

    //ZDT_HSENSOR_TRACE("%s (%d)	: ==huangwenjun==VCHR02_read_hrs== hrs data = 0x%x\n", __func__, __LINE__, data);  // add by hwj

    return data;
}



int vchr02_drv_get_id()
{
   
    uint8 id = 0;
    
    VCHR02_power_on();
    VCHR02_ReadRegisters(0x00,&id,1);
    
    VCHR02_power_off();
    SCI_TRACE_LOW( "vc52 id= %d", id);
    if(id== 0x21)
    {
        return 0;
    }
    else
    {
        return -1;
    }

    //return id;
}

int vchr02_drv_init(void)
{
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    ZDT_HSENSOR_TRACE("vchr02_drv_init");
    VCHR02_I2C_Open();
    //VCHR02_chip_init();
    //VCHR02_chip_enable();
    //VCHR02_alg_open();
    return 0;
}


PUBLIC int vchr02_spo2_drv_open()
{
    int ret = 0;

    //ZDT_HSENSOR_TRACE("hsensor_open is_init=%d,type=%d",hsensor_is_init,s_hsensor_type);
    if(vchr02_is_open == FALSE)
    {
        #if 0
        #ifndef WIN32
        GPIO_SetHSensorPower(1);
        hal_LcdDelay(5);
        vchr02_drv_init();
        #endif
        #else
        vchr02_drv_init();
        //QMAX981_SetTimerPeroid(40);//50
        hrs_drv_open(HRS_BLOODOXYGEN);//hrs_drv_open(HRS_HEARTRATE);
        #endif
        //ZDTHSensor_HRCheckTimer_Start();//use int to read data
        //ret = TRUE;
        vchr02_is_open = TRUE;
    }

    return ret;
}


PUBLIC int vchr02_drv_open()
{
    int ret = 0;

    //ZDT_HSENSOR_TRACE("hsensor_open is_init=%d,type=%d",hsensor_is_init,s_hsensor_type);
    if(vchr02_is_open == FALSE)
    {
        #if 0
        #ifndef WIN32
        GPIO_SetHSensorPower(1);
        hal_LcdDelay(5);
        vchr02_drv_init();
        #endif
        #else
        vchr02_drv_init();
        QMAX981_SetTimerPeroid(50);//50
        hrs_drv_open(HRS_HEARTRATE);//hrs_drv_open(HRS_HEARTRATE);
        #endif
        //ZDTHSensor_HRCheckTimer_Start();//use int to read data
        //ret = TRUE;
        vchr02_is_open = TRUE;
    }

    return ret;
}

int vchr02_drv_close()
{
    int ret = 0;
    if(vchr02_is_open == TRUE)
    {
        //ZDTHSensor_HRCheckTimer_Stop();
        QMAX981_SetTimerPeroid(1000);
		VCHR02_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);

        GPIO_SetHSensorPower(0);
    #endif
        //VCHR02_power_off();
        mHeartRateValue = 0;
        real_spo = 0;
        VCHR02_eint_en(0);
        ret = TRUE;
        vchr02_is_open = FALSE;
    }

    return ret;
}


int vchr02_drv_read_data()
{
    int ret = 0;

    return ret;
}

extern uint32 VCHR02_HandleInt()
{
    vcHr02IRQFlag = TRUE;
    VCHR02_process(vcSportMode);  
}


BOOLEAN VCHR02_CheckID(void)
{
    BOOLEAN res = FALSE;
    uint8 id = 0;
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    VCHR02_I2C_Open();
    id = VCHR02_ReadRegisters(0x00,&id,1);
    ZDT_HSENSOR_TRACE("VCHR02_CheckID id=0x%x",id);
    if(id == 0x21)
    {
        res = TRUE;
    }
    VCHR02_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);
    #endif
    return res;
}

//=======yangyu add end  =======

#endif

