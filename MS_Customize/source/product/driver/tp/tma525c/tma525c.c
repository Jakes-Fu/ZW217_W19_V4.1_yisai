/******************************************************************************
 ** File Name:      tp_TMA525C.c                                               				*
 ** Author:         lanqing.liu                              								*
 ** DATE:           2019.08.15                                                					*
 ** Copyright:      2019 Spreatrum, Incoporated. All Rights Reserved.         		*
 ** Description:                                                              						*
 **                                                                           						*
 **                                                                           						*
 ******************************************************************************/
 
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "tp_multi_cfg.h"
#include "gpio_prod_api.h"
#include "gpio_drv.h"

 
 #define TP_FIRMWARE_UPDATE
#define TP_I2C_PORT                 TPC_GetCtrlPort()
#define I2C_NUM_MAX		107
#define TP_RECORD_OFFSET		7
//The internal of reg of every finger is the same.
#define REG_FINGER_INTERVAL     0x06
#define TP_TMA525C_I2C_WRITE_ADDR       0x48
#define TP_TMA525C_I2C_READ_ADDR         0x49

#define    FTS_PACKET_LENGTH        128
TG_STATE_E tpc_gesture_id = TG_UNKNOWN_STATE;

/*This chip Coordinate has 12bit-data in differnet two regeristers.*/
#define XY_Coordinate_Transform(_data, _high_data, _low_data) do{   \
        _data = ((_high_data & 0xff) << 8) | _low_data;                      \
            }while(0)

struct touch_record {
	uint8 touch_type;
	uint8 event;
	uint8 pos_x_low;
	uint8 pos_x_high;
	uint8 pos_y_low;
	uint8 pos_y_high;
	uint8 pressure;
	uint8 major_axis;
	uint8 minor_axis;
	uint8 orientation;
};

LOCAL I2C_DEV TMA525C_I2C_cfg =
{
    3,          
    400000,                   // freq is standard for TMA525C
    0,                          // bus
    TP_TMA525C_I2C_WRITE_ADDR,  // slave_addr
    0,                          // reg_addr_num
    1,                          // check_ack
    1                           // no_stop
};

LOCAL int32 TMA525C_I2C_Handler = -1;
XY_DATA_T XY_Coordinate[MAX_FINGER_NUM]={0};
TP_STATE_E tp_event = TP_PEN_NONE;
uint8 tma525c_data[I2C_NUM_MAX];
uint8 event_count[3] = {0};//0,down;1,move;2,up
uint16 Finger_Num = 0;


extern PUBLIC uint32 OS_TickDelay(uint32 ticks);
LOCAL uint32 _Get_TMA525C_I2C_Status(void);
LOCAL uint32 _TMA525C_Read_RegAll(void);
 
LOCAL uint32 TMA525C_I2C_Open(void)
{
	//TMA525C_I2C_cfg.id =  TP_I2C_PORT;

	if(TMA525C_I2C_Handler == -1) 
		if((TMA525C_I2C_Handler = I2C_HAL_Open(&TMA525C_I2C_cfg)) == -1)
			return TP_MULTI_ERROR;    
	return TP_MULTI_SUCCESS;
}

LOCAL uint32 TMA525C_I2C_Close(void)
{
    if(_Get_TMA525C_I2C_Status() == TP_MULTI_SUCCESS)
	{
 		I2C_HAL_Close(TMA525C_I2C_Handler);
		TMA525C_I2C_Handler = -1;
	}	
    return TP_MULTI_SUCCESS;
}

LOCAL void TMA525C_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_TMA525C_I2C_Status() == TP_MULTI_SUCCESS)
        I2C_HAL_Write(TMA525C_I2C_Handler, reg, addr, cnt);
}

LOCAL int TMA525C_I2C_Read(uint8 *addr, uint32 cnt)
{
	int ret;

	if (!cnt)
		return TP_MULTI_ERROR;

	if(_Get_TMA525C_I2C_Status() == TP_MULTI_SUCCESS) {
	   ret =  I2C_HAL_Read(TMA525C_I2C_Handler, NULL, addr, cnt);
	   if (ret != cnt)
	   	return TP_MULTI_ERROR;
	}

	return TP_MULTI_SUCCESS;
}
 
LOCAL uint32 _Get_TMA525C_I2C_Status(void)
{
    if(TMA525C_I2C_Handler==-1)
    {
        return TP_MULTI_ERROR;
    }
    else
    {
        return TP_MULTI_SUCCESS;
    }
}
 
LOCAL int _TMA525C_Config(void)
{

	uint8 reg_addr, data[2], try_cnt = 10 ;
	int ret;

	ret = TMA525C_I2C_Read(data, 2);
	if (ret != TP_MULTI_SUCCESS ) {
		SCI_TRACE_LOW("tma525c i2c transfer error\n"); 
		return -1;
	}
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT, "tma525c init data =%x \r\n",(uint8*)"dd", data);
	while (data[0] != 0x02) {
		ret = TMA525C_I2C_Read(data, 2);
		if (!try_cnt ||ret != TP_MULTI_SUCCESS ) {
			SCI_TRACE_LOW("tma525c init error, try_cnt = %d\n", try_cnt);
			TMA525C_I2C_Close();
			return -1;
		}
		try_cnt --;
	}

	return 0;
}
 
LOCAL uint32 TMA525C_Initial(void)
{
	uint32 delay_time = 150;

	GPIO_ResetTP(0, delay_time );
	TMA525C_I2C_Open();
	if(_Get_TMA525C_I2C_Status() == TP_MULTI_SUCCESS) {
		if (_TMA525C_Config() < 0) {
			SCI_TRACE_LOW("TMA525C_Initial error\n"); 
			return TP_MULTI_ERROR;	
		}
		return TP_MULTI_SUCCESS;
	}

	return TP_MULTI_ERROR;
}
 
LOCAL uint32 TMA525C_Get_Gesture_Data(void)
{
	uint16 data_len;
	int ret;

	ret = TMA525C_I2C_Read(&data_len, 2);
	if (ret != TP_MULTI_SUCCESS) {
		SCI_TRACE_LOW("Get_Gesture_Data i2c transfer fail \n"); 
		return TP_MULTI_ERROR;
	}

	if (data_len == 0xffff)
		while(1);
	ret = TMA525C_I2C_Read(tma525c_data, data_len);
	if (ret != TP_MULTI_SUCCESS) {
		SCI_TRACE_LOW("Get_Gesture_Data i2c read fail \n"); 
		return TP_MULTI_ERROR;
	}

	Finger_Num = tma525c_data[5] & 0x1f;
	if(Finger_Num > 10) 
		Finger_Num = 10;

	SCI_TRACE_LOW("data_len = %d (0x%x), Finger_Num =%d\n", data_len, data_len, Finger_Num); 
 	tpc_gesture_id = TG_NO_DETECT;
	return TP_MULTI_SUCCESS;
}
 
LOCAL uint32 TMA525C_Get_Finger_Coordinate(void)
{
	struct touch_record *touch_record= (struct touch_record *) (&tma525c_data[7]);
	uint8 *touch_record_cur;
	int16 i=0;

	memset(XY_Coordinate, 0, sizeof(XY_Coordinate));
	memset(event_count,0, sizeof(event_count));
	
	for (i=0; i<Finger_Num;i++)
	{
		XY_Coordinate_Transform(XY_Coordinate[i].x_position, touch_record[i].pos_x_high, touch_record[i].pos_x_low);
		XY_Coordinate_Transform(XY_Coordinate[i].y_position, touch_record[i].pos_y_high, touch_record[i].pos_y_low);	
		SCI_TRACE_LOW("x= %d, y=%d, event = 0x%x\n", XY_Coordinate[i].x_position, XY_Coordinate[i].y_position, (touch_record[i].event >> 5)  & 0x3 ); 
		switch((touch_record[i].event >> 5)  & 0x3)//for example: reg0x3 [7:6]
		{
			case 1://pen down
			    XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_DOWN) << 8;/*high byte is Event Flag*/
			    event_count[0]++;
			    break;

			case 2://move
			    XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_DOWN) << 8;/*high byte is Event Flag*/
			    event_count[1]++;
			    break;
			case 3://pen up
			    XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_UP) << 8;/*high byte is Event Flag*/
			    event_count[2]++;
			    break;
			default:
			    XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_NONE) << 8;/*high byte is Event Flag*/
			    break;
		}        
	}
}
 
LOCAL TP_STATE_E  TMA525C_Get_Cal_Msg(void)
{
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
 
LOCAL uint32 TMA525C_XY_Data(void)
{
	uint32 ret;

	ret = TMA525C_Get_Gesture_Data();
	if (ret  == TP_MULTI_ERROR)
		return ret;

	TMA525C_Get_Finger_Coordinate();

	return TP_MULTI_SUCCESS;
}
 
LOCAL uint32 TMA525C_Read(TPDSVR_SIG_T *data)
{
    int16 i;
	uint32 ret;
   
    ret = TMA525C_XY_Data();
	if (ret  == TP_MULTI_ERROR)
		return ret;
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
    
    switch(TMA525C_Get_Cal_Msg())
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

		return TP_MULTI_SUCCESS;
}
 
LOCAL int TMA525C_Open(void)
{
    return TP_MULTI_SUCCESS;
}
 
LOCAL int TMA525C_Write(void)
{
    return TP_MULTI_SUCCESS;
}

LOCAL uint32 TMA525C_Ioctl(uint32 cmd, void *arg)
{
	return TP_MULTI_SUCCESS;
}

LOCAL void TMA525C_Suspend(void)
{

}

LOCAL void TMA525C_Resume(void)
{
	
}

LOCAL int TMA525C_Close(void)
{
    TMA525C_I2C_Close();
    return TP_MULTI_SUCCESS;
}

LOCAL int TMA525C_Deinitial(void)
{
    TMA525C_I2C_Close();
    return TP_MULTI_SUCCESS;
}

PUBLIC TPC_OPERATION_T tpc_TMA525C_ops= {
    TMA525C_Initial,
    TMA525C_Open,
    TMA525C_Write,
    TMA525C_Read,
    TMA525C_Ioctl,
    TMA525C_Suspend,
    TMA525C_Resume,
    TMA525C_Close,
    TMA525C_Deinitial   
};
