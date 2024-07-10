/***************************************************************************************
** File Name:      a_fsm0611.c                                           		    	       *
** DATE:           11/07/2006                                                		   *
** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         		   *
** Description:    this file defines AF interfaces,								       *
****************************************************************************************

****************************************************************************************
**                         Edit History                                       		   *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                               				   *
** 11/07/2006     	       Create.													   *
****************************************************************************************/


/**------------------------------------------------------------------------------------*
 **                         Dependencies           			                           *
 **------------------------------------------------------------------------------------*/
#include "isp_service.h"
#include "isp_drv_sc8800h.h"
#include "gpio_ext_drv.h"

#include "gpio_prod_api.h"
/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                               		       *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define GPIO_MASK   0x3c

#define AF_INCREASE_THRESHOLD 13 // the average length of the FV curve increase slope, used to estimate the reaching of the FV curve peak
#define AF_MAX_STEP_COUNT	  20 // total step count for the whole focus range

#define MOTOR_IS_OPEND	      1
#define MOTOR_IS_CLOSED       0
/**------------------------------------------------------------------------------------*
 **                         Data Type           			                           *
 **------------------------------------------------------------------------------------*/
typedef enum
{
	MOTOR_PHASE_0 = 0,
	MOTOR_PHASE_1    ,
	MOTOR_PHASE_2    ,
    MOTOR_PHASE_3    ,
    MOTOR_PHASE_4
}AF_MOTOR_COMMAND_E;


LOCAL AF_MOTOR_COMMAND_E s_motor_control_current_state = MOTOR_PHASE_1;
LOCAL uint32 s_is_opend = 0;

LOCAL void motor_delay(uint32 delay_ms)
{
	SCI_Sleep(delay_ms);
}
/*******************************************************************************/
// Description:Send command that is used to control motor 
// Global resource dependence:
// Author: Jianping.wang
// Note:
//		input: 
//			cmd - control command
//		output:
//			none
//		return
//			none
/********************************************************************************/	
LOCAL void  SendMotorCommand(AF_MOTOR_COMMAND_E cmd)
{
	switch(cmd)
	{	
		case MOTOR_PHASE_0:
			GPIO_MOTOR_SetPortValue(GPIO_EXT_DEV_2_NUM,GPIO_MASK, 0x0000);
			break;
		case MOTOR_PHASE_1:
			GPIO_MOTOR_SetPortValue(GPIO_EXT_DEV_2_NUM,GPIO_MASK, 0x24);
			break;
		case MOTOR_PHASE_2:
			GPIO_MOTOR_SetPortValue(GPIO_EXT_DEV_2_NUM,GPIO_MASK, 0x14);
			break;
		case MOTOR_PHASE_3:
			GPIO_MOTOR_SetPortValue(GPIO_EXT_DEV_2_NUM,GPIO_MASK, 0x18);
			break;
		case MOTOR_PHASE_4:
			GPIO_MOTOR_SetPortValue(GPIO_EXT_DEV_2_NUM,GPIO_MASK, 0x28);
			break;
		default:
			break;
	}
}
/*******************************************************************************/
// Description:Open the motor 
// Global resource dependence:
// Author: Jianping.wang
// Note:
//		input: 
//			none
//		output:
//			none
//		return
//			none
/********************************************************************************/	
LOCAL void OpenMotor(void)
{	
	SendMotorCommand(s_motor_control_current_state);
	s_is_opend = MOTOR_IS_OPEND;
}
/*******************************************************************************/
// Description:Close the motor 
// Global resource dependence:
// Author: Jianping.wang
// Note:
//		input: 
//			none
//		output:
//			none
//		return
//			none
/********************************************************************************/	
LOCAL void CloseMotor(void)
{
	SendMotorCommand(MOTOR_PHASE_0);
	s_is_opend = MOTOR_IS_CLOSED;
}
/*******************************************************************************/
// Description:move the motor  
// Global resource dependence:
// Author: Jianping.wang
// Note:
//		input: 
//			step_number - move step 
//		output:
//			none
//		return
//			none
/********************************************************************************/	
LOCAL void RunMotorReserve (uint8 step_number)
{
	uint8 a;

	a = step_number;
	while(a != 0)
	{
		a--;		
		switch(s_motor_control_current_state)
		{
			case MOTOR_PHASE_1:
				s_motor_control_current_state = MOTOR_PHASE_4;
				motor_delay(1);
				SendMotorCommand(s_motor_control_current_state);			
				break;
			case MOTOR_PHASE_2:
				s_motor_control_current_state = MOTOR_PHASE_1;
				motor_delay(1);
				SendMotorCommand(s_motor_control_current_state);			
				break;
			case MOTOR_PHASE_3:
				s_motor_control_current_state = MOTOR_PHASE_2;
				motor_delay(1);
				SendMotorCommand(s_motor_control_current_state);			
				break;
			case MOTOR_PHASE_4:
				s_motor_control_current_state = MOTOR_PHASE_3;
				motor_delay(1);
				SendMotorCommand(s_motor_control_current_state);			
				break;
		   	default:
				break;
		}
		
	}
}
/*******************************************************************************/
// Description:move the motor positively
// Global resource dependence:
// Author: Jianping.wang
// Note:
//		input: 
//			step_number - move step
//		output:
//			none
//		return
//			none
/********************************************************************************/	
void RunMotorPositive (uint8 step_number)
{
	uint8 a;

	a = step_number;
	while(a != 0)
	{
		a--;
		switch(s_motor_control_current_state)
		{
			case MOTOR_PHASE_1:
				s_motor_control_current_state = MOTOR_PHASE_2;
				motor_delay(1);
				SendMotorCommand(s_motor_control_current_state);
				break;
			case MOTOR_PHASE_2:
				s_motor_control_current_state = MOTOR_PHASE_3;
				motor_delay(1);
				SendMotorCommand(s_motor_control_current_state);
				break;
			case MOTOR_PHASE_3:
				s_motor_control_current_state = MOTOR_PHASE_4;
				motor_delay(1);
				SendMotorCommand(s_motor_control_current_state);
				break;
			case MOTOR_PHASE_4:
				s_motor_control_current_state = MOTOR_PHASE_1;
				motor_delay(1);
				SendMotorCommand(s_motor_control_current_state);
				break;
		   	default:
				break;
		}
	}
}
/*******************************************************************************/
// Description: calculate the average focus value of the latest 3 frames, 
//			    to prevent random noise affect
// Global resource dependence:
// Author: Xiaoguang.Feng
// Note:
//		input: 
//			FV1 - 
//			FV2 -
//			FV3 -
//			CurrentFV - current frame focus value
//		output:
//			none
//		return
//			average focus value
/********************************************************************************/	
LOCAL uint32 Average_FV(uint32* FV1, uint32* FV2, uint32* FV3, uint32 CurrentFV)
{
	if (*FV1==0)
	{
		*FV1=CurrentFV+1; // plus one: to prevent zero focus value
		return *FV1;
	}
	else if (*FV2==0)
	{
		*FV2=CurrentFV+1; // plus one: to prevent zero focus value
		 return (*FV1+*FV2)/2;
	}
	else if (*FV3==0)
	{
		*FV3=CurrentFV+1; // plus one: to prevent zero focus value
		return (*FV1+*FV2+*FV3)/3;
	}
	else
	{
		*FV1=*FV2;
		*FV2=*FV3;
		*FV3=CurrentFV+1; // plus one: to prevent zero focus value
		 return (*FV1+*FV2+*FV3)/3;
	}			
}
/*******************************************************************************/
// Description: Single mode auto focus searching function
// Global resource dependence:
// Author: Xiaoguang.Feng
// Note:
//		input: 
//			iCurrentFV - the focus value of current frame
//		output:
//			none
//		return
//			0
/********************************************************************************/	
uint32 SingleModeAutoFocus(uint32 iCurrentFV)
{
	// static variables
	static uint8 g_iStepCount=0; // count of the searching steps
	static uint32 g_FV1=0; // last-last frame's focus value
	static uint32 g_FV2=0; // last frame's focus value
	static uint32 g_FV3=0; // current frame's focus value
	static uint32 g_MaxFocusValue=0; // maximum focus value
	static int32 g_iFocusedStep=0; // the index of steps corresponding to the maximum focus value
	static uint8 g_iIncreaseStepCount=0; // number of increasing slope steps in the FV curve

	// local variables
	uint32 iFocusValue=0;
	//int i=0;

	// number of searched steps increases
	g_iStepCount++;
		
	if (g_iStepCount==1) // first run, open the motor
	{
		OpenMotor(); // to open the motor
	}
	
	// calculate the average focus value of the latest 3 frames, 
	// to prevent random noise affect
	iFocusValue=Average_FV(&g_FV1, &g_FV2, &g_FV3, iCurrentFV);
	
	// the increase_step_count is used to estimate the FV curve peak	
	if (g_FV2<g_FV3)
	{
		g_iIncreaseStepCount++;
	}
	else
	{
		g_iIncreaseStepCount=0;
	}
		
	// if we haven't searched the whole focus range, 
	// then move the motor to next step. 
	if (g_iStepCount<AF_MAX_STEP_COUNT)
	{ 
		// if the focus value is larger than the max focus value,
		// update the max focus value & position
		if (iFocusValue>g_MaxFocusValue)
		{
			g_MaxFocusValue=iFocusValue;
			g_iFocusedStep=g_iStepCount;
		}

		RunMotorReserve(1); // to move the motor by one step

		SCI_Sleep(20); //wait for the motor to be still stable

		// set 6800 af enable
		ISP_StartAF();
	}
	else // we have searched the whole range, move the lens to the max FV value point (focused point)
	{
		if (	iFocusValue>g_MaxFocusValue*9/10  	// if current focus value is close to the maximum focus value
			|| g_iIncreaseStepCount>=AF_INCREASE_THRESHOLD // OR the increase slope step count indicates reaching to the peak
			|| g_iStepCount>=AF_MAX_STEP_COUNT+g_iFocusedStep) // OR reached the max FV point
		{
			// focused
			ISP_SetAFFocusState(1); // set af focus flag to true
			
			// reset static variables
			g_iStepCount=0;
			g_FV1=0; 
			g_FV2=0;
			g_FV3=0;
			g_MaxFocusValue=0;
			g_iFocusedStep=0;
			g_iIncreaseStepCount=0;
			
			//turn motor off
			CloseMotor(); //to close the motor
		}
		else // have not reached the focus point, keep moving
		{
			RunMotorReserve(1); // to move the motor by one step

			SCI_Sleep(20); //wait for the motor to be still stable

			// set 6800 af enable
			ISP_StartAF();
		}
	}
	
	return 0;
}
/*******************************************************************************/
// Description: Continuous mode auto focus searching function
// Global resource dependence:
// Author: Xiaoguang.Feng
// Note:
//		input: 
//			iCurrentFV - the focus value of current frame
//		output:
//			none
//		return
//			0
/********************************************************************************/	
uint32 ContinuousAutoFocus(uint32 iCurrentFV) 
{
	// static variables
	static uint8 g_iStepCount=0; // count of the searching steps
	static uint32 g_FV1=0; // last-last frame's focus value
	static uint32 g_FV2=0; // last frame's focus value
	static uint32 g_FV3=0; // current frame's focus value
	static uint32 g_MaxFocusValue=0; // maximum focus value
	static int32 g_iFocusedStep=0; // the index of steps corresponding to the maximum focus value
	static uint8 g_iIncreaseStepCount=0; // number of increasing slope steps in the FV curve
	
	// local variables
	uint32 iFocusValue=0;
	uint8  bFocused; // flag of whether the lens is in focus now
	//int i=0;

	// calculate the average focus value of the latest 3 frames, 
	// to prevent random noise affect
	iFocusValue=Average_FV(&g_FV1, &g_FV2, &g_FV3, iCurrentFV);

	// get current focus status
	bFocused=ISP_GetAFFocusState(); // get current af focus flag

	if (bFocused==1) // already in focus now
	{
		if (abs(g_FV3-g_FV2)<g_FV2/5 // if current FV value is close to last frame's FV value 
			&& iFocusValue>g_MaxFocusValue/2) // AND current FV value is NOT too less than the MAX focus value
		{
			// still in focus, no need to make any change
			SCI_Sleep(100);
			// set 6800 af enable
			ISP_StartAF();
		}
		else // not in focus anymore, reset focus flag to false and start AF searching approach
		{
			// reset af focus flag to false
			ISP_SetAFFocusState(0);

			// reset static variables
			g_iStepCount=0;
			g_FV1=0; 
			g_FV2=0;
			g_FV3=0;
			g_MaxFocusValue=0;
			g_iFocusedStep=0;
			g_iIncreaseStepCount=0;			
			
			// set 6800 af enable
			ISP_StartAF();
		}
	}
	else // not in focus now
	{
		// number of searched steps increases
		g_iStepCount++;
			
		if (g_iStepCount==1) //first run, turn motor on
		{
			OpenMotor();
		}
					
		// the increase_step_count is used to estimate the FV curve peak	
		if (g_FV2<g_FV3)
		{
			g_iIncreaseStepCount++;
		}
		else
		{
			g_iIncreaseStepCount=0;
		}
			
		// if we haven't searched the whole focus range, 
		// then move the motor to next step. 
		if (g_iStepCount<AF_MAX_STEP_COUNT)
		{ 
			// if the focus value is larger than the max focus value,
			// update the max focus value & position
			if (iFocusValue>g_MaxFocusValue)
			{
				g_MaxFocusValue=iFocusValue;
				g_iFocusedStep=g_iStepCount;
			}

			RunMotorReserve(1); // to move the motor by one step

			SCI_Sleep(20); //wait for the motor to be still stable

			// set 6800 af enable
			ISP_StartAF();
		}
		else // we have searched the whole range, move the lens to the max FV value point (focused point)
		{
			if (	iFocusValue>g_MaxFocusValue*9/10  	// if current focus value is close to the maximum focus value
				|| g_iIncreaseStepCount>=AF_INCREASE_THRESHOLD // OR the increase slope step count indicates reaching to the peak
				|| g_iStepCount>=AF_MAX_STEP_COUNT+g_iFocusedStep) // OR reached the max FV point
			{
				// focused
				ISP_SetAFFocusState(1); // set af focus flag to true
				
				// close motor
				CloseMotor();

				// set 6800 af enable
				ISP_StartAF();
			}
			else // have not reached the focus point, keep moving
			{
				RunMotorReserve(1); // to move the motor by one step

				SCI_Sleep(20); //wait for the motor to be still stable

				// set 6800 af enable
				ISP_StartAF();
			}
		}
	
	}
  
	return 0;
}
/*******************************************************************************/
// Description: Manual adjust the motor to move
// Global resource dependence:
// Author: Jianping.Wang
// Note:
//		input: 
//			step_number - move step number
//		output:
//			none
//		return
//			0
/********************************************************************************/	
LOCAL void ManualAdjustMotor(uint8 step_number)
{
	SCI_ASSERT( (step_number <= 20) && (step_number > 0) );/*assert verified*/
	if(s_is_opend == MOTOR_IS_CLOSED)
	{
		OpenMotor();
	}
	
	RunMotorReserve(step_number);
	
	CloseMotor();
}
/*******************************************************************************/
// Description: FSM0611 operations
/********************************************************************************/	
AF_OPERATIONS_T FSM0611_Operations = 
{
	ContinuousAutoFocus,
	SingleModeAutoFocus,
	ManualAdjustMotor,
	PNULL,
	PNULL,
	PNULL,
	PNULL,
};



/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                             					        *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


//end of af.c

