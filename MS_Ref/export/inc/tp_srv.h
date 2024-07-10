/******************************************************************************
 ** File Name:      tp_srv.h                                                *
 ** Author:         tao.feng                                            *
 ** DATE:           06/10/2008                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic data structure for Touch Panel module  *
 **                                                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/10/2008     tao.feng         Create.                                  *                                              *
 ******************************************************************************/
#ifndef _TP_SRV_H_
#define _TP_SRV_H_

#include "sci_types.h"
#include "os_api.h"

/*NOTE:NOTE:NOTE:MAX_FINGER_NUM's value has a bit doubt, if the cap-TP supports 
more finger numbers,it need be modified.*/
#define MAX_FINGER_NUM           8
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
typedef enum{
    TG_NO_DETECT = 0x00,
    TG_ZOOM_IN = 1,
    TG_ZOOM_OUT,
    TG_DOUBLE_TAP,
    TG_SINGLE_TAP,
    TG_ROTATE_LEFT,
    TG_ROTATE_RIGHT,
    TG_SIG_SLIDE_UP,
    TG_SIG_SLIDE_DOWN,
    TG_SIG_SLIDE_LEFT,
    TG_SIG_SLIDE_RIGHT,
    TG_MUL_SLIDE_UP,
    TG_MUL_SLIDE_DOWN,
    TG_MUL_SLIDE_LEFT,
    TG_MUL_SLIDE_RIGHT,
    TG_UNKNOWN_STATE
}TG_STATE_E;

typedef enum {
	TP_PEN_NONE = 0x00,			/*lint -esym(749, TP_PEN_NONE)*/
    TP_PEN_DOWN=1,    /*1*/  
    TP_PEN_MOVE,
    TP_PEN_UP,      /*2*/
    TP_PEN_DONE,    /*3*/		/*lint -esym(749, TP_PEN_DONE)*/
    TP_UNKNOWN_STATE/*4*/		/*lint -esym(749, TP_UNKNOWN_STATE)*/
} TP_STATE_E;

//TPC IOCTL COMMAND
typedef enum {
    TP_IOCTL_CMD_SPEED_MODE = 0x0,
    TP_IOCTL_GET_INT_TIME,
    TP_IOCTL_SET_THRES_ZOOM,
    TP_IOCTL_GET_THRES_ZOOM,
    TP_IOCTL_CMD_TO_MMI = 0x100,
    TP_IOCTL_GES_INTV_TIME,
    TP_IOCTL_ENABLE_ZOOM,
    TP_IOCTL_DISABLE_ZOOM,
    TP_IOCTL_ENABLE_ROTATE,
    TP_IOCTL_DISABLE_ROTATE,
    TP_IOCTL_CMD_IIC_OD_MODE,
    TP_IOCTL_IIC_AND_IRQ_EN1V8,
    TP_IOCTL_CMD_MAX
}TP_IOCTL_CMD_E;

typedef enum {
    TP_SPEED_MODE_SLOW= 0xa4,
    TP_SPEED_MODE_NORMAL,
    TP_SPEED_MODE_FAST,
}TP_SPEED_MODE_E;


typedef enum {
	#ifdef WIN32
	TP_DOWN_MSG =  0x211 ,
	#else
	TP_DOWN_MSG =  (TP_SERVICE << 8) | 1 ,
	#endif
	TP_UP_MSG,
	TP_MOVE_MSG,
	TP_GESTURE_MSG,
	TP_LONGTAP_MSG,
	TP_MAX_MSG
} TP_MSG_E;


/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
typedef struct XY_DATA_tag				
{				
    int16 x_position;				
    int16 y_position;				
    uint16 finger_id;
}XY_DATA_T;				


typedef struct TPDSVR_SIG_tag
{
    SIGNAL_VARS
    int16      x_key;
    int16      y_key;
    uint16      cur_index;
    XY_DATA_T   xy_position[MAX_FINGER_NUM];
    uint16      gesture_type;
    uint16      num_of_point;
} TPDSVR_SIG_T;


typedef enum TPD_TO_TPSVR_MSG_ENUM
{
	TP_NONE_MESSAGE = 007,
	TP_ENTER_INTO_GPIO_HANDLER,
	TP_ENTER_INTO_TIMER_HANDLER,
	TP_ENTER_ENABLE,
	TP_ENTER_DISABLE,
	TP_ENTER_CMD
} TPD_TO_TPSVR_MSG_E;


typedef struct TPD_TO_TPSVR_MSG_TAG
{
	SIGNAL_VARS
	TPD_TO_TPSVR_MSG_E  tpd_msg_valu;
	TP_IOCTL_CMD_E cmd;
	void *arg;
} TPD_TO_TPSVR_MSG_T;

//#if defined(TOUCHPANEL_TYPE_MULTITP) || (defined(TOUCHPANEL_TYPE_HW) && defined(TPC_TYPE_DOUBLE))
PUBLIC void TP_Ioctl(uint32 cmd, void *arg);
PUBLIC uint32 TP_Read(TPDSVR_SIG_T *data);
//#endif
#endif // _TP_SRV_H_

