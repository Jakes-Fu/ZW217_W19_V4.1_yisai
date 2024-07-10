#if 0
/************************************************Copyright(c)***************************************************
**                                   Quintic(Beijing) Microelectronics Co,Ltd.
**                                   
**                                   http://www.quinticcorp.com 
**
**--------------------File Info----------------------------------------------------------------
** File Name:       qmaio.c
** Last Modified Date: 2009-
** Last Version:     0.1
** Descriptions: The qmaio.c is used to driver I2C interface
**
**----------------------------------------------------------------------------------------
** Created by:   ws
** Created Date: 2009-08-26
** Version:      0.1
** Descriptions: 
**
**-------------------------------------------------------------------------------------
** Modified by:
** Descriptions:   QN8610 A1 version
**
**************************************************************************************************/
#include "qmadriver.h"
#include "qmaio.h"
#include "Os_api.h"

UINT8 qma_i2c;

/*****************************************************************************************************
** Name:      Msdelay()
** Function:  time delay
*****************************************************************************************************/
void Msdelay(unsigned short dly)
{ 
    	unsigned short i; 
   
    for( ; dly>0; dly--)
    for(i=0; i<MS_DELAY_CONST; i++);
    
}

/*************************************************************************************************
** Name:      I2C_subfunction
** Function:  Software i2c code  
*****************************************************************************************************/
void Set_SCL(UINT8 i)      
{
    if(i) 
    {
        SCL_1;
    }
    else  
    { 
        SCL_0;
    }
}

void Set_SDA(UINT8 i)      
{
    if(i) 
    {
        Set_Datain;
    } 
    else 
    {
        Set_Dataout;
        SDA_0;
    }
}

void Start(void)               
{ 
    Msdelay(1);
    Set_SCL(1);
    Msdelay(1);
    Set_SDA(1);
    Msdelay(1);
    Set_SDA(0);
    Msdelay(2);
    Set_SCL(0);
}

void Stop(void)              
{
    Msdelay(1);
    Set_SDA(0);
    Msdelay(1);
    Set_SCL(1);
    Msdelay(2);
    Set_SDA(1);
}

/* i=0 for ACK and i=1 for Nack */
void Send_ACK(UINT8 i) 
{
    Set_SDA(i);
    Msdelay(1);
    Set_SCL(1);
    Msdelay(1);
    Set_SCL(0);
    Msdelay(1);
    Set_SDA(1);
}
 
/**************************************************
In: None
Out: the level of SDA's pin
**************************************************/ 
UINT8 Check_ACK(void)
{
    UINT8 status = 0;
	
    Set_Datain; 
    Msdelay(1);
    Set_SCL(1);
    status = (UINT8)Read_SDA;
    Msdelay(1);
    Set_SCL(0);
    Set_Dataout; 

    return status;
}

void Write_Bit(UINT8 i)
{
    Msdelay(1);
    Set_SDA(i);
    Msdelay(1);
    Set_SCL(1);
    Msdelay(2);
    Set_SCL(0);
}

void Write_Byte(UINT8 Data)
{
    UINT8 i;
	
    for( i = 0;i < 8;i++)
    {
        Write_Bit(Data>>7);
        Data<<=1;
    }
}

UINT8 Read_Byte(void)
{
    UINT8 j;
    UINT8 Data1=0x00;
	
    Set_Datain;
    for( j = 0;j < 8;j++)
    {
        Msdelay(1);
        Set_SCL(1);
        Data1 = (Data1<<1) | ((UINT8) Read_SDA & 0x01);
        Msdelay(1);
            Set_SCL(0);
        }
    Set_Dataout;
	
    return Data1;
}
 
/************************************************************************************************************
** Name: I2C_Write_1byte                         
** Funcation:write a data to a desired            
**           register through i2c bus 
** Description: Slave---------device address
**              Regis_Addr----register address
return :
		1: successful to write
		0: failed to write
*************************************************************************************************************/
UINT8 I2C_Write_1byte(UINT8 Slave,UINT8 Regis_Addr,UINT8 Data)
{
    UINT8 status = 0;
    UINT8 temp;

    temp=Slave;

    Start();
    Write_Byte(temp);
    status = Check_ACK();
    if (!status) 
    {
        Write_Byte(Regis_Addr);
        status = Check_ACK();
        if (!status) 
        {
            Write_Byte(Data);
            status = Check_ACK();
        }
    }
    Stop();

    return !status;
}
 
/***********************************************************************************************************
** Name: I2C_Read_1byte                          
** Function: Read a data from a desired register 
**           through i2c bus 
** Description: Slave---------device address
**              Regis_Addr----register address
return :
		1: successful to write
		0: failed to write
************************************************************************************************************/
UINT8 I2C_Read_1byte(UINT8 Slave,UINT8 Regis_Addr,UINT8 *Regis_Data)
{ 
    UINT8 status;
    UINT8 temp;

    temp =Slave | 0x01;

    Start();
    Write_Byte(Slave);
    status = Check_ACK();
    if (!status) 
    {
        Write_Byte(Regis_Addr);
        status = Check_ACK();
        if(!status) 
        {
            Start();
            Write_Byte(temp);
            status = Check_ACK();
            if (!status) 
            {
                *Regis_Data = Read_Byte();
                Send_ACK(1);
            }
        }
    }
    Stop();

    return !status;
}

  /***********************************************************************************************************
  ** Name: I2C_Read_nbyte                            
  ** Function: Read n data from a desired register    
  **           through i2c bus 
  ** Description: Slave---------device address
  **              Regis_Addr----register address
return :
		1: successful to write
		0: failed to write
  ************************************************************************************************************/
  UINT8 I2C_Read_nbyte(UINT8 Slave,UINT8 Regis_Addr, UINT8 *buf, UINT8 n)
  { 
    UINT8 temp;
    UINT8 i;
    UINT8 status = 0;
	
    temp =Slave | 0x01;

    Start();
    Write_Byte(Slave);
    status = Check_ACK();
    if(!status) 
    {
        Write_Byte(Regis_Addr);
        status = Check_ACK();
        if (!status)
        {
            Start();
            Write_Byte(temp);
            status = Check_ACK();
            if (!status)
            {
                Set_Datain;  
                for(i=0; i<n-1; i++) {
                    buf[i] = Read_Byte();
                    Send_ACK(0);
                }
                buf[i] = Read_Byte();
                Send_ACK(1);
            }
        }
    }
    Stop();

    return !status;
}

  /***********************************************************************************************************
    ** Name: I2C_Write_nbyte                
    ** Function: Write n bytes data to register 
    **             through i2c bus 
    ** Description: Slave---------device address
    **                Regis_Addr----register address
return :
		1: successful to write
		0: failed to write
    ************************************************************************************************************/
UINT8 I2C_Write_nbyte(UINT8 Slave,UINT8 Regis_Addr, UINT8 *buf, UINT8 n)
{ 
    UINT8 temp;
    UINT8 i;
    UINT8 status = 0;
	
    temp=Slave;

    Start();
    Write_Byte(temp);
    status = Check_ACK();
    if(!status) 
    {
        Write_Byte(Regis_Addr);
        status = Check_ACK();
        for(i=0; (i<n) && !status; i++)
        {
            Write_Byte(buf[i]);
            status = Check_ACK();
        }
    }
    Stop();

    return !status;
}

extern UINT8 qma_DevIndex;
#if 0
UINT8 QMA_WriteReg(UINT8 Regis_Addr, UINT8 Data)
{
    return QMA_I2C_WRITE(Regis_Addr, Data);
}

UINT8 QMA_ReadReg(UINT8 Regis_Addr )
{
    UINT8 Regis_Data;

    QMA_I2C_READ(Regis_Addr, &Regis_Data);

    return Regis_Data;
}
#endif
UINT8 QMA_I2C_WRITE(UINT8 Regis_Addr, UINT8 Data)
{
    UINT8 status;
    UINT8 tryCount = I2C_TIMEOUT_COUNT;

    qma_i2c = 1;	
    while(--tryCount) 
    {
        	status = I2C_Write_1byte(I2C_DEV0_ADDRESS, Regis_Addr, Data);
		
        	if(status) 
			break;
    }
	
    if(!tryCount) 
    {
    SPRINTF(("QMA_I2C_WRITE error!\n"));    
        return 0;
    }
    qma_i2c = 0;
	
    return status;
}
/**************************************************************
return :
		1: successful to read
		0: failed to read
*************************************************************/
UINT8 QMA_I2C_READ(UINT8 Regis_Addr, UINT8 *Regis_Data)
{
    UINT8 status = 0;
    UINT8 tryCount = I2C_TIMEOUT_COUNT;
	 
    qma_i2c = 1;
    while(--tryCount) 
    {
		status = I2C_Read_1byte(I2C_DEV0_ADDRESS, Regis_Addr, Regis_Data);
		
        	if(status) 
			break;
    }
	
    if(!tryCount) 
    {
		SPRINTF(("QMA_I2C_READ error!\n"));    
    }
    qma_i2c = 0;
	
    return status;
}

/**************************************************************
return :
		1: successful to read
		0: failed to read
*************************************************************/
UINT8 QMA_I2C_NREAD(UINT8 Regis_Addr, UINT8 *buf, UINT8 n)
{
    UINT8 status = 0;
    UINT8 tryCount = I2C_TIMEOUT_COUNT;
	
    qma_i2c = 1;	
    while(--tryCount) 
    {
        status = I2C_Read_nbyte(I2C_DEV0_ADDRESS, Regis_Addr, buf, n);
		
        if(status) 
		break;
    }
	
    if(!tryCount) 
    {
		SPRINTF(("QMA_I2C_READ error!\n"));    
    }
    qma_i2c = 0;

    return status;
}
/**************************************************************
return :
		1: successful to write
		0: failed to write
*************************************************************/
UINT8 QMA_I2C_NWRITE(UINT8 Regis_Addr, UINT8 *buf, UINT8 n)
{
    UINT8 status = 0;
	
    UINT8 tryCount = I2C_TIMEOUT_COUNT;
	
    qma_i2c = 1;	
    while(--tryCount) 
    {
        status = I2C_Write_nbyte(I2C_DEV0_ADDRESS, Regis_Addr, buf, n);
		
        if(status) 
		break;
    }
	
    if(!tryCount) 
    {
        SPRINTF(("QMA_I2C_READ error!\n"));    
    }
    qma_i2c = 0;
	
    return status;
}
#if 0
UINT8 QMA_WriteReg(UINT8 Regis_Addr,UINT8 Data)
{
    return QMA_I2C_WRITE(Regis_Addr,Data);
}

UINT8 QMA_ReadReg(UINT8 Regis_Addr) 
{
    UINT8 Data;
    
    Data = QMA_I2C_READ(Regis_Addr);
    return Data;
}
#endif//add in i2csample.c

/*************************************************************************************************************
                              end file
 ************************************************************************************************************/
#endif
