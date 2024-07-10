
#ifndef LIB_vcHr02_HCI_vcHr02HCI_H_
#define LIB_vcHr02_HCI_vcHr02HCI_H_


#include "algo.h"
#include "vcSpo2Algo.h" 
#include "vcSportMotionAlgo.h"


/*****************************************************************************
*
*					         Register Address Declaration
*
******************************************************************************/
#if   1

/* Read Only */
#define  VCHR02ISVC31B 			0
#define  VCHR02ISVC32S 			0

#define  VCHR02ISVC51B 			0
#define  VCHR02ISVC52S 			1
#define  VCHR02ISVC52T 			0
#define  VCHR02ISVC9201 		0

#define VCREG0					        				(0x00)
#define VCREG0_SIZE			         					(1)
#define VCREG1											(0x01)
#define VCREG1_SIZE							 			(1)
#define VCREG2										    (0x02)
#define VCREG2_SIZE						 				(1)
#define VCREG3											(0x03)
#define VCREG3_SIZE										(1)
#define VCREG4											(0x04)
#define VCREG4_SIZE			   							(1)
#define VCREG5											(0x05)
#define VCREG5_SIZE				 						(1)
#define VCREG6				  							(0x06)
#define VCREG6_SIZE			   							(1)
#define VCREG7					    					(0x07)
#define VCREG7_SIZE				     					(2)
#define VCREG8											(0x09)
#define VCREG8_SIZE				     					(2)
#define VCREG9				      						(0x80)
#define VCREG9_SIZE			       					    (128)

/* Write Only */
#define VCREG10				      						(0x3B)
#define VCREG10_SIZE			       					(1)

/* Read or Write */
#define VCREG11					 						(0x10)
#define VCREG11_SIZE			   						(1)
#define VCREG12					    				    (0x11)
#define VCREG12_SIZE			     					(1)
#define VCREG13				      						(0x12)
#define VCREG13_SIZE			       					(1)
#define VCREG14			  								(0x13)
#define VCREG14_SIZE		     						(1)
#define VCREG15				          				    (0x14)
#define VCREG15_SIZE		             			    (2)
#define VCREG16             						    (0x16)
#define VCREG16_SIZE	               			        (1)
#define VCREG17    										(0x17)
#define VCREG17_SIZE	 								(1)
#define VCREG18		   									(0x18)
#define VCREG18_SIZE	 								(1)
#define VCREG19										    (0x19)
#define VCREG19_SIZE	 								(1)
#define VCREG20      									(0x1A)
#define VCREG20_SIZE	     							(1)
#define VCREG21      									(0x1B)
#define VCREG21_SIZE	     							(1)
#define VCREG22      								    (0x1C)
#define VCREG22_SIZE	     						    (1)
#define VCREG23                 			            (0x1D)
#define VCREG23_SIZE	     							(1)
#define VCREG24         								(0x1E)
#define VCREG24_SIZE	     							(1)
#define VCREG25         								(0x1F)
#define VCREG25_SIZE	     							(1)
#define VCREG26                      		            (0x20)
#define VCREG26_SIZE	   								(1)
#define VCREG27                 				        (0x21)
#define VCREG27_SIZE	   					 			(1)

/* Bit fields for VCREG1s */
#define VCREG1_CONFLICT			  		                (0x01 << 4)
#define VCREG1_INSAMPLE		    		                (0x01 << 3)
#define VCREG1_INTPS			    		            (0x01 << 4)
#define VCREG1_INTOV			   			            (0x01 << 3)
#define VCREG1_INTFIFO	      		                    (0x01 << 2)
#define VCREG1_INTENV			    		            (0x01 << 1)
#define VCREG1_INTPPG			    		            (0x01 << 0)

//Threshold Settings
#if VCHR02ISVC31B 
	#define	VCPS_TH   									8
#elif VCHR02ISVC32S 
	#define	VCPS_TH   									5
#elif VCHR02ISVC51B  
	#define	VCPS_TH   									5
#elif VCHR02ISVC52S  
	#define	VCPS_TH   									7
#elif VCHR02ISVC52T 
	#define	VCPS_TH   									4
#elif VCHR02ISVC9201  
	#define	VCPS_TH   									10
#endif

#define	VCENV_TH										9  // Causes of ENV interruption
#define	VCPPG_TH										10 // Causes of PPG interruption PPG_TH = 300 
#define VCADJUST_INCREASE						        22 // 1.4 << 4 = 22.4//1.4f 
#define VCADJUST_DECREASE					            11 // 0.7 << 4 = 11.2//0.7f
#define	VCADJUST_STEP_MAX						        32
#define	VCADJUST_STEP_MIN						        1

#define VCUNWEAR_CNT						            3
#define VCISWEAR_CNT							        1

#define SLOTCHNU  										3

#define VCWEAR_STATUS_DETECTION_OFF	                    0
#define VCWEAR_STATUS_DETECTION_ON	                    1
#define VCWEAR_STATUS_DETECTION_MODE VCWEAR_STATUS_DETECTION_ON

#define VCWEAR_READ_FIFO_BY_TIME	                    0
#define VCWEAR_READ_FIFO_BY_INT	                        1
#define VCWEAR_READ_FIFO_MODE VCWEAR_READ_FIFO_BY_INT

#define SportMotionEn                                   0
#define BiometricEn                                     0
#define GsensorEn                                     0///1
/*****************************************************************************
*
*					           Data Type Declaration
*
******************************************************************************/
typedef enum
{
  VCWORK_MODE_HRWORK        = 0,
  VCWORK_MODE_CROSSTALKTEST = 1,
  VCWORK_MODE_SPO2WORK      = 2,
  VCWORK_MODE_GSENSORWORK   = 3,
  VCWORK_MODE_TEMPWORK      = 4, 
  VCWORK_MODE_NOISEWORK     = 5,
  VCWORK_MODE_LPDETECTION   = 6,//LowpowerDeteciton
}vcHr02Mode_t;

typedef enum
{
	VCHR02ADJUSTDIRECTION_NULL = 0,	
	VCHR02ADJUSTDIRECTION_UP   = 1,
	VCHR02ADJUSTDIRECTION_DOWN = 2,
}vcHr02AdjustDirection;

typedef struct
{
	vcHr02AdjustDirection direction;
	vcHr02AdjustDirection directionLast;
	uint16 step;
}vcHr02AdjustInfo_t;

typedef enum
{
	VCHR02WEARST_UNWEAR	= 0,
	VCHR02WEARST_ISWEAR	= 1,
}vcHr02WearStatus_e;

typedef enum
{
	VCHR02RET_ISOK				  = 0,
	VCHR02RET_ISWEARTOUNWEAR	  = 1,
	VCHR02RET_UNWEARTOISWEAR	  = 2,
	VCHR02RET_ISERR				  = 4,
	VCHR02RET_ISCONFLICT          = 8,
	VCHR02RET_ISINSAMPLE          = 16,
	VCHR02RET_SOLTISOVERLOAD      = 32,
	VCHR02RET_PSISOVERLOAD        = 64,
	VCHR02RET_ENVCURRENTISLARGE   = 128,
	VCHR02RET_PSENVCURRENTISLARGE = 256,
	VCHR02RET_PPGCANNOTADJUSTABLE = 512,
	VCHR02RET_FIFOISOVERFLOW      = 1024,
	VCHR02RET_PDRESISMAX          = 2048,
	VCHR02RET_PDRESCANNOTRELEASE  = 4096,
}vcHr02Ret_t;

typedef struct
{
	uint8 maxLedCur;
    uint8 pdResValue[3];
    uint8 currentValue[3];
	uint8 psValue;	    //PS Sample value.
	uint8 preValue[2];	//Environment Sample value.
	uint8 envValue[3];	//Environment Sample value.
	uint16 ppgValue[128];	//PPG sample value.
}vcHr02SampleData_t;

typedef struct
{
	BOOLEAN isRunning;
	BOOLEAN vcFifoReadFlag;
	BOOLEAN vcPsFlag;
	BOOLEAN oscCheckFinishFlag;
	
	uint8 regConfig[17];
	uint8 unWearCnt;
	uint8 isWearCnt;
	uint8 vcHr02Status;
	uint8 intReason;
	vcHr02Mode_t workMode;
	vcHr02WearStatus_e wearStatus;
	
	uint16 vcSampleRate;
	uint16 mcuOscValue;
	uint16 mcuSclRate;
	vcHr02AdjustInfo_t adjustInfo[2];
	vcHr02SampleData_t sampleData;
}vcHr02_t;

/*****************************************************************************
*
*					             Function Declaration
*
******************************************************************************/

/*
 * @brief
 *   Get sample datas from vcHr02.
 *
 **/
vcHr02Ret_t  vcHr02GetSampleValues(vcHr02_t *pvcHr02,uint8 *DataLen);


/*
 * @brief
 *   Manual start sample of vcHr02.
 *
 **/
vcHr02Ret_t vcHr02StartSample(vcHr02_t *pvcHr02);


/*
 * @brief
 *   Manual stop sample of vcHr02.
 *
 **/
vcHr02Ret_t vcHr02StopSample(vcHr02_t *pvcHr02);


/*
 * @brief
 *   Manual SoftReset the vcHr02
 *
 **/
vcHr02Ret_t vcHr02SoftReset(vcHr02_t *pvcHr02);


/*
 * @brief
 *  
 *   
 **/
vcHr02Ret_t vcHr02TimeReadValues(vcHr02_t *pvcHr02,uint8 *DataLen);



/*****************************************************************************
*
*					         External Function Declaration
*
******************************************************************************/
/*
 * @brief
 *  
 *   
 **/
extern void vcHr02Init(vcHr02_t *pVcHr02,uint8 vcHr02WorkMode);

/*
 * @brief
 *  
 *   
 **/
extern void vcHr02_process(AlgoSportMode vcSportMode);


/*
 * @brief
 *  
 *   
 **/
extern void vcHr02PhyInit(void);


/*
 * @brief
 *  
 *   
 **/
extern void vcHr02IRQHandler(void);


/*
 * @brief
 *  
 *   
 **/
extern vcHr02Ret_t VCHR02_ReadRegisters(uint8 startAddress, uint8 *pRegisters, uint8 len);


/*
 * @brief
 *  
 *   
 **/
extern vcHr02Ret_t VCHR02_WriteRegisters(uint8 startAddress, uint8 *pRegisters, uint8 len);


/*
 * @brief
 *   Read Registers from vcHr02. This function needs to be implemented
 *   according to the MCU.
 **/
extern uint32 vcHr02GetRtcCountFromMCU(void);


/*
 * @brief
 *   Read Registers from vcHr02. This function needs to be implemented
 *   according to the MCU.
 **/
extern void green_led_off_state_gsensor_abs_sum_diff_func(int32 x_axis_value, int32 y_axis_value, int32 z_axis_value);


typedef vcHr02Ret_t (* vcHr02_read_cb)(uint8 startAddress, uint8 *pRegisters, uint8 len);
typedef vcHr02Ret_t (* vcHr02_write_cb)(uint8 startAddress, uint8 *pRegisters, uint8 len);
typedef uint32 (* vc_gettime_cb)(void);

#endif /* LIB_vcHr02_HCI_vcHr02HCI_H_ */
#endif
