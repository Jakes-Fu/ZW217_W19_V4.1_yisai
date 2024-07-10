/******************************************************************************
 ** File Name:      demod_config.c                                                        *
 ** Author:                                                                                          *
 ** DATE:            31/12/2010                                                               *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    demod config                                               *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 31/12/2010     	                Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "demod_config.h"

#ifdef DEMOD_HW_INNOFIDEI
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#endif

#ifdef CMMB_WIFI_SHARE_SPI_SUPPORT
#include "gpio_prod_api.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
 


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//! \fn uint8 Demod_GetSPIConfig(SPI_CONFIG_T *spicfg_ptr)
//! \param void
//! \return 
//! \brief  Description: 
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC uint8 Demod_GetSPIConfig(SPI_DEV *spicfg_ptr)
{
	spicfg_ptr->id = 1; /* logic ID */
	spicfg_ptr->mode = 0; /* No sck reverse(Bit13:0),  Enable NG_Tx(Bit1:1),Disable NG_Rx(Bit0:0) */
	spicfg_ptr->tx_bit_length = 8;

#ifdef DEMOD_HW_SIANO
	spicfg_ptr->freq = 10000000;
#endif

#ifdef DEMOD_HW_INNOFIDEI
	spicfg_ptr->freq = 8000000;
#endif

    return 0;
}


/*****************************************************************************/
//! \fn uint8 Demod_ClkOpen(void)
//! \param void
//! \return 
//  0 -> 26M Clock
//  1 -> 12M Clock
//  2 -> 20M Clock
//! \brief  Description: 
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC uint8 Demod_ClkOpen(void)
{
#if defined(DEMOD_HW_SIANO)

    /* Currently siano demod use 26M clock */
	return 0;

#elif defined(DEMOD_HW_INNOFIDEI)

	DPSLP_XTLOpenInSleep(TRUE);

    return 0;

#else

	return 0;

#endif
}

/*****************************************************************************/
//! \fn uint8 Demod_ClkClose(void)
//! \param void
//! \return 
//! \brief  Description: 
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC uint8 Demod_ClkClose(void)
{
#ifdef DEMOD_HW_SIANO
#endif

#ifdef DEMOD_HW_INNOFIDEI
	DPSLP_XTLOpenInSleep(FALSE);
#endif

    
    return 0;
}

/*****************************************************************************/
//! \fn void Demod_SetSPICallback(void)
//! \param spi_bus_logic_id
//! \return 
//! \brief  Description: 
//! \author Author: Chi.Chen
//! \note   Note:  
/*****************************************************************************/
PUBLIC void Demod_SetSPICallback(uint32 logic_id)
{
#ifdef CMMB_WIFI_SHARE_SPI_SUPPORT
    SPI_HAL_Ioctl(logic_id, SPI_CTL_S_CS_CALLBACK, (uint32 *)GPIO_SPISwitch);/*lint !e611 */
#endif
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif

