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
#define     TW_SENSOR_CHECK_TIMER                   (300)   //(2*1000)

#define TEMP_I2C_ERROR 1
#define TEMP_I2C_SUCCESS 0

int16 zdt_cur_tw = 0;
static SCI_TIMER_PTR      s_zdt_tw_check_timer = PNULL;
LOCAL int s_temperature_i2c_dev_handler=TW_SENSOR_I2C_NULL_HANDLE;

    
BOOLEAN  ZDT_temperature_CheckTimer_Start(void);
BOOLEAN  ZDT_temperature_CheckTimer_Stop(void);
LOCAL uint32 temperature_I2C_Close(void);

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
    dev.slave_addr= 0x30;
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
    //LDO_SetVoltValue(LDO_LDO_CAMA, SENSOR_AVDD_2800MV);
    //LDO_TurnOnLDO(LDO_LDO_CAMA);     
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

LOCAL void temperature_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(Get_temperature_I2C_Status() == TEMP_I2C_SUCCESS)
        I2C_HAL_Write(s_temperature_i2c_dev_handler, reg, addr, cnt);
}

/******************************************************************************/
// Description: QMAX981_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
LOCAL void temperature_I2C_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(Get_temperature_I2C_Status() == TEMP_I2C_SUCCESS)
        I2C_HAL_Read(s_temperature_i2c_dev_handler,reg, addr, cnt);
}

static void temperature_measure(void) //(uint8 cycle_measure,int16 *temperature_object)
{
    uint8 reg; 
    int16 value;
    uint32 i=0;
    uint8 out_buf[2]={0};
    uint8 Reg_Read=0;
    //int16 m_temp = 0.00;
    //uint8 tmp_str[50] = {0};
    
    Reg_Read=0x1A;
    temperature_I2C_Read(&Reg_Read,out_buf,2);
    value=out_buf[1]<<8|out_buf[0];
    SCI_TRACE_LOW("temperature_measure 0x1A value=%d,out_buf=%d,%d",value,out_buf[0],out_buf[1]);
    //SCI_TRACE_LOW("temperature_measure 0x1A out_buf=%x,%x",out_buf[0],out_buf[1]);	
    hal_LcdDelay(350); //(1000*1);  // 15000 @ MCU_CLOCK 104M	
    
    Reg_Read=0x1C;
    temperature_I2C_Read(&Reg_Read,out_buf,2);
    value=out_buf[1]<<8|out_buf[0];
    SCI_TRACE_LOW("temperature_measure 0x1C value=%d,out_buf=%x,%x",value,out_buf[0],out_buf[1]);
    //temperature_object =value;
    //m_temp = value;
    //m_temp = m_temp /10;
    zdt_cur_tw = value;
    //sprintf((char*)tmp_str, (char*) "%.1lf",zdt_cur_tw);
    //SCI_TRACE_LOW("temperature_measure tmp_str=%s",tmp_str);
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
    temperature_measure();
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


