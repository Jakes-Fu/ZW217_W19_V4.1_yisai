/*************************************************************************
 *
 * MODULE NAME:    bt_rda_test.c
 * PROJECT CODE:   rda5868
 * DESCRIPTION:    rda5868 test file.
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


#include "bt_rda.h"
#include "bt_rda_test.h"

/*****************************BCSP Interface*******************************/

extern bool OSUartDrv_Tx(char* buf,uint16 num_to_send,uint16 *numSend);

uint32 rda_uint8_convert_to_uint32(uint8* src);  
void rda_uint32_convert_to_uint8(uint32 src,uint8* des); 
uint32 rda_tx_char_expand(uint8* buf,uint32 len);  
uint32 rda_tx_ack_le_prepare(uint8* buf,uint8 type);
void rda_test_tx(uint8* buf,uint32 len);   

enum RDA_RX_STATUS
{    
    RDA_RX_INIT,
    RDA_RX_HEAD,
    RDA_RX_PAYLOAD,
    RDA_RX_CRC,
    RDA_RX_END
};

#define BCSP_ACK_CHANNEL            0       /* ack unrel */
#define BCSP_LE_CHANNEL             1
#define BCSP_CMD_CHANNEL          5       /* evt rel */
#define BCSP_EVENT_CHANNEL      5

#define BCSP_PKT_SYNC            0
#define BCSP_PKT_SYNC_RESP       1
#define BCSP_PKT_CONF            2
#define BCSP_PKT_CONF_RESP       3
#define BCSP_PKT_ACK                   4
#define BCSP_PKT_NONE            5

uint8 BCSP_LE_HEAD[4]={0x00,0x41,0x00,0xBE};
uint8 BCSP_LE_CONTENT[4][4]=
{
    0xDA,0xDC,0xED,0xED,
    0xAC,0xAF,0xEF,0xEE,
    0xAD,0xEF,0xAC,0xED,
    0xDE,0xAD,0xD0,0xD0
};

uint8 bcsp_send_seq=0;
uint8 bcsp_send_ack=0;

uint32 rda_uint8_convert_to_uint32(uint8* src)
{
    uint32 des;
    des=src[0]+(src[1]<<8)+(src[2]<<16)+(src[3]<<24);
    return des;    
}

void rda_uint32_convert_to_uint8(uint32 src,uint8* des) 
{
    uint32 i;
    for(i=0;i<4;i++)
    {
    	des[i]=(uint8)(src & 0xff);
    	src >>= 8;
    }
}

uint32 rda_tx_char_expand(uint8* buf,uint32 len)
{//exclude 0xC0 in the head and the tail
    uint32 i,j;
    uint32 temp_len=len;
        
    for(i=0;i<temp_len;i++)
    {
        if((buf[i]==0xC0)||(buf[i]==0xDB))																																		
        {                                 																																		
            for(j=temp_len;j>i+1;j--) 																																		
            {                         																																		
                buf[j]=buf[j-1];  																																		
            }                         																																		
            if(buf[i]==0xC0)          																																		
            {                         																																		
            	buf[i]=0xDB;      																																		
            	buf[i+1]=0xDC;    																																		
            }                         																																		
            else                      																																		
            {                         																																		
            	buf[i]=0xDB;      																																		
            	buf[i+1]=0xDD;    																																		
            }                         																																		
            i++;                      																																		
            temp_len++;               																																		
        }		                  																																		
    }	    
    return temp_len;
}

uint32 rda_tx_ack_le_prepare(uint8* buf,uint8 type)//le info or ack
{
    uint32 temp_len;    
    buf[0]=0xC0;
    if(type==BCSP_PKT_ACK)//ACK
    {
        buf[2] = 0;
        buf[3] = 0;

        buf[1] = (bcsp_send_ack << 3);
        buf[4] = (~(buf[1] + buf[2] + buf[3]))&0xff;
        temp_len=rda_tx_char_expand(&buf[1],4);

        buf[temp_len+1] = 0xC0;
    }
    else//LE
    {
        memcpy(&buf[1],BCSP_LE_HEAD,4);
        memcpy(&buf[5],BCSP_LE_CONTENT[type],4);
        temp_len=rda_tx_char_expand(&buf[1],8);        
        buf[temp_len+1]=0xC0;        
    }
    return (temp_len+2);
}

void rda_test_tx(uint8* buf,uint32 len)
{
    uint8 temp_buf[50]={0};
    uint32 temp_len=len-1;
    uint16 i=0;
    temp_buf[0]=0xC0;
    temp_buf[1] = (1 << 7) + (bcsp_send_ack<< 3)+ (bcsp_send_seq);
    
    temp_buf[2] = ((temp_len & 0x0f )<<4) +BCSP_CMD_CHANNEL ;
    temp_buf[3] = (temp_len & 0x0ff0 )>>4;
    temp_buf[4] = (~(temp_buf[1] + temp_buf[2]+ temp_buf[3])) & 0xff;
    
    memcpy(&temp_buf[5],(buf+1),temp_len);
    temp_len=rda_tx_char_expand(&temp_buf[1],(temp_len+4));
    temp_buf[temp_len+1]=0xc0;
    temp_len+=2;
    OSUartDrv_Tx((char*)temp_buf,(uint16)temp_len,&i);
    bcsp_send_ack=(bcsp_send_ack+1)&7;
    bcsp_send_seq=(bcsp_send_seq+1)&7;
}

/*******************************I2c PSeky For Test*******************/

static uint16 RDA5400_VCO_TEST[][2] = 
{
    {0x3f,0x0000},///page down
    {0x1A,0x03D1},//XTAL cap
    {0x16,0x9d3c},//XTAL cap
    {0x02,0xF180},//bbdac_range
};
 

static uint32 RDA5868_VCO_TEST[][2]=
{
    {0x40200000,0x38001aaf},
    {0x40200020,0x00000157}
};

/*if the bluetooth working baudrate is other than 921600, 
please modify the register 0x10000020 to the certain working baud value*/

static uint32 RDA5868_DUT_RF[][2]=
{
    {0x40240000,0x0004F398},
    {0x10000020,0x000E1000},
    {0x10000080,0x00000005},															
    {0x10000084,0x00470d37},															
    {0x10000088,0x00482a57},															
    {0x1000008c,0x00520400},															
    {0x10000090,0x00530511},															
    {0x10000094,0x00437f58},		
    //{0x10000000,0x10000000}
    {0x10000000,0x10000100}//Lu modified baud rate															
};

static uint32 RDA5868_DUT_CMU200_PSKEY[][2] = 
{
    {0x1000003C,0xA0A0BF30}, //pskey rf timing
    {0x10000000,0x00008000}  //pskey flag
};              

static uint32 RDA5868_DUT_TC3000_PSKEY[][2] = 
{
    {0x1000003c,0xA0A0BF30},//pskey rf timing
    {0x10000030,0x00002000},//pskey max power enable   
    {0x10000000,0x00009000},//pskey flag
};  

static uint32 RDA5868_DUT_N4010_PSKEY[][2]=
{
    {0x40180000,0x00000000},
    {0x40180044,0x00000001},
    {0x40240000,0x1004fb98},//SCU: SPI2_clk_en pclk_SPI2_en          
    {0x10000030,0x000000fd},//Max_output_power (for TC3000B)         
    {0x10000038,0x0f054000},//Radio_setup_time                       
    {0x1000003c,0xa0a0bf30},//TxRx_low_setup_time TxRx_setup_time  
    {0x10000034,0x05025040},
    {0x10000000,0x0000f000} //PSKEY: Modify Flag                     
};


static uint32 RDA5868_N4010_PATCH[][2]=
{
    {0x40180004,0x000146c4},
    {0x40180024,0x00021800},
    {0x4018000c,0x00013d84},
    {0x4018002c,0x00021800},
    //DV packet and packet length patch
    {0x100000c0,0xe248fc8c},
    {0x100000c4,0xe3540008},
    {0x100000c8,0x03a06009},
    {0x100000cc,0x11a06002},
    {0x100000d0,0xe1c060bc},
    {0x100000d4,0xe3a00470},
    {0x100000d8,0xe2800e61},
    {0x100000dc,0xe5901000},
    {0x100000e0,0xe2400e40},
    {0x100000e4,0xe5801000},
    {0x100000e8,0xe2800e40},
    {0x100000ec,0xe5901004},
    {0x100000f0,0xe2400e40},
    {0x100000f4,0xe5801004},
    {0x100000f8,0xe3a01bb9},
    {0x100000fc,0xe281ffc9},
                          
    {0x40180008,0x0002e6fc},
    {0x40180028,0x100000C0},
    {0x40180000,0x00007000}
};


static uint32 RDA5868_N4010_PATCH_2[][2] = 
{
    {0x40180004,0x000073b4},
    {0x40180024,0x0000eef4},
    {0x40180000,0x00000001}
};

/***********************UART PSKey For Test*********************/

static RDA5868_RF_BBTEST[] = 
{
    0x01,0x10,0xFD,0x18,0x00,0x00,0x01,0x04,0x04,
    0x00,0x00,0x00,0x33,0x1A,0x3A,0xE2,0x4E,0x7A,
    0x2C,0xCE,0xFF,0x01,0x55,0x55,0x00,0x00,0x12,0x00
};

static uint8 RDA5868_RF_SINGLEHOP[] = 
{
    0x01,0x13,0xFc,0x01,0x00
};

static uint8 RDA5868_RF_INQUIRY[] = 
{
    0x01,0x01,0x04, 0x05,0x33, 0x8B,0x9E, 0x30,0x00
};

static uint8 RDA5868_ENABLE_DUT[] = 
{
    0x01,0x03, 0x18, 0x00
};
static uint8 RDA5868_ENABLE_ALLSCAN[] = 
{
    0x01, 0x1a, 0x0c, 0x01, 0x03
};
static uint8 RDA5868_ENABLE_INQUIRYSCAN[] =
{
    0x01,0x1a,0x0c,0x01,0x01	
};

static uint8 RDA5868_ENABLE_PAGESCAN[]=
{
    0x01,0x1a,0x0c,0x01,0x02	
};


static uint8 RDA5868_AUTOACCEPT_CONNECT[] = 
{
    0x01,0x05, 0x0c, 0x03, 0x02, 0x00, 0x02
};

static uint8 RDA5868_HCI_RESET[] = 
{
    0x01,0x03,0x0c,0x00
};



void rda5868_dut_rf_write(void);
void rda5868_dut_cmu200_pskey_write(void);
void rda5868_dut_tc3000_pskey_write(void);
void rda5868_dut_n4010_pskey_write(void); 
void rda5868_dut_n4010_patch_write(void);
void rda5868_dut_n4010_patch_write_2(void);
void rda5868_vco_test_write(void);  
void rda5400_vco_test(void);


void rda5868_dut_rf_write(void)
{
    uint8 i;   
    for(i=0;i<sizeof(RDA5868_DUT_RF)/sizeof(RDA5868_DUT_RF[0]);i++)
    {
        RDA5868_WriteData(RDA5868_DUT_RF[i][0],&RDA5868_DUT_RF[i][1],1);      
        OS_TickDelay(20);
    }
}


void rda5868_dut_cmu200_pskey_write(void)
{
    uint8 i;  	
    for(i=0;i<sizeof(RDA5868_DUT_CMU200_PSKEY)/sizeof(RDA5868_DUT_CMU200_PSKEY[0]);i++)
    {
        RDA5868_WriteData(RDA5868_DUT_CMU200_PSKEY[i][0],&RDA5868_DUT_CMU200_PSKEY[i][1],1);
        OS_TickDelay(20);		  
    }
}

void rda5868_dut_tc3000_pskey_write(void)
{
    uint8 i;
    for(i=0;i<sizeof(RDA5868_DUT_TC3000_PSKEY)/sizeof(RDA5868_DUT_TC3000_PSKEY[0]);i++)
    {
        RDA5868_WriteData(RDA5868_DUT_TC3000_PSKEY[i][0],&RDA5868_DUT_TC3000_PSKEY[i][1],1);
        OS_TickDelay(20);
    }
}


void rda5868_dut_n4010_pskey_write(void)
{
    uint8 i;
    for(i=0;i<sizeof(RDA5868_DUT_N4010_PSKEY)/sizeof(RDA5868_DUT_N4010_PSKEY[0]);i++)
    {
        RDA5868_WriteData(RDA5868_DUT_N4010_PSKEY[i][0],&RDA5868_DUT_N4010_PSKEY[i][1],1);
        OS_TickDelay(20);
    }
}

void rda5868_dut_n4010_patch_write(void)
{
    uint8 i;    	
    for(i=0;i<sizeof(RDA5868_N4010_PATCH)/sizeof(RDA5868_N4010_PATCH[0]);i++)
    {
        RDA5868_WriteData(RDA5868_N4010_PATCH[i][0],&RDA5868_N4010_PATCH[i][1],1);
        OS_TickDelay(20);
    }
}

void rda5868_dut_n4010_patch_write_2(void)
{
    uint8 i;   
    for(i=0;i<sizeof(RDA5868_N4010_PATCH_2)/sizeof(RDA5868_N4010_PATCH_2[0]);i++)
    {
        RDA5868_WriteData(RDA5868_N4010_PATCH_2[i][0],&RDA5868_N4010_PATCH_2[i][1],1);
        OS_TickDelay(20);
    }
}

void rda5868_vco_test_write(void)
{
    uint8 i;
    for(i=0;i<sizeof(RDA5868_VCO_TEST)/sizeof(RDA5868_VCO_TEST[0]);i++)
    {
        RDA5868_WriteData(RDA5868_VCO_TEST[i][0],&RDA5868_VCO_TEST[i][1],1);
        OS_TickDelay(20);
    }
}

void rda5400_vco_test(void)
{
    uint8 i=0;
    uint8 temp[3]={0};	
    for(i =0;i<sizeof(RDA5400_VCO_TEST)/sizeof(RDA5400_VCO_TEST[0]); i++)
    {
        RDA5400_WriteData(RDA5400_VCO_TEST[i][0],&RDA5400_VCO_TEST[i][1],1);
    }	
    OS_TickDelay(20);  
}

/***********************UART  for Test ****************************/

#define RDA5868_TEST_DELAY    50 

void rda5868_buffer_process(uint8* buf,uint32 len);
void rda5868_rf_bbtest_Write(void);
void rda5868_rf_singlehop_Write(void);
void rda5868_enable_dut_write(void);
void rda5868_le_init_process(void);


void  rda5868_buffer_process(uint8* buf,uint32 len)
{
    rda_test_tx(buf,len);
    OS_TickDelay(RDA5868_TEST_DELAY);
}

void rda5868_le_init_process(void)
{
    uint8 buf[50]={0};
    uint32 len=0;
    uint32 i=0;
    bcsp_send_ack=1;
    bcsp_send_seq=0;
    len=rda_tx_ack_le_prepare(buf,BCSP_PKT_CONF);
    OSUartDrv_Tx(buf,len,&i);
    len=rda_tx_ack_le_prepare(buf,BCSP_PKT_ACK);
    for(i=0;i<2;i++)
    {
        OSUartDrv_Tx(buf,len,&i);
        OS_TickDelay(2);
    }
}

void rda5868_rf_bbtest_write(void)
{   
    rda5868_le_init_process();
    rda5868_buffer_process(RDA5868_RF_BBTEST,sizeof(RDA5868_RF_BBTEST));	
}

void rda5868_rf_singlehop_write(void)
{
    rda5868_le_init_process();
    rda5868_buffer_process(RDA5868_RF_SINGLEHOP,sizeof(RDA5868_RF_SINGLEHOP));
}

void rda5868_enable_dut_write(void)
{
    rda5868_le_init_process();
    rda5868_buffer_process(RDA5868_ENABLE_ALLSCAN,sizeof(RDA5868_ENABLE_ALLSCAN));
    rda5868_buffer_process(RDA5868_AUTOACCEPT_CONNECT,sizeof(RDA5868_AUTOACCEPT_CONNECT));
    rda5868_buffer_process(RDA5868_ENABLE_DUT,sizeof(RDA5868_ENABLE_DUT));
}

/********************************************************************************/

void rda5868_send_bbtest(void);
void rda5868_send_bbtest_singlehop_inquiry(void);
void rda5868_send_vco_test(void);
void rda_bt_send_cmu200_enable_dut(void);
void rda_bt_send_tc3000_enable_dut(void);
void rda_bt_send_n4010_enable_dut(void);

void rda5868_send_bbtest(void)
{
    rda5868_rf_bbtest_write();    
}

void rda5868_send_bbtest_singlehop_inquiry(void)
{
    rda5868_rf_singlehop_write();
}


void rda5868_send_vco_test(void)
{   
    OS_TickDelay(4);  
    rda5400_vco_test(); 
    OS_TickDelay(4);  
    rda5868_vco_test_write();
}

void rda_bt_send_cmu200_enable_dut(void)
{
    rda5868_enable_dut_write();
    rda5868_dut_cmu200_pskey_write();
}


void rda_bt_send_tc3000_enable_dut(void)
{
    rda5868_enable_dut_write();
    rda5868_dut_tc3000_pskey_write();
}


void rda_bt_send_n4010_enable_dut(void)
{
    rda5868_enable_dut_write();
    rda5868_dut_n4010_pskey_write();
    rda5868_dut_n4010_patch_write_2();
}



void rda_bt_poweron_for_test(void) 
{
//    RDA_BT_POWER_ON;
    GPIO_SetValue(BT_PWR_EN_GPIO,SCI_TRUE);
    OS_TickDelay(10);
    GPIO_SetBtReset(SCI_TRUE);
    OS_TickDelay(50);  
    RDA5400_Initialization();
    GPIO_SetBtReset(SCI_FALSE);
    OS_TickDelay(10);                 
    GPIO_SetBtReset(SCI_TRUE); 
    OS_TickDelay(250);        
    rda5868_dut_rf_write();
    OS_TickDelay(100);
}

/********************************************************************************/

void SSCHandle_RDA_BT_TEST_VCO(void)
{
    rda_bt_poweron_for_test();	
    OS_TickDelay(20);
    rda5868_send_vco_test();
}

void SSCHandle_RDA_BT_TEST_SINGLEHOP(void)
{
    rda_bt_poweron_for_test();	
    OS_TickDelay(20);
    rda5868_send_bbtest_singlehop_inquiry();	
}


void SSCHandle_RDA_BT_TEST_CMU200(void)
{	
    rda_bt_poweron_for_test();	
    OS_TickDelay(20);
    rda_bt_send_cmu200_enable_dut();	
}

void SSCHandle_RDA_BT_TEST_TC3000(void)
{	
    rda_bt_poweron_for_test();	
    OS_TickDelay(20);
    rda_bt_send_tc3000_enable_dut();	
}

void SSCHandle_RDA_BT_TEST_N4010(void)/////输入号码 调入函数
{
    rda_bt_poweron_for_test();
    OS_TickDelay(20);
    rda_bt_send_n4010_enable_dut();	
}
