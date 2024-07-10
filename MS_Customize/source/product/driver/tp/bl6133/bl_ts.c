
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
*
* Filename:
* ---------
*   Capacitive_TP_Cypress_CY8CTMA340.c
*
*
* Description:
* ------------
*   Capacitive Touch Panel I2C read/write Driver
*
* Author:
* -------
* -------
*
*============================================================================
*             HISTORY
* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*------------------------------------------------------------------------------
* removed!
* removed!
* removed!
*------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*============================================================================
*****************************************************************************/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "tp_multi_cfg.h"
#include "gpio_prod_api.h"
#include "gpio_drv.h"
#include "dal_keypad.h"

#include "bl_chip_common.h"
#include "bl_chip_custom.h"

#define REG_FINGER_INTERVAL     0x06
#define MAX_FINGER_NUM 1
#define TP_DATA_BUF_SIZE (128) /* must be power of 2 */
#define TP_DATA_BYTE         (6)
static char tp_data_buf[TP_DATA_BUF_SIZE];
LOCAL XY_DATA_T XY_Coordinate[MAX_FINGER_NUM]={0};

LOCAL TG_STATE_E tpc_gesture_id = TG_UNKNOWN_STATE;

LOCAL uint16 Finger_Num = 0;
LOCAL TP_STATE_E tp_event = TP_PEN_NONE;
LOCAL uint8 event_count[3] = {0};//0,down;1,move;2,up
LOCAL char g_is_have_tp = 0;


/*This chip Coordinate has 12bit-data in differnet two regeristers.*/
#define XY_Coordinate_Transform(_data, _high_data, _low_data) do{   \
        _data = ((_high_data & 0xf) << 8) | _low_data;                      \
            }while(0)

LOCAL I2C_DEV BL6133_I2C_cfg =
{
    4,
    400000,                   // freq is standard for BL6133
    0,                          // bus
    TP_BL6133_I2C_WRITE_ADDR,  // slave_addr
    1,                          // reg_addr_num
    0,                          // check_ack
    1                           // no_stop
};

LOCAL int32 BL6133_I2C_Handler = -1;

/******************************************************************************/
// Description: BL6133_I2C_Open
// Dependence:
// Note:
/******************************************************************************/
LOCAL uint32 BL6133_I2C_Open(void)
{
    if(BL6133_I2C_Handler == -1)
    {
        if((BL6133_I2C_Handler = I2C_HAL_Open(&BL6133_I2C_cfg)) == -1)
        {
			SCI_TraceLow("BL6133_I2C_Open I2C failed !!!\n");
            return TP_MULTI_ERROR;
        }
        else
            return TP_MULTI_SUCCESS;
    }
    else
    {
		//SCI_TraceLow("[BL6133] BL6133_I2C_Open I2C success !!!\n");
        return TP_MULTI_SUCCESS;
    }
}
/******************************************************************************/
// Description: _Get_BL6133_I2C_Status
// Dependence:  It can get the status from i2c
// Note:
/******************************************************************************/
LOCAL uint32 _Get_BL6133_I2C_Status(void)
{
    if(BL6133_I2C_Handler==-1)
    {
        return TP_MULTI_ERROR;
    }
    else
    {
        return TP_MULTI_SUCCESS;
    }
}
/******************************************************************************/
// Description: BL6133_I2C_Close
// Dependence:
// Note:
/******************************************************************************/
LOCAL uint32 BL6133_I2C_Close(void)
{
    if(_Get_BL6133_I2C_Status() == TP_MULTI_SUCCESS)
	{
 		I2C_HAL_Close(BL6133_I2C_Handler);
		BL6133_I2C_Handler = -1;
	}
    return TP_MULTI_SUCCESS;
}
/******************************************************************************/
// Description: BL6133_I2C_Write
// Dependence:
// Note:
/******************************************************************************/
int BL6133_I2C_Write(uint8 *addr, uint32 cnt)
{
    if(_Get_BL6133_I2C_Status() == TP_MULTI_SUCCESS)
    {
        if(I2C_HAL_Write_Byte(BL6133_I2C_Handler,addr, cnt) == 0)
        {
            SCI_TraceLow("BL6133_I2C_Write error !\n");
            return SCI_FALSE;
        }
        return SCI_TRUE;
    }
    else
    {
	    SCI_TraceLow("W BL6133_I2C_Status error !!!\n");
	    return SCI_FALSE;
    }
}
/******************************************************************************/
// Description: BL6133_I2C_Read
// Dependence:
// Note:
/******************************************************************************/
int BL6133_I2C_Read(uint8 *addr, uint32 cnt)
{
    if(_Get_BL6133_I2C_Status() == TP_MULTI_SUCCESS)
    {
        if(I2C_HAL_Read_Byte(BL6133_I2C_Handler,addr, cnt) == 0)
        {
            SCI_TraceLow("BL6133_I2C_Read error !!!\n");
            return SCI_FALSE;
        }
        return SCI_TRUE;
    }
    else
    {
        SCI_TraceLow("R BL6133_I2C_Status error !!!\n");
        return SCI_FALSE;
    }
}

void bl_ts_set_intmode(char mode)
{
	if(0 == mode)
	{//GPIO mode
		//GPIO_TPIRQCtrl(0);
		_GPIO_PROD_SetDirection (GPIO_PROD_TP_INT_ID, SCI_TRUE);

	}
	else if(1 == mode)
	{//INT mode
		//GPIO_TPIRQCtrl(1);
		_GPIO_PROD_SetDirection (GPIO_PROD_TP_INT_ID, SCI_FALSE);
	}
}
void bl_ts_set_intup(char level)
{
	if(level==1)
	{
		_GPIO_PROD_SetVal (GPIO_PROD_TP_INT_ID,  level);
	}
	else if(level==0)
	{
		_GPIO_PROD_SetVal (GPIO_PROD_TP_INT_ID,  level);
	}
}
#ifdef INT_PIN_WAKEUP
void bl_ts_int_wakeup(void)
{
	bl_ts_set_intmode(0);
	bl_ts_set_intup(1);
	OS_TickDelay(20);
	bl_ts_set_intup(0);
	OS_TickDelay(1);
	bl_ts_set_intup(1);
	OS_TickDelay(20);
	bl_ts_set_intmode(1);
}
#endif
void bl_ts_reset_wakeup(void)
{
	GPIO_ResetTP(1,50);
}

void bl_ts_resume_reset_wakeup(void)
{
	GPIO_ResetTP_Sleep(1,50);
}

extern PUBLIC int bl_read_fw(unsigned char *reg_addr, unsigned char *buf, int len);
LOCAL uint32 _Get_BL_Gesture_Data(void)
{
	uint8 Reg_Data[2]={0,0},reg =0;
	unsigned char cmd[1];
	cmd[0] = 0x01;
	bl_read_fw(cmd,Reg_Data, 2);//here read two registers, gesture_id, finger_num
	//SCI_TraceLow("[BL6133] _Get_Gesture_Data Reg_Data[0]:%d Reg_Data[1]:%x \n",Reg_Data[0],Reg_Data[1]);
	switch(Reg_Data[0])//for example: reg0x1
	{
		case 0x01://pen up
			tpc_gesture_id = TG_SIG_SLIDE_UP;
			break;
		case 0x02://pen down
			tpc_gesture_id = TG_SIG_SLIDE_DOWN;
			break;
		case 0x03://pen left
			tpc_gesture_id = TG_SIG_SLIDE_LEFT;
			break;
		case 0x04://pen right
			tpc_gesture_id = TG_SIG_SLIDE_RIGHT;
			break;
		case 0x05://pen singer click
			tpc_gesture_id = TG_SINGLE_TAP;
			break;
		case 0x0b://pen double click
			tpc_gesture_id = TG_DOUBLE_TAP;
			break;
		case 0x0c://pen longer press
			//tpc_gesture_id = TG_DOUBLE_TAP;
			break;
		default:
			tpc_gesture_id = TG_NO_DETECT;
			break;
	}
	Finger_Num = Reg_Data[1] & 0xf;
	if(Finger_Num > MAX_FINGER_NUM)//if i2c failed.
	Finger_Num = MAX_FINGER_NUM;
	//SCI_TraceLow("[BL6133] _Get_Gesture_Data Finger_Num:%d tpc_gesture_id:%x \n",Finger_Num,tpc_gesture_id);
	tpc_gesture_id = TG_NO_DETECT;
	return TP_MULTI_SUCCESS;
}
LOCAL uint32 _Get_BL_Finger_Coordinate(void)
{
    uint8 Coordinate_Num = 0;
    uint8 Reg_Data[REG_FINGER_INTERVAL*MAX_FINGER_NUM]={0};
    int16 i=0;
    unsigned char readPointCmd = TD_STAT_ADDR;

    //according to finger num,read finger's message.
    Coordinate_Num = Finger_Num;
    if(Coordinate_Num == 0)//Because it will be read message, when pen up
        Coordinate_Num = 1;

    //BL6133_I2C_Read(Reg_Data, REG_FINGER_INTERVAL*Coordinate_Num);
    BL6133_I2C_Write(&readPointCmd, 1);
    BL6133_I2C_Read(Reg_Data, sizeof(Reg_Data));

    //finger's message gives to global variable.
    memset(XY_Coordinate, 0, sizeof(XY_Coordinate));
    memset(event_count,0, sizeof(event_count));

    for (i=0; i<Coordinate_Num;i++)
    {
        XY_Coordinate_Transform(XY_Coordinate[i].x_position, Reg_Data[i*REG_FINGER_INTERVAL+2], Reg_Data[i*REG_FINGER_INTERVAL+3]);
        XY_Coordinate_Transform(XY_Coordinate[i].y_position, Reg_Data[i*REG_FINGER_INTERVAL+4], Reg_Data[i*REG_FINGER_INTERVAL+5]);
        XY_Coordinate[i].finger_id = (Reg_Data[i*REG_FINGER_INTERVAL+2] & 0xf0)>>4;/*low byte is Touch ID*/
		//SCI_TraceLow("[BL6133] _Get_Finger_Coordinate Reg_Data[i*REG_FINGER_INTERVAL+2]:%x \n",Reg_Data[i*REG_FINGER_INTERVAL+2]);
        switch(Reg_Data[i*REG_FINGER_INTERVAL+2]&0xf0)//for example: reg0x1
        {
            case 0x40://pen up
                XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_UP) << 8;/*high byte is Event Flag*/
                event_count[2]++;
                break;
            case 0x00://pen down
                XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_DOWN) << 8;/*high byte is Event Flag*/
                event_count[0]++;
                break;
			case 0x80://pen move
                XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_MOVE) << 8;/*high byte is Event Flag*/
                event_count[1]++;
                break;
	     default:
                XY_Coordinate[i].finger_id |= ((uint16 )TP_PEN_NONE) << 8;/*high byte is Event Flag*/
                break;
        }
    }
    return 0;
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
	//SCI_TraceLow("[BL6133] _Get_Cal_Msg event_count [0]:0x%x [1]:0x%x  [2]:0x%x \n",event_count[0],event_count[1],event_count[2]);
    if((event_count[0]!=0) && (event_count[1] ==0) && (event_count[2] ==0))
       tp_event = TP_PEN_DOWN;
    else if( (event_count[0] ==0) && (event_count[1] == 0) && (event_count[2] !=0))
       tp_event = TP_PEN_UP;
    else if(event_count[1] !=0)
       tp_event = TP_PEN_MOVE;
    else
       tp_event = TP_PEN_NONE;

	//SCI_TraceLow("[BL6133] _Get_Cal_Msg event_count tp_event:%d\n",tp_event);
    return tp_event;
}
/******************************************************************************/
// Description: BL6133_XY_Data
// Dependence:  get the most data about capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL uint32 BL6133_XY_Data(void)
{
     int ret = 0;//get gesture
     _Get_BL_Gesture_Data();

     //get coordinate
     ret = _Get_BL_Finger_Coordinate();
     return 0;
}
/******************************************************************************/
// Description: BL6133_Read
// Dependence:  GETTING the data about touchpanel. the up-layer calls the
//              function to get the data.
// Note:
/******************************************************************************/
LOCAL uint32 BL6133_Read(TPDSVR_SIG_T *data)
{
    int16 i;
    uint32 ret = 0;
    TB_MSG  key_msg;
    ret = BL6133_XY_Data();
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
        case TP_PEN_UP:
            data->SignalCode = TP_UP_MSG;
            break;
        case TP_PEN_DOWN:
            data->SignalCode = TP_DOWN_MSG;
            break;
        case TP_PEN_MOVE:
            data->SignalCode = TP_MOVE_MSG;
            break;
            default:
            data->SignalCode = TP_MAX_MSG;
            break;
    }
	SCI_TraceLow("BL6133_Read x_key=%d y_key=%d SignalCode=%d\n",data->x_key,data->y_key,data->SignalCode);
	if(data->x_key == 120 && data->y_key == 300)
	{
		if(data->SignalCode == TP_DOWN_MSG)
			key_msg.message = TB_KPD_PRESSED;
		else
			key_msg.message = TB_KPD_RELEASED;

		//SCI_TraceLow("[BL6133] BL6133_Read send key SignalCode:%x\n",data->SignalCode);
		key_msg.wparam  = SCI_VK_MENU_CANCEL;
		key_msg.lparam  = TB_NULL;
		DRV_Callback (TB_GPIO_INT,&key_msg);
		data->SignalCode = TP_UP_MSG;
	}
	//SCI_TraceLow("[BL6133] BL6133_Read Finger_Num:%d x_key:%d y_key:%d SignalCode:%x\n",Finger_Num,data->x_key,data->y_key,data->SignalCode);
	return 0;
}

/******************************************************************************/
// Description: BL6133_Initial
// Dependence:
// Note:
/******************************************************************************/
LOCAL uint32 BL6133_Initial(void)
{
    char  ret = 0;
    unsigned char retry = 3;
    #ifdef BTL_CHECK_CHIPID
    unsigned char chipID = 0x00;
    #endif

    SCI_TraceLow("BL6133_Initial\n");
    if(g_is_have_tp == 1)
    {
       	return TP_MULTI_ERROR;
    }
    bl_ts_reset_wakeup();
    GPIO_ResetTPIntExt(1,50);

    BL6133_I2C_Open();
    if(_Get_BL6133_I2C_Status() == TP_MULTI_SUCCESS)
    {
        #ifdef  BTL_CHECK_CHIPID
        bl_ts_set_intmode(0);
        OS_TickDelay(20);
        //sunwen.he add for w217 user version TP init failed
        while(retry--)
        {
            ret = bl_get_chip_id(&chipID);
            if(SCI_FALSE == ret)
            {
                bl_log_trace("bl_get_chip_id error\n");
                break;
            }
            else
            {
                if(chipID != BTL_FLASH_ID)
                {
                    bl_log_trace("bl_get_chip_id =%x retry =%d\n",chipID,retry);
                    continue;
                }
                else
                {
                      break;
                }
            }
        }
        bl_ts_set_intmode(1);
        OS_TickDelay(10);
       	if((ret < 0)|| (chipID != BTL_FLASH_ID))
       	{
       	    bl_log_trace("ctp_bl_ts_init:Read chipID Fail:chipID = %x\n",chipID);
       	    return TP_MULTI_ERROR;
       	}
       	else
       	{
            bl_log_trace("ctp_bl_ts_init:Read chipID succes:chipID = %x\n",chipID);
       	}
        #endif
        g_is_have_tp=1;
        OS_TickDelay(20);
        #ifdef  BL_AUTO_UPDATE_FARMWARE
        bl_ts_set_intmode(0);
        bl_auto_update_fw();
        bl_ts_set_intmode(1);
        OS_TickDelay(10);
        #endif

        //_BL6133_Config();
        return TP_MULTI_SUCCESS;
    }
   	else
   	{
   	    SCI_TraceLow("BL6133_Initial Error!\n");
   	    return TP_MULTI_ERROR;
   	}
    //default is ok,if BL6133_Deinitial called,then  initial must be reset.
    //return _BL6133_Reg_Initial();//CHIP REG SET
}


/******************************************************************************/
// Description: BL6133_Open
// Dependence:  Open capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL int BL6133_Open(void)
{
	//SCI_TraceLow("[BL6133] BL6133_Open\n");
    return TP_MULTI_SUCCESS;
}

/******************************************************************************/
// Description: BL6133_Write
// Dependence:
// Note:
/******************************************************************************/
LOCAL int BL6133_Write(void)
{
	//SCI_TraceLow("[BL6133] BL6133_Write\n");
    return TP_MULTI_SUCCESS;
}

LOCAL uint32 BL6133_Ioctl(uint32 cmd, uint8 mod_en)
{
    BOOLEAN gpio_interrupt_value;
    uint8 reg_addr,val_read, val_write;
    TP_SPEED_MODE_E speed_mode;

	SCI_TraceLow("BL6133_Ioctl cmd:%d mod_en:%d\n",cmd,mod_en);
    switch(cmd)
    {
        case TP_IOCTL_CMD_IIC_OD_MODE:
            reg_addr = 0xfd;
            BL6133_I2C_Read(&val_read, 1);
            if (mod_en) {
                val_write = val_read|0x02;
                }
            BL6133_I2C_Write(&val_write, 1);

            break;
        case TP_IOCTL_IIC_AND_IRQ_EN1V8:

            reg_addr = 0xfd;
            BL6133_I2C_Read(&val_read, 1);
            if (mod_en) {
                val_write = val_read|0x01;
                }
            BL6133_I2C_Write(&val_read, 1);

            break;

            default:
                break;
    }
    return TP_MULTI_SUCCESS;
}
/******************************************************************************/
// Description: BL6133_Suspend
// Dependence:  When the phone enter sleep mode for saving.
// Note:
/******************************************************************************/

LOCAL void BL6133_Suspend(void)
{
	unsigned char sleepCmd[2] = {0xa5,0x03};
    unsigned char ret = 0;

	ret = BL6133_I2C_Write(sleepCmd, sizeof(sleepCmd));
	bl_log_trace("BL6133_Suspend %d",ret);
	//OS_TickDelay(100);

	//bl_ts_set_intmode(0);
	//bl_ts_set_intup(1);
	SCI_TraceLow("BL6133 enter sleep");
}
/******************************************************************************/
// Description: BL6133_Resume
// Dependence:
// Note:
/******************************************************************************/
LOCAL void BL6133_Resume(void)
{
    #if 1
        GPIO_ResetTPInt(1,50);
        GPIO_ResetTP(1,5);
    #else
	bl_ts_resume_reset_wakeup();
	#ifdef INT_PIN_WAKEUP
	bl_ts_int_wakeup();
	#endif
    #endif
	//bl_ts_set_intmode(1);
	SCI_TraceLow("BL6133 exit sleep");
}

/******************************************************************************/
// Description: BL6133_Close
// Dependence:  Close capacitive touchpanel.
// Note:
/******************************************************************************/

LOCAL int BL6133_Close(void)
{
	SCI_TraceLow("BL6133_Close !\n");
	BL6133_I2C_Close();
	return TP_MULTI_SUCCESS;
}
/******************************************************************************/
// Description: BL6133_Deinitial
// Dependence:  Deinitial capacitive touchpanel.
// Note:
/******************************************************************************/
LOCAL int BL6133_Deinitial(void)
{
	SCI_TraceLow("BL6133_Deinitial !\n");
	BL6133_I2C_Close();
	return TP_MULTI_SUCCESS;
}
PUBLIC TPC_OPERATION_T tpc_bl6133_ops=
{
	BL6133_Initial,
	BL6133_Open,
	BL6133_Write,
	BL6133_Read,
	BL6133_Ioctl,
	BL6133_Suspend,
	BL6133_Resume,
	BL6133_Close,
	BL6133_Deinitial
};
