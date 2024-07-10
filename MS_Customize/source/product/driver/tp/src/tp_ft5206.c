/******************************************************************************
 ** File Name:      tp_ft5206.c                                               *
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
#include "tp_ft5206.h"
#include "gpio_prod_api.h"
#include "gpio_drv.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
 #define TP_FIRMWARE_UPDATE
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define TP_I2C_PORT                 TPC_GetCtrlPort()


//The internal of reg of every finger is the same.
#define REG_FINGER_INTERVAL     0x06
#define TP_FT5206_I2C_WRITE_ADDR       0x70
#define TP_FT5206_I2C_READ_ADDR         0x71

#define    FTS_PACKET_LENGTH        128
TG_STATE_E tpc_gesture_id = TG_UNKNOWN_STATE;

/*This chip Coordinate has 12bit-data in differnet two regeristers.*/
#define XY_Coordinate_Transform(_data, _high_data, _low_data) do{   \
        _data = ((_high_data & 0xf) << 8) | _low_data;                      \
            }while(0)
            
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
LOCAL I2C_DEV FT5206_I2C_cfg =
{
    3,          
    400000,                   // freq is standard for FT5206
    0,                          // bus
    TP_FT5206_I2C_WRITE_ADDR,  // slave_addr
    1,                          // reg_addr_num
    1,                          // check_ack
    1                           // no_stop
};

#ifdef TP_FIRMWARE_UPDATE
LOCAL I2C_DEV FT5206_I2C_fm_cfg =
{
    3,          
    400000,                   // freq is standard for FT5206
    0,                          // bus
    TP_FT5206_I2C_WRITE_ADDR,  // slave_addr
    0,                          // reg_addr_num
    1,                          // check_ack
    1                           // no_stop
};

LOCAL int32 i2c_handle_tp_firmware = -1;
#endif

LOCAL int32 FT5206_I2C_Handler = -1;

XY_DATA_T XY_Coordinate[MAX_FINGER_NUM]={0};
uint16 Finger_Num = 0;
TP_STATE_E tp_event = TP_PEN_NONE;
uint8 event_count[3] = {0};//0,down;1,move;2,up

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

/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/
extern PUBLIC uint32 OS_TickDelay(uint32 ticks);
LOCAL int _FT5206_Reg_Initial(void);
LOCAL uint32 _Get_FT5206_I2C_Status(void);
LOCAL uint32 _FT5206_Read_RegAll(void);
LOCAL int Firmware_data_transfer(uint8* pbt_buf, int dw_lenth);
/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: FT5206_I2C_Open
// Dependence: 
// Note:
/******************************************************************************/

LOCAL uint32 FT5206_I2C_Open(void)
{
    FT5206_I2C_cfg.id =  TP_I2C_PORT;
    if(FT5206_I2C_Handler == -1) 
    {
        
        if((FT5206_I2C_Handler = I2C_HAL_Open(&FT5206_I2C_cfg)) == -1)
        {
            
            //SCI_TRACE_LOW:"TP:: open I2C failed!\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_114_112_2_18_0_35_2_2069,(uint8*)"");
            return TP_MULTI_ERROR;    
        } 
        else
            return TP_MULTI_SUCCESS;
    }
    else
    {
        //SCI_TRACE_LOW:"TP:: open I2C success!\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_122_112_2_18_0_35_2_2070,(uint8*)"");
        return TP_MULTI_SUCCESS;
        
    }
}

/******************************************************************************/
// Description: FT5206_I2C_Close
// Dependence: 
// Note:
/******************************************************************************/

LOCAL uint32 FT5206_I2C_Close(void)
{
    if(_Get_FT5206_I2C_Status() == TP_MULTI_SUCCESS)
	{
 		I2C_HAL_Close(FT5206_I2C_Handler);
		FT5206_I2C_Handler = -1;
	}	
    return TP_MULTI_SUCCESS;
}
/******************************************************************************/
// Description: FT5206_I2C_Write
// Dependence: 
// Note:
/******************************************************************************/
LOCAL void FT5206_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_FT5206_I2C_Status() == TP_MULTI_SUCCESS)
        I2C_HAL_Write(FT5206_I2C_Handler, reg, addr, cnt);
}


/******************************************************************************/
// Description: FT5206_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
LOCAL void FT5206_I2C_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_FT5206_I2C_Status() == TP_MULTI_SUCCESS)
        I2C_HAL_Read(FT5206_I2C_Handler,reg, addr, cnt);
}


/******************************************************************************/
// Description: _Get_FT5206_I2C_Status
// Dependence:  It can get the status from i2c
// Note:
/******************************************************************************/
LOCAL uint32 _Get_FT5206_I2C_Status(void)
{
    if(FT5206_I2C_Handler==-1)
    {
        return TP_MULTI_ERROR;
    }
    else
    {
        return TP_MULTI_SUCCESS;
    }
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

	FT5206_I2C_fm_cfg.id =  TP_I2C_PORT;
	i2c_handle_tp_firmware  = I2C_HAL_Open(&FT5206_I2C_fm_cfg);
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
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_241_112_2_18_0_35_2_2071,(uint8*)"dd",reg_val[0],reg_val[1]);
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
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_259_112_2_18_0_35_2_2072,(uint8*)"");

		/**************Step 5:write firmware(FW) to ctpm flash*****/
		bt_ecc = 0;
		//SCI_TRACE_LOW:"[TSP] Step 5: start upgrade. \n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_263_112_2_18_0_35_2_2073,(uint8*)"");
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
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_287_112_2_18_0_35_2_2074,(uint8*)"d", ((unsigned int)j) * FTS_PACKET_LENGTH);
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
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_331_112_2_18_0_35_3_2075,(uint8*)"dd", reg_val[0], bt_ecc);
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
// Description: _FT5206_Config
// Dependence: 
// Note:
/******************************************************************************/
LOCAL void _FT5206_Config(void)
{

	uint8 reg_addr, val_write;
#ifdef TP_FIRMWARE_UPDATE
   	uint8*     pbt_buf = PNULL;
	uint8 Reg_Data = 0x10;
   	int i_ret;


	reg_addr = REG_FIRMWARE_ID;
    	FT5206_I2C_Read(&reg_addr,&Reg_Data, 1);
	if(Reg_Data != 0x57)
	{
		pbt_buf = CTPM_FW;
		//SCI_TRACE_LOW:"FW FIRMWARE_ID = 0x%x\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_367_112_2_18_0_35_3_2076,(uint8*)"d", Reg_Data);
		/*call the upgrade function*/
		i_ret =  Firmware_data_transfer(pbt_buf,sizeof(CTPM_FW));
		OS_TickDelay(350);
		reg_addr = REG_CALI_TPC;
		val_write = 0x4;
		FT5206_I2C_Write(&reg_addr, &val_write, 1);
		OS_TickDelay(4500);
		FT5206_I2C_Read(&reg_addr, &Reg_Data, 1);
		if(Reg_Data == 0x1)
		{
			//SCI_TRACE_LOW:"TPC UPDATE FIRMWARE SUCCESS\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_378_112_2_18_0_35_3_2077,(uint8*)"");
		}
		else
		{
			//SCI_TRACE_LOW:"TPC UPDATE FIRMWARE FAILED\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_382_112_2_18_0_35_3_2078,(uint8*)"");
		}
	}
	else
#endif
	{
		reg_addr = REG_VALID_DETECT;//finger detection is little size, more sensitivity
		val_write= TOUCH_VALID_THRESHOLD;//default:70
		FT5206_I2C_Write(&reg_addr, &val_write, 1);
		   	    
		reg_addr = REG_ZOOM_DISTANCE;//zoom in/out is little size , more sensitivity
		val_write = TOUCH_VAL_ZOOM_THRESHOLD;
		FT5206_I2C_Write(&reg_addr, &val_write, 1);

              reg_addr = REG_PERIODACTIVE;
		val_write = TOUCH_PERIODACTIVE;
		FT5206_I2C_Write(&reg_addr, &val_write, 1);
	}
}

/******************************************************************************/
// Description: FT5206_Initial
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint32 FT5206_Initial(void)
{
    uint32 delay_time = TP_MAX_SLEEPTIME;
    
    //default is ok,if FT5206_Deinitial called,then  initial must be reset.
    GPIO_ResetTP(1,delay_time );
    return _FT5206_Reg_Initial();//CHIP REG SET
    
}


/******************************************************************************/
// Description: _FT5206_XY_LOG
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint32 _FT5206_XY_LOG()
{
    int16 i;

    i= 0;
    while(i<MAX_FINGER_NUM)
    {
        //SCI_TRACE_LOW:"TP:: XY_Coordinate Finger[%d] x=%d, y=%d, finger_id = 0x%x!rn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_427_112_2_18_0_35_3_2079,(uint8*)"dddd", i/2, XY_Coordinate[i].x_position,XY_Coordinate[i].y_position, XY_Coordinate[i].finger_id);
        i++;
    }
}

/******************************************************************************/
// Description: _FT5206_Read_RegAll
// Dependence: 
// Note:
/******************************************************************************/
LOCAL uint32 _FT5206_Read_RegAll(void)
{
    uint8 Reg_Read=0;
    uint8 Reg_value[10]={0};
    int16 i=0;
    FT5206_I2C_Read(&Reg_Read, Reg_value, 7);
    for(i=0;i<7;i++)
    {
        //SCI_TRACE_LOW:"FT5206 Reg[0x%x]=0x%x \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_443_112_2_18_0_35_3_2080,(uint8*)"dd", i,Reg_value[i]);
    }
    Reg_Read = 0x80;
    FT5206_I2C_Read(&Reg_Read, Reg_value, 10);
    for(i=0;i<10;i++)
    {
        //SCI_TRACE_LOW:"FT5206 Reg[0x80+%x]=0x%x \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_449_112_2_18_0_35_3_2081,(uint8*)"dd", i,Reg_value[i]);
    }
    Reg_Read = 0x92;
    FT5206_I2C_Read(&Reg_Read, Reg_value, 10);
    for(i=0;i<10;i++)
    {
        //SCI_TRACE_LOW:"FT5206 Reg[0x92+%x]=0x%x \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_455_112_2_18_0_35_3_2082,(uint8*)"dd", i,Reg_value[i]);
    }
    Reg_Read = 0x9c;
    FT5206_I2C_Read(&Reg_Read, Reg_value, 10);
    for(i=0;i<10;i++)
    {
        //SCI_TRACE_LOW:"FT5206 Reg[0x9c+%x]=0x%x \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_461_112_2_18_0_35_3_2083,(uint8*)"dd", i,Reg_value[i]);
    }
    Reg_Read = 0xa6;
    FT5206_I2C_Read(&Reg_Read, Reg_value, 8);
    for(i=0;i<8;i++)
    {
        //SCI_TRACE_LOW:"FT5206 Reg[0xa6+%x]=0x%x \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_467_112_2_18_0_35_3_2084,(uint8*)"dd", i,Reg_value[i]);
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
    uint8 Reg_Data[2]={0,0},reg =0;
    
    reg = REG_GESTURE_ID;
    FT5206_I2C_Read(&reg,Reg_Data, 2);//here read two registers, gesture_id, finger_num
    
    switch(Reg_Data[0])
    {
        case REG_GT_ZOOM_OUT:
            tpc_gesture_id = TG_ZOOM_OUT;
            break;
        case REG_GT_ZOOM_IN:
            tpc_gesture_id = TG_ZOOM_IN;
            break;
        default:
            tpc_gesture_id = TG_NO_DETECT;
            break;
    }

    Finger_Num = Reg_Data[1] & 0xf;
    if(Finger_Num > MAX_FINGER_NUM)//if i2c failed.
        Finger_Num = MAX_FINGER_NUM;

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
    FT5206_I2C_Read(&reg, Reg_Data, REG_FINGER_INTERVAL*Coordinate_Num);

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
// Description: FT5206_XY_Data
// Dependence:  get the most data about capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL uint32 FT5206_XY_Data(void)
{
     //get gesture
     _Get_Gesture_Data();
     
     //get coordinate
     _Get_Finger_Coordinate();
}


/******************************************************************************/
// Description: FT5206_Read
// Dependence:  GETTING the data about touchpanel. the up-layer calls the 
//              function to get the data.
// Note:
/******************************************************************************/
LOCAL uint32 FT5206_Read(TPDSVR_SIG_T *data)
{
    int16 i;
   
    FT5206_XY_Data();
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
}


/******************************************************************************/
// Description: _FT5206_Reg_Initial
// Dependence:  Initial capacitive touchpanel.
// Note:
/******************************************************************************/

LOCAL int _FT5206_Reg_Initial(void)
{
    FT5206_I2C_Open();
    if(_Get_FT5206_I2C_Status() == TP_MULTI_SUCCESS)
    {
        _FT5206_Config();
        return TP_MULTI_SUCCESS;
    }
    else
        return TP_MULTI_ERROR;

    
}

/******************************************************************************/
// Description: FT5206_Open
// Dependence:  Open capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL int FT5206_Open(void)
{
    return TP_MULTI_SUCCESS;
}

/******************************************************************************/
// Description: FT5206_Write
// Dependence:  
// Note:
/******************************************************************************/
LOCAL int FT5206_Write(void)
{
    return TP_MULTI_SUCCESS;
}

LOCAL uint32 FT5206_Ioctl(uint32 cmd, void *arg)
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
                FT5206_I2C_Write(&reg_addr, &val_write, 1);
                break;
            }
            break;
        case TP_IOCTL_GET_INT_TIME:
            
            reg_addr = REG_PERIODACTIVE;
            
            FT5206_I2C_Read(&reg_addr, &val_read, 1);
            
            *((uint8 *)arg) = 100/val_read;
            
            //SCI_TRACE_LOW:"value freq_cycle time = 0x%x\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_689_112_2_18_0_35_3_2085,(uint8*)"d", *((uint8 *)arg));
            
            break;

        case TP_IOCTL_SET_THRES_ZOOM:
            reg_addr = REG_ZOOM_DISTANCE;
            val_write = *((uint16 *)arg);
            FT5206_I2C_Write(&reg_addr, &val_write, 1);
            FT5206_I2C_Read(&reg_addr, &val_read, 1);
            //SCI_TRACE_LOW:"value zoom_distance = 0x%x\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_698_112_2_18_0_35_3_2086,(uint8*)"d", val_read);
            break;

            default:
                break;
    }
    return TP_MULTI_SUCCESS;
}
/******************************************************************************/
// Description: FT5206_Suspend
// Dependence:  When the phone enter sleep mode for saving.
// Note:
/******************************************************************************/
LOCAL void FT5206_Suspend(void)
{
    uint8 reg_addr, val_write;

    reg_addr = REG_CHIP_MODE;
    val_write = CHIP_WORK_MODE_HIBERNATE;// about only 100uA
    FT5206_I2C_Write(&reg_addr, &val_write, 1);
}


/******************************************************************************/
// Description: FT5206_Resume
// Dependence:  
// Note:
/******************************************************************************/
LOCAL void FT5206_Resume(void)
{
    uint8 reg_addr, val_read;
    uint32 delay_time = TP_MAX_SLEEPTIME+5;
	uint32 i =0;

	/*CHIP WAKE PIN  least time is 5ms in LOW level when reset according to SPEC.*/
	GPIO_ResetTP(1,delay_time );
	
  	reg_addr = REG_CHIP_MODE;
	FT5206_I2C_Read(&reg_addr, &val_read, 1);
	//SCI_TRACE_LOW("TP: FT5206_Resume 0x%x \r\n", val_read);	

	/*here ,if tp wake failure, do it(suspend/resume) again */
	
	for(i=0; i<5; i++)
	{
		if(val_read != 0x0)//work mode
		{
			FT5206_Suspend();
			OS_TickDelay(10);
			GPIO_ResetTP(1,delay_time );
			reg_addr = REG_CHIP_MODE;
			FT5206_I2C_Read(&reg_addr, &val_read, 1);
			if(val_read != 0x0)
			{
				//SCI_TRACE_LOW:"TP: FT5206_Resume Retry Count =0x%x,value=0x%x \r\n"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_FT5206_748_112_2_18_0_35_3_2087,(uint8*)"dd", i+1,val_read);
			}
			//if(val_read != 0x0)
			//{
			//	SCI_PASSERT(SCI_FALSE, ("reg 0xa5 =  %d", val_read));
			//}
		}
		else
		{
			break;
		}
	}
}
/******************************************************************************/
// Description: FT5206_Close
// Dependence:  Close capacitive touchpanel.
// Note:
/******************************************************************************/

LOCAL int FT5206_Close(void)
{
    FT5206_I2C_Close();
    return TP_MULTI_SUCCESS;
}

/******************************************************************************/
// Description: FT5206_Deinitial
// Dependence:  Deinitial capacitive touchpanel.
// Note:
/******************************************************************************/

LOCAL int FT5206_Deinitial(void)
{
    FT5206_I2C_Close();
    return TP_MULTI_SUCCESS;
}

PUBLIC TPC_OPERATION_T tpc_ft5206_ops=
{
    FT5206_Initial,
    FT5206_Open,
    FT5206_Write,
    FT5206_Read,
    FT5206_Ioctl,
    FT5206_Suspend,
    FT5206_Resume,
    FT5206_Close,
    FT5206_Deinitial   
};

/******************************************************************************/
