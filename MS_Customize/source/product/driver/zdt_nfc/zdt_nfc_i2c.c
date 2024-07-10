#ifndef _ZDT_NFC_I2C_C
#define _ZDT_NFC_I2C_C

#include "zdt_nfc.h"
extern uint32_t gtrecvlen;

#define NFC_I2C_ERROR                SCI_ERROR
#define NFC_I2C_SUCCESS              SCI_SUCCESS

//uint8_t I2C_DEV = 0x28;//eflash地址只能为0x28  boot地址可以0x28到0x2b配置

//0x2a
#define TP_NFC_I2C_FW_ADDR         0x54

//0x28
#define TP_NFC_I2C_ADDR       0x50

#define I2C_WRITE		0x00
#define	I2C_READ		0x01

LOCAL I2C_DEV NFC_I2C_cfg =
{
#ifdef ZDT_PCBA_ZC100_HW_V22_SUPPORT
    2,          
#else
    1,          
#endif
    100000,                   // freq is standard for NFC
    0,                          // bus
    TP_NFC_I2C_ADDR,  // slave_addr
    0,                          // reg_addr_num
    1,                          // check_ack
    0                           // no_stop
};

LOCAL I2C_DEV NFC_I2C_fw_cfg =
{
#ifdef ZDT_PCBA_ZC100_HW_V22_SUPPORT
    2,          
#else
    1,          
#endif
    100000,                   // freq is standard for NFC
    0,                          // bus
    TP_NFC_I2C_FW_ADDR,  // slave_addr
    0,                          // reg_addr_num
    1,                          // check_ack
    1                           // no_stop
};

LOCAL int32 NFC_I2C_FW_Handler = -1;
LOCAL int32 NFC_I2C_Handler = -1;

LOCAL uint32 _Get_NFC_I2C_FW_Status(void);
LOCAL uint32 _Get_NFC_I2C_Status(void);


LOCAL uint32 NFC_I2C_Open(void)
{    
    if(NFC_I2C_Handler == -1) 
    {
        if((NFC_I2C_Handler = I2C_HAL_Open(&NFC_I2C_cfg)) == -1)
        {
            NFC_TRACE("NFC_I2C_Open Err");
            return NFC_I2C_ERROR;    
        } 
        else
        {
            return NFC_I2C_SUCCESS;
        }
    }
    else
    {
        return NFC_I2C_SUCCESS;
        
    }
}

/******************************************************************************/
// Description: NFC_I2C_Close
// Dependence: 
// Note:
/******************************************************************************/

LOCAL uint32 NFC_I2C_Close(void)
{
    if(_Get_NFC_I2C_Status() == NFC_I2C_SUCCESS)
	{
 		I2C_HAL_Close(NFC_I2C_Handler);
		NFC_I2C_Handler = -1;
	}	
    return NFC_I2C_SUCCESS;
}

/******************************************************************************/
// Description: NFC_I2C_Write
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint32 NFC_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    uint32 ret = 0;
    if(_Get_NFC_I2C_Status() == NFC_I2C_SUCCESS)
    {
        ret = I2C_HAL_Write(NFC_I2C_Handler, reg, addr, cnt);
        ZDT_NFCTraceI2C_Send(addr, ret);
    }
    else
    {
        NFC_TRACE("NFC_I2C_Write ERR No handle");
    }
    return ret;
}


/******************************************************************************/
// Description: NFC_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint32 NFC_I2C_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    uint32 ret = 0;
    if(_Get_NFC_I2C_Status() == NFC_I2C_SUCCESS)
    {
        ret = I2C_HAL_Read_NFC(NFC_I2C_Handler,reg, addr, cnt);
        //NFC_TRACE("NFC_I2C_Read cnt=%d,ret=%d",cnt,ret);
        ZDT_NFCTraceI2C_RCV(addr, ret);

    }
    else
    {
        NFC_TRACE("NFC_I2C_Read ERR No handle");
    }
    return ret;
}

/******************************************************************************/
// Description: _Get_NFC_I2C_Status
// Dependence:  It can get the status from i2c
// Note:
/******************************************************************************/
LOCAL uint32 _Get_NFC_I2C_Status(void)
{
    if(NFC_I2C_Handler==-1)
    {
        return NFC_I2C_ERROR;
    }
    else
    {
        return NFC_I2C_SUCCESS;
    }
}

LOCAL uint32 NFC_I2C_FW_Open(void)
{    
    if(NFC_I2C_FW_Handler == -1) 
    {
        if((NFC_I2C_FW_Handler = I2C_HAL_Open(&NFC_I2C_fw_cfg)) == -1)
        {
            NFC_TRACE("NFC_I2C_Buf_Open Err");
            return NFC_I2C_ERROR;    
        } 
        else
        {
            return NFC_I2C_SUCCESS;
        }
    }
    else
    {
        return NFC_I2C_SUCCESS;
    }
}

LOCAL uint32 NFC_I2C_FW_Close(void)
{
    if(_Get_NFC_I2C_FW_Status() == NFC_I2C_SUCCESS)
	{
 		I2C_HAL_Close(NFC_I2C_FW_Handler);
		NFC_I2C_FW_Handler = -1;
	}	
    return NFC_I2C_SUCCESS;
}

LOCAL uint32 NFC_I2C_FW_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    uint32 ret = 0;
    if(_Get_NFC_I2C_FW_Status() == NFC_I2C_SUCCESS)
    {
        ret = I2C_HAL_Write(NFC_I2C_FW_Handler, &reg, addr, cnt);
    }
    return ret;
}

/******************************************************************************/
// Description: NFC_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint32 NFC_I2C_FW_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    uint32 ret = 0;
    if(_Get_NFC_I2C_FW_Status() == NFC_I2C_SUCCESS)
    {
        ret = I2C_HAL_Read(NFC_I2C_FW_Handler, &reg, addr, cnt);
    }
    return ret;
}

LOCAL uint32 _Get_NFC_I2C_FW_Status(void)
{
    if(NFC_I2C_FW_Handler==-1)
    {
        return NFC_I2C_ERROR;
    }
    else
    {
        return NFC_I2C_SUCCESS;
    }
}


void NFCC_I2C_Init(void) {
    NFC_I2C_Open();
}

void NFCC_I2C_DeInit(void) {
    NFC_I2C_Close();
}
/**
 *
 * @param[in] reg:寄存器地址
 * @param[in] len:字节数
 * @param[in] data:待写入的数据
 */
uint16_t i2cWriteBytes(uint8_t* data, uint16_t len) {
    if(0 == NFC_I2C_Write(data,data,len))
    {
        return 1;
    }
    return 0;
}


/**
 * 从I2C设备中读取数据
 * @param[in] reg:寄存器地址
 * @param[in] len:数据字节数
 * @param[out] data:读出的数据
 */
uint16_t i2cReadBytes(uint8_t *data) {
    uint32 len = 0;
    uint32 max_len = gtrecvlen;
    len = NFC_I2C_Read(data,data,max_len);
    //NFC_TRACE("i2cReadBytes len=%d",len);
    return (uint16)len;
}

#endif/*_ZDT_NFC_UART_C*/
