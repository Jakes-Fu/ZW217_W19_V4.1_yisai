#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "i2c_api.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "gpio_prod_api.h"
#include "ldo_drv.h"
#include "qn8610i2c.h"

uint8 QMA_WriteReg(uint8 Regis_Addr,uint8 Data)
{
    Sensor_WriteReg(Regis_Addr,Data);

#if 0 //open to check iic
    ret = Sensor_ReadReg(Regis_Addr);

    if(Data!=ret)
    {
        //SCI_TRACE_LOW:"ATV:QN:QMA_WriteReg check :Fail Write[%d] Read[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,QN8610I2C_20_112_2_18_0_29_51_654,(uint8*)"dd",Data,ret);
    }
    else
    {
        //Do nothing
    }
#endif

    return SCI_SUCCESS;
}

uint8 QMA_ReadReg(uint8 Regis_Addr) 
{
    return Sensor_ReadReg(Regis_Addr);
}
