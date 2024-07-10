/******************************************************************************
 ** File Name:      bt_prod.c                                                 *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           07/26/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of BT    *
 **					about product
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/11/2008     Liangwen.Zhen    Create.                         *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/*lint -e785 -e415 -e64 */

#include "ms_customize_trc.h"
#include "os_api.h"
#include "nv_item_id.h"
//#include "layer1_engineering.h"
#include "bt_abs.h"
#include "chng_freq.h"
#include "nvitem.h"
#include "gpio_prod_api.h"
#include "bt_prod.h"
#include "bt_cfg.h"
#include "spzip_api.h"
#if  1 //defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) 	|| defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
#include "arm9_shm_operation.h"
#include "ref_param.h"
#endif
#include "ldo_drvapi.h"
#include "pmic_26M_clock.h"

extern BOOLEAN g_uart_wait_bt_wakeup_cfm;
uint32 g_bt_sleep_time = 0;//if 8910, BT Controller send the sleep time in mailbox
uint32 g_bt_sleep_time_ap32k = 0;//if 8910, when we received the g_bt_sleep_time, we record the the current AP 32K time.

extern uint8 s_bt_sleep_enter_time_1_index;//debug info
extern uint32 s_bt_sleep_enter_time_1[10];//debug info
extern uint8 s_bt_sleep_enter_time_2_index;//debug info
extern uint32 s_bt_sleep_enter_time_2[10];//debug info
extern uint8 s_bt_sleep_enter_time_3_index;//debug info
extern uint32 s_bt_sleep_enter_time_3[10];//debug info
extern uint8 s_bt_sleep_enter_time_4_index;//debug info
extern uint32 s_bt_sleep_enter_time_4[10];//debug info
extern uint32 s_bt_current_ap_32k;//debug info
extern uint32 s_bt_not_wakeup_error_times;//debug info
static uint32 s_WCN_BT_Sleep_Callback_wcnoscen_high=0;//debug info
static uint32 s_WCN_BT_Sleep_Callback=0;//debug info
static uint32 s_WCN_AWAKE_IrqHandler=0;//debug info
static uint32 s_WCN_AWAKE_IrqHandler_overtime_times=0;//debug info
static uint32 s_WCN_AWAKE_IrqHandler_enter_time=0;//debug info
static uint32 s_WCN_AWAKE_IrqHandler_exit_time=0;//debug info
static uint32 s_WCN_BT_Sleep_Callback_time_index=0;//debug info
static uint32 s_WCN_BT_Sleep_Callback_time[10] = {0};//debug info
static uint32 s_WCN_AWAKE_IrqHandler_time_index=0;//debug info
static uint32 s_WCN_AWAKE_IrqHandler_time[10] = {0};//debug info

extern BOOLEAN POWER_IsCalibrationResetMode(void);

#if 1// defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
uint32 g_curr_uart_bt_use;     // 1: curr uart is used by bt,0:curr uart is used by apuart 
void BTI_Rst_Bootup(uint8 *boot_addr_ptr)
{
    //WCN Clock
#if 0
    hwp_sysreg->RF_ANA_26M_CTRL_SET = 0x100;    //wcn 26M clock enable
    if (POWER_IsCalibrationResetMode())
    {
        //RPC interface cannot be used in Calibration Mode 
        *((volatile unsigned long *)0x500304c8) = 0x1;//ANA_ctrl_reg32=1; pmic 26M clock enable
    }
    else
    {
        aud_SetBtClk();//ANA_ctrl_reg32=1; pmic 26M clock enable
    }
#endif
    PMIC_26M_open(PMIC_26M_USED_BY_WCN);

    //WCN BT CPU clock frequecny from APPLL(1000MHz) /*3 means 1000MHz/8=125MHz */ /*2 means 1000MHz/12=83MHz*/ /*1 means 1000MHz/16=62MHz*/
    //Accoding to ASIC's suggestion, 8910 WCN clock cannot be more than 100MHz.
    //bringup_tmp
    #if 0
    //WCN Power
    while(!(hwp_pwrctrl->BTFM_PWR_STAT & CP_PWRCTRL_BTFM_STABLE));
    if (hwp_pwrctrl->BTFM_PWR_STAT != 0x3)   //open bt power
    {
        hwp_pwrctrl->BTFM_PWR_CTRL = CP_PWRCTRL_BTFM_PON;
    }
    while(hwp_pwrctrl->BTFM_PWR_STAT != 0x3);
    #endif
    //TIMER_DELAY_US(200);   /*need to wait for 200us*/
    SCI_Sleep(100);//100ms

    //WCN RF Power
    LDO_SetVoltValue(LDO_LDO_BTPHY, 1700);
    LDO_TurnOnLDO(LDO_LDO_BTPHY);
    SCI_Sleep(100);//100ms


    //Sleep-Wakeup Setting.
    //We will use AP<->RISC-V software control Sleep-Wakeup, not use OSC_En now.
    *(volatile uint32*)0x50090010 = 0;//hwp_idle->IDL_WCN_EN = 0;
    //*(volatile uint32*)0x50090010 = (BIT_4|BIT_3|BIT_2|BIT_1|BIT_0);
    *(volatile uint32*)0x500901F8 = 0x880;//eliminat_jitter  (for BT Sleep)


   //WCN boot up
    hwp_wcnSysCtrl->branch_addr = (((uint32)boot_addr_ptr)&0x7FFFFFFF)|0x22000000;

    hwp_wcnSysCtrl->soft_reset = 0x8000ffff;

    SCI_Sleep(310);//250ms for Risc-V init and calibration
}
#endif

#ifdef BLUETOOTH_SUPPORT_SPRD_BT
//#define BT_HL_PATCH_SUPPORT
#ifdef BT_HL_PATCH_SUPPORT

    #include "com_drv.h"
    #include "deepsleep_drvapi.h"
    #ifdef PLATFORM_SC6600L
    	#include "bt_hl_patch_66XX.h"  //patch for 6600
    #else
    	#include "bt_hl_patch_6800_6530.h"	//default for 6530 and 6800
    #endif
    
    #define HL_CMD_TX_LEN(i)     (hl_patch_cmd[i][0])
    #define HL_RF_CMD_TX_LEN(i)     (hl_patch_rf_cmd[i][0])

#endif
//#define DEBUG_BT_PROD
#ifdef  DEBUG_BT_PROD
	#define BT_TRACE   SCI_TRACE_LOW
	#define BT_PASSERT SCI_TRACE_HIGH
#else
  #ifdef WIN32
	#define BT_TRACE
	#define BT_PASSERT
  #else
	#define BT_TRACE(...) 
	#define BT_PASSERT(...) 
  #endif
#endif

char* controller_version = NULL;
#if 1 //defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)

#ifdef LOAD_FROM_EXTERL_MEM

#pragma arm section rwdata = "BT_SHARE_PSRAM_AREA"
#include "BT_INIT_IRAM.h";

#pragma arm section rwdata
#endif

#if 1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910)  || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
#define UINT8_TO_STREAM(p, u8)   {*(p)++ = (uint8)(u8);}
#define UINT24_TO_STREAM(p, u24) {*(p)++ = (uint8)(u24); *(p)++ = (uint8)((u24) >> 8); *(p)++ = (uint8)((u24) >> 16);}
#define UINT32_TO_STREAM(p, u32) {*(p)++ = (uint8)(u32); *(p)++ = (uint8)((u32) >> 8); *(p)++ = (uint8)((u32) >> 16); *(p)++ = (uint8)((u32) >> 24);}
#define UINT16_TO_STREAM(p, u16) {*(p)++ = (uint8)(u16); *(p)++ = (uint8)((u16) >> 8);}

t_sharememory * psharemem = (t_sharememory *)(0x30100000);//指向sharemem起始地址

#define PSKEY_NV_SIZE   268
//the BT_SPRD NV structure
typedef struct SPRD_BT_PSKEY_NV_INFO_T{
    uint8    g_dbg_source_sink_syn_test_data;
    uint8    g_sys_sleep_in_standby_supported;
    uint8    g_sys_sleep_master_supported;
    uint8    g_sys_sleep_slave_supported;

    uint32  default_ahb_clk;
    uint32  device_class;
    uint32  win_ext;
    uint32  g_aGainValue[6];
    uint32  g_aPowerValue[5];

    uint8    feature_set[16];
    uint8    device_addr[6];
    uint8    g_sys_sco_transmit_mode; //true tramsmit by uart, otherwise by share memory
    uint8    g_sys_uart0_communication_supported; //true use uart0, otherwise use uart1 for debug

    uint8    edr_tx_edr_delay;
    uint8    edr_rx_edr_delay;
    uint16  g_wbs_nv_117;

    uint32  is_wdg_supported;
    uint32  share_memo_rx_base_addr;
    uint32  share_memo_tx_base_addr;
    uint32  share_memo_tx_packet_num_addr;
    uint32  share_memo_tx_data_base_addr;
    uint32  g_PrintLevel;

    uint16  share_memo_tx_block_length;
    uint16  share_memo_rx_block_length;

    uint16  share_memo_tx_water_mark;
    uint16  share_memo_tx_timeout_value;

    uint16  uart_rx_watermark;
    uint16  uart_flow_control_thld;

    uint32  comp_id;

    uint16  pcm_clk_divd;
    uint16  g_nbs_nv_118;

    uint16  br_edr_diff_reserved;
    uint16  br_edr_powercontrol_flag;

    uint32  gain_br_channel_power[8];
    uint32  gain_edr_channel_power[8];
    uint32  g_aLEPowerControlFlag;
    uint16  g_aLEChannelpwrvalue[8];
    uint32  host_reserved[2];
    uint32  config0;
    uint32  config1;
    uint32  config2;
    uint32  config3;
    //uint16 bt_calibration_flag;//this is NOT in NV bt_sprd, this is defined in host/controller PSK structrue
    //uint16 bt_log_set;//this is NOT in NV bt_sprd, this is defined in host/controller PSK structrue
    uint32  reserved1[4];
    //uint32  chip_version;// 0 old 8910: 1 new 8910 ----- 
    uint16  reserved2[3];
}BT_PSKEY_NV_CONFIG_T;

BT_PSKEY_NV_CONFIG_T bt_para_setting = {0};

#else
t_sharememory * psharemem = (t_sharememory *)(0x10100000);//指向sharemem起始地址


typedef struct 
{

    uint8   g_dbg_source_sink_syn_test_data; //config0->bit0: 0
    uint8   g_sys_sleep_in_standby_supported; //config0->bit1:1
    uint8   g_sys_sleep_master_supported; //config0->bit2:1
    uint8   g_sys_sleep_slave_supported; //config0->bit3:1
    uint32  default_baud_rate;//26000000
    uint32  device_class;//0x001f00
    uint32  win_ext;//30
    uint32  g_aGainValue[6];
    uint32  g_aPowerValue[5];
    uint8   feature_set[16];
    uint8   device_addr[6];   
    uint8  g_sys_sco_by_uart_supported; // true transmit by uart, otherwise by share memory   config0->bit4:0
    uint8  g_sys_uart0_communication_supported; //true use uart0, otherwise use uart1 for debug config0->bit5:1
    uint8 edr_tx_edr_delay;//config1->byte0:5
    uint8 edr_rx_edr_delay;//config1->byte1:14
    uint32 is_wdg_supported; //config0->bit6:1
    uint32 share_memo_rx_base_addr;//const
    uint32 share_memo_tx_base_addr;//const
    uint32 share_memo_tx_packet_num_addr;//const
    uint32 share_memo_tx_data_base_addr;//const
    uint32 g_PrintLevel;//0xFFFFFFFF
    uint16 share_memo_tx_block_length; //const
    uint16 share_memo_rx_block_length;//const
    uint16 share_memo_tx_water_mark;	//const
    uint16 share_memo_tx_timeout_value;//config1->byte3+byte4: 32
    uint16 uart_rx_watermark;          //config2->char0 48
    uint16 uart_flow_control_thld;     //config2->char1 63
    uint32 comp_id; //const   
    uint16 debug_bb_gain; //0x8CFB
    uint16 debug_tx_power;//0xDC19
    uint16 controller_reserved;
	uint32 controller_reserved_1;
}BT_PSKEY_CONFIG_INFO_T;




BT_PSKEY_CONFIG_INFO_T bt_para_setting={
	0,
1,
1,
1,
26000000,
0x001f00,
30,
{0x0000,  0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
{0xB502, 
 0xBBC3, 0xC943,  
 0xCE41, 0xD243},
 {0xFF,0xFF,0x8D,0xFE,0x9B,0xFF,0x79,0x83,
 0xFF,0xA7,0xFF,0x7F,0x00,0xE0,0xF7,0x3E},
 {0x11,0xff,0x0,0x22,0x2d,0xae},
 0,//
 1,
 5,
 14,
 1,
 0x50000000,
 0x50000150,
 0x50000150,
 0x50000154,
 0xFFFFFFFF,//control point
 0x100,
 0xA0,
 220,
 32,
 48,
 63,
 0,
 0
};
#endif

//keddy end
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


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
#ifdef EXSTERN_WIFI_CO_EXIST
extern void AthBtFilter_HCI_Cmd_Notify(void* param,uint32 length);
extern void AthBtFilter_HCI_Evn_Notify(void* param,uint32 length);
extern void AthBtFilter_Acl_DataIn(void* param,uint32 length);
extern void AthBtFilter_Acl_DataOut(void* param,uint32 length);
extern void AthBtFilter_Attach(void );
extern void AthBtFilter_Detach(void);
void Set_Coexist_wifi_BT(uint8);
#endif

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
//s_bt_power_status is not ACCESSED!!!
//LOCAL BOOLEAN s_bt_power_status = SCI_FALSE;

LOCAL uint32 s_bt_idle_freq_handler = CHNG_FREQ_REQ_HANDLER_NULL;
LOCAL uint32 s_bt_ftp_freq_handler = CHNG_FREQ_REQ_HANDLER_NULL;
LOCAL uint32 s_bt_hfg_freq_handler = CHNG_FREQ_REQ_HANDLER_NULL;
LOCAL uint32 s_bt_a2dp_freq_handler = CHNG_FREQ_REQ_HANDLER_NULL;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
#ifdef FULL_VERSION
typedef struct Coexist_operate_str
{
	void (*init)(void);
    void (*close)(void);
    void (*cmd_notify)(void*,uint32);
    void (*even_notify)(void*,uint32);
    void (*acl_data_in)(void*,uint32);
    void (*acl_data_out)(void*,uint32);
    void (* sco_data_in)(void*,uint32);
    void (*sco_data_out)(void*,uint32);
}COEXIST_OPERATE_T;

COEXIST_OPERATE_T g_operate_func = {0};

uint8 Coexist_regist_operate(COEXIST_OPERATE_T *param)
{
	g_operate_func.init  = param->init;
    g_operate_func.close = param->close;
    g_operate_func.cmd_notify = param->cmd_notify;
    g_operate_func.even_notify = param->even_notify;
    g_operate_func.acl_data_in = param->acl_data_in;
    g_operate_func.acl_data_out = param->acl_data_out;
}    

void Coexist_Attach_start(void)
{
	if(g_operate_func.init != NULL)
    {
    	g_operate_func.init();
    }
}
void Coexist_End_Attach(void)
{
	if(g_operate_func.close != NULL)
	{
		g_operate_func.close();
    }
}    
void Coexist_HCI_Cmd_Notify(void* param,uint32 length)
{
	if(g_operate_func.cmd_notify != NULL)
	{
		g_operate_func.cmd_notify(param,length);
    }
}
void Coexist_HCI_Evn_Notify(void* param,uint32 length)
{
	if(g_operate_func.even_notify != NULL)
	{
		g_operate_func.even_notify(param,length);
    }
}
void Coexist_Acl_DataIn(void* param,uint32 length)
{
	if(g_operate_func.acl_data_in != NULL)
	{
		g_operate_func.acl_data_in(param,length);
    }
}
void Coexist_Acl_DataOut(void* param,uint32 length)
{
	if(g_operate_func.acl_data_out != NULL)
	{
		g_operate_func.acl_data_out(param,length);
    }
}    


#else
void Coexist_Attach_start(void)
{
	#ifdef EXSTERN_WIFI_CO_EXIST
	AthBtFilter_Attach();
    #endif
}
void Coexist_End_Attach(void)
{
#ifdef EXSTERN_WIFI_CO_EXIST
	AthBtFilter_Detach();
#endif
}    

void BtFilter_HCI_Cmd_Notify(void* param,uint32 length)
{
	#ifdef EXSTERN_WIFI_CO_EXIST
	AthBtFilter_HCI_Cmd_Notify(param,length);
    #endif
}
void BtFilter_HCI_Evn_Notify(void* param,uint32 length)
{
	#ifdef EXSTERN_WIFI_CO_EXIST
	AthBtFilter_HCI_Evn_Notify(param,length);
    #endif
}


void Coexist_HCI_Cmd_Notify(void* param,uint32 length)
{
	#ifdef EXSTERN_WIFI_CO_EXIST
	AthBtFilter_HCI_Cmd_Notify(param,length);
    #endif
}
void Coexist_HCI_Evn_Notify(void* param,uint32 length)
{
	#ifdef EXSTERN_WIFI_CO_EXIST
	AthBtFilter_HCI_Evn_Notify(param,length);
    #endif
}
void Coexist_Acl_DataIn(void* param,uint32 length)
{
	#ifdef EXSTERN_WIFI_CO_EXIST
	AthBtFilter_Acl_DataIn(param,length);
    #endif
}
void Coexist_Acl_DataOut(void* param,uint32 length)
{
	#ifdef EXSTERN_WIFI_CO_EXIST
	AthBtFilter_Acl_DataOut(param,length);
    #endif
}    
#endif

#ifdef BT_HL_PATCH_SUPPORT
/*****************************************************************************/
//  Description:    _BTI_Uart_Callback, dummy
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 _BTI_Uart_Callback(uint32 event)
{
    BT_TRACE("_BTI_Uart_Callback event = %d", event);
    return  0;
}

/*****************************************************************************/
//  Description:    _BTI_UartOpen
//  Author:         
//  Note:           
/*****************************************************************************/

LOCAL void _BTI_UartOpen(uint32 port, uint32 baudrate)
{
    UART_INIT_PARA_T    uart_st;
    
    BT_TRACE("[BT_PROD]  _BTI_UartOpen %d, 0x%X ", port, baudrate);

    uart_st.tx_watermark  = TX_WATER_MARK; 
    uart_st.rx_watermark  = TX_WATER_MARK;
    uart_st.baud_rate     = baudrate;
    uart_st.parity        = FALSE; 
    uart_st.parity_enable = PARITY_DISABLE;
    uart_st.byte_size     = EIGHT_BITS;
    uart_st.stop_bits     = ONE_STOP_BIT;
    uart_st.flow_control  = NO_FLOW_CONTROL;
    uart_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;
        
    UART_Initilize(port, &uart_st, _BTI_Uart_Callback);
    UART_Tx_Int_Enable(port, FALSE);
    UART_Rx_Int_Enable(port, FALSE);

}
/*****************************************************************************/
//  Description:    _BTI_UartOpen2, useless!!
//  Author:         
//  Note:           
/*****************************************************************************/

LOCAL void _BTI_UartOpen2(uint32 port, uint32 baudrate)
{
    UART_INIT_PARA_T    uart_st={0};
    
    BT_TRACE("[BT_PROD]  _BTI_UartOpen %d, 0x%X ", port, baudrate);

    uart_st.tx_watermark  = TX_WATER_MARK; 
    uart_st.rx_watermark  = TX_WATER_MARK;
    uart_st.baud_rate     = baudrate;
    uart_st.parity        = EVEN_PARITY; 
    uart_st.parity_enable = PARITY_ENABLE;
    uart_st.byte_size     = EIGHT_BITS;
    uart_st.stop_bits     = ONE_STOP_BIT;
    uart_st.flow_control  = NO_FLOW_CONTROL;
    uart_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;
        
    UART_Initilize(port, &uart_st, _BTI_Uart_Callback);
    UART_Tx_Int_Enable(port, FALSE);
    UART_Rx_Int_Enable(port, FALSE);

}

/*****************************************************************************/
//  Description:    UART_ReadData_ByFIFO_Count
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 UART_ReadData_ByFIFO_Count (uint32 port, uint8 *buffer, uint32 length, uint32  read_err_count )
{
    uint32	read_count=0;
    uint32 	uart_fifo_size=0;
    uint32 	err_index=0;
    uint32  i=0;
	   
	BT_TRACE("[BT_PROD]  UART_ReadData_ByFIFO_Count  port=%d, length=0x%X ", port, length);

	SCI_ASSERT(buffer != PNULL); /*assert verified*/
   
      do{	 
			uart_fifo_size= UART_GetRxFifoCnt(port);
			if(uart_fifo_size==length)
			{
				read_count=UART_ReadDataSync (port, buffer, uart_fifo_size);		
				if(read_count <10)
				{
                    for(i=0; i< read_count; i++)
                    {
                    	BT_TRACE("[BT_PROD] UART_ReadData_ByFIFO_Count: read_count=%d,  rx data: buffer[%d] =  0x%X ", uart_fifo_size, i, 
                    	buffer[i]);
                    }
                
				}
				
			}
            else if(uart_fifo_size>length)
            {
                BT_PASSERT("UART_ReadData_ByFIFO_Count, read count num=%d", uart_fifo_size);
            }
			else
			{
				SCI_Sleep(5);
			}
            BT_TRACE("[BT_PROD] UART_ReadData_ByFIFO_Count: uart_fifo_size = %d,  read_err_count=%d", uart_fifo_size, err_index );  						
			err_index++;
			
	}while(( read_count==0 && err_index <read_err_count));

    return  read_count;

}

/*****************************************************************************/
//  Description:    BTI_EarlyPatch: FOR HL BT CHIP
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint8 BTI_EarlyPatch(void)
{   
	uint32 	i = 0,j=0, k=0,err_count = 0;
	BT_CONFIG 	bt_cfg = {0};
	uint8 	cmd[HL_CMD_MAX_LEN] = {0x12, 0xc0, 0x00, 0xc8, 0x00, 0x37, 0x00, 0x05, 0x88, 0x00, \
								0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0}; 

	uint8 	rx_buffer[HL_CMD_RX_ACK_LEN] = {0}, remote_id = 0;
	uint8	rx_buffer_err[12] = {0};
	uint32	read_count=0,read_count_err=0, uart_fifo_size=0,trans_packet_time=0;
	uint32  patch_uartport=0;
	uint8 	reset_cmd[HL_CMD_MAX_LEN] = {0};
	uint32 	cmd_num = sizeof(hl_patch_cmd) / sizeof(hl_patch_cmd[0]);

	SCI_MEMCPY(reset_cmd, hl_reset_cmd, sizeof(hl_reset_cmd));
	
	BT_GetConfigInfo(&bt_cfg);
	// set bt uart port
	patch_uartport = bt_cfg.port_num;
	// set bt address
	SCI_MEMCPY(&cmd[11], bt_cfg.bd_addr.addr, sizeof(BT_ADDRESS));
	cmd[17] = cmd[6] + cmd[7] + cmd[8] + cmd[9] + cmd[10] + cmd[11] + 
	cmd[12]+ cmd[13]+ cmd[14]+ cmd[15]+ cmd[16];
	cmd[17] = 0xFF - cmd[17];
	for(i = 11, j = 18; i < j;i++)
	{
		if(cmd[i] == 0xc0){
				for(k = j; k > i; k--){
					cmd[k + 1] = cmd[k];	
				}
				cmd[i] = 0xdb;
				cmd[i + 1] = 0xdc;
				cmd[0]++;
				j++;
		}	else if(cmd[i] == 0xdb){
				for(k = j; k > i; k--){
					cmd[k + 1] = cmd[k];	
				}
				cmd[i] = 0xdb;
				cmd[i + 1] = 0xdd;
				cmd[0]++;
				j++;
		}
	}

	BT_TRACE("[BT_PROD] BTI_EarlyPatch: open uart = %d, tick = %d"
	  "bt_addr: 0x%X%X%X%X%X%X", 
	patch_uartport, SCI_GetTickCount(),
	bt_cfg.bd_addr.addr[0], bt_cfg.bd_addr.addr[1], bt_cfg.bd_addr.addr[2],
	bt_cfg.bd_addr.addr[3], bt_cfg.bd_addr.addr[4], bt_cfg.bd_addr.addr[5]);

	
	// uart open
	_BTI_UartOpen(patch_uartport, BAUD_115200);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_FALSE);
	// sync baudrate with HL
	UART_DumpPutChar(patch_uartport, HL_UART_BAUDRATE_SYNC_FLAG);

	SCI_Sleep(1); //wait sync 1ms 
	
	UART_Close(patch_uartport);
	_BTI_UartOpen2(patch_uartport, BAUD_115200);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_FALSE);
	
	
	for (i=0; i < 3; i++)
	{
		//UART_DumpPutChar(patch_uartport, HL_UART_BAUDRATE_SYNC_FLAG);
		//UART_WriteDataSync(patch_uartport, &hl_awake_cmd[1], hl_awake_cmd[0]);
		UART_WriteDataSync(patch_uartport, &hl_id_cmd[1], hl_id_cmd[0]);
		SCI_Sleep(3); //wait sync 3ms
	}
	uart_fifo_size=UART_GetRxFifoCnt(patch_uartport);
	while (uart_fifo_size >0)
	{
		UART_ReadDataSync (patch_uartport, &rx_buffer[0], 1);
		uart_fifo_size --;
	}
	//read CHIP ID
	for( i = 0; i < 10 ; i++)
	{
		UART_WriteDataSync(patch_uartport, &hl_id_cmd[1], hl_id_cmd[0]);
		SCI_Sleep(3);
		//UART_ReadDataSync (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN);
		UART_ReadData_ByFIFO_Count (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN, 3);

		if(HL_CMD_RX_ACK_OK == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
	  		BT_TRACE("[BT_PROD] BTI_EarlyPatch: read id ok");
            remote_id = 0;
            break;
		}
		else if(0 == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
            BT_TRACE("[BT_PROD] BTI_EarlyPatch: read id ok");
            remote_id = rx_buffer[HL_CMD_RX_ID_INDEX] + 1;
            break;
		}
		else if(i == (9))
		{
            BT_TRACE("[BT_PROD] *****************BTI_EarlyPatch: read id i=9  err" );
            BT_PASSERT("BTI_EarlyPatch read id 10  times no ack"); 
            break; //false
		}
	}

	BT_TRACE("[BT_PROD] BTI_EarlyPatch: RX_ACK = 0x%X, CHIP ID = 0x%2X, cmd_num=%d", 
	rx_buffer[HL_CMD_RX_ACK_INDEX], rx_buffer[HL_CMD_RX_ID_INDEX],cmd_num);	
	reset_cmd[6] = remote_id;
	reset_cmd[11] = hl_reset_cmd[11] - reset_cmd[6];

	//send reset
	UART_WriteDataSync(patch_uartport, &reset_cmd[1], reset_cmd[0]);
	SCI_Sleep(10); //wait sync 10ms

	UART_Close(patch_uartport);
	_BTI_UartOpen(patch_uartport, BAUD_115200);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_FALSE);
	UART_DumpPutChar(patch_uartport, HL_UART_BAUDRATE_SYNC_FLAG);
	SCI_Sleep(3); //wait sync 3ms

	
	uart_fifo_size=UART_GetRxFifoCnt(patch_uartport);//	uart_fifo_size= UART_PHY_GetRxFifoCnt(patch_uartport);
	if (uart_fifo_size > 0)
	{
	  BT_PASSERT("BTI_EarlyPatch UART Rx error!, uart_fifo_size = %d", uart_fifo_size); 
	}

	UART_Close(patch_uartport);
	_BTI_UartOpen2(patch_uartport, BAUD_115200);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_FALSE);
 
    //read CHIP ID
	for( i = 0; i < HL_CMD_MAX_ERR_COUNT ; i++)
	{
		UART_WriteDataSync(patch_uartport, &hl_id_cmd[1], hl_id_cmd[0]);
		SCI_Sleep(3);
		//UART_ReadDataSync (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN);
		UART_ReadData_ByFIFO_Count (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN, 3);

		if(HL_CMD_RX_ACK_OK == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
            BT_TRACE("[BT_PROD] BTI_EarlyPatch: send read id cmd ok");
            break;
		}
		else if(i == (HL_CMD_MAX_ERR_COUNT - 1))
		{
            BT_TRACE("[BT_PROD] *****************BTI_EarlyPatch: send the BD address  err" );
            BT_PASSERT("BTI_EarlyPatch send read id cmd 3 times err"); 
            break; //false
		}
	}  
  
	BT_TRACE("[BT_PROD] BTI_EarlyPatch: RX_ACK = 0x%X, CHIP ID = 0x%2X, cmd_num=%d", 
	rx_buffer[HL_CMD_RX_ACK_INDEX], rx_buffer[HL_CMD_RX_ID_INDEX],cmd_num);

	if (HL_CHIP_ID != rx_buffer[HL_CMD_RX_ID_INDEX])
    {
        BT_PASSERT("BTI_EarlyPatch is not BL V%d Chip ,id is %d", HL_CHIP_VERSION, rx_buffer[HL_CMD_RX_ID_INDEX]);
    }

	//send the BD address
	for( i = 0; i < HL_CMD_MAX_ERR_COUNT ; i++)
	{
		UART_WriteDataSync(patch_uartport, &cmd[1], cmd[0]);
		SCI_Sleep(3);
		//UART_ReadDataSync (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN);
		UART_ReadData_ByFIFO_Count (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN, 3);

		if(HL_CMD_RX_ACK_OK == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
            BT_TRACE("[BT_PROD] BTI_EarlyPatch: send the BD address  ok");
            break;
		}
		else if(i == (HL_CMD_MAX_ERR_COUNT - 1))
		{
            BT_TRACE("[BT_PROD] *****************BTI_EarlyPatch: send the BD address  err" );
            BT_PASSERT("BTI_EarlyPatch send the BD address 3 times err"); 
            break; //false
		}
	}  
      
	//send patch file  send patch one by one
	i=0;
	err_count = 0;
	
	while(i< cmd_num && err_count < HL_CMD_MAX_ERR_COUNT)
	{
		UART_WriteDataSync(patch_uartport, &hl_patch_cmd[i][1], HL_CMD_TX_LEN(i));
          
		SCI_MEMSET(&rx_buffer[0], 0, HL_CMD_RX_ACK_LEN);
		trans_packet_time = (100 * (HL_CMD_TX_LEN(i)) + 1400)/1000;
		BT_TRACE("[BT_PROD] BTI_EarlyPatch: trans_packet_time=%d", trans_packet_time);	
        
		SCI_Sleep(trans_packet_time);	

		do{	 

			read_count=0;	
			uart_fifo_size=UART_GetRxFifoCnt(patch_uartport);//	uart_fifo_size= UART_PHY_GetRxFifoCnt(patch_uartport);
	      	BT_TRACE("[BT_PROD] BTI_EarlyPatch:uart_fifo_size =%d",uart_fifo_size);
				
			if(uart_fifo_size==2)
			{    
			      //normal case for 2 byte ack
				read_count=UART_ReadDataSync (patch_uartport, &rx_buffer[0],HL_CMD_RX_ACK_LEN);
			}
			else if(uart_fifo_size>2  && i< cmd_num-1)
			{
				//clear all count data and assert;
				if(uart_fifo_size < 12)
				{
					read_count=UART_ReadDataSync (patch_uartport, &rx_buffer_err[0], uart_fifo_size);

					for(j=0; j<uart_fifo_size ;j++)
					{
						BT_TRACE("[BT_PROD] ***************** BTI_EarlyPatch: err rx_buffer_err[%d]=0x%X", j,  rx_buffer_err[j]);
					}
				}
				
				SCI_Sleep(100);
                BT_PASSERT("hl_patch_cmd[%d] uart_fifo_size =%d", i, uart_fifo_size);
			}
			else  if(uart_fifo_size==3  && i==cmd_num-1)
			{
				//clear all count data;
				read_count=UART_ReadDataSync (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN);
				UART_ReadDataSync (patch_uartport, 	&rx_buffer_err[0], 1);
				BT_TRACE("[BT_PROD] *****************BTI_EarlyPatch: err  rx_buffer[0]=0x%x, rx_buffer[1]=0x%X", 	rx_buffer[0], rx_buffer[1]);	
				BT_TRACE("[BT_PROD] *****************BTI_EarlyPatch: err  rx_buffer_err=0x%X", 	rx_buffer_err[0]);	
				BT_TRACE("[BT_PROD] *****************BTI_EarlyPatch: err  rx_buffer[0]=0x%x, rx_buffer[1]=0x%X", 	rx_buffer[0], rx_buffer[1]);	
				BT_TRACE("[BT_PROD] *****************BTI_EarlyPatch: err  rx_buffer_err=0x%X", 	rx_buffer_err[0]);	
                            //SCI_Sleep(100);
				//SCI_ASSERT(0);
			}		
			else  if(uart_fifo_size==1  && i==cmd_num-1)
			{
                if(read_count_err==3 )
                {
                    //clear all count data and assert;
                    read_count=UART_ReadDataSync (patch_uartport, &rx_buffer[0], uart_fifo_size);
                    //SCI_ASSERT(0); //remove for debug
                }
			}
			else
			{			      
				SCI_Sleep(10);
			}

			BT_TRACE("[BT_PROD] BTI_EarlyPatch: uart_fifo_size=%d", uart_fifo_size);	

			BT_TRACE("[BT_PROD] BTI_EarlyPatch: i = %d, TX_ID = 0x%2X, RX_ACK = 0x%X, RX_ID= 0x%2X, read_count=%d, read_count_err=%d, err = %d", 
			i, hl_patch_cmd[i][HL_CMD_TX_ID_INDEX], 
			rx_buffer[HL_CMD_RX_ACK_INDEX],
			rx_buffer[HL_CMD_RX_ID_INDEX], read_count,read_count_err,err_count );
			read_count_err++;

		}while(( read_count==0 && read_count_err <5));

		read_count_err=0;

		// check ack
		if(HL_CMD_RX_ACK_OK == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
			i++;           // next cmd
			err_count = 0; // reset error count
		}
		else
		{
			err_count++;
		}
	}

	UART_Close(patch_uartport);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_TRUE);

	BT_TRACE("[BT_PROD] BTI_EarlyPatch: close uart = %d, tick = %d", patch_uartport, SCI_GetTickCount() );    
	
	if(i < cmd_num)
	{
        SCI_Sleep(100);
        BT_PASSERT("BTI_EarlyPatch hl_patch_cmd[%d] 3 times transfer error , no ack, total %d", i, cmd_num);
        return 0;
	}
	else
	{
		return 1;
	}
}

/*****************************************************************************/
//  Description:    BTI_EutPatch: FOR HL BT EUT
//  Author:         
//  Note:           
/*****************************************************************************/

LOCAL uint8 BTI_HL_Patch(uint8* patch_cmd, uint32 cmd_num)
{   
	uint32 	i = 0,j=0, k=0,err_count = 0;
	BT_CONFIG 	bt_cfg = {0};
	uint8 	cmd[HL_CMD_MAX_LEN] = {0x12, 0xc0, 0x00, 0xc8, 0x00, 0x37, 0x00, 0x05, 0x88, 0x00,
								0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0};

	uint8 	rx_buffer[HL_CMD_RX_ACK_LEN] = {0}, remote_id = 0;
	uint8		rx_buffer_err[12] = {0};
	uint32	read_count=0,read_count_err=0, uart_fifo_size=0,trans_packet_time=0;
	uint32  patch_uartport=0;
	uint8 	reset_cmd[HL_CMD_MAX_LEN] = {0};
	//uint32 	cmd_num = sizeof(hl_patch_cmd) / sizeof(hl_patch_cmd[0]);

	SCI_MEMCPY(reset_cmd, hl_reset_cmd, sizeof(hl_reset_cmd));
	
	BT_GetConfigInfo(&bt_cfg);
	// set bt uart port
	patch_uartport = bt_cfg.port_num;
	// set bt address
	SCI_MEMCPY(&cmd[11], bt_cfg.bd_addr.addr, sizeof(BT_ADDRESS));
	cmd[17] = cmd[6] + cmd[7] + cmd[8] + cmd[9] + cmd[10] + cmd[11] + 
	cmd[12]+ cmd[13]+ cmd[14]+ cmd[15]+ cmd[16];
	cmd[17] = 0xFF - cmd[17];
	for(i = 11, j = 18; i < j;i++)
	{
		if(cmd[i] == 0xc0){
				for(k = j; k > i; k--){
					cmd[k + 1] = cmd[k];	
				}
				cmd[i] = 0xdb;
				cmd[i + 1] = 0xdc;
				cmd[0]++;
				j++;
		}	else if(cmd[i] == 0xdb){
				for(k = j; k > i; k--){
					cmd[k + 1] = cmd[k];	
				}
				cmd[i] = 0xdb;
				cmd[i + 1] = 0xdd;
				cmd[0]++;
				j++;
		}
	}

	BT_TRACE("[BT_PROD] BTI_HL_Patch: open uart = %d, tick = %d"
	  "bt_addr: 0x%X%X%X%X%X%X", 
	patch_uartport, SCI_GetTickCount(),
	bt_cfg.bd_addr.addr[0], bt_cfg.bd_addr.addr[1], bt_cfg.bd_addr.addr[2],
	bt_cfg.bd_addr.addr[3], bt_cfg.bd_addr.addr[4], bt_cfg.bd_addr.addr[5]);
	// uart open
	_BTI_UartOpen(patch_uartport, BAUD_115200);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_FALSE);
	// sync baudrate with HL
	UART_DumpPutChar(patch_uartport, HL_UART_BAUDRATE_SYNC_FLAG);

	SCI_Sleep(1); //wait sync 1ms 
	
	UART_Close(patch_uartport);
	_BTI_UartOpen2(patch_uartport, BAUD_115200);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_FALSE);
	
	
	//read CHIP ID
	for( i = 0; i < 3 ; i++)
	{
		UART_WriteDataSync(patch_uartport, &hl_id_cmd[1], hl_id_cmd[0]);
		SCI_Sleep(5);
		UART_ReadData_ByFIFO_Count (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN, 3);

		if(HL_CMD_RX_ACK_OK == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
	  		BT_TRACE("[BT_PROD] BTI_HL_Patch: read id ok");
            remote_id = 0;
            goto get_hl_id_response;
		}
	}
	
	UART_Close(patch_uartport);
#ifdef PLATFORM_SC6600L
	_BTI_UartOpen2(patch_uartport, BAUD_921600);
#else   
	_BTI_UartOpen2(patch_uartport, BAUD_3250000);
#endif   
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_FALSE);
	
	//read CHIP ID
	for( i = 0; i < 10 ; i++)
	{
		UART_WriteDataSync(patch_uartport, &hl_id_cmd[1], hl_id_cmd[0]);
		SCI_Sleep(3);
		//UART_ReadDataSync (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN);
		UART_ReadData_ByFIFO_Count (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN, 3);

		if(HL_CMD_RX_ACK_OK == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
	  		BT_TRACE("[BT_PROD] BTI_HL_Patch: read id ok");
            remote_id = 0;
            break;
		}
		else if(0 == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
            BT_TRACE("[BT_PROD] BTI_HL_Patch: read id ok");
            remote_id = rx_buffer[HL_CMD_RX_ID_INDEX] + 1;
            break;
		}
		else if(i == (9))
		{
            BT_TRACE("[BT_PROD] *****************BTI_HL_Patch: read id i=9  err" );
            BT_PASSERT("BTI_HL_Patch read id 10  times no ack"); 
            break; //false
		}
	}

get_hl_id_response:
	BT_TRACE("[BT_PROD] BTI_HL_Patch: RX_ACK = 0x%X, CHIP ID = 0x%2X, cmd_num=%d", 
	rx_buffer[HL_CMD_RX_ACK_INDEX], rx_buffer[HL_CMD_RX_ID_INDEX],cmd_num);	
	reset_cmd[6] = remote_id;
	reset_cmd[11] = hl_reset_cmd[11] - reset_cmd[6];

	//send reset
	UART_WriteDataSync(patch_uartport, &reset_cmd[1], reset_cmd[0]);
	SCI_Sleep(10); //wait sync 10ms

	UART_Close(patch_uartport);
	_BTI_UartOpen(patch_uartport, BAUD_115200);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_FALSE);
	UART_DumpPutChar(patch_uartport, HL_UART_BAUDRATE_SYNC_FLAG);
	SCI_Sleep(3); //wait sync 3ms

	
	uart_fifo_size=UART_GetRxFifoCnt(patch_uartport);//	uart_fifo_size= UART_PHY_GetRxFifoCnt(patch_uartport);
	if (uart_fifo_size > 0)
	{
        BT_PASSERT("BTI_HL_Patch UART Rx error!, uart_fifo_size = %d", uart_fifo_size); 
	}

	UART_Close(patch_uartport);
	_BTI_UartOpen2(patch_uartport, BAUD_115200);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_FALSE);
    //read CHIP ID
	for( i = 0; i < HL_CMD_MAX_ERR_COUNT ; i++)
	{
		UART_WriteDataSync(patch_uartport, &hl_id_cmd[1], hl_id_cmd[0]);
		SCI_Sleep(3);
		//UART_ReadDataSync (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN);
		UART_ReadData_ByFIFO_Count (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN, 3);

		if(HL_CMD_RX_ACK_OK == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
            BT_TRACE("[BT_PROD] BTI_HL_Patch: read id ok");
            break;
		}
		else if(i == (HL_CMD_MAX_ERR_COUNT - 1))
		{
            BT_TRACE("[BT_PROD] *****************BTI_HL_Patch: send the BD address  err" );
            BT_PASSERT("BTI_HL_Patch read id 3 times no ack"); 
            break; //false
		}
	}  
 
	BT_TRACE("[BT_PROD] BTI_HL_Patch: RX_ACK = 0x%X, CHIP ID = 0x%2X, cmd_num=%d", 
	rx_buffer[HL_CMD_RX_ACK_INDEX], rx_buffer[HL_CMD_RX_ID_INDEX],cmd_num);

	if (HL_CHIP_ID != rx_buffer[HL_CMD_RX_ID_INDEX])
    {
        BT_PASSERT("BTI_HL_Patch is not BL V%d Chip ,id is %d", HL_CHIP_VERSION, rx_buffer[HL_CMD_RX_ID_INDEX]);
    }

   
	//send the BD address
	for( i = 0; i < HL_CMD_MAX_ERR_COUNT ; i++)
	{
		UART_WriteDataSync(patch_uartport, &cmd[1], cmd[0]);
		SCI_Sleep(3);
		//UART_ReadDataSync (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN);
		UART_ReadData_ByFIFO_Count (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN, 3);

		if(HL_CMD_RX_ACK_OK == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
		     BT_TRACE("[BT_PROD] BTI_HL_Patch: send the BD address  ok");
		 	 break;
		}
		else if(i == (HL_CMD_MAX_ERR_COUNT - 1))
		{
		     BT_TRACE("[BT_PROD] *****************BTI_EarlyPatch: send the BD address  err" );
             BT_PASSERT("BTI_HL_Patch send the BD address 3 times err"); 
			 break; //false
		}
	}  
      
	//send patch file  send patch one by one
	i=0;
	err_count = 0;
	
	while(i< cmd_num && err_count < HL_CMD_MAX_ERR_COUNT)
	{
		UART_WriteDataSync(patch_uartport, &hl_patch_cmd[i][1], HL_CMD_TX_LEN(i));
          
		SCI_MEMSET(&rx_buffer[0], 0, HL_CMD_RX_ACK_LEN);
		trans_packet_time = (100 * (HL_CMD_TX_LEN(i)) + 1400)/1000;
		BT_TRACE("[BT_PROD] BTI_HL_Patch: trans_packet_time=%d", trans_packet_time);	
        
		SCI_Sleep(trans_packet_time);	

		do{	 

			read_count=0;	
			uart_fifo_size=UART_GetRxFifoCnt(patch_uartport);//	uart_fifo_size= UART_PHY_GetRxFifoCnt(patch_uartport);
			BT_TRACE("[BT_PROD] BTI_HL_Patch:uart_fifo_size =%d",uart_fifo_size);
				
			if(uart_fifo_size==2)
			{    
			  //normal case for 2 byte ack
				read_count=UART_ReadDataSync (patch_uartport, &rx_buffer[0],HL_CMD_RX_ACK_LEN);
			}
			else if(uart_fifo_size>2  && i< cmd_num-1)
			{
				//clear all count data and assert;
				if(uart_fifo_size < 12)
				{
					read_count=UART_ReadDataSync (patch_uartport, &rx_buffer_err[0], uart_fifo_size);

					for(j=0; j<uart_fifo_size ;j++)
					{
						BT_TRACE("[BT_PROD] **************** BTI_HL_Patch: err rx_buffer_err[%d]=0x%X", j,  rx_buffer_err[j]);
					}
				}
				
				SCI_Sleep(100);
                BT_PASSERT("BTI_HL_Patch hl_patch_cmd[%d] uart_fifo_size =%d", i, uart_fifo_size);
			}
			else  if(uart_fifo_size==3  && i==cmd_num-1)
			{
				//clear all count data;
				read_count=UART_ReadDataSync (patch_uartport, &rx_buffer[0], HL_CMD_RX_ACK_LEN);
				UART_ReadDataSync (patch_uartport, 	&rx_buffer_err[0], 1);
				BT_TRACE("[BT_PROD] *****************BTI_HL_Patch: err  rx_buffer[0]=0x%x, rx_buffer[1]=0x%X", 	rx_buffer[0], rx_buffer[1]);	
				BT_TRACE("[BT_PROD] *****************BTI_HL_Patch: err  rx_buffer_err=0x%X", 	rx_buffer_err[0]);	
				BT_TRACE("[BT_PROD] *****************BTI_HL_Patch: err  rx_buffer[0]=0x%x, rx_buffer[1]=0x%X", 	rx_buffer[0], rx_buffer[1]);	
				BT_TRACE("[BT_PROD] *****************BTI_HL_Patch: err  rx_buffer_err=0x%X", 	rx_buffer_err[0]);	
                //SCI_Sleep(100);
				//SCI_ASSERT(0);
			}
			
			else  if(uart_fifo_size==1  && i==cmd_num-1)
			{
                if(read_count_err==3 )
                {
                    //clear all count data and assert;
                    read_count=UART_ReadDataSync (patch_uartport, &rx_buffer[0], uart_fifo_size);
                    //SCI_ASSERT(0); //remove for debug
                }
			}
			else
			{			      
				SCI_Sleep(10);
			}

			BT_TRACE("[BT_PROD] BTI_HL_Patch: uart_fifo_size=%d", uart_fifo_size);	

			BT_TRACE("[BT_PROD] BTI_HL_Patch: i = %d, TX_ID = 0x%2X, RX_ACK = 0x%X, RX_ID= 0x%2X, read_count=%d, read_count_err=%d, err = %d", 
			i, hl_patch_cmd[i][HL_CMD_TX_ID_INDEX], 
			rx_buffer[HL_CMD_RX_ACK_INDEX],
			rx_buffer[HL_CMD_RX_ID_INDEX], read_count,read_count_err,err_count );
			read_count_err++;

		}while(( read_count==0 && read_count_err <5));

		read_count_err=0;

		// check ack
		if(HL_CMD_RX_ACK_OK == rx_buffer[HL_CMD_RX_ACK_INDEX] )
		{
			i++;           // next cmd
			err_count = 0; // reset error count
		}
		else
		{
			err_count++;
		}
	}

	UART_Close(patch_uartport);
	DPSLP_UART_EnableApbSleep(patch_uartport, SCI_TRUE);

	BT_TRACE("[BT_PROD] BTI_HL_Patch: close uart = %d, tick = %d", patch_uartport, SCI_GetTickCount() );    
	
	if(i < cmd_num)
	{
        SCI_Sleep(100);
        BT_PASSERT("BTI_HL_Patch hl_patch_cmd[%d] three times transfer error , no ack, total %d", i, cmd_num);
        return 0;
	}
	else
	{
		return 1;
	}
}

 

PUBLIC uint8 BTI_LaterPatch(void)
{
	return 	BTI_HL_Patch(hl_patch_cmd, sizeof(hl_patch_cmd)/sizeof(hl_patch_cmd[0]));
}

/*****************************************************************************/
//  Description:    BTI_EutPatch: FOR HL BT EUT
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint8 BTI_EutPatch(void)
{
	return 	BTI_HL_Patch(hl_patch_rf_cmd, sizeof(hl_patch_rf_cmd)/sizeof(hl_patch_rf_cmd[0]));
}

#endif

/*****************************************************************************/
//  Description:    This function is used to restore system clock about BT
//  Author:         
//  Note:           
/*****************************************************************************/
void BTI_RestoreSystemClk(uint8 type)
{
	  //SCI_TRACE_LOW:"[BT]BTI_RestoreSystemClk:type = 0x%X\r\n"
	  SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_210_112_2_18_0_29_4_584,(uint8*)"d", type);
	  switch(type)
	  {
	 case BT_IDLE_CLK:
	 {
	 	if(s_bt_idle_freq_handler != CHNG_FREQ_REQ_HANDLER_NULL)
	 	{
	  		CHNG_FREQ_RestoreARMClk(s_bt_idle_freq_handler);
	  		CHNG_FREQ_DeleteReqHandler(s_bt_idle_freq_handler);
	  		s_bt_idle_freq_handler = CHNG_FREQ_REQ_HANDLER_NULL;
	  		//SCI_TRACE_LOW:"[BT]BTI_RestoreSystemClk:Idle restore successful\r\n"
	  		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_220_112_2_18_0_29_4_585,(uint8*)"");
	  	}
	  	else
	  	{
	  		//SCI_TRACE_LOW:"[BT]BTI_RestoreSystemClk:Idle restore error\r\n"
	  		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_224_112_2_18_0_29_4_586,(uint8*)"");
	  	}
	  break;
	 }
	 case BT_FTP_CLK:
	 {
	 	if(s_bt_ftp_freq_handler != CHNG_FREQ_REQ_HANDLER_NULL)
	 	{
	  		CHNG_FREQ_RestoreARMClk(s_bt_ftp_freq_handler);
	  		CHNG_FREQ_DeleteReqHandler(s_bt_ftp_freq_handler);
	  		s_bt_ftp_freq_handler = CHNG_FREQ_REQ_HANDLER_NULL;
	  		//SCI_TRACE_LOW:"[BT]BTI_RestoreSystemClk:FTP restore successful\r\n"
	  		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_235_112_2_18_0_29_4_587,(uint8*)"");
	  	}
	  	else
	  	{
	  		//SCI_TRACE_LOW:"[BT]BTI_RestoreSystemClk:FTP restore error\r\n"
	  		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_239_112_2_18_0_29_4_588,(uint8*)"");
	  	}
	  break;
	 }
	 case BT_HFG_CLK:
	 {
	   	if(s_bt_hfg_freq_handler != CHNG_FREQ_REQ_HANDLER_NULL)
	 	{
	  		CHNG_FREQ_RestoreARMClk(s_bt_hfg_freq_handler);
	  		CHNG_FREQ_DeleteReqHandler(s_bt_hfg_freq_handler);
	  		s_bt_hfg_freq_handler = CHNG_FREQ_REQ_HANDLER_NULL;
	  		//SCI_TRACE_LOW:"[BT]BTI_RestoreSystemClk:HFG restore successful\r\n"
	  		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_250_112_2_18_0_29_4_589,(uint8*)"");
	  	}
	  	else
	  	{
	  		//SCI_TRACE_LOW:"[BT]BTI_RestoreSystemClk:HFG restore error\r\n"
	  		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_254_112_2_18_0_29_4_590,(uint8*)"");
	  	};
	  break;
	 }
	 case BT_A2DP_CLK:
	 {
	   	if(s_bt_a2dp_freq_handler != CHNG_FREQ_REQ_HANDLER_NULL)
	 	{
	  		CHNG_FREQ_RestoreARMClk(s_bt_a2dp_freq_handler);
	  		CHNG_FREQ_DeleteReqHandler(s_bt_a2dp_freq_handler);
	  		s_bt_a2dp_freq_handler = CHNG_FREQ_REQ_HANDLER_NULL;
	  		//SCI_TRACE_LOW:"[BT]BTI_RestoreSystemClk:A2DP restore successful\r\n"
	  		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_265_112_2_18_0_29_4_591,(uint8*)"");
	  	}
	  	else
	  	{
	  		//SCI_TRACE_LOW:"[BT]BTI_RestoreSystemClk:A2DP restore error\r\n"
	  		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_269_112_2_18_0_29_4_592,(uint8*)"");
	  	};
	  break;
	 }
	 default:
	 	//SCI_ASSERT(0);
	 	//SCI_TRACE_LOW:"[BT]<ERROR> BTI_RestoreSystemClk: NOT supported type\r\n"
	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_275_112_2_18_0_29_4_593,(uint8*)"");

	 break; 
	}
}

/*****************************************************************************/
//  Description:    This function is used to change system clock about BT
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC void BTI_ChangeSystemClk(uint8 type)
{
  //SCI_TRACE_LOW:"[BT]BTI_ChangeSystemClk:type = 0x%X\r\n"
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_286_112_2_18_0_29_4_594,(uint8*)"d", type);
  switch(type)
  {
 case BT_IDLE_CLK:
	if(CHNG_FREQ_REQ_HANDLER_NULL == s_bt_idle_freq_handler)
	{
		s_bt_idle_freq_handler = CHNG_FREQ_GetReqHandler("BT_IDLE_FREQ");
		CHNG_FREQ_SetArmClk(s_bt_idle_freq_handler, FREQ_INDEX_BT_IDLE); 
		SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_bt_idle_freq_handler); /*assert verified*/
	 	//SCI_TRACE_LOW:"[BT]BTI_ChangeSystemClk:IDLE's clolck change successful\r\n"
	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_295_112_2_18_0_29_4_595,(uint8*)"");
	}
	else
	{
		//SCI_TRACE_LOW:"[BT]BTI_ChangeSystemClk:IDLE changed clolck error\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_299_112_2_18_0_29_4_596,(uint8*)"");
	}
	break;
 case BT_FTP_CLK:
 	 if(CHNG_FREQ_REQ_HANDLER_NULL == s_bt_ftp_freq_handler)
 	 {
        s_bt_ftp_freq_handler = CHNG_FREQ_GetReqHandler("BT_FTP_FREQ");
	 	CHNG_FREQ_SetArmClk(s_bt_ftp_freq_handler, FREQ_INDEX_BT_FTP); 
	 	SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_bt_ftp_freq_handler); /*assert verified*/
	 	//SCI_TRACE_LOW:"[BT]BTI_ChangeSystemClk:FTP's clolck change successful\r\n"
	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_308_112_2_18_0_29_4_597,(uint8*)"");
	 }
	 else
	 {
	 	//SCI_TRACE_LOW:"[BT]BTI_ChangeSystemClk:FTP changed clolck error\r\n"
	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_312_112_2_18_0_29_4_598,(uint8*)"");
	 }
	 break;
 case BT_HFG_CLK:
	if(CHNG_FREQ_REQ_HANDLER_NULL == s_bt_hfg_freq_handler)
	{
        s_bt_hfg_freq_handler = CHNG_FREQ_GetReqHandler("BT_HFG_FREQ");	
		CHNG_FREQ_SetArmClk(s_bt_hfg_freq_handler, FREQ_INDEX_BT_HFG); 
		SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_bt_hfg_freq_handler); /*assert verified*/
		//SCI_TRACE_LOW:"[BT]BTI_ChangeSystemClk:HFG's clolck change successful\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_321_112_2_18_0_29_4_599,(uint8*)"");
	}
	else
	{
		//SCI_TRACE_LOW:"[BT]BTI_ChangeSystemClk:HFG changed clolck error\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_325_112_2_18_0_29_4_600,(uint8*)"");
	}
	break;
 case BT_A2DP_CLK:
	 if(CHNG_FREQ_REQ_HANDLER_NULL == s_bt_a2dp_freq_handler)
 	 {
        s_bt_a2dp_freq_handler = CHNG_FREQ_GetReqHandler("BT_A2DP_FREQ");	 	 
	 	CHNG_FREQ_SetArmClk(s_bt_a2dp_freq_handler, FREQ_INDEX_BT_A2DP); 
	 	SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_bt_a2dp_freq_handler); /*assert verified*/
	 	//SCI_TRACE_LOW:"[BT]BTI_ChangeSystemClk:A2DP's clolck change successful\r\n"
	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_334_112_2_18_0_29_4_601,(uint8*)"");
	 }
	 else
	 {
	 	//SCI_TRACE_LOW:"[BT]BTI_ChangeSystemClk:A2DP changed clolck error\r\n"
	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_338_112_2_18_0_29_4_602,(uint8*)"");
	 }
	break;
 default:
	//SCI_ASSERT(0);
	//SCI_TRACE_LOW:"[BT]<ERROR> BTI_ChangeSystemClk: NOT supported type\r\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_PROD_343_112_2_18_0_29_4_603,(uint8*)"");
  break;      
  }
}

/*****************************************************************************/
//  Description:    This function is used to save bd address
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC void BTI_SaveBdAddr(const BT_ADDRESS *addr)
{
    BT_NV_PARAM    nv_param;
    NVITEM_ERROR_E status;
    
    SCI_ASSERT(addr); /*assert verified*/
    
    status = EFS_NvitemRead(NV_BT_CONFIG, sizeof nv_param, (uint8*)&nv_param);
    SCI_ASSERT(NVERR_NONE == status); /*assert verified*/
    nv_param.bd_addr = *addr;
    status = NVITEM_UpdateCali(NV_BT_CONFIG, sizeof nv_param, (uint8*)&nv_param);
    SCI_ASSERT(NVERR_NONE == status); /*assert verified*/
}

/*****************************************************************************/
//  Description:    This function is used to save xtal ftrim
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
void BTI_SaveXtalFtrim(uint16 ftrim)
{
    BT_NV_PARAM    nv_param;
    NVITEM_ERROR_E status;
    
    status = EFS_NvitemRead(NV_BT_CONFIG, sizeof nv_param, (uint8*)&nv_param);
    SCI_ASSERT(NVERR_NONE == status); /*assert verified*/
    nv_param.xtal_dac = ftrim;
    status = NVITEM_UpdateCali(NV_BT_CONFIG, sizeof nv_param, (uint8*)&nv_param);
    SCI_ASSERT(NVERR_NONE == status); /*assert verified*/
}

/*****************************************************************************/
//  Description:    This function is used to get BT configure information
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC void BT_GetConfigInfo(BT_CONFIG *config)
{ 
    uint32 status = NVERR_NONE;
    BT_NV_PARAM  bt_nv_param;
#if 1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
    BT_PSKEY_NV_CONFIG_T bt_sprd_nv_param;
#endif

    SCI_ASSERT( NULL != config); /*assert verified*/

    status = EFS_NvitemRead(NV_BT_CONFIG, sizeof(bt_nv_param), (uint8*)&bt_nv_param);
    SCI_ASSERT(NVERR_NONE == status); /*assert verified*/

    BT_GetProdConfig(config);
	
    /*config bt address*/
    SCI_MEMCPY((uint8*)&config->bd_addr, (uint8*)&bt_nv_param.bd_addr, sizeof(BT_ADDRESS));
    config->xtal_dac = bt_nv_param.xtal_dac;
 
    SCI_MEMCPY(config->bt_file_dir, BT_FILE_DIR, sizeof BT_FILE_DIR);
    SCI_MEMCPY(config->local_name, BT_DEFAULT_LOCAL_NAME, sizeof BT_DEFAULT_LOCAL_NAME);

#if 1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
    status = EFS_NvitemRead(NV_BT_SPRD, sizeof(bt_sprd_nv_param), (uint8*)&bt_sprd_nv_param);
    SCI_ASSERT(NVERR_NONE == status);/*assert verified*/

    config->host_nv_ctrl[0] = bt_sprd_nv_param.host_reserved[0];
    config->host_nv_ctrl[1] = bt_sprd_nv_param.host_reserved[1];
#else
#if defined(CHIP_VER_6531EFM)
    status = EFS_NvitemRead(NV_BT_SPRD, sizeof(bt_sprd_nv_param), (uint8*)&bt_sprd_nv_param);
    SCI_ASSERT(NVERR_NONE == status);/*assert verified*/

    config->host_nv_ctrl[0] = bt_sprd_nv_param.host_reserved[0];
    config->host_nv_ctrl[1] = bt_sprd_nv_param.host_reserved[1];
#else
#ifdef BT_DIALER_SUPPORT
    config->host_nv_ctrl[0] = 0x73;
#else
    config->host_nv_ctrl[0] = 0x53;
#endif
    config->host_nv_ctrl[1] = 0;
#endif
#endif
}

PUBLIC char* BTI_Get_Ctrlversion()
{
	return controller_version;	
}
#if 1 //defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910)  || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
extern BOOLEAN SLEEP_BT_PowerDomain_Set(BOOLEAN is_on);//define sleep_phy_sc6531.h
BOOLEAN BTI_Wakeup_BT_control()
{
	//return SLEEP_BT_PowerDomain_Set(TRUE);
	return TRUE; //temply wuding
}
BOOLEAN BTI_Sleep_BT_control()
{
	//return SLEEP_BT_PowerDomain_Set(FALSE);
	return TRUE; //temply wuding
}

//extern unsigned int BT_GetRamCode(unsigned char** p_bt_ram_code_ptr); // be defined in connectivity\BT\BT_PS_V3X\BT_RAM_CODE_6531.C
extern unsigned int BT_GetIRAMCode(unsigned char **p_bt_ram_code_ptr);
extern unsigned int BT_GetPSRAMCode(unsigned char **p_bt_ram_code_ptr);

#ifdef BT_NONSIG_SUPPORT
#ifdef CHIP_VER_6531
extern unsigned int BT_GetRamCode1(unsigned char** p_bt_ram_code_ptr); // be defined in connectivity\BT\BT_PS_V3X\BT_RAM_CODE_6531.C
extern unsigned int BT_GetRamCode2(unsigned char** p_bt_ram_code_ptr); // be defined in connectivity\BT\BT_PS_V3X\BT_RAM_CODE_6531.C

extern CONST uint32    g_sram_start;
extern CONST uint32	   g_sram_size;
#define  BT_AREA_SIZE             0x40000

#ifdef VM_SUPPORT
#ifdef VM_TABLE_CONFIG_TINY
#define  MAX_PHY_SYS_MEM_SIZE       (112*1024)   //include MMU Page Table + code swap memory
#define  MAX_PHY_MMU_MEM_SIZE       (MAX_PHY_SYS_MEM_SIZE)
#elif defined(VM_TABLE_CONFIG_SMALL)
#define  MAX_PHY_SYS_MEM_SIZE       (256*1024)   //include MMU Page Table + code swap memory
#define  MAX_PHY_MMU_MEM_SIZE       (MAX_PHY_SYS_MEM_SIZE)
#else
#define  MAX_PHY_SYS_MEM_SIZE       (512*1024)   //include MMU Page Table + code swap memory
#define  MAX_PHY_RES_MEM_SIZE       (512*1024)
#define  MAX_PHY_MMU_MEM_SIZE       (MAX_PHY_SYS_MEM_SIZE + MAX_PHY_RES_MEM_SIZE)
#endif //VM_TABLE_CONFIG_TINY
#else
#define  MAX_PHY_MMU_MEM_SIZE   0
#endif //VM_SUPPORT

uint32 bt_sram_start;
#endif
#endif  //BT_NONSIG_SUPPORT

#if 1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)|| defined(CHIP_VER_UWS6121E)
extern uint8 *bt_code_addr;
extern uint32 bt_code_size;
#endif


void  BTI_Load_BT_Control(void)
{
    uint32 i;
    uint32 ECO_8910_chip_flag = 0;
    //uint8 *ptr;
    BT_NV_PARAM    bt_nv_param;
#if 1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
    BT_PSKEY_NV_CONFIG_T bt_sprd_nv_param;
    uint16 bt_log_set = 0;
    //static uint16 bt_calibration_flag = 1;
    //uint32 r_cali_data[3] = {0};
#endif
    NVITEM_ERROR_E status;
    //uint32 chip_id = 0;
    unsigned int *bt_ram_code_ptr  = NULL;
    unsigned int   bt_ram_code_size = 0;

#if 1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
    //Copy the BT controller bin into RAM.
    bt_ram_code_size = BT_GetPSRAMCode(&bt_ram_code_ptr);

    SCI_TRACE_LOW("btprod:BTI_Load_bt_controller addr=0x%x, size=%d",bt_code_addr,bt_ram_code_size);

    bt_code_addr[0] = 0xFF;
    bt_code_addr[1] = 0xFF;
    bt_code_addr[2] = 0xFF;
    bt_code_addr[3] = 0xFF;
    SCI_MEMCPY((uint32*)(bt_code_addr+4), bt_ram_code_ptr, bt_ram_code_size);

#if 1
    status = EFS_NvitemRead(NV_BT_CONFIG, sizeof(bt_nv_param), (uint8*)&bt_nv_param);
    SCI_ASSERT(NVERR_NONE == status);/*assert verified*/
    
    status = EFS_NvitemRead(NV_BT_SPRD, sizeof(bt_sprd_nv_param), (uint8*)&bt_sprd_nv_param);
    SCI_ASSERT(NVERR_NONE == status);/*assert verified*/

    /* set SSP debug mode */
    if(FALSE != BT_GetSSPDebugMode())
    {
        bt_sprd_nv_param.feature_set[6] &= ~0x08;
    }

{
        uint8    *p;
        uint8     p_buf[PSKEY_NV_SIZE+4+4] = {0};//PSKEY_SIZE is NV BT_SPRD size, 4 is uint16 bt_log_set and uint16 bt_calibration_flag
        p = p_buf;

        UINT8_TO_STREAM(p, bt_sprd_nv_param.g_dbg_source_sink_syn_test_data);
        if (BT_TESTMODE_NONE != BT_GetTestMode())
        {
            //Controller should not SLEEP in the test mode
            bt_sprd_nv_param.g_sys_sleep_in_standby_supported = 0;
            bt_sprd_nv_param.g_sys_sleep_master_supported = 0;
            bt_sprd_nv_param.g_sys_sleep_slave_supported = 0;
        }
        UINT8_TO_STREAM(p, bt_sprd_nv_param.g_sys_sleep_in_standby_supported);
        UINT8_TO_STREAM(p, bt_sprd_nv_param.g_sys_sleep_master_supported);
        UINT8_TO_STREAM(p, bt_sprd_nv_param.g_sys_sleep_slave_supported);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.default_ahb_clk);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.device_class);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.win_ext);

        for (i = 0; i < 6; i++) 
        {
            UINT32_TO_STREAM(p, bt_sprd_nv_param.g_aGainValue[i]);
        }
        
        for (i = 0; i < 5; i++) 
        {
            UINT32_TO_STREAM(p, bt_sprd_nv_param.g_aPowerValue[i]);
        }
                
        for (i = 0; i < 16; i++) 
        {
            UINT8_TO_STREAM(p, bt_sprd_nv_param.feature_set[i]);
        }
        
        for (i = 0; i < 6; i++) 
        {
            UINT8_TO_STREAM(p, bt_nv_param.bd_addr.addr[i]);
        }
    
        UINT8_TO_STREAM(p, bt_sprd_nv_param.g_sys_sco_transmit_mode);
        UINT8_TO_STREAM(p, bt_sprd_nv_param.g_sys_uart0_communication_supported);
        UINT8_TO_STREAM(p, bt_sprd_nv_param.edr_tx_edr_delay);
        UINT8_TO_STREAM(p, bt_sprd_nv_param.edr_rx_edr_delay);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.g_wbs_nv_117);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.is_wdg_supported);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.share_memo_rx_base_addr);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.share_memo_tx_base_addr);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.share_memo_tx_packet_num_addr);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.share_memo_tx_data_base_addr);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.g_PrintLevel);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.share_memo_tx_block_length);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.share_memo_rx_block_length);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.share_memo_tx_water_mark);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.share_memo_tx_timeout_value);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.uart_rx_watermark);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.uart_flow_control_thld);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.comp_id);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.pcm_clk_divd);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.g_nbs_nv_118);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.br_edr_diff_reserved);
        UINT16_TO_STREAM(p, bt_sprd_nv_param.br_edr_powercontrol_flag);

        for (i = 0; i < 8; i++)
        {
            UINT32_TO_STREAM(p, bt_sprd_nv_param.gain_br_channel_power[i]);
        }
    
        for (i = 0; i < 8; i++)
        {
            UINT32_TO_STREAM(p, bt_sprd_nv_param.gain_edr_channel_power[i]);
        }

        UINT32_TO_STREAM(p, bt_sprd_nv_param.g_aLEPowerControlFlag);
        for (i = 0; i < 8; i++)
        {
            UINT16_TO_STREAM(p, bt_sprd_nv_param.g_aLEChannelpwrvalue[i]);
        }

        for (i = 0; i < 2; i++) 
        {
            UINT32_TO_STREAM(p, bt_sprd_nv_param.host_reserved[i]);
        }

        UINT32_TO_STREAM(p, bt_sprd_nv_param.config0);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.config1);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.config2);
        UINT32_TO_STREAM(p, bt_sprd_nv_param.config3);

        //set calibration flag
        UINT16_TO_STREAM(p, 0);

        //The bt_log_set is NOT in NV_BT_SPRD. But it is in the Host and Controller PSK structure.
        //0 -- no log;
        // 1 -- debug host (coolwatcher);
        // 2 -- AP output log;
#ifdef TRACE_INFO_SUPPORT
        bt_log_set = 2;
        if (BT_TESTMODE_NONSIG == BT_GetTestMode())
        {
            bt_log_set = 0;//no BT log in non-sig test mode (NPI)
        }
#else
        bt_log_set = 0;//No log
#endif
        UINT16_TO_STREAM(p, bt_log_set);

        bt_sprd_nv_param.reserved1[0] = 0;
        bt_sprd_nv_param.reserved1[1] = 0; 
        for (i = 0; i < 4; i++)
        {
            UINT32_TO_STREAM(p, bt_sprd_nv_param.reserved1[i]);
        }

        //The ECO_8910_chip_flag is NOT in NV_BT_SPRD. But it is in the Host and Controller PSK structure.
        ECO_8910_chip_flag = CHIP_PHY_GetECOFlag();
        if (1 == ECO_8910_chip_flag)//  1 means old 8910 chip
        {
            ECO_8910_chip_flag = 0;
        }
        else// 2 means ECO 8910 chip
        {
            ECO_8910_chip_flag = 1;
        }
        UINT32_TO_STREAM(p, ECO_8910_chip_flag);

        //bt_sprd_nv_param.reserved2[0] = 1;//You can enable Bluetooth BR/EDR power config by NV after 20210107 with a new patch, or just keep default.
        for (i = 0; i < 3; i++)
        {
            UINT16_TO_STREAM(p, bt_sprd_nv_param.reserved2[i]);
        }

        SCI_MEMCPY((uint8*)(bt_code_addr+0x200), (uint8*)p_buf, (PSKEY_NV_SIZE+4+4));
        //SCI_TRACE_LOW("BTYWD copy done 0x%x, sizeof(%d) chip=0x%x",(bt_code_addr+0x200),sizeof(bt_sprd_nv_param),ECO_8910_chip_flag);
    }
#endif

    //RESET and Boot up the RISC-V
    BTI_Rst_Bootup(bt_code_addr+4);

#endif
}//lint !e529

void BTI_Wait_BTcontol_Load_Finish()
{
#if 0
    uint32 test_blue_debug_ticket = 0;
    uint32 test_blue_debug_ticket_count = 0;
    uint32 test_blue_debug_ticket_last = 0;
    uint32 controller_debug_time = 0;

    test_blue_debug_ticket = SCI_GetTickCount(); 
    while(psharemem->cont_host_hand == 0)//wait btcontroller set load flag to 1
    {
        SCI_Sleep(1);
    }
    //*(uint8 *)0x10100140 = (uint8)0;
    psharemem->cont_host_hand = 0;
    controller_debug_time = SCI_GetTickCount();
    psharemem->host_sys_timer = controller_debug_time;

	/*
	SCI_MEMCPY((uint8 *)controller_version ,(uint8*)(psharemem->controller_version),8);
	SCI_TRACE_LOW("btprod:controller_version %c%c%c%c%c%c%c%c",
		          controller_version[0],
		          controller_version[1],
		          controller_version[2],
		          controller_version[3],
		          controller_version[4],
		          controller_version[5],
		          controller_version[6],
		          controller_version[7]);
    */
    controller_version = (char*)(psharemem->controller_version);
    SCI_TRACE_LOW("btprod:controller_version %s", controller_version);
    SCI_TRACE_LOW("[BT_NV]debug_time:%d,%d",psharemem->host_sys_timer,controller_debug_time);
    test_blue_debug_ticket_last = SCI_GetTickCount(); 
	
    test_blue_debug_ticket_count = test_blue_debug_ticket_last - test_blue_debug_ticket;
    SCI_TRACE_LOW("[BT]Load controll expire:%d", test_blue_debug_ticket_count);
#endif
    return;
}
#endif

/*****************************************************************************/
//  Description:    This function is used by RISC-V to notify AP sleep   
//  Author:         
//  Note:           
/*****************************************************************************/
void WCN_BT_Sleep_Callback(uint32 sleeptime)
{
    uint32 temp_reg = 0;
    uint32 temp_reg_index = 0;

    g_bt_sleep_time = sleeptime;
    g_bt_sleep_time_ap32k = hwp_idle->IDL_32K_REF;

    SCI_BT_EnableDeepSleep(1);//ENABLE_DEEP_SLEEP

    //debug info
    s_WCN_BT_Sleep_Callback++;
    s_WCN_BT_Sleep_Callback_time[s_WCN_BT_Sleep_Callback_time_index] = g_bt_sleep_time_ap32k;
    s_WCN_BT_Sleep_Callback_time_index++;
    if (s_WCN_BT_Sleep_Callback_time_index >= 10)
    {
        s_WCN_BT_Sleep_Callback_time_index=0;
    }
    //debug info

    //We should make sure the WCN_OSC_EN line is low, which means it already enter sleep.
    temp_reg = hwp_pwrctrl->WCN_LPS;
    while(CP_PWRCTRL_WCN2SYS_OSC_EN == (temp_reg & CP_PWRCTRL_WCN2SYS_OSC_EN))
    {
        //delay a little moment for WCN_OSC_EN line low
        temp_reg_index++;
        if (temp_reg_index > 80)//I record max is 52
        {
            s_WCN_BT_Sleep_Callback_wcnoscen_high++;//debug info , count it
            return;//avoid dead lock
        }
        temp_reg = hwp_pwrctrl->WCN_LPS;
    }

    return;
}

/*****************************************************************************/
//  Description:    This function is used by RISC-V to notify AP wakeup   
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC ISR_EXE_T WCN_AWAKE_IrqHandler(uint32 num)
{
    uint32 reg = 0;

    s_WCN_AWAKE_IrqHandler_enter_time = hwp_idle->IDL_32K_REF;//debug info

    reg = hwp_sysCtrl->Cfg_MISC1_CFG;
    reg &= ~SYS_CTRL_WCN_OSC_EN_CTRL;//Forceto give the WCN clock to access the WCN register
    hwp_sysCtrl->Cfg_MISC1_CFG = reg;

    reg = *(volatile uint32*)0x15000028;
    //clear this interrupt
    reg |= BIT_0;
    *(volatile uint32*)0x15000028 = reg;

    reg = hwp_sysCtrl->Cfg_MISC1_CFG;
    reg |= SYS_CTRL_WCN_OSC_EN_CTRL;//close the force.
    hwp_sysCtrl->Cfg_MISC1_CFG = reg;

    s_WCN_AWAKE_IrqHandler_exit_time = hwp_idle->IDL_32K_REF;//debug info

    g_uart_wait_bt_wakeup_cfm = FALSE;

    SCI_BT_EnableDeepSleep(0);//DISABLE_DEEP_SLEEP

    g_bt_sleep_time = 0;
    g_bt_sleep_time_ap32k = 0;

    //debug info
    if (s_WCN_AWAKE_IrqHandler_exit_time > s_WCN_AWAKE_IrqHandler_enter_time)
    {
        if ((s_WCN_AWAKE_IrqHandler_exit_time - s_WCN_AWAKE_IrqHandler_enter_time) > 100)
        {
            s_WCN_AWAKE_IrqHandler_overtime_times++;
#if 0
            //Stop the AP
            SCI_ASSERT(0);//在BT wakup 中断处理函数 里花了太久时间,主动assert.
#endif
        }
    }
    s_WCN_AWAKE_IrqHandler++;
    s_WCN_AWAKE_IrqHandler_time[s_WCN_AWAKE_IrqHandler_time_index]=s_WCN_AWAKE_IrqHandler_enter_time;
    s_WCN_AWAKE_IrqHandler_time_index++;
    if (s_WCN_AWAKE_IrqHandler_time_index>=10)
    {
        s_WCN_AWAKE_IrqHandler_time_index=0;
    }
    //debug info

    return ISR_DONE;
}

/*****************************************************************************/
//  Description:    This function is used to start up BT chip    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC void BTI_StartupChip(void)
{
#if !defined(MOS_SUPPORT)
    uint32 status = 0;

    s_WCN_BT_Sleep_Callback=0;//debug info
    s_WCN_AWAKE_IrqHandler=0;//debug info
    s_WCN_AWAKE_IrqHandler_overtime_times=0;//debug info
    s_WCN_AWAKE_IrqHandler_enter_time=0;//debug info
    s_WCN_AWAKE_IrqHandler_exit_time=0;//debug info
    s_WCN_BT_Sleep_Callback_time_index=0;//debug info
    s_WCN_BT_Sleep_Callback_wcnoscen_high=0;//debug info
    s_WCN_AWAKE_IrqHandler_time_index=0;//debug info

    g_bt_sleep_time = 0;
    g_bt_sleep_time_ap32k = 0;

    SCI_TRACE_LOW("btprod:BTI_StartupChip");

#if 1//defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
    SCI_BT_EnableDeepSleep(0);//DISABLE_DEEP_SLEEP

    status = ISR_RegHandler(TB_WCN_AWAKE_INT, (TB_ISR)WCN_AWAKE_IrqHandler); 
    if ((TB_SUCCESS == status) || (TB_ISR_ASSIGNED == status))
    {
        /*enable wcn bt awake int*/
        CHIPDRV_EnableIRQINT(TB_WCN_AWAKE_INT);
    }

    BTI_Wakeup_BT_control();

    //Copy RAM, and bootup RISC-V
    BTI_Load_BT_Control();

    //Shake-hand with RISC-V, if we need.
    BTI_Wait_BTcontol_Load_Finish();

    //UART driver ready
    g_curr_uart_bt_use = 1;
    UartDrv_StopApUart();
    hwp_sysCtrl->Cfg_MISC_CFG &= ~(SYS_CTRL_WCN_UART_OUT_SEL | SYS_CTRL_AP_UART_OUT_SEL);

    UartDrv_Configure(BT_BAUD_3250000);//In fact, 8910's BT_BAUD_3250000 is 3000000.
    UartDrv_Start();

#elif defined (CHIP_VER_6531)
    BTI_Wakeup_BT_control();
    BTI_Load_BT_Control();
    BTI_Wait_BTcontol_Load_Finish();

    UartDrv_Configure(BT_BAUD_3250000);
    UartDrv_Start();
#else
    GPIO_SetBtReset(SCI_TRUE);
    SCI_Sleep(4);  
    GPIO_SetBtReset(SCI_FALSE);
    SCI_Sleep(10);    
    GPIO_SetBtReset(SCI_TRUE);

    //s_bt_power_status is not ACCESSED!!!
    //s_bt_power_status = SCI_TRUE;
    SCI_Sleep(8);
    #ifdef BT_HL_PATCH_SUPPORT
    BTI_LaterPatch();
    //BTI_EarlyPatch();
    #endif
    #ifdef EXSTERN_WIFI_CO_EXIST
    Set_Coexist_wifi_BT(TRUE);
    Coexist_Attach_start();
    #endif

    UartDrv_Configure(BT_BAUD_3250000);
    UartDrv_Start();
#endif 
#endif
}

PUBLIC BT_CHIP_ID_T BTI_BtChipId(void)
{   
    #ifdef BT_CHIP_RDA
        return BT_CHIP_ID_RDA;
    #elif defined(BT_CHIP_BEKEN)
        return BT_CHIP_ID_BEKEN;
    #else
        return BT_CHIP_ID_NULL;
    #endif 
}

/*****************************************************************************/
//  Description:    This function is used to shut down BT chip
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC void BTI_ShutdownChip(void)
{
    controller_version = NULL;
#if 1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)

    BTI_Sleep_BT_control();

#elif defined(CHIP_VER_6531EFM)
    //psharemem->rf_powerdown = 1;
    *(uint32*)APB_INT_SET_CLR0 = BIT_6; //interupt to bt controller 
    //while(psharemem->rf_powerdown == 1)
    //{
        //SCI_Sleep(1);
    //}

    //Set ARM9_MTX
    *(volatile uint32 *)(0x52700200) |= BIT_6;

    *(volatile uint32*)ARM7_TOP_CTL1 |= 0x8001c;/*remapbit0  iram0, */	
    *(volatile uint32*)APB_EB0_CLR = 0x21;
    BTI_Sleep_BT_control();

    *(volatile uint32*)FM_RF1 &= ~FM_BB_BTRF_ENB;
    *(volatile uint32*)CLK_EN &= ~(CLK_RF_BT_26M_EN |CLK_BT_RF_CTL_EN |CLK_BT_LPO_EN);

    Shm_Close();
    ANA_REG_OR(ANA_LDO_BTPA_REG0, LDO_BTPA_PD);
    //ANA_REG_OR (ANA_SLP_LDO_PD_CTRL0,SLP_LDOBTPA_PD_EN);

    /*begin add cm4 check bug:620209*/
    //SIO_BTOpenCheckCm4Status(SCI_FALSE);
    /*end add cm4 check bug:620209*/   
#elif defined (CHIP_VER_6531)
	 psharemem->rf_powerdown= 1;
	//*(uint32*)(0x8B000160) |= BIT_6; //interupt to bt controller 
	*(uint32*)APB_INT_SET_CLR0 = BIT_6; //interupt to bt controller 
	 while(psharemem->rf_powerdown == 1)
	 {
		SCI_Sleep(1);
	 }
   *(volatile uint32*)ARM7_TOP_CTL1 |=0x1E;/*remapbit0  iram0, */	
   *(volatile uint32*)APB_EB0_CLR  = 0x21;//close ARM7
    BTI_Sleep_BT_control();
#else
    GPIO_SetBtReset(SCI_FALSE);

#ifdef EXSTERN_WIFI_CO_EXIST
    Coexist_End_Attach();
    Set_Coexist_wifi_BT(FALSE);
#endif
#endif


#if defined(CHIP_VER_6531EFM)
#else
    UartDrv_Stop();
#if  1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
#if 0  //bug1444039 as port=0xff while assert when close BT.Now,no need to do this,so delete it
    SIO_ReInitWithBtUartShutDown();
    hwp_sysCtrl->Cfg_MISC_CFG &= ~(SYS_CTRL_WCN_UART_OUT_SEL | SYS_CTRL_AP_UART_OUT_SEL);
    hwp_sysCtrl->Cfg_MISC_CFG |= SYS_CTRL_AP_UART_OUT_SEL;
#endif
    g_curr_uart_bt_use = 0;

    //WCN Clock Off
#if 0
    if (POWER_IsCalibrationResetMode())
    {
        //RPC interface cannot be used in Calibration Mode
        *((volatile unsigned long *)0x500304c8) = 0x0;//ANA_ctrl_reg32=0; pmic 26M clock disable
    }
    else
    {
        aud_ClrBtClk();//ANA_ctrl_reg32=0; pmic 26M clock disable
    }
    hwp_sysreg->RF_ANA_26M_CTRL_CLR = 0x100;//wcn 26M clock disable
#endif
    PMIC_26M_close(PMIC_26M_USED_BY_WCN);

    //WCN RF Power Off
    LDO_TurnOffLDO(LDO_LDO_BTPHY);

    //WCN Power Off
    hwp_pwrctrl->BTFM_PWR_CTRL = CP_PWRCTRL_BTFM_POFF;

    SCI_Sleep(100);
#endif

    g_bt_sleep_time = 0;
    g_bt_sleep_time_ap32k = 0;
    SCI_BT_EnableDeepSleep(1);//ENABLE_DEEP_SLEEP

    s_WCN_BT_Sleep_Callback=0;//debug info
    s_WCN_AWAKE_IrqHandler=0;//debug info
    s_WCN_AWAKE_IrqHandler_overtime_times=0;//debug info
    s_WCN_AWAKE_IrqHandler_enter_time=0;//debug info
    s_WCN_AWAKE_IrqHandler_exit_time=0;//debug info
    s_WCN_BT_Sleep_Callback_time_index=0;//debug info
    s_WCN_BT_Sleep_Callback_wcnoscen_high=0;//debug info
    SCI_MEMSET(&s_WCN_BT_Sleep_Callback_time[0],0,(10*sizeof(uint32)));//debug info
    s_WCN_AWAKE_IrqHandler_time_index=0;//debug info
    SCI_MEMSET(&s_WCN_AWAKE_IrqHandler_time[0],0,(10*sizeof(uint32)));//debug info
    s_bt_sleep_enter_time_1_index=0;//debug info
    SCI_MEMSET(&s_bt_sleep_enter_time_1[0],0,(10*sizeof(uint32)));//debug info
    s_bt_sleep_enter_time_2_index=0;//debug info
    SCI_MEMSET(&s_bt_sleep_enter_time_2[0],0,(10*sizeof(uint32)));//debug info
    s_bt_sleep_enter_time_3_index=0;//debug info
    SCI_MEMSET(&s_bt_sleep_enter_time_3[0],0,(10*sizeof(uint32)));//debug info
    s_bt_sleep_enter_time_4_index=0;//debug info
    SCI_MEMSET(&s_bt_sleep_enter_time_4[0],0,(10*sizeof(uint32)));//debug info
    s_bt_current_ap_32k=0;//debug info
    s_bt_not_wakeup_error_times=0;//debug info
#endif
}


#ifndef WIFI_VENDOR_CSR /*library co-exist wifi and bluetooth*/
void *CsrPmemAllocExt(size_t size, const char* file, const int line)
{    
    return SCI_Malloc(size, __FILE__, __LINE__);
}

void CsrBt2WiFiSchedMessagePut(unsigned short  a, unsigned short b, void *ptr)
{    
    if (ptr != NULL)
    SCI_FREE(ptr);
}
#endif


void BTI_GSMTable_WriteShareMemory(const uint8 *buffer, uint8 length)
{
#if 1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
    //nothing
#else
#if defined(CHIP_VER_6531EFM)
    SCI_MEMCPY(psharemem->reserved,&buffer[4],13);
#endif
#endif
}
#endif

BOOLEAN BTI_GSMTable_GetSchedStatus(void)
{
    BOOLEAN ret = FALSE;
#if 1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
    //nothing
#else
#if defined(CHIP_VER_6531EFM)
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
    ret = OS_GetSchedStatus();
#endif
#endif
#endif

    return ret;
}

void BTI_GSMTable_SendMsgToSchedTask(uint32 event, void *msg_ptr)
{
#if 1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
    //nothing
#else
    //This function will not be called if BTI_GSMTable_GetSchedStatus return FALSE
#if defined(CHIP_VER_6531EFM)
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
    OS_SendMsgToSchedTask(event, msg_ptr);
#endif
#endif
#endif
}

BOOLEAN BTI_IsSystemBusy(void)
{
    BOOLEAN ret = FALSE;

#if  1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
    //nothing
#else
#if defined(CHIP_VER_6531EFM)
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
#ifdef MODEM_PLATFORM
    ret = FALSE;
#else
    //Is the music/video playing?
    //if ((MMIMP3_AUDIO_PLAYING == MMIMP3_GetAudioState()) || (MMIAPIVP_IsLiving()))
    if ((1 == MMIMP3_GetAudioState()) || (MMIAPIVP_IsLiving()))
    {
        ret = TRUE;
    }
#endif
#endif
#endif
#endif

    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of bt_prod.c

/*lint +e785 +e415 +e64 */
