/******************************************************************************
 ** File Name:      msensor_mir3da.c                                          *
 ** Author:          Yc.Wang                                                *
 ** DATE:            2015.08.31                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2015.08.31   Yc.Wang            Create.                               *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"

#include "timer_drvapi.h"
#include "sensor_drv.h"
#include "ldo_drvapi.h"

//#include "mn_api.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/

#ifdef WIN32
#define __func__ 
#endif

#ifndef ZDT_DELAY_US
#define ZDT_DELAY_US TIMER_DELAY_US
#endif

#ifndef ZDT_DELAY_MS
#define ZDT_DELAY_MS hal_LcdDelay
#endif

#define     TW_SENSOR_I2C_NULL_HANDLE     -1
#define     TW_SENSOR_I2C_FREQ                   (100*1000)
#define     TW_SENSOR_CHECK_TIMER                   (500)   //(2*1000)

#define TEMP_I2C_ERROR 1
#define TEMP_I2C_SUCCESS 0

int16 zdt_cur_tw = 0;

int16 zdt_cur_ntcttemp = 0;
int16 zdt_cur_objecttemp = 0;
int16 zdt_cur_CalibrationDelta = 0;
int16 zdt_cur_ObjectVol = 0;
 
    
static SCI_TIMER_PTR      s_zdt_tw_check_timer = PNULL;
LOCAL int s_temperature_i2c_dev_handler=TW_SENSOR_I2C_NULL_HANDLE;

#define fm78102_i2c_address 0xDA //0x6d// 7bit  no RW bit

#define FM78102_CHIP_ID   0x24
    
LOCAL BOOLEAN  ZDT_temperature_CheckTimer_Start(void);
LOCAL BOOLEAN  ZDT_temperature_CheckTimer_Stop(void);
LOCAL uint32 temperature_I2C_Close(void);
LOCAL int fm78102_getObjectVol();

PUBLIC int32 Sensor_temperature_I2cInit() 
{
    int32 dev_handler=0;
    I2C_DEV  dev={0x00};

	SCI_TRACE_LOW("Sensor_temperature_I2cInit");
    
    if(!(TW_SENSOR_I2C_NULL_HANDLE==s_temperature_i2c_dev_handler))        
    {
    	SCI_TRACE_LOW("_Sensor_IicHandlerRelease");
        _Sensor_IicHandlerRelease();
    }
    
    dev.id = 0;
    dev.freq=TW_SENSOR_I2C_FREQ;
    dev.slave_addr= fm78102_i2c_address;
    dev.reg_addr_num = 1;
    dev.check_ack=1;
    dev.no_stop = 1;

    dev_handler=I2C_HAL_Open(&dev);

    if(dev_handler==TW_SENSOR_I2C_NULL_HANDLE)
    {
        SCI_TRACE_LOW("dev_handler==TW_SENSOR_I2C_NULL_HANDLE");
    }
	
    s_temperature_i2c_dev_handler=dev_handler;
            
    return dev_handler;

}

PUBLIC void Sensor_temperature_poweron(void)
{
    zdt_cur_tw = 0;
    Sensor_SetVoltage(SENSOR_AVDD_1800MV, SENSOR_AVDD_2800MV, SENSOR_AVDD_1800MV);    
    Sensor_temperature_I2cInit();
    ZDT_temperature_CheckTimer_Start();
}

PUBLIC void Sensor_temperature_poweroff(void)
{
    temperature_I2C_Close();
    Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);
    ZDT_temperature_CheckTimer_Stop();
}

LOCAL uint32 Get_temperature_I2C_Status(void)
{
    if(s_temperature_i2c_dev_handler==-1)
    {
        return TEMP_I2C_ERROR;
    }
    else
    {
        return TEMP_I2C_SUCCESS;
    }
}

LOCAL uint32 temperature_I2C_Close(void)
{
    if(Get_temperature_I2C_Status() == TEMP_I2C_SUCCESS)
    {

        I2C_HAL_Close(s_temperature_i2c_dev_handler);
        s_temperature_i2c_dev_handler = -1;
    }	
    return TEMP_I2C_SUCCESS;
}

LOCAL void temperature_I2C_Write(uint8 reg, uint8 addr)
{
    if(Get_temperature_I2C_Status() == TEMP_I2C_SUCCESS)
        I2C_HAL_Write(s_temperature_i2c_dev_handler, &reg, &addr, 1);   
}

/******************************************************************************/
// Description: QMAX981_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint8 temperature_I2C_Read(uint8 reg)
{
    uint8 cmd_buf[2]={0};
    if(Get_temperature_I2C_Status() == TEMP_I2C_SUCCESS)
        I2C_HAL_Read(s_temperature_i2c_dev_handler,&reg, &cmd_buf[0], 1);
    return cmd_buf[0];
}

uint8 fm78102_chip_id()
{
	uint8 chip_id =0;	
	chip_id = temperature_I2C_Read(0x01);	
	SCI_TRACE_LOW("fm78102_chip_id chip_id=%x",chip_id);   
	return chip_id;
}	

uint32 fm78102_Get_Chip_CH0_Rawdata()
{
	uint8 databuf[4];	
	uint32 ch0_rawdata;
	databuf[0] = temperature_I2C_Read(0xb5);	
	databuf[1] = temperature_I2C_Read(0xb6);		
	databuf[2] = temperature_I2C_Read(0xb7);
	databuf[3] = temperature_I2C_Read(0xb8);      
	ch0_rawdata = (((uint32)databuf[3]<<24)|((uint32)databuf[2]<<16)|((uint32)databuf[1]<<8)|databuf[0]);
       SCI_TRACE_LOW("fm78102_Get_Chip_CH0_Rawdata databuf[]=%d,%d,%d,%d ch0_rawdata=%d",databuf[0],databuf[1],databuf[2],databuf[3],ch0_rawdata);    
	return ch0_rawdata;
}

uint32 fm78102_Get_Chip_CH2_Rawdata()
{
    uint8 databuf[3],Drdy,convert_finish,wait_count;
    uint32 ch2_rawdata;

    convert_finish = 0;
    temperature_I2C_Write(0x30, 0x08);//read ntc
    ZDT_DELAY_MS(5);
    wait_count = 0;	
    
    while(convert_finish==0)
    {
        Drdy = temperature_I2C_Read(0x02);
        if((Drdy&0x01) == 0x01)// 读 0x02值  等待 bit0为1  
        	convert_finish = 1;
    	if(wait_count>300)   //  in case no ack & i2c fatel  goto
    	{
    		convert_finish = 1;
    		return 0;
    	}
    	wait_count++;
    	ZDT_DELAY_MS(5);		
    }
    
    databuf[0] = temperature_I2C_Read(0x06);	
    databuf[1] = temperature_I2C_Read(0x07);	
    databuf[2] = temperature_I2C_Read(0x08);		
    ch2_rawdata = ((uint32)databuf[0]<<16)|((uint32)databuf[1]<<8)|databuf[2];
    SCI_TRACE_LOW("fm78102_Get_Chip_CH2_Rawdata databuf[]=%d,%d,%d,%d ch0_rawdata=%d",databuf[0],databuf[1],databuf[2],databuf[3],ch2_rawdata);
    return ch2_rawdata;
}

uint32 fm78102_Get_Chip_CH1_Rawdata()
{
    uint8 databuf[3],Drdy,convert_finish,wait_count;
    uint32 ch1_rawdata;
    		
    ZDT_DELAY_MS(5);
    convert_finish = 0;
    temperature_I2C_Write(0x30, 0x09);//read mems
    ZDT_DELAY_MS(5);
    wait_count = 0;	
    while(convert_finish==0)
    {
        Drdy = temperature_I2C_Read(0x02);
        if((Drdy&0x01) == 0x01)// 读 0x02值  等待 bit0为1  
        	convert_finish = 1;
        if(wait_count>300)   //  in case no ack & i2c fatel  goto
        {
        	convert_finish = 1;
        	return 0;
        }
        wait_count++;
        ZDT_DELAY_MS(5);		
    }
	
    databuf[0] = temperature_I2C_Read(0x06);	//H
    databuf[1] = temperature_I2C_Read(0x07);	//M 
    databuf[2] = temperature_I2C_Read(0x08);	//L 		
    ch1_rawdata = ((uint32)databuf[0]<<16)|((uint32)databuf[1]<<8)|databuf[2];
    SCI_TRACE_LOW("fm78102_Get_Chip_CH1_Rawdata databuf[]=%d,%d,%d,%d ch0_rawdata=%d",databuf[0],databuf[1],databuf[2],databuf[3],ch1_rawdata);    
    return ch1_rawdata;
}	

int fm78102_getntcttemp()
{
	uint32 ch2 = fm78102_Get_Chip_CH2_Rawdata();
	uint32 ch0 = fm78102_Get_Chip_CH0_Rawdata();
       zdt_cur_ntcttemp = temp78102_getntcttemp(ch2, ch0); 
       SCI_TRACE_LOW("fm78102 zdt_cur_ntcttemp=%d",zdt_cur_ntcttemp);  
	return zdt_cur_ntcttemp;
}

extern int fm78102_objectDelta;
int fm78102_getobjecttemp()
{
	//int objectDelta = 1;
	uint32 ch1 = fm78102_Get_Chip_CH1_Rawdata();
	uint32 ch0 = fm78102_Get_Chip_CH0_Rawdata();
	int ntc_temp = fm78102_getntcttemp();
       int object_temp = 0;
       if(fm78102_objectDelta==0)
        fm78102_objectDelta =1;
       object_temp = temp78102_getobjecttemp( ch1,ntc_temp,ch0,fm78102_objectDelta,0);
	zdt_cur_tw = temp78102_getbodytemp( ntc_temp,object_temp,2500)*10;   
       SCI_TRACE_LOW("fm78102 zdt_cur_objecttemp=%d,ntc_temp=%d,object_temp=%d,objectDelta=%d",zdt_cur_tw,ntc_temp,object_temp,fm78102_objectDelta);  
	return zdt_cur_tw;
}

int fm78102_ObjectCalibrationDelta()
{
    uint8 delta_str[20] = {0};
    int objectDelta = 0; 
    int temp37_arry[2]={0}; //{2500,700};//is  for example
    //temp37_arry[0] =2500;
    //temp37_arry[1] =700;
    temp37_arry[0]=fm78102_getntcttemp();
    temp37_arry[1]=fm78102_getObjectVol();
    SCI_TRACE_LOW("fm78102 A fm78102_ObjectCalibrationDelta=%d,temp37_arry[0]=%d,temp37_arry[1]=%d",objectDelta,temp37_arry[0],temp37_arry[1]); 
    objectDelta = temp78102_getobjecttempDelta(temp37_arry);
    //sprintf((char*)delta_str, (char*) "%.5f",objectDelta);
    //temp_IMEI_FixedNvAccess(NV_IMEI3, delta_str, MN_MAX_IMEI_LENGTH);
    SCI_TRACE_LOW("fm78102 B fm78102_ObjectCalibrationDelta=%d,temp37_arry[0]=%d,temp37_arry[1]=%d",objectDelta,temp37_arry[0],temp37_arry[1]);    
    return objectDelta;
}
LOCAL int fm78102_getObjectVol()
{

    uint32 ch1 = fm78102_Get_Chip_CH1_Rawdata();
    uint32 ch0 = fm78102_Get_Chip_CH0_Rawdata();
    int ntc_temp = fm78102_getntcttemp();
    zdt_cur_ObjectVol = temp78102_getobjecttemp( ch1,ntc_temp,ch0,1,1);
    SCI_TRACE_LOW("fm78102 fm78102_getObjectVol=%d",zdt_cur_ObjectVol);  
    return zdt_cur_ObjectVol;

}

PUBLIC void Sensor_temperature_cali_start(void)
{
    Sensor_SetVoltage(SENSOR_AVDD_1800MV, SENSOR_AVDD_2800MV, SENSOR_AVDD_1800MV);    
    Sensor_temperature_I2cInit();
    //fm78102_ObjectCalibrationDelta();
}

PUBLIC void Sensor_temperature_cali_stop(void)
{
    temperature_I2C_Close();
    Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);
}

static void temperature_measure(void) //(uint8 cycle_measure,int16 *temperature_object)
{
    //fm78102_chip_id();
    //fm78102_getntcttemp();
    fm78102_getobjecttemp();
    //fm78102_ObjectCalibrationDelta();
    //fm78102_getObjectVol();
    return;
}

double get_temperature_degree(void)
{
      double temp = 0.00;
      temperature_measure();
	return temp;
}

static void ZDT_temperature_CheckTimerOut(uint32 param)
{	        
    if(s_zdt_tw_check_timer != PNULL)
    {
        if(SCI_IsTimerActive(s_zdt_tw_check_timer))
        {
            SCI_DeactiveTimer(s_zdt_tw_check_timer);
        }
    }
    SCI_TRACE_LOW("ZDT_temperature_CheckTimerOut");
    if(0x24==fm78102_chip_id())
    {
    	temperature_measure();
    }
#if 0    
    if(s_zdt_tw_check_timer == PNULL)
    {
        s_zdt_tw_check_timer = SCI_CreateTimer("ZDTTW_Timer", ZDT_temperature_CheckTimerOut,
                                    1, TW_SENSOR_CHECK_TIMER, SCI_AUTO_ACTIVATE);
    }
    else
    {
        SCI_ChangeTimer(s_zdt_tw_check_timer, ZDT_temperature_CheckTimerOut,TW_SENSOR_CHECK_TIMER);
        SCI_ActiveTimer(s_zdt_tw_check_timer);	
    }
#endif    
    return ;
}

BOOLEAN  ZDT_temperature_CheckTimer_Start(void)
{
    SCI_TRACE_LOW("ZDT_temperature_CheckTimer_Start");
    if(s_zdt_tw_check_timer == PNULL)
    {
        s_zdt_tw_check_timer = SCI_CreateTimer("ZDTTW_Timer", ZDT_temperature_CheckTimerOut,
                                    1, TW_SENSOR_CHECK_TIMER, SCI_AUTO_ACTIVATE);
    }
    else
    {
        SCI_ChangeTimer(s_zdt_tw_check_timer, ZDT_temperature_CheckTimerOut,TW_SENSOR_CHECK_TIMER);
        SCI_ActiveTimer(s_zdt_tw_check_timer);	
    }
    return TRUE;
}

BOOLEAN  ZDT_temperature_CheckTimer_Stop(void)
{
    SCI_TRACE_LOW("ZDT_temperature_CheckTimer_Stop");
    if(s_zdt_tw_check_timer != PNULL)
    {
        if(SCI_IsTimerActive(s_zdt_tw_check_timer))
        {
            SCI_DeactiveTimer(s_zdt_tw_check_timer);
        }
    }
    return TRUE;
}

int16 ZDT_temperature_GetTW(void)
{
    SCI_TRACE_LOW("ZDT_temperature_GetTW zdt_cur_tw=%d",zdt_cur_tw);    
    return zdt_cur_tw;
}

#ifdef __cplusplus
}
#endif  // End of file


