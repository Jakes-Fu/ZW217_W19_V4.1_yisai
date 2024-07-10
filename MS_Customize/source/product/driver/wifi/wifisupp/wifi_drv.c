/******************************************************************************
 ** File Name:      wifi_drv.c                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           06/05/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of WIFI.   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/05/2010     Liangwen.zhen    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "ms_customize_trc.h"
#include "os_api.h"
#include "wifi_drv.h"
#include "sc_reg.h"
//#include "spi_drv.h"
#include "nv_item_id.h"
#include "gpio_prod_api.h"
#include "nvitem.h"
#include "deepsleep_drvapi.h"
#include "spi_drvapi.h"

#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
#include "ldo_drvapi.h"
#include "osc_drvapi.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#define DEBUG_WIFI

#ifdef DEBUG_WIFI
#define WIFI_PRINT   SCI_TRACE_LOW 
#else
#define WIFI_PRINT( _format_string )  
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#if defined(PLATFORM_SC6600L)
#define WIFI_SPI_PORT           1
#elif defined(PLATFORM_SC6800H)
#define WIFI_SPI_PORT           0
#elif defined(PLATFORM_SC8800G)
#define WIFI_SPI_PORT           0
#elif defined(PLATFORM_SC6530)
#ifdef CHIP_VER_6531
#define WIFI_SPI_PORT           SPI0_3_CS0
#else
#define WIFI_SPI_PORT           SPI1_3_CS0
#endif
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define WIFI_SPI_PORT           SPI0_3_CS0
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structure Definition                         *
 **---------------------------------------------------------------------------*/
typedef struct
{
    uint32 port_num;
    uint32 (*open)( uint32 port_num );
    uint32 (*read)( uint32 port_num, uint8* buf_ptr, uint32 lenght);
    uint32 (*write)( uint32 port_num, uint8* buf_ptr, uint32 lenght);
    uint32 (*readwrite)( uint32 port_num, uint8* buf_ptr_in, uint8* buf_ptr_out,  uint32 lenght); // CSR: Added RW function to optimize short switch commands
    uint32 (*read_asyn)( uint32 port_num, uint8* buf_ptr, uint32 lenght,WIFI_Asyn_Callback call_func);
    uint32 (*write_asyn)( uint32 port_num, uint8* buf_ptr, uint32 lenght,WIFI_Asyn_Callback call_func);
    uint32 (*close)( uint32 port_num );
    
}WIFI_PORT_T;

typedef struct
{
    SPI_DEV spiConfig;     
}WIFI_SPI_PORT_CFG_T;

WIFI_Asyn_Callback wifi_asyn_write_call_func = NULL;
WIFI_Asyn_Callback wifi_asyn_read_call_func = NULL;

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL WIFI_STATUS_E          s_wifi_status = WIFI_STATUS_NOT_INIT;
LOCAL WIFI_PORT_T            s_wifi_port   = {0};
LOCAL WIFI_IRQ_CALLBACK_FUNC s_wifi_irq_callback = PNULL;
// customer should modiy the configuration according to the special product in the different platform
LOCAL WIFI_SPI_PORT_CFG_T s_spi_port_cfg = {0};
LOCAL WIFI_CMD_CALLBACK_FUNC s_wifi_cmd_callback = NULL;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to open SPI module 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _WIFI_SPI_Open(uint32 port_num)
{
    SPI_PAD_CFG_T pad_cfg;
    // init SPI_PORT_1
    //SCI_TRACE_LOW:"_WIFI_SPI_Open(...)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_101_112_2_18_0_36_20_2261,(uint8*)"");
#ifdef WIFI_VENDOR_CSR
    s_spi_port_cfg.spiConfig.freq          = 400000;
#elif defined WIFI_VENDOR_REALTEK
    s_spi_port_cfg.spiConfig.freq          = 19500000;
#elif defined WIFI_VENDOR_ATHEROS
    #ifdef PLATFORM_SC8800G
    s_spi_port_cfg.spiConfig.freq          = 48000000;
    #else
    s_spi_port_cfg.spiConfig.freq          = 24000000;
    #endif
#else 
#error "no define wifi vendor!"
#endif

#ifdef WIFI_SUPPORT_AR6102
    s_spi_port_cfg.spiConfig.tx_bit_length = 16;
#elif defined WIFI_VENDOR_REALTEK
    s_spi_port_cfg.spiConfig.tx_bit_length = 32;
#else
    s_spi_port_cfg.spiConfig.tx_bit_length = 8;
#endif

#ifdef WIFI_VENDOR_REALTEK
    s_spi_port_cfg.spiConfig.mode      = 0;
#else
    s_spi_port_cfg.spiConfig.mode      = 3;
#endif
    
    s_spi_port_cfg.spiConfig.id        = WIFI_SPI_PORT;//logic id reference spi_cfg.h
    s_spi_port_cfg.spiConfig.spi_cs_cb = PNULL;
    s_spi_port_cfg.spiConfig.spi_tx_cb = PNULL;
    s_spi_port_cfg.spiConfig.spi_rx_cb = PNULL;
    
    if(WIFI_RET_OK != SPI_HAL_Open(&s_spi_port_cfg.spiConfig))
    {
        //SCI_TRACE_LOW:"_WIFI_SPI_Open Open failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_137_112_2_18_0_36_20_2262,(uint8*)"");
        return WIFI_RET_PORT_ERR;
    } 

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    pad_cfg.pad_name = PAD_SPI1_3_CLK_ALTERNATE;
    pad_cfg.pad_fun  = SPI_PAD_SPI;
    SPI_HAL_Ioctl (s_spi_port_cfg.spiConfig.id, SPI_CTL_S_PAD, (uint32 *)&pad_cfg);
#endif
        
    return WIFI_RET_OK;     
}
/*****************************************************************************/
//  Description:    This function is used to read data to wifi chip by SPI 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _WIFI_SPI_Read(uint32 port_num, uint8* buf_ptr, uint32 lenght)
{   

    if((PNULL == buf_ptr) || (0 == lenght) || ((uint32)MAX_SPI_PORT_NUM <= port_num))
    {
        //WIFI_PRINT:"_WIFI_SPI_Read: port_num = %d, buf_ptr = 0x%x, lenght = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_157_112_2_18_0_36_20_2263,(uint8*)"ddd", port_num, buf_ptr, lenght);
        return WIFI_RET_PARAM_ERR;
    }

#ifdef WIFI_VENDOR_ATHEROS
    WIFI_Ctl_Bus(TRUE);
#endif

    if(SCI_FALSE == SPI_HAL_Read(s_spi_port_cfg.spiConfig.id, buf_ptr, lenght))
    {
        //SCI_TRACE_LOW:"WIFI_SPI_1 read failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_167_112_2_18_0_36_20_2264,(uint8*)"");
        return WIFI_RET_PORT_ERR;
    }  
    
#ifdef WIFI_VENDOR_ATHEROS
    WIFI_Ctl_Bus(FALSE);  
#endif
    
    return WIFI_RET_OK;
}

/*****************************************************************************/
//  Description:    This function is used to write data to wifi chip by SPI 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _WIFI_SPI_Write(uint32 port_num, uint8* buf_ptr, uint32 lenght)
{


    if((PNULL == buf_ptr) || (0 == lenght) || ((uint32)MAX_SPI_PORT_NUM <= port_num))
    {
        //WIFI_PRINT:"_WIFI_SPI_Write: port_num = %d, buf_ptr = 0x%x, lenght = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_187_112_2_18_0_36_20_2265,(uint8*)"ddd", port_num, buf_ptr, lenght);
        return WIFI_RET_PARAM_ERR;
    }

#ifdef WIFI_VENDOR_ATHEROS
    WIFI_Ctl_Bus(TRUE);
#endif    

    if(SCI_FALSE == SPI_HAL_Write(s_spi_port_cfg.spiConfig.id,buf_ptr,lenght))
    {
        //SCI_TRACE_LOW:"WIFI_SPI_1 write failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_197_112_2_18_0_36_20_2266,(uint8*)"");
        return WIFI_RET_PORT_ERR;
    }          
    
#ifdef WIFI_VENDOR_ATHEROS
    WIFI_Ctl_Bus(FALSE);
#endif

    return WIFI_RET_OK;
}

/*****************************************************************************/
//  Description:    ReadWrite function that lets us do short write+read combo fast 
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 _WIFI_SPI_ReadWrite( uint32 port_num, uint8* buf_ptr_in, uint8* buf_ptr_out,  uint32 lenght)
{
     if((PNULL == buf_ptr_in) || (PNULL == buf_ptr_out) ||  (0 == lenght) || ((uint32)MAX_SPI_PORT_NUM <= port_num))
    {
        //WIFI_PRINT:"_WIFI_SPI_ReadWrite: port_num = %d, buf_ptr_in = 0x%x, buf_ptr_out = 0x%x, lenght = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_215_112_2_18_0_36_20_2267,(uint8*)"dddd", port_num, buf_ptr_in, buf_ptr_out, lenght);
        return WIFI_RET_PARAM_ERR;
    }
     
     if(SCI_FALSE == SPI_HAL_RW(s_spi_port_cfg.spiConfig.id, buf_ptr_in,  buf_ptr_out, lenght))
    {
        //SCI_TRACE_LOW:"WIFI_SPI_1 write failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_221_112_2_18_0_36_20_2268,(uint8*)"");
        return WIFI_RET_PORT_ERR;
    }         

    return WIFI_RET_OK;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.10.12                                                *
// Note:
******************************************************************************/
uint32 _WIFI_SPI_Write_Asyn(uint32 port_num, uint8* buf_ptr, uint32 lenght,WIFI_Asyn_Callback read_call_func)
{
    if((PNULL == buf_ptr) || (0 == lenght) || ((uint32)MAX_SPI_PORT_NUM <= port_num) || (read_call_func == NULL))
    {
        //WIFI_PRINT:"_WIFI_SPI_Write_Asyn: port_num = %d, buf_ptr = 0x%x, lenght = %d,read_call_func=%p"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_238_112_2_18_0_36_20_2269,(uint8*)"dddd", port_num, buf_ptr, lenght,read_call_func);
        return WIFI_RET_PARAM_ERR;
    }

    if(SCI_FALSE == SPI_HAL_Write_Async(s_spi_port_cfg.spiConfig.id,buf_ptr,lenght,read_call_func))
    {
        //SCI_TRACE_LOW:"WIFI_SPI_1 write failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_244_112_2_18_0_36_20_2270,(uint8*)"");
        return WIFI_RET_PORT_ERR;
    }          
    return WIFI_RET_OK;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.10.12                                                *
// Note:
******************************************************************************/
uint32 _WIFI_SPI_Read_Asyn(uint32 port_num, uint8* buf_ptr, uint32 lenght,WIFI_Asyn_Callback read_call_func)
{

    if((PNULL == buf_ptr) || (0 == lenght) || ((uint32)MAX_SPI_PORT_NUM <= port_num) || (read_call_func == NULL))
    {
        //WIFI_PRINT:"_WIFI_SPI_Read_Asyn: port_num = %d, buf_ptr = 0x%x, lenght = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_261_112_2_18_0_36_20_2271,(uint8*)"ddd", port_num, buf_ptr, lenght);
        return WIFI_RET_PARAM_ERR;
    }

    if(SCI_FALSE == SPI_HAL_Read_Async(s_spi_port_cfg.spiConfig.id,buf_ptr,lenght,read_call_func))
    {
        //SCI_TRACE_LOW:"WIFI_SPI_1 write failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_267_112_2_18_0_36_20_2272,(uint8*)"");
        return WIFI_RET_PORT_ERR;
    }          
    return WIFI_RET_OK;
}

/*****************************************************************************/
//  Description:    This function is used to close SPI 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _WIFI_SPI_Close( uint32 port_num )
{
    if( (uint32)MAX_SPI_PORT_NUM <= port_num)
    {
        //WIFI_PRINT:"_WIFI_SPI_Close: port_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_280_112_2_18_0_36_21_2273,(uint8*)"d", port_num);
        return WIFI_RET_PARAM_ERR;
    }
    
    SPI_HAL_Close(s_spi_port_cfg.spiConfig.id);

    return WIFI_RET_OK;

}

/*****************************************************************************/
//  Description:    This function is used to open SDIO 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _WIFI_SDIO_Open(uint32 port_num)
{
    return WIFI_RET_ERR;     
}
/*****************************************************************************/
//  Description:    This function is used to read data from wifi chip by SDIO 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _WIFI_SDIO_Read(uint32 port_num, uint8* buf_ptr, uint32 lenght)
{           
    return WIFI_RET_ERR;
}

/*****************************************************************************/
//  Description:    This function is used to write data to wifi chip by SIDO 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _WIFI_SDIO_Write(uint32 port_num, uint8* buf_ptr, uint32 lenght)
{   
    return WIFI_RET_ERR;
}

/*****************************************************************************/
//  Description:    This function is used to close SIDO 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _WIFI_SDIO_Close( uint32 port_num )
{
    return WIFI_RET_ERR;
}

/*****************************************************************************/
//  Description:    This function is used to power on/down wifi chip 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _WIFI_PowerDown(BOOLEAN is_powerdown)
{
    GPIO_SetWifiPowerDown(is_powerdown);
    SCI_Sleep (10); //we need power stable
}

/*****************************************************************************/
//  Description:    This function is used to set status for wifi 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _WIFI_SetStatus(WIFI_STATUS_E status)
{
    s_wifi_status = status;
}

/*****************************************************************************/
//  Description:    This function is used to get status for wifi 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL WIFI_STATUS_E _WIFI_GetStatus( void )
{
    return s_wifi_status ;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.10.12                                                *
// Note:
******************************************************************************/
int32 _WIFI_Write_Asyn_CallBack(uint32* param)
{
    SCI_WIFI_EnableDeepSleep(TRUE);
    wifi_asyn_write_call_func(0);
    return 0;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.10.12                                                *
// Note:
******************************************************************************/
int32 _WIFI_Read_Asyn_CallBack(uint32* param)
{
    SCI_WIFI_EnableDeepSleep(TRUE);
    wifi_asyn_read_call_func(0);
    return 0;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.01.27                                                *
// Note:
******************************************************************************/
uint32 _WIFI_Bus_Ioctl(uint32 logic_id, uint32 cmd, uint32 *arg)
{
    
    SPI_HAL_Ioctl(s_spi_port_cfg.spiConfig.id, cmd ,arg);
    
    return WIFI_RET_OK;
}

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to power on and open transmit port 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_Open(
                WIFI_PORT_TYPE_E port_type,
                WIFI_IRQ_CALLBACK_FUNC callback_func
                )
{
    uint32 ret_val = WIFI_RET_OK;
    
    if(WIFI_STATUS_OPEN == _WIFI_GetStatus())
    {
    	SCI_TRACE_LOW("\n wifi:WIFI_Open:wifi is already open");
        return ret_val;
    }
    
    // Register port function
    if(WIFI_PORT_TYPE_SPI == port_type)
    {
        s_wifi_port.open  = _WIFI_SPI_Open;
        s_wifi_port.read  = _WIFI_SPI_Read;
        s_wifi_port.write = _WIFI_SPI_Write;
        s_wifi_port.close = _WIFI_SPI_Close;
        s_wifi_port.write_asyn = _WIFI_SPI_Write_Asyn;
        s_wifi_port.read_asyn = _WIFI_SPI_Read_Asyn;
        s_wifi_port.readwrite = _WIFI_SPI_ReadWrite; 
    }
    else if( WIFI_PORT_TYPE_SDIO == port_type)
    {
        s_wifi_port.open  = _WIFI_SDIO_Open;
        s_wifi_port.read  = _WIFI_SDIO_Read;
        s_wifi_port.write = _WIFI_SDIO_Write;
        s_wifi_port.close = _WIFI_SDIO_Close;
    }
    else
    {
    	SCI_TRACE_LOW("\n wifi:WIFI_Open:WIFI_RET_PARAM_ERR");
        return WIFI_RET_PARAM_ERR;
    }

    // Register irq callback function
    if(PNULL != callback_func)
    {
        s_wifi_irq_callback = callback_func;
    }
	
    SCI_TRACE_LOW("\n wifi:WIFI_Open:pull PWD PIN NOW,WIFI PORT type. is %d",port_type);
	#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
		//LDO_TurnOnLDO (LDO_LDO_VDD33);		
		OSC_HAL_EnableRefAutoSleep(DCXO_REF1,SCI_TRUE);//enable 26M
	#endif
	_WIFI_PowerDown(SCI_FALSE);

    if(s_wifi_port.open)
    {
        ret_val = s_wifi_port.open(s_wifi_port.port_num);
    }

    _WIFI_SetStatus(WIFI_STATUS_OPEN);

    return ret_val;
    
}

/*****************************************************************************/
//  Description:    This function is used to power down and close transmit port 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_Close( void )
{
    uint32 ret_val = WIFI_RET_OK;
    
    if(WIFI_STATUS_OPEN == _WIFI_GetStatus())
    {       
        _WIFI_SetStatus(WIFI_STATUS_CLOSE);

        s_wifi_irq_callback = PNULL;
        
        if(s_wifi_port.close)
        {
            ret_val = s_wifi_port.close(s_wifi_port.port_num);
			
        }
		#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
		//LDO_TurnOffLDO (LDO_LDO_VDD33);		
		OSC_HAL_EnableRefAutoSleep(DCXO_REF1,SCI_FALSE);//disable 26M
		#endif
        _WIFI_PowerDown(SCI_TRUE);      
    }   

    return ret_val;
}

/*****************************************************************************/
//  Description:    This function is used to read data from wifi chip 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_Read( uint8 * buf_ptr, uint32 lenght)
{
    uint32 ret_val = WIFI_RET_OK;
    
    if(WIFI_STATUS_OPEN == _WIFI_GetStatus())
    {
        if(s_wifi_port.read)
        {
            ret_val = s_wifi_port.read(s_wifi_port.port_num, buf_ptr, lenght);
        }
    }
    else
    {
        ret_val = WIFI_RET_STATUS_ERR;
    }   

    return ret_val;
}

/*****************************************************************************/
//  Description:    This function is used to write data to wifi chip 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_Write( uint8 * buf_ptr, uint32 lenght)
{
    uint32 ret_val = WIFI_RET_OK;
    
    if(WIFI_STATUS_OPEN == _WIFI_GetStatus())
    {
        if(s_wifi_port.write)
        {
            ret_val = s_wifi_port.write(s_wifi_port.port_num, buf_ptr, lenght);
        }       
    }
    else
    {
        ret_val = WIFI_RET_STATUS_ERR;
    }

    return ret_val;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.10.10                                                *
// Note:
******************************************************************************/
PUBLIC uint32 WIFI_Read_Asyn(uint8* buf_ptr, uint32 lenght,WIFI_Asyn_Callback call_back)
{
    uint32 ret_val = WIFI_RET_OK;
    
    if(WIFI_STATUS_OPEN == _WIFI_GetStatus())
    {
        if(s_wifi_port.read_asyn)
        {
            //SCI_TRACE_LOW:"s_wifi_port.WIFI_Read_Asyn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_526_112_2_18_0_36_21_2274,(uint8*)"");
            wifi_asyn_read_call_func = call_back;
            SCI_WIFI_EnableDeepSleep(FALSE);
            ret_val = s_wifi_port.read_asyn(s_wifi_port.port_num, buf_ptr, lenght, _WIFI_Read_Asyn_CallBack);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"WIFI_Read_Asyn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_534_112_2_18_0_36_21_2275,(uint8*)"");
        ret_val = WIFI_RET_STATUS_ERR;
    }   

    return ret_val;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.10.10                                                *
// Note:
******************************************************************************/
PUBLIC uint32 WIFI_Write_Asyn( uint8 * buf_ptr, uint32 lenght,WIFI_Asyn_Callback call_back)
{
    uint32 ret_val = WIFI_RET_OK;
    
    if(WIFI_STATUS_OPEN == _WIFI_GetStatus())
    {
        if(s_wifi_port.write_asyn)
        {
            //SCI_TRACE_LOW:"s_wifi_port.write_asyn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_555_112_2_18_0_36_21_2276,(uint8*)"");
            wifi_asyn_write_call_func = call_back;          
            SCI_WIFI_EnableDeepSleep(FALSE);
            ret_val = s_wifi_port.write_asyn(s_wifi_port.port_num, buf_ptr, lenght, _WIFI_Write_Asyn_CallBack);
        }       
    }
    else
    {
        //SCI_TRACE_LOW:"WIFI_RET_STATUS_ERR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_563_112_2_18_0_36_21_2277,(uint8*)"");
        ret_val = WIFI_RET_STATUS_ERR;
    }

    return ret_val;
}

/*****************************************************************************/
//  Description:    ReadWrite function that lets us do short write+read combo fast 
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_ReadWrite( uint8 * buf_ptr_in, uint8 *buf_ptr_out, uint32 lenght)
{
     uint32 ret_val = WIFI_RET_OK;
    
    if(WIFI_STATUS_OPEN == _WIFI_GetStatus())
    {
        if(s_wifi_port.readwrite)
        {
            ret_val = s_wifi_port.readwrite(s_wifi_port.port_num, buf_ptr_in, buf_ptr_out, lenght);
        }
    }
    else
    {
        ret_val = WIFI_RET_STATUS_ERR;
    }

    return ret_val;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.02.14                                                *
// Note:
******************************************************************************/
uint32 WIFI_Ctl_Signal(BOOLEAN mutex)
{
    SPI_MUTEX_E spi_mutex;
    
    
    if(mutex == TRUE)
    {
        spi_mutex = SPI_MUTEX_GET;
        
    }
    else
    {
        spi_mutex = SPI_MUTEX_PUT;
    }

    _WIFI_Bus_Ioctl(NULL,SPI_CTL_S_MUTEX,&spi_mutex);
    
    return WIFI_RET_OK;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.02.14                                                *
// Note:
******************************************************************************/
uint32 WIFI_Ctl_Bus(BOOLEAN cs_select)
{
    uint32 status;

    if(cs_select == TRUE)
    {
        status = SPI_CS_LOW;
    }
    else
    {
        status = SPI_CS_HIGH;
    }
    
    _WIFI_Bus_Ioctl(NULL,SPI_CTL_S_CS, &status);

    return WIFI_RET_OK;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author: 
// DATE:
// Note:
******************************************************************************/
PUBLIC uint32 WIFI_Ctl_Bus_No_Mutex(BOOLEAN cs_select)
{
    uint32 status;

    if(cs_select == TRUE)
    {
        status = SPI_CS_LOW;
    }
    else
    {
        status = SPI_CS_HIGH;
    }
    
    _WIFI_Bus_Ioctl(NULL,SPI_CTL_S_CS_NO_MUTEX, &status);

    return WIFI_RET_OK;
}

/******************************************************************************
// Description: 
// Dependence: 
// Author: 
// DATE:
// Note:
******************************************************************************/
PUBLIC void WIFI_interrupt_enable(BOOLEAN is_enable)
{
    GPIO_SPIIRQCtrl(is_enable);
}

/******************************************************************************
// Description: 
// Dependence: 
// Author: 
// DATE:
// Note:
******************************************************************************/
PUBLIC void WIFI_Set_Clock_Frequency(uint32 hz)
{
    SPI_HAL_Ioctl (WIFI_SPI_PORT, SPI_CTL_S_FREQ, &hz);
}

/*****************************************************************************/
//  Description:    This function is used to get the status of wifi chip 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC WIFI_STATUS_E WIFI_GetStatus( void )
{
    return _WIFI_GetStatus( );
}

/*****************************************************************************/
//  Description:    This function is used to callback wifi irq  
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void WIFI_RegCallback(WIFI_IRQ_CALLBACK_FUNC  callback_func)
{
    SCI_TRACE_LOW(__func__);    
    // Register irq callback function
    if(PNULL != callback_func)
    {
        s_wifi_irq_callback = callback_func;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 WIFI_IrqCallback(uint32 param)
{
    //SCI_TRACE_LOW(__func__);
    if(PNULL != s_wifi_irq_callback)
    {
        return s_wifi_irq_callback(param);
    }
    else
    {
        return WIFI_RET_OK;
    }
}
/*****************************************************************************/
//  Description:    This function is used to get the nv parameter for fixed nv 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_GetNvParam(
                    WIFI_ADDR_T*      addr_ptr,
                    WIFI_TX_FACTOR_T* tx_factor_ptr
                    )
{
    NVITEM_ERROR_E  status  = NVERR_NONE;
    uint32          ret_val = WIFI_RET_OK;
    WIFI_NV_PARAM_T nv_param = {0};

    if((PNULL != addr_ptr) || (PNULL != tx_factor_ptr))
    {    
        status = EFS_NvitemRead(NV_WIFI_CONFIG, sizeof(WIFI_NV_PARAM_T), (uint8*)&nv_param);
        //SCI_TRACE_LOW:"wifi WIFI_GetNvParam:%x%x%x%x%x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFI_DRV_739_112_2_18_0_36_22_2278,(uint8*)"dddddd", nv_param.addr.address[0], nv_param.addr.address[1], nv_param.addr.address[2], nv_param.addr.address[3],nv_param.addr.address[4],nv_param.addr.address[5]);
        if(NVERR_NONE != status)
        {
            ret_val = WIFI_RET_NV_ERR;
        }

        if(PNULL != addr_ptr)
        {
            *addr_ptr = nv_param.addr;
        }

        if(PNULL != tx_factor_ptr)
        {
            *tx_factor_ptr = nv_param.tx_factor;
        }
    }
    else
    {
        ret_val = WIFI_RET_PARAM_ERR;
    }

    return ret_val;
    
}

/*****************************************************************************/
//  Description:    
//  Author:
//  Note:           
/*****************************************************************************/
PUBLIC void WIFI_CmdCallbackReg(WIFI_CMD_CALLBACK_FUNC func)
{
    s_wifi_cmd_callback = func;
}

/*****************************************************************************/
//  Description:    
//  Author:
//  Note:           
/*****************************************************************************/
PUBLIC void WIFI_CmdHandler(
                    WIFI_CMD_PRINTF_FUNC cprintf,
                    int argc,
                    char *argv[]
                    )
{
    if (s_wifi_cmd_callback != NULL)
        s_wifi_cmd_callback(cprintf, argc, argv);
}

/*****************************************************************************/
//  Description:    This function is used to save nv parameter to fixed nv 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_SaveNvParam(
                    WIFI_ADDR_T*      addr_ptr,
                    WIFI_TX_FACTOR_T* tx_factor_ptr
                    )
{
    NVITEM_ERROR_E  nv_status = NVERR_NONE;
    uint32          ret_val   = WIFI_RET_OK;
    WIFI_NV_PARAM_T nv_param  = {0};

    if((PNULL != addr_ptr) || (PNULL != tx_factor_ptr))
    {    
        nv_status = EFS_NvitemRead(NV_WIFI_CONFIG, sizeof(WIFI_NV_PARAM_T), (uint8*)&nv_param);
        if(NVERR_NONE == nv_status)
        {
            if(PNULL != addr_ptr)
            {
                nv_param.addr = *addr_ptr;
            }

            if(PNULL != tx_factor_ptr)
            {
                nv_param.tx_factor = *tx_factor_ptr;
            }

            nv_status = NVITEM_UpdateCali(NV_WIFI_CONFIG, sizeof(WIFI_NV_PARAM_T), (uint8*)&nv_param);
            if(NVERR_NONE != nv_status)
            {
                ret_val = WIFI_RET_NV_ERR;
            }
        }
        else
        {
            ret_val = WIFI_RET_NV_ERR;
        }
    }
    else
    {
        ret_val = WIFI_RET_PARAM_ERR;
    }

    return ret_val; 
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of wifi_drv.c

