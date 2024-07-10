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
#include "ms_customize_trc.h"
#include "isp_service.h"
#include "isp_drv_sc8800h.h"

#include "gpio_prod_api.h"

#include "pwm_drv.h"


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




LOCAL uint32 s_is_opend = 0;


LOCAL uint32 FocusValue[102] = {0};
LOCAL int16 ReferenceStep[102] = 
{
	//0, 40, 55,63,75,85,97,110,123,135,147,160,172,180,197,210,220,230,240,255

	0, 55, 63, 96,123,147,172,197,230,255
	//0,15,21,24,29,33,38,43,48,52,57,62,67,70,77,82,86,90,94,100

};
PUBLIC int16  Focusindex = 0;
PUBLIC int16  MaxFocusIndex = 0;

PUBLIC uint32 MAXFocusValue = 0;
LOCAL int16 FineFocusindex = 0;
LOCAL int16 FineMaxFocusindex = 0;
LOCAL ISP_AF_MODE AF_mode_index;
LOCAL int16 skipFrameflag = 1;


#define AFVCMSTEP 10


/********************************************************************************/	
// Description:Set VCM_GoPosition(uint8 position) 
// Global resource dependence:
// Author: Hansen.sun
// Note:
//		input: 
//			position : 0 ~ 100
//		output:
//			none
//		return
//			none
/********************************************************************************/	
LOCAL void VCM_GoPosition(uint8 position)
{
	uint8 lpos = position;
	if(position > 255)
	{
		lpos = 255;
	}
		
	*(volatile uint32*)0x8C00004c |=0x0800;  // backup to original point
	*(volatile uint32*)0x8C00004c &=0xffffbff;
	lops = ( lops / 255)  * 100;
	PWM_Config(3,  100000,  lpos);	//PWM_Config( int pwm_num,  uint32  freq,  uint16  duty_cycle );
	
	SCI_Sleep(3);    //fos stable
}
/********************************************************************************/	
// Description:Open the AutoFocus 
// Global resource dependence:
// Author: Hansen.sun
// Note:
//		input: 
//			none
//		output:
//			none
//		return
//			none
/********************************************************************************/	
LOCAL void VCM_OpenAutoFocus(void)
{	
	Focusindex = 0;
      MaxFocusIndex = 0;
       MAXFocusValue = 0;
	VCM_GoPosition(255);
	GPIO_SetVCMEnable (TRUE);

}
/*******************************************************************************/
// Description:Close autofocus
// Global resource dependence:
// Author: Hansen.sun
// Note:
//		input: 
//			none
//		output:
//			none
//		return
//			none
/********************************************************************************/	
LOCAL void VCM_CloseAutoFocus(void)
{
	
	Focusindex = 0;
      MaxFocusIndex = 0;
       MAXFocusValue = 0;
	VCM_GoPosition(255);
       GPIO_SetVCMEnable (FALSE);
}

/*******************************************************************************/
// Description:Set Preview Autofocus
// Global resource dependence:
// Author: Hansen.sun
// Note:
//		input: 
//			none
//		output:
//			none
//		return
//			none
/********************************************************************************/

LOCAL void VCM_PreviewAutoFocus(void)
{
	
	Focusindex = 0;
      MaxFocusIndex = 0;
       MAXFocusValue = 0;
	VCM_GoPosition(255);
	 GPIO_SetVCMEnable (FALSE);
}

/*******************************************************************************/
// Description:Set Capture Autofocus
// Global resource dependence:
// Author: Hansen.sun
// Note:
//		input: 
//			none
//		output:
//			none
//		return
//			none
/********************************************************************************/

LOCAL void VCM_CaptureAutoFocus(void)
{
	
	Focusindex = 0;
      MaxFocusIndex = 0;
       MAXFocusValue = 0;
	VCM_GoPosition(255);
}

LOCAL void VCM_SETMODE(uint32 af_set_mode)
{
	ISP_AF_MODE mode = (ISP_AF_MODE)af_set_mode;

	AF_mode_index = mode;
	
	switch (mode)
	{
		ISP_AF_AUTO :

			VCM_CloseAutoFocus();
			
	       break;
		ISP_AF_MICRO :

			VCM_GoPosition(0);
		break;

	       default:

			VCM_CloseAutoFocus();
	       break;
	}
	
}


/*******************************************************************************/
// Description: Single mode auto focus searching function
// Global resource dependence:
// Author: Hansen Sun
// Note:
//		input: 
//			iCurrentFV - the focus value of current frame
//		output:
//			none
//		return
//			0
/********************************************************************************/	
LOCAL uint32 VCM_SingleModeAutoFocus(uint32 iCurrentFV)
{

	// local variables
	uint32 iFocusValue=0;
	uint32 Focusstate = 0;
	
	uint16 VCMStep = 0;
	

	//SCI_TRACE_LOW:"AF: index  = %d,  value = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,AF_NFMW_MICRON_3M_216_112_2_18_0_29_2_578,(uint8*)"dd" , Focusindex, iCurrentFV);
	
	//GPIO_SetVCMEnable (TRUE);

	if(Focusindex == 200)
	{
              return 0;
	}

	
	if(Focusindex == 0)  //skip the first frame af value
	{
		FineFocusindex = 0;
		FineMaxFocusindex = 0;
		//VCMStep = 255; 
		VCMStep = (uint16)(255 -ReferenceStep[0] );
		Focusindex ++;
		skipFrameflag = 1;
	}
	else if(Focusindex < (AFVCMSTEP ))  //coarse tune
	{
	

	      if(  skipFrameflag == 1)
	      	{
	      		skipFrameflag = 0;
			ISP_StartAF();
	      		return 0;
	      	}else{

		 	skipFrameflag = 1;
	 
			FocusValue[Focusindex-1] = iCurrentFV;
			if(iCurrentFV > MAXFocusValue )
			{
				MAXFocusValue = iCurrentFV;
				MaxFocusIndex = Focusindex - 1;
			
			}

			VCMStep = (uint16)(255 -ReferenceStep[Focusindex] );
			//SCI_TRACE_LOW:"AF: index_1  = %d MaxFoucus Index ' %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,AF_NFMW_MICRON_3M_257_112_2_18_0_29_2_579,(uint8*)"dd" , Focusindex ,  MaxFocusIndex  );
		
			Focusindex ++;

			//SCI_TRACE_LOW:"AF: index  = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,AF_NFMW_MICRON_3M_261_112_2_18_0_29_2_580,(uint8*)"d" , Focusindex);

		}
			
	}  
	
	else if (Focusindex  <150)  //coarse tune final
	{

		FocusValue[Focusindex-1] = iCurrentFV;
		if(iCurrentFV > MAXFocusValue )
		{
			MAXFocusValue = iCurrentFV;
			MaxFocusIndex = Focusindex - 1;
			
		}

			
		Focusindex =150;  //
		
		

		if(MaxFocusIndex == AFVCMSTEP-1)                                                                        
		{                                                                                                     
                                                                                                                      
			FineFocusindex = -14;                                                                             

		}                                                               
		else if(MaxFocusIndex == 0)        //focus OK                                                                  
		{                                                                                                     
                                                                                                                      
			Focusstate = 1;   //infinity , will do not fine tune      
			ISP_SetAFFocusState(1); // set af focus flag to true   
			//VCMStep = (uint16)(100 -ReferenceStep[MaxFocusIndex -1] );    
			FineFocusindex = 0;
			Focusindex =200;  //         	

		}
		else
		{
			FineFocusindex = -7;
		}
		
		VCM_GoPosition(255);	

		VCMStep = (uint16)(255 -ReferenceStep[MaxFocusIndex] - FineFocusindex);									   
			
	}
	else if(Focusindex < 180)    //fine tune 
	{
		                                                                         
	      if(  skipFrameflag == 1)
	      	{
	      		skipFrameflag = 0;
			ISP_StartAF();
	      		return 0;          
		}
		else{
		 	

		  	skipFrameflag = 1;
		//VCM_GoPosition(100);	                                                                              
		//VCMStep = (100- (( MaxFocusIndex-1) * AFVCMSTEPMUL) );//;* 5                                        
			if(iCurrentFV > MAXFocusValue)                                                                        
			{                                                                                                     
			  FineMaxFocusindex = FineFocusindex;       
			 MAXFocusValue = iCurrentFV ;
			}                                                                                                     
	                                 
	                                                                   
		       FineFocusindex++;                                                                                                            
			                                                                                                    
			VCMStep = (uint16)(255 -ReferenceStep[MaxFocusIndex] - FineFocusindex );                                            
	                                                                                                                        

			//SCI_TRACE_LOW:"AF SDY: check--  stable: MaxFocusIndex: %d ; FineFocusindex = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,AF_NFMW_MICRON_3M_336_112_2_18_0_29_2_581,(uint8*)"dd" , MaxFocusIndex, FineFocusindex);

			if( (MaxFocusIndex == (AFVCMSTEP-1) && 0 == FineFocusindex) ||
			    (MaxFocusIndex!= (AFVCMSTEP-1) && 7 == FineFocusindex) )     //focus OK                                                                  
			{                                                                                                     
				
				//ISP_SetAFFocusState(1); // set af focus flag to true         
				Focusindex =180;  //         																												 
				//Focusstate = 1;                                                                  

			}     
		}
		                                                              
                                                                                  
	}
	else  // fine tune final
	{

		 if(  skipFrameflag == 1)
	      	{
	      		skipFrameflag = 0;
			ISP_StartAF();
	      		return 0;          
		}
		                               
		if(iCurrentFV > MAXFocusValue)                                                                        
		{                                                                                                     
			  FineMaxFocusindex = FineFocusindex;                                                         
		}                                                                                                     
	                                 
                                                                                                
		//VCMStep = (uint16)(255-ReferenceStep[MaxFocusIndex + FineMaxFocusindex-1] );                                            
		VCMStep = (uint16)(255 -ReferenceStep[MaxFocusIndex] - FineMaxFocusindex  );        


		VCM_GoPosition(255);	
		ISP_SetAFFocusState(1); // set af focus flag to true         
		Focusindex =200;  //         																												 
		Focusstate = 1;   //infinity , will do not fine tune                                                                        

	  	//SCI_TRACE_LOW:"AF SDY: check--  stable: MaxFocusIndex: %d ; FineMaxFocusindex = %d"
	  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,AF_NFMW_MICRON_3M_376_112_2_18_0_29_2_582,(uint8*)"dd" , MaxFocusIndex, FineMaxFocusindex);
	}	
	



	
	VCM_GoPosition(VCMStep);



	ISP_StartAF();

	//SCI_TRACE_LOW:"Focusstate = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,AF_NFMW_MICRON_3M_389_112_2_18_0_29_2_583,(uint8*)"d" , Focusstate);
	return Focusstate;
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
LOCAL uint32 VCM_ContinuousAutoFocus(uint32 iCurrentFV) 
{
  
	return 0;
}

/*******************************************************************************/
// Description: FSM0611 operations
/********************************************************************************/	
AF_OPERATIONS_T nfmw_Operations = 
{
	VCM_ContinuousAutoFocus,
	VCM_SingleModeAutoFocus,
	PNULL,
	VCM_OpenAutoFocus,
	VCM_CloseAutoFocus,
	VCM_PreviewAutoFocus,
	VCM_CaptureAutoFocus,
	//VCM_SETMODE,
	
};



/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                             					        *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


//end of af.c

