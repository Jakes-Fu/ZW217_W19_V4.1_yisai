
#include "os_api.h"
#include "sci_types.h"
#include "spi_drvapi.h"

//======================================================================
//                        SPI commands (Si24R1)
//======================================================================
#define R_REGISTER    		0x00  	// Define Read command to register
#define W_REGISTER   		0x20  	// Define Write command to register
#define R_RX_PAYLOAD 		0x61  	// Define Read RX payload register address
#define W_TX_PAYLOAD 		0xA0  	// Define Write TX payload register address
#define FLUSH_TX    		0xE1  	// Define Flush TX register command
#define FLUSH_RX    		0xE2  	// Define Flush RX register command
#define REUSE_TX_PL 		0xE3  	// Define Reuse TX payload register command
#define R_RX_PL_WID     	0x60  	// Define Read RX payload width for the R_RX_PAYLOAD in the RX FIFO
#define W_ACK_PAYLOAD   	0xA8	// Define Write Payload to be transmitted together with ACK packet on PIPExxx xxx valid in the range from 000 to 101)
#define W_TX_PAYLOAD_NOACK	0xB0	// Define Write TX Disables AUTOACK on this specific packet
#define NOP         		0xFF  	// Define No Operation, might be used to read status register

//======================================================================
//                    SPI registers address(Si24R1)
//======================================================================
#define CONFIG      0x00  // 'Config' register address
#define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    0x03  // 'Setup address width' register address
#define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       0x05  // 'RF channel' register address
#define RF_SETUP    0x06  // 'RF setup' register address
#define STATUS      0x07  // 'Status' register address
#define OBSERVE_TX  0x08  // 'Observe TX' register address
#define RPD         0x09  // 'Received Power Detector' register address  equal to Si24R1 is "CD"(Carrier Detecotr)
#define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define TX_ADDR     0x10  // 'TX address' register address
#define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17  // 'FIFO Status Register' register address
#define DYNPD       0x1C  // 'Enable dynamic paload length' register address
#define FEATURE     0x1D  //  Feature Register address

#define TX_ADR_WIDTH    	5
#define TX_PLOAD_WIDTH      32
#define RX_PLOAD_WIDTH      10   	 //PLOAD长度
#define MAX_TX  			0x10
#define TX_OK				0x20

const uint8 TX_ADDRESS[TX_ADR_WIDTH]={0xE7,0xE7,0xE7,0xE7,0xE7};

#define RFID_SI24R2E_LOGIC_ID 	1

#define CE_GPIO_STATUS(x)	GPIO_SetValue(25, x)
#define IRQ_GPIO 20


typedef struct
{
	SPI_DEV spiConfig;  
	BOOLEAN is_open_spi;
}SI24R2E_SPI_PORT_CFG_T;


SI24R2E_SPI_PORT_CFG_T  si24r2eSpi = {0};
static uint8  si24r2e_rd_nvm[52];
static uint8  si24r2e_rxbuf[52]; //READ BUFFER 

void si24r2e_Write_Reg(uint8 buffer_ptr1, uint8 buffer_ptr2)
{
	uint32 cs_sel;
	uint8 read;
	uint32 result = 0;
	uint8 buffer[2] = {0};
	uint8 readCmd[2] = {0};
	
	buffer[0] = buffer_ptr1;
	buffer[1] = buffer_ptr2;

	SPI_HAL_RW(RFID_SI24R2E_LOGIC_ID, readCmd, buffer, 2);
	OS_TickDelay(1);
	SCI_TRACE_LOW("%s[0x%02x]=0x%02x Read=0x%02x, 0x%02x", __FUNCTION__, buffer_ptr1,buffer_ptr2,readCmd[0], readCmd[1]);
}

uint8 si24r2e_Read_Reg(uint8 buffer_ptr){

	uint32 cs_sel;
	uint32 result = 0;
	uint8 data[2] = {0};
	uint8 readCmd[2];

	readCmd[0] = buffer_ptr;
	readCmd[1] = 0x0;
    
	SPI_HAL_RW(RFID_SI24R2E_LOGIC_ID, data, readCmd, 2);
	//OS_TickDelay(1);		
	
	SCI_TRACE_LOW("%s[0x%02x]=0x%02x,0x%02x", __FUNCTION__, buffer_ptr,data[0],data[1]);

	return data[1];
}

void si24r2e_Write_Buf(uint8 buffer_ptr1, uint8 *buffer2, uint32 len){

	uint32 result = 0;
	uint8 data[72] = {0};
	int i = 0;
	uint8 buffer[72] = {0};

	buffer[0] = buffer_ptr1;

	for(i = 0; i < len; i++){
		buffer[i + 1] = buffer2[i];
	}
	
	SPI_HAL_Write(RFID_SI24R2E_LOGIC_ID, buffer, len + 1);
	OS_TickDelay(2);	
}

void si24r2e_gpio_Init(void)
{	
	CE_GPIO_STATUS(0);
}

void rfid_si24r2e_open_spi(){
	if(!si24r2eSpi.is_open_spi){	
		si24r2eSpi.spiConfig.id = RFID_SI24R2E_LOGIC_ID;
		si24r2eSpi.spiConfig.freq = 100000;
		si24r2eSpi.spiConfig.tx_bit_length = 8;
		si24r2eSpi.spiConfig.mode = CPOL0_CPHA0;
		si24r2eSpi.is_open_spi = TRUE;
		
		SPI_HAL_Open (&si24r2eSpi.spiConfig);
	}
}

void rfid_si24r2e_close_spi(){
	si24r2eSpi.is_open_spi = FALSE;
	SPI_HAL_Close (si24r2eSpi.spiConfig.id);
}

void rfid_si24r2e_spi_set_cs(uint8 is_high){
    uint32 cur_cs = SPI_CS_LOW;
    if(is_high)
    {
        cur_cs = SPI_CS_HIGH;
    }
    SPI_HAL_Ioctl(si24r2eSpi.spiConfig.id, SPI_CTL_S_CS_NO_MUTEX, &cur_cs);
}

static void si24r2e_nvmop_en(void)
{
    //开启NVM 操作使能，能够读出 NVM 数据。
    si24r2e_Write_Reg(0x5a,0xa5);
}

static void si24r2e_latprog(void)
{
    // 使能NVM 编程使能。
    si24r2e_Write_Reg(0x5b,0xa6);
}

static void si24r2e_autx_disable(void)
{
    uint8 status = 0;
    rfid_si24r2e_spi_set_cs(1);
    OS_TickDelay(2); 
    rfid_si24r2e_spi_set_cs(0);
    OS_TickDelay(2); 
    rfid_si24r2e_spi_set_cs(1);
    OS_TickDelay(2); 
    //rfid_si24r2e_spi_set_cs(0);
    
    status = si24r2e_Read_Reg(0x07);
    if( (status & 0x40))
    {
        OS_TickDelay(2); 
    }
    
    si24r2e_Write_Reg(0x5c,0xa7); //强制关闭 AUTXON
  
}

static void si24r2e_sw_rst(void)
{
    rfid_si24r2e_spi_set_cs(1);
    si24r2e_Write_Reg(0x63,0x00);
    OS_TickDelay(20);
}

static void si24r2e_PROGBYTE(uint8 adr, uint8 din)
{
    si24r2e_Write_Reg(0x80 + (adr & 0x3F),din);
}

static void si24r2e_Rnvm(uint8 adr)
{
    si24r2e_Read_Reg(0xC0 + (adr & 0x3F));
}

static void si24r2e_Rnvm_All(void)
{
    uint8 i;
    uint8 tmp  = 0;
    si24r2e_autx_disable();
    OS_TickDelay(1);
    si24r2e_nvmop_en();
    OS_TickDelay(1);
    
    si24r2e_latprog();

    SCI_Sleep(600);
    si24r2e_PROGBYTE(0, 0xff);
    OS_TickDelay(2);
    for(i = 1; i < 52; i++)
    {
        si24r2e_Rnvm(i);
        tmp = si24r2e_Read_Reg(0x15);
        si24r2e_rd_nvm[i] = tmp;
        SCI_TRACE_LOW("%s[0x%02x]=0x%02x", __FUNCTION__, i,tmp);
    }
    si24r2e_PROGBYTE(0, 0xfc);
    OS_TickDelay(2);
    si24r2e_latprog() ;
    OS_TickDelay(2);
    si24r2e_nvmop_en();
    OS_TickDelay(10);
    si24r2e_sw_rst();
    OS_TickDelay(1);
}

static void si24r2e_Rnvm_50()
{
    uint8 bkcnt;
    uint8 i;
    uint8 tmp  = 0;
    
    si24r2e_autx_disable();
    OS_TickDelay(1);
    si24r2e_nvmop_en();
    SCI_Sleep(600);
    
    for(i=1;i<52;i++)
    {
        si24r2e_Rnvm(i);
        tmp = si24r2e_Read_Reg(0x15);
        si24r2e_rd_nvm[i] = tmp;
        SCI_TRACE_LOW("%s[0x%02x]=0x%02x", __FUNCTION__, i,tmp);
    }

    si24r2e_nvmop_en();
    OS_TickDelay(10);
        
    si24r2e_sw_rst();
 }

static uint8 si24r2e_Rnvm_addr(uint8 addr)
{
    uint8 cur_byte;
  
    rfid_si24r2e_spi_set_cs(1);
    OS_TickDelay(1);
    rfid_si24r2e_spi_set_cs(0);
   OS_TickDelay(2);

  

    si24r2e_autx_disable();
    OS_TickDelay(2);
    
    //force miso output signals 
    si24r2e_Write_Reg(0x20+0x0C, 0x00);
    si24r2e_Write_Reg(0x5e, 0xa9);
    
    si24r2e_nvmop_en();
    OS_TickDelay(2);

	
    si24r2e_Rnvm(addr);
    OS_TickDelay(1);
    cur_byte = si24r2e_Read_Reg(0x15);
 
    OS_TickDelay(1);
    si24r2e_nvmop_en();
    OS_TickDelay(1);
    
    //cancle miso output signals 
    si24r2e_Write_Reg(0x20+0x0C, 0x00);
    si24r2e_Write_Reg(0x5e, 0xa9);
    
    OS_TickDelay(2);
    si24r2e_sw_rst();
    return cur_byte;
 }
 

static uint8 si24r2e_lock_chip()
{
    uint8 r00_val = 0x00;
	// CSN=0;
  // CSN=1;
  // delay_us(2);

 /* while((status & 0x40) == 0)
  { 
	  delay_us(25);
	  status = spi_rd_reg(0x07);
		si24r2e_sw_rst();
		delay_900us();
	} */

	//si24r2e_autx_disable();

    si24r2e_nvmop_en();
    OS_TickDelay(1);

    si24r2e_latprog();
    OS_TickDelay(1);


    si24r2e_PROGBYTE(0, 0xfb);
    OS_TickDelay(3);	
    si24r2e_latprog() ;
    OS_TickDelay(3);
    si24r2e_nvmop_en();
    OS_TickDelay(2);
    si24r2e_sw_rst();
    OS_TickDelay(1);
    r00_val = si24r2e_Rnvm_addr(0x00);
    return r00_val;
}

static void si24r2e_prog(void)
{
    uint8 i;
    uint8 init_buf[64] = {0xfc,0x12,0x9a,0x50,0x09,0x60,0x12,0x4a,0x32,0x23,
                                  0xab,0xba,0xe7,0xee,0x07,0x01,0xe7,0x33,0x34,0x9a,
                                  0x02,0x2e,0xde,0x93,0x07,0x00,0x00,0x00,0x00,0x00,
                                  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                                  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                                  0xff,0xff,0x00};
    
    si24r2e_autx_disable();
    OS_TickDelay(1);
    si24r2e_nvmop_en();
    OS_TickDelay(1);
    
    si24r2e_latprog();

    SCI_Sleep(600);
    si24r2e_PROGBYTE(0, 0xfb);
    OS_TickDelay(2);
#if 1
    for(i = 1; i < 52; i++)
    {
        si24r2e_PROGBYTE(i, init_buf[i]);
    }
#else
    for(i=0;i<52;i++)
    {
        //TXDATA 10-31	
        if(si24r2e_rxbuf[i] == 0xff)
        {
        	OS_TickDelay(2);
        }
        else
        {
            si24r2e_PROGBYTE(i, si24r2e_rxbuf[i]);
            OS_TickDelay(4);	
        }
        //printf("%x,",(uint)si24r2e_rxbuf[i]);
    }
#endif
    si24r2e_PROGBYTE(0, 0xfc);
    OS_TickDelay(2);
    si24r2e_latprog();
    OS_TickDelay(2);
    si24r2e_nvmop_en();
    OS_TickDelay(10);
    si24r2e_sw_rst();
    OS_TickDelay(1);
}

PUBLIC void si24r2e_init_all()
{	
    si24r2e_gpio_Init();	//初始化gpio
    
    // 重新上电
    GPIO_SetRFIDLdo(FALSE);
    SCI_Sleep(100);
    GPIO_SetRFIDLdo(TRUE);
    SCI_Sleep(300); //延迟一下把,时序不对有问题
    
    //初始化spi
    rfid_si24r2e_open_spi();
    OS_TickDelay(2); 
    
    //开始编程注意: RFID编程的总次数不能超过128次
    si24r2e_prog();

    //根据编程开始发射数据
    CE_GPIO_STATUS(1);

    //关闭SPI
    rfid_si24r2e_close_spi(); 
}

PUBLIC void si24r2e_read_all()
{	
    si24r2e_gpio_Init();	//初始化gpio
    
    // 重新上电
    GPIO_SetRFIDLdo(FALSE);
    SCI_Sleep(100);
    GPIO_SetRFIDLdo(TRUE);
    SCI_Sleep(300); //延迟一下把,时序不对有问题
    
    //初始化spi
    rfid_si24r2e_open_spi();
    OS_TickDelay(2); 
    
    //开始读取
    si24r2e_Rnvm_All();

    //根据编程开始发射数据
    CE_GPIO_STATUS(1);

    //关闭SPI
    rfid_si24r2e_close_spi(); 
}

PUBLIC void si24r2e_read_test()
{	
    uint8 status = 0;
    // 3.3转换电压
    si24r2e_gpio_Init();	//初始化gpio
    GPIO_SetRFIDLdo(FALSE);
    SCI_Sleep(100);
    GPIO_SetRFIDLdo(TRUE);
    SCI_Sleep(300); //延迟一下把,时序不对有问题
    
    rfid_si24r2e_open_spi(); //初始化spi
    OS_TickDelay(2);

    rfid_si24r2e_spi_set_cs(1);
    OS_TickDelay(2);
    rfid_si24r2e_spi_set_cs(0);
    OS_TickDelay(2);
    rfid_si24r2e_spi_set_cs(1);
    OS_TickDelay(2);
    //rfid_si24r2e_spi_set_cs(0);
    
    status = si24r2e_Read_Reg(0x07);
    if( (status & 0x40))
    {
        OS_TickDelay(2); 
    }
    
    rfid_si24r2e_close_spi(); //close spi
}

