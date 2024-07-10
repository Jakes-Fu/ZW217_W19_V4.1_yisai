/***************************************************************************************
** File Name:      af_cfg.c                                        		    	       *
** DATE:           11/07/2006                                                		   *
** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         		   *
** Description:    this file defines AF configure interfaces					       *
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
#include "ms_customize_trc.h"
#include "isp_service.h"

/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                               		       *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**------------------------------------------------------------------------------------*
 **                         Data Type           			                           *
 **------------------------------------------------------------------------------------*/
typedef struct af_cofiguration_tag
{ 
	uint32  is_used;
	AF_OPERATIONS_T *af_operations;
}AF_CONFIG_TABLE_T;


LOCAL AF_OPERATIONS_T s_af_operations;

#ifdef PRODUCT_DRIVER_SC6800_DVB
extern AF_OPERATIONS_T FSM0611_Operations;
extern AF_OPERATIONS_T nfmw_Operations;
#endif

LOCAL uint32 af_is_exist_flag = 0;

LOCAL AF_CONFIG_TABLE_T s_cfg_table[] = {
#ifdef PRODUCT_DRIVER_SC6800_DVB										 
										{0, (AF_OPERATIONS_T*)&FSM0611_Operations} , 
										 
										 {1, (AF_OPERATIONS_T*)&nfmw_Operations} , 	
#endif
										 {0xffffffff, PNULL}
									    };
	

									  
/*******************************************************************************/
// Description: Identify AF 
// Global resource dependence:
// Author: Xiaoguang.Feng
// Note:
//		input: 
//			none
//		output:
//			none
//		return
//			none
/********************************************************************************/										  
LOCAL uint32 AutoIdentify(void)
{
	uint32 i = 0;
	uint32 af_is_being = 0;

	
	//The more conventional form of infinite loop prefix is for(;;)  --PCLINT
	//while(1)
	for(;;)
	{
		if(s_cfg_table[i].is_used != 0xffffffff)
		{
			if(s_cfg_table[i].is_used == 1)
			{
				s_af_operations = *s_cfg_table[i].af_operations;
				af_is_being = 1;
				break;
			}
			i++;
		}
		else
		{
			break;
		}
	}
	af_is_exist_flag = af_is_being;
	
	//SCI_TRACE_LOW:"AF Is beding:%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,AF_CFG_92_112_2_18_0_29_0_576,(uint8*)"d", af_is_being);
	
	return af_is_being;
	
}									  
/*******************************************************************************/
// Description: Get AF Operations that is identied 
// Global resource dependence:
// Author: Xiaoguang.Feng
// Note:
//		input: 
//			operations_ptr - pointer to AF operations
//		output:
//			none
//		return
//			none
/********************************************************************************/	
AF_OPERATIONS_T *GetAFOperations (void)
{	
	return  &s_af_operations;
}
/*******************************************************************************/
// Description: AF Continuous frame mode
// Global resource dependence:
// Author: Jianping.Wang
// Note:
//		input: 
//			af_value - ISP AF value
//		output:
//			none
//		return
//			0 - success
/********************************************************************************/	
PUBLIC uint32 AF_SingleModeAutoFocus (uint32 af_value)
{
	if(af_is_exist_flag == 1)
	{
		SCI_ASSERT(GetAFOperations()->af_SingleModeAutoFocus != PNULL);/*assert verified*/
		return (GetAFOperations()->af_SingleModeAutoFocus(af_value));
	}
	return 0;
}
/*******************************************************************************/
// Description: AF Single frame mode
// Global resource dependence:
// Author: Jianping.Wang
// Note:
//		input: 
//			af_value - ISP AF value
//		output:
//			none
//		return
//			0 - success
/********************************************************************************/	
PUBLIC uint32 AF_ContinuousAutoFocus (uint32 af_value)
{
	if(af_is_exist_flag == 1)
	{
		SCI_ASSERT(GetAFOperations()->af_ContinuousAutoFocus != PNULL);/*assert verified*/
		return (GetAFOperations()->af_ContinuousAutoFocus(af_value));
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




PUBLIC void AF_ManualAdjustMotor(uint8 step_number)
{
	if(af_is_exist_flag == 1)
	{
		SCI_ASSERT(GetAFOperations()->af_ManualAdjustMotor != PNULL);/*assert verified*/
		GetAFOperations()->af_ManualAdjustMotor(step_number);
	}
}


/*******************************************************************************/
// Description: Manual adjust the motor to move
// Global resource dependence:
// Author: Hansen.Sun
// Note:
//		input: 
//			
//		output:
//			none
//		return
//			0
/********************************************************************************/

PUBLIC void AF_Open(void)
{
	if(af_is_exist_flag == 1)
	{
		SCI_ASSERT(GetAFOperations()->af_Open != PNULL);/*assert verified*/
		GetAFOperations()->af_Open();	
	}
}

PUBLIC void AF_Close(void)
{
	if(af_is_exist_flag == 1)
	{
		SCI_ASSERT(GetAFOperations()->af_Close != PNULL);/*assert verified*/
		GetAFOperations()->af_Close();	
	}
}

PUBLIC void AF_Preview(void)
{
	if(af_is_exist_flag == 1)
	{
		SCI_ASSERT(GetAFOperations()->af_PreviewAutoFocus != PNULL);/*assert verified*/
		GetAFOperations()->af_PreviewAutoFocus();	
		//SCI_TRACE_LOW:"Set Preview auto focus state"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,AF_CFG_209_112_2_18_0_29_0_577,(uint8*)"");
	}
}

PUBLIC void AF_Capture(void)
{
	if(af_is_exist_flag == 1)
	{
		SCI_ASSERT(GetAFOperations()->af_CaptureAutoFocus != PNULL);/*assert verified*/
		GetAFOperations()->af_CaptureAutoFocus();	
	}
}
/*AF mode :*/
PUBLIC void AF_Set_Mode(ISP_AF_MODE af_mode_index)
{
	if(af_is_exist_flag == 1)
	{
//		SCI_ASSERT(GetAFOperations()->af_set_mode != PNULL);
//		GetAFOperations()->af_set_mode ( (uint32)af_mode_index );	
	}
}


/*******************************************************************************/
// Description: AF module initialize
// Global resource dependence:
// Author: Jianping.Wang
// Note:
//		input: 
//			none
//		output:
//			0: NO AF suport;
//                 1: AF Support 
//		return
//			none
/********************************************************************************/	
PUBLIC uint32 AF_Init(void)
{
	return (AutoIdentify());
}

/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                             					        *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


//end of af_cfg.c


