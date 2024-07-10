/******************************************************************************
 ** File Name:      msensor_drv.h                                             *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           08/31/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of Motion*
 **                 Sensor.                                                   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/31/2009     Liangwen.Zhen    Create.                                   *
 ******************************************************************************/

#ifndef _MSENSOR_DRV_H_
#define _MSENSOR_DRV_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Include file                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define MSENSOR_SUCCESS 				SCI_SUCCESS
#define MSENSOR_ERROR					SCI_ERROR
#define MSENSOR_ERROR_BASE              0X8000
#define MSENSOR_ERROR_INTT_FAILED       (MSENSOR_ERROR_BASE + 1)
#define MSENSOR_ERROR_IIC_READ_FAILED   (MSENSOR_ERROR_BASE + 2)
#define MSENSOR_ERROR_IIC_WRITE_FAILED  (MSENSOR_ERROR_BASE + 3)
#define MSENSOR_ERROR_PARA				(MSENSOR_ERROR_BASE + 4)
#define MSENSOR_ERROR_NOT_SUPPORT       (MSENSOR_ERROR_BASE + 5)
#define MSENSOR_ERROR_NOT_INITED        (MSENSOR_ERROR_BASE + 6)
#define MSENSOR_ERROR_IIC_OPEN_FAILED   (MSENSOR_ERROR_BASE + 7)
#define MSENSOR_ERROR_INVALID_DATA      (MSENSOR_ERROR_BASE + 8)

#define	MSENSOR_MOTION_CAP_XY_AXIS      (0x1)
#define	MSENSOR_MOTION_CAP_Z_AXIS       (0x1 << 1)
#define	MSENSOR_MOTION_CAP_SHAKE        (0x1 << 2)
#define	MSENSOR_MOTION_CAP_DATA         (0x1 << 3)

typedef enum
{
	MSENSOR_TYPE_ACCELEROMETER = 0x1,   // detect common motion: gravity, rotation, shake ...
	MSENSOR_TYPE_MAGNETIC,              // detect orientaion ...
	MSENSOR_TYPE_OPTICAL,               // detect distance / brightness ...
	MSENSOR_TYPE_OFN,					// ofn, can be used as keyboard,mouse ...
	MSENSOR_TYPE_MAX
}MSENSOR_TYPE_E;

//------------------------------------------------------------------------------
//SENSOR MSG
//BIT31--BIT16   BIT15--BIT0
//SENSOR-TYPE      MSG-TYPE
#define MSG_MISC_SENSOR_MASK          0XFFFF
#define MSG_MISC_SENSOR_BIT_LEN       16
#define MSG_ACCELEROMETER_SENSOR_BASE (MSENSOR_TYPE_ACCELEROMETER << MSG_MISC_SENSOR_BIT_LEN)
#define MSG_MAGNETIC_SENSOR_BASE      (MSENSOR_TYPE_MAGNETIC << MSG_MISC_SENSOR_BIT_LEN)
#define MSG_OPTICAL_SENSOR_BASE       (MSENSOR_TYPE_OPTICAL << MSG_MISC_SENSOR_BIT_LEN)

//------------------------------------------------------------------------------
typedef enum
{
	//
	MSENSOR_EVENT_NONE            = 0,

	// ACCELEROMETER
/****************************
      +Y     up
         +-----------+
       ^ |           |
       | |           |
  left | |  LCD      | right
       | |  (forward)|
       | |           |
       | |           |
       | |           |
       | +-----------+
       +-----------> +X
      /     down
     /
    /
  |/ +Z
*****************************
     Figure 1
*****************************/


	MSENSOR_MOTION_TYPE_NONE = MSG_ACCELEROMETER_SENSOR_BASE,
	MSENSOR_MOTION_TYPE_SHAKE,          // 1. shake without orientation
	MSENSOR_MOTION_TYPE_LEFT,           // shake left
	MSENSOR_MOTION_TYPE_RIGHT,          // shake right
	MSENSOR_MOTION_TYPE_UP,             // shake up
	MSENSOR_MOTION_TYPE_DOWN,           // 5. shake down
	MSENSOR_MOTION_TYPE_FORWARD,        // shake forward
	MSENSOR_MOTION_TYPE_BACKWARD,       // shake backward
	MSENSOR_MOTION_TYPE_LEFT_SIDE,      // left @ TOP
	MSENSOR_MOTION_TYPE_RIGHT_SIDE,	    // ritht @ TOP
	MSENSOR_MOTION_TYPE_UP_SIDE,        // 10. up @ TOP
	MSENSOR_MOTION_TYPE_DOWN_SIDE,      // down @ TOP
	MSENSOR_MOTION_TYPE_FORWRAD_SIDE,   // forward @ TOP
	MSENSOR_MOTION_TYPE_BACKWARD_SIDE,  // backward @ TOP

	// MAGNETIC
	MSENSOR_MAGNETIC_NONE = MSG_MAGNETIC_SENSOR_BASE,

	// OPTICAL
	MSENSOR_OPTICAL_NONE = MSG_OPTICAL_SENSOR_BASE,
	MSENSOR_OPTICAL_NO_DETECTED,
	MSENSOR_OPTICAL_STH_DETECTED,

}MSENSOR_MOTION_TYPE_E;


//------------------------------------------------------------------------------
typedef enum
{
    MSENSOR_SENSI_LOW = 0,
    MSENSOR_SENSI_MID,
    MSENSOR_SENSI_HIGH
}MSENSOR_SENSI_E;

//------------------------------------------------------------------------------
typedef enum
{
	MSENSOR_IOCTL_GET_DATA = 0x0,   // get specific type of sensor valid MSENSOR_MOTION_VECTOR_T
	MSENSOR_IOCTL_GET_EVENT,        // get specific type of sensor MSENSOR_MOTION_TYPE_E
	MSENSOR_IOCTL_SET_SENSI,        // set specific type of sensor's sensitivity by MSENSOR_SENSI_E
	MSENSOR_IOCTL_SET_TIMER,        // set specific type of sensor timer expire value
	MSENSOR_IOCTL_SET_PRIO,         // set sensor task priority, NOTE: only one task avilaible, whatever sensor type is
	MSENSOR_IOCTL_GET_CAP,          // get specific type of sensor capability
	MSENSOR_IOCTL_SET_CLEAN,        // clear step count(qma7981 special)
	MSENSOR_IOCTL_MAX
}MSENSOR_IOCTL_E;
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
// refer to figure 1
typedef struct
{
	int32 x;            // x-axis value
	int32 y;            // y-axis value
	int32 z;            // z-axis value
}MSENSOR_MOTION_VECTOR_T;

typedef struct {
    /* sensor identifier */
    MSENSOR_TYPE_E  type;
    union {
        /* x,y,z values of the given sensor */
        // acc/gsensor: unit: mg
        MSENSOR_MOTION_VECTOR_T   vector;
        int32   temperature;
        int32   distance;
        int32   light;
    } u;
    uint32      reserved;
} MSENSOR_MOTION_DATA_T;

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/******************************************************************************/
// Description:   Create a semaphore to proect OPEN and CLOSE operations
// Dependence:
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC uint32 MSensor_InitModule(void);

/*****************************************************************************/
//  Description:    This function is used to open MSENSOR .
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MSensor_Open(MSENSOR_TYPE_E sensor_type);

/*****************************************************************************/
//  Description:    This function is used to close MSENSOR.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MSensor_Close(MSENSOR_TYPE_E sensor_type);
/*****************************************************************************/
//  Description: msensor ioctl
//  Note:        data_ptr[in/out] should be ready before call it
/*****************************************************************************/
PUBLIC uint32 MSensor_ioctl(MSENSOR_TYPE_E sensor_type,
    MSENSOR_IOCTL_E cmd,
    void * data_ptr);
/*****************************************************************************/
//  Description:    This function is used to handler interrupt
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MSensor_IntHandler(uint32 id);
/*****************************************************************************/
//  Description:    This function is used to SENSOR timer
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MSensor_TimerHandler(MSENSOR_TYPE_E sensor_type);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  // _MSENSOR_DRV_H_

// End of msensor_drv.h
