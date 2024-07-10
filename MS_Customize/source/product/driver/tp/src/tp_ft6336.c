/******************************************************************************
 ** File Name:      tp_ft6336.c                                               *
 ** Author:         Sunny.Fei                                                 *
 ** DATE:           2011.03.15                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2011.03.15   Sunny.Fei             Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "tp_multi_cfg.h"
#include "tp_ft6336.h"
#include "gpio_prod_api.h"
#include "gpio_drv.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
//#define TP_FIRMWARE_UPDATE
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define TP_I2C_PORT                 TPC_GetCtrlPort()


//The internal of reg of every finger is the same.
#define REG_FINGER_INTERVAL     0x06

//0x6A<<1 (01101010) (11010100)
#define TP_FT6336_I2C_FW_ADDR         0xD4

//0x2A<<1 (00101010) (01010100)
//#define TP_FT6336_I2C_ADDR       0x54

//0x38<<1 (00111000) (01110000)
#define TP_FT6336_I2C_ADDR       0x70

#define    FTS_PACKET_LENGTH        128
TG_STATE_E tpc_gesture_id = TG_UNKNOWN_STATE;

/*This chip Coordinate has 12bit-data in differnet two regeristers.*/
#define XY_Coordinate_Transform(_data, _high_data, _low_data) do{   \
        _data = ((_high_data & 0xf) << 8) | _low_data;                      \
            }while(0)
            
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                        Global/Local Variables                             *
 **---------------------------------------------------------------------------*/
#ifdef TP_FIRMWARE_UPDATE
LOCAL uint8  CTPM_FW[]=
{
	#include "ft_app.h"
};
#endif
LOCAL I2C_DEV FT6336_I2C_fm_cfg =
{
    4,          
    100000,                   // freq is standard for FT6336
    0,                          // bus
    TP_FT6336_I2C_FW_ADDR,  // slave_addr
    2,                          // reg_addr_num
    1,                          // check_ack
    0                           // no_stop
};

LOCAL I2C_DEV FT6336_I2C_cfg =
{
    4,          
    100000,                   // freq is standard for FT6336
    0,                          // bus
    TP_FT6336_I2C_ADDR,  // slave_addr
    1,                          // reg_addr_num
    1,                          // check_ack
    0                           // no_stop
};

LOCAL int32 FT6336_I2C_FW_Handler = -1;
LOCAL int32 FT6336_I2C_Handler = -1;

XY_DATA_T XY_Coordinate[MAX_FINGER_NUM]={0};
uint16 Finger_Num = 0;
TP_STATE_E tp_event = TP_PEN_NONE;
uint8 event_count[3] = {0};//0,down;1,move;2,up

extern PUBLIC uint32 OS_TickDelay(uint32 ticks);
LOCAL int _FT6336_Reg_Initial(void);
LOCAL uint32 _Get_FT6336_I2C_FW_Status(void);
LOCAL uint32 _Get_FT6336_I2C_Status(void);
LOCAL uint32 _FT6336_Read_RegAll(void);

static void CST_DelayMs(int ms_delay)
{
    uint32 wait_time = 0;
    uint32 wait_time2 = 0;
    wait_time2 = ms_delay*16384/1000;
    wait_time = timer_TimRealTickGet(2);  //TIMER_2

    while(1)
    {
        if((timer_TimRealTickGet(2) - wait_time) > wait_time2)
        {
            return;
        }
    }
}

/******************************************************************************/
// Description: FT6336_I2C_Open
// Dependence: 
// Note:
/******************************************************************************/

LOCAL uint32 FT6336_I2C_Open(void)
{
    FT6336_I2C_cfg.id =  TP_I2C_PORT;
    
    if(FT6336_I2C_Handler == -1) 
    {
        if((FT6336_I2C_Handler = I2C_HAL_Open(&FT6336_I2C_cfg)) == -1)
        {
            return TP_MULTI_ERROR;    
        } 
        else
        {
            return TP_MULTI_SUCCESS;
        }
    }
    else
    {
        return TP_MULTI_SUCCESS;
        
    }
}

/******************************************************************************/
// Description: FT6336_I2C_Close
// Dependence: 
// Note:
/******************************************************************************/

LOCAL uint32 FT6336_I2C_Close(void)
{
    if(_Get_FT6336_I2C_Status() == TP_MULTI_SUCCESS)
	{
 		I2C_HAL_Close(FT6336_I2C_Handler);
		FT6336_I2C_Handler = -1;
	}	
    return TP_MULTI_SUCCESS;
}
/******************************************************************************/
// Description: FT6336_I2C_Write
// Dependence: 
// Note:
/******************************************************************************/
LOCAL void FT6336_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_FT6336_I2C_Status() == TP_MULTI_SUCCESS)
        I2C_HAL_Write(FT6336_I2C_Handler, reg, addr, cnt);
}


/******************************************************************************/
// Description: FT6336_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
LOCAL void FT6336_I2C_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_FT6336_I2C_Status() == TP_MULTI_SUCCESS)
        I2C_HAL_Read(FT6336_I2C_Handler,reg, addr, cnt);
}


/******************************************************************************/
// Description: _Get_FT6336_I2C_Status
// Dependence:  It can get the status from i2c
// Note:
/******************************************************************************/
LOCAL uint32 _Get_FT6336_I2C_Status(void)
{
    if(FT6336_I2C_Handler==-1)
    {
        return TP_MULTI_ERROR;
    }
    else
    {
        return TP_MULTI_SUCCESS;
    }
}

LOCAL uint32 FT6336_I2C_FW_Open(void)
{
    FT6336_I2C_fm_cfg.id =  TP_I2C_PORT;
    
    if(FT6336_I2C_FW_Handler == -1) 
    {
        if((FT6336_I2C_FW_Handler = I2C_HAL_Open(&FT6336_I2C_fm_cfg)) == -1)
        {
            return TP_MULTI_ERROR;    
        } 
        else
        {
            return TP_MULTI_SUCCESS;
        }
    }
    else
    {
        return TP_MULTI_SUCCESS;
    }
}

LOCAL uint32 FT6336_I2C_FW_Close(void)
{
    if(_Get_FT6336_I2C_FW_Status() == TP_MULTI_SUCCESS)
	{
 		I2C_HAL_Close(FT6336_I2C_FW_Handler);
		FT6336_I2C_FW_Handler = -1;
	}	
    return TP_MULTI_SUCCESS;
}

LOCAL void FT6336_I2C_FW_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_FT6336_I2C_FW_Status() == TP_MULTI_SUCCESS)
        I2C_HAL_Write(FT6336_I2C_FW_Handler, reg, addr, cnt);
}

/******************************************************************************/
// Description: FT6336_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
LOCAL void FT6336_I2C_FW_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_FT6336_I2C_FW_Status() == TP_MULTI_SUCCESS)
        I2C_HAL_Read(FT6336_I2C_FW_Handler,reg, addr, cnt);
}

LOCAL uint32 _Get_FT6336_I2C_FW_Status(void)
{
    if(FT6336_I2C_FW_Handler==-1)
    {
        return TP_MULTI_ERROR;
    }
    else
    {
        return TP_MULTI_SUCCESS;
    }
}

int8 CTP_FLASH_I2C_WRITE(uint16 reg2, uint8 *value, uint16 len) 
{
    uint8 reg[3] = {0};
    reg[0] = (uint8)(reg2 >> 8);
    reg[1] = (uint8)(reg2 & 0x00FF);
    if(_Get_FT6336_I2C_FW_Status() == TP_MULTI_SUCCESS)
    {
        if(0 < I2C_HAL_Write(FT6336_I2C_FW_Handler, &reg, value, len))
        {
            return 0;
        }
    }
    return 1;
}

int8 CTP_FLASH_I2C_READ(uint16 reg2, uint8 *value, uint16 len) 
{
    uint8 reg[3] = {0};
    reg[0] = (uint8)(reg2 >> 8);
    reg[1] = (uint8)(reg2 & 0x00FF);
    if(_Get_FT6336_I2C_FW_Status() == TP_MULTI_SUCCESS)
    {
        if(0 <  I2C_HAL_Read(FT6336_I2C_FW_Handler, &reg, value, len))
        {
            return 0;
        }
    }
    return 1;
}


#ifdef TP_FIRMWARE_UPDATE

/******************************************************************************/
// Description: Firmware_data_transfer
// Dependence:  
// Note:
/******************************************************************************/
LOCAL void cmd_write(uint8 btcmd, uint8 btPara1,uint8 btPara2,uint8 btPara3,uint8 num)
{
    uint8 write_cmd[4] = {0};

    write_cmd[0] = btcmd;
    write_cmd[1] = btPara1;
    write_cmd[2] = btPara2;
    write_cmd[3] = btPara3;
    I2C_HAL_Write(i2c_handle_tp_firmware, NULL, write_cmd, num);
}


LOCAL int Firmware_data_transfer(uint8* pbt_buf, int dw_lenth)
{
	uint8 reg_val[2] = {0,0};
	uint8  packet_buf[FTS_PACKET_LENGTH + 6];
	uint8  auc_i2c_write_buf[10];
	uint8  bt_ecc;
	
	int  j,temp,lenght,i_ret,packet_number, i = 0;
    	int  i_is_new_protocol = 0;

	FT6336_I2C_fm_cfg.id =  TP_I2C_PORT;
	i2c_handle_tp_firmware  = I2C_HAL_Open(&FT6336_I2C_fm_cfg);
	if(i2c_handle_tp_firmware != -1 )
	{
		OS_TickDelay(50);
    		/**************Step 1:Reset  CTPM ***********/
    		/************write 0xaa to register 0xfc********/
		reg_val[0] = 0xfc;
		reg_val[1] =0xaa;
		I2C_HAL_Write(i2c_handle_tp_firmware, NULL, reg_val,2);
		OS_TickDelay(50);
	
		/************write 0x55 to register 0xfc********/
		reg_val[0] =0xfc;
		reg_val[1] =0x55;
		I2C_HAL_Write(i2c_handle_tp_firmware, NULL, reg_val,2);
		OS_TickDelay(30);   

		/**************Step 2:Enter upgrade mode *****/
		auc_i2c_write_buf[0] = 0x55;
		auc_i2c_write_buf[1] = 0xaa;
		do
		{
			i ++;
			i_ret = I2C_HAL_Write(i2c_handle_tp_firmware, NULL, auc_i2c_write_buf, 2);
			OS_TickDelay(5);
		}while(i_ret <= 0 && i < 10 );

		if (i > 1)
		{
			i_is_new_protocol = 1;
		}

		/**************Step 3:check READ-ID**********/        
		cmd_write(0x90,0x00,0x00,0x00,4);
		I2C_HAL_Read(i2c_handle_tp_firmware, NULL,  reg_val, 2);
		if (reg_val[0] == 0x79 && reg_val[1] == 0x3)
		{
			//SCI_TRACE_LOW:"[TSP] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT6336_241_112_2_18_0_35_2_2071,(uint8*)"dd",reg_val[0],reg_val[1]);
		}
		else
		{
			//SCI_ASSERT(0);
        		return TP_ERR_READID;
    		}
    
		/**************Step 4:erase app**************/
		if (i_is_new_protocol)
		{
			cmd_write(0x61,0x00,0x00,0x00,1);
		}
		else
		{
			cmd_write(0x60,0x00,0x00,0x00,1);
		}
    		OS_TickDelay(1500);
    		//SCI_TRACE_LOW:"[TSP] Step 4: erase. \n"
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT6336_259_112_2_18_0_35_2_2072,(uint8*)"");

		/**************Step 5:write firmware(FW) to ctpm flash*****/
		bt_ecc = 0;
		//SCI_TRACE_LOW:"[TSP] Step 5: start upgrade. \n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT6336_263_112_2_18_0_35_2_2073,(uint8*)"");
		dw_lenth = dw_lenth - 8;
		packet_number = (dw_lenth) / FTS_PACKET_LENGTH;
		packet_buf[0] = 0xbf;
		packet_buf[1] = 0x00;
		for (j=0;j<packet_number;j++)
		{
			temp = j * FTS_PACKET_LENGTH;
			packet_buf[2] = (uint8)(temp>>8);
			packet_buf[3] = (uint8)temp;
			lenght = FTS_PACKET_LENGTH;
			packet_buf[4] = (uint8)(lenght>>8);
			packet_buf[5] = (uint8)lenght;

			for (i=0;i<FTS_PACKET_LENGTH;i++)
			{
				packet_buf[6+i] = pbt_buf[j*FTS_PACKET_LENGTH + i]; 
				bt_ecc ^= packet_buf[6+i];
			}
        
			I2C_HAL_Write(i2c_handle_tp_firmware, NULL, &packet_buf[0],FTS_PACKET_LENGTH + 6);
			OS_TickDelay(FTS_PACKET_LENGTH/6 + 1);
			if ((j * FTS_PACKET_LENGTH % 1024) == 0)
			{
				//SCI_TRACE_LOW:"[TSP] upgrade the 0x%x th byte.\n"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT6336_287_112_2_18_0_35_2_2074,(uint8*)"d", ((unsigned int)j) * FTS_PACKET_LENGTH);
			}
    		}

		if ((dw_lenth) % FTS_PACKET_LENGTH > 0)
		{
			temp = packet_number * FTS_PACKET_LENGTH;
			packet_buf[2] = (uint8)(temp>>8);
			packet_buf[3] = (uint8)temp;

			temp = (dw_lenth) % FTS_PACKET_LENGTH;
			packet_buf[4] = (uint8)(temp>>8);
			packet_buf[5] = (uint8)temp;

			for (i=0;i<temp;i++)
			{
				packet_buf[6+i] = pbt_buf[ packet_number*FTS_PACKET_LENGTH + i]; 
				bt_ecc ^= packet_buf[6+i];
			}

			I2C_HAL_Write(i2c_handle_tp_firmware, NULL, &packet_buf[0],temp+6);    
			OS_TickDelay(20);
		}

		/***************send the last six byte**************/
		for (i = 0; i<6; i++)
		{
			temp = 0x6ffa + i;
			packet_buf[2] = (uint8)(temp>>8);
			packet_buf[3] = (uint8)temp;
			temp =1;
			packet_buf[4] = (uint8)(temp>>8);
			packet_buf[5] = (uint8)temp;
			packet_buf[6] = pbt_buf[ dw_lenth + i]; 
			bt_ecc ^= packet_buf[6];

			I2C_HAL_Write(i2c_handle_tp_firmware, NULL, &packet_buf[0],7);  
			OS_TickDelay(20);
		}

		/********Step 6: read out checksum*********/
		/********send the opration head************/
		cmd_write(0xcc,0x00,0x00,0x00,1);
		I2C_HAL_Read(i2c_handle_tp_firmware, NULL,  reg_val,1);
		//SCI_TRACE_LOW:"[TSP] Step 6:  ecc read 0x%x, new firmware 0x%x. \n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT6336_331_112_2_18_0_35_3_2075,(uint8*)"dd", reg_val[0], bt_ecc);
		if(reg_val[0] != bt_ecc)
		{
			//SCI_ASSERT(0);
			return TP_ERR_ECC;
		}
		cmd_write(0x07,0x00,0x00,0x00,1);
	
		I2C_HAL_Close(i2c_handle_tp_firmware);
		i2c_handle_tp_firmware = -1;
	}
    /*******Step 7: reset the new FW**********/
    
	return TP_OK;
}

#endif

/******************************************************************************/
// Description: _FT6336_Config
// Dependence: 
// Note:
/******************************************************************************/
uint8 g_tp_read_id_buf[3] = {0};
uint8 ft6336_get_chip_id(uint8 *buf)
{
    //0xa3 = 0x64
    uint8 cmd = REG_FT_CHIP_ID;
    uint8 ret = 0x00;
    uint8 retry = 3;
    OS_TickDelay(50);
    for(retry=0;retry<3;retry++)
    {
        FT6336_I2C_Read(&cmd, buf,1);
        SCI_TRACE_LOW("[TP]:ft6336_get_chip_id[%d]=0x%x",retry,*buf);
        g_tp_read_id_buf[retry] = *buf;
        if(*buf == 0x64)
        {
            ret = 1;
            break;
        }
        OS_TickDelay(100);
    }
    return ret;
}

#if 1
LOCAL int _FT6336_Config(void)
{
    uint8 chip_id = 0;
    SCI_TRACE_LOW("[TP]:_FT6336_Config");
    
    if(ft6336_get_chip_id(&chip_id))
    {
        return TP_MULTI_SUCCESS;
    }
    return TP_MULTI_ERROR;
}
#else
LOCAL int _FT6336_Config(void)
{

	uint8 reg_addr, val_write;
#ifdef TP_FIRMWARE_UPDATE
   	uint8*     pbt_buf = PNULL;
	uint8 Reg_Data = 0x10;
   	int i_ret;


	reg_addr = REG_FIRMWARE_ID;
    	FT6336_I2C_Read(&reg_addr,&Reg_Data, 1);
	if(Reg_Data != 0x57)
	{
		pbt_buf = CTPM_FW;
		//SCI_TRACE_LOW:"FW FIRMWARE_ID = 0x%x\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT6336_367_112_2_18_0_35_3_2076,(uint8*)"d", Reg_Data);
		/*call the upgrade function*/
		i_ret =  Firmware_data_transfer(pbt_buf,sizeof(CTPM_FW));
		OS_TickDelay(350);
		reg_addr = REG_CALI_TPC;
		val_write = 0x4;
		FT6336_I2C_Write(&reg_addr, &val_write, 1);
		OS_TickDelay(4500);
		FT6336_I2C_Read(&reg_addr, &Reg_Data, 1);
		if(Reg_Data == 0x1)
		{
			//SCI_TRACE_LOW:"TPC UPDATE FIRMWARE SUCCESS\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT6336_378_112_2_18_0_35_3_2077,(uint8*)"");
		}
		else
		{
			//SCI_TRACE_LOW:"TPC UPDATE FIRMWARE FAILED\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT6336_382_112_2_18_0_35_3_2078,(uint8*)"");
		}
	}
	else
#endif
	{
		reg_addr = REG_VALID_DETECT;//finger detection is little size, more sensitivity
		val_write= TOUCH_VALID_THRESHOLD;//default:70
		FT6336_I2C_Write(&reg_addr, &val_write, 1);
		
        
		reg_addr = REG_ZOOM_DISTANCE;//zoom in/out is little size , more sensitivity
		val_write = TOUCH_VAL_ZOOM_THRESHOLD;
		FT6336_I2C_Write(&reg_addr, &val_write, 1);
            
              reg_addr = REG_PERIODACTIVE;
		val_write = TOUCH_PERIODACTIVE;
		FT6336_I2C_Write(&reg_addr, &val_write, 1);
             _FT6336_Read_RegAll();
	}
      return TP_MULTI_SUCCESS;
}
#endif

/******************************************************************************/
// Description: FT6336_Initial
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint32 FT6336_Initial(void)
{
    return _FT6336_Reg_Initial();//CHIP REG SET
    
}


/******************************************************************************/
// Description: _FT6336_XY_LOG
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint32 _FT6336_XY_LOG()
{
    int16 i;

    i= 0;
    while(i<MAX_FINGER_NUM)
    {
        SCI_TRACE_LOW("TP: XY_Coordinate Finger[%d] x=%d, y=%d, finger_id = 0x%x!rn", i, XY_Coordinate[i].x_position,XY_Coordinate[i].y_position, XY_Coordinate[i].finger_id);
        i++;
    }
}

/******************************************************************************/
// Description: _FT6336_Read_RegAll
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint32 _FT6336_Read_RegAll(void)
{
    uint8 Reg_Read=0;
    uint8 Reg_value[10]={0};
    int16 i=0;
    FT6336_I2C_Read(&Reg_Read, Reg_value, 7);
    for(i=0;i<7;i++)
    {
        SCI_TRACE_LOW("FT6336 Reg[0x%x]=0x%x \r\n", i,Reg_value[i]);
    }
    Reg_Read = 0x80;
    FT6336_I2C_Read(&Reg_Read, Reg_value, 10);
    for(i=0;i<10;i++)
    {
        SCI_TRACE_LOW("FT6336 Reg[0x80+%x]=0x%x \r\n", i,Reg_value[i]);
    }
    Reg_Read = 0x92;
    FT6336_I2C_Read(&Reg_Read, Reg_value, 10);
    for(i=0;i<10;i++)
    {
        SCI_TRACE_LOW("FT6336 Reg[0x92+%x]=0x%x \r\n", i,Reg_value[i]);
    }
    Reg_Read = 0x9c;
    FT6336_I2C_Read(&Reg_Read, Reg_value, 10);
    for(i=0;i<10;i++)
    {
        SCI_TRACE_LOW("FT6336 Reg[0x9c+%x]=0x%x \r\n", i,Reg_value[i]);
    }
    Reg_Read = 0xa6;
    FT6336_I2C_Read(&Reg_Read, Reg_value, 8);
    for(i=0;i<8;i++)
    {
        SCI_TRACE_LOW("FT6336 Reg[0xa6+%x]=0x%x \r\n", i,Reg_value[i]);
    }
    return TP_MULTI_SUCCESS;
}


/******************************************************************************/
// Description: _Get_Gesture_Data
// Dependence:  get the gesture data about capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL uint32 _Get_Gesture_Data(void)
{
    tpc_gesture_id = TG_NO_DETECT;
    Finger_Num = 1;

    return TP_MULTI_SUCCESS;
}



/******************************************************************************/
// Description: _Get_Finger_Coordinate
// Dependence:  get coordinate about capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL uint32 _Get_Finger_Coordinate(void)
{
    uint8 Coordinate_Num = 0,reg=0;
    uint8 Reg_Data[REG_FINGER_INTERVAL*MAX_FINGER_NUM]={0};
    int16 i=0;

    //according to finger num,read finger's message.
    Coordinate_Num = Finger_Num;
    if(Coordinate_Num == 0)//Because it will be read message, when pen up
        Coordinate_Num = 1;//for read 6 bytes.
    
    reg = REG_1ST_FINGER;
    FT6336_I2C_Read(&reg, Reg_Data, REG_FINGER_INTERVAL*Coordinate_Num);
    //SCI_TRACE_LOW("[TP]:Reg_0x03=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",Reg_Data[0],Reg_Data[1],Reg_Data[2],Reg_Data[3],Reg_Data[4],Reg_Data[5]);
    //finger's message gives to global variable.
    memset(XY_Coordinate, 0, sizeof(XY_Coordinate));
    memset(event_count,0, sizeof(event_count));
    for (i=0; i<Coordinate_Num;i++)
    {
        XY_Coordinate_Transform(XY_Coordinate[i].x_position, Reg_Data[i*REG_FINGER_INTERVAL], Reg_Data[i*REG_FINGER_INTERVAL+1]);
        XY_Coordinate_Transform(XY_Coordinate[i].y_position, Reg_Data[i*REG_FINGER_INTERVAL+2], Reg_Data[i*REG_FINGER_INTERVAL+3]);
        XY_Coordinate[i].finger_id = (Reg_Data[i*REG_FINGER_INTERVAL+2] & 0xf0)>>4;/*low byte is Touch ID*/
        switch((Reg_Data[i*REG_FINGER_INTERVAL] & 0xc0) >>6 )//for example: reg0x3 [7:6]
        {
            case 2://pen move
                XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_MOVE) << 8;/*high byte is Event Flag*/
                event_count[1]++;
                break;
            case 1://pen up
                XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_UP) << 8;/*high byte is Event Flag*/
                event_count[2]++;
                break;
            case 0://pen down
                XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_DOWN) << 8;/*high byte is Event Flag*/
                event_count[0]++;
                break;
            default:
                XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_NONE) << 8;/*high byte is Event Flag*/
                break;
        }        
    }
}

/******************************************************************************/
// Description: _Get_Cal_Msg
// Dependence:  get event message about capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL TP_STATE_E _Get_Cal_Msg(void)
{
    //give the event mode,because it can get from register.
    //SCI_TRACE_LOW("TP: event_count [0]:0x%x [1]:0x%x  [2]:0x%x \r\n", event_count[0],event_count[1],event_count[2]);
    if((event_count[0]!=0) && (event_count[1] ==0) && (event_count[2] ==0))
       tp_event = TP_PEN_DOWN;
    else if( (event_count[0] ==0) && (event_count[1] == 0) && (event_count[2] !=0))
       tp_event = TP_PEN_UP;
    else if(event_count[1] !=0)
       tp_event = TP_PEN_MOVE;
    else 
       tp_event = TP_PEN_NONE;
    return tp_event;
}

/******************************************************************************/
// Description: FT6336_XY_Data
// Dependence:  get the most data about capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL uint32 FT6336_XY_Data(void)
{
    //SCI_TRACE_LOW("[TP]:FT6336_XY_Data");
     //get gesture
     _Get_Gesture_Data();
     
     //get coordinate
     _Get_Finger_Coordinate();
}


/******************************************************************************/
// Description: FT6336_Read
// Dependence:  GETTING the data about touchpanel. the up-layer calls the 
//              function to get the data.
// Note:
/******************************************************************************/
LOCAL uint32 FT6336_Read(TPDSVR_SIG_T *data)
{
    int16 i;
    //SCI_TRACE_LOW("[TP]:FT6336_Read");
    FT6336_XY_Data();
    data->x_key = XY_Coordinate[0].x_position;
    data->y_key = XY_Coordinate[0].y_position;
    data->cur_index = XY_Coordinate[0].finger_id;
    
    data->gesture_type = tpc_gesture_id;
    data->num_of_point = Finger_Num;
    
    if(Finger_Num>1)
    {
        for(i=1;i<Finger_Num;i++)
        {
            data->xy_position[i-1].x_position = XY_Coordinate[i].x_position;
            data->xy_position[i-1].y_position = XY_Coordinate[i].y_position;
            data->xy_position[i-1].finger_id = XY_Coordinate[i].finger_id;
        }
    }
    
    switch(_Get_Cal_Msg())
    {
        case TP_PEN_DOWN:
            data->SignalCode = TP_DOWN_MSG;
            break;
        case TP_PEN_UP:
            data->SignalCode = TP_UP_MSG;
            break;
        case TP_PEN_MOVE:
            data->SignalCode = TP_MOVE_MSG;
            break;
            default:
            data->SignalCode = TP_MAX_MSG;
            break;
    }       
    //SCI_TRACE_LOW("[TP]:FT6336_Read Finger_Num=%d,x=%d,y=%d,SignalCode=0x%x",Finger_Num,data->x_key,data->y_key,data->SignalCode);
    return 0;
}


/******************************************************************************/
// Description: _FT6336_Reg_Initial
// Dependence:  Initial capacitive touchpanel.
// Note:
/******************************************************************************/

LOCAL int _FT6336_Reg_Initial(void)
{
    FT6336_I2C_Open();
    if(_Get_FT6336_I2C_Status() == TP_MULTI_SUCCESS)
    {
        uint32 delay_time = TP_MAX_SLEEPTIME;
        GPIO_ResetTP(1,delay_time);
        return _FT6336_Config();
    }
    else
    {
        SCI_TRACE_LOW("[TP]:_FT6336_Reg_Initial I2C Err");
        return TP_MULTI_ERROR;
    }
}

/******************************************************************************/
// Description: FT6336_Open
// Dependence:  Open capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL int FT6336_Open(void)
{
    FT6336_I2C_Open();
    return TP_MULTI_SUCCESS;
}

/******************************************************************************/
// Description: FT6336_Write
// Dependence:  
// Note:
/******************************************************************************/
LOCAL int FT6336_Write(void)
{
    return TP_MULTI_SUCCESS;
}

LOCAL uint32 FT6336_Ioctl(uint32 cmd, void *arg)
{
    BOOLEAN gpio_interrupt_value;
     uint8 reg_addr,val_read, val_write;
     TP_SPEED_MODE_E speed_mode;
    switch(cmd)
    {
        case TP_IOCTL_CMD_SPEED_MODE:
            speed_mode = *(TP_SPEED_MODE_E *)arg;
            gpio_interrupt_value = GPIO_GetValue(GPIO_TPInterruptPin());
            while((uint16)!gpio_interrupt_value == GPIO_TPGetInterruptStatus())
            {
                reg_addr = REG_PERIODACTIVE;
                if(speed_mode == TP_SPEED_MODE_FAST)
                    val_write = 6;// about only 100uA
                 else 
                    val_write = 4;
                FT6336_I2C_Write(&reg_addr, &val_write, 1);
                break;
            }
            break;
        case TP_IOCTL_GET_INT_TIME:
            
            reg_addr = REG_PERIODACTIVE;
            
            FT6336_I2C_Read(&reg_addr, &val_read, 1);
            
            *((uint8 *)arg) = 100/val_read;
            
            SCI_TRACE_LOW("value freq_cycle time = 0x%x\n", *((uint8 *)arg));
            
            break;

        case TP_IOCTL_SET_THRES_ZOOM:
            reg_addr = REG_ZOOM_DISTANCE;
            val_write = *((uint16 *)arg);
            FT6336_I2C_Write(&reg_addr, &val_write, 1);
            FT6336_I2C_Read(&reg_addr, &val_read, 1);
            SCI_TRACE_LOW("value zoom_distance = 0x%x\n", val_read);
            break;

            default:
                break;
    }
    return TP_MULTI_SUCCESS;
}
/******************************************************************************/
// Description: FT6336_Suspend
// Dependence:  When the phone enter sleep mode for saving.
// Note:
/******************************************************************************/
LOCAL void FT6336_Suspend(void)
{
    uint8 reg_addr, val_write;

    reg_addr = REG_CHIP_MODE;
    val_write = CHIP_WORK_MODE_HIBERNATE;// about only 100uA
    FT6336_I2C_Write(&reg_addr, &val_write, 1);
    //ZDT_LogTrace("TP: FT6336_Suspend OK");
}


/******************************************************************************/
// Description: FT6336_Resume
// Dependence:  
// Note:
/******************************************************************************/
LOCAL void FT6336_Resume(void)
{
    uint8 i = 0;
    uint8 reg_addr = REG_CHIP_MODE;
    uint8 val_read = 0xff;
    uint32 delay_time = TP_MAX_SLEEPTIME;

    /*CHIP WAKE PIN  least time is 5ms in LOW level when reset according to SPEC.*/
    GPIO_ResetTP(1,delay_time);
    #if 0
    if(0 == ft6336_get_chip_id(&val_read))
    {
        GPIO_ResetTP(1,delay_time);
        ft6336_get_chip_id(&val_read);
    }
    #else
	FT6336_I2C_Read(&reg_addr, &val_read, 1);
	for(i=0; i<5; i++)
	{
		if(val_read != 0x0)//work mode
		{
			FT6336_Suspend();
			OS_TickDelay(10);
			GPIO_ResetTP(1,delay_time );
			reg_addr = REG_CHIP_MODE;
			FT6336_I2C_Read(&reg_addr, &val_read, 1);
			if(val_read != 0x0)
			{
				//ZDT_LogTrace("TP: FT6336_Resume Retry Count(%d),value=0x%x \r\n", i+1,val_read);
			}
		}
		else
		{
			break;
		}
	}
    #endif
    //ZDT_LogTrace("TP: FT6336_Resume val_read=0x%02x, 0x%02x,0x%02x,0x%02x",val_read,g_tp_read_id_buf[0],g_tp_read_id_buf[1],g_tp_read_id_buf[2]);
    return;
}
/******************************************************************************/
// Description: FT6336_Close
// Dependence:  Close capacitive touchpanel.
// Note:
/******************************************************************************/

LOCAL int FT6336_Close(void)
{
    FT6336_I2C_Close();
    return TP_MULTI_SUCCESS;
}

/******************************************************************************/
// Description: FT6336_Deinitial
// Dependence:  Deinitial capacitive touchpanel.
// Note:
/******************************************************************************/

LOCAL int FT6336_Deinitial(void)
{
    FT6336_I2C_Close();
    return TP_MULTI_SUCCESS;
}

PUBLIC TPC_OPERATION_T tpc_ft6336_ops=
{
    FT6336_Initial,
    FT6336_Open,
    FT6336_Write,
    FT6336_Read,
    FT6336_Ioctl,
    FT6336_Suspend,
    FT6336_Resume,
    FT6336_Close,
    FT6336_Deinitial   
};


int TP_FT6336_Test(void)
{
    //SCI_TRACE_LOW("[TP]:TP_FT6336_Test");
    FT6336_Initial();
    return TP_MULTI_SUCCESS;
}

/******************************************************************************/
