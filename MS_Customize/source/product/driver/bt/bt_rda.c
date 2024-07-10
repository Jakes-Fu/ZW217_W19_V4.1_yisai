/*************************************************************************
 *
 * MODULE NAME:    bt_rda.c
 * PROJECT CODE:   rda5868
 * DESCRIPTION:    rda5868 initialization file.
 * MAINTAINER:     lugongyu
 * CREATION DATE:  08/11/09
 * VERSION:        1.9
 * LICENSE:
 *     This source code is copyright (c) 2008-2009 rda.
 *     All rights reserved.
 *
 * NOTES TO USERS:
 *   None.
 *				 
 *************************************************************************/


#include "ms_customize_trc.h"
#include "bt_rda.h"

extern void RDA5868_Initialization(void);//defined in the platform code

//默认状态下RDA5400不支持连续地址读写，所以rda5400_write_data和rda5400_read_data函数的datalen参数总为1
//默认状态下RDA5868不支持连续地址读，但支持连续地址写，所以rda5868_write_data函数的datalen参数可以不为1.
//参数说明regaddr：为regaddr为写入数据的寄存器地址，data为写入或读入数据的首地址指针，datalen为写入数据的个数 


static void RDA_InitGpioI2C(void);
static void RDA_DeinitGpioI2C(void);

static void RDA_InitGpioI2C(void)
{
    //SCI_TraceLow:"rda_InitGpioI2C"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_RDA_32_112_2_18_0_29_5_604,(uint8*)"");
#ifdef _IIC_GPIO_SIMULATOR_  //GPIO模拟I2C方式
    GPIO_SetDataMask(RDA_I2C_SDA,SCI_TRUE);
    GPIO_SetDataMask(RDA_I2C_SCL,SCI_TRUE); 
   
    GPIO_SetValue(RDA_I2C_SCL, SCI_TRUE); //Set to idle(high)
    GPIO_SetValue(RDA_I2C_SDA, SCI_TRUE);//Set to idle(high)
    GPIO_SetDirection(RDA_I2C_SCL, SCI_TRUE); //Set to output
    GPIO_SetDirection(RDA_I2C_SDA, SCI_TRUE);//Set to output
#else    
    I2C_Init(100000);//100K I2C Clock       
#endif
 
}

static void RDA_DeinitGpioI2C(void)
{
#ifdef _IIC_GPIO_SIMULATOR_
    //SCI_TraceLow:"rda_DeinitGpioI2C"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_RDA_50_112_2_18_0_29_5_605,(uint8*)"");
    GPIO_SetValue(RDA_I2C_SCL, SCI_TRUE); //Set to idle(high)
    GPIO_SetValue(RDA_I2C_SDA, SCI_TRUE);//Set to idle(high)

    GPIO_SetDirection(RDA_I2C_SCL, SCI_FALSE); //Set to input 
    GPIO_SetDirection(RDA_I2C_SDA, SCI_FALSE);//Set to input   	
#endif
}

#ifdef _IIC_GPIO_SIMULATOR_  //GPIO模拟I2C方式

static void RDA_bt_SerialCommStart(void);
static void RDA_bt_SerialCommStop(void);
static uint8 RDA_bt_SerialCommTxByte(uint8 data);
static void RDA_bt_SerialCommRxByte(uint8 *data, uint8 ack);
static void RDA_bt_SerialCommInit(void);

#define  RDA_SDA_DIR_OUT   GPIO_SetDirection(RDA_I2C_SDA, 1)
#define  RDA_SDA_DIR_IN    GPIO_SetDirection(RDA_I2C_SDA, 0)
#define  RDA_DATA_LOW    GPIO_SetValue(RDA_I2C_SDA, 0)
#define  RDA_DATA_HIGH   GPIO_SetValue(RDA_I2C_SDA, 1)
#define  RDA_CLK_LOW       GPIO_SetValue(RDA_I2C_SCL, 0)
#define  RDA_CLK_HIGH      GPIO_SetValue(RDA_I2C_SCL, 1)
#define  RDA_DATA_IN       GPIO_GetValue(RDA_I2C_SDA) 

/*8 nops, About 800ns.*/
#define CLK_DELAY_600NS  \
        __asm   \
        {     \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop;\
     nop;\
     nop;\
        }
                
/*10 nops, About 1200ns. */
#define CLK_DELAY_900NS  \
        __asm   \
        {     \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop;\
     nop;\
     nop;\
        }
        
/*12 nops, About 1600ns. 133ns/nop*/
#define CLK_DELAY_1300NS  \
        __asm   \
        {     \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop; \
     nop;\
     nop;\
     nop;\
     nop;\
        }
 
#define DELAY_DURATION_START_1 CLK_DELAY_600NS
#define DELAY_DURATION_START_2 CLK_DELAY_600NS
#define DELAY_DURATION_START_3 CLK_DELAY_900NS
 
#define DELAY_DURATION_STOP_1   CLK_DELAY_900NS
#define DELAY_DURATION_STOP_2    CLK_DELAY_600NS
#define DELAY_DURATION_STOP_3    CLK_DELAY_1300NS
 
#define DELAY_DURATION_HIGH     CLK_DELAY_900NS
#define DELAY_DURATION_LOW     CLK_DELAY_900NS

static void RDA_bt_SerialCommStart(void) /* start or re-start */
{ 
    RDA_SDA_DIR_OUT;
    RDA_DATA_HIGH;
    RDA_CLK_HIGH;
    DELAY_DURATION_START_1;
    RDA_DATA_LOW;
    DELAY_DURATION_START_2;
    DELAY_DURATION_START_2;
    RDA_CLK_LOW;
    DELAY_DURATION_START_3;
}
 
static void RDA_bt_SerialCommStop(void)
{
    RDA_CLK_LOW;
    RDA_SDA_DIR_OUT;
    DELAY_DURATION_LOW;
    RDA_DATA_LOW;
    DELAY_DURATION_STOP_1;
    RDA_CLK_HIGH;
    DELAY_DURATION_STOP_2;
    RDA_DATA_HIGH;
    DELAY_DURATION_STOP_3;
}
 
static uint8 RDA_bt_SerialCommTxByte(uint8 data) /* return 0 --> ack */
{
    int8 i;
    uint8 temp_value = 0;
    
    for(i=7; (i>=0)&&(i<=7); i--)
    {  
        RDA_CLK_LOW;
        DELAY_DURATION_LOW;  
        if(i==7)
        RDA_SDA_DIR_OUT;
        DELAY_DURATION_LOW;
    
        if((data>>i)&0x01)
            RDA_DATA_HIGH;
        else
            RDA_DATA_LOW;
        
        DELAY_DURATION_LOW;  
        RDA_CLK_HIGH;
        DELAY_DURATION_HIGH;  
    }
    
    RDA_CLK_LOW;
    DELAY_DURATION_LOW;
    RDA_SDA_DIR_IN;
    DELAY_DURATION_LOW;
    
    
    RDA_CLK_HIGH;
    DELAY_DURATION_HIGH;
    temp_value = RDA_DATA_IN; 
    RDA_CLK_LOW;
    
    DELAY_DURATION_LOW;
    
    if(temp_value)
        //SCI_TRACE_LOW:"\"# bt i2c write failed "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_RDA_201_112_2_18_0_29_5_606,(uint8*)"");
    
    return temp_value;
}
 
static void RDA_bt_SerialCommRxByte(uint8 *data, uint8 ack)
{
    int8 i;
    uint32 dataCache;

    dataCache = 0;

    for(i=7; (i>=0)&&(i<=7); i--)
    {  
        RDA_CLK_LOW;
        DELAY_DURATION_LOW;
        if(i==7) RDA_SDA_DIR_IN;
        DELAY_DURATION_LOW;
        RDA_CLK_HIGH;
        DELAY_DURATION_HIGH;
        dataCache |=((RDA_DATA_IN)<<i);
        DELAY_DURATION_LOW;
    }

    RDA_CLK_LOW;
    DELAY_DURATION_LOW;
    RDA_SDA_DIR_OUT;

    if(ack==0)       
       RDA_DATA_LOW;
    else      
       RDA_DATA_HIGH;

    DELAY_DURATION_LOW;
    RDA_CLK_HIGH;
    DELAY_DURATION_HIGH;
    RDA_CLK_LOW;
    DELAY_DURATION_LOW;
    *data = (uint8)dataCache;
}
 
static void RDA_bt_SerialCommInit(void)
{
    RDA_SDA_DIR_OUT;
    RDA_CLK_HIGH;
    RDA_DATA_HIGH;
    DELAY_DURATION_LOW;
}


void RDA5400_WriteData(uint8 regaddr, uint16* data, uint32 datalen)
{
    uint32  i=0;

    RDA_bt_SerialCommStart();///start
    RDA_bt_SerialCommTxByte(RDA5400_ADRW);//chip adress
    RDA_bt_SerialCommTxByte(regaddr);

    for(i=0;i<datalen;i++,data++)//data
    {
        RDA_bt_SerialCommTxByte(*data>>8);
        RDA_bt_SerialCommTxByte(*data);
    }
    RDA_bt_SerialCommStop();
}
 
void RDA5400_ReadData(uint8  regaddr, uint16*data, uint32 datalen)
{
    uint8  tempdata;
    uint32  i=0;

    RDA_bt_SerialCommStart();///start
    RDA_bt_SerialCommTxByte(RDA5400_ADRW);//chip adress
    RDA_bt_SerialCommTxByte(regaddr);

    RDA_bt_SerialCommStart();//start
    RDA_bt_SerialCommTxByte(RDA5400_ADRR);//chip adress

    for( i=0;i<datalen-1;i++,data++)//data
    {
        RDA_bt_SerialCommRxByte(&tempdata, 0);
        *data = (tempdata<<8); 
        RDA_bt_SerialCommRxByte(&tempdata, 0);   
        *data |= tempdata;  
    }

    RDA_bt_SerialCommRxByte(&tempdata, 0);
    *data = (tempdata<<8); 
    RDA_bt_SerialCommRxByte(&tempdata, 1);   
    *data |= tempdata; 

    RDA_bt_SerialCommStop();
}
 
 
void RDA5868_WriteData(uint32 regaddr, uint32 *data, uint32 datalen)
{
    uint32=0;

    RDA_bt_SerialCommStart();///start
    RDA_bt_SerialCommTxByte(RDA5868_ADRW);//chip adress
    RDA_bt_SerialCommTxByte(regaddr>>24);/////reg adress
    RDA_bt_SerialCommTxByte(regaddr>>16);
    RDA_bt_SerialCommTxByte(regaddr>>8); 
    RDA_bt_SerialCommTxByte(regaddr);
    RDA_bt_SerialCommStart();///start
    RDA_bt_SerialCommTxByte(RDA5868_ADRW);//chip adress
    for(i=0;i<datalen;i++,data++)//data
    {
        RDA_bt_SerialCommTxByte(*data>>24);
        RDA_bt_SerialCommTxByte(*data>>16);
        RDA_bt_SerialCommTxByte(*data>>8);
        RDA_bt_SerialCommTxByte(*data);
    }
    RDA_bt_SerialCommStop();
}
 
void RDA5868_ReadData(uint32 regaddr, uint32 *data, uint32 datalen)
{
    uint8  tempdata;
    uint32  i=0;

    RDA_bt_SerialCommStart();///start
    RDA_bt_SerialCommTxByte(RDA5868_ADRW);//chip adress
    RDA_bt_SerialCommTxByte(regaddr>>24);
    RDA_bt_SerialCommTxByte(regaddr>>16);
    RDA_bt_SerialCommTxByte(regaddr>>8);
    RDA_bt_SerialCommTxByte(regaddr);

    RDA_bt_SerialCommStart();//start
    RDA_bt_SerialCommTxByte(RDA5868_ADRR);//chip adress

    for( i=0;i<datalen-1;i++,data++)//data
    {
        RDA_bt_SerialCommRxByte(&tempdata, 0);
        *data  = (tempdata<<24); 
        RDA_bt_SerialCommRxByte(&tempdata, 0);   
        *data |= (tempdata<<16);    
        RDA_bt_SerialCommRxByte(&tempdata, 0);
        *data |= (tempdata<<8); 
        RDA_bt_SerialCommRxByte(&tempdata, 0);   
        *data |= tempdata;  
    }
    RDA_bt_SerialCommRxByte(&tempdata, 0);
    *data = (tempdata<<24); 
    RDA_bt_SerialCommRxByte(&tempdata, 0);   
    *data |= (tempdata<<16);  
    RDA_bt_SerialCommRxByte(&tempdata, 0);
    *data |= (tempdata<<8); 
    RDA_bt_SerialCommRxByte(&tempdata, 1);   
    *data |= tempdata; 

    RDA_bt_SerialCommStop();
}

#else  //硬件I2C方式 

//不支持连续地址写，所以datalen总为1; 
void RDA5400_WriteData(uint8 regaddr, uint16* data, uint32 datalen)
{
    uint8 temp[3];
    uint32 i=0;
    for(i=0;i<datalen;i++)
    {
        temp[0]=regaddr;
        temp[1]=(uint8)(((*data) & 0xff00)>>8);
        temp[2]=(uint8)((*data) & 0x00ff);
        I2C_RDA5400_WriteCmd(RDA5400_ADRW,temp,3,SCI_TRUE);
    }
}

//不支持连续地址读，所以datalen总为1; 
void RDA5400_ReadData(uint8 regaddr, uint16*data, uint32 datalen)
{
   uint8 temp[3]={0};
   uint32 i=0;
   for(i=0;i<datalen;i++)
   {
       temp[0]=regaddr;      
       I2C_RDA5400_WriteCmd(RDA5400_ADRW,temp,3,SCI_TRUE);
       *data=(temp[1]<<8)+(temp[2]);       		
   }
} 	

void RDA5868_WriteData(uint32 regaddr, uint32 *data, uint32 datalen)
{   
   I2C_RDA5868_WriteCmd(RDA5868_ADRW,regaddr,data,datalen,SCI_TRUE);  
}

//不支持连续地址读，所以datalen总为1;  
void RDA5868_ReadData(uint32 regaddr, uint32 *data,uint32 datalen)
{
   I2C_RDA5868_ReadCmd(RDA5868_ADRW,regaddr,data,datalen,SCI_TRUE); 
}

#endif


// RDA RF chip 5400  init register value
uint16 RDA5400_PSKEY[][2] = 
{
    {0x3f,0x0000},///page down
    {0x01,0x010F},//
    {0x0A,0x2004},//
    {0x16,0x9D2c},// XTAL cap
    {0x19,0xFb78},// 20090122
    {0x1A,0x034E},// tmx_lo_reg,tmx_out_reg
    {0x27,0x0800},// pll_stable_bypass
    {0x3B,0x31EA},// Freq setting available immediately
    {0x3D,0xF9F8},// lp4_en_deepsleep
    {0x3f,0x0001},///page up
    {0x04,0x0416},// int_reg_sw_en
    {0x06,0x262C},// reg1_dvdd_ref_bit_deepsleep
    {0x07,0x0624},// reg3_dvdd_ref_bit_deepsleep
    {0x08,0x2624},// Reg4_pu_deepsleep
    {0x10,0x0120},// Thermo_tmx_gain  TX power
    {0x19,0x001F},// thermo_pll_lobuf_ibit
    {0x20,0x0D10},// PADrv_gain
    {0x21,0x15A2},// PADrv_gain
    {0x22,0x1DB2},// PADrv_gain
    {0x23,0x25C2},// PADrv_gain
    {0x24,0x2E52},// PADrv_gain
    {0x25,0x3662},// PADrv_gain
    {0x26,0x3A70},// PADrv_gain

    {0x3f,0x0000}///page down
};


void RDA5400_Initialization(void)
{    
    uint32 i=0;
    for(i=0;i<sizeof(RDA5400_PSKEY)/sizeof(RDA5400_PSKEY[0]); i++)
    {
        RDA5400_WriteData(RDA5400_PSKEY[i][0],&RDA5400_PSKEY[i][1],1);  
    } 
    OS_TickDelay(100);
}

void RDA_bt_get_chipid(void)
{
    uint32 chipidaddr=0x40200028;
    uint32 chipid=0;
    RDA5868_ReadData(chipidaddr,&chipid,1);
    RDA5868_ReadData(chipidaddr,&chipid,1);
    //SCI_TRACE_LOW:"The chip id of RDA5868+ is 0x%08x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_RDA_446_112_2_18_0_29_6_607,(uint8*)"d",chipid);
}


void RDA_Chip_Init(void)//RDA5868+ init code 
{
    uint32 reg_value1;
    //SCI_TRACE_LOW:" BTI_StartupChip\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BT_RDA_453_112_2_18_0_29_6_608,(uint8*)"");

    /*sel IISMCK pin clk_32 function*/
    reg_value1 = *(volatile uint32*)0x8c000050;
    reg_value1 |= (BIT_6 |BIT_7);
    *(volatile uint32*)0x8c000050 = reg_value1;//open 32K
   
//    RDA_BT_POWER_ON;//BT chip powerup
    GPIO_SetValue(BT_PWR_EN_GPIO,SCI_TRUE);

    OS_TickDelay(120);
    RDA_InitGpioI2C();

#ifndef _IIC_GPIO_SIMULATOR_   
    LDO_TurnOnLDO(LDO_LDO_LCM);  
    LDO_TurnOnLDO(LDO_LDO_CMR);//I2C power on
#endif
    
    GPIO_SetBtReset(SCI_FALSE);
    OS_TickDelay(8);
    GPIO_SetBtReset(SCI_TRUE);
    
    OS_TickDelay(40);    

    RDA5400_Initialization();
    GPIO_SetBtReset(SCI_FALSE);
    OS_TickDelay(5);
    GPIO_SetBtReset(SCI_TRUE);
    OS_TickDelay(250);
    RDA5868_Initialization(); 
    RDA_bt_get_chipid();  
    
    RDA_DeinitGpioI2C();
       
#ifndef _IIC_GPIO_SIMULATOR_   
    LDO_TurnOffLDO(LDO_LDO_LCM);  
    LDO_TurnOffLDO(LDO_LDO_CMR);
#endif
    OS_TickDelay(8); 
}

