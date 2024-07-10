/******************************************************************************
 ** File Name:      mtv_cfg.c                                                 *
 ** Author:         hanbing.zhang                                             *
 ** DATE:           05/08/2008                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic configuration for mtv         *
 **                 by customer.                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/08/2008     hanbing.zhang        Create.                                *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mtv_cfg.h"
#include "Gpio_prod_cfg.h"
#include "lcm_prod.h"
#include "dal_lcd.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
MTV_COMM_CONFIG_T mtv_communication_config = 
{
    MTV_COMM_CFG_ID,
    15,
    14,
    0,
    0,
    100,
    MTV_LCD_TYPE_COMM,//MTV_LCD_TYPE_LGDP4531MCU,//cxw modified @2008/05/15
    0,         
    1,         
    0x7fffffff,
    460800,
    1,
    1,
    0,
    0,
    0,
	13000000,
	{
		IIS_MODE_SLAVE,
		150000000,
		3,
	},
	AUDIO_MODE_IIS,
    0,
    0,
    0xff,
    0
}; 

void * mtv_communication_config_ptr = (void *)(&mtv_communication_config); /* 4 MTV_MIDDLEWARE_ON_BB */

SDRAM_PARAM_S EMC_Device_Config =
{
/*t_RP:			*/ 	{0,24},           	//precharge to active delay, unit(ns)
/*t_RCDRD		*/  {0,24},				//active to read delay, unit(ns)
/*t_RCDWR		*/	{0,24},				//active to write delay,uint(ns)
/*t_WR			*/ 	{1,2},            	//write recovery time, unit(ck)
/*t_RAS			*/  {0,50},				//active to charge delay,unit(ns)
/*t_MRD			*/  {1,2},				//LOAD MODE REGISTER to other command delay,unit(ck)	
/*t_RRD			*/  {0,18},				//two active to different bank delay
/*t_RFC			*/  {0,98},//{0,80},   	//auto refresh period, unit(ns)
/*t_WTR			*/  {1,1},			    //write to read delay,uint(ck)
/*CAS_Latency	*/  3,         			//2 or 3
/*refresh period*/  64,                 //refresh period time, unit(ms)
/*ROW number	*/	4096*1,				//row number
/*ROW SIZE		*/  ROW_SIZE_8Kb,       //row capacity,unit(bit)
/*BANK SIZE		*/  BM_4BANK,			//bank number

/*device capacity*/ CAPACITY_128MBIT  ,  //unit:Mbit

/*device type	*/  EMC_DEV_DDR,		//device type
};

//bb<->seine spi config
KDuint32 SPI_TV_PORT  = 1;
KDuint32 SPI_TV_CLOCK = 13000000; // spi clock 13M

KDuint32 DTL_LCM_GPIO = 49;// Be careful! This should the real gpio number of SC6600R

#ifdef REG32
#undef REG32
#endif
#define REG32(x)      (*((volatile KDuint32 *)(x)))
extern void LCD_ILI9325_GetSeineCfg(KDuint32 **cfg_pptr, KDuint32 *len_ptr);
//extern void LCD_HX8347_GetSeineCfg(KDuint32 **cfg_pptr, KDuint32 *len_ptr);

LOCAL MTVCFG_LCD_T  s_mtvcfg_lcd[] = 
{
   // {0x20000,LCD_HX8347_GetSeineCfg},
    {(KDuint32)0x9325,(MTVCFG_LCDCFG)LCD_ILI9325_GetSeineCfg}
};
/*****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: config 26M clock
//	Global resource dependence: 
//  Author: hanbing.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MTVCFG_Open_26MClock(void)
{

    REG32(0X8c00023c) &= ~(BIT_6|BIT_7);//select pin function
    REG32(0X8c00023c) |= BIT_6;

    REG32(0X8B000070) &= ~(BIT_10|BIT_11);  //select slk_src
    REG32(0X8B000070) |= BIT_11; 
    
    REG32(0X8B000018) &= ~(0xff); //set clk_aux0 div
    
    REG32(0X8B000018) |= BIT_10; //enable clk_aux0
    SCI_TRACE_LOW("MTVCFG_Open_26MClock: %x" ,REG32(0X8B000018));
   // while(1);

}


/*****************************************************************************/
//  Description: 
//	Global resource dependence: 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MTVCFG_Close_26MClock(void)
{
    REG32(0X8B000018) &= ~(BIT_10); 
}
/*****************************************************************************/
//  Description: Get MTV LCD Config
//	Global resource dependence: 
//  Author:
//	Note:
/*****************************************************************************/
void MTVCFG_GetLCDCfg(KDuint32 **cfg_pptr,
                             KDuint32 *len_ptr)
{
    uint32 i = 0;
    uint32 lcd_dev_id = LCM_GetLcdDeviceID(MAIN_LCD_ID);
	uint32 count = sizeof(s_mtvcfg_lcd)/sizeof(MTVCFG_LCD_T);
	
    for ( i = 0; i< count; i++)
    {
        if ( lcd_dev_id == s_mtvcfg_lcd[i].lcd_id)
        {
            break;
        }
    }

	if(i >= count)
	{
		i = 0;
	}

	(*s_mtvcfg_lcd[i].cfg_func)(cfg_pptr,len_ptr);	

	SCI_TRACE_LOW("MTV: (lcd id) index = %d, dev_id = 0x%x, dev_id_in_tab = 0x%x", i, lcd_dev_id, s_mtvcfg_lcd[i].lcd_id);

}

#ifndef MTV_MIDDLEWARE_ON_BB
PUBLIC void VB_Hold_by_MTV (uint32 sample_rate)
{
	;
}
PUBLIC void VB_Set_Dophin_IIS_Internal_Mode()
{
	;
}
PUBLIC void VB_Set_Dophin_IIS_External_Mode()
{
	;
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

