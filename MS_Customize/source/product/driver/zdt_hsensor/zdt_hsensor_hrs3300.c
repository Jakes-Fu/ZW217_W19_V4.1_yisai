/******************************************************************************
 ** File Name:      msensor_mir3da.c                                          *
 ** Author:          Yc.Wang                                                *
 ** DATE:            2015.08.31                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2015.08.31   Yc.Wang            Create.                               *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"
#include "Power_cfg.h"
#include "zdt_hsensor.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif

//#define RTT_PRINT
//#define RAW_DATA_PRINTF
//#define FLT_DATA_PRINT
//#define HRS_DEBUG

#define BP_CUSTDOWN_ALG_LIB

#ifndef BOOLEAN
#define BOOLEAN unsigned char
#endif

typedef enum {
	MSG_ALG_NOT_OPEN = 0x01,
	MSG_NO_TOUCH = 0x02,
	MSG_PPG_LEN_TOO_SHORT = 0x03,
	MSG_HR_READY = 0x04,
	MSG_ALG_TIMEOUT = 0x05,
	MSG_SETTLE = 0x06
} hrs3300_msg_code_t;

typedef enum {
	MSG_BP_ALG_NOT_OPEN = 0x01,
	MSG_BP_NO_TOUCH = 0x02,
	MSG_BP_PPG_LEN_TOO_SHORT = 0x03,
	MSG_BP_READY = 0x04,
	MSG_BP_ALG_TIMEOUT = 0x05,
	MSG_BP_SETTLE = 0x06
} hrs3300_bp_msg_code_t;

typedef struct {
  hrs3300_msg_code_t alg_status;
  uint32           data_cnt;
  uint8            hr_result;
  uint8            hr_result_qual; // ericy add20170111
	BOOLEAN               object_flg;
} hrs3300_results_t;

typedef struct {
  hrs3300_bp_msg_code_t bp_alg_status;
  uint8            sbp;
  uint8            dbp; 
	uint32           data_cnt;
  uint8            hr_result; //20170614 ericy
	BOOLEAN               object_flg;
} hrs3300_bp_results_t;

BOOLEAN Hrs3300_chip_init(void);
void Hrs3300_chip_enable(void);
void Hrs3300_chip_disable(void);
uint16 Hrs3300_read_hrs(void);
void Hrs3300_set_exinf(uint8 age, uint8 height, uint8 weight , uint8 gender , uint8 ref_sbp , uint8 ref_dbp);
extern BOOLEAN Hrs3300_write_reg(uint8 addr, uint8 data);
extern uint8 Hrs3300_read_reg(uint8 addr);
hrs3300_results_t Hrs3300_alg_get_results(void);
hrs3300_bp_results_t Hrs3300_alg_get_bp_results(void);
extern BOOLEAN Hrs3300_alg_open(void);
extern void Hrs3300_bp_alg_open(void);
extern BOOLEAN Hrs3300_alg_send_data(int16 new_raw_data, int16 als_raw_data, int16 gsen_data_x, int16 gsen_data_y, int16 gsen_data_z, uint16 timer_time);
extern BOOLEAN Hrs3300_bp_alg_send_data(int16 new_raw_data);
extern void Hrs3300_alg_close(void);

const uint8 init_register_array[][2] = {
{0x01, 0xd0},  //11010000  bit[7]=1,HRS enable;bit[6:4]=101,wait time=50ms,bit[3]=0,LED DRIVE=22 mA
//{0x01, 0xf0},   //11010000  bit[7]=1,HRS enable;bit[6:4]=101,wait time=50ms,bit[3]=0,LED DRIVE=22 mA v13.05
{0x0c, 0x4e},  //00001110  bit[6]=0,LED DRIVE=22mA;bit[5]=0,sleep mode;p_pulse=1110,duty=50% 
{0x16, 0x78},  //01111000  bits[7:4]=0111,HRS 15bits
{0x17, 0x0d},  //00001101  bits[7:5]=011,HRS gain 16*;bit[1]=0,HRS data=AD0 
{0x02, 0x80},
{0x03, 0x00},
{0x04, 0x00},
{0x05, 0x00},
{0x06, 0x00},
{0x07, 0x00},
{0x08, 0x74},
{0x09, 0x00},
{0x0a, 0x08},
{0x0b, 0x00},
{0x0c, 0x6e},
{0x0d, 0x02},
{0x0e, 0x07},
{0x0f, 0x0f},
};
#define INIT_ARRAY_SIZE (sizeof(init_register_array)/sizeof(init_register_array[0]))

// hrs3300 customer config
const uint8  hrs3300_bp_timeout_grade = 0;  // max 15
const uint8  hrs3300_agc_init_stage = 0x04;  // init AGC state  
const uint8  hrs3300_bp_power_grade = 0;
const uint8  hrs3300_accurate_first_shot = 0;
const uint8  hrs3300_up_factor = 3;
const uint8  hrs3300_up_shift = 2;
const uint16 hrs3300_AMP_LTH = 120;
const uint16 hrs3300_hr_AMP_LTH = 150;
const uint16 hrs3300_hr_PVAR_LTH = 10;
// hrs3300 customer config end

//20161117 added by ericy for "low power in no_touch state"
static BOOLEAN hrs3300_power_up_flg = 0 ;
uint8 reg_0x7f ;
uint8 reg_0x80 ;
uint8 reg_0x81 ;
uint8 reg_0x82 ;
//20161117 added by ericy for "low power in no_touch state"

#if 1
#define HRS_I2C_ERROR                0
#define HRS_I2C_SUCCESS              1

//0x44
#define HRS3300_I2C_WRITE_ADDR       0x88
#define HRS3300_I2C_READ_ADDR         0x89
static int32 HRS3300_I2C_Handler = -1;

static uint32 _Get_HRS3300_I2C_Status(void);
/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: HRS3300_I2C_Open
// Dependence: 
// Note:
/******************************************************************************/
static I2C_DEV HRS3300_I2C_cfg =
{
    1,          
    100000,                   // freq is standard for HRS3300
    0,                          // bus
    HRS3300_I2C_WRITE_ADDR,  // slave_addr
    1,                          // reg_addr_num
    1,                          // check_ack
    1                           // no_stop
};

static uint32 HRS3300_I2C_Open(void)
{
    if(HRS3300_I2C_Handler == -1) 
    {
        if((HRS3300_I2C_Handler = I2C_HAL_Open(&HRS3300_I2C_cfg)) == -1)
        {
            ZDT_HSENSOR_TRACE("HRS3300_I2C_Open Err");
            return HRS_I2C_ERROR;    
        } 
        else
            return HRS_I2C_SUCCESS;
    }
    else
    {
        return HRS_I2C_SUCCESS;
        
    }
}

/******************************************************************************/
// Description: HRS3300_I2C_Close
// Dependence: 
// Note:
/******************************************************************************/

static uint32 HRS3300_I2C_Close(void)
{
    if(_Get_HRS3300_I2C_Status() == HRS_I2C_SUCCESS)
	{
 		I2C_HAL_Close(HRS3300_I2C_Handler);
		HRS3300_I2C_Handler = -1;
	}	
    return HRS_I2C_SUCCESS;
}
/******************************************************************************/
// Description: HRS3300_I2C_Write
// Dependence: 
// Note:
/******************************************************************************/
static void HRS3300_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_HRS3300_I2C_Status() == HRS_I2C_SUCCESS)
        I2C_HAL_Write(HRS3300_I2C_Handler, reg, addr, cnt);
}


/******************************************************************************/
// Description: HRS3300_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
static void HRS3300_I2C_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_HRS3300_I2C_Status() == HRS_I2C_SUCCESS)
        I2C_HAL_Read(HRS3300_I2C_Handler,reg, addr, cnt);
}


/******************************************************************************/
// Description: _Get_HRS3300_I2C_Status
// Dependence:  It can get the status from i2c
// Note:
/******************************************************************************/
static uint32 _Get_HRS3300_I2C_Status(void)
{
    if(HRS3300_I2C_Handler==-1)
    {
        return HRS_I2C_ERROR;
    }
    else
    {
        return HRS_I2C_SUCCESS;
    }
}

BOOLEAN Hrs3300_write_reg(uint8 addr, uint8 data) 
{
    uint32 ret = 0;
    if(_Get_HRS3300_I2C_Status() == HRS_I2C_SUCCESS)
    {
        ret = I2C_HAL_Write(HRS3300_I2C_Handler, &addr, &data, 1);
        if(ret == 0)
        {
            ZDT_HSENSOR_TRACE("Hrs3300_write_reg I2C_HAL_Write Err");
        }
    }
    else
    {
        ZDT_HSENSOR_TRACE("Hrs3300_write_reg Err");
    }
	return 0;  	
}

uint8 Hrs3300_read_reg(uint8 addr) 
{
    uint32 ret = 0;
    uint8 data_buf = 0;	
    if(_Get_HRS3300_I2C_Status() == HRS_I2C_SUCCESS)
    {
        ret = I2C_HAL_Read(HRS3300_I2C_Handler,&addr, &data_buf, 1);
        if(ret == 0)
        {
            ZDT_HSENSOR_TRACE("Hrs3300_read_reg I2C_HAL_Read Err");
        }
    }
    else
    {
        ZDT_HSENSOR_TRACE("Hrs3300_read_reg Err");
    }
    return data_buf;
}
#endif

uint16 Hrs3300_read_hrs(void)
{
    uint8  databuf[3];
    uint16 data;

    databuf[0] = Hrs3300_read_reg(0x09);	// addr09, bit
    databuf[1] = Hrs3300_read_reg(0x0a);	// addr0a, bit
    databuf[2] = Hrs3300_read_reg(0x0f);	// addr0f, bit

    data = ((databuf[0]<<8)|((databuf[1]&0x0F)<<4)|(databuf[2]&0x0F));

    //ZDT_HSENSOR_TRACE("%s (%d)	: ==huangwenjun==Hrs3300_read_hrs== hrs data = 0x%x\n", __func__, __LINE__, data);  // add by hwj

    return data;
}

uint16 Hrs3300_read_als(void)
{
	uint8  databuf[3];
	uint16 data;

	databuf[0] = Hrs3300_read_reg(0x08);	// addr09, bit [10:3]
  databuf[1] = Hrs3300_read_reg(0x0d);	// addr0a, bit [17:11]
  databuf[2] = Hrs3300_read_reg(0x0e);	// addr0f, bit [2:0]
	
	data = ((databuf[0]<<3)|((databuf[1]&0x3F)<<11)|(databuf[2]&0x07));
	
	if (data > 32767) data = 32767;  // prevent overflow of other function

	return data;
}


BOOLEAN Hrs3300_chip_init(void)
{
	int i =0 ;
	uint8 id =0;
	ZDT_HSENSOR_TRACE(" hrs3300 init \n");
      
	for(i = 0; i < INIT_ARRAY_SIZE;i++)
	{
		if ( Hrs3300_write_reg( init_register_array[i][0],
		                init_register_array[i][1]) != 0 )
		{
                return FALSE;
		}
	}	

	//20161117 added by ericy for "low power in no_touch state"		
	if(hrs3300_power_up_flg == 0){
		reg_0x7f=Hrs3300_read_reg(0x7f) ;
		reg_0x80=Hrs3300_read_reg(0x80) ;
		reg_0x81=Hrs3300_read_reg(0x81) ;
		reg_0x82=Hrs3300_read_reg(0x82) ;		
		hrs3300_power_up_flg =  1; 
	}
	//20161117 added by ericy for "low power in no_touch state"

	id = Hrs3300_read_reg(0x00);
	ZDT_HSENSOR_TRACE("<<< hrs3300 init done id = 0x%x \r\n", id); // 0x21	
	ZDT_HSENSOR_TRACE("%s (%d)	: ==huangwenjun==Hrs3300_chip_init== init done id = 0x%x\n", __func__, __LINE__, id);  // add by hwj

	return TRUE;
}

void Hrs3300_chip_enable()
{	
	Hrs3300_write_reg( 0x16, 0x78 );
	Hrs3300_write_reg( 0x01, 0xd0 );	
	Hrs3300_write_reg( 0x0c, 0x2e );
	ZDT_HSENSOR_TRACE("%s (%d)	: ==huangwenjun==Hrs3300_chip_enable== END\n", __func__, __LINE__);  // add by hwj
	return ;	
}

void Hrs3300_chip_disable()
{
	Hrs3300_write_reg( 0x01, 0x08 );
	Hrs3300_write_reg( 0x02, 0x80 );
	Hrs3300_write_reg( 0x0c, 0x4e );
	
	Hrs3300_write_reg( 0x16, 0x88 );
	
	Hrs3300_write_reg( 0x0c, 0x22 );
	Hrs3300_write_reg( 0x01, 0xf0 );
	Hrs3300_write_reg( 0x0c, 0x02 );

	Hrs3300_write_reg( 0x0c, 0x22 );
	Hrs3300_write_reg( 0x01, 0xf0 );
	Hrs3300_write_reg( 0x0c, 0x02 );
	
	Hrs3300_write_reg( 0x0c, 0x22 );
	Hrs3300_write_reg( 0x01, 0xf0 );
	Hrs3300_write_reg( 0x0c, 0x02 );
	
	Hrs3300_write_reg( 0x0c, 0x22 );
	Hrs3300_write_reg( 0x01, 0xf0 );
	Hrs3300_write_reg( 0x0c, 0x02 );
	ZDT_HSENSOR_TRACE("%s (%d)	: ==huangwenjun==Hrs3300_chip_disable== END\n", __func__, __LINE__);  // add by hwj
	return ;	
}

unsigned int hrs3300_hr_data = 0;
unsigned int hrs3300_up_data = 0;
unsigned int hrs3300_down_data = 0;

void heart_rate_meas_timeout_handler(void * p_context)
{
    #if 1
   // uint32        err_code;
   // uint16        heart_rate;
	  uint16 hrm_raw_data;
	uint16 als_raw_data;
//	  uint8 gsen_data;	
	  hrs3300_results_t alg_results;
#ifdef BP_CUSTDOWN_ALG_LIB		
	hrs3300_bp_results_t	bp_alg_results ;	
#endif	
	  static uint16 timer_index =0;
#ifdef GSENSER_DATA
	  AxesRaw_t gsen_buf;
#endif

//    UNUSED_PARAMETER(p_context);
#ifdef GSENSER_DATA
	  LIS3DH_GetAccAxesRaw(&gsen_buf);
#endif
	  hrm_raw_data = Hrs3300_read_hrs();
	  als_raw_data = Hrs3300_read_als();  // 20170430
#ifdef GSENSER_DATA
        Hrs3300_alg_send_data(hrm_raw_data, als_raw_data, gsen_buf.AXIS_X, gsen_buf.AXIS_Y, gsen_buf.AXIS_Z, 0); 
#else
	  Hrs3300_alg_send_data(hrm_raw_data, als_raw_data, 0, 0, 0,0);
#endif
#if 0
	   alg_results = Hrs3300_alg_get_results();	
	   SEGGER_RTT_printf(0,"%d %d %d %d %d %d %d\n", hrm_raw_data, als_raw_data,gsen_buf.AXIS_X,gsen_buf.AXIS_Y,gsen_buf.AXIS_Z,alg_results.hr_result,alg_results.alg_status);
#endif

    timer_index ++;
    if (timer_index >= 25)  {    // get result per second
			  timer_index =0;
		alg_results = Hrs3300_alg_get_results();

		if (alg_results.alg_status == MSG_NO_TOUCH)
		{
			//opr_display_hr(0);    // customer can print no touch information here
			hrs3300_hr_data = 0;
		}
		else if (alg_results.alg_status == MSG_PPG_LEN_TOO_SHORT)
		{
			//opr_display_wait( ((alg_results.data_cnt/100)%3)+1);  // customer can print waiting information here
			hrs3300_hr_data = 0;
		}
		else
		{
#ifdef BP_CUSTDOWN_ALG_LIB					
        bp_alg_results = Hrs3300_alg_get_bp_results();  
        if (bp_alg_results.sbp!= 0){
			hrs3300_up_data = bp_alg_results.sbp;
			hrs3300_down_data = bp_alg_results.dbp;
           //opr_display_bp(bp_alg_results.sbp, bp_alg_results.dbp);
        }
#endif				


			hrs3300_hr_data = alg_results.hr_result;
			//opr_display_hr(alg_results.hr_result);  // customer can print real heart rate here
		}
	}
    #endif
}

void blood_presure_meas_timeout_handler(void * p_context)
{
#ifdef  BP_CUSTDOWN_ALG_LIB
	uint16 hrm_raw_data;
	//uint8 gsen_data;	
	hrs3300_bp_results_t bp_alg_results;
	static uint16 timer_index =0;

//    UNUSED_PARAMETER(p_context);
	  hrm_raw_data = Hrs3300_read_hrs();
//	  SEGGER_RTT_printf(0,"------------- %d\n", hrm_raw_data);

	  //Hrs3300_bp_alg_send_data(hrm_raw_data);
//		SEGGER_RTT_printf(0,"status is %d\n", alg_status);
	  timer_index ++;
	if (timer_index >= 50)  {    // get result per second
		timer_index =0;
		bp_alg_results = Hrs3300_alg_get_bp_results();
			
		if (bp_alg_results.object_flg == 1){ 
			// ¹ÌÌå¼ì²â´¦¬q
		}
		if (bp_alg_results.bp_alg_status == MSG_BP_NO_TOUCH)
		{
			//opr_display_hr(0);    // customer can print no touch information here
		}
	        else if (bp_alg_results.bp_alg_status == MSG_BP_PPG_LEN_TOO_SHORT)
		{
			//opr_display_hr(0);  // customer can print waiting information here
		}	          
		else if (bp_alg_results.bp_alg_status == MSG_BP_ALG_TIMEOUT)
		{
			if (bp_alg_results.sbp != 0)
			{
			}
			else
			{
				//bp_alg_results.sbp = 120 + rand()%8;
				//bp_alg_results.dbp = 68 + rand()%8;
			}
			//opr_display_bp(bp_alg_results.sbp, bp_alg_results.dbp);  // customer can print real heart rate here
		}
		else if (bp_alg_results.bp_alg_status == MSG_BP_READY)
		{
			//opr_display_bp(bp_alg_results.sbp, bp_alg_results.dbp);  // customer can print real heart rate here
		}


	}
#endif
}

int hrs3300_init(void)
{
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    ZDT_HSENSOR_TRACE("hrs3300_init");
    HRS3300_I2C_Open();
    Hrs3300_chip_init();
    Hrs3300_chip_enable();
    Hrs3300_alg_open();
    return 0;
}
/*----------------------------------------------------------------------------*/
void  hrs3300_exit(void)
{
    HRS3300_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);
    #endif
}

BOOLEAN hrs3300_CheckID(void)
{
    BOOLEAN res = FALSE;
    uint8 id = 0;
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    HRS3300_I2C_Open();
    id = Hrs3300_read_reg(0x00);
    ZDT_HSENSOR_TRACE("hrs3300_CheckID id=0x%x",id);
    if(id == 0x21)
    {
        res = TRUE;
    }
    HRS3300_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);
    #endif
    return res;
}

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif  // End of file


