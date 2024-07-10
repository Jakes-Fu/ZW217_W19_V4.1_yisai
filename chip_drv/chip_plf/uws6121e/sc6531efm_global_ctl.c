/******************************************************************************
 ** File Name:      sc6531efm_global_ctl.c                                        *
 ** Author:         Yong.Li                                                  *
 ** DATE:           12/22/2011                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic function for ldo management.  *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/22/2011     Yong.Li          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"

#include "global_ctl_drvapi.h"



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Local variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Declaration                              *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:  Global ctl port select
//  Global resource dependence:
//  Author: Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GlB_CTL_PORT_SEL (GLB_MODULE_E module, uint32 port_num)
{
	BOOLEAN ret = SCI_TRUE;
	uint32 tmp = 0;
	
	tmp = CHIP_REG_GET(APB_PIN_CTL0);
	
	switch(module)
	{
		case SIM0:		
			SCI_ASSERT(port_num < MAX_SIM0_PORT); /*assert to do*/			
			
			// select sim port
			tmp &= ~SIM_SEL_MSK;
			tmp |= port_num << SIM_SEL_SHIFT;				
		break;

		case UART0:
			SCI_ASSERT(port_num < MAX_UART0_PORT); /*assert to do*/		
			
			// select uart0 port
			/*
			UART0_SEL=0:TXD=U0TXD,RXD=U0RXD
			UART0_SEL=1:TXD=SPI0_CLK,RXD=SPI0_CS0
			*/
			tmp &= ~UART0_SEL_MSK;
			tmp |= port_num << UART0_SEL_SHIFT;			
		break;		

		case UART1:
			SCI_ASSERT(port_num < MAX_UART1_PORT); /*assert to do*/		
			
			// select uart1 port
			/*
			UART1_SEL=0-1:TXD=U0RTS,RXD=U0CTS
			UART1_SEL=2-3:TXD=KEYIN3,RXD=KEYIN4
			UART1_SEL=4-5:TXD=SPI0_DO,RXD=SPI0_DOI
			*/
			tmp &= ~UART1_SEL_MSK;
			tmp |= (port_num<<1) << UART1_SEL_SHIFT;			
		break;
		
		case I2C1:
			SCI_ASSERT(port_num < MAX_I2C0_PORT); /*assert to do*/			
			
			// select i2c port
			tmp &= ~I2C1_SEL_MSK;
			tmp |= port_num << I2C1_SEL_SHIFT;			
		break;
		
		default:
			SCI_ASSERT(0); /*assert to do*/
		break;
	}
	
	CHIP_REG_SET(APB_PIN_CTL0, tmp);

	return ret;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

