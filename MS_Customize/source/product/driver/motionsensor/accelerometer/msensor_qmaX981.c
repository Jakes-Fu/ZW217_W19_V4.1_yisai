/******************************************************************************
 ** File Name:      msensor_qmax981.c                                         *
 ** Author:         Haifeng.Yang                                              *
 ** DATE:           05/14/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/14/2010     Haifeng.Yang     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "msensor_drv.h"
#include "msensor_cfg.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define QMAX981_DEBUG
#define SENSOR_INT_MODE
#ifdef QMAX981_DEBUG
#define QMAX981_TRACE			SCI_TRACE_LOW
#else
#define QMAX981_TRACE(x)
#endif

#ifdef WIN32
#define __func__
#endif
//#define QMAX981_THRESHOLD_0_3G            300
#define QMAX981_THRESHOLD_0_5G            500
// #define QMAX981_THRESHOLD_0_6G            600
#define QMAX981_THRESHOLD_0_7G            700
#define QMAX981_THRESHOLD_0_8G            800
#define QMAX981_THRESHOLD_0_8_5G            850
#define QMAX981_THRESHOLD_0_9G            900
#define QMAX981_THRESHOLD_1_0G            1000
#define QMAX981_THRESHOLD_1_1G            1100
#define QMAX981_THRESHOLD_1_2G            1200
#define QMAX981_THRESHOLD_1_3G            1300
#define QMAX981_THRESHOLD_1_5G            1500
#define QMAX981_LOW_THRESHOLD             200

#define QMAX981_SHAKE_DETECT_TIME         200
#define QMAX981_DESHAKE_TIME              300

//#define USE_SPI

//#define QMA7981_DOUBLE_TRIPLE_CLICK
//#define QMA7981_ABNORMAL_SHAKE_CHECK

#define QMAX981_STEPCOUNTER
//#define QMA7981_STEP_INT
#define QMA7981_HAND_UP_DOWN

//#define QMAX981_USE_IRQ1
//#define QMA7981_ANY_MOTION
//#define QMA7981_NO_MOTION
//#define QMA7981_SIGNIFICANT_MOTION


typedef enum
{
	ACC_RANGE_2G,
	ACC_RANGE_4G,
	ACC_RANGE_8G,
	ACC_RANGE_16G,
	ACC_RANGE_32G,

	ACC_RANGE_TOTAL
}qmaX981_range;

#define QMAX981_I2C_SLAVE_ADDR		(0x12<<1)	// AD0 GND 0x12, AD0 VDD 0x13
#define QMAX981_I2C_SLAVE_ADDR2		(0x13<<1)	// AD0 GND 0x12, AD0 VDD 0x13

#define GRAVITY_EARTH_1000          9807	// about (9.80665f)*1000   mm/s2
#define QMAX981_ABS(X)              ((X) < 0 ? (-1 * (X)) : (X))

/*Register Map*/
#define QMAX981_CHIP_ID             0x00
#define QMAX981_XOUTL				0x01
#define QMAX981_XOUTH				0x02
#define QMAX981_YOUTL				0x03
#define QMAX981_YOUTH				0x04
#define QMAX981_ZOUTL				0x05
#define QMAX981_ZOUTH				0x06
#define QMAX981_STEP_CNT_L			0x07
#define QMAX981_INT_STAT0			0x0a
#define QMAX981_INT_STAT1			0x0b
#define QMAX981_INT_STAT2			0x0c
#define QMAX981_INT_STAT3			0x0d
#define QMAX981_FIFO_STATE			0x0e
#define QMA7981_STEP_CNT_M			0x0e
#define QMAX981_REG_RANGE			0x0f
#define QMAX981_REG_BW_ODR			0x10
#define QMAX981_REG_POWER_CTL		0x11
#define QMAX981_STEP_SAMPLE_CNT		0x12
#define QMAX981_STEP_PRECISION		0x13
#define QMAX981_STEP_TIME_LOW		0x14
#define QMAX981_STEP_TIME_UP		0x15
#define QMAX981_INTPIN_CFG			0x20
#define QMAX981_INT_CFG				0x21
#define QMAX981_OS_CUST_X		    0x27
#define QMAX981_OS_CUST_Y			0x28
#define QMAX981_OS_CUST_Z			0x29
#define QMAX981_STEP_TIME_UP		0x15
/*ODR SET @lower ODR*/
#define QMA6981_ODR_1000HZ			0x07
#define QMA6981_ODR_500HZ			0x06
#define QMA6981_ODR_250HZ			0x05
#define QMA6981_ODR_125HZ			0x04
#define QMA6981_ODR_62HZ			0x03
#define QMA6981_ODR_31HZ			0x02
#define QMA6981_ODR_16HZ			0x01
#define QMA6981_ODR_HIGH			0x20

/* Accelerometer Sensor Full Scale */
#define QMAX981_RANGE_2G			0x01
#define QMAX981_RANGE_4G			0x02
#define QMAX981_RANGE_8G			0x04
#define QMAX981_RANGE_16G			0x08
#define QMAX981_RANGE_32G			0x0f

/* 0x11 Set the sleep time, when device is in power cycling power saving.*/
#define QMA6981_SLEEP_DUR0			0x00
#define QMA6981_SLEEP_DUR1			0x06
#define QMA6981_SLEEP_DUR2			0x07
#define QMA6981_SLEEP_DUR4			0x08
#define QMA6981_SLEEP_DUR6			0x09
#define QMA6981_SLEEP_DUR10			0x0a
#define QMA6981_SLEEP_DUR25			0x0b
#define QMA6981_SLEEP_DUR50			0x0c
#define QMA6981_SLEEP_DUR100		0x0d
#define QMA6981_SLEEP_DUR500		0x0e
#define QMA6981_SLEEP_DUR1000		0x0f

#define STEP_W_TIME_L	300
#define STEP_W_TIME_H	250

//#define QMA6100_ODR_27
#define QMA6100_ODR_55
//#define QMA6100_ODR_100

typedef enum
{
	QMA6100_DISABLE = 0,
	QMA6100_ENABLE = 1
}qma6100_enable;

typedef enum
{
	QMA6100_MODE_SLEEP,
	QMA6100_MODE_ACTIVE,
	QMA6100_MODE_MAX
}qma6100_power_mode;

typedef enum
{
	QMA6100_FIFO_MODE_NONE,
	QMA6100_FIFO_MODE_FIFO,
	QMA6100_FIFO_MODE_STREAM,
	QMA6100_FIFO_MODE_BYPASS,
	QMA6100_FIFO_MODE_MAX
}qma6100_fifo_mode;

typedef enum
{
	QMA6100_TAP_SINGLE = 0x80,
	QMA6100_TAP_DOUBLE = 0x20,
	QMA6100_TAP_TRIPLE = 0x10,
	QMA6100_TAP_QUARTER = 0x01,
	QMA6100_TAP_MAX = 0xff
}qma6100_tap;

typedef enum
{
	QMA6100_MAP_INT1,
	QMA6100_MAP_INT2,
	QMA6100_MAP_INT_NONE
}qma6100_int_map;

typedef enum
{
	QMA6100_MCLK_500K = 0x00,
	QMA6100_MCLK_333K = 0x01,
	QMA6100_MCLK_200K = 0x02,
	QMA6100_MCLK_100K = 0x03,
	QMA6100_MCLK_50K = 0x04,
	QMA6100_MCLK_20K = 0x05,
	QMA6100_MCLK_10K = 0x06,
	QMA6100_MCLK_5K = 0x07,
	QMA6100_MCLK_RESERVED = 0xff
}qma6100_mclk;

typedef enum
{
	QMA6100_DIV_512 = 0x00,
	QMA6100_DIV_256 = 0x01,
	QMA6100_DIV_128 = 0x02,
	QMA6100_DIV_64 = 0x03,
	QMA6100_DIV_32 = 0x04,
	QMA6100_DIV_1024 = 0x05,
	QMA6100_DIV_2048 = 0x06,
	QMA6100_DIV_4096 = 0x07,
	QMA6100_DIV_RESERVED = 0xff
}qma6100_div;

typedef enum
{
	QMA6100_LPF_0 = (0x00<<5),
	QMA6100_LPF_2 = (0x01<<5),
	QMA6100_LPF_4 = (0x02<<5),
	QMA6100_LPF_16 = (0x03<<5),
	QMA6100_LPF_RESERVED = 0xff
}qma6100_lpf;

typedef enum
{
  QMA6100_STEP_LPF_0 = (0x00<<6),
  QMA6100_STEP_LPF_2 = (0x01<<6),
  QMA6100_STEP_LPF_4 = (0x02<<6),
  QMA6100_STEP_LPF_8 = (0x03<<6),
  QMA6100_STEP_LPF_RESERVED = 0xff
}qma6100_step_lpf;

typedef enum
{
  QMA6100_STEP_AXIS_ALL = 0x00,
  QMA6100_STEP_AXIS_YZ = 0x01,
  QMA6100_STEP_AXIS_XZ = 0x02,
  QMA6100_STEP_AXIS_XY = 0x03,
  QMA6100_STEP_AXIS_RESERVED = 0xff
}qma6100_step_axis;

typedef enum
{
  QMA6100_STEP_START_0 = 0x00,
  QMA6100_STEP_START_4 = 0x20,
  QMA6100_STEP_START_8 = 0x40,
  QMA6100_STEP_START_12 = 0x60,
  QMA6100_STEP_START_16 = 0x80,
  QMA6100_STEP_START_24 = 0xa0,
  QMA6100_STEP_START_32 = 0xc0,
  QMA6100_STEP_START_40 = 0xe0,
  QMA6100_STEP_START_RESERVED = 0xff
}qma6100_step_start_cnt;

/* Accelerometer Sensor Full Scale */

#if defined(QMA7981_DOUBLE_TRIPLE_CLICK)
typedef struct
{
	unsigned char check_click;
	unsigned short click_num;
	unsigned short read_data_num;
	unsigned short static_num;
	unsigned short t_msec_1;			// check_click timer
	unsigned short t_msec_2;			// check static timer
	unsigned short t_msec_out;			// timeout
}qst_click_check;
#endif

#if defined(QMA7981_ABNORMAL_SHAKE_CHECK)
typedef struct
{
	unsigned char check_shake;
	unsigned short shake_num;
	unsigned short t_msec_1;
	unsigned short t_msec_out;			// timeout
}qst_shake_check;
#endif
int32 qmaX981_init(void);
int32 qmaX981_read_acc(int32 *accData);
#if defined(QMAX981_STEPCOUNTER)
uint32 qmaX981_read_stepcounter(void);
#endif
#if defined(QMAX981_STEP_DEBOUNCE_IN_INT)
extern void qmaX981_step_debounce_reset(void);
extern int32 qmaX981_step_debounce_s32_work(int32 data, uint8 irq_level);
extern int32 qmaX981_step_debounce_read_data(int32 result);
#endif
#if defined(QMAX981_CHECK_ABNORMAL_DATA)
extern int32 qmaX981_check_abnormal_data(int32 data_in, int32 *data_out);
#endif

typedef enum
{
	CHIP_ID_TYPE_QMA6981 = 0,
	CHIP_ID_TYPE_QMA7981,
	CHIP_ID_TYPE_QMA6100,
	CHIP_ID_TYPE_UNDEFINE,
	CHIP_ID_TYPE_MAX
}qmaX981_type;

typedef struct
{
    int16 sign[3];
    uint16 map[3];
}qst_convert;

typedef struct
{
	uint8				chip_id;
	qmaX981_type		chip_type;
	int32				lsb_1g;
	uint8				layout;
	qst_convert			cvt;
	uint8				s32_level;
}qmaX981_data;

static const qst_convert qst_map[] =
{
    { { 1, 1, 1}, {0, 1, 2} },
    { {-1, 1, 1}, {1, 0, 2} },
    { {-1,-1, 1}, {0, 1, 2} },
    { { 1,-1, 1}, {1, 0, 2} },

    { {-1, 1, -1}, {0, 1, 2} },
    { { 1, 1, -1}, {1, 0, 2} },
    { { 1,-1, -1}, {0, 1, 2} },
    { {-1,-1, -1}, {1, 0, 2} },
	{ { 1, 1, -1}, {0, 1, 2} }	//sunwen.he add for w217 pcb
};


static qmaX981_data g_qmaX981;
#if !defined(USE_SPI)
static uint8 QMAX981_I2C_ADDR_W	= QMAX981_I2C_SLAVE_ADDR;
#endif
#if defined(QMA7981_DOUBLE_TRIPLE_CLICK)
static uint acc_data_curr[3];
static uint acc_data[3];
static qst_click_check g_click;
#endif
#if defined(QMA7981_ABNORMAL_SHAKE_CHECK)
static qst_shake_check g_shake;
#endif
/*
qma7981 odr setting
0x10<2:0>		ODR(Hz)				Time(ms)	|	RANGE 0x0f<3:0>
000				43.3125				23.088		|	0001	2g          244ug/LSB
001				86.4453				11.568		|	0010	4g          488ug/LSB
002				172.1763			5.808		|	0100	8g          977ug/LSB
003				341.5300			2.928		|	1000	16g         1.95mg/LSB
004				672.0430			1.488		|	1111	32g         3.91mg/LSB
005				32.5013				30.768		|	Others	2g          244ug/LSB
006				129.3995			7.728		|
007				257.2016			3.888		|
*/
const uint8 qma7981_init_tbl[][2] =
{
	{0x11, 0x80},
	{0x36, 0xb6},
	{0xff, 5},
	{0x36, 0x00},
	{0x0f, QMAX981_RANGE_4G},
	{0x10, 0xe1},		// ODR 130hz
	//{0x4a, 0x08},		//Force I2C I2C s32erface.SPI is disabled,SENB can be used as ATB
	//{0x20, 0x05},
	{0x11, 0x80},
	{0x5f, 0x80},		// enable test mode,take control the FSM
	{0x5f, 0x00},		//normal mode

	{0xff, 20}
};

/**---------------------------------------------------------------------------*
 **                         DataStruct Definition                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Variables and function                            *
 **---------------------------------------------------------------------------*/
LOCAL uint32 qmax981_init(void);
LOCAL uint32 qmax981_open(void);
LOCAL uint32 qmax981_close(void);
LOCAL void qmax981_TimerCallback(uint32 param);
LOCAL uint32 qmax981_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr);
LOCAL uint32 qmax981_SetSensi(MSENSOR_SENSI_E sensi_val);

PUBLIC MSENSOR_CFG_T g_qmax981_msensor_info =
{
    "QMA7981",
	MSENSOR_TYPE_ACCELEROMETER,
    MSENSOR_STATUS_NOT_INIT,
	qmax981_init,
	qmax981_open,
	qmax981_close,
	qmax981_ioctl
};
LOCAL MSENSOR_MISC_CFG_T s_qmax981_misc_cfg = {0};
LOCAL int32 s_qmax981_IIC_Handler           = -1;

#define TIMER_PERIOD_QMAX981                  500
LOCAL SCI_TIMER_PTR         s_qmax981_timer	= PNULL;
// LOCAL SCI_SEMAPHORE_PTR     s_lis35de_sm    = PNULL;
LOCAL int32                 s_qmax981_sh_th = 0; // shake threshhold for sensitivity
LOCAL int32                 s_qmax981_or_th = 0; // orientation threshhold for sensitivity
/*****************************************************************************/
//  Description:    read chip register content
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void msensor_delayms(unsigned short wait_ms)
{
	unsigned int wait_time = timer_TimRealTickGet(1);//Use TIMER1 16k
	unsigned int wait_time2 = wait_ms*16384/1000;

	while((timer_TimRealTickGet(1) - wait_time) < wait_time2){}
}

LOCAL uint32 qmax981_read(uint8 addr, uint8* data,uint8 len)
{
	if(I2C_HAL_Read(s_qmax981_IIC_Handler, &addr, data, len) == 0)
	{
		SCI_TraceLow("[qmax981]read error!!!");
		return MSENSOR_ERROR_IIC_READ_FAILED;
	}
	//READ OK!
	return MSENSOR_SUCCESS;
}

/*****************************************************************************/
//  Description:    write configuration data to chip registers
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 qmax981_write(uint8 addr, uint8 data)
{
	if(I2C_HAL_Write(s_qmax981_IIC_Handler, &addr, &data, 1) == 0)
	{
		SCI_TraceLow("[qmax981]write error!!!");
		return MSENSOR_ERROR_IIC_WRITE_FAILED;
	}
	//WRITE OK!
	return MSENSOR_SUCCESS;
}

/*****************************************************************************/
//  Description:    Init g-sensor chip
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void qmaX981_set_range(uint8 range)
{
	uint8 ret;
	//SCI_TraceLow("[qmax981]set_range range:%d\n",range);

	if(g_qmaX981.chip_type == CHIP_ID_TYPE_QMA6981)
	{
		if(range == QMAX981_RANGE_4G)
			g_qmaX981.lsb_1g = 128;
		else if(range == QMAX981_RANGE_8G)
			g_qmaX981.lsb_1g = 64;
		else
			g_qmaX981.lsb_1g = 256;
	}
	else if((g_qmaX981.chip_type == CHIP_ID_TYPE_QMA7981)||(g_qmaX981.chip_type == CHIP_ID_TYPE_QMA6100))
	{
		if(range == QMAX981_RANGE_4G)
			g_qmaX981.lsb_1g = 2048;
		else if(range == QMAX981_RANGE_8G)
			g_qmaX981.lsb_1g = 1024;
		else if(range == QMAX981_RANGE_16G)
			g_qmaX981.lsb_1g = 512;
		else if(range == QMAX981_RANGE_32G)
			g_qmaX981.lsb_1g = 256;
		else
			g_qmaX981.lsb_1g = 4096;
	}

	ret = qmax981_write(QMAX981_REG_RANGE, range);
	if(ret == 0){
		SCI_TraceLow("[qmax981]set_range error!!!\n");
	}
}
#if defined(QMAX981_STEPCOUNTER)
uint32 qmaX981_read_stepcounter(void)
{
	uint8 data[3] = {0};
	int32 ret;
	uint32 step_num = 0;

	if(g_qmaX981.chip_type == CHIP_ID_TYPE_QMA6981)
	{
		ret = qmax981_read(QMAX981_STEP_CNT_L, data, 2);
		step_num = (data[1]<<8)|data[0];
	}
	else if(g_qmaX981.chip_type == CHIP_ID_TYPE_QMA7981)
	{
		ret = qmax981_read(QMAX981_STEP_CNT_L, data, 2);
		ret = qmax981_read(QMA7981_STEP_CNT_M, &data[2], 1);
		step_num = (uint32)(((uint32)data[2]<<16)|((uint32)data[1]<<8)|data[0]);
	}
#if defined(QMAX981_CHECK_ABNORMAL_DATA)
	ret=qmaX981_check_abnormal_data(step_num, &step_num);
	if(ret != 0)
	{
		return -1;
	}
#endif
#if defined(QMAX981_STEP_DEBOUNCE_IN_INT)
	step_num = qmaX981_step_debounce_read_data(step_num);
#endif
	SCI_TraceLow("[qmax981]read_stepcounter step_num:%d!!!\n",step_num);
	return step_num;
}
#endif
static int32 qma7981_read_raw_xyz(int32 *data)
{
	uint8 databuf[6] = {0};
	short data_raw[3] = {0};
	int32 ret;
	//qma7981_acc_format data_14bit;

	ret = qmax981_read(QMAX981_XOUTL, databuf, 6);
	if(ret != MSENSOR_SUCCESS){
		SCI_TraceLow("[qmax981]read_acc error!!!\n");
		return 0;
	}
	data_raw[0] = (short)((databuf[1]<<8)|(databuf[0]));
	data_raw[1] = (short)((databuf[3]<<8)|(databuf[2]));
	data_raw[2] = (short)((databuf[5]<<8)|(databuf[4]));
	data[0] = (int32)data_raw[0]>>2;
	data[1] = (int32)data_raw[1]>>2;
	data[2] = (int32)data_raw[2]>>2;
	//SCI_TraceLow("[qmax981]read_raw_xyz x:%d,y:%d,z:%d \n",data[0],data[1],data[2]);
	return 1;
}

int32 qmaX981_read_acc(int32 *accData)
{
	int32 rawData[3]={0};
	uint8 data[3]= {0};
	int32 ret;
	uint32 step_num;

	ret=qma7981_read_raw_xyz(rawData);

	//SCI_TraceLow("[qmax981] read_acc cvt.map:{%d,%d,%d} \n",g_qmaX981.cvt.map[0],g_qmaX981.cvt.map[1],g_qmaX981.cvt.map[2]);
	//SCI_TraceLow("[qmax981] read_acc cvt.sign:{%d,%d,%d} \n",g_qmaX981.cvt.sign[0],g_qmaX981.cvt.sign[1],g_qmaX981.cvt.sign[2]);
	accData[g_qmaX981.cvt.map[0]] = g_qmaX981.cvt.sign[0]*rawData[0];
	accData[g_qmaX981.cvt.map[1]] = g_qmaX981.cvt.sign[1]*rawData[1];
	accData[g_qmaX981.cvt.map[2]] = g_qmaX981.cvt.sign[2]*rawData[2];

	accData[0] = (accData[0]*GRAVITY_EARTH_1000)/(g_qmaX981.lsb_1g);
	accData[1] = (accData[1]*GRAVITY_EARTH_1000)/(g_qmaX981.lsb_1g);
	accData[2] = (accData[2]*GRAVITY_EARTH_1000)/(g_qmaX981.lsb_1g);
	SCI_TraceLow("[qmax981]read_acc x:%d,y:%d,z:%d \n",accData[0],accData[1],accData[2]);

	ret = qmax981_read(QMAX981_STEP_CNT_L, data, 2);
	ret = qmax981_read(QMA7981_STEP_CNT_M, &data[2], 1);
	step_num = (uint32)(((uint32)data[2]<<16)|((uint32)data[1]<<8)|data[0]);
	SCI_TraceLow("[qmax981]read_stepcounter step_num:%d!!!\n",step_num);

	return ret;
}

#if defined(QMA7981_HAND_UP_DOWN)
void qma7981_set_hand_up_down(int layout)
{
#if 1//defined(QMA7981_SWAP_XY)
	uint8 reg_0x42 = 0;
#endif
	uint8 reg_0x1e = 0;
	uint8 reg_0x34 = 0;
	uint8 yz_th_sel = 4;
	char y_th = -3; //-2;				// -16 ~ 15
	uint8 x_th = 6;		// 0--7.5
	char z_th = 6;				// -8--7
	SCI_TraceLow("[qmax981]set_hand_up_down layout:%d\n",layout);

#if 1//defined(QMA7981_SWAP_XY)	// swap xy
	if(layout%2)
	{
		qmax981_read(0x42, &reg_0x42, 1);
		reg_0x42 |= 0x80;		// 0x42 bit 7 swap x and y
		qmax981_write(0x42, reg_0x42);
	}
#endif

	if((layout >=0) && (layout<=3))
	{
		z_th = 3;
		if((layout == 2)||(layout == 3))
			y_th = 3;
		else if((layout == 0)||(layout == 1))
			y_th = -3;
	}
	else if((layout >=4) && (layout<=7))
	{
		z_th = -3;

		if((layout == 6)||(layout == 7))
			y_th = 3;
		else if((layout == 4)||(layout == 5))
			y_th = -3;
	}

	// 0x34 YZ_TH_SEL[7:5]	Y_TH[4:0], default 0x9d  (YZ_TH_SEL   4   9.0 m/s2 | Y_TH  -3  -3 m/s2)
	//qmax981_write(0x34, 0x9d);	//|yz|>8 m/s2, y>-3 m/m2
	if((y_th&0x80))
	{
		reg_0x34 |= yz_th_sel<<5;
		reg_0x34 |= (y_th&0x0f)|0x10;
		qmax981_write(0x34, reg_0x34);
	}
	else
	{
		reg_0x34 |= yz_th_sel<<5;
		reg_0x34 |= y_th;
		qmax981_write(0x34, reg_0x34);	//|yz|>8m/s2, y<3 m/m2
	}
	//Z_TH<7:4>: -8~7, LSB 1 (unit : m/s2)	X_TH<3:0>: 0~7.5, LSB 0.5 (unit : m/s2)
	//qmax981_write(0x1e, 0x68);	//6 m/s2, 4 m/m2

	qmax981_write(0x2a, (0x19|(0x03<<6)));			// 12m/s2 , 0.5m/s2
	qmax981_write(0x2b, (0x7c|(0x03>>2)));
	//qmax981_write(0x2a, (0x19|(0x02<<6)));			// 12m/s2 , 0.5m/s2
	//qmax981_write(0x2b, (0x7c|(0x02)));

	//qmax981_read(0x1e, &reg_0x1e, 1);
	if((z_th&0x80))
	{
		reg_0x1e |= (x_th&0x0f);
		reg_0x1e |= ((z_th<<4)|0x80);
		qmax981_write(0x1e, reg_0x1e);
	}
	else
	{
		reg_0x1e |= (x_th&0x0f);
		reg_0x1e |= (z_th<<4);
		qmax981_write(0x1e, reg_0x1e);
	}
}
#endif

LOCAL uint8 qmaX981_chip_id()
{
	uint8 chip_id = 0x00;
	qmax981_write(QMAX981_REG_POWER_CTL, 0x80);
	qmax981_read(QMAX981_CHIP_ID, &chip_id, 1);
	SCI_TraceLow("[qmax981]chip_id=0x%x\n", chip_id);
	return chip_id;
}
static int32 qma7981_initialize(void)
{
	uint32 i;
	uint8 data[2] = {0};
	uint8 reg = 0,total=0;
	int32 ret = 0;
	uint8 DieId_H = 0, DieId_L = 0, WaferID = 0;
	uint8 reg_0x10 = 0;
	uint8 reg_0x11 = 0;
#if defined(QMAX981_STEPCOUNTER)
	uint8 reg_0x14 = 0;
	uint8 reg_0x15 = 0;
#endif
	uint8 reg_0x16 = 0;
	uint8 reg_0x18 = 0;
	uint8 reg_0x19 = 0;
	uint8 reg_0x1a = 0;
#if defined(QMA7981_ANY_MOTION)||defined(QMA7981_NO_MOTION)
	uint8 reg_0x2c = 0;
#endif
	uint8 reg_0x20 = 0;

	//SCI_TraceLow("[qmax981]qma7981_initialize start \n");

	total = sizeof(qma7981_init_tbl)/sizeof(qma7981_init_tbl[0]);
	for(i = 0; i < total; i++)
	{
		data[0] = qma7981_init_tbl[i][0];
		data[1] = qma7981_init_tbl[i][1];
		if(data[0] == 0xff)
		{
			OS_TickDelay(data[1]);
		}
		else
		{
			if(data[0] == QMAX981_REG_RANGE)
			{
				if(data[1] == QMAX981_RANGE_4G)
					g_qmaX981.lsb_1g = 2048;
				else if(data[1] == QMAX981_RANGE_8G)
					g_qmaX981.lsb_1g = 1024;
				else if(data[1] == QMAX981_RANGE_16G)
					g_qmaX981.lsb_1g = 512;
				else if(data[1] == QMAX981_RANGE_32G)
					g_qmaX981.lsb_1g = 256;
				else
					g_qmaX981.lsb_1g = 4096;
			}
			ret = qmax981_write(data[0],data[1]);
			if(ret != MSENSOR_SUCCESS)
			{
				SCI_TraceLow("[qmax981]qma7981_initialize ret=%d\n", ret);
				return ret;
			}
			OS_TickDelay(2);
		}
	}

	// read reg
	qmax981_read(0x47, &DieId_L, 1);
	qmax981_read(0x48, &DieId_H, 1);
	qmax981_read(0x5a, &WaferID, 1);
	SCI_TraceLow("[qmax981]DieId_L:%d DieId_H:%d WaferID:%d\n", DieId_L, DieId_H, WaferID&0x1f);
	qmax981_read(0x16, &reg_0x16, 1);
	qmax981_read(0x18, &reg_0x18, 1);
	qmax981_read(0x19, &reg_0x19, 1);
	qmax981_read(0x1a, &reg_0x1a, 1);
#if defined(QMA7981_ANY_MOTION)||defined(QMA7981_NO_MOTION)
	qmax981_read(0x2c, &reg_0x2c, 1);
#endif
	qmax981_read(0x20, &reg_0x20, 1);

	//0xe0	[MCLK/7695]
	//0xe1	[MCLK/3855]
	//0xe2	[MCLK/1935]
	//0xe3	[MCLK/975]
	reg_0x10 = 0xe1;
	qmax981_write(0x10, reg_0x10);
	reg_0x11 = 0x80;
	qmax981_write(0x11, reg_0x11);

#if defined(QMAX981_STEPCOUNTER)
	if(reg_0x11 == 0x80)		// 500K
	{
		reg_0x10 = 0xe1;
		qmax981_write(0x10, reg_0x10);
		reg_0x14 = (((STEP_W_TIME_L*100)/771)+1);		// odr 129.7hz, 7.71ms
		reg_0x15 = (((STEP_W_TIME_H*100)/771)+1);
		if(reg_0x10 == 0xe0)		// odr 65hz
		{
			reg_0x14 = (reg_0x14>>1);
			reg_0x15 = (reg_0x15>>1);
		}
		else if(reg_0x10 == 0xe5)	// odr 32.5hz
		{
			reg_0x14 = (reg_0x14>>2);
			reg_0x15 = (reg_0x15>>2);
		}
	}
	else if(reg_0x11 == 0x81)	// 333K
	{
		reg_0x10 = 0xe1;
		qmax981_write(0x10, reg_0x10);

		reg_0x14 = (((STEP_W_TIME_L*100)/581)+1);	// odr 172.0930233 hz, 5.81ms
		reg_0x15 = (((STEP_W_TIME_H*100)/581)+1);
		if(reg_0x10 == 0xe1)	// 86.38132296 hz
		{
			reg_0x14 = (reg_0x14>>1);
			reg_0x15 = (reg_0x15>>1);
		}
		else if(reg_0x10 == 0xe0)		// 43.2748538
		{
			reg_0x14 = (reg_0x14>>2);
			reg_0x15 = (reg_0x15>>2);
		}
	}
	else if(reg_0x11 == 0x82)		// 200K
	{
		reg_0x10 = 0xe2;
		qmax981_write(0x10, reg_0x10);

		reg_0x14 = (((STEP_W_TIME_L*100)/967)+1);	// 103.3591731 hz, 9.675 ms
		reg_0x15 = (((STEP_W_TIME_H*100)/967)+1);
		if(reg_0x10 == 0xe1)
		{
			reg_0x14 = (reg_0x14>>1);		// 51.88067445 hz
			reg_0x15 = (reg_0x15>>1);
		}
		else if(reg_0x10 == 0xe3)
		{
			reg_0x14 = (reg_0x14<<1);		// 205.1282051 hz
			reg_0x15 = (reg_0x15<<1);
		}
	}
	else if(reg_0x11 == 0x83)		// 100K
	{
		reg_0x10 = 0xe3;
		qmax981_write(0x10, reg_0x10);

		reg_0x14 = (((STEP_W_TIME_L*100)/975)+1);	// 102.5641026 hz, 9.75 ms
		reg_0x15 = (((STEP_W_TIME_H*100)/975)+1);
		if(reg_0x10 == 0xe2)
		{
			reg_0x14 = (reg_0x14>>1);		// 51.67958656 hz
			reg_0x15 = (reg_0x15>>1);
		}
	}

	SCI_TraceLow("[qmax981] 0x14[%d] 0x15[%d]\n", reg_0x14, reg_0x15);
	qmax981_write(0x12, 0x94);
	qmax981_write(0x13, 0x80);		// clear step
	qmax981_write(0x13, 0x7f);		// 0x7f(1/16) 0x00(1/8)
	qmax981_write(0x14, reg_0x14);		// STEP_TIME_LOW<7:0>*(1/ODR)
	qmax981_write(0x15, reg_0x15);		// STEP_TIME_UP<7:0>*8*(1/ODR)

	//qmax981_write(0x1f, 0x09); 	// 0 step
	//qmax981_write(0x1f, 0x29); 	// 4 step
	//qmax981_write(0x1f, 0x49); 	// 8 step
	//qmax981_write(0x1f, 0x69); 	// 12 step
	//qmax981_write(0x1f, 0x89); 	// 16 step
	qmax981_write(0x1f, 0xa9);		// 24 step
	//qmax981_write(0x1f, 0xc9); 	// 32 step
	//qmax981_write(0x1f, 0xe9); 	// 40 step

	// step int
#if defined(QMA7981_STEP_INT)
	reg_0x16 |= 0x08;
	reg_0x19 |= 0x08;
	qmax981_write(0x16, reg_0x16);
	qmax981_write(0x19, reg_0x19);
#endif
#if defined(QMA7981_SIGNIFICANT_STEP)
	qmax981_write(0x1d, 0x26);		//every 30 step
	reg_0x16 |= 0x40;
	reg_0x19 |= 0x40;
	qmax981_write(0x16, reg_0x16);
	qmax981_write(0x19, reg_0x19);
#endif
#endif

//RANGE<3:0> Acceleration range Resolution
//0001 2g 244ug/LSB
//0010 4g 488ug/LSB
//0100 8g 977ug/LSB
//1000 16g 1.95mg/LSB
//1111 32g 3.91mg/LSB
//Others 2g 244ug/LSB

//0x2c
//Duration = (NO_MOT_DUR<3:0> + 1) * 1s, if NO_MOT_DUR<5:4> =b00
//Duration = (NO_MOT_DUR<3:0> + 4) * 5s, if NO_MOT_DUR<5:4> =b01
//Duration = (NO_MOT_DUR<3:0> + 10) * 10s, if NO_MOT_DUR<5:4> =b1x
//ANY_MOT_DUR<1:0>: any motion interrupt will be triggered when slope > ANY_MOT_TH for (ANY_MOT_DUR<1:0> + 1) samples

//0x2e ANY MOTION MOT_CONF2
//TH= ANY_MOT_TH<7:0> * 16 * LSB

#if defined(QMA7981_ANY_MOTION)
	reg_0x18 |= 0x07;
	reg_0x1a |= 0x01;
	reg_0x2c |= 0x00;	//BIT[0-1]	 (ANY_MOT_DUR<1:0> + 1) samples

	qmax981_write(0x18, reg_0x18);
	qmax981_write(0x1a, reg_0x1a);
	qmax981_write(0x2c, reg_0x2c);
	//qmax981_write(0x2e, 0x14);	// 0.488*16*20 = 156mg
	//qmax981_write(0x2e, 0x80);	// 0.488*16*128 = 1g
	//qmax981_write(0x2e, 0xa0);	// 0.488*16*160 = 1.25g
	//qmax981_write(0x2e, 0x60);	// 0.488*16*96 = 750mg
	//qmax981_write(0x2e, 0x40);	// 0.488*16*64 = 500mg
	//qmax981_write(0x2e, 0x20);	// 0.488*16*32 = 250mg
	qmax981_write(0x2e, 0x40);		// 0.488*16*64 = 500mg

#if defined(QMA7981_ABNORMAL_SHAKE_CHECK)
	reg_0x10 = 0xe0;		// ODR: 65hz 15.48 ms
	qmax981_write(0x10, reg_0x10);
	qmaX981_set_range(QMAX981_RANGE_8G);
	qmax981_write(0x2e, 0x60);		// 0.977*16*96 = 1500mg
#endif

#if defined(QMA7981_SIGNIFICANT_MOTION)
	//SIG_MOT_TPROOF [BIT4-5]<1:0>: 00: T_PROOF=0.25s,	01: T_PROOF=0.5s,  10: T_PROOF=1s,	11: T_PROOF=2s
	//SIG_MOT_TSKIP[BIT2-3]<1:0>: 00: T_SKIP=1.5s,	01: T_SKIP=3s,	10: T_SKIP=6s,	11: T_SKIP=12s
	//SIG_MOT_SEL: 1: select significant motion interrupt ,  0: select any motion interrupt

	//qmax981_write(0x2f, 0x0c|0x01);
	qmax981_write(0x2f, 0x01);		// bit0   1 significant motion, 0: any motion.

	reg_0x19 |= 0x01;
	qmax981_write(0x19, reg_0x19);
#endif
#endif
#if defined(QMA7981_NO_MOTION)
	reg_0x18 |= 0xe0;
	reg_0x1a |= 0x80;
	reg_0x2c |= 0x00;	//1s	//0x24;

	qmax981_write(0x18, reg_0x18);
	qmax981_write(0x1a, reg_0x1a);
	qmax981_write(0x2c, reg_0x2c);
	qmax981_write(0x2d, 0x14);
#endif

#if defined(QMA7981_HAND_UP_DOWN)
	qma7981_set_hand_up_down(g_qmaX981.layout);
	reg_0x16 |= 0x02;
	reg_0x19 |= 0x02;
	qmax981_write(0x16, reg_0x16);	// hand up
	qmax981_write(0x19, reg_0x19);
	reg_0x16 |= 0x04;
	reg_0x19 |= 0x04;
	qmax981_write(0x16, reg_0x16);	// hand down
	qmax981_write(0x19, reg_0x19);
#endif

#if defined(QMA7981_DATA_READY)
	reg_0x1a |= 0x10;
	qmax981_write(0x17, 0x10);
	qmax981_write(0x1a, reg_0x1a);
#endif

#if defined(QMA7981_INT_LATCH)
	qmax981_write(0x21, 0x1f);	// default 0x1c, step latch mode
#endif
	SCI_TraceLow("[qmax981] 0x20[0x%x]\n", reg_0x20);
	// int default level set //0x05 L_H_L:_______/````````\________
	//reg_0x20 &=0xf0;		//0x00  H_L_H:```````\________/````````
	//qmax981_write(0x20, reg_0x20);
	// int default level set

#if defined(QMA7981_DOUBLE_TRIPLE_CLICK)
	//memset(&g_click, 0, sizeof(g_click));
	g_click.check_click = 1;
	g_click.click_num = 0;
	g_click.static_num = 0;
	g_click.read_data_num = 0;

	g_click.t_msec_1 = 75;
	g_click.t_msec_2 = 8;
	g_click.t_msec_out = 300;
	//g_click.t_msec_1 = 200;
	//g_click.t_msec_2 = 8;
	//g_click.t_msec_out = 350;
#endif
#if defined(QMA7981_ABNORMAL_SHAKE_CHECK)
	g_shake.check_shake = 1;
	g_shake.shake_num = 0;
	g_shake.t_msec_1 = 200;
	g_shake.t_msec_out = 500;
#endif
	//SCI_TraceLow("[qmax981]qma7981_initialize end \n");
    return 0;
}

void qma6100_stepcounter_config(int enable)
{
//  qs32 odr = 27;   // 100Hz
unsigned char reg_12 = 0x00;
unsigned char reg_14 = 0x00;
unsigned char reg_15 = 0x00;
unsigned char reg_1e = 0x00;

	qmax981_write(0x13, 0x80);
	msensor_delayms(3);
	qmax981_write(0x13, 0x7f);     //  0x7f(P2P/16), 0.977*16*LSB
	// Windows time
	if(enable)
	{
	#if defined(QMA6100_ODR_27)
		reg_12 = 0x84;
		reg_14 = 0x05;      //((200*odr)/(1000));      // about:200 ms
		reg_15 = 0x07;      //(((2200/8)*odr)/1000);   // 2000 ms
	#elif defined(QMA6100_ODR_55)
		reg_12 = 0x89;  //0x89
		reg_14 = 0x0b;    //0x0c  //((200*odr)/(1000));
		reg_15 = 0x0c;     //0x0e //(((2200/8)*odr)/1000);   // 2000 ms
	#elif defined(QMA6100_ODR_100)
		reg_12 = 0x91;//0x8f
		reg_14 = 0x1a;//0x1c//0x1f;      //((280*odr)/(1000));      // about:280 ms
		reg_15 = 0x20;      //(((2200/8)*odr)/1000);   // 2000 ms
	#endif
		//QMA6100_IRQ_LOG("step time config 0x14=0x%x 0x15=0x%x\n", reg_14, reg_15);
		qmax981_write(0x12, reg_12);
		qmax981_write(0x14, reg_14);
		qmax981_write(0x15, reg_15);
		// lpf
		qmax981_read(0x1e, &reg_1e,1);
		reg_1e &= 0x3f;
		qmax981_write(0x1e, (reg_1e|QMA6100_STEP_LPF_0));   // default 0x08
		// start count, p2p, fix peak
		qmax981_write(0x1f, QMA6100_STEP_START_12|0x08);    // 0x10
	}

}
void qma6100_soft_reset(void)
{
	unsigned char reg_0x11 = 0;
	unsigned char reg_0x33 = 0;
	int retry = 0;

	//SCI_TraceLow("[qmax981] qma6100_soft_reset");
	qmax981_write(0x36, 0xb6);
	msensor_delayms(5);
	qmax981_write(0x36, 0x00);
	msensor_delayms(5);
	retry = 0;
	while(reg_0x11 != 0x84)
	{
		qmax981_write(0x11, 0x84);
		msensor_delayms(2);
		qmax981_read(0x11, &reg_0x11,1);
		//SCI_TraceLow("[qmax981] confirm read 0x11 = 0x%x\n",reg_0x11);
		if(retry++ > 100)
			break;
	}

	// load otp
	qmax981_write(0x33, 0x08);
	msensor_delayms(5);

	retry = 0;
	while(reg_0x33 != 0x05)
	{
		qmax981_read(0x33, &reg_0x33,1);
		//SCI_TraceLow("[qmax981] confirm read 0x33 = 0x%x\n",reg_0x33);
		msensor_delayms(2);
		if(retry++ > 100)
			break;
	}
}

static int qma6100_initialize(void)
{
	uint32 i;
	uint8 data[2] = {0};
	uint8 reg = 0,total=0;
	unsigned char reg_0x0f;
	int32 ret = 0;

	qma6100_soft_reset();

	qmax981_write(0x4a, 0x20);
	qmax981_write(0x50, 0x51);   //0x49  0x51
	qmax981_write(0x56, 0x01);

	//qmaX981_set_range(QMA6100_RANGE_4G);
	reg_0x0f = QMAX981_RANGE_4G;

	if(reg_0x0f == QMAX981_RANGE_4G)
		g_qmaX981.lsb_1g = 2048;
	else if(reg_0x0f == QMAX981_RANGE_8G)
		g_qmaX981.lsb_1g = 1024;
	else if(reg_0x0f == QMAX981_RANGE_16G)
		g_qmaX981.lsb_1g = 512;
	else if(reg_0x0f == QMAX981_RANGE_32G)
		g_qmaX981.lsb_1g = 256;
	else
		g_qmaX981.lsb_1g = 4096;

	qmax981_write(0x0f, reg_0x0f);

#if defined(QMA6100_ODR_27)
	qmax981_write(0x10, 0x06);
#elif defined(QMA6100_ODR_55)
	qmax981_write(0x10, 0x05);
#elif defined(QMA6100_ODR_100)
	qmax981_write(0x10, 0x00);
#endif
	qmax981_write(0x11, 0x84);

	qmax981_write(0x4A, 0x28);
	msensor_delayms(5);
	qmax981_write(0x20, 0x05);
	msensor_delayms(5);
	qmax981_write(0x5F, 0x80);
	msensor_delayms(5);
	qmax981_write(0x5F, 0x00);
	msensor_delayms(5);

#if 1 //defined(QMAX981_STEP_COUNTER)
	qma6100_stepcounter_config(1);
#endif

	return 0;
}

LOCAL uint32 qmax981_init(void)
{
	uint8 ctrl_data[2];
	int32 ret = 0;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo(GPIO_PROD_GSENSOR_INT_ID);

	//SCI_TraceLow("[qmax981]qmax981_init !!!\n");
    if(cfg_ptr)
		GPIO_DisableIntCtl(cfg_ptr->gpio_num);

    MSensor_GetMiscCfg(&g_qmax981_msensor_info, &s_qmax981_misc_cfg);
	SCI_TraceLow("[qmax981]init IIC addr=%x\n",s_qmax981_misc_cfg.i2c_cfg.slave_addr);
    qmax981_SetSensi(MSENSOR_SENSI_MID);
	if((s_qmax981_IIC_Handler == -1) &&
		((s_qmax981_IIC_Handler = I2C_HAL_Open(&(s_qmax981_misc_cfg.i2c_cfg))) == -1))
	{
		//QMAX981_TRACE:"MSENSOR:: ==> open IIC failed!\r\n"
		SCI_TraceLow("[qmax981]init open IIC failed!\n");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}
	memset(&g_qmaX981, 0, sizeof(g_qmaX981));
	g_qmaX981.chip_id = qmaX981_chip_id();
	if((g_qmaX981.chip_id>=0xa9) && (g_qmaX981.chip_id<=0xb9))
	{
		//SCI_TraceLow("[qmax981]init qma6981 find\n");
		g_qmaX981.chip_type = CHIP_ID_TYPE_QMA6981;
	}
	else if((g_qmaX981.chip_id>=0xe0) && (g_qmaX981.chip_id<=0xe7))
	{
		SCI_TraceLow("[qmax981]init qma7981 find\n");
		g_qmaX981.chip_type = CHIP_ID_TYPE_QMA7981;
	}
	else if(g_qmaX981.chip_id==0xfa)
	{
		//SCI_TraceLow("[qmax981]init qma6100 find\n");
		g_qmaX981.chip_type = CHIP_ID_TYPE_QMA6100;
	}
	else
	{
		SCI_TraceLow("[qmax981]init qma acc chip id =%x not defined!!!\n",g_qmaX981.chip_id);
		g_qmaX981.chip_type = CHIP_ID_TYPE_UNDEFINE;
		return MSENSOR_ERROR;
	}
	// add by yangzhiqiang
	//g_qmaX981.layout = 6;
	//sunwen.he add for w217 pcb x y z orientation
	g_qmaX981.layout = 8;
	memcpy(&g_qmaX981.cvt, &qst_map[g_qmaX981.layout], sizeof(qst_convert));

	if(g_qmaX981.chip_type == CHIP_ID_TYPE_QMA7981)
		qma7981_initialize();
	else if(g_qmaX981.chip_type == CHIP_ID_TYPE_QMA6100)
		qma6100_initialize();

#if defined(QMAX981_STEP_DEBOUNCE_IN_INT)
	qmaX981_step_debounce_reset();
#endif

#if defined(QMAX981_USE_IRQ1)
	qmaX981_setup_irq1();
#endif

    if(cfg_ptr)
		GPIO_EnableIntCtl(cfg_ptr->gpio_num);

	I2C_HAL_Close(s_qmax981_IIC_Handler);
	s_qmax981_IIC_Handler = -1;

	//OK, it seems like a QMAX981 CHIP!
	//QMAX981_TRACE:"MSENSOR:: ==> SUCCEESSFULLY!"
	SCI_TraceLow("[qmax981] init SUCCEESSFULLY!!!\n");
	return MSENSOR_SUCCESS;
}

/*****************************************************************************/
//  Description:    open g-sensor chip
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 qmax981_open(void)
{
	uint8 val=0;

    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);

	//SCI_TraceLow("[qmax981]qmax981_open !!!\n");
	if((s_qmax981_IIC_Handler == -1) &&
		((s_qmax981_IIC_Handler = I2C_HAL_Open(&(s_qmax981_misc_cfg.i2c_cfg))) == -1))
	{
		//QMAX981_TRACE:"MSENSOR::==> open IIC failed!\r\n"
		SCI_TraceLow("[qmax981]open IIC failed!\n");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}
	//qma7981_initialize();
	qmax981_read(QMAX981_REG_POWER_CTL,&val,1);
	//SCI_TraceLow("[qmax981]qmax981_open val:%x\n",val);
	if(!(val&0x80))
	{
		val |= 0x80;
		qmax981_write(QMAX981_REG_POWER_CTL, val);
	}
    if(!cfg_ptr)
    {
		if(s_qmax981_timer == NULL)
		{
			//SCI_TraceLow("[qmax981] open SCI_CreateTimer start!");
			s_qmax981_timer = SCI_CreateTimer("qmax981_Timer", qmax981_TimerCallback,
				MSENSOR_TYPE_ACCELEROMETER, TIMER_PERIOD_QMAX981, SCI_AUTO_ACTIVATE);
			//SCI_TraceLow("[qmax981] open SCI_CreateTimer end s_qmax981_timer:%p",s_qmax981_timer);
		}
		else
		{
			SCI_ChangeTimer(s_qmax981_timer, qmax981_TimerCallback, TIMER_PERIOD_QMAX981);
			SCI_ActiveTimer(s_qmax981_timer);
		}
    }
	// msensor_drv.c has a semaphore already
	#if 0
	if(s_lis35de_sm == NULL)
	{
		s_lis35de_sm = SCI_CreateSemaphore ("MSENSOR SEMAPHORE", 1);
		SCI_ASSERT(s_lis35de_sm != NULL);
	}
	#endif
	//QMAX981_TRACE:"MSENSOR:: ==> SUCCEESSFULLY!"
	SCI_TraceLow("[qmax981] open SUCCEESSFULLY!");
	return MSENSOR_SUCCESS;
}
/*****************************************************************************/
//  Description:    close g-sensor chip
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 qmax981_close(void)
{
	uint8   reg_data[2] = {0};
	uint8   val = 0;
	//Timer and semaphore need NOT be deleted. Just to deactive them.
	if(s_qmax981_timer != NULL)
	{
		SCI_DeactiveTimer(s_qmax981_timer);
	}

	qmax981_read(QMAX981_REG_POWER_CTL,&val,1);
	//SCI_TraceLow("[qmax981]qmax981_close val:%x\n",val);
	/*
	if(val&0x80)
	{
		val &= 0x7f;
		qmax981_write(QMAX981_REG_POWER_CTL, val);
	}
	*/
	if(s_qmax981_IIC_Handler != -1)
	{
		I2C_HAL_Close(s_qmax981_IIC_Handler);
		s_qmax981_IIC_Handler = -1;
	}
	SCI_TraceLow("[qmax981]close");
	return MSENSOR_SUCCESS;
}

uint32 qmax981_last_steps=0;
uint32 qmax981_read_steps=0;
static uint8 read_flag=0;
/******************************************************************************/
// Description:
// Dependence:
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 qmax981_GetVector(MSENSOR_MOTION_DATA_T * data_ptr)
{
	uint8 reg[6] = {0}, data[3]= {0,0,0};
	int16 data_raw[3] = {0};
	int32 x,y,z,temp,rawData[3]= {0},i = 0;
	//static uint8 read_flag=0;
	// msensor_drv.c has a semaphore already
	// SCI_GetSemaphore(s_lis35de_sm, SCI_WAIT_FOREVER);
	qmax981_read(QMAX981_STEP_CNT_L, data, 2);
	if(g_qmaX981.chip_type == CHIP_ID_TYPE_QMA7981)
	{
		qmax981_read(QMA7981_STEP_CNT_M, &data[2], 1);
	}
	else if(g_qmaX981.chip_type == CHIP_ID_TYPE_QMA6100)
	{
		qmax981_read(0x0d, &data[2], 1);
	}
	qmax981_read_steps = (uint32)(((uint32)data[2]<<16)|((uint32)data[1]<<8)|data[0]);
	if(read_flag==0)
	{
		read_flag=1;
		qmax981_last_steps = qmax981_read_steps;
	}

	if(qmax981_read_steps>=qmax981_last_steps)
	{
		data_ptr->reserved = qmax981_read_steps-qmax981_last_steps;
	}
	else
	{
		qmax981_last_steps = qmax981_read_steps;
		data_ptr->reserved = 0;
	}
	//SCI_TraceLow("[qmax981]read_step= %d,last_step=%d",qmax981_read_steps,qmax981_last_steps);

    qmax981_read(QMAX981_XOUTL,reg,6);
    data_raw[0] = (int16)((reg[1] << 8) | reg[0]);
	data_raw[1] = (int16)((reg[3] << 8) | reg[2]);
	data_raw[2] = (int16)((reg[5] << 8) | reg[4]);
	rawData[0] = (int32)data_raw[0] >> 2;
	rawData[1] = (int32)data_raw[1] >> 2;
	rawData[2] = (int32)data_raw[2] >> 2;
	//SCI_TraceLow("[qmax981]GetVector SENSOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d",\
		//rawData[0], rawData[1], rawData[2]);
	//SCI_TraceLow("[qmax981] GetVector SENSOR_COOR: g_qmaX981.lsb_1g = %d",\
		//g_qmaX981.lsb_1g);
	rawData[0] = (rawData[0]*GRAVITY_EARTH_1000)/(g_qmaX981.lsb_1g);
	rawData[1] = (rawData[1]*GRAVITY_EARTH_1000)/(g_qmaX981.lsb_1g);
	rawData[2] = (rawData[2]*GRAVITY_EARTH_1000)/(g_qmaX981.lsb_1g);
	x = g_qmaX981.cvt.sign[0]*rawData[g_qmaX981.cvt.map[0]];
	y = g_qmaX981.cvt.sign[1]*rawData[g_qmaX981.cvt.map[1]];
	z = g_qmaX981.cvt.sign[2]*rawData[g_qmaX981.cvt.map[2]];

	SCI_TraceLow("[qmax981]GetVector SENSOR_COOR: acc_x = %d, acc_y = %d, acc_z = %d,step_num:%d",\
		x,y,z,data_ptr->reserved);
	data_ptr->u.vector.x = x;
	data_ptr->u.vector.y = y;
	data_ptr->u.vector.z = z;
	data_ptr->type     = MSENSOR_TYPE_ACCELEROMETER;
#if 0
	{
		uint8 i=0;
		for(i=0x0f;i<=0x3f;i++)  //0x0f ---0x3f
		{
			uint8 reg_data;
			qmax981_read(i,&reg_data,1);
			SCI_TraceLow("[qmax981] 0x%x= %x",i,reg_data);
		}
	}
#endif

	return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   Handle timer event
// Dependence:
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL void qmax981_TimerCallback(uint32 param)
{
	//SCI_TraceLow("[qmax981] TimerCallback");
    MSensor_TimerHandler(param);
	SCI_ChangeTimer(s_qmax981_timer, qmax981_TimerCallback, TIMER_PERIOD_QMAX981);
	SCI_ActiveTimer(s_qmax981_timer);
	return ;
}

/******************************************************************************/
// Description:
// Dependence:
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 qmax981_GetEvent(MSENSOR_MOTION_TYPE_E * event_ptr)
{
	static MSENSOR_MOTION_TYPE_E s_last_motion_type = MSENSOR_MOTION_TYPE_NONE;
    MSENSOR_MOTION_TYPE_E motion_type = MSENSOR_MOTION_TYPE_NONE;

	uint8 int_st[3]={0},reg_0x1a=0,int_type= 0xff;
	uint32        count_tick = 0;
	static uint32 count_tick_old = 0;
	static uint32 count_tick_deshake = 0;
	MSENSOR_MOTION_DATA_T data_val ={0};
	int32  x = 0, y = 0, z = 0;
    static int32 x_old = 0;
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);

	if(g_qmaX981.chip_type == CHIP_ID_TYPE_QMA6100)
	{
		*event_ptr = MSENSOR_EVENT_NONE;
		return MSENSOR_SUCCESS;
	}

	qmax981_read(0x09,int_st,3);
	SCI_TraceLow("[qmax981]GetEvent 09h[%x] 0Ah[%x] 0Bh[%x] !\n",int_st[0],int_st[1],int_st[2]);

	if(int_st[0] & 0xF)
	{
#if defined(QMA7981_NO_MOTION)
		qmax981_read(0x1a,&reg_0x1a,1);
		reg_0x1a |= 0x80;			// enable nomotion
		//reg_0x1a &= 0xfe;			// disable anymotion
		qmax981_write(0x1a, reg_0x1a);
#endif
		//SCI_TraceLow("[qmax981]GetEvent any_motion !\n");

		int_type = 1;
	}
	else if(int_st[0] & 0x80)
	{
		qmax981_read(0x1a,&reg_0x1a,1);
		reg_0x1a &= 0x7f;
		qmax981_write(0x1a, reg_0x1a);		// disable nomotion
		int_type = 2;
		//SCI_TraceLow("[qmax981]GetEvent no motion!\n");
	}

	if(int_st[1] & 0x01)
	{
		int_type = 3;
		qmax981_read(0x1a,&reg_0x1a,1);
		reg_0x1a |= 0x80;			// enable nomotion
		//reg_0x1a &= 0xfe;			// disable anymotion
		qmax981_write(0x1a, reg_0x1a);
		//SCI_TraceLow("[qmax981]GetEvent significant motion!\n");
	}
	else if(int_st[1] & 0x40)
	{
		int_type = 4;
		//SCI_TraceLow("[qmax981]GetEvent significant step int!\n");
	}
	else if(int_st[1] & 0x08)
	{
		int_type = 5;
		//SCI_TraceLow("[qmax981]GetEvent step int!\n");
	}
#if defined(QMA7981_HAND_UP_DOWN)
	else if(int_st[1] & 0x02)
	{
		int_type = 6;
		//SCI_TraceLow("[qmax981]GetEvent hand raise!\n");
	}
	else if(int_st[1] & 0x04)
	{
		int_type = 7;
		//SCI_TraceLow("[qmax981]GetEvent hand down!\n");
	}
#endif

#if defined(QMA7981_DATA_READY)
	if(int_st[2] & 0x10)
	{
		int_type = 8;
		//SCI_TraceLow("[qmax981]GetEvent data ready !\n");
	}
#endif
    SCI_TraceLow("[qmax981]GetEvent int_type=%d\n", int_type);
    // get raw vector value
    qmax981_GetVector(&data_val);

    x = data_val.u.vector.x;
    y = data_val.u.vector.y;
    z = data_val.u.vector.z;

    //QMAX981_SHAKE_DETECT_TIME ms for detect shaking
    count_tick = SCI_GetTickCount();
#ifdef SENSOR_INT_MODE
    if(count_tick - count_tick_old > QMAX981_SHAKE_DETECT_TIME)
	{
	  x_old = 0;
	}
	//only x for detect shaking event
    if((x > QMAX981_LOW_THRESHOLD) && (x_old < -s_qmax981_sh_th))
    {
        motion_type = MSENSOR_MOTION_TYPE_RIGHT;
        x_old = 0;
    }
    else if((x < -QMAX981_LOW_THRESHOLD) && (x_old > s_qmax981_sh_th))
    {
        motion_type = MSENSOR_MOTION_TYPE_LEFT;
        x_old = 0;
    }

    //when x > s_qmax981_sh_th,start timer
    else if(abs(x) > s_qmax981_sh_th)
    {
		count_tick_old = count_tick;
		x_old = x;
    }
    //the time(abs(x_old) > s_qmax981_sh_th) for detect shaking,ignore orientation event
    else if(abs(x_old) > s_qmax981_sh_th)
        NULL;
#else
    if(x > s_qmax981_sh_th)
    {
        motion_type = MSENSOR_MOTION_TYPE_LEFT;
    }
    else if(x < -s_qmax981_sh_th)
    {
        motion_type = MSENSOR_MOTION_TYPE_RIGHT;
    }
#endif
    else
    {
		// quiescent status.
		// Forward or Backward side,
		// 20110212: will.jiang_cr227245. :
		//  treated specially to avoid stop ring by mistake
		if((x < QMAX981_THRESHOLD_0_5G && x > -QMAX981_THRESHOLD_0_5G) &&
		   (y < QMAX981_THRESHOLD_0_5G && y > -QMAX981_THRESHOLD_0_5G))
		{
			if(z < -QMAX981_THRESHOLD_0_9G)
				motion_type = MSENSOR_MOTION_TYPE_FORWRAD_SIDE;
			else if(z > QMAX981_THRESHOLD_0_9G)
				motion_type = MSENSOR_MOTION_TYPE_BACKWARD_SIDE;
		}
		// 20101013: Will.Jiang_cr205835 : update(reduce) the threshhold
		// left or right side
		if((z < QMAX981_THRESHOLD_0_8G && z > -QMAX981_THRESHOLD_0_8G) &&
		   (y < QMAX981_THRESHOLD_0_8G && y > -QMAX981_THRESHOLD_0_8G))
		{
			if(x < -s_qmax981_or_th)
				motion_type = MSENSOR_MOTION_TYPE_RIGHT_SIDE;
			else if(x > s_qmax981_or_th)
				motion_type = MSENSOR_MOTION_TYPE_LEFT_SIDE;
		}
		// up or down
		if((z < QMAX981_THRESHOLD_0_8G && z > -QMAX981_THRESHOLD_0_8G) &&
		   (x < QMAX981_THRESHOLD_0_8G && x > -QMAX981_THRESHOLD_0_8G))
		{
			if(y < -s_qmax981_or_th)
				motion_type = MSENSOR_MOTION_TYPE_UP_SIDE;
			else if(y > s_qmax981_or_th)
				motion_type = MSENSOR_MOTION_TYPE_DOWN_SIDE;
		}
    }

    //QMAX981_TRACE:"MSENSOR:: s_last_motion_type= 0x%08X, "
	SCI_TraceLow("[qmax981]GetEvent s_last_motion_type=0x%X,motion_type:0x%x",s_last_motion_type, motion_type);
	//The following message should not be send to client, otherwise there will
	//be too much message which could confuse the user!

	if(cfg_ptr || s_last_motion_type != motion_type)
    {   // always report at INT mode or New Event
		*event_ptr  = motion_type;
    }
	else
	{	// do NOT report the same event,only for timer mode
        *event_ptr = MSENSOR_EVENT_NONE;
	}
	s_last_motion_type = motion_type;

    //only one shaking event within QMAX981_DESHAKE_TIME ms
    //avoid double shake
	if(motion_type == MSENSOR_MOTION_TYPE_LEFT || motion_type == MSENSOR_MOTION_TYPE_RIGHT)
	{
	//    count_tick = SCI_GetTickCount();

		if(count_tick - count_tick_deshake < QMAX981_DESHAKE_TIME)
		{
		    *event_ptr = MSENSOR_EVENT_NONE;
		}
		count_tick_deshake = count_tick;
    }
	return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   qmax981_SetSensi
// Note:
/******************************************************************************/
LOCAL uint32 qmax981_SetSensi(MSENSOR_SENSI_E sensi_val)
{
	//SCI_TraceLow("[qmax981]SetSensi sensi_val:%d",sensi_val);

    switch(sensi_val)
    {
        case MSENSOR_SENSI_LOW:
        {
            s_qmax981_sh_th = QMAX981_THRESHOLD_1_5G;
            s_qmax981_or_th = QMAX981_THRESHOLD_1_0G;
            break;
        }
        case MSENSOR_SENSI_MID:
        {
            s_qmax981_sh_th = QMAX981_THRESHOLD_1_2G;
            s_qmax981_or_th = QMAX981_THRESHOLD_0_9G;
            break;
        }
        case MSENSOR_SENSI_HIGH:
        {
            s_qmax981_sh_th = QMAX981_THRESHOLD_1_0G;
            s_qmax981_or_th = QMAX981_THRESHOLD_0_7G;
            break;
        }
        default:
        {
            s_qmax981_sh_th = QMAX981_THRESHOLD_1_1G;
            s_qmax981_or_th = QMAX981_THRESHOLD_0_8G;
            break;
        }
    }

    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   qmax981_SetSensi
// Note:
/******************************************************************************/
LOCAL uint32 qmax981_ChangeTimer(uint32 timer_expire)
{
	SCI_ChangeTimer(s_qmax981_timer, qmax981_TimerCallback, timer_expire);
	SCI_ActiveTimer(s_qmax981_timer);
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   qmax981_GetCapability
// Note:
/******************************************************************************/
LOCAL uint32 qmax981_GetCapability(uint32 * Capability)
{
    *Capability = MSENSOR_MOTION_CAP_XY_AXIS | MSENSOR_MOTION_CAP_Z_AXIS | MSENSOR_MOTION_CAP_SHAKE | MSENSOR_MOTION_CAP_DATA;
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   qmax981_ioctl
// Note:
/******************************************************************************/
LOCAL uint32 qmax981_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr)
{
	uint32 ret = MSENSOR_SUCCESS;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
    //QMAX981_TRACE:"MSENSOR:: cmd = %d \r\n"
    switch(g_qmaX981.chip_type)
    {
		case CHIP_ID_TYPE_QMA6981:
			//SCI_TraceLow("[qmax981] [QMA6981] ioctl cmd:%d",cmd);
			break;
		case CHIP_ID_TYPE_QMA7981:
			SCI_TraceLow("[qmax981] [QMA7981] ioctl cmd:%d",cmd);
			break;
		case CHIP_ID_TYPE_QMA6100:
			//SCI_TraceLow("[qmax981] [QMA6100] ioctl cmd:%d",cmd);
			break;
		default:
			//SCI_TraceLow("[qmax981] [UNDEFINE] ioctl cmd:%d",cmd);
			break;
	}

    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:
        {
			ret = qmax981_GetVector((MSENSOR_MOTION_DATA_T *)data_ptr);
			break;
        }

        case MSENSOR_IOCTL_GET_EVENT:
        {
            if(cfg_ptr)
            {
                GPIO_DisableIntCtl(cfg_ptr->gpio_num);
            }
			ret = qmax981_GetEvent((MSENSOR_MOTION_TYPE_E *)data_ptr);
            if(cfg_ptr)
            {
				//resume InterruptSense from changed in GPIO_GSensorIntHandler()
				if(cfg_ptr->valid_level == GPIO_PROD_LOW_LEVEL)
				{
				    GPIO_SetInterruptSense(cfg_ptr->gpio_num, GPIO_INT_EDGES_FALLING);
				}
				if(cfg_ptr->valid_level == GPIO_PROD_HIGH_LEVEL)
				{
				    GPIO_SetInterruptSense(cfg_ptr->gpio_num, GPIO_INT_EDGES_RISING);
				}

				GPIO_EnableIntCtl(cfg_ptr->gpio_num);
			}
			break;
        }
        case MSENSOR_IOCTL_SET_SENSI:
        {
			ret = qmax981_SetSensi(*(MSENSOR_SENSI_E *)data_ptr);
			break;
        }
        case MSENSOR_IOCTL_SET_TIMER:
        {
			ret = qmax981_ChangeTimer(*(uint32 *)data_ptr);
			break;
        }
        case MSENSOR_IOCTL_GET_CAP:
        {
            ret = qmax981_GetCapability((uint32 *)data_ptr);
			break;
        }
		case MSENSOR_IOCTL_SET_CLEAN:
        {
			SCI_TraceLow("[qmax981]MSENSOR_IOCTL_SET_CLEAN");
			qmax981_last_steps = qmax981_read_steps;
			if(qmax981_read_steps > 30000)
			{
				uint8 data;
				SCI_TraceLow("[qmax981]MSENSOR_IOCTL_SET_CLEAN reg");
				ret = qmax981_read(QMAX981_STEP_PRECISION,&data,1);
				ret=qmax981_write(QMAX981_STEP_PRECISION,0x80);
				msensor_delayms(2);
				ret = qmax981_write(QMAX981_STEP_PRECISION,data);
				qmax981_last_steps = 0;
				qmax981_read_steps = 0;
			}
			break;
        }
        default:
            ret = MSENSOR_ERROR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif  // End of file

